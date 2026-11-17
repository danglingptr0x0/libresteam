#ifndef LIBRESTEAM_REMOTE_PLAY_H
#define LIBRESTEAM_REMOTE_PLAY_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/remote_play/ops.h>
#include <libresteam/steam_id/steam_id.h>

LDG_EXPORT uint32_t libresteam_remote_play_sesh_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_sesh_id_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_together_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint8_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_steam_id_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_guest_id_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_avatar_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_remote_play_avatar_size_t size, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_client_name_get(const libresteam_backend_set_t *set, uint32_t sesh_id, const char **out);
LDG_EXPORT uint32_t libresteam_remote_play_form_factor_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_remote_play_form_factor_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_client_res_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t *width_out, uint32_t *height_out);
LDG_EXPORT uint32_t libresteam_remote_play_ui_show(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_remote_play_invite_send(const libresteam_backend_set_t *set, libresteam_steam_id_t friend_id);
LDG_EXPORT uint32_t libresteam_remote_play_direct_input_en(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_remote_play_direct_input_disable(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_remote_play_input_get(const libresteam_backend_set_t *set, libresteam_remote_play_input_t *input, uint32_t cap, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_mouse_visible_set(const libresteam_backend_set_t *set, uint32_t sesh_id, uint8_t visible);
LDG_EXPORT uint32_t libresteam_remote_play_mouse_pos_set(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_fp32_t pos_x, libresteam_fp32_t pos_y);
LDG_EXPORT uint32_t libresteam_remote_play_cursor_create(const libresteam_backend_set_t *set, uint32_t width, uint32_t height, uint32_t hot_x, uint32_t hot_y, const void *bgra, uint32_t pitch, uint32_t *out);
LDG_EXPORT uint32_t libresteam_remote_play_cursor_set(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t cursor_id);

#endif
