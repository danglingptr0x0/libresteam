#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/mem/secure.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_FRIENDS_SIGN_BIT 0x80000000U

static uint32_t steam_friends_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->friends)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->friends_desc, method_idx, out);
}

static uint32_t steam_friends_str_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_friends_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->friends, slot, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_cunt_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_friends_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->friends, slot, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_FRIENDS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_INIT;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_void_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_friends_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->friends, slot, arg, arg_cunt);
}

static uint32_t steam_friends_persona_state_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, libresteam_friends_persona_state_t *out)
{
    uint32_t state = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_friends_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->friends, slot, arg, arg_cunt, &state);
    if (res != LDG_ERR_AOK) { return res; }

    if (state >= LIBRESTEAM_FRIENDS_PERSONA_STATE_MAX) { return LDG_ERR_RANGE; }

    *out = (libresteam_friends_persona_state_t)state;

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_persona_name_get(void *ctx, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PERSONA_NAME_GET, arg, cunt, out);
}

static uint32_t steam_friends_persona_state_get(void *ctx, libresteam_friends_persona_state_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_persona_state_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PERSONA_STATE_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_cunt_get(void *ctx, uint32_t flags, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_cunt_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_get(void *ctx, uint32_t idx, uint32_t flags, libresteam_steam_id_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_friends_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_struct_call(steam->friends, slot, arg, cunt, out, (uint32_t)sizeof(libresteam_steam_id_t));
}

static uint32_t steam_friends_friend_rel_get(void *ctx, libresteam_steam_id_t id, libresteam_friends_rel_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t rel = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_REL_NONE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    res = steam_friends_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_REL_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->friends, slot, arg, cunt, &rel);
    if (res != LDG_ERR_AOK) { return res; }

    if (rel >= LIBRESTEAM_FRIENDS_REL_MAX) { return LDG_ERR_RANGE; }

    *out = (libresteam_friends_rel_t)rel;

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_friend_persona_state_get(void *ctx, libresteam_steam_id_t id, libresteam_friends_persona_state_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_persona_state_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_PERSONA_STATE_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_persona_name_get(void *ctx, libresteam_steam_id_t id, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_PERSONA_NAME_GET, arg, cunt, out);
}

static uint32_t steam_friends_rich_presence_set(void *ctx, const char *key, const char *val)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t ok = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_friends_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_FRIENDS_RICH_PRESENCE_SET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->friends, slot, arg, cunt, &ok);
    if (res != LDG_ERR_AOK) { return res; }

    if (ok != LDG_TRUTH_TRUE) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_rich_presence_clear(void *ctx)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_void_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_RICH_PRESENCE_CLEAR, arg, cunt);
}

static uint32_t steam_friends_friend_rich_presence_get(void *ctx, libresteam_steam_id_t id, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_RICH_PRESENCE_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_rich_presence_key_cunt_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_cunt_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_rich_presence_key_get(void *ctx, libresteam_steam_id_t id, uint32_t idx, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_rich_presence_req(void *ctx, libresteam_steam_id_t id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_void_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_RICH_PRESENCE_REQ, arg, cunt);
}

static uint32_t steam_friends_played_with_set(void *ctx, libresteam_steam_id_t id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_void_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PLAYED_WITH_SET, arg, cunt);
}

static uint32_t steam_friends_friend_game_played_get(void *ctx, libresteam_steam_id_t id, libresteam_friends_game_info_t *info_out, uint8_t *playing_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!info_out || !playing_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *playing_out = LDG_TRUTH_FALSE;
    res = ldg_mem_secure_zero(info_out, (uint64_t)sizeof(*info_out));
    if (res != LDG_ERR_AOK) { return res; }

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FRIEND_GAME_PLAYED_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = info_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return ldg_cpp_uint8_call(steam->friends, slot, arg, cunt, playing_out);
}

static uint32_t steam_friends_client_str_call(libresteam_backend_steam_t *steam, uint32_t slot, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    uint64_t str = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam->client_friends)) { return LDG_ERR_NOT_INIT; }

    res = ldg_cpp_uint64_call(steam->client_friends, slot, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_friend_app_rich_presence_get(void *ctx, libresteam_steam_id_t id, uint32_t app_id, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_client_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLIENT_FRIENDS_RICH_PRESENCE_GET, arg, cunt, out);
}

static uint32_t steam_friends_friend_app_rich_presence_key_cunt_get(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->client_friends)) { return LDG_ERR_NOT_INIT; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    res = ldg_cpp_uint32_call(steam->client_friends, LIBRESTEAM_BACKEND_STEAM_CLIENT_FRIENDS_RICH_PRESENCE_KEY_CUNT_GET, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_FRIENDS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_INIT;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_friend_app_rich_presence_key_get(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t idx, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_client_str_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLIENT_FRIENDS_RICH_PRESENCE_KEY_GET, arg, cunt, out);
}

uint32_t libresteam_backend_steam_friends_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->friends_ops.persona_name_get = steam_friends_persona_name_get;
    steam->friends_ops.persona_state_get = steam_friends_persona_state_get;
    steam->friends_ops.friend_cunt_get = steam_friends_friend_cunt_get;
    steam->friends_ops.friend_get = steam_friends_friend_get;
    steam->friends_ops.friend_rel_get = steam_friends_friend_rel_get;
    steam->friends_ops.friend_persona_state_get = steam_friends_friend_persona_state_get;
    steam->friends_ops.friend_persona_name_get = steam_friends_friend_persona_name_get;
    steam->friends_ops.rich_presence_set = steam_friends_rich_presence_set;
    steam->friends_ops.rich_presence_clear = steam_friends_rich_presence_clear;
    steam->friends_ops.friend_rich_presence_get = steam_friends_friend_rich_presence_get;
    steam->friends_ops.friend_rich_presence_key_cunt_get = steam_friends_friend_rich_presence_key_cunt_get;
    steam->friends_ops.friend_rich_presence_key_get = steam_friends_friend_rich_presence_key_get;
    steam->friends_ops.friend_rich_presence_req = steam_friends_friend_rich_presence_req;
    steam->friends_ops.played_with_set = steam_friends_played_with_set;
    steam->friends_ops.friend_game_played_get = steam_friends_friend_game_played_get;
    steam->friends_ops.friend_app_rich_presence_get = steam_friends_friend_app_rich_presence_get;
    steam->friends_ops.friend_app_rich_presence_key_cunt_get = steam_friends_friend_app_rich_presence_key_cunt_get;
    steam->friends_ops.friend_app_rich_presence_key_get = steam_friends_friend_app_rich_presence_key_get;
    steam->ops.friends = &steam->friends_ops;

    return LDG_ERR_AOK;
}
