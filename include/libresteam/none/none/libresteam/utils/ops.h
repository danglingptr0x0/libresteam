#ifndef LIBRESTEAM_UTILS_OPS_H
#define LIBRESTEAM_UTILS_OPS_H

#include <stdint.h>

#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_utils_ops
{
    uint32_t (*app_active_secs_get)(void *ctx, uint32_t *out);
    uint32_t (*computer_active_secs_get)(void *ctx, uint32_t *out);
    uint32_t (*universe_get)(void *ctx, uint32_t *out);
    uint32_t (*srv_time_get)(void *ctx, uint32_t *out);
    uint32_t (*ip_country_get)(void *ctx, const char **out);
    uint32_t (*img_size_get)(void *ctx, uint32_t img, uint32_t *width_out, uint32_t *height_out);
    uint32_t (*img_rgba_get)(void *ctx, uint32_t img, uint8_t *dst, uint32_t cap);
    uint32_t (*battery_get)(void *ctx, uint8_t *out);
    uint32_t (*app_id_get)(void *ctx, uint32_t *out);
    uint32_t (*notification_pos_set)(void *ctx, uint32_t pos);
    uint32_t (*call_completed_get)(void *ctx, uint64_t call, uint8_t *done_out, uint8_t *failed_out);
    uint32_t (*call_failure_get)(void *ctx, uint64_t call, uint32_t *out);
    uint32_t (*ipc_call_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*warning_hook_set)(void *ctx, void *hook);
    uint32_t (*overlay_en_get)(void *ctx, uint8_t *out);
    uint32_t (*overlay_present_get)(void *ctx, uint8_t *out);
    uint32_t (*file_sig_ck)(void *ctx, const char *name, uint64_t *call_out);
    uint32_t (*gamepad_text_show)(void *ctx, uint32_t mode, uint32_t line_mode, const char *desc, uint32_t char_max, const char *existing);
    uint32_t (*gamepad_text_len_get)(void *ctx, uint32_t *out);
    uint32_t (*gamepad_text_get)(void *ctx, char *text, uint32_t cap);
    uint32_t (*ui_lang_get)(void *ctx, const char **out);
    uint32_t (*vr_get)(void *ctx, uint8_t *out);
    uint32_t (*notification_inset_set)(void *ctx, uint32_t horizontal, uint32_t vertical);
    uint32_t (*big_picture_get)(void *ctx, uint8_t *out);
    uint32_t (*vr_dashboard_start)(void *ctx);
    uint32_t (*vr_streaming_get)(void *ctx, uint8_t *out);
    uint32_t (*vr_streaming_set)(void *ctx, uint8_t en);
    uint32_t (*china_launcher_get)(void *ctx, uint8_t *out);
    uint32_t (*text_filter_init)(void *ctx, uint32_t opts);
    uint32_t (*text_filter)(void *ctx, uint32_t filter_ctx, libresteam_steam_id_t src, const char *text, char *out, uint32_t cap, uint32_t *filtered_out);
    uint32_t (*ipv6_state_get)(void *ctx, uint32_t proto, uint32_t *out);
    uint32_t (*steam_deck_get)(void *ctx, uint8_t *out);
    uint32_t (*gamepad_text_float_show)(void *ctx, uint32_t mode, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height);
    uint32_t (*launcher_mode_set)(void *ctx, uint8_t en);
    uint32_t (*gamepad_text_float_dismiss)(void *ctx);
    uint32_t (*gamepad_text_dismiss)(void *ctx);
} libresteam_utils_ops_t;

#endif
