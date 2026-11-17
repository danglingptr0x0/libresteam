#include <stdarg.h>
#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/secure.h>
#include <dangling/parse/parse.h>
#include <dangling/str/fmt.h>

#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_STUB_RET_NONE 0U
#define LIBRESTEAM_SHIM_STR_EMPTY ""
#define LIBRESTEAM_SHIM_TAG_CALL "call"
#define LIBRESTEAM_SHIM_TAG_STUB "stub"
#define LIBRESTEAM_SHIM_LOG_LVL LDG_LOGGING_LVL_INFO

#ifndef LIBRESTEAM_SHIM_ARG_LOG_EN
#define LIBRESTEAM_SHIM_ARG_LOG_EN 0U
#endif

static uint32_t libresteam_shim_hit_log(libresteam_shim_iface_t *iface, uint32_t idx, const char *tag)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    const char *name = LIBRESTEAM_SHIM_STR_EMPTY;
    uint32_t kind = LIBRESTEAM_SHIM_RET_INT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!iface || !tag)) { return LDG_ERR_FUNC_ARG_NULL; }

    ctx = (libresteam_shim_ctx_t *)iface->ctx;
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_NOT_INIT; }

    if (LDG_UNLIKELY(iface->id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LDG_ERR_RANGE; }

    if (LDG_UNLIKELY(idx >= ctx->def[iface->id].method_cunt)) { return LDG_ERR_RANGE; }

    ctx->hit[iface->id][idx] += LIBRESTEAM_SHIM_HIT_STEP;
    if (ctx->hit[iface->id][idx] != LIBRESTEAM_SHIM_HIT_FIRST) { return LDG_ERR_AOK; }

    res = ctx->def[iface->id].name_get(idx, &name);
    if (res != LDG_ERR_AOK) { name = LIBRESTEAM_SHIM_STR_EMPTY; }

    res = ctx->def[iface->id].kind_get(idx, &kind);
    if (res != LDG_ERR_AOK) { kind = LIBRESTEAM_SHIM_RET_INT; }

    return ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_LOG_LVL, "%s; iface: %s; idx: %u32; method: %s; kind: %u32", tag, ctx->def[iface->id].version, idx, name, kind);
}

uint32_t libresteam_shim_call_note(libresteam_shim_iface_t *iface, uint32_t idx)
{
    if (LDG_UNLIKELY(!iface)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!iface->ctx)) { return LDG_ERR_NOT_INIT; }

    return libresteam_shim_hit_log(iface, idx, LIBRESTEAM_SHIM_TAG_CALL);
}

uint32_t libresteam_shim_set_get(void *self, uint32_t idx, const libresteam_backend_set_t **out)
{
    libresteam_shim_iface_t *iface = (libresteam_shim_iface_t *)self;
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!iface)) { return LDG_ERR_FUNC_ARG_NULL; }

    ctx = (libresteam_shim_ctx_t *)iface->ctx;
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_NOT_INIT; }

    if (LDG_UNLIKELY(ctx->steam_up != LDG_TRUTH_TRUE)) { return LDG_ERR_NOT_INIT; }

    if (LDG_UNLIKELY(iface->id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LDG_ERR_RANGE; }

    if (LDG_UNLIKELY(idx >= ctx->def[iface->id].method_cunt)) { return LDG_ERR_RANGE; }

    res = libresteam_shim_hit_log(iface, idx, LIBRESTEAM_SHIM_TAG_CALL);
    *out = &ctx->set;
    if (res != LDG_ERR_AOK) { return LDG_ERR_AOK; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_arg_log(void *self, uint32_t idx, uint32_t res, const char *fmt, ...)
{
    libresteam_shim_iface_t *iface = (libresteam_shim_iface_t *)self;
    libresteam_shim_ctx_t *ctx = 0x0;
    char body[LDG_LOGGING_LINE_CAP + LDG_STR_TERM_SIZE] = LDG_ARR_ZERO_INIT;
    const char *name = LIBRESTEAM_SHIM_STR_EMPTY;
    va_list args = LDG_STRUCT_ZERO_INIT;
    uint64_t len = 0;
    uint32_t fmt_res = LDG_ERR_AOK;
    uint32_t log_res = LDG_ERR_AOK;

    if (LIBRESTEAM_SHIM_ARG_LOG_EN != LDG_TRUTH_TRUE) { return res; }

    if (LDG_UNLIKELY(!iface || !fmt)) { return res; }

    ctx = (libresteam_shim_ctx_t *)iface->ctx;
    if (LDG_UNLIKELY(!ctx || iface->id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return res; }

    if (LDG_UNLIKELY(idx >= ctx->def[iface->id].method_cunt)) { return res; }

    if (ctx->def[iface->id].name_get(idx, &name) != LDG_ERR_AOK) { name = LIBRESTEAM_SHIM_STR_EMPTY; }

    va_start(args, fmt);
    fmt_res = ldg_str_variable_fmt(body, (uint64_t)sizeof(body), &len, fmt, args);
    va_end(args);
    if (fmt_res != LDG_ERR_AOK && fmt_res != LDG_ERR_STR_TRUNC) { return res; }

    log_res = ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_LOG_LVL, "arg; iface: %s; method: %s; %s; res: %u32", ctx->def[iface->id].version, name, body, res);
    if (LDG_UNLIKELY(log_res != LDG_ERR_AOK)) { return res; }

    return res;
}

uint64_t libresteam_shim_stub_hit(void *self, uint32_t idx)
{
    libresteam_shim_iface_t *iface = (libresteam_shim_iface_t *)self;
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t kind = LIBRESTEAM_SHIM_RET_INT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!iface)) { return LIBRESTEAM_SHIM_STUB_RET_NONE; }

    ctx = (libresteam_shim_ctx_t *)iface->ctx;
    if (LDG_UNLIKELY(!ctx)) { return LIBRESTEAM_SHIM_STUB_RET_NONE; }

    if (LDG_UNLIKELY(iface->id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LIBRESTEAM_SHIM_STUB_RET_NONE; }

    if (LDG_UNLIKELY(idx >= ctx->def[iface->id].method_cunt)) { return LIBRESTEAM_SHIM_STUB_RET_NONE; }

    res = libresteam_shim_hit_log(iface, idx, LIBRESTEAM_SHIM_TAG_STUB);
    if (res != LDG_ERR_AOK) { kind = LIBRESTEAM_SHIM_RET_INT; }

    res = ctx->def[iface->id].kind_get(idx, &kind);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_STUB_RET_NONE; }

    if (kind == LIBRESTEAM_SHIM_RET_STR) { return (uint64_t)(uintptr_t)LIBRESTEAM_SHIM_STR_EMPTY; }

    return LIBRESTEAM_SHIM_STUB_RET_NONE;
}

uint32_t libresteam_shim_iface_def_get(uint32_t id, libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    switch (id)
    {
        case LIBRESTEAM_SHIM_IFACE_USER: { return libresteam_shim_user_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_FRIENDS: { return libresteam_shim_friends_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_USER_STATS: { return libresteam_shim_user_stats_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_REMOTE_STORAGE: { return libresteam_shim_remote_storage_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_APPS: { return libresteam_shim_apps_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_UTILS: { return libresteam_shim_utils_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_MATCHMAKING: { return libresteam_shim_matchmaking_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_MATCHMAKING_SRVS: { return libresteam_shim_matchmaking_srvs_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING: { return libresteam_shim_networking_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_SOCKS: { return libresteam_shim_networking_socks_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_UTILS: { return libresteam_shim_networking_utils_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_SOCKS_012: { return libresteam_shim_networking_socks_012_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_UTILS_004: { return libresteam_shim_networking_utils_004_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_MSGS: { return libresteam_shim_networking_msgs_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_CLIENT: { return libresteam_shim_client_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_REMOTE_PLAY_001: { return libresteam_shim_remote_play_001_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_REMOTE_PLAY_004: { return libresteam_shim_remote_play_004_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_USER_020: { return libresteam_shim_user_020_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_CLIENT_019: { return libresteam_shim_client_019_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_USER_STATS_011: { return libresteam_shim_user_stats_011_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_UTILS_009: { return libresteam_shim_utils_009_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_NETWORKING_005: { return libresteam_shim_networking_005_def_get(out);
        }
        case LIBRESTEAM_SHIM_IFACE_CONTROLLER_007: { return libresteam_shim_controller_007_def_get(out);
        }
        default: { break;
        }
    }

    if (LDG_UNLIKELY(id < LIBRESTEAM_SHIM_IFACE_GEN_BASE || id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LDG_ERR_RANGE; }

    return libresteam_shim_gen_def_get(id - LIBRESTEAM_SHIM_IFACE_GEN_BASE, out);
}

static uint32_t libresteam_shim_name_id_fill(libresteam_shim_ctx_t *ctx, uint32_t id)
{
    const libresteam_shim_iface_def_t *def = 0x0;
    const char *name = 0x0;
    const char *prev = 0x0;
    uint32_t idx = 0;
    uint32_t prev_idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LDG_ERR_RANGE; }

    def = &ctx->def[id];

    for (idx = 0; idx < def->method_cunt; idx++)
    {
        ctx->name_id[id][idx] = idx;

        res = def->name_get(idx, &name);
        if (res != LDG_ERR_AOK) { return res; }

        for (prev_idx = 0; prev_idx < idx; prev_idx++)
        {
            res = def->name_get(prev_idx, &prev);
            if (res != LDG_ERR_AOK) { return res; }

            if (ldg_parse_streq_is(name, prev) == LDG_TRUTH_TRUE) { ctx->name_id[id][idx] = prev_idx; break; }
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_vtable_fill(libresteam_shim_ctx_t *ctx, uint32_t id)
{
    const libresteam_shim_iface_def_t *def = 0x0;
    void *fn = 0x0;
    uint32_t idx = 0;
    uint32_t slot = 0;
    uint32_t kind = LIBRESTEAM_SHIM_RET_INT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(id >= LIBRESTEAM_SHIM_IFACE_CUNT)) { return LDG_ERR_RANGE; }

    def = &ctx->def[id];

    for (idx = 0; idx < def->method_cunt; idx++)
    {
        res = ldg_cpp_slot_get(&ctx->desc[id], idx, &slot);
        if (res != LDG_ERR_AOK) { return res; }

        if (LDG_UNLIKELY(slot >= LIBRESTEAM_SHIM_SLOT_MAX)) { return LDG_ERR_RANGE; }

        res = def->impl_get(idx, &fn);
        if (res != LDG_ERR_AOK) { return res; }

        if (!fn)
        {
            res = def->kind_get(idx, &kind);
            if (res != LDG_ERR_AOK) { return res; }

            if (kind == LIBRESTEAM_SHIM_RET_STRUCT) { res = libresteam_shim_stub_sid_get(idx, &fn); }
            else { res = libresteam_shim_stub_get(idx, &fn); }

            if (res != LDG_ERR_AOK) { return res; }
        }

        ctx->vtable[id][slot] = fn;
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_iface_build(libresteam_shim_ctx_t *ctx)
{
    uint32_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (id = 0; id < LIBRESTEAM_SHIM_IFACE_CUNT; id++)
    {
        res = libresteam_shim_iface_def_get(id, &ctx->def[id]);
        if (res != LDG_ERR_AOK) { return res; }

        if (LDG_UNLIKELY(!ctx->def[id].version || !ctx->def[id].name_get || !ctx->def[id].kind_get || !ctx->def[id].impl_get)) { return LDG_ERR_NOT_INIT; }

        if (LDG_UNLIKELY(ctx->def[id].method_cunt > LIBRESTEAM_SHIM_SLOT_MAX)) { return LDG_ERR_RANGE; }

        res = libresteam_shim_name_id_fill(ctx, id);
        if (res != LDG_ERR_AOK) { return res; }

        ctx->desc[id].name_id = ctx->name_id[id];
        ctx->desc[id].method_cunt = ctx->def[id].method_cunt;
        ctx->desc[id].dtor_idx = LDG_CPP_DTOR_NONE;

        res = libresteam_shim_vtable_fill(ctx, id);
        if (res != LDG_ERR_AOK) { return res; }

        ctx->iface[id].vtable = ctx->vtable[id];
        ctx->iface[id].ctx = ctx;
        ctx->iface[id].id = id;
    }

    ctx->cb_name_id[LIBRESTEAM_SHIM_CB_RUN] = LIBRESTEAM_SHIM_CB_RUN;
    ctx->cb_name_id[LIBRESTEAM_SHIM_CB_RUN_CALL] = LIBRESTEAM_SHIM_CB_RUN;
    ctx->cb_name_id[LIBRESTEAM_SHIM_CB_SIZE_GET] = LIBRESTEAM_SHIM_CB_SIZE_GET;
    ctx->cb_desc.name_id = ctx->cb_name_id;
    ctx->cb_desc.method_cunt = LIBRESTEAM_SHIM_CB_METHOD_CUNT;
    ctx->cb_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    return libresteam_shim_networking_socks_ctx_fill(ctx);
}

uint32_t libresteam_shim_iface_find(libresteam_shim_ctx_t *ctx, const char *version, void **out)
{
    uint32_t id = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx || !version)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (id = 0; id < LIBRESTEAM_SHIM_IFACE_CUNT; id++) { if (ldg_parse_streq_is(ctx->def[id].version, version) == LDG_TRUTH_TRUE)
        {
            *out = &ctx->iface[id];
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_NOT_FOUND;
}
