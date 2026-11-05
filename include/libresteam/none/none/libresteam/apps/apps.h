#ifndef LIBRESTEAM_APPS_H
#define LIBRESTEAM_APPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/apps/ops.h>
#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>

LDG_EXPORT uint32_t libresteam_apps_subscribed_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_low_violence_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_cybercafe_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_vac_banned_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_game_lang_get(const libresteam_backend_set_t *set, const char **out);
LDG_EXPORT uint32_t libresteam_apps_avail_lang_get(const libresteam_backend_set_t *set, const char **out);
LDG_EXPORT uint32_t libresteam_apps_app_subscribed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_dlc_installed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_purchase_time_get(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_apps_free_weekend_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_dlc_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_apps_dlc_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *app_id_out, uint8_t *avail_out, char *name, uint32_t name_cap);
LDG_EXPORT uint32_t libresteam_apps_dlc_install(const libresteam_backend_set_t *set, uint32_t app_id);
LDG_EXPORT uint32_t libresteam_apps_dlc_uninstall(const libresteam_backend_set_t *set, uint32_t app_id);
LDG_EXPORT uint32_t libresteam_apps_purchase_key_req(const libresteam_backend_set_t *set, uint32_t app_id);
LDG_EXPORT uint32_t libresteam_apps_beta_name_get(const libresteam_backend_set_t *set, char *name, uint32_t name_cap);
LDG_EXPORT uint32_t libresteam_apps_content_corrupt_mark(const libresteam_backend_set_t *set, uint8_t missing_only);
LDG_EXPORT uint32_t libresteam_apps_depots_get(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t *depot, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_apps_install_dir_get(const libresteam_backend_set_t *set, uint32_t app_id, char *dir, uint32_t cap, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_apps_app_installed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_apps_launch_query_param_get(const libresteam_backend_set_t *set, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_apps_dlc_progress_get(const libresteam_backend_set_t *set, uint32_t app_id, uint64_t *done_out, uint64_t *total_out);
LDG_EXPORT uint32_t libresteam_apps_build_id_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_apps_purchase_key_all_req(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_apps_file_details_req(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_apps_launch_cmd_line_get(const libresteam_backend_set_t *set, char *cmd_line, uint32_t cap, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_apps_family_sharing_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_apps_timed_trial_get(const libresteam_backend_set_t *set, uint8_t *trial_out, uint32_t *allowed_out, uint32_t *played_out);
LDG_EXPORT uint32_t libresteam_apps_dlc_ctx_set(const libresteam_backend_set_t *set, uint32_t app_id);
LDG_EXPORT uint32_t libresteam_apps_beta_cunt_get(const libresteam_backend_set_t *set, uint32_t *cunt_out, uint32_t *avail_out, uint32_t *priv_out);
LDG_EXPORT uint32_t libresteam_apps_beta_info_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *flags_out, uint32_t *build_id_out, char *name, uint32_t name_cap, char *desc, uint32_t desc_cap, uint32_t *updated_out);
LDG_EXPORT uint32_t libresteam_apps_beta_active_set(const libresteam_backend_set_t *set, const char *name);

#endif
