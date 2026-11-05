#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/lobby/ops.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_LOBBY_SIGN_BIT 0x80000000U
#define STEAM_LOBBY_ARG_NONE 0U
#define STEAM_LOBBY_ID_SIZE ((uint32_t)sizeof(libresteam_steam_id_t))

static uint32_t steam_lobby_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->matchmaking)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->matchmaking_desc, method_idx, out);
}

static uint32_t steam_lobby_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_lobby_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->matchmaking, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_lobby_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_lobby_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_lobby_cunt_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_lobby_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->matchmaking, slot, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_LOBBY_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_lobby_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_lobby_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(steam->matchmaking, slot, arg, arg_cunt, out);
}

static uint32_t steam_lobby_str_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    uint64_t str = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = steam_lobby_u64_call(ctx, method_idx, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_lobby_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_lobby_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->matchmaking, slot, arg, arg_cunt);
}

static uint32_t steam_lobby_sid_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, libresteam_steam_id_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_lobby_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_struct_call(steam->matchmaking, slot, arg, arg_cunt, out, STEAM_LOBBY_ID_SIZE);
}

static uint32_t steam_lobby_favorite_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_CUNT_GET, 0x0, STEAM_LOBBY_ARG_NONE, out);
}

static uint32_t steam_lobby_favorite_get(void *ctx, uint32_t idx, libresteam_lobby_favorite_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = (libresteam_lobby_favorite_t)LDG_STRUCT_ZERO_INIT;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->app_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->ip, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->conn_port, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->query_port, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->flags, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &out->last_played, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_favorite_add(void *ctx, const libresteam_lobby_favorite_t *fav, uint32_t *idx_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!fav)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->conn_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->query_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->last_played, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_ADD, arg, cunt, idx_out);
}

static uint32_t steam_lobby_favorite_remove(void *ctx, const libresteam_lobby_favorite_t *fav)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!fav)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->conn_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->query_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = fav->flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_REMOVE, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_list_req(void *ctx, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_lobby_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_REQ, 0x0, STEAM_LOBBY_ARG_NONE, call_out);
}

static uint32_t steam_lobby_list_str_filter_add(void *ctx, const char *key, const char *val, uint32_t cmp)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cmp, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_STR_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_numeric_filter_add(void *ctx, const char *key, uint32_t val, uint32_t cmp)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cmp, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_near_filter_add(void *ctx, const char *key, uint32_t val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_NEAR_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_slot_filter_add(void *ctx, uint32_t slot_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = slot_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_SLOT_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_dist_filter_add(void *ctx, libresteam_lobby_dist_t dist)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)dist, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_DIST_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_res_cunt_filter_add(void *ctx, uint32_t res_cunt_max)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = res_cunt_max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_compatible_member_filter_add(void *ctx, libresteam_steam_id_t lobby)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD, arg, cunt);
}

static uint32_t steam_lobby_list_lobby_get(void *ctx, uint32_t idx, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_lobby_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LIST_LOBBY_GET, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (out->all == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_lobby_create(void *ctx, libresteam_lobby_type_t type, uint32_t member_max, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = member_max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_CREATE, arg, cunt, call_out);
}

static uint32_t steam_lobby_join(void *ctx, libresteam_steam_id_t lobby, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_JOIN, arg, cunt, call_out);
}

static uint32_t steam_lobby_leave(void *ctx, libresteam_steam_id_t lobby)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LEAVE, arg, cunt);
}

static uint32_t steam_lobby_invite(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t invitee)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &invitee, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_INVITE, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_member_cunt_get(void *ctx, libresteam_steam_id_t lobby, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_lobby_member_get(void *ctx, libresteam_steam_id_t lobby, uint32_t idx, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_GET, arg, cunt, out);
}

static uint32_t steam_lobby_data_get(void *ctx, libresteam_steam_id_t lobby, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_GET, arg, cunt, out);
}

static uint32_t steam_lobby_data_set(void *ctx, libresteam_steam_id_t lobby, const char *key, const char *val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_data_cunt_get(void *ctx, libresteam_steam_id_t lobby, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_lobby_data_idx_get(void *ctx, libresteam_steam_id_t lobby, uint32_t idx, char *key_out, uint32_t key_cap, char *val_out, uint32_t val_cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key_out || !val_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = key_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_IDX_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_data_delete(void *ctx, libresteam_steam_id_t lobby, const char *key)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_DELETE, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_member_data_get(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t user, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &user, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_DATA_GET, arg, cunt, out);
}

static uint32_t steam_lobby_member_data_set(void *ctx, libresteam_steam_id_t lobby, const char *key, const char *val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_DATA_SET, arg, cunt);
}

static uint32_t steam_lobby_chat_msg_send(void *ctx, libresteam_steam_id_t lobby, const void *body, uint32_t len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!body)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = body, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_CHAT_MSG_SEND, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_chat_entry_get(void *ctx, libresteam_steam_id_t lobby, uint32_t chat_id, libresteam_steam_id_t *user_out, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *entry_type_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!user_out || !buff || !entry_type_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    user_out->all = 0;
    *entry_type_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = chat_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = user_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = entry_type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_CHAT_ENTRY_GET, arg, cunt, len_out);
}

static uint32_t steam_lobby_data_req(void *ctx, libresteam_steam_id_t lobby)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_DATA_REQ, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_game_srv_set(void *ctx, libresteam_steam_id_t lobby, uint32_t ip, uint16_t port, libresteam_steam_id_t srv)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &srv, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_GAME_SRV_SET, arg, cunt);
}

static uint32_t steam_lobby_game_srv_get(void *ctx, libresteam_steam_id_t lobby, uint32_t *ip_out, uint16_t *port_out, libresteam_steam_id_t *srv_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ip_out || !port_out || !srv_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ip_out = 0;
    *port_out = 0;
    srv_out->all = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ip_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = port_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = srv_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_GAME_SRV_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_lobby_member_limit_set(void *ctx, libresteam_steam_id_t lobby, uint32_t member_max)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = member_max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_LIMIT_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_member_limit_get(void *ctx, libresteam_steam_id_t lobby, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_MEMBER_LIMIT_GET, arg, cunt, out);
}

static uint32_t steam_lobby_type_set(void *ctx, libresteam_steam_id_t lobby, libresteam_lobby_type_t type)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_TYPE_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_joinable_set(void *ctx, libresteam_steam_id_t lobby, uint8_t joinable)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = joinable, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_JOINABLE_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_owner_get(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_OWNER_GET, arg, cunt, out);
}

static uint32_t steam_lobby_owner_set(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t owner)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &owner, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_OWNER_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_lobby_linked_set(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t dependent)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lobby, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &dependent, .size = STEAM_LOBBY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_lobby_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_LINKED_SET, arg, cunt, LDG_ERR_DENIED);
}

uint32_t libresteam_backend_steam_lobby_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_lobby_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->lobby_ops;
    ops->favorite_cunt_get = steam_lobby_favorite_cunt_get;
    ops->favorite_get = steam_lobby_favorite_get;
    ops->favorite_add = steam_lobby_favorite_add;
    ops->favorite_remove = steam_lobby_favorite_remove;
    ops->list_req = steam_lobby_list_req;
    ops->list_str_filter_add = steam_lobby_list_str_filter_add;
    ops->list_numeric_filter_add = steam_lobby_list_numeric_filter_add;
    ops->list_near_filter_add = steam_lobby_list_near_filter_add;
    ops->list_slot_filter_add = steam_lobby_list_slot_filter_add;
    ops->list_dist_filter_add = steam_lobby_list_dist_filter_add;
    ops->list_res_cunt_filter_add = steam_lobby_list_res_cunt_filter_add;
    ops->list_compatible_member_filter_add = steam_lobby_list_compatible_member_filter_add;
    ops->list_lobby_get = steam_lobby_list_lobby_get;
    ops->create = steam_lobby_create;
    ops->join = steam_lobby_join;
    ops->leave = steam_lobby_leave;
    ops->invite = steam_lobby_invite;
    ops->member_cunt_get = steam_lobby_member_cunt_get;
    ops->member_get = steam_lobby_member_get;
    ops->data_get = steam_lobby_data_get;
    ops->data_set = steam_lobby_data_set;
    ops->data_cunt_get = steam_lobby_data_cunt_get;
    ops->data_idx_get = steam_lobby_data_idx_get;
    ops->data_delete = steam_lobby_data_delete;
    ops->member_data_get = steam_lobby_member_data_get;
    ops->member_data_set = steam_lobby_member_data_set;
    ops->chat_msg_send = steam_lobby_chat_msg_send;
    ops->chat_entry_get = steam_lobby_chat_entry_get;
    ops->data_req = steam_lobby_data_req;
    ops->game_srv_set = steam_lobby_game_srv_set;
    ops->game_srv_get = steam_lobby_game_srv_get;
    ops->member_limit_set = steam_lobby_member_limit_set;
    ops->member_limit_get = steam_lobby_member_limit_get;
    ops->type_set = steam_lobby_type_set;
    ops->joinable_set = steam_lobby_joinable_set;
    ops->owner_get = steam_lobby_owner_get;
    ops->owner_set = steam_lobby_owner_set;
    ops->linked_set = steam_lobby_linked_set;
    steam->ops.lobby = &steam->lobby_ops;

    return LDG_ERR_AOK;
}
