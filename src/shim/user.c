#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/user.h>

#define LIBRESTEAM_SHIM_USER_VERSION "SteamUser021"
#define LIBRESTEAM_SHIM_USER_METHOD_CUNT 32U
#define LIBRESTEAM_SHIM_USER_020_VERSION "SteamUser020"
#define LIBRESTEAM_SHIM_USER_020_METHOD_CUNT 31U
#define LIBRESTEAM_SHIM_USER_HANDLE_GET 0U
#define LIBRESTEAM_SHIM_USER_LOGGED_ON_IS 1U
#define LIBRESTEAM_SHIM_USER_STEAM_ID_GET 2U
#define LIBRESTEAM_SHIM_USER_GAME_CONN_INITIATE 3U
#define LIBRESTEAM_SHIM_USER_GAME_CONN_TERMINATE 4U
#define LIBRESTEAM_SHIM_USER_APP_USAGE_TRACK 5U
#define LIBRESTEAM_SHIM_USER_DATA_DIR_GET 6U
#define LIBRESTEAM_SHIM_USER_VOICE_RECORD_START 7U
#define LIBRESTEAM_SHIM_USER_VOICE_RECORD_STOP 8U
#define LIBRESTEAM_SHIM_USER_VOICE_AVAIL_GET 9U
#define LIBRESTEAM_SHIM_USER_VOICE_GET 10U
#define LIBRESTEAM_SHIM_USER_VOICE_DECOMPRESS 11U
#define LIBRESTEAM_SHIM_USER_VOICE_SAMPLE_RATE_GET 12U
#define LIBRESTEAM_SHIM_USER_AUTH_SESH_TKT_GET 13U
#define LIBRESTEAM_SHIM_USER_AUTH_SESH_BEGIN 14U
#define LIBRESTEAM_SHIM_USER_AUTH_SESH_END 15U
#define LIBRESTEAM_SHIM_USER_AUTH_TKT_CANCEL 16U
#define LIBRESTEAM_SHIM_USER_LICENSE_HAS_GET 17U
#define LIBRESTEAM_SHIM_USER_BEHIND_NAT_GET 18U
#define LIBRESTEAM_SHIM_USER_GAME_ADVERTISE 19U
#define LIBRESTEAM_SHIM_USER_ENC_APP_TKT_REQ 20U
#define LIBRESTEAM_SHIM_USER_ENC_APP_TKT_GET 21U
#define LIBRESTEAM_SHIM_USER_BADGE_LEVEL_GET 22U
#define LIBRESTEAM_SHIM_USER_STEAM_LEVEL_GET 23U
#define LIBRESTEAM_SHIM_USER_ST_AUTH_URL_REQ 24U
#define LIBRESTEAM_SHIM_USER_PHONE_VERIFIED_GET 25U
#define LIBRESTEAM_SHIM_USER_TWO_FACTOR_GET 26U
#define LIBRESTEAM_SHIM_USER_PHONE_IDENT_GET 27U
#define LIBRESTEAM_SHIM_USER_PHONE_VERIFY_REQUIRED_GET 28U
#define LIBRESTEAM_SHIM_USER_MARKET_ELIGIBILITY_REQ 29U
#define LIBRESTEAM_SHIM_USER_DURATION_CONTROL_REQ 30U
#define LIBRESTEAM_SHIM_USER_DURATION_CONTROL_STATE_SET 31U
#define LIBRESTEAM_SHIM_USER_STEAM_ID_NONE 0U
#define LIBRESTEAM_SHIM_USER_NONE 0U
#define LIBRESTEAM_SHIM_USER_CALL_NONE 0U
#define LIBRESTEAM_SHIM_USER_TKT_NONE 0U
#define LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT 1U
#define LIBRESTEAM_SHIM_USER_AUTH_TKT_INVALID 1U
#define LIBRESTEAM_SHIM_USER_LICENSE_NO_AUTH 2U
#define LIBRESTEAM_SHIM_USER_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_user_flag_get(uint32_t res, uint8_t flag)
{
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (flag == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t libresteam_shim_user_bool_get(uint8_t val)
{
    if (val) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_user_handle_get(void *self)
{
    if (!self) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_call_note((libresteam_shim_iface_t *)self, LIBRESTEAM_SHIM_USER_HANDLE_GET) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_HANDLE; }

    return LIBRESTEAM_SHIM_USER_HANDLE;
}

static uint8_t libresteam_shim_user_logged_on_is(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_LOGGED_ON_IS, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_logged_on_get(set, &flag), flag);
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_user_steam_id_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t))
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t res = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STEAM_ID_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(res); }

    if (libresteam_user_steam_id_get(set, &res) != LDG_ERR_AOK) { res.all = LIBRESTEAM_SHIM_USER_STEAM_ID_NONE; }

    LDG_CPP_STRUCT_RET(res);
}

static uint32_t libresteam_shim_user_game_conn_initiate(void *self, void *blob, uint32_t cap, uint64_t srv_id, uint32_t ip, uint16_t port, uint8_t secure)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t srv = LDG_STRUCT_ZERO_INIT;
    uint32_t len = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_GAME_CONN_INITIATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    srv.all = srv_id;
    if (libresteam_user_game_conn_initiate(set, blob, cap, srv, ip, port, libresteam_shim_user_bool_get(secure), &len) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    return len;
}

static void libresteam_shim_user_game_conn_terminate(void *self, uint32_t ip, uint16_t port)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_GAME_CONN_TERMINATE, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_user_game_conn_terminate(set, ip, port) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_user_app_usage_track(void *self, uint64_t game_id, uint32_t event, const char *extra)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_APP_USAGE_TRACK, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_user_app_usage_track(set, game_id, event, extra) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_user_data_dir_get(void *self, char *buff, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_DATA_DIR_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_user_data_dir_get(set, buff, cap) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static void libresteam_shim_user_voice_record_start(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_RECORD_START, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_user_voice_record_start(set) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_user_voice_record_stop(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_RECORD_STOP, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_user_voice_record_stop(set) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_user_voice_avail_get(void *self, uint32_t *compressed_out, uint32_t *uncompressed_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t compressed = 0;
    uint32_t eres = LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT;

    if (uncompressed_out) { *uncompressed_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_AVAIL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (libresteam_user_voice_avail_get(set, &compressed, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (compressed_out) { *compressed_out = compressed; }

    return eres;
}

static uint32_t libresteam_shim_user_voice_get(void *self, uint8_t want_compressed, void *buff, uint32_t cap, uint32_t *len_out, uint8_t want_uncompressed, void *uncompressed_buff, uint32_t uncompressed_cap, uint32_t *uncompressed_len_out, uint32_t sample_rate)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = 0;
    uint32_t uncompressed_len = 0;
    uint32_t eres = LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT;

    if (len_out) { *len_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (uncompressed_len_out) { *uncompressed_len_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (libresteam_user_voice_get(set, buff, cap, &len, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (want_compressed && len_out) { *len_out = len; }

    if (!want_uncompressed || !uncompressed_buff || len == 0) { return eres; }

    if (libresteam_user_voice_decompress(set, buff, len, uncompressed_buff, uncompressed_cap, &uncompressed_len, sample_rate, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (uncompressed_len_out) { *uncompressed_len_out = uncompressed_len; }

    return eres;
}

static uint32_t libresteam_shim_user_voice_decompress(void *self, const void *src, uint32_t src_len, void *dst, uint32_t dst_cap, uint32_t *len_out, uint32_t sample_rate)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = 0;
    uint32_t eres = LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT;

    if (len_out) { *len_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_DECOMPRESS, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (libresteam_user_voice_decompress(set, src, src_len, dst, dst_cap, &len, sample_rate, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_VOICE_NOT_INIT; }

    if (len_out) { *len_out = len; }

    return eres;
}

static uint32_t libresteam_shim_user_voice_sample_rate_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t rate = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_VOICE_SAMPLE_RATE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_user_voice_sample_rate_get(set, &rate) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    return rate;
}

static uint32_t libresteam_shim_user_auth_sesh_tkt_get(void *self, void *buff, uint32_t cap, uint32_t *len_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = 0;
    uint32_t tkt = LIBRESTEAM_SHIM_USER_TKT_NONE;

    if (len_out) { *len_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_AUTH_SESH_TKT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_TKT_NONE; }

    if (libresteam_user_auth_sesh_tkt_get(set, buff, cap, &len, 0x0, &tkt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_TKT_NONE; }

    if (len_out) { *len_out = len; }

    return tkt;
}

static uint32_t libresteam_shim_user_auth_sesh_begin(void *self, const void *tkt, uint32_t len, uint64_t id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint32_t eres = LIBRESTEAM_SHIM_USER_AUTH_TKT_INVALID;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_AUTH_SESH_BEGIN, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_AUTH_TKT_INVALID; }

    user.all = id;
    if (libresteam_user_auth_sesh_begin(set, tkt, len, user, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_AUTH_TKT_INVALID; }

    return eres;
}

static void libresteam_shim_user_auth_sesh_end(void *self, uint64_t id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_AUTH_SESH_END, &set) != LDG_ERR_AOK) { return; }

    user.all = id;
    if (libresteam_user_auth_sesh_end(set, user) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_user_auth_tkt_cancel(void *self, uint32_t tkt)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_AUTH_TKT_CANCEL, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_user_auth_tkt_cancel(set, tkt) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_user_license_has_get(void *self, uint64_t id, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint32_t eres = LIBRESTEAM_SHIM_USER_LICENSE_NO_AUTH;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_LICENSE_HAS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_LICENSE_NO_AUTH; }

    user.all = id;
    if (libresteam_user_license_has_get(set, user, app_id, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_LICENSE_NO_AUTH; }

    return eres;
}

static uint8_t libresteam_shim_user_behind_nat_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_BEHIND_NAT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_behind_nat_get(set, &flag), flag);
}

static void libresteam_shim_user_game_advertise(void *self, uint64_t srv_id, uint32_t ip, uint16_t port)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t srv = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_GAME_ADVERTISE, &set) != LDG_ERR_AOK) { return; }

    srv.all = srv_id;
    if (libresteam_user_game_advertise(set, srv, ip, port) != LDG_ERR_AOK) { return; }
}

static uint64_t libresteam_shim_user_enc_app_tkt_req(void *self, const void *data, uint32_t len)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_CALL_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_ENC_APP_TKT_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    if (libresteam_user_enc_app_tkt_req(set, data, len, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_enc_app_tkt_get(void *self, void *buff, uint32_t cap, uint32_t *len_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = 0;

    if (len_out) { *len_out = LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_ENC_APP_TKT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_user_enc_app_tkt_get(set, buff, cap, &len) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (len_out) { *len_out = len; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_user_badge_level_get(void *self, uint32_t series, uint8_t foil)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t level = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_BADGE_LEVEL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_user_badge_level_get(set, series, libresteam_shim_user_bool_get(foil), &level) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    return level;
}

static uint32_t libresteam_shim_user_steam_level_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t level = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STEAM_LEVEL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    if (libresteam_user_steam_level_get(set, &level) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_NONE; }

    return level;
}

static uint64_t libresteam_shim_user_st_auth_url_req(void *self, const char *redirect)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_CALL_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_ST_AUTH_URL_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    if (libresteam_user_st_auth_url_req(set, redirect, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_phone_verified_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_PHONE_VERIFIED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_phone_verified_get(set, &flag), flag);
}

static uint8_t libresteam_shim_user_two_factor_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_TWO_FACTOR_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_two_factor_get(set, &flag), flag);
}

static uint8_t libresteam_shim_user_phone_ident_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_PHONE_IDENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_phone_ident_get(set, &flag), flag);
}

static uint8_t libresteam_shim_user_phone_verify_required_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_PHONE_VERIFY_REQUIRED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_phone_verify_required_get(set, &flag), flag);
}

static uint64_t libresteam_shim_user_market_eligibility_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_CALL_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_MARKET_ELIGIBILITY_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    if (libresteam_user_market_eligibility_req(set, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_duration_control_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_CALL_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_DURATION_CONTROL_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    if (libresteam_user_duration_control_req(set, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_duration_control_state_set(void *self, uint32_t state)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_DURATION_CONTROL_STATE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_user_flag_get(libresteam_user_duration_control_state_set(set, state, &flag), flag);
}

static uint32_t libresteam_shim_user_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_USER_HANDLE_GET:
        { *out = "GetHSteamUser";
          break;
        }
        case LIBRESTEAM_SHIM_USER_LOGGED_ON_IS:
        { *out = "BLoggedOn";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STEAM_ID_GET:
        { *out = "GetSteamID";
          break;
        }
        case LIBRESTEAM_SHIM_USER_GAME_CONN_INITIATE:
        { *out = "InitiateGameConnection_DEPRECATED";
          break;
        }
        case LIBRESTEAM_SHIM_USER_GAME_CONN_TERMINATE:
        { *out = "TerminateGameConnection_DEPRECATED";
          break;
        }
        case LIBRESTEAM_SHIM_USER_APP_USAGE_TRACK:
        { *out = "TrackAppUsageEvent";
          break;
        }
        case LIBRESTEAM_SHIM_USER_DATA_DIR_GET:
        { *out = "GetUserDataFolder";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_RECORD_START:
        { *out = "StartVoiceRecording";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_RECORD_STOP:
        { *out = "StopVoiceRecording";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_AVAIL_GET:
        { *out = "GetAvailableVoice";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_GET:
        { *out = "GetVoice";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_DECOMPRESS:
        { *out = "DecompressVoice";
          break;
        }
        case LIBRESTEAM_SHIM_USER_VOICE_SAMPLE_RATE_GET:
        { *out = "GetVoiceOptimalSampleRate";
          break;
        }
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_TKT_GET:
        { *out = "GetAuthSessionTicket";
          break;
        }
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_BEGIN:
        { *out = "BeginAuthSession";
          break;
        }
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_END:
        { *out = "EndAuthSession";
          break;
        }
        case LIBRESTEAM_SHIM_USER_AUTH_TKT_CANCEL:
        { *out = "CancelAuthTicket";
          break;
        }
        case LIBRESTEAM_SHIM_USER_LICENSE_HAS_GET:
        { *out = "UserHasLicenseForApp";
          break;
        }
        case LIBRESTEAM_SHIM_USER_BEHIND_NAT_GET:
        { *out = "BIsBehindNAT";
          break;
        }
        case LIBRESTEAM_SHIM_USER_GAME_ADVERTISE:
        { *out = "AdvertiseGame";
          break;
        }
        case LIBRESTEAM_SHIM_USER_ENC_APP_TKT_REQ:
        { *out = "RequestEncryptedAppTicket";
          break;
        }
        case LIBRESTEAM_SHIM_USER_ENC_APP_TKT_GET:
        { *out = "GetEncryptedAppTicket";
          break;
        }
        case LIBRESTEAM_SHIM_USER_BADGE_LEVEL_GET:
        { *out = "GetGameBadgeLevel";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STEAM_LEVEL_GET:
        { *out = "GetPlayerSteamLevel";
          break;
        }
        case LIBRESTEAM_SHIM_USER_ST_AUTH_URL_REQ:
        { *out = "RequestStoreAuthURL";
          break;
        }
        case LIBRESTEAM_SHIM_USER_PHONE_VERIFIED_GET:
        { *out = "BIsPhoneVerified";
          break;
        }
        case LIBRESTEAM_SHIM_USER_TWO_FACTOR_GET:
        { *out = "BIsTwoFactorEnabled";
          break;
        }
        case LIBRESTEAM_SHIM_USER_PHONE_IDENT_GET:
        { *out = "BIsPhoneIdentifying";
          break;
        }
        case LIBRESTEAM_SHIM_USER_PHONE_VERIFY_REQUIRED_GET:
        { *out = "BIsPhoneRequiringVerification";
          break;
        }
        case LIBRESTEAM_SHIM_USER_MARKET_ELIGIBILITY_REQ:
        { *out = "GetMarketEligibility";
          break;
        }
        case LIBRESTEAM_SHIM_USER_DURATION_CONTROL_REQ:
        { *out = "GetDurationControl";
          break;
        }
        case LIBRESTEAM_SHIM_USER_DURATION_CONTROL_STATE_SET:
        { *out = "BSetDurationControlOnlineState";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_user_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_USER_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_SHIM_USER_STEAM_ID_GET) { *out = LIBRESTEAM_SHIM_RET_STRUCT; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_user_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_USER_HANDLE_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_handle_get)
        case LIBRESTEAM_SHIM_USER_LOGGED_ON_IS:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_logged_on_is)
        case LIBRESTEAM_SHIM_USER_STEAM_ID_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_steam_id_get)
        case LIBRESTEAM_SHIM_USER_GAME_CONN_INITIATE:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_game_conn_initiate)
        case LIBRESTEAM_SHIM_USER_GAME_CONN_TERMINATE:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_game_conn_terminate)
        case LIBRESTEAM_SHIM_USER_APP_USAGE_TRACK:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_app_usage_track)
        case LIBRESTEAM_SHIM_USER_DATA_DIR_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_data_dir_get)
        case LIBRESTEAM_SHIM_USER_VOICE_RECORD_START:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_record_start)
        case LIBRESTEAM_SHIM_USER_VOICE_RECORD_STOP:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_record_stop)
        case LIBRESTEAM_SHIM_USER_VOICE_AVAIL_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_avail_get)
        case LIBRESTEAM_SHIM_USER_VOICE_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_get)
        case LIBRESTEAM_SHIM_USER_VOICE_DECOMPRESS:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_decompress)
        case LIBRESTEAM_SHIM_USER_VOICE_SAMPLE_RATE_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_voice_sample_rate_get)
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_TKT_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_auth_sesh_tkt_get)
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_BEGIN:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_auth_sesh_begin)
        case LIBRESTEAM_SHIM_USER_AUTH_SESH_END:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_auth_sesh_end)
        case LIBRESTEAM_SHIM_USER_AUTH_TKT_CANCEL:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_auth_tkt_cancel)
        case LIBRESTEAM_SHIM_USER_LICENSE_HAS_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_license_has_get)
        case LIBRESTEAM_SHIM_USER_BEHIND_NAT_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_behind_nat_get)
        case LIBRESTEAM_SHIM_USER_GAME_ADVERTISE:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_game_advertise)
        case LIBRESTEAM_SHIM_USER_ENC_APP_TKT_REQ:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_enc_app_tkt_req)
        case LIBRESTEAM_SHIM_USER_ENC_APP_TKT_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_enc_app_tkt_get)
        case LIBRESTEAM_SHIM_USER_BADGE_LEVEL_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_badge_level_get)
        case LIBRESTEAM_SHIM_USER_STEAM_LEVEL_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_steam_level_get)
        case LIBRESTEAM_SHIM_USER_ST_AUTH_URL_REQ:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_st_auth_url_req)
        case LIBRESTEAM_SHIM_USER_PHONE_VERIFIED_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_phone_verified_get)
        case LIBRESTEAM_SHIM_USER_TWO_FACTOR_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_two_factor_get)
        case LIBRESTEAM_SHIM_USER_PHONE_IDENT_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_phone_ident_get)
        case LIBRESTEAM_SHIM_USER_PHONE_VERIFY_REQUIRED_GET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_phone_verify_required_get)
        case LIBRESTEAM_SHIM_USER_MARKET_ELIGIBILITY_REQ:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_market_eligibility_req)
        case LIBRESTEAM_SHIM_USER_DURATION_CONTROL_REQ:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_duration_control_req)
        case LIBRESTEAM_SHIM_USER_DURATION_CONTROL_STATE_SET:
            LIBRESTEAM_SHIM_USER_IMPL(libresteam_shim_user_duration_control_state_set)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_user_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_USER_VERSION;
    out->name_get = libresteam_shim_user_name_get;
    out->kind_get = libresteam_shim_user_kind_get;
    out->impl_get = libresteam_shim_user_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_USER_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_user_020_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_USER_020_VERSION;
    out->name_get = libresteam_shim_user_name_get;
    out->kind_get = libresteam_shim_user_kind_get;
    out->impl_get = libresteam_shim_user_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_USER_020_METHOD_CUNT;

    return LDG_ERR_AOK;
}
