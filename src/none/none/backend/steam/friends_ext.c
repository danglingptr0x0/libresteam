#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_FRIENDS_EXT_SIGN_BIT 0x80000000U
#define STEAM_FRIENDS_EXT_ARG_NONE 0U
#define STEAM_FRIENDS_EXT_ID_SIZE ((uint32_t)sizeof(libresteam_steam_id_t))

static uint32_t steam_friends_ext_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->friends)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->friends_desc, method_idx, out);
}

static uint32_t steam_friends_ext_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->friends, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_ext_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_friends_ext_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_ext_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->friends, slot, arg, arg_cunt, out);
}

static uint32_t steam_friends_ext_cunt_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_friends_ext_u32_call(ctx, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_FRIENDS_EXT_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_ext_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(steam->friends, slot, arg, arg_cunt, out);
}

static uint32_t steam_friends_ext_str_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    uint64_t str = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = steam_friends_ext_u64_call(ctx, method_idx, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_friends_ext_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->friends, slot, arg, arg_cunt);
}

static uint32_t steam_friends_ext_sid_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, libresteam_steam_id_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_struct_call(steam->friends, slot, arg, arg_cunt, out, STEAM_FRIENDS_EXT_ID_SIZE);
}

static uint32_t steam_friends_ext_id_cunt_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_cunt_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_id_str_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_str_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_id_flag_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_flag_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_id_bool_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, uint32_t fail_res)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_bool_call(ctx, method_idx, arg, cunt, fail_res);
}

static uint32_t steam_friends_ext_id_u64_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_u64_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_id_void_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_void_call(ctx, method_idx, arg, cunt);
}

static uint32_t steam_friends_ext_id_idx_sid_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, uint32_t idx, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_sid_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_idx_sid_call(void *ctx, uint32_t method_idx, uint32_t idx, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_sid_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_str_void_call(void *ctx, uint32_t method_idx, const char *str)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!str)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = str, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_void_call(ctx, method_idx, arg, cunt);
}

static uint32_t steam_friends_ext_id_str_bool_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t id, const char *str, uint32_t fail_res)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!str)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = str, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_bool_call(ctx, method_idx, arg, cunt, fail_res);
}

static uint32_t steam_friends_ext_id_id_flag_call(void *ctx, uint32_t method_idx, libresteam_steam_id_t lhs, libresteam_steam_id_t rhs, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &lhs, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &rhs, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_flag_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_friends_ext_friend_name_history_get(void *ctx, libresteam_steam_id_t id, uint32_t idx, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_NAME_HISTORY_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_friend_steam_level_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_STEAM_LEVEL_GET, id, out);
}

static uint32_t steam_friends_ext_nickname_get(void *ctx, libresteam_steam_id_t id, const char **out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_NICKNAME_GET, id, out);
}

static uint32_t steam_friends_ext_group_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GROUP_CUNT_GET, 0x0, STEAM_FRIENDS_EXT_ARG_NONE, out);
}

static uint32_t steam_friends_ext_group_get(void *ctx, uint32_t idx, uint16_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_friends_ext_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GROUP_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return ldg_cpp_uint16_call(steam->friends, slot, arg, cunt, out);
}

static uint32_t steam_friends_ext_group_name_get(void *ctx, uint16_t group, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GROUP_NAME_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_group_member_cunt_get(void *ctx, uint16_t group, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GROUP_MEMBER_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_group_members_get(void *ctx, uint16_t group, libresteam_steam_id_t *member, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!member)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = member, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GROUP_MEMBERS_GET, arg, cunt);
}

static uint32_t steam_friends_ext_has_friend_get(void *ctx, libresteam_steam_id_t id, uint32_t flags, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_HAS_FRIEND_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_clan_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CUNT_GET, 0x0, STEAM_FRIENDS_EXT_ARG_NONE, out);
}

static uint32_t steam_friends_ext_clan_get(void *ctx, uint32_t idx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_idx_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_GET, idx, out);
}

static uint32_t steam_friends_ext_clan_name_get(void *ctx, libresteam_steam_id_t clan, const char **out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_NAME_GET, clan, out);
}

static uint32_t steam_friends_ext_clan_tag_get(void *ctx, libresteam_steam_id_t clan, const char **out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_TAG_GET, clan, out);
}

static uint32_t steam_friends_ext_clan_activity_get(void *ctx, libresteam_steam_id_t clan, uint32_t *online_out, uint32_t *in_game_out, uint32_t *chatting_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!online_out || !in_game_out || !chatting_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *online_out = 0;
    *in_game_out = 0;
    *chatting_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &clan, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = online_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = in_game_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = chatting_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_ACTIVITY_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_friends_ext_clan_activity_req(void *ctx, libresteam_steam_id_t *clan, uint32_t cunt_in, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!clan)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = clan, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cunt_in, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_ACTIVITY_REQ, arg, cunt, call_out);
}

static uint32_t steam_friends_ext_src_friend_cunt_get(void *ctx, libresteam_steam_id_t src, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_SRC_FRIEND_CUNT_GET, src, out);
}

static uint32_t steam_friends_ext_src_friend_get(void *ctx, libresteam_steam_id_t src, uint32_t idx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_idx_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_SRC_FRIEND_GET, src, idx, out);
}

static uint32_t steam_friends_ext_src_user_in_get(void *ctx, libresteam_steam_id_t user, libresteam_steam_id_t src, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_id_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_SRC_USER_IN_GET, user, src, out);
}

static uint32_t steam_friends_ext_voice_speaking_set(void *ctx, libresteam_steam_id_t user, uint8_t speaking)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &user, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = speaking, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_VOICE_SPEAKING_SET, arg, cunt);
}

static uint32_t steam_friends_ext_overlay_activate(void *ctx, const char *dialog)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_str_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_ACTIVATE, dialog);
}

static uint32_t steam_friends_ext_overlay_user_activate(void *ctx, const char *dialog, libresteam_steam_id_t id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!dialog)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = dialog, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_USER_ACTIVATE, arg, cunt);
}

static uint32_t steam_friends_ext_overlay_web_activate(void *ctx, const char *url, uint32_t mode)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!url)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = url, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = mode, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_WEB_ACTIVATE, arg, cunt);
}

static uint32_t steam_friends_ext_overlay_st_activate(void *ctx, uint32_t app_id, uint32_t flag)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flag, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_ST_ACTIVATE, arg, cunt);
}

static uint32_t steam_friends_ext_overlay_invite_activate(void *ctx, libresteam_steam_id_t lobby)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_INVITE_ACTIVATE, lobby);
}

static uint32_t steam_friends_ext_avatar_small_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_AVATAR_SMALL_GET, id, out);
}

static uint32_t steam_friends_ext_avatar_medium_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_AVATAR_MEDIUM_GET, id, out);
}

static uint32_t steam_friends_ext_avatar_large_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_AVATAR_LARGE_GET, id, out);
}

static uint32_t steam_friends_ext_user_info_req(void *ctx, libresteam_steam_id_t id, uint8_t name_only, uint8_t *pending_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = name_only, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_USER_INFO_REQ, arg, cunt, pending_out);
}

static uint32_t steam_friends_ext_clan_officer_req(void *ctx, libresteam_steam_id_t clan, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_OFFICER_REQ, clan, call_out);
}

static uint32_t steam_friends_ext_clan_owner_get(void *ctx, libresteam_steam_id_t clan, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &clan, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_OWNER_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_clan_officer_cunt_get(void *ctx, libresteam_steam_id_t clan, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_OFFICER_CUNT_GET, clan, out);
}

static uint32_t steam_friends_ext_clan_officer_get(void *ctx, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_idx_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_OFFICER_GET, clan, idx, out);
}

static uint32_t steam_friends_ext_game_invite(void *ctx, libresteam_steam_id_t id, const char *connect)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_GAME_INVITE, id, connect, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_coplay_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_COPLAY_CUNT_GET, 0x0, STEAM_FRIENDS_EXT_ARG_NONE, out);
}

static uint32_t steam_friends_ext_coplay_get(void *ctx, uint32_t idx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_idx_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_COPLAY_GET, idx, out);
}

static uint32_t steam_friends_ext_coplay_time_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_COPLAY_TIME_GET, id, out);
}

static uint32_t steam_friends_ext_coplay_game_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_friends_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_COPLAY_GAME_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_clan_chat_join(void *ctx, libresteam_steam_id_t clan, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_JOIN, clan, call_out);
}

static uint32_t steam_friends_ext_clan_chat_leave(void *ctx, libresteam_steam_id_t clan)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_LEAVE, clan, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_friends_ext_clan_chat_member_cunt_get(void *ctx, libresteam_steam_id_t clan, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET, clan, out);
}

static uint32_t steam_friends_ext_clan_chat_member_get(void *ctx, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_idx_sid_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_MEMBER_GET, clan, idx, out);
}

static uint32_t steam_friends_ext_clan_chat_msg_send(void *ctx, libresteam_steam_id_t chat, const char *text)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_MSG_SEND, chat, text, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_clan_chat_msg_get(void *ctx, libresteam_steam_id_t chat, uint32_t idx, void *buff, uint32_t cap, uint32_t *type_out, libresteam_steam_id_t *chatter_out, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff || !type_out || !chatter_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *type_out = 0;
    chatter_out->all = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &chat, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = chatter_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_MSG_GET, arg, cunt, len_out);
}

static uint32_t steam_friends_ext_clan_chat_admin_get(void *ctx, libresteam_steam_id_t chat, libresteam_steam_id_t user, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_id_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_ADMIN_GET, chat, user, out);
}

static uint32_t steam_friends_ext_clan_chat_window_open_get(void *ctx, libresteam_steam_id_t chat, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET, chat, out);
}

static uint32_t steam_friends_ext_clan_chat_window_open(void *ctx, libresteam_steam_id_t chat)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_WINDOW_OPEN, chat, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_clan_chat_window_close(void *ctx, libresteam_steam_id_t chat)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE, chat, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_msg_listen_set(void *ctx, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_MSG_LISTEN_SET, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_msg_reply(void *ctx, libresteam_steam_id_t id, const char *text)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_str_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_MSG_REPLY, id, text, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_msg_get(void *ctx, libresteam_steam_id_t id, uint32_t msg_id, void *buff, uint32_t cap, uint32_t *type_out, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff || !type_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *type_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = msg_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_MSG_GET, arg, cunt, len_out);
}

static uint32_t steam_friends_ext_follower_cunt_req(void *ctx, libresteam_steam_id_t id, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FOLLOWER_CUNT_REQ, id, call_out);
}

static uint32_t steam_friends_ext_following_req(void *ctx, libresteam_steam_id_t id, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FOLLOWING_REQ, id, call_out);
}

static uint32_t steam_friends_ext_following_list_req(void *ctx, uint32_t start, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = start, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_FOLLOWING_LIST_REQ, arg, cunt, call_out);
}

static uint32_t steam_friends_ext_clan_public_get(void *ctx, libresteam_steam_id_t clan, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_PUBLIC_GET, clan, out);
}

static uint32_t steam_friends_ext_clan_official_get(void *ctx, libresteam_steam_id_t clan, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLAN_OFFICIAL_GET, clan, out);
}

static uint32_t steam_friends_ext_unread_chat_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_UNREAD_CHAT_CUNT_GET, 0x0, STEAM_FRIENDS_EXT_ARG_NONE, out);
}

static uint32_t steam_friends_ext_overlay_remote_play_invite_activate(void *ctx, libresteam_steam_id_t lobby)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE, lobby);
}

static uint32_t steam_friends_ext_overlay_protocol_reg(void *ctx, const char *proto)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!proto)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = proto, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_friends_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_PROTOCOL_REG, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_friends_ext_overlay_invite_connect_activate(void *ctx, const char *connect)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_str_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE, connect);
}

static uint32_t steam_friends_ext_profile_items_req(void *ctx, libresteam_steam_id_t id, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_friends_ext_id_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PROFILE_ITEMS_REQ, id, call_out);
}

static uint32_t steam_friends_ext_profile_item_has_get(void *ctx, libresteam_steam_id_t id, uint32_t type, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PROFILE_ITEM_HAS_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_profile_item_str_get(void *ctx, libresteam_steam_id_t id, uint32_t type, uint32_t prop, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prop, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PROFILE_ITEM_STR_GET, arg, cunt, out);
}

static uint32_t steam_friends_ext_profile_item_u32_get(void *ctx, libresteam_steam_id_t id, uint32_t type, uint32_t prop, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_FRIENDS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prop, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_friends_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_FRIENDS_PROFILE_ITEM_U32_GET, arg, cunt, out);
}

uint32_t libresteam_backend_steam_friends_ext_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_friends_ext_ops_t *ext = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ext = &steam->friends_ext_ops;
    ext->friend_name_history_get = steam_friends_ext_friend_name_history_get;
    ext->friend_steam_level_get = steam_friends_ext_friend_steam_level_get;
    ext->nickname_get = steam_friends_ext_nickname_get;
    ext->group_cunt_get = steam_friends_ext_group_cunt_get;
    ext->group_get = steam_friends_ext_group_get;
    ext->group_name_get = steam_friends_ext_group_name_get;
    ext->group_member_cunt_get = steam_friends_ext_group_member_cunt_get;
    ext->group_members_get = steam_friends_ext_group_members_get;
    ext->has_friend_get = steam_friends_ext_has_friend_get;
    ext->clan_cunt_get = steam_friends_ext_clan_cunt_get;
    ext->clan_get = steam_friends_ext_clan_get;
    ext->clan_name_get = steam_friends_ext_clan_name_get;
    ext->clan_tag_get = steam_friends_ext_clan_tag_get;
    ext->clan_activity_get = steam_friends_ext_clan_activity_get;
    ext->clan_activity_req = steam_friends_ext_clan_activity_req;
    ext->src_friend_cunt_get = steam_friends_ext_src_friend_cunt_get;
    ext->src_friend_get = steam_friends_ext_src_friend_get;
    ext->src_user_in_get = steam_friends_ext_src_user_in_get;
    ext->voice_speaking_set = steam_friends_ext_voice_speaking_set;
    ext->overlay_activate = steam_friends_ext_overlay_activate;
    ext->overlay_user_activate = steam_friends_ext_overlay_user_activate;
    ext->overlay_web_activate = steam_friends_ext_overlay_web_activate;
    ext->overlay_st_activate = steam_friends_ext_overlay_st_activate;
    ext->overlay_invite_activate = steam_friends_ext_overlay_invite_activate;
    ext->avatar_small_get = steam_friends_ext_avatar_small_get;
    ext->avatar_medium_get = steam_friends_ext_avatar_medium_get;
    ext->avatar_large_get = steam_friends_ext_avatar_large_get;
    ext->user_info_req = steam_friends_ext_user_info_req;
    ext->clan_officer_req = steam_friends_ext_clan_officer_req;
    ext->clan_owner_get = steam_friends_ext_clan_owner_get;
    ext->clan_officer_cunt_get = steam_friends_ext_clan_officer_cunt_get;
    ext->clan_officer_get = steam_friends_ext_clan_officer_get;
    ext->game_invite = steam_friends_ext_game_invite;
    ext->coplay_cunt_get = steam_friends_ext_coplay_cunt_get;
    ext->coplay_get = steam_friends_ext_coplay_get;
    ext->coplay_time_get = steam_friends_ext_coplay_time_get;
    ext->coplay_game_get = steam_friends_ext_coplay_game_get;
    ext->clan_chat_join = steam_friends_ext_clan_chat_join;
    ext->clan_chat_leave = steam_friends_ext_clan_chat_leave;
    ext->clan_chat_member_cunt_get = steam_friends_ext_clan_chat_member_cunt_get;
    ext->clan_chat_member_get = steam_friends_ext_clan_chat_member_get;
    ext->clan_chat_msg_send = steam_friends_ext_clan_chat_msg_send;
    ext->clan_chat_msg_get = steam_friends_ext_clan_chat_msg_get;
    ext->clan_chat_admin_get = steam_friends_ext_clan_chat_admin_get;
    ext->clan_chat_window_open_get = steam_friends_ext_clan_chat_window_open_get;
    ext->clan_chat_window_open = steam_friends_ext_clan_chat_window_open;
    ext->clan_chat_window_close = steam_friends_ext_clan_chat_window_close;
    ext->msg_listen_set = steam_friends_ext_msg_listen_set;
    ext->msg_reply = steam_friends_ext_msg_reply;
    ext->msg_get = steam_friends_ext_msg_get;
    ext->follower_cunt_req = steam_friends_ext_follower_cunt_req;
    ext->following_req = steam_friends_ext_following_req;
    ext->following_list_req = steam_friends_ext_following_list_req;
    ext->clan_public_get = steam_friends_ext_clan_public_get;
    ext->clan_official_get = steam_friends_ext_clan_official_get;
    ext->unread_chat_cunt_get = steam_friends_ext_unread_chat_cunt_get;
    ext->overlay_remote_play_invite_activate = steam_friends_ext_overlay_remote_play_invite_activate;
    ext->overlay_protocol_reg = steam_friends_ext_overlay_protocol_reg;
    ext->overlay_invite_connect_activate = steam_friends_ext_overlay_invite_connect_activate;
    ext->profile_items_req = steam_friends_ext_profile_items_req;
    ext->profile_item_has_get = steam_friends_ext_profile_item_has_get;
    ext->profile_item_str_get = steam_friends_ext_profile_item_str_get;
    ext->profile_item_u32_get = steam_friends_ext_profile_item_u32_get;
    steam->friends_ops.ext = ext;

    return LDG_ERR_AOK;
}
