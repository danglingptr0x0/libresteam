#ifndef LIBRESTEAM_UTILS_H
#define LIBRESTEAM_UTILS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/utils/ops.h>

LDG_EXPORT uint32_t libresteam_utils_app_active_secs_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_computer_active_secs_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_universe_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_srv_time_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_ip_country_get(const libresteam_backend_set_t *set, const char **out);
LDG_EXPORT uint32_t libresteam_utils_img_size_get(const libresteam_backend_set_t *set, uint32_t img, uint32_t *width_out, uint32_t *height_out);
LDG_EXPORT uint32_t libresteam_utils_img_rgba_get(const libresteam_backend_set_t *set, uint32_t img, uint8_t *dst, uint32_t cap);
LDG_EXPORT uint32_t libresteam_utils_battery_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_app_id_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_notification_pos_set(const libresteam_backend_set_t *set, uint32_t pos);
LDG_EXPORT uint32_t libresteam_utils_call_completed_get(const libresteam_backend_set_t *set, uint64_t call, uint8_t *done_out, uint8_t *failed_out);
LDG_EXPORT uint32_t libresteam_utils_call_failure_get(const libresteam_backend_set_t *set, uint64_t call, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_ipc_call_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_warning_hook_set(const libresteam_backend_set_t *set, void *hook);
LDG_EXPORT uint32_t libresteam_utils_overlay_en_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_overlay_present_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_file_sig_ck(const libresteam_backend_set_t *set, const char *name, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_show(const libresteam_backend_set_t *set, uint32_t mode, uint32_t line_mode, const char *desc, uint32_t char_max, const char *existing);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_len_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_get(const libresteam_backend_set_t *set, char *text, uint32_t cap);
LDG_EXPORT uint32_t libresteam_utils_ui_lang_get(const libresteam_backend_set_t *set, const char **out);
LDG_EXPORT uint32_t libresteam_utils_vr_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_notification_inset_set(const libresteam_backend_set_t *set, uint32_t horizontal, uint32_t vertical);
LDG_EXPORT uint32_t libresteam_utils_big_picture_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_vr_dashboard_start(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_utils_vr_streaming_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_vr_streaming_set(const libresteam_backend_set_t *set, uint8_t en);
LDG_EXPORT uint32_t libresteam_utils_china_launcher_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_text_filter_init(const libresteam_backend_set_t *set, uint32_t opts);
LDG_EXPORT uint32_t libresteam_utils_text_filter(const libresteam_backend_set_t *set, uint32_t filter_ctx, libresteam_steam_id_t src, const char *text, char *out, uint32_t cap, uint32_t *filtered_out);
LDG_EXPORT uint32_t libresteam_utils_ipv6_state_get(const libresteam_backend_set_t *set, uint32_t proto, uint32_t *out);
LDG_EXPORT uint32_t libresteam_utils_steam_deck_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_float_show(const libresteam_backend_set_t *set, uint32_t mode, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height);
LDG_EXPORT uint32_t libresteam_utils_launcher_mode_set(const libresteam_backend_set_t *set, uint8_t en);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_float_dismiss(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_utils_gamepad_text_dismiss(const libresteam_backend_set_t *set);

#endif
