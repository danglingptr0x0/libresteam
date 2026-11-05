#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/net/types.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/ops.h>

#define STEAM_USER_ARG_NONE 0U
#define STEAM_USER_DEPRECATED_NONE 0U
#define STEAM_USER_SIGN_BIT 0x80000000U

static uint32_t steam_user_slot_get(void *ctx, uint32_t method_idx, libresteam_backend_steam_t **steam_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *steam_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->user_iface)) { return LDG_ERR_NOT_INIT; }

    *steam_out = steam;

    return ldg_cpp_slot_get(&steam->user_desc, method_idx, slot_out);
}

static uint32_t steam_user_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_user_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->user_iface, slot, arg, arg_cunt);
}

static uint32_t steam_user_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->user_iface, slot, arg, arg_cunt, out);
}

static uint32_t steam_user_call_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_iface, slot, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_steam_id_get(void *ctx, libresteam_steam_id_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STEAM_ID_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_struct_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, out, (uint32_t)sizeof(libresteam_steam_id_t));
}

static uint32_t steam_user_logged_on_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_LOGGED_ON, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_game_conn_initiate(void *ctx, void *blob, uint32_t cap, libresteam_steam_id_t srv, uint32_t ip, uint16_t port, uint8_t secure, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = blob, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &srv, .size = (uint32_t)sizeof(srv), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = secure == LDG_TRUTH_TRUE ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_GAME_CONN_INITIATE, arg, cunt, &len);
    if (res != LDG_ERR_AOK) { return res; }

    if ((len & STEAM_USER_SIGN_BIT) != 0 || len == 0) { return LDG_ERR_INVALID; }

    *len_out = len;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_game_conn_terminate(void *ctx, uint32_t ip, uint16_t port)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_GAME_CONN_TERMINATE, arg, cunt);
}

static uint32_t steam_user_app_usage_track(void *ctx, uint64_t game_id, uint32_t event, const char *extra)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &game_id, .size = (uint32_t)sizeof(game_id), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = extra ? extra : "", .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_APP_USAGE_TRACK, arg, cunt);
}

static uint32_t steam_user_data_dir_get(void *ctx, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_user_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_DATA_DIR_GET, arg, cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return LDG_ERR_STR_TRUNC; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_voice_record_start(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_RECORD_START, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE);
}

static uint32_t steam_user_voice_record_stop(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_RECORD_STOP, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE);
}

static uint32_t steam_user_voice_avail_get(void *ctx, uint32_t *compressed_out, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = compressed_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = 0x0, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = STEAM_USER_DEPRECATED_NONE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_AVAIL_GET, arg, cunt, eres_out);
}

static uint32_t steam_user_voice_get(void *ctx, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = LDG_TRUTH_TRUE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = len_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = LDG_TRUTH_FALSE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = 0x0, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = STEAM_USER_DEPRECATED_NONE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = 0x0, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = STEAM_USER_DEPRECATED_NONE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_GET, arg, cunt, eres_out);
}

static uint32_t steam_user_voice_decompress(void *ctx, const void *src, uint32_t src_len, void *dst, uint32_t dst_cap, uint32_t *len_out, uint32_t sample_rate, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = src, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = src_len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = dst, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = dst_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = len_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = sample_rate, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_DECOMPRESS, arg, cunt, eres_out);
}

static uint32_t steam_user_voice_sample_rate_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_VOICE_SAMPLE_RATE_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, out);
}

static uint32_t steam_user_auth_sesh_tkt_get(void *ctx, void *buff, uint32_t cap, uint32_t *len_out, const libresteam_net_ident_t *remote, uint32_t *tkt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = len_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_AUTH_SESH_TKT_GET, arg, cunt, tkt_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*tkt_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_auth_web_tkt_get(void *ctx, const char *ident, uint32_t *tkt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ident, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_AUTH_WEB_TKT_GET, arg, cunt, tkt_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*tkt_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_auth_sesh_begin(void *ctx, const void *tkt, uint32_t len, libresteam_steam_id_t id, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tkt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(id), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_AUTH_SESH_BEGIN, arg, cunt, eres_out);
}

static uint32_t steam_user_auth_sesh_end(void *ctx, libresteam_steam_id_t id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(id), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_user_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_AUTH_SESH_END, arg, cunt);
}

static uint32_t steam_user_auth_tkt_cancel(void *ctx, uint32_t tkt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = tkt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_AUTH_TKT_CANCEL, arg, cunt);
}

static uint32_t steam_user_license_has_get(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(id), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_LICENSE_HAS_GET, arg, cunt, eres_out);
}

static uint32_t steam_user_behind_nat_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_BEHIND_NAT_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_game_advertise(void *ctx, libresteam_steam_id_t srv, uint32_t ip, uint16_t port)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &srv, .size = (uint32_t)sizeof(srv), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_GAME_ADVERTISE, arg, cunt);
}

static uint32_t steam_user_enc_app_tkt_req(void *ctx, const void *data, uint32_t len, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_ENC_APP_TKT_REQ, arg, cunt, call_out);
}

static uint32_t steam_user_enc_app_tkt_get(void *ctx, void *buff, uint32_t cap, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = len_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_ENC_APP_TKT_GET, arg, cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_badge_level_get(void *ctx, uint32_t series, uint8_t foil, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = series, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = foil == LDG_TRUTH_TRUE ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_BADGE_LEVEL_GET, arg, cunt, out);
}

static uint32_t steam_user_steam_level_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STEAM_LEVEL_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, out);
}

static uint32_t steam_user_st_auth_url_req(void *ctx, const char *redirect, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = redirect, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_ST_AUTH_URL_REQ, arg, cunt, call_out);
}

static uint32_t steam_user_phone_verified_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_PHONE_VERIFIED_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_two_factor_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_TWO_FACTOR_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_phone_ident_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_PHONE_IDENT_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_phone_verify_required_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_PHONE_VERIFY_REQUIRED_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_market_eligibility_req(void *ctx, uint64_t *call_out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_MARKET_ELIGIBILITY_REQ, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, call_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*call_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_duration_control_req(void *ctx, uint64_t *call_out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = steam_user_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_DURATION_CONTROL_REQ, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_iface, slot, 0x0, STEAM_USER_ARG_NONE, call_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*call_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_duration_control_state_set(void *ctx, uint32_t state, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = state, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_DURATION_CONTROL_STATE_SET, arg, cunt, out);
}

uint32_t libresteam_backend_steam_user_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->user_ops.steam_id_get = steam_user_steam_id_get;
    steam->user_ops.logged_on_get = steam_user_logged_on_get;
    steam->user_ops.game_conn_initiate = steam_user_game_conn_initiate;
    steam->user_ops.game_conn_terminate = steam_user_game_conn_terminate;
    steam->user_ops.app_usage_track = steam_user_app_usage_track;
    steam->user_ops.data_dir_get = steam_user_data_dir_get;
    steam->user_ops.voice_record_start = steam_user_voice_record_start;
    steam->user_ops.voice_record_stop = steam_user_voice_record_stop;
    steam->user_ops.voice_avail_get = steam_user_voice_avail_get;
    steam->user_ops.voice_get = steam_user_voice_get;
    steam->user_ops.voice_decompress = steam_user_voice_decompress;
    steam->user_ops.voice_sample_rate_get = steam_user_voice_sample_rate_get;
    steam->user_ops.auth_sesh_tkt_get = steam_user_auth_sesh_tkt_get;
    steam->user_ops.auth_web_tkt_get = steam_user_auth_web_tkt_get;
    steam->user_ops.auth_sesh_begin = steam_user_auth_sesh_begin;
    steam->user_ops.auth_sesh_end = steam_user_auth_sesh_end;
    steam->user_ops.auth_tkt_cancel = steam_user_auth_tkt_cancel;
    steam->user_ops.license_has_get = steam_user_license_has_get;
    steam->user_ops.behind_nat_get = steam_user_behind_nat_get;
    steam->user_ops.game_advertise = steam_user_game_advertise;
    steam->user_ops.enc_app_tkt_req = steam_user_enc_app_tkt_req;
    steam->user_ops.enc_app_tkt_get = steam_user_enc_app_tkt_get;
    steam->user_ops.badge_level_get = steam_user_badge_level_get;
    steam->user_ops.steam_level_get = steam_user_steam_level_get;
    steam->user_ops.st_auth_url_req = steam_user_st_auth_url_req;
    steam->user_ops.phone_verified_get = steam_user_phone_verified_get;
    steam->user_ops.two_factor_get = steam_user_two_factor_get;
    steam->user_ops.phone_ident_get = steam_user_phone_ident_get;
    steam->user_ops.phone_verify_required_get = steam_user_phone_verify_required_get;
    steam->user_ops.market_eligibility_req = steam_user_market_eligibility_req;
    steam->user_ops.duration_control_req = steam_user_duration_control_req;
    steam->user_ops.duration_control_state_set = steam_user_duration_control_state_set;
    steam->ops.user = &steam->user_ops;

    return LDG_ERR_AOK;
}
