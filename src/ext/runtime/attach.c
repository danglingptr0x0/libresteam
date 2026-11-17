#include <stdint.h>
#include <windows.h>
#include <tlhelp32.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>

#include <libresteam/ext/runtime/runtime.h>

#define ATTACH_ASCII_MAX 0x7fU
#define ATTACH_PID_ARG 1U
#define ATTACH_PATH_ARG 2U
#define ATTACH_WINDOW_ARG 3U
#define ATTACH_ARGS_MIN 3U
#define ATTACH_ARGS_MAX 4U
#define ATTACH_MSG_CAP 1024U
#define ATTACH_PROC_ACCESS (PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ | SYNCHRONIZE)

typedef struct attach_ctx
{
    libresteam_ext_runtime_req_t req;
    MODULEENTRY32W entry;
    IMAGE_DOS_HEADER dos;
    IMAGE_NT_HEADERS64 nt;
    HANDLE proc;
    HANDLE thread;
    HANDLE ready;
    HMODULE image;
    HWND window;
    void *remote;
    uintptr_t remote_host;
    uintptr_t load_func;
    uintptr_t init_func;
    char path[LDG_IO_PATH_MAX];
    WCHAR wide_path[LDG_IO_PATH_MAX];
    WCHAR owner_path[LDG_IO_PATH_MAX];
    char msg[ATTACH_MSG_CAP];
    uint32_t pid;
    uint32_t windows;
    uint32_t path_len;
    uint32_t win_err;
    uint8_t remote_in_use;
    uint8_t pudding[7];
    const char *stage;
} attach_ctx_t;

static uint32_t attach_text(const char *text)
{
    DWORD written = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE);

    if (LDG_UNLIKELY(!text)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_str_len_get(text, ATTACH_MSG_CAP, &len);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!WriteFile(output, text, (DWORD)len, &written, 0x0) || written != len) { return LDG_ERR_IO_WR; }

    return LDG_ERR_AOK;
}

static uint32_t attach_num_get(const char *text, uint64_t *out)
{
    uint32_t idx = UINT32_MAX;
    uint32_t base = LDG_BASE_DEC;

    if (LDG_UNLIKELY(!text || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) { text += 2; base = LDG_BASE_HEX; }

    if (!*text) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; text[idx] != LDG_STR_TERM; idx++)
    {
        if (base == LDG_BASE_DEC && ldg_char_digit_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }

        if (base == LDG_BASE_HEX && ldg_char_hex_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }
    }
    return ldg_str_uint_parse(text, base, idx, out);
}

static BOOL CALLBACK attach_window_cb(HWND window, LPARAM arg)
{
    attach_ctx_t *ctx = (attach_ctx_t *)arg;
    DWORD pid = 0x0;

    if (!GetWindowThreadProcessId(window, &pid)) { return TRUE; }

    if (pid == ctx->pid && IsWindowVisible(window) && !GetWindow(window, GW_OWNER))
    {
        ctx->window = window;
        ctx->windows++;
    }

    return TRUE;
}

static uint32_t attach_module_get(attach_ctx_t *ctx, const WCHAR *path, uintptr_t *out)
{
    HANDLE snapshot = INVALID_HANDLE_VALUE;
    uint32_t res = LDG_ERR_NOT_FOUND;

    *out = 0;
    snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, (DWORD)ctx->pid);
    if (snapshot == INVALID_HANDLE_VALUE) { return LDG_ERR_IO_RD; }

    ctx->entry.dwSize = sizeof(ctx->entry);
    if (Module32FirstW(snapshot, &ctx->entry))
    {
        do{ if (lstrcmpiW(ctx->entry.szExePath, path) == 0)
            {
                *out = (uintptr_t)ctx->entry.modBaseAddr;
                res = LDG_ERR_AOK;
                break;
            }
        } while (Module32NextW(snapshot, &ctx->entry));
    }

    if (!CloseHandle(snapshot)) { return LDG_ERR_IO_CLOSE; }

    return res;
}

static uint32_t attach_export_get(attach_ctx_t *ctx, HMODULE local, const char *name, uintptr_t *out)
{
    HMODULE owner = 0x0;
    FARPROC proc = 0x0;
    uintptr_t addr = 0x0;
    uintptr_t remote = 0x0;
    uintptr_t off = 0x0;
    SIZE_T bytes = 0x0;
    const IMAGE_DOS_HEADER *local_dos = 0x0;
    const IMAGE_NT_HEADERS64 *local_nt = 0x0;
    uint32_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    proc = GetProcAddress(local, name);
    if (!proc) { return LDG_ERR_NOT_FOUND; }

    res = ldg_mem_secure_cpy(&addr, &proc, sizeof(addr));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT, (LPCSTR)addr, &owner)) { return LDG_ERR_NOT_FOUND; }

    len = (uint32_t)GetModuleFileNameW(owner, ctx->owner_path, (DWORD)(sizeof(ctx->owner_path) / sizeof(ctx->owner_path[0])));
    if (!len || len >= sizeof(ctx->owner_path) / sizeof(ctx->owner_path[0])) { return LDG_ERR_STR_TRUNC; }

    res = attach_module_get(ctx, ctx->owner_path, &remote);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    off = addr - (uintptr_t)owner;
    if (off >= ctx->entry.modBaseSize) { return LDG_ERR_BOUNDS; }

    if (!ReadProcessMemory(ctx->proc, (const void *)remote, &ctx->dos, sizeof(ctx->dos), &bytes) || bytes != sizeof(ctx->dos)) { return LDG_ERR_IO_RD; }

    if (ctx->dos.e_magic != IMAGE_DOS_SIGNATURE || ctx->dos.e_lfanew < 0 || ctx->entry.modBaseSize < sizeof(ctx->nt) || (uint32_t)ctx->dos.e_lfanew > ctx->entry.modBaseSize - sizeof(ctx->nt)) { return LDG_ERR_INVALID; }

    if (!ReadProcessMemory(ctx->proc, (const void *)(remote + (uint32_t)ctx->dos.e_lfanew), &ctx->nt, sizeof(ctx->nt), &bytes) || bytes != sizeof(ctx->nt)) { return LDG_ERR_IO_RD; }

    local_dos = (const IMAGE_DOS_HEADER *)owner;
    local_nt = (const IMAGE_NT_HEADERS64 *)((uintptr_t)owner + (uint32_t)local_dos->e_lfanew);
    if (ctx->nt.Signature != IMAGE_NT_SIGNATURE || ctx->nt.FileHeader.Machine != IMAGE_FILE_MACHINE_AMD64 || ctx->nt.OptionalHeader.Magic != IMAGE_NT_OPTIONAL_HDR64_MAGIC || ctx->nt.FileHeader.TimeDateStamp != local_nt->FileHeader.TimeDateStamp || ctx->nt.OptionalHeader.SizeOfImage != local_nt->OptionalHeader.SizeOfImage) { return LDG_ERR_UNSUPPORTED; }

    *out = remote + off;
    return LDG_ERR_AOK;
}

static uint32_t attach_thread_start(attach_ctx_t *ctx, uintptr_t addr, void *arg)
{
    LPTHREAD_START_ROUTINE entry = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_secure_cpy(&entry, &addr, sizeof(entry));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->thread = CreateRemoteThread(ctx->proc, 0x0, 0, entry, arg, 0, 0x0);
    if (!ctx->thread) { return LDG_ERR_DENIED; }

    ctx->remote_in_use = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t attach_host_ld(attach_ctx_t *ctx)
{
    HMODULE kernel = GetModuleHandleA("kernel32.dll");
    SIZE_T bytes = 0x0;
    uint64_t size = ((uint64_t)ctx->path_len + LDG_STR_TERM_SIZE) * sizeof(WCHAR);
    uint32_t res = LDG_ERR_AOK;
    uint32_t wait_res = 0x0;

    if (!kernel) { return LDG_ERR_NOT_FOUND; }

    res = attach_module_get(ctx, ctx->wide_path, &ctx->remote_host);
    if (res == LDG_ERR_AOK) { return LDG_ERR_EXISTS; }

    if (res != LDG_ERR_NOT_FOUND) { return res; }

    res = attach_export_get(ctx, kernel, "LoadLibraryW", &ctx->load_func);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->remote = VirtualAllocEx(ctx->proc, 0x0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!ctx->remote) { return LDG_ERR_ALLOC_NULL; }

    if (!WriteProcessMemory(ctx->proc, ctx->remote, ctx->wide_path, size, &bytes) || bytes != size) { return LDG_ERR_IO_WR; }

    res = attach_thread_start(ctx, ctx->load_func, ctx->remote);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    wait_res = (uint32_t)WaitForSingleObject(ctx->thread, LIBRESTEAM_EXT_RUNTIME_WAIT_MS);
    if (wait_res != WAIT_OBJECT_0) { return wait_res == WAIT_TIMEOUT ? LDG_ERR_TIMEOUT : LDG_ERR_INVALID; }

    ctx->remote_in_use = LDG_TRUTH_FALSE;
    if (!CloseHandle(ctx->thread)) { return LDG_ERR_IO_CLOSE; }

    ctx->thread = 0x0;
    if (!VirtualFreeEx(ctx->proc, ctx->remote, 0, MEM_RELEASE)) { return LDG_ERR_INVALID; }

    ctx->remote = 0x0;
    return attach_module_get(ctx, ctx->wide_path, &ctx->remote_host);
}

static uint32_t attach_host_init(attach_ctx_t *ctx)
{
    HANDLE waits[2] = LDG_ARR_ZERO_INIT;
    SIZE_T bytes = 0x0;
    uint64_t len = 0x0;
    uint32_t wait_res = 0x0;
    uint32_t res = LDG_ERR_AOK;
    DWORD thread_res = 0x0;

    ctx->req.size = sizeof(ctx->req);
    ctx->req.version = LIBRESTEAM_EXT_RUNTIME_VERSION;
    ctx->req.window = (uint64_t)(uintptr_t)ctx->window;
    ctx->req.res = LDG_ERR_NOT_INIT;
    res = ldg_str_fmt(ctx->req.ready, sizeof(ctx->req.ready), &len, LIBRESTEAM_EXT_RUNTIME_READY_FMT, ctx->pid, (uint32_t)GetCurrentProcessId());
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->ready = CreateEventA(0x0, TRUE, FALSE, ctx->req.ready);
    if (!ctx->ready) { return LDG_ERR_DENIED; }

    if (GetLastError() == ERROR_ALREADY_EXISTS) { return LDG_ERR_EXISTS; }

    ctx->image = LoadLibraryExW(ctx->wide_path, 0x0, DONT_RESOLVE_DLL_REFERENCES);
    if (!ctx->image) { return LDG_ERR_IO_OPEN; }

    res = attach_export_get(ctx, ctx->image, LIBRESTEAM_EXT_RUNTIME_START_SYM, &ctx->init_func);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->remote = VirtualAllocEx(ctx->proc, 0x0, sizeof(ctx->req), MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
    if (!ctx->remote) { return LDG_ERR_ALLOC_NULL; }

    if (!WriteProcessMemory(ctx->proc, ctx->remote, &ctx->req, sizeof(ctx->req), &bytes) || bytes != sizeof(ctx->req)) { return LDG_ERR_IO_WR; }

    res = attach_thread_start(ctx, ctx->init_func, ctx->remote);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    waits[0] = ctx->ready;
    waits[1] = ctx->thread;
    wait_res = (uint32_t)WaitForMultipleObjects(2, waits, FALSE, LIBRESTEAM_EXT_RUNTIME_WAIT_MS * 2U);
    if (wait_res == WAIT_OBJECT_0)
    {
        ctx->remote_in_use = LDG_TRUTH_FALSE;
        if (!ReadProcessMemory(ctx->proc, ctx->remote, &ctx->req, sizeof(ctx->req), &bytes) || bytes != sizeof(ctx->req)) { return LDG_ERR_IO_RD; }

        ctx->win_err = ctx->req.win_err;
        return ctx->req.res;
    }

    if (wait_res == WAIT_OBJECT_0 + 1U)
    {
        ctx->remote_in_use = LDG_TRUTH_FALSE;
        if (!GetExitCodeThread(ctx->thread, &thread_res)) { return LDG_ERR_INVALID; }

        return thread_res ? (uint32_t)thread_res : LDG_ERR_NOT_INIT;
    }

    return wait_res == WAIT_TIMEOUT ? LDG_ERR_TIMEOUT : LDG_ERR_INVALID;
}

static uint32_t attach_stop(attach_ctx_t *ctx)
{
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_str_fmt(ctx->msg, sizeof(ctx->msg), &len, LIBRESTEAM_EXT_RUNTIME_STOP_FMT, ctx->pid);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->ready = OpenEventA(EVENT_MODIFY_STATE, FALSE, ctx->msg);
    if (!ctx->ready) { return LDG_ERR_NOT_FOUND; }

    if (!SetEvent(ctx->ready)) { return LDG_ERR_DENIED; }

    return attach_text("Stop requested; extension shutdown runs on the selected window thread. DLLs remain resident until game exit.\n");
}

static uint32_t attach_run(attach_ctx_t *ctx, uint32_t argc, char **argv)
{
    uint64_t val = 0x0;
    uint64_t len = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    DWORD pid = 0x0;
    BOOL wow64 = FALSE;

    if (argc == ATTACH_ARGS_MIN && lstrcmpA(argv[ATTACH_PID_ARG], "stop") == 0)
    {
        res = attach_num_get(argv[ATTACH_PATH_ARG], &val);
        if (res != LDG_ERR_AOK || !val || val > UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        ctx->pid = (uint32_t)val;
        return attach_stop(ctx);
    }

    res = attach_num_get(argv[ATTACH_PID_ARG], &val);
    if (res != LDG_ERR_AOK || !val || val > UINT32_MAX || val == GetCurrentProcessId()) { return LDG_ERR_FUNC_ARG_INVALID; }

    ctx->pid = (uint32_t)val;
    ctx->stage = "open process";
    ctx->proc = OpenProcess(ATTACH_PROC_ACCESS, FALSE, (DWORD)ctx->pid);
    if (!ctx->proc) { return LDG_ERR_DENIED; }

    if (!IsWow64Process(ctx->proc, &wow64)) { return LDG_ERR_INVALID; }

    if (wow64) { return LDG_ERR_UNSUPPORTED; }

    ctx->stage = "select game window";
    if (argc == ATTACH_ARGS_MAX)
    {
        res = attach_num_get(argv[ATTACH_WINDOW_ARG], &val);
        if (res != LDG_ERR_AOK || !val) { return LDG_ERR_FUNC_ARG_INVALID; }

        ctx->window = (HWND)(uintptr_t)val;
    }
    else
    {
        if (!EnumWindows(attach_window_cb, (LPARAM)ctx)) { return LDG_ERR_INVALID; }

        if (ctx->windows != 1U)
        {
            res = attach_text("Expected one visible unowned game window. Supply its HWND as the third argument to select explicitly.\n");
            return res == LDG_ERR_AOK ? LDG_ERR_FUNC_ARG_INVALID : res;
        }
    }

    ctx->req.thread_id = (uint32_t)GetWindowThreadProcessId(ctx->window, &pid);
    if (!ctx->req.thread_id || pid != ctx->pid) { return LDG_ERR_FUNC_ARG_INVALID; }

    ctx->stage = "resolve host path";
    for (idx = 0; argv[ATTACH_PATH_ARG][idx]; idx++) { if ((uint8_t)argv[ATTACH_PATH_ARG][idx] > ATTACH_ASCII_MAX) { return LDG_ERR_STR_INVALID; } }
    ctx->path_len = (uint32_t)GetFullPathNameA(argv[ATTACH_PATH_ARG], (DWORD)sizeof(ctx->path), ctx->path, 0x0);
    if (!ctx->path_len || ctx->path_len >= sizeof(ctx->path)) { return LDG_ERR_STR_TRUNC; }

    for (idx = 0; idx <= ctx->path_len; idx++)
    {
        if ((uint8_t)ctx->path[idx] > ATTACH_ASCII_MAX) { return LDG_ERR_STR_INVALID; }

        ctx->wide_path[idx] = (WCHAR)(uint8_t)ctx->path[idx];
    }
    ctx->stage = "load host";
    res = attach_host_ld(ctx);
    if (res == LDG_ERR_AOK)
    {
        ctx->stage = "initialize window-thread dispatch";
        res = attach_host_init(ctx);
    }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_str_fmt(ctx->msg, sizeof(ctx->msg), &len, "Attached; pid: %u32; window: 0x%x64; dispatch thread: %u32; transport: Windows message hook\nConnect: libresteam-repl.exe %u32\n", ctx->pid, (uint64_t)(uintptr_t)ctx->window, ctx->req.thread_id, ctx->pid);
    return res == LDG_ERR_AOK ? attach_text(ctx->msg) : res;
}

static uint32_t attach_cleanup(attach_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (ctx->remote && !ctx->remote_in_use && !VirtualFreeEx(ctx->proc, ctx->remote, 0, MEM_RELEASE)) { res = LDG_ERR_INVALID; }

    if (ctx->thread && !CloseHandle(ctx->thread)) { res = LDG_ERR_IO_CLOSE; }

    if (ctx->ready && !CloseHandle(ctx->ready)) { res = LDG_ERR_IO_CLOSE; }

    if (ctx->image && !FreeLibrary(ctx->image)) { res = LDG_ERR_IO_CLOSE; }

    if (ctx->proc && !CloseHandle(ctx->proc)) { res = LDG_ERR_IO_CLOSE; }

    return res;
}

int main(int argc, char **argv)
{
    attach_ctx_t *ctx = 0x0;
    void *mem = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (argc < (int)ATTACH_ARGS_MIN || argc > (int)ATTACH_ARGS_MAX)
    {
        res = attach_text("Usage: libresteam-attach.exe PID path\\libresteam-runtime.dll [HWND]\n       libresteam-attach.exe stop PID\nWindows x64 only. Launch the game normally first.\n");
        return (int)(res == LDG_ERR_AOK ? LDG_ERR_FUNC_ARG_INVALID : res);
    }

    res = ldg_mem_init();
    if (res == LDG_ERR_AOK) { res = ldg_mem_alloc(sizeof(*ctx), &mem); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return (int)res; }

    res = ldg_mem_secure_zero(mem, sizeof(*ctx));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        res = ldg_mem_dealloc(mem);
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    ctx = (attach_ctx_t *)mem;
    if (res == LDG_ERR_AOK) { res = ldg_mem_lock(); }

    ctx->stage = "arguments";
    if (res == LDG_ERR_AOK) { res = attach_run(ctx, (uint32_t)argc, argv); }

    if (res != LDG_ERR_AOK)
    {
        if (!ctx->win_err) { ctx->win_err = (uint32_t)GetLastError(); }

        res = ldg_str_fmt(ctx->msg, sizeof(ctx->msg), &len, "Attach failed; stage: %s; res: %u32; last win32: %u32; remote request retained: %u32. Runtime log is beside the host DLL. Restart the game before retrying a loaded host.\n", ctx->stage, res, ctx->win_err, (uint32_t)ctx->remote_in_use);
        if (res != LDG_ERR_AOK) { return res; }

        res = attach_text(ctx->msg);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = attach_cleanup(ctx);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_dealloc(ctx);
    return res;
}
