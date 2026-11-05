#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user_stats/ops.h>
#include <libresteam/user_stats/user_stats.h>

static uint32_t user_stats_ext_ops_get(const libresteam_backend_set_t *set, const libresteam_user_stats_ext_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->user_stats || !primary->ops->user_stats->ext) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->user_stats->ext;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_user_stats_achievement_icon_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_icon_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_icon_get(ctx, name, out);
}

uint32_t libresteam_user_stats_achievement_display_attr_get(const libresteam_backend_set_t *set, const char *name, const char *key, const char **out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_display_attr_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_display_attr_get(ctx, name, key, out);
}

uint32_t libresteam_user_stats_user_stats_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_stats_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_stats_req(ctx, id, call_out);
}

uint32_t libresteam_user_stats_user_stat_int_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint32_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_stat_int_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_stat_int_get(ctx, id, name, out);
}

uint32_t libresteam_user_stats_user_stat_fp64_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, double *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_stat_fp64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_stat_fp64_get(ctx, id, name, out);
}

uint32_t libresteam_user_stats_user_achievement_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint8_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_achievement_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_achievement_get(ctx, id, name, out);
}

uint32_t libresteam_user_stats_user_achievement_unlock_time_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!achieved_out || !time_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *achieved_out = LDG_TRUTH_FALSE;
    *time_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_achievement_unlock_time_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_achievement_unlock_time_get(ctx, id, name, achieved_out, time_out);
}

uint32_t libresteam_user_stats_lboard_find_or_create(const libresteam_backend_set_t *set, const char *name, uint32_t sort, uint32_t display, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_find_or_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_find_or_create(ctx, name, sort, display, call_out);
}

uint32_t libresteam_user_stats_lboard_find(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_find) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_find(ctx, name, call_out);
}

uint32_t libresteam_user_stats_lboard_name_get(const libresteam_backend_set_t *set, uint64_t lboard, const char **out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_name_get(ctx, lboard, out);
}

uint32_t libresteam_user_stats_lboard_entry_cunt_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_entry_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_entry_cunt_get(ctx, lboard, out);
}

uint32_t libresteam_user_stats_lboard_sort_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_sort_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_sort_get(ctx, lboard, out);
}

uint32_t libresteam_user_stats_lboard_display_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_display_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_display_get(ctx, lboard, out);
}

uint32_t libresteam_user_stats_lboard_entries_download(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t req_type, uint32_t range_start, uint32_t range_end, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_entries_download) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_entries_download(ctx, lboard, req_type, range_start, range_end, call_out);
}

uint32_t libresteam_user_stats_lboard_entries_users_download(const libresteam_backend_set_t *set, uint64_t lboard, libresteam_steam_id_t *users, uint32_t cunt, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_entries_users_download) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_entries_users_download(ctx, lboard, users, cunt, call_out);
}

uint32_t libresteam_user_stats_lboard_downloaded_entry_get(const libresteam_backend_set_t *set, uint64_t entries, uint32_t idx, libresteam_user_stats_lboard_entry_t *out, uint32_t *details, uint32_t details_max)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_downloaded_entry_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_downloaded_entry_get(ctx, entries, idx, out, details, details_max);
}

uint32_t libresteam_user_stats_lboard_score_upload(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t method, uint32_t score, const uint32_t *details, uint32_t detail_cunt, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!details && detail_cunt != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_score_upload) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_score_upload(ctx, lboard, method, score, details, detail_cunt, call_out);
}

uint32_t libresteam_user_stats_lboard_ugc_attach(const libresteam_backend_set_t *set, uint64_t lboard, uint64_t ugc, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->lboard_ugc_attach) { return LDG_ERR_UNSUPPORTED; }

    return ops->lboard_ugc_attach(ctx, lboard, ugc, call_out);
}

uint32_t libresteam_user_stats_current_players_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->current_players_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->current_players_req(ctx, call_out);
}

uint32_t libresteam_user_stats_glob_achievement_percent_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_achievement_percent_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_achievement_percent_req(ctx, call_out);
}

uint32_t libresteam_user_stats_most_achieved_get(const libresteam_backend_set_t *set, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!percent_out || !achieved_out || !idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *percent_out = 0;
    *achieved_out = LDG_TRUTH_FALSE;
    *idx_out = UINT32_MAX;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->most_achieved_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->most_achieved_get(ctx, name, cap, percent_out, achieved_out, idx_out);
}

uint32_t libresteam_user_stats_next_most_achieved_get(const libresteam_backend_set_t *set, uint32_t previous, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!percent_out || !achieved_out || !idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *percent_out = 0;
    *achieved_out = LDG_TRUTH_FALSE;
    *idx_out = UINT32_MAX;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->next_most_achieved_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->next_most_achieved_get(ctx, previous, name, cap, percent_out, achieved_out, idx_out);
}

uint32_t libresteam_user_stats_achievement_percent_get(const libresteam_backend_set_t *set, const char *name, double *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->achievement_percent_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->achievement_percent_get(ctx, name, out);
}

uint32_t libresteam_user_stats_glob_stats_req(const libresteam_backend_set_t *set, uint32_t history_days, uint64_t *call_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_stats_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_stats_req(ctx, history_days, call_out);
}

uint32_t libresteam_user_stats_glob_stat_int64_get(const libresteam_backend_set_t *set, const char *name, uint64_t *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_stat_int64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_stat_int64_get(ctx, name, out);
}

uint32_t libresteam_user_stats_glob_stat_fp64_get(const libresteam_backend_set_t *set, const char *name, double *out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_stat_fp64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_stat_fp64_get(ctx, name, out);
}

uint32_t libresteam_user_stats_glob_stat_history_int64_get(const libresteam_backend_set_t *set, const char *name, uint64_t *data, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_stat_history_int64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_stat_history_int64_get(ctx, name, data, cap, cunt_out);
}

uint32_t libresteam_user_stats_glob_stat_history_fp64_get(const libresteam_backend_set_t *set, const char *name, double *data, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->glob_stat_history_fp64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->glob_stat_history_fp64_get(ctx, name, data, cap, cunt_out);
}

uint32_t libresteam_user_stats_progress_limit_int_get(const libresteam_backend_set_t *set, const char *name, uint32_t *min_out, uint32_t *max_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!min_out || !max_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *min_out = 0;
    *max_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->progress_limit_int_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->progress_limit_int_get(ctx, name, min_out, max_out);
}

uint32_t libresteam_user_stats_progress_limit_fp64_get(const libresteam_backend_set_t *set, const char *name, double *min_out, double *max_out)
{
    const libresteam_user_stats_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!min_out || !max_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *min_out = 0;
    *max_out = 0;

    res = user_stats_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->progress_limit_fp64_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->progress_limit_fp64_get(ctx, name, min_out, max_out);
}
