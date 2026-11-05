#ifndef LIBRESTEAM_USER_H
#define LIBRESTEAM_USER_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/ops.h>

#define LIBRESTEAM_USER_LICENSE_HAS 0U
#define LIBRESTEAM_USER_LICENSE_NO_AUTH 2U

LDG_EXPORT uint32_t libresteam_user_steam_id_get(const libresteam_backend_set_t *set, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_user_logged_on_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_game_conn_initiate(const libresteam_backend_set_t *set, void *blob, uint32_t cap, libresteam_steam_id_t srv, uint32_t ip, uint16_t port, uint8_t secure, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_user_game_conn_terminate(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port);
LDG_EXPORT uint32_t libresteam_user_app_usage_track(const libresteam_backend_set_t *set, uint64_t game_id, uint32_t event, const char *extra);
LDG_EXPORT uint32_t libresteam_user_data_dir_get(const libresteam_backend_set_t *set, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_user_voice_record_start(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_user_voice_record_stop(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_user_voice_avail_get(const libresteam_backend_set_t *set, uint32_t *compressed_out, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_user_voice_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_user_voice_decompress(const libresteam_backend_set_t *set, const void *src, uint32_t src_len, void *dst, uint32_t dst_cap, uint32_t *len_out, uint32_t sample_rate, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_user_voice_sample_rate_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_auth_sesh_tkt_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out, const libresteam_net_ident_t *remote, uint32_t *tkt_out);
LDG_EXPORT uint32_t libresteam_user_auth_web_tkt_get(const libresteam_backend_set_t *set, const char *ident, uint32_t *tkt_out);
LDG_EXPORT uint32_t libresteam_user_auth_sesh_begin(const libresteam_backend_set_t *set, const void *tkt, uint32_t len, libresteam_steam_id_t id, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_user_auth_sesh_end(const libresteam_backend_set_t *set, libresteam_steam_id_t id);
LDG_EXPORT uint32_t libresteam_user_auth_tkt_cancel(const libresteam_backend_set_t *set, uint32_t tkt);
LDG_EXPORT uint32_t libresteam_user_license_has_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_user_behind_nat_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_game_advertise(const libresteam_backend_set_t *set, libresteam_steam_id_t srv, uint32_t ip, uint16_t port);
LDG_EXPORT uint32_t libresteam_user_enc_app_tkt_req(const libresteam_backend_set_t *set, const void *data, uint32_t len, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_enc_app_tkt_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_user_badge_level_get(const libresteam_backend_set_t *set, uint32_t series, uint8_t foil, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_steam_level_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_user_st_auth_url_req(const libresteam_backend_set_t *set, const char *redirect, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_phone_verified_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_two_factor_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_phone_ident_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_phone_verify_required_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_user_market_eligibility_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_duration_control_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_user_duration_control_state_set(const libresteam_backend_set_t *set, uint32_t state, uint8_t *out);

#endif
