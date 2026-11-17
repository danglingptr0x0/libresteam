#ifndef LIBRESTEAM_APPS_OPS_H
#define LIBRESTEAM_APPS_OPS_H

#include <stdint.h>

#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_apps_ops
{
    uint32_t (*subscribed_get)(void *ctx, uint8_t *out);
    uint32_t (*low_violence_get)(void *ctx, uint8_t *out);
    uint32_t (*cybercafe_get)(void *ctx, uint8_t *out);
    uint32_t (*vac_banned_get)(void *ctx, uint8_t *out);
    uint32_t (*game_lang_get)(void *ctx, const char **out);
    uint32_t (*avail_lang_get)(void *ctx, const char **out);
    uint32_t (*app_subscribed_get)(void *ctx, uint32_t app_id, uint8_t *out);
    uint32_t (*dlc_installed_get)(void *ctx, uint32_t app_id, uint8_t *out);
    uint32_t (*purchase_time_get)(void *ctx, uint32_t app_id, uint32_t *out);
    uint32_t (*free_weekend_get)(void *ctx, uint8_t *out);
    uint32_t (*dlc_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*dlc_get)(void *ctx, uint32_t idx, uint32_t *app_id_out, uint8_t *avail_out, char *name, uint32_t name_cap);
    uint32_t (*dlc_install)(void *ctx, uint32_t app_id);
    uint32_t (*dlc_uninstall)(void *ctx, uint32_t app_id);
    uint32_t (*purchase_key_req)(void *ctx, uint32_t app_id);
    uint32_t (*beta_name_get)(void *ctx, char *name, uint32_t name_cap);
    uint32_t (*content_corrupt_mark)(void *ctx, uint8_t missing_only);
    uint32_t (*depots_get)(void *ctx, uint32_t app_id, uint32_t *depot, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*install_dir_get)(void *ctx, uint32_t app_id, char *dir, uint32_t cap, uint32_t *len_out);
    uint32_t (*app_installed_get)(void *ctx, uint32_t app_id, uint8_t *out);
    uint32_t (*owner_get)(void *ctx, libresteam_steam_id_t *out);
    uint32_t (*launch_query_param_get)(void *ctx, const char *key, const char **out);
    uint32_t (*dlc_progress_get)(void *ctx, uint32_t app_id, uint64_t *done_out, uint64_t *total_out);
    uint32_t (*build_id_get)(void *ctx, uint32_t *out);
    uint32_t (*purchase_key_all_req)(void *ctx);
    uint32_t (*file_details_req)(void *ctx, const char *name, uint64_t *call_out);
    uint32_t (*launch_cmd_line_get)(void *ctx, char *cmd_line, uint32_t cap, uint32_t *len_out);
    uint32_t (*family_sharing_get)(void *ctx, uint8_t *out);
    uint32_t (*timed_trial_get)(void *ctx, uint8_t *trial_out, uint32_t *allowed_out, uint32_t *played_out);
    uint32_t (*dlc_ctx_set)(void *ctx, uint32_t app_id);
    uint32_t (*beta_cunt_get)(void *ctx, uint32_t *cunt_out, uint32_t *avail_out, uint32_t *priv_out);
    uint32_t (*beta_info_get)(void *ctx, uint32_t idx, uint32_t *flags_out, uint32_t *build_id_out, char *name, uint32_t name_cap, char *desc, uint32_t desc_cap, uint32_t *updated_out);
    uint32_t (*beta_active_set)(void *ctx, const char *name);
} libresteam_apps_ops_t;

#endif
