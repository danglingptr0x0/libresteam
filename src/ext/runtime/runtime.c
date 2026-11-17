#include <stdint.h>
#include <windows.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/time/time.h>

#include <libresteam/backends/il2cpp/il2cpp.h>
#include <libresteam/backends/java/java.h>
#include <libresteam/backends/lua/lua.h>
#include <libresteam/ext/runtime/runtime.h>
#include <libresteam/mod/mod.h>

#define RUNTIME_PROPERTY "LibreSteam.Runtime.Context.v1"
#define RUNTIME_MESSAGE "LibreSteam.Runtime.Dispatch.v1"
#define RUNTIME_MUTEX_FMT "Local\\LibreSteam-runtime-%u32"
#define RUNTIME_TICK_MS 10U
#define RUNTIME_POLL_MS 0U
#define RUNTIME_MSG_PARAM_UNUSED 0U
#define RUNTIME_STOP_LOG_FMT "runtime stopped; res: %u32; DLLs resident: 1"
#define RUNTIME_IL2CPP_RETRY_NS (2U * LDG_NS_PER_SEC)
#define RUNTIME_ASCII_MAX 0x7fU

typedef struct runtime_ctx
{
    libresteam_mod_host_t mods;
    libresteam_backends_il2cpp_t il2cpp;
    libresteam_mod_api_t seed;
    libresteam_backends_lua_api_t lua_api;
    libresteam_backends_lua_t lua;
    libresteam_backends_java_api_t java_api;
    libresteam_backends_java_t java;
    HWND window;
    HMODULE module;
    HHOOK hook;
    HANDLE ack;
    HANDLE stop;
    HANDLE singleton;
    char path[LDG_IO_PATH_MAX];
    char root[LDG_IO_PATH_MAX];
    char name[LIBRESTEAM_EXT_RUNTIME_NAME_CAP];
    uint64_t next_init_ns;
    uint32_t thread_id;
    uint32_t message;
    uint32_t res;
    uint32_t last_res;
    uint8_t initialized;
    uint8_t stopped;
    uint8_t busy;
    uint8_t published;
    uint8_t pudding[4];
} runtime_ctx_t;

static uint32_t runtime_il2cpp_tick(runtime_ctx_t *ctx)
{
    uint64_t now = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->il2cpp.ready || ctx->il2cpp.failed) { return LDG_ERR_AOK; }

    res = ldg_time_monotonic_get(&now);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (now < ctx->next_init_ns) { return LDG_ERR_AOK; }

    if (LDG_UNLIKELY(now > UINT64_MAX - RUNTIME_IL2CPP_RETRY_NS)) { return LDG_ERR_OVERFLOW; }

    ctx->next_init_ns = now + RUNTIME_IL2CPP_RETRY_NS;
    res = libresteam_backends_il2cpp_init(&ctx->il2cpp);
    return res == LDG_ERR_NOT_INIT || ctx->il2cpp.failed ? LDG_ERR_AOK : res;
}

static uint32_t runtime_backends_init(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backends_lua_init(&ctx->lua, &ctx->lua_api);
    if (res == LDG_ERR_AOK) { res = libresteam_backends_java_init(&ctx->java, &ctx->java_api); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->seed.il2cpp = &ctx->il2cpp;
    ctx->seed.class_find = libresteam_backends_il2cpp_class_find;
    ctx->seed.method_find = libresteam_backends_il2cpp_method_find;
    ctx->seed.invoke = libresteam_backends_il2cpp_invoke;
    ctx->seed.field_class_find = libresteam_backends_il2cpp_field_class_find;
    ctx->seed.field_kind_find = libresteam_backends_il2cpp_field_kind_find;
    ctx->seed.objs_find = libresteam_backends_il2cpp_objs_find;
    ctx->seed.str_ascii_get = libresteam_backends_il2cpp_str_ascii_get;
    ctx->seed.obj_name_get = libresteam_backends_il2cpp_obj_name_get;
    ctx->seed.pos_get = libresteam_backends_il2cpp_pos_get;
    ctx->seed.enum_name_get = libresteam_backends_il2cpp_enum_name_get;
    ctx->seed.objs_hold = libresteam_backends_il2cpp_objs_hold;
    ctx->seed.obj_alive_is = libresteam_backends_il2cpp_obj_alive_is;
    ctx->seed.lua = &ctx->lua_api;
    ctx->seed.lua_api_size = sizeof(ctx->lua_api);
    ctx->seed.java = &ctx->java_api;
    ctx->seed.java_api_size = sizeof(ctx->java_api);
    return LDG_ERR_AOK;
}

static uint32_t runtime_mods_unld(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;
    uint32_t backend_res = LDG_ERR_AOK;

    if (!ctx->mods.ld_done) { return LDG_ERR_AOK; }

    res = libresteam_mod_unld(&ctx->mods);
    backend_res = libresteam_backends_lua_shutdown(&ctx->lua);
    if (res == LDG_ERR_AOK) { res = backend_res; }

    backend_res = libresteam_backends_java_shutdown(&ctx->java);
    if (res == LDG_ERR_AOK) { res = backend_res; }

    return res;
}

static uint32_t runtime_dispatch(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res = (uint32_t)WaitForSingleObject(ctx->stop, RUNTIME_POLL_MS);
    if (res == WAIT_FAILED) { return LDG_ERR_INVALID; }

    if (res == WAIT_OBJECT_0)
    {
        res = runtime_mods_unld(ctx);

        ctx->stopped = LDG_TRUTH_TRUE;
        return res;
    }

    if (!ctx->initialized)
    {
        res = runtime_backends_init(ctx);
        if (res == LDG_ERR_AOK) { res = libresteam_mod_dir_ld(&ctx->mods, &ctx->seed, ctx->root); }

        if (res == LDG_ERR_AOK) { res = ldg_logging_line_wr(ctx->mods.log_path, LDG_LOGGING_LVL_INFO, "runtime ready; dispatch: window message; thread: %u32; window: 0x%x64; root: %s", ctx->thread_id, (uint64_t)(uintptr_t)ctx->window, ctx->root); }

        if (res == LDG_ERR_AOK) { res = ldg_mem_lock(); }

        ctx->initialized = LDG_TRUTH_TRUE;
        ctx->res = res;
    }
    else
    {
        res = runtime_il2cpp_tick(ctx);
        if (res == LDG_ERR_AOK) { res = libresteam_mod_tick(&ctx->mods); }
    }

    if (res != ctx->last_res)
    {
        ctx->last_res = res;
        if (ldg_logging_line_wr(ctx->mods.log_path, LDG_LOGGING_LVL_INFO, "runtime tick; res: %u32", res) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }
    }

    return res;
}

static LRESULT CALLBACK runtime_msg_cb(int code, WPARAM removal, LPARAM param)
{
    MSG *msg = 0x0;
    runtime_ctx_t *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (code != HC_ACTION || removal != PM_REMOVE || !param) { return CallNextHookEx(0x0, code, removal, param); }

    msg = (MSG *)param;
    ctx = (runtime_ctx_t *)GetPropA(msg->hwnd, RUNTIME_PROPERTY);
    if (!ctx || msg->message != ctx->message || ctx->window != msg->hwnd || ctx->thread_id != (uint32_t)GetCurrentThreadId()) { return CallNextHookEx(0x0, code, removal, param); }

    msg->message = WM_NULL;
    msg->wParam = 0;
    msg->lParam = 0;
    if (ctx->busy) { return CallNextHookEx(0x0, code, removal, param); }

    ctx->busy = LDG_TRUTH_TRUE;
    res = runtime_dispatch(ctx);
    if (res != LDG_ERR_AOK && !ctx->initialized) { ctx->res = res; }

    if (WaitForSingleObject(ctx->stop, 0) == WAIT_OBJECT_0 && !ctx->stopped)
    {
        res = runtime_mods_unld(ctx);

        ctx->stopped = LDG_TRUTH_TRUE;
        if (res != LDG_ERR_AOK) { ctx->res = res; }
    }

    ctx->busy = LDG_TRUTH_FALSE;
    if (!SetEvent(ctx->ack)) { ctx->res = LDG_ERR_INVALID; }

    return CallNextHookEx(0x0, code, removal, param);
}

static uint32_t runtime_hook_init(runtime_ctx_t *ctx)
{
    DWORD pid = 0x0;
    HOOKPROC callback = runtime_msg_cb;
    uintptr_t addr = 0x0;
    uint64_t len = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t path_len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    ctx->thread_id = (uint32_t)GetWindowThreadProcessId(ctx->window, &pid);
    if (!ctx->thread_id || pid != GetCurrentProcessId()) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = ldg_mem_secure_cpy(&addr, &callback, sizeof(addr));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_PIN, (LPCSTR)addr, &ctx->module)) { return LDG_ERR_NOT_FOUND; }

    path_len = (uint32_t)GetModuleFileNameA(ctx->module, ctx->path, (DWORD)sizeof(ctx->path));
    if (!path_len || path_len >= sizeof(ctx->path)) { return LDG_ERR_STR_TRUNC; }

    for (idx = 0; idx < path_len; idx++) { if ((uint8_t)ctx->path[idx] > RUNTIME_ASCII_MAX) { return LDG_ERR_STR_INVALID; } }
    res = ldg_io_path_parent_get(ctx->path, ctx->root, sizeof(ctx->root));
    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->name, sizeof(ctx->name), &len, RUNTIME_MUTEX_FMT, (uint32_t)pid); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->singleton = CreateMutexA(0x0, FALSE, ctx->name);
    if (!ctx->singleton) { return LDG_ERR_DENIED; }

    if (GetLastError() == ERROR_ALREADY_EXISTS) { return LDG_ERR_EXISTS; }

    res = ldg_str_fmt(ctx->name, sizeof(ctx->name), &len, LIBRESTEAM_EXT_RUNTIME_STOP_FMT, (uint32_t)pid);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->stop = CreateEventA(0x0, TRUE, FALSE, ctx->name);
    if (!ctx->stop) { return LDG_ERR_DENIED; }

    if (GetLastError() == ERROR_ALREADY_EXISTS) { return LDG_ERR_EXISTS; }

    ctx->ack = CreateEventA(0x0, FALSE, FALSE, 0x0);
    if (!ctx->ack) { return LDG_ERR_DENIED; }

    ctx->message = (uint32_t)RegisterWindowMessageA(RUNTIME_MESSAGE);
    if (!ctx->message) { return LDG_ERR_INVALID; }

    if (GetPropA(ctx->window, RUNTIME_PROPERTY)) { return LDG_ERR_EXISTS; }

    if (!SetPropA(ctx->window, RUNTIME_PROPERTY, (HANDLE)ctx)) { return LDG_ERR_DENIED; }

    ctx->published = LDG_TRUTH_TRUE;
    ctx->hook = SetWindowsHookExA(WH_GETMESSAGE, runtime_msg_cb, ctx->module, (DWORD)ctx->thread_id);
    if (!ctx->hook)
    {
        if (RemovePropA(ctx->window, RUNTIME_PROPERTY) != (HANDLE)ctx) { return LDG_ERR_INVALID; }

        return LDG_ERR_DENIED;
    }

    return LDG_ERR_AOK;
}

static uint32_t runtime_request(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (!PostMessageA(ctx->window, ctx->message, RUNTIME_MSG_PARAM_UNUSED, RUNTIME_MSG_PARAM_UNUSED)) { return LDG_ERR_CLOSED; }

    res = (uint32_t)WaitForSingleObject(ctx->ack, LIBRESTEAM_EXT_RUNTIME_WAIT_MS);
    if (res == WAIT_TIMEOUT) { return LDG_ERR_TIMEOUT; }

    if (res != WAIT_OBJECT_0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t runtime_hook_stop(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->hook) { return LDG_ERR_AOK; }

    if (!SetEvent(ctx->stop)) { return LDG_ERR_INVALID; }

    res = runtime_request(ctx);

    if (!UnhookWindowsHookEx(ctx->hook)) { return LDG_ERR_INVALID; }

    ctx->hook = 0x0;
    if (IsWindow(ctx->window) && RemovePropA(ctx->window, RUNTIME_PROPERTY) != (HANDLE)ctx) { return LDG_ERR_INVALID; }

    return res;
}

static uint32_t runtime_unpublished_dealloc(runtime_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (ctx->ack && !CloseHandle(ctx->ack)) { return LDG_ERR_IO_CLOSE; }

    if (ctx->stop && !CloseHandle(ctx->stop)) { return LDG_ERR_IO_CLOSE; }

    if (ctx->singleton && !CloseHandle(ctx->singleton)) { return LDG_ERR_IO_CLOSE; }

    res = ldg_mem_dealloc(ctx);
    return res;
}

uint32_t libresteam_ext_runtime_init(void *arg)
{
    libresteam_ext_runtime_req_t *req = (libresteam_ext_runtime_req_t *)arg;
    runtime_ctx_t *ctx = 0x0;
    HANDLE ready = 0x0;
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (req->size != sizeof(*req) || req->version != LIBRESTEAM_EXT_RUNTIME_VERSION || req->ready[sizeof(req->ready) - 1U]) { return LDG_ERR_FUNC_ARG_INVALID; }

    ready = OpenEventA(EVENT_MODIFY_STATE, FALSE, req->ready);
    if (!ready) { return LDG_ERR_NOT_FOUND; }

    res = ldg_mem_init();
    if (res == LDG_ERR_AOK) { res = ldg_mem_alloc(sizeof(*ctx), &mem); }

    if (res == LDG_ERR_AOK)
    {
        res = ldg_mem_secure_zero(mem, sizeof(*ctx));
        if (res == LDG_ERR_AOK)
        {
            ctx = (runtime_ctx_t *)mem;
            ctx->window = (HWND)(uintptr_t)req->window;
            res = runtime_hook_init(ctx);
        }
        else
        {
            res = ldg_mem_dealloc(mem);
            if (res == LDG_ERR_AOK) { res = LDG_ERR_NOT_INIT; }
        }

        if (res == LDG_ERR_AOK) { res = runtime_request(ctx); }

        if (res == LDG_ERR_AOK) { res = ctx->res; }

        if (res == LDG_ERR_AOK && (!ctx->initialized || ctx->stopped)) { res = LDG_ERR_CANCELED; }
    }

    req->res = res;
    req->win_err = res == LDG_ERR_AOK ? ERROR_SUCCESS : (uint32_t)GetLastError();
    req->thread_id = ctx ? ctx->thread_id : 0;
    if (!SetEvent(ready)) { res = LDG_ERR_INVALID; }

    if (!CloseHandle(ready)) { res = LDG_ERR_IO_CLOSE; }

    while (res == LDG_ERR_AOK && ctx && !ctx->stopped)
    {
        res = (uint32_t)WaitForSingleObject(ctx->stop, RUNTIME_TICK_MS);
        if (res != WAIT_TIMEOUT && res != WAIT_OBJECT_0) { res = LDG_ERR_INVALID; break; }

        res = runtime_request(ctx);
    }

    if (ctx)
    {
        ctx->res = res;
        res = runtime_hook_stop(ctx);
        if (res != LDG_ERR_AOK) { return res; }

        if (ctx->mods.log_path[0])
        {
            res = ldg_logging_line_wr(ctx->mods.log_path, LDG_LOGGING_LVL_INFO, RUNTIME_STOP_LOG_FMT, ctx->res);
            if (res != LDG_ERR_AOK) { return res; }
        }

        /* Published API contexts and DLLs remain resident until process exit. */
        if (!ctx->published)
        {
            res = runtime_unpublished_dealloc(ctx);
            if (res != LDG_ERR_AOK) { return res; }
        }
    }

    return res;
}
