#ifndef LIBRESTEAM_USER_STATS_H
#define LIBRESTEAM_USER_STATS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user_stats/ops.h>

LDG_EXPORT uint32_t libresteam_user_stats_stat_int_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_stat_int_set(const libresteam_backend_set_t *set, const char *name, uint32_t val);
LDG_EXPORT uint32_t libresteam_user_stats_stat_fp64_get(const libresteam_backend_set_t *set, const char *name, double *out);
LDG_EXPORT uint32_t libresteam_user_stats_stat_fp64_set(const libresteam_backend_set_t *set, const char *name, double val);
LDG_EXPORT uint32_t libresteam_user_stats_avg_rate_stat_update(const libresteam_backend_set_t *set, const char *name, double cunt_sesh, double sesh_len);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_set(const libresteam_backend_set_t *set, const char *name);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_clear(const libresteam_backend_set_t *set, const char *name);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_unlock_time_get(const libresteam_backend_set_t *set, const char *name, uint8_t *achieved_out, uint32_t *time_out);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_progress_indicate(const libresteam_backend_set_t *set, const char *name, uint32_t curr, uint32_t max);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_name_get(const libresteam_backend_set_t *set, uint32_t idx, const char **out);
LDG_EXPORT uint32_t libresteam_user_stats_stats_st(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_user_stats_stats_rst(const libresteam_backend_set_t *set, uint8_t achievements_too);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_icon_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_display_attr_get(const libresteam_backend_set_t *set, const char *name, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_user_stats_user_stats_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_user_stat_int_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_user_stat_fp64_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, double *out);
LDG_EXPORT uint32_t libresteam_user_stats_user_achievement_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_user_achievement_unlock_time_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *name, uint8_t *achieved_out, uint32_t *time_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_find_or_create(const libresteam_backend_set_t *set, const char *name, uint32_t sort, uint32_t display, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_find(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_name_get(const libresteam_backend_set_t *set, uint64_t lboard, const char **out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_entry_cunt_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_sort_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_display_get(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_entries_download(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t req_type, uint32_t range_start, uint32_t range_end, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_entries_users_download(const libresteam_backend_set_t *set, uint64_t lboard, libresteam_steam_id_t *users, uint32_t cunt, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_downloaded_entry_get(const libresteam_backend_set_t *set, uint64_t entries, uint32_t idx, libresteam_user_stats_lboard_entry_t *out, uint32_t *details, uint32_t details_max);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_score_upload(const libresteam_backend_set_t *set, uint64_t lboard, uint32_t method, uint32_t score, const uint32_t *details, uint32_t detail_cunt, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_lboard_ugc_attach(const libresteam_backend_set_t *set, uint64_t lboard, uint64_t ugc, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_current_players_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_achievement_percent_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_most_achieved_get(const libresteam_backend_set_t *set, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out);
LDG_EXPORT uint32_t libresteam_user_stats_next_most_achieved_get(const libresteam_backend_set_t *set, uint32_t previous, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out);
LDG_EXPORT uint32_t libresteam_user_stats_achievement_percent_get(const libresteam_backend_set_t *set, const char *name, double *out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_stats_req(const libresteam_backend_set_t *set, uint32_t history_days, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_stat_int64_get(const libresteam_backend_set_t *set, const char *name, uint64_t *out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_stat_fp64_get(const libresteam_backend_set_t *set, const char *name, double *out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_stat_history_int64_get(const libresteam_backend_set_t *set, const char *name, uint64_t *data, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_user_stats_glob_stat_history_fp64_get(const libresteam_backend_set_t *set, const char *name, double *data, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_user_stats_progress_limit_int_get(const libresteam_backend_set_t *set, const char *name, uint32_t *min_out, uint32_t *max_out);
LDG_EXPORT uint32_t libresteam_user_stats_progress_limit_fp64_get(const libresteam_backend_set_t *set, const char *name, double *min_out, double *max_out);

#endif
