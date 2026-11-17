#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/ops.h>
#include <libresteam/user/user.h>

#define USER_TKT_INVALID 0U

static uint32_t user_ops_get(const libresteam_backend_set_t *set, const libresteam_user_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->user) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->user;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_user_steam_id_get(const libresteam_backend_set_t *set, libresteam_steam_id_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->steam_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->steam_id_get(ctx, out);
}

uint32_t libresteam_user_logged_on_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->logged_on_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->logged_on_get(ctx, out);
}

uint32_t libresteam_user_game_conn_initiate(const libresteam_backend_set_t *set, void *blob, uint32_t cap, libresteam_steam_id_t srv, uint32_t ip, uint16_t port, uint8_t secure, uint32_t *len_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;

    if (LDG_UNLIKELY(!blob)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_conn_initiate) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_conn_initiate(ctx, blob, cap, srv, ip, port, secure, len_out);
}

uint32_t libresteam_user_game_conn_terminate(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_conn_terminate) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_conn_terminate(ctx, ip, port);
}

uint32_t libresteam_user_app_usage_track(const libresteam_backend_set_t *set, uint64_t game_id, uint32_t event, const char *extra)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_usage_track) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_usage_track(ctx, game_id, event, extra);
}

uint32_t libresteam_user_data_dir_get(const libresteam_backend_set_t *set, char *buff, uint32_t cap)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    *buff = LDG_STR_TERM;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_dir_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_dir_get(ctx, buff, cap);
}

uint32_t libresteam_user_voice_record_start(const libresteam_backend_set_t *set)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_record_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_record_start(ctx);
}

uint32_t libresteam_user_voice_record_stop(const libresteam_backend_set_t *set)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_record_stop) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_record_stop(ctx);
}

uint32_t libresteam_user_voice_avail_get(const libresteam_backend_set_t *set, uint32_t *compressed_out, uint32_t *eres_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!compressed_out || !eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *compressed_out = 0;
    *eres_out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_avail_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_avail_get(ctx, compressed_out, eres_out);
}

uint32_t libresteam_user_voice_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *eres_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out || !eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;
    *eres_out = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_get(ctx, buff, cap, len_out, eres_out);
}

uint32_t libresteam_user_voice_decompress(const libresteam_backend_set_t *set, const void *src, uint32_t src_len, void *dst, uint32_t dst_cap, uint32_t *len_out, uint32_t sample_rate, uint32_t *eres_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out || !eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;
    *eres_out = 0;

    if (LDG_UNLIKELY(!src || !dst)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_decompress) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_decompress(ctx, src, src_len, dst, dst_cap, len_out, sample_rate, eres_out);
}

uint32_t libresteam_user_voice_sample_rate_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_sample_rate_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_sample_rate_get(ctx, out);
}

uint32_t libresteam_user_auth_sesh_tkt_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out, const libresteam_net_ident_t *remote, uint32_t *tkt_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out || !tkt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;
    *tkt_out = USER_TKT_INVALID;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->auth_sesh_tkt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->auth_sesh_tkt_get(ctx, buff, cap, len_out, remote, tkt_out);
}

uint32_t libresteam_user_auth_web_tkt_get(const libresteam_backend_set_t *set, const char *ident, uint32_t *tkt_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!tkt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *tkt_out = USER_TKT_INVALID;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->auth_web_tkt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->auth_web_tkt_get(ctx, ident, tkt_out);
}

uint32_t libresteam_user_auth_sesh_begin(const libresteam_backend_set_t *set, const void *tkt, uint32_t len, libresteam_steam_id_t id, uint32_t *eres_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!tkt)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->auth_sesh_begin) { return LDG_ERR_UNSUPPORTED; }

    return ops->auth_sesh_begin(ctx, tkt, len, id, eres_out);
}

uint32_t libresteam_user_auth_sesh_end(const libresteam_backend_set_t *set, libresteam_steam_id_t id)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->auth_sesh_end) { return LDG_ERR_UNSUPPORTED; }

    return ops->auth_sesh_end(ctx, id);
}

uint32_t libresteam_user_auth_tkt_cancel(const libresteam_backend_set_t *set, uint32_t tkt)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->auth_tkt_cancel) { return LDG_ERR_UNSUPPORTED; }

    return ops->auth_tkt_cancel(ctx, tkt);
}

uint32_t libresteam_user_license_has_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t *eres_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = LIBRESTEAM_USER_LICENSE_NO_AUTH;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->license_has_get) { return LDG_ERR_UNSUPPORTED; }

    res = ops->license_has_get(ctx, id, app_id, eres_out);
    if (res != LDG_ERR_AOK || *eres_out > LIBRESTEAM_USER_LICENSE_NO_AUTH) { *eres_out = LIBRESTEAM_USER_LICENSE_NO_AUTH; }

    return res;
}

uint32_t libresteam_user_behind_nat_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->behind_nat_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->behind_nat_get(ctx, out);
}

uint32_t libresteam_user_game_advertise(const libresteam_backend_set_t *set, libresteam_steam_id_t srv, uint32_t ip, uint16_t port)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_advertise) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_advertise(ctx, srv, ip, port);
}

uint32_t libresteam_user_enc_app_tkt_req(const libresteam_backend_set_t *set, const void *data, uint32_t len, uint64_t *call_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!data && len != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->enc_app_tkt_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->enc_app_tkt_req(ctx, data, len, call_out);
}

uint32_t libresteam_user_enc_app_tkt_get(const libresteam_backend_set_t *set, void *buff, uint32_t cap, uint32_t *len_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->enc_app_tkt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->enc_app_tkt_get(ctx, buff, cap, len_out);
}

uint32_t libresteam_user_badge_level_get(const libresteam_backend_set_t *set, uint32_t series, uint8_t foil, uint32_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->badge_level_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->badge_level_get(ctx, series, foil, out);
}

uint32_t libresteam_user_steam_level_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->steam_level_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->steam_level_get(ctx, out);
}

uint32_t libresteam_user_st_auth_url_req(const libresteam_backend_set_t *set, const char *redirect, uint64_t *call_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!redirect)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->st_auth_url_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->st_auth_url_req(ctx, redirect, call_out);
}

uint32_t libresteam_user_phone_verified_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phone_verified_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->phone_verified_get(ctx, out);
}

uint32_t libresteam_user_two_factor_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->two_factor_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->two_factor_get(ctx, out);
}

uint32_t libresteam_user_phone_ident_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phone_ident_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->phone_ident_get(ctx, out);
}

uint32_t libresteam_user_phone_verify_required_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phone_verify_required_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->phone_verify_required_get(ctx, out);
}

uint32_t libresteam_user_market_eligibility_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->market_eligibility_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->market_eligibility_req(ctx, call_out);
}

uint32_t libresteam_user_duration_control_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->duration_control_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->duration_control_req(ctx, call_out);
}

uint32_t libresteam_user_duration_control_state_set(const libresteam_backend_set_t *set, uint32_t state, uint8_t *out)
{
    const libresteam_user_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = user_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->duration_control_state_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->duration_control_state_set(ctx, state, out);
}
