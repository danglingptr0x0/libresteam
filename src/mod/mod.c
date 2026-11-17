#include <stdarg.h>
#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/dir.h>
#include <dangling/io/file.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>
#include <dangling/sys/lib.h>

#include <libresteam/mod/mod.h>

#define MOD_LOG_PATH "libresteam.log"
#define MOD_SHUTDOWN_LOG_FMT "mod shutdown; file: %s; resident: 1"
#define MOD_LAST_IDX_OFF 1U

static uint32_t mod_log_cb(void *handle, const char *fmt, ...)
{
    libresteam_mod_t *mod = (libresteam_mod_t *)handle;
    libresteam_mod_host_t *mods = 0x0;
    char text[LDG_LOGGING_LINE_CAP] = LDG_ARR_ZERO_INIT;
    va_list args;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t trunc = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!mod || !fmt || !mod->owner)) { return LDG_ERR_FUNC_ARG_NULL; }

    mods = (libresteam_mod_host_t *)mod->owner;
    va_start(args, fmt);
    res = ldg_str_variable_fmt(text, sizeof(text), &len, fmt, args);
    va_end(args);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_STR_TRUNC)) { return res; }

    if (res == LDG_ERR_STR_TRUNC) { trunc = LDG_TRUTH_TRUE; }

    res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod; %s; %s", mod->name, text);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (trunc) { return LDG_ERR_STR_TRUNC; }

    return LDG_ERR_AOK;
}

static uint32_t mod_tick_reg_cb(void *handle, libresteam_mod_tick_cb_t cb, void *user)
{
    libresteam_mod_t *mod = (libresteam_mod_t *)handle;
    libresteam_mod_host_t *mods = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mod || !cb || !mod->owner)) { return LDG_ERR_FUNC_ARG_NULL; }

    mods = (libresteam_mod_host_t *)mod->owner;
    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!mod->accepting || mods->stopping)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_CLOSED;
    }

    if (mods->tick_cunt == LIBRESTEAM_MOD_TICK_MAX)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod; %s; tick registration failed; limit: %u32; res: %u32", mod->name, (uint32_t)LIBRESTEAM_MOD_TICK_MAX, (uint32_t)LDG_ERR_FULL);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_FULL;
    }

    mods->tick[mods->tick_cunt] = (libresteam_mod_tick_t) { .cb = cb, .user = user, .mod = mod };
    mods->tick_cunt++;
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_cleanup_reg_cb(void *handle, libresteam_mod_cleanup_cb_t cb, void *user)
{
    libresteam_mod_t *mod = (libresteam_mod_t *)handle;
    libresteam_mod_host_t *mods = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mod || !cb || !mod->owner)) { return LDG_ERR_FUNC_ARG_NULL; }

    mods = (libresteam_mod_host_t *)mod->owner;
    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!mod->accepting || mods->stopping)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_CLOSED;
    }

    if (mod->cleanup)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_EXISTS;
    }

    mod->cleanup = cb;
    mod->cleanup_user = user;
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_cmd_reg_cb(void *handle, const libresteam_mod_cmd_t *cmd)
{
    libresteam_mod_t *mod = (libresteam_mod_t *)handle;
    libresteam_mod_host_t *mods = 0x0;
    uint32_t idx = 0x0;
    uint32_t cmp = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mod || !cmd || !mod->owner)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!cmd->name || !cmd->desc || !cmd->words || !cmd->cb)) { return LDG_ERR_FUNC_ARG_INVALID; }

    mods = (libresteam_mod_host_t *)mod->owner;
    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!mod->accepting || mods->stopping)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_CLOSED;
    }

    if (mods->cmd_cunt == LIBRESTEAM_MOD_CMD_MAX)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_FULL;
    }

    for (idx = 0; idx < mods->cmd_cunt; idx++)
    {
        res = ldg_str_cmp(mods->cmd[idx].name, cmd->name, LIBRESTEAM_MOD_NAME_MAX, &cmp);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK))
        {
            res = ldg_mut_unlock(&mods->mut);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            return LDG_ERR_STR_INVALID;
        }

        if (cmp == LDG_ARITH_ORD_EQ)
        {
            res = ldg_mut_unlock(&mods->mut);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            return LDG_ERR_EXISTS;
        }
    }

    mods->cmd[mods->cmd_cunt] = *cmd;
    mods->cmd[mods->cmd_cunt].mod = mod;
    mods->cmd_cunt++;
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_cmd_get_cb(void *handle, uint32_t idx, libresteam_mod_cmd_t *out)
{
    libresteam_mod_t *mod = (libresteam_mod_t *)handle;
    libresteam_mod_host_t *mods = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mod || !out || !mod->owner)) { return LDG_ERR_FUNC_ARG_NULL; }

    mods = (libresteam_mod_host_t *)mod->owner;
    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (mods->stopping || idx >= mods->cmd_cunt)
    {
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_NOT_FOUND;
    }

    *out = mods->cmd[idx];
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_cmds_remove(libresteam_mod_host_t *mods, libresteam_mod_t *mod)
{
    uint32_t src = UINT32_MAX;
    uint32_t dst = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    for (src = 0; src < mods->cmd_cunt; src++)
    {
        if (mods->cmd[src].mod == mod) { continue; }

        mods->cmd[dst++] = mods->cmd[src];
    }
    mods->cmd_cunt = dst;
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_ticks_remove(libresteam_mod_host_t *mods, libresteam_mod_t *mod)
{
    uint32_t src = UINT32_MAX;
    uint32_t dst = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mod->accepting = LDG_TRUTH_FALSE;
    for (src = 0; src < mods->tick_cunt; src++)
    {
        if (mods->tick[src].mod == mod) { continue; }

        mods->tick[dst++] = mods->tick[src];
    }
    mods->tick_cunt = dst;
    return ldg_mut_unlock(&mods->mut);
}

static uint32_t mod_file_ld(libresteam_mod_host_t *mods)
{
    libresteam_mod_t *mod = 0x0;
    libresteam_mod_init_cb_t init = 0x0;
    ldg_sys_lib_func_t sym = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (mods->mod_cunt == LIBRESTEAM_MOD_MAX) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod skipped; file: %s; reason: mod limit; res: %u32", mods->mod_name, (uint32_t)LDG_ERR_FULL); }

    mod = &mods->mod[mods->mod_cunt];
    res = ldg_str_rbrcpy(mod->name, mods->mod_name, sizeof(mod->name));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mod->owner = mods;
    res = ldg_sys_lib_open(mods->file_path, &mod->lib);
    if (res != LDG_ERR_AOK) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod skipped; file: %s; reason: load failed; res: %u32", mod->name, res); }

    mods->mod_cunt++;
    mod->accepting = LDG_TRUTH_TRUE;
    res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod load; file: %s; res: %u32", mod->name, res);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_sys_lib_sym_get(mod->lib, LIBRESTEAM_MOD_SHUTDOWN_SYM, &sym);
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&mod->shutdown, &sym, sizeof(mod->shutdown)); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_NOT_FOUND)) { return res; }

    res = ldg_sys_lib_sym_get(mod->lib, LIBRESTEAM_MOD_INIT_SYM, &sym);
    if (res == LDG_ERR_NOT_FOUND) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod init; file: %s; entry: absent; loaded: 1", mod->name); }

    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&init, &sym, sizeof(init)); }

    if (res == LDG_ERR_AOK) { res = init(&mods->api, mod); }

    if (res == LDG_ERR_AOK) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod init; file: %s; res: %u32", mod->name, res); }

    res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod init; file: %s; res: %u32", mod->name, res);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = mod_ticks_remove(mods, mod);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = mod_cmds_remove(mods, mod);
    return res;
}

static uint32_t mod_next_get(const libresteam_mod_host_t *mods, const char *path, const char *after, char *out, uint64_t cap)
{
    ldg_io_dir_t *dir = 0x0;
    ldg_io_entry_t entry = LDG_STRUCT_ZERO_INIT;
    uint32_t cmp = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = LDG_STR_TERM;
    res = ldg_io_dir_open(path, &dir);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    for (;;)
    {
        res = ldg_io_dir_rd(dir, &entry);
        if (res == LDG_ERR_STR_INVALID || res == LDG_ERR_STR_TRUNC)
        {
            if (*after == LDG_STR_TERM)
            {
                res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod entry skipped; dir: %s; reason: unsupported filename; res: %u32", path, res);
                if (LDG_UNLIKELY(res != LDG_ERR_AOK))
                {
                    res = ldg_io_dir_close(dir);
                    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

                    return LDG_ERR_IO_WR;
                }
            }

            continue;
        }

        if (res != LDG_ERR_AOK) { break; }

        if (entry.is_dir) { continue; }

        res = ldg_str_cmp(entry.name, after, sizeof(entry.name), &cmp);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { break; }

        if (cmp != LDG_ARITH_ORD_GT) { continue; }

        if (*out != LDG_STR_TERM)
        {
            res = ldg_str_cmp(entry.name, out, sizeof(entry.name), &cmp);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { break; }

            if (cmp != LDG_ARITH_ORD_LT) { continue; }
        }

        res = ldg_str_rbrcpy(out, entry.name, cap);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { break; }
    }
    if (LDG_UNLIKELY(res != LDG_ERR_EOF))
    {
        res = ldg_io_dir_close(dir);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_IO_RD;
    }

    res = ldg_io_dir_close(dir);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (*out == LDG_STR_TERM) { return LDG_ERR_EOF; }

    return LDG_ERR_AOK;
}

static uint32_t mod_candidate_ld(libresteam_mod_host_t *mods)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    char ext[sizeof(LIBRESTEAM_MOD_EXT)] = LDG_ARR_ZERO_INIT;
    uint32_t cmp = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_io_path_ext_get(mods->file_name, ext, sizeof(ext));
    if (res == LDG_ERR_AOK) { res = ldg_str_cmp(ext, LIBRESTEAM_MOD_EXT, sizeof(ext), &cmp); }

    if (res != LDG_ERR_AOK || cmp != LDG_ARITH_ORD_EQ) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod skipped; file: %s; reason: extension", mods->mod_name); }

    res = ldg_io_path_join(mods->dir_path, mods->file_name, mods->file_path, sizeof(mods->file_path));
    if (res != LDG_ERR_AOK) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod skipped; file: %s; reason: path; res: %u32", mods->mod_name, res); }

    res = ldg_io_path_info_get(mods->file_path, &info);
    if (res != LDG_ERR_AOK || !info.is_file) { return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod skipped; file: %s; reason: not a regular file; res: %u32", mods->mod_name, res); }

    return mod_file_ld(mods);
}

static uint32_t mod_dir_ld(libresteam_mod_host_t *mods)
{
    char next[LIBRESTEAM_MOD_NAME_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    *mods->file_name = LDG_STR_TERM;
    while ((res = mod_next_get(mods, mods->dir_path, mods->file_name, next, sizeof(next))) == LDG_ERR_AOK)
    {
        res = ldg_str_rbrcpy(mods->file_name, next, sizeof(mods->file_name));
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        res = ldg_io_path_join(mods->dir_name, next, mods->mod_name, sizeof(mods->mod_name));
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        res = mod_candidate_ld(mods);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    if (res == LDG_ERR_EOF) { return LDG_ERR_AOK; }

    return ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod directory skipped; dir: %s; res: %u32", mods->dir_path, res);
}

static uint32_t mod_scan_init(libresteam_mod_host_t *mods)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (!mods->runtime)
    {
        res = ldg_io_path_prog_get(mods->file_path, sizeof(mods->file_path));
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        res = ldg_io_path_info_get(mods->file_path, &info);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (info.is_dir == LDG_TRUTH_TRUE) { res = ldg_io_path_normalize(mods->file_path, mods->root_path, sizeof(mods->root_path)); }
        else { res = ldg_io_path_parent_get(mods->file_path, mods->root_path, sizeof(mods->root_path)); }
    }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_str_rbrcpy(mods->dir_name, LIBRESTEAM_MOD_DIR, sizeof(mods->dir_name));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_io_path_join(mods->root_path, mods->dir_name, mods->dir_path, sizeof(mods->dir_path));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod scan root; dir: %s", mods->dir_path);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    return mod_dir_ld(mods);
}

uint32_t libresteam_mod_ld(libresteam_mod_host_t *mods, const libresteam_mod_api_t *seed)
{
    uint32_t res = LDG_ERR_AOK;
    uint32_t mod_cunt = 0x0;
    uint32_t tick_cunt = 0x0;
    uint8_t stopping = LDG_TRUTH_FALSE;
    uint8_t scan_failed = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!mods)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (mods->ld_done) { return LDG_ERR_AOK; }

    if (!mods->runtime) { res = ldg_str_rbrcpy(mods->log_path, MOD_LOG_PATH, sizeof(mods->log_path)); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_mut_init(&mods->mut, LDG_TRUTH_FALSE);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = seed ? ldg_mem_secure_cpy(&mods->api, seed, sizeof(mods->api)) : ldg_mem_secure_zero(&mods->api, sizeof(mods->api));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        res = ldg_mut_destroy(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    mods->ld_done = LDG_TRUTH_TRUE;
    mods->loading = LDG_TRUTH_TRUE;
    mods->api.version = LIBRESTEAM_MOD_API_VERSION;
    mods->api.size = (uint32_t)sizeof(mods->api);
    mods->api.log = mod_log_cb;
    mods->api.tick_reg = mod_tick_reg_cb;
    mods->api.cleanup_reg = mod_cleanup_reg_cb;
    mods->api.cmd_reg = mod_cmd_reg_cb;
    mods->api.cmd_get = mod_cmd_get_cb;
    res = mod_scan_init(mods);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_WARN, "mod scan failed; res: %u32", res);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        scan_failed = LDG_TRUTH_TRUE;
    }

    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mods->loading = LDG_TRUTH_FALSE;
    mods->active = !mods->stopping;
    mod_cunt = mods->mod_cunt;
    tick_cunt = mods->tick_cunt;
    stopping = mods->stopping;
    res = ldg_mut_unlock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, "mod scan; loaded: %u32; ticks: %u32", mod_cunt, tick_cunt);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (stopping) { return libresteam_mod_unld(mods); }

    if (scan_failed) { return LDG_ERR_NOT_INIT; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_mod_dir_ld(libresteam_mod_host_t *mods, const libresteam_mod_api_t *seed, const char *root)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mods || !root)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (mods->ld_done) { return LDG_ERR_EXISTS; }

    res = ldg_str_rbrcpy(mods->root_path, root, sizeof(mods->root_path));
    if (res == LDG_ERR_AOK) { res = ldg_io_path_join(root, "libresteam-runtime.log", mods->log_path, sizeof(mods->log_path)); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mods->runtime = LDG_TRUTH_TRUE;
    return libresteam_mod_ld(mods, seed);
}

uint32_t libresteam_mod_unld(libresteam_mod_host_t *mods)
{
    libresteam_mod_t *mod = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!mods)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!mods->ld_done) { return LDG_ERR_AOK; }

    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mods->active = LDG_TRUTH_FALSE;
    mods->stopping = LDG_TRUTH_TRUE;
    if (mods->dispatching || mods->loading) { return ldg_mut_unlock(&mods->mut); }

    mods->tick_cunt = 0;
    mods->cmd_cunt = 0;
    res = ldg_mut_unlock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    for (idx = mods->mod_cunt; idx > 0; idx--)
    {
        res = ldg_mut_lock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        mod = &mods->mod[idx - MOD_LAST_IDX_OFF];
        if (mod->stopped)
        {
            res = ldg_mut_unlock(&mods->mut);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            continue;
        }

        mod->stopped = LDG_TRUTH_TRUE;
        mod->accepting = LDG_TRUTH_FALSE;
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (mod->cleanup) { mod->cleanup(mod->cleanup_user); }

        if (mod->shutdown) { mod->shutdown(); }

        res = ldg_logging_line_wr(mods->log_path, LDG_LOGGING_LVL_INFO, MOD_SHUTDOWN_LOG_FMT, mod->name);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    return LDG_ERR_AOK;
}

uint32_t libresteam_mod_tick(libresteam_mod_host_t *mods)
{
    libresteam_mod_tick_t tick = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = UINT32_MAX;
    uint32_t cunt = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t stopping = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!mods)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!mods->ld_done) { return LDG_ERR_AOK; }

    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!mods->active || mods->dispatching || mods->loading) { return ldg_mut_unlock(&mods->mut); }

    mods->dispatching = LDG_TRUTH_TRUE;
    cunt = mods->tick_cunt;
    res = ldg_mut_unlock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    for (idx = 0; idx < cunt; idx++)
    {
        res = ldg_mut_lock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        stopping = mods->stopping;
        tick = mods->tick[idx];
        res = ldg_mut_unlock(&mods->mut);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (stopping) { break; }

        tick.cb(tick.user);
    }
    res = ldg_mut_lock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    mods->dispatching = LDG_TRUTH_FALSE;
    stopping = mods->stopping;
    res = ldg_mut_unlock(&mods->mut);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (stopping) { return libresteam_mod_unld(mods); }

    return LDG_ERR_AOK;
}
