#ifndef LIBRESTEAM_USER_OPS_H
#define LIBRESTEAM_USER_OPS_H

#include <stdint.h>

#include <libresteam/net/types.h>
#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_user_ops
{
    uint32_t (*steam_id_get)(void *ctx, libresteam_steam_id_t *out);
    uint32_t (*logged_on_get)(void *ctx, uint8_t *out);
    uint32_t (*game_conn_initiate)(void *ctx, void *blob, uint32_t cap, libresteam_steam_id_t srv, uint32_t ip, uint16_t port, uint8_t secure, uint32_t *len_out);
    uint32_t (*game_conn_terminate)(void *ctx, uint32_t ip, uint16_t port);
    uint32_t (*app_usage_track)(void *ctx, uint64_t game_id, uint32_t event, const char *extra);
    uint32_t (*data_dir_get)(void *ctx, char *buff, uint32_t cap);
    uint32_t (*voice_record_start)(void *ctx);
    uint32_t (*voice_record_stop)(void *ctx);
    uint32_t (*voice_avail_get)(void *ctx, uint32_t *compressed_out, uint32_t *eres_out);
    uint32_t (*voice_get)(void *ctx, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *eres_out);
    uint32_t (*voice_decompress)(void *ctx, const void *src, uint32_t src_len, void *dst, uint32_t dst_cap, uint32_t *len_out, uint32_t sample_rate, uint32_t *eres_out);
    uint32_t (*voice_sample_rate_get)(void *ctx, uint32_t *out);
    uint32_t (*auth_sesh_tkt_get)(void *ctx, void *buff, uint32_t cap, uint32_t *len_out, const libresteam_net_ident_t *remote, uint32_t *tkt_out);
    uint32_t (*auth_web_tkt_get)(void *ctx, const char *ident, uint32_t *tkt_out);
    uint32_t (*auth_sesh_begin)(void *ctx, const void *tkt, uint32_t len, libresteam_steam_id_t id, uint32_t *eres_out);
    uint32_t (*auth_sesh_end)(void *ctx, libresteam_steam_id_t id);
    uint32_t (*auth_tkt_cancel)(void *ctx, uint32_t tkt);
    uint32_t (*license_has_get)(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t *eres_out);
    uint32_t (*behind_nat_get)(void *ctx, uint8_t *out);
    uint32_t (*game_advertise)(void *ctx, libresteam_steam_id_t srv, uint32_t ip, uint16_t port);
    uint32_t (*enc_app_tkt_req)(void *ctx, const void *data, uint32_t len, uint64_t *call_out);
    uint32_t (*enc_app_tkt_get)(void *ctx, void *buff, uint32_t cap, uint32_t *len_out);
    uint32_t (*badge_level_get)(void *ctx, uint32_t series, uint8_t foil, uint32_t *out);
    uint32_t (*steam_level_get)(void *ctx, uint32_t *out);
    uint32_t (*st_auth_url_req)(void *ctx, const char *redirect, uint64_t *call_out);
    uint32_t (*phone_verified_get)(void *ctx, uint8_t *out);
    uint32_t (*two_factor_get)(void *ctx, uint8_t *out);
    uint32_t (*phone_ident_get)(void *ctx, uint8_t *out);
    uint32_t (*phone_verify_required_get)(void *ctx, uint8_t *out);
    uint32_t (*market_eligibility_req)(void *ctx, uint64_t *call_out);
    uint32_t (*duration_control_req)(void *ctx, uint64_t *call_out);
    uint32_t (*duration_control_state_set)(void *ctx, uint32_t state, uint8_t *out);
} libresteam_user_ops_t;

#endif
