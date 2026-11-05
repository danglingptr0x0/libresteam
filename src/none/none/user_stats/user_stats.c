#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/logging/logging.h>

#include <libresteam/backend/backend.h>
#include <libresteam/user_stats/ops.h>
#include <libresteam/user_stats/user_stats.h>

static uint32_t user_stats_primary_ops_get(const libresteam_backend_set_t *set, const libresteam_user_stats_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->user_stats) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->user_stats;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

static uint32_t user_stats_tee_ops_get(const libresteam_backend_set_t *set, uint32_t idx, const libresteam_user_stats_ops_t **ops_out, void **ctx_out)
{
    *ops_out = set->backend[idx].ops->user_stats;
    *ctx_out = set->backend[idx].ctx;

    if (!*ops_out) { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_user_stats_stat_int_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->stat_int_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->stat_int_get(ctx, name, out);
}

uint32_t libresteam_user_stats_stat_fp64_get(const libresteam_backend_set_t *set, const char *name, double *out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->stat_fp64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->stat_fp64_get(ctx, name, out);
}

uint32_t libresteam_user_stats_achievement_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_get(ctx, name, out);
}

uint32_t libresteam_user_stats_achievement_unlock_time_get(const libresteam_backend_set_t *set, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!achieved_out || !time_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *achieved_out = LDG_TRUTH_FALSE;
    *time_out = 0;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_unlock_time_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_unlock_time_get(ctx, name, achieved_out, time_out);
}

uint32_t libresteam_user_stats_achievement_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_cunt_get(ctx, out);
}

uint32_t libresteam_user_stats_achievement_name_get(const libresteam_backend_set_t *set, uint32_t idx, const char **out)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_name_get(ctx, idx, out);
}

uint32_t libresteam_user_stats_stat_int_set(const libresteam_backend_set_t *set, const char *name, uint32_t val)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->stat_int_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->stat_int_set(ctx, name, val); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend stat_int_set failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_stat_fp64_set(const libresteam_backend_set_t *set, const char *name, double val)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->stat_fp64_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->stat_fp64_set(ctx, name, val); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend stat fp64 set failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_avg_rate_stat_update(const libresteam_backend_set_t *set, const char *name, double cunt_sesh, double sesh_len)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->avg_rate_stat_update) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->avg_rate_stat_update(ctx, name, cunt_sesh, sesh_len); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend avg rate stat update failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_achievement_set(const libresteam_backend_set_t *set, const char *name)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->achievement_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->achievement_set(ctx, name); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend achievement set failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_achievement_clear(const libresteam_backend_set_t *set, const char *name)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->achievement_clear) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->achievement_clear(ctx, name); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend achievement clear failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_achievement_progress_indicate(const libresteam_backend_set_t *set, const char *name, uint32_t curr, uint32_t max)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->achievement_progress_indicate) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->achievement_progress_indicate(ctx, name, curr, max); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend achievement progress indicate failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_stats_st(const libresteam_backend_set_t *set)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->stats_st) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->stats_st(ctx); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend stats st failed"); }
    }

    return primary_res;
}

uint32_t libresteam_user_stats_stats_rst(const libresteam_backend_set_t *set, uint8_t achievements_too)
{
    const libresteam_user_stats_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = user_stats_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->stats_rst) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->stats_rst(ctx, achievements_too); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "user_stats: tee backend stats rst failed"); }
    }

    return primary_res;
}
