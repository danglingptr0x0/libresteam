#ifndef LIBRESTEAM_USER_STATS_OPS_H
#define LIBRESTEAM_USER_STATS_OPS_H

#include <stdint.h>

#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_user_stats_lboard_entry
{
    libresteam_steam_id_t user;
    uint32_t global_rank;
    uint32_t score;
    uint32_t detail_cunt;
    uint8_t pudding[4];
    uint64_t ugc;
} libresteam_user_stats_lboard_entry_t;

typedef struct libresteam_user_stats_ext_ops
{
    uint32_t (*achievement_icon_get)(void *ctx, const char *name, uint32_t *out);
    uint32_t (*achievement_display_attr_get)(void *ctx, const char *name, const char *key, const char **out);
    uint32_t (*user_stats_req)(void *ctx, libresteam_steam_id_t id, uint64_t *call_out);
    uint32_t (*user_stat_int_get)(void *ctx, libresteam_steam_id_t id, const char *name, uint32_t *out);
    uint32_t (*user_stat_fp64_get)(void *ctx, libresteam_steam_id_t id, const char *name, double *out);
    uint32_t (*user_achievement_get)(void *ctx, libresteam_steam_id_t id, const char *name, uint8_t *out);
    uint32_t (*user_achievement_unlock_time_get)(void *ctx, libresteam_steam_id_t id, const char *name, uint8_t *achieved_out, uint32_t *time_out);
    uint32_t (*lboard_find_or_create)(void *ctx, const char *name, uint32_t sort, uint32_t display, uint64_t *call_out);
    uint32_t (*lboard_find)(void *ctx, const char *name, uint64_t *call_out);
    uint32_t (*lboard_name_get)(void *ctx, uint64_t lboard, const char **out);
    uint32_t (*lboard_entry_cunt_get)(void *ctx, uint64_t lboard, uint32_t *out);
    uint32_t (*lboard_sort_get)(void *ctx, uint64_t lboard, uint32_t *out);
    uint32_t (*lboard_display_get)(void *ctx, uint64_t lboard, uint32_t *out);
    uint32_t (*lboard_entries_download)(void *ctx, uint64_t lboard, uint32_t req_type, uint32_t range_start, uint32_t range_end, uint64_t *call_out);
    uint32_t (*lboard_entries_users_download)(void *ctx, uint64_t lboard, libresteam_steam_id_t *users, uint32_t cunt, uint64_t *call_out);
    uint32_t (*lboard_downloaded_entry_get)(void *ctx, uint64_t entries, uint32_t idx, libresteam_user_stats_lboard_entry_t *out, uint32_t *details, uint32_t details_max);
    uint32_t (*lboard_score_upload)(void *ctx, uint64_t lboard, uint32_t method, uint32_t score, const uint32_t *details, uint32_t detail_cunt, uint64_t *call_out);
    uint32_t (*lboard_ugc_attach)(void *ctx, uint64_t lboard, uint64_t ugc, uint64_t *call_out);
    uint32_t (*current_players_req)(void *ctx, uint64_t *call_out);
    uint32_t (*glob_achievement_percent_req)(void *ctx, uint64_t *call_out);
    uint32_t (*most_achieved_get)(void *ctx, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out);
    uint32_t (*next_most_achieved_get)(void *ctx, uint32_t previous, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out);
    uint32_t (*achievement_percent_get)(void *ctx, const char *name, double *out);
    uint32_t (*glob_stats_req)(void *ctx, uint32_t history_days, uint64_t *call_out);
    uint32_t (*glob_stat_int64_get)(void *ctx, const char *name, uint64_t *out);
    uint32_t (*glob_stat_fp64_get)(void *ctx, const char *name, double *out);
    uint32_t (*glob_stat_history_int64_get)(void *ctx, const char *name, uint64_t *data, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*glob_stat_history_fp64_get)(void *ctx, const char *name, double *data, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*progress_limit_int_get)(void *ctx, const char *name, uint32_t *min_out, uint32_t *max_out);
    uint32_t (*progress_limit_fp64_get)(void *ctx, const char *name, double *min_out, double *max_out);
} libresteam_user_stats_ext_ops_t;

typedef struct libresteam_user_stats_ops
{
    uint32_t (*stat_int_get)(void *ctx, const char *name, uint32_t *out);
    uint32_t (*stat_int_set)(void *ctx, const char *name, uint32_t val);
    uint32_t (*stat_fp64_get)(void *ctx, const char *name, double *out);
    uint32_t (*stat_fp64_set)(void *ctx, const char *name, double val);
    uint32_t (*avg_rate_stat_update)(void *ctx, const char *name, double cunt_sesh, double sesh_len);
    uint32_t (*achievement_get)(void *ctx, const char *name, uint8_t *out);
    uint32_t (*achievement_set)(void *ctx, const char *name);
    uint32_t (*achievement_clear)(void *ctx, const char *name);
    uint32_t (*achievement_unlock_time_get)(void *ctx, const char *name, uint8_t *achieved_out, uint32_t *time_out);
    uint32_t (*achievement_progress_indicate)(void *ctx, const char *name, uint32_t curr, uint32_t max);
    uint32_t (*achievement_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*achievement_name_get)(void *ctx, uint32_t idx, const char **out);
    uint32_t (*stats_st)(void *ctx);
    uint32_t (*stats_rst)(void *ctx, uint8_t achievements_too);
    const libresteam_user_stats_ext_ops_t *ext;
} libresteam_user_stats_ops_t;

#endif
