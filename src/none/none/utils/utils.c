#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/utils/ops.h>
#include <libresteam/utils/utils.h>

static uint32_t utils_ops_get(const libresteam_backend_set_t *set, const libresteam_utils_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->utils) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->utils;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_utils_app_active_secs_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_active_secs_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_active_secs_get(ctx, out);
}

uint32_t libresteam_utils_computer_active_secs_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->computer_active_secs_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->computer_active_secs_get(ctx, out);
}

uint32_t libresteam_utils_universe_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->universe_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->universe_get(ctx, out);
}

uint32_t libresteam_utils_srv_time_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->srv_time_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->srv_time_get(ctx, out);
}

uint32_t libresteam_utils_ip_country_get(const libresteam_backend_set_t *set, const char **out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ip_country_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ip_country_get(ctx, out);
}

uint32_t libresteam_utils_img_size_get(const libresteam_backend_set_t *set, uint32_t img, uint32_t *width_out, uint32_t *height_out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->img_size_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->img_size_get(ctx, img, width_out, height_out);
}

uint32_t libresteam_utils_img_rgba_get(const libresteam_backend_set_t *set, uint32_t img, uint8_t *dst, uint32_t cap)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->img_rgba_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->img_rgba_get(ctx, img, dst, cap);
}

uint32_t libresteam_utils_battery_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->battery_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->battery_get(ctx, out);
}

uint32_t libresteam_utils_app_id_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_id_get(ctx, out);
}

uint32_t libresteam_utils_notification_pos_set(const libresteam_backend_set_t *set, uint32_t pos)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->notification_pos_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->notification_pos_set(ctx, pos);
}

uint32_t libresteam_utils_call_completed_get(const libresteam_backend_set_t *set, uint64_t call, uint8_t *done_out, uint8_t *failed_out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->call_completed_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->call_completed_get(ctx, call, done_out, failed_out);
}

uint32_t libresteam_utils_call_failure_get(const libresteam_backend_set_t *set, uint64_t call, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->call_failure_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->call_failure_get(ctx, call, out);
}

uint32_t libresteam_utils_ipc_call_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ipc_call_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ipc_call_cunt_get(ctx, out);
}

uint32_t libresteam_utils_warning_hook_set(const libresteam_backend_set_t *set, void *hook)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->warning_hook_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->warning_hook_set(ctx, hook);
}

uint32_t libresteam_utils_overlay_en_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_en_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_en_get(ctx, out);
}

uint32_t libresteam_utils_overlay_present_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_present_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_present_get(ctx, out);
}

uint32_t libresteam_utils_file_sig_ck(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_sig_ck) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_sig_ck(ctx, name, call_out);
}

uint32_t libresteam_utils_gamepad_text_show(const libresteam_backend_set_t *set, uint32_t mode, uint32_t line_mode, const char *desc, uint32_t char_max, const char *existing)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_show) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_show(ctx, mode, line_mode, desc, char_max, existing);
}

uint32_t libresteam_utils_gamepad_text_len_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_len_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_len_get(ctx, out);
}

uint32_t libresteam_utils_gamepad_text_get(const libresteam_backend_set_t *set, char *text, uint32_t cap)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_get(ctx, text, cap);
}

uint32_t libresteam_utils_ui_lang_get(const libresteam_backend_set_t *set, const char **out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ui_lang_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ui_lang_get(ctx, out);
}

uint32_t libresteam_utils_vr_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vr_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->vr_get(ctx, out);
}

uint32_t libresteam_utils_notification_inset_set(const libresteam_backend_set_t *set, uint32_t horizontal, uint32_t vertical)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->notification_inset_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->notification_inset_set(ctx, horizontal, vertical);
}

uint32_t libresteam_utils_big_picture_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->big_picture_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->big_picture_get(ctx, out);
}

uint32_t libresteam_utils_vr_dashboard_start(const libresteam_backend_set_t *set)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vr_dashboard_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->vr_dashboard_start(ctx);
}

uint32_t libresteam_utils_vr_streaming_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vr_streaming_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->vr_streaming_get(ctx, out);
}

uint32_t libresteam_utils_vr_streaming_set(const libresteam_backend_set_t *set, uint8_t en)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vr_streaming_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->vr_streaming_set(ctx, en);
}

uint32_t libresteam_utils_china_launcher_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->china_launcher_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->china_launcher_get(ctx, out);
}

uint32_t libresteam_utils_text_filter_init(const libresteam_backend_set_t *set, uint32_t opts)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->text_filter_init) { return LDG_ERR_UNSUPPORTED; }

    return ops->text_filter_init(ctx, opts);
}

uint32_t libresteam_utils_text_filter(const libresteam_backend_set_t *set, uint32_t filter_ctx, libresteam_steam_id_t src, const char *text, char *out, uint32_t cap, uint32_t *filtered_out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->text_filter) { return LDG_ERR_UNSUPPORTED; }

    return ops->text_filter(ctx, filter_ctx, src, text, out, cap, filtered_out);
}

uint32_t libresteam_utils_ipv6_state_get(const libresteam_backend_set_t *set, uint32_t proto, uint32_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ipv6_state_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ipv6_state_get(ctx, proto, out);
}

uint32_t libresteam_utils_steam_deck_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->steam_deck_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->steam_deck_get(ctx, out);
}

uint32_t libresteam_utils_gamepad_text_float_show(const libresteam_backend_set_t *set, uint32_t mode, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_float_show) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_float_show(ctx, mode, pos_x, pos_y, width, height);
}

uint32_t libresteam_utils_launcher_mode_set(const libresteam_backend_set_t *set, uint8_t en)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->launcher_mode_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->launcher_mode_set(ctx, en);
}

uint32_t libresteam_utils_gamepad_text_float_dismiss(const libresteam_backend_set_t *set)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_float_dismiss) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_float_dismiss(ctx);
}

uint32_t libresteam_utils_gamepad_text_dismiss(const libresteam_backend_set_t *set)
{
    const libresteam_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->gamepad_text_dismiss) { return LDG_ERR_UNSUPPORTED; }

    return ops->gamepad_text_dismiss(ctx);
}
