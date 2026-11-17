#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/apps/apps.h>
#include <libresteam/apps/ops.h>
#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>

static uint32_t apps_ops_get(const libresteam_backend_set_t *set, const libresteam_apps_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->apps) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->apps;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_apps_subscribed_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->subscribed_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->subscribed_get(ctx, out);
}

uint32_t libresteam_apps_low_violence_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->low_violence_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->low_violence_get(ctx, out);
}

uint32_t libresteam_apps_cybercafe_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->cybercafe_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->cybercafe_get(ctx, out);
}

uint32_t libresteam_apps_vac_banned_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vac_banned_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->vac_banned_get(ctx, out);
}

uint32_t libresteam_apps_game_lang_get(const libresteam_backend_set_t *set, const char **out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_lang_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_lang_get(ctx, out);
}

uint32_t libresteam_apps_avail_lang_get(const libresteam_backend_set_t *set, const char **out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->avail_lang_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->avail_lang_get(ctx, out);
}

uint32_t libresteam_apps_app_subscribed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_subscribed_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_subscribed_get(ctx, app_id, out);
}

uint32_t libresteam_apps_dlc_installed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_installed_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_installed_get(ctx, app_id, out);
}

uint32_t libresteam_apps_purchase_time_get(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->purchase_time_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->purchase_time_get(ctx, app_id, out);
}

uint32_t libresteam_apps_free_weekend_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->free_weekend_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->free_weekend_get(ctx, out);
}

uint32_t libresteam_apps_dlc_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_cunt_get(ctx, out);
}

uint32_t libresteam_apps_dlc_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *app_id_out, uint8_t *avail_out, char *name, uint32_t name_cap)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_get(ctx, idx, app_id_out, avail_out, name, name_cap);
}

uint32_t libresteam_apps_dlc_install(const libresteam_backend_set_t *set, uint32_t app_id)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_install) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_install(ctx, app_id);
}

uint32_t libresteam_apps_dlc_uninstall(const libresteam_backend_set_t *set, uint32_t app_id)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_uninstall) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_uninstall(ctx, app_id);
}

uint32_t libresteam_apps_purchase_key_req(const libresteam_backend_set_t *set, uint32_t app_id)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->purchase_key_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->purchase_key_req(ctx, app_id);
}

uint32_t libresteam_apps_beta_name_get(const libresteam_backend_set_t *set, char *name, uint32_t name_cap)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->beta_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->beta_name_get(ctx, name, name_cap);
}

uint32_t libresteam_apps_content_corrupt_mark(const libresteam_backend_set_t *set, uint8_t missing_only)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->content_corrupt_mark) { return LDG_ERR_UNSUPPORTED; }

    return ops->content_corrupt_mark(ctx, missing_only);
}

uint32_t libresteam_apps_depots_get(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t *depot, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->depots_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->depots_get(ctx, app_id, depot, cap, cunt_out);
}

uint32_t libresteam_apps_install_dir_get(const libresteam_backend_set_t *set, uint32_t app_id, char *dir, uint32_t cap, uint32_t *len_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->install_dir_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->install_dir_get(ctx, app_id, dir, cap, len_out);
}

uint32_t libresteam_apps_app_installed_get(const libresteam_backend_set_t *set, uint32_t app_id, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_installed_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_installed_get(ctx, app_id, out);
}

uint32_t libresteam_apps_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->owner_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->owner_get(ctx, out);
}

uint32_t libresteam_apps_launch_query_param_get(const libresteam_backend_set_t *set, const char *key, const char **out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->launch_query_param_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->launch_query_param_get(ctx, key, out);
}

uint32_t libresteam_apps_dlc_progress_get(const libresteam_backend_set_t *set, uint32_t app_id, uint64_t *done_out, uint64_t *total_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_progress_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_progress_get(ctx, app_id, done_out, total_out);
}

uint32_t libresteam_apps_build_id_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->build_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->build_id_get(ctx, out);
}

uint32_t libresteam_apps_purchase_key_all_req(const libresteam_backend_set_t *set)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->purchase_key_all_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->purchase_key_all_req(ctx);
}

uint32_t libresteam_apps_file_details_req(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_details_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_details_req(ctx, name, call_out);
}

uint32_t libresteam_apps_launch_cmd_line_get(const libresteam_backend_set_t *set, char *cmd_line, uint32_t cap, uint32_t *len_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->launch_cmd_line_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->launch_cmd_line_get(ctx, cmd_line, cap, len_out);
}

uint32_t libresteam_apps_family_sharing_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;
    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->family_sharing_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->family_sharing_get(ctx, out);
}

uint32_t libresteam_apps_timed_trial_get(const libresteam_backend_set_t *set, uint8_t *trial_out, uint32_t *allowed_out, uint32_t *played_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->timed_trial_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->timed_trial_get(ctx, trial_out, allowed_out, played_out);
}

uint32_t libresteam_apps_dlc_ctx_set(const libresteam_backend_set_t *set, uint32_t app_id)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dlc_ctx_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->dlc_ctx_set(ctx, app_id);
}

uint32_t libresteam_apps_beta_cunt_get(const libresteam_backend_set_t *set, uint32_t *cunt_out, uint32_t *avail_out, uint32_t *priv_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->beta_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->beta_cunt_get(ctx, cunt_out, avail_out, priv_out);
}

uint32_t libresteam_apps_beta_info_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *flags_out, uint32_t *build_id_out, char *name, uint32_t name_cap, char *desc, uint32_t desc_cap, uint32_t *updated_out)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->beta_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->beta_info_get(ctx, idx, flags_out, build_id_out, name, name_cap, desc, desc_cap, updated_out);
}

uint32_t libresteam_apps_beta_active_set(const libresteam_backend_set_t *set, const char *name)
{
    const libresteam_apps_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = apps_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->beta_active_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->beta_active_set(ctx, name);
}
