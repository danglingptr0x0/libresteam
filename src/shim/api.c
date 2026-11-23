#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam/steam.h>
#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_BACKEND_CUNT 1U
#define LIBRESTEAM_SHIM_PRIMARY_IDX 0U
#define LIBRESTEAM_SHIM_INSTANCE_NONE 0U
#define LIBRESTEAM_SHIM_API_LOG_LVL LDG_LOGGING_LVL_INFO

typedef void (*libresteam_shim_ctx_init_cb_t)(void *ptr);

static libresteam_shim_ctx_t *libresteam_shim_ctx = 0x0;

uint32_t libresteam_shim_ctx_get(libresteam_shim_ctx_t **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = libresteam_shim_ctx;
    if (LDG_UNLIKELY(!libresteam_shim_ctx)) { return LDG_ERR_NOT_INIT; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_ctx_ensure_get(libresteam_shim_ctx_t **out)
{
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = libresteam_shim_ctx;
    if (libresteam_shim_ctx) { return LDG_ERR_AOK; }

    res = ldg_mem_init();
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_alloc((uint64_t)sizeof(libresteam_shim_ctx_t), &mem);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_zero(mem, (uint64_t)sizeof(libresteam_shim_ctx_t));
    if (res != LDG_ERR_AOK) { return res; }

    libresteam_shim_ctx = (libresteam_shim_ctx_t *)mem;

    res = libresteam_shim_iface_build(libresteam_shim_ctx);
    if (res != LDG_ERR_AOK) { return res; }

    *out = libresteam_shim_ctx;

    return LDG_ERR_AOK;
}

uint8_t libresteam_shim_api_init(void)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t log_res = LDG_ERR_AOK;

    log_res = ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init enter");
    res = libresteam_shim_ctx_ensure_get(&ctx);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: context; ready: 0; res: %u32; prior log res: %u32", res, log_res) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

        return LDG_TRUTH_FALSE;
    }

    if (ctx->steam_up == LDG_TRUTH_TRUE)
    {
        if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: already initialized; ready: 1; prior log res: %u32", log_res) != LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

        return LDG_TRUTH_TRUE;
    }

    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init backend begin");
    res = libresteam_steam_init(&ctx->steam, &ctx->steam_conf, &ctx->backend);
    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init backend returned; res: %u32", res);
    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init; steamclient: %s; pipe: %u32; user: %u32; res: %u32", ctx->steam.client_path, ctx->steam.pipe, ctx->steam.user, res);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: backend; ready: 0; res: %u32; prior log res: %u32", res, log_res) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

        return LDG_TRUTH_FALSE;
    }

    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init set begin");
    res = libresteam_backend_set_init(&ctx->set, &ctx->backend, LIBRESTEAM_SHIM_BACKEND_CUNT, LIBRESTEAM_SHIM_PRIMARY_IDX);
    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "set init; res: %u32", res);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: set; ready: 0; res: %u32; prior log res: %u32", res, log_res) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

        return LDG_TRUTH_FALSE;
    }

    res = libresteam_shim_flat_ck(ctx);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        log_res |= libresteam_steam_shutdown(&ctx->steam);
        if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: flat ck; res: %u32; prior log res: %u32", res, log_res) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

        return LDG_TRUTH_FALSE;
    }

    ctx->steam_up = LDG_TRUTH_TRUE;
    ctx->instance += LIBRESTEAM_SHIM_INSTANCE_STEP;

    log_res |= ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init done; instance: %u64", ctx->instance);
    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "init exit; stage: complete; ready: 1; prior log res: %u32", log_res) != LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_api_shutdown(void)
{
    libresteam_shim_ctx_t *ctx = libresteam_shim_ctx;
    uint32_t res = LDG_ERR_AOK;

    if (!ctx) { return; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return; }

    res = libresteam_steam_shutdown(&ctx->steam);
    ctx->steam_up = LDG_TRUTH_FALSE;
    ctx->instance += LIBRESTEAM_SHIM_INSTANCE_STEP;

    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "shutdown; res: %u32", res) != LDG_ERR_AOK) { return; }
}

uint8_t libresteam_shim_api_app_restart_is(uint32_t app_id)
{
    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "restart app if necessary; app_id: %u32", app_id) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

uint32_t libresteam_shim_api_user_get(void)
{
    if (!libresteam_shim_ctx) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_ctx->steam_up != LDG_TRUTH_TRUE) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    return LIBRESTEAM_SHIM_USER_HANDLE;
}

uint32_t libresteam_shim_api_pipe_get(void)
{
    if (!libresteam_shim_ctx) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_ctx->steam_up != LDG_TRUTH_TRUE) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    return LIBRESTEAM_SHIM_PIPE_HANDLE;
}

void* libresteam_shim_api_ctx_init(void *data)
{
    uintptr_t *slot = (uintptr_t *)data;
    libresteam_shim_ctx_init_cb_t fn = 0x0;
    uint64_t instance = LIBRESTEAM_SHIM_INSTANCE_NONE;

    if (!slot) { return 0x0; }

    if (libresteam_shim_ctx) { instance = libresteam_shim_ctx->instance; }

    if ((uint64_t)slot[LIBRESTEAM_SHIM_CTX_INIT_CUNTR] != instance)
    {
        if (ldg_mem_secure_cpy(&fn, &slot[LIBRESTEAM_SHIM_CTX_INIT_FN], (uint64_t)sizeof(fn)) != LDG_ERR_AOK) { return &slot[LIBRESTEAM_SHIM_CTX_INIT_PTR]; }

        if (fn) { fn(&slot[LIBRESTEAM_SHIM_CTX_INIT_PTR]); }

        slot[LIBRESTEAM_SHIM_CTX_INIT_CUNTR] = (uintptr_t)instance;
    }

    return &slot[LIBRESTEAM_SHIM_CTX_INIT_PTR];
}

void* libresteam_shim_api_user_iface_get(uint32_t user, const char *version)
{
    void *iface = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!libresteam_shim_ctx || !version) { return 0x0; }

    if (libresteam_shim_ctx->steam_up != LDG_TRUTH_TRUE) { return 0x0; }

    res = libresteam_shim_iface_find(libresteam_shim_ctx, version, &iface);
    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "iface; user: %u32; version: %s; res: %u32", user, version, res) != LDG_ERR_AOK) { return iface; }

    return iface;
}

void* libresteam_shim_api_iface_create(const char *version)
{
    if (!libresteam_shim_ctx) { return 0x0; }

    return libresteam_shim_api_user_iface_get(LIBRESTEAM_SHIM_USER_HANDLE, version);
}

void* libresteam_shim_api_game_srv_iface_get(uint32_t user, const char *version)
{
    if (!version) { return 0x0; }

    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "game srv iface unsupported; user: %u32; version: %s", user, version) != LDG_ERR_AOK) { return 0x0; }

    return 0x0;
}

void libresteam_shim_api_cb_reg(void *obj, uint32_t cb_id)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (!obj) { return; }

    if (libresteam_shim_ctx_ensure_get(&ctx) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_cb_reg(ctx, obj, cb_id) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_api_cb_unreg(void *obj)
{
    if (!obj || !libresteam_shim_ctx) { return; }

    if (libresteam_shim_cb_unreg(libresteam_shim_ctx, obj) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_api_cb_run(void)
{
    uint32_t res = LDG_ERR_AOK;

    if (!libresteam_shim_ctx) { return; }

    if (libresteam_shim_ctx->steam_up != LDG_TRUTH_TRUE) { return; }

    res = libresteam_shim_cb_run(libresteam_shim_ctx);
    if (libresteam_shim_ctx->cb_run_seen == LDG_TRUTH_TRUE && libresteam_shim_ctx->cb_run_res == res) { return; }

    libresteam_shim_ctx->cb_run_seen = LDG_TRUTH_TRUE;
    libresteam_shim_ctx->cb_run_res = res;

    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "cb run; res: %u32", res) != LDG_ERR_AOK) { return; }
}

uint8_t libresteam_shim_game_srv_init(uint32_t ip, uint16_t steam_port, uint16_t game_port, uint16_t query_port, uint32_t mode, const char *version)
{
    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "game srv init unsupported; ip: %u32; steam port: %u32; game port: %u32; query port: %u32; mode: %u32; version: %s", ip, (uint32_t)steam_port, (uint32_t)game_port, (uint32_t)query_port, mode, version) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

void libresteam_shim_game_srv_shutdown(void)
{
    if (ldg_logging_ln_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_API_LOG_LVL, "game srv shutdown") != LDG_ERR_AOK) { return; }
}

void libresteam_shim_game_srv_cb_run(void)
{
    if (!libresteam_shim_ctx) { return; }
}

uint32_t libresteam_shim_game_srv_user_get(void)
{
    if (!libresteam_shim_ctx) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    return LIBRESTEAM_SHIM_HANDLE_NONE;
}

uint32_t libresteam_shim_game_srv_pipe_get(void)
{
    if (!libresteam_shim_ctx) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    return LIBRESTEAM_SHIM_HANDLE_NONE;
}
