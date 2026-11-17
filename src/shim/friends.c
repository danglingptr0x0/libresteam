#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/friends/friends.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_SHIM_FRIENDS_VERSION "SteamFriends017"
#define LIBRESTEAM_SHIM_FRIENDS_METHOD_CUNT 76U
#define LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET 0U
#define LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_SET 1U
#define LIBRESTEAM_SHIM_FRIENDS_PERSONA_STATE_GET 2U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_CUNT_GET 3U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET 4U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_REL_GET 5U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_STATE_GET 6U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET 7U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_GAME_PLAYED_GET 8U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET 9U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_STEAM_LEVEL_GET 10U
#define LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET 11U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_CUNT_GET 12U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_GET 13U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET 14U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBER_CUNT_GET 15U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBERS_GET 16U
#define LIBRESTEAM_SHIM_FRIENDS_HAS_FRIEND_GET 17U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CUNT_GET 18U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_GET 19U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET 20U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET 21U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_GET 22U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_REQ 23U
#define LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_CUNT_GET 24U
#define LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET 25U
#define LIBRESTEAM_SHIM_FRIENDS_SRC_USER_IN_GET 26U
#define LIBRESTEAM_SHIM_FRIENDS_VOICE_SPEAKING_SET 27U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ACTIVATE 28U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_USER_ACTIVATE 29U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_WEB_ACTIVATE 30U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ST_ACTIVATE 31U
#define LIBRESTEAM_SHIM_FRIENDS_PLAYED_WITH_SET 32U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_ACTIVATE 33U
#define LIBRESTEAM_SHIM_FRIENDS_AVATAR_SMALL_GET 34U
#define LIBRESTEAM_SHIM_FRIENDS_AVATAR_MEDIUM_GET 35U
#define LIBRESTEAM_SHIM_FRIENDS_AVATAR_LARGE_GET 36U
#define LIBRESTEAM_SHIM_FRIENDS_USER_INFO_REQ 37U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_REQ 38U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET 39U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_CUNT_GET 40U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET 41U
#define LIBRESTEAM_SHIM_FRIENDS_USER_RESTRICTIONS_GET 42U
#define LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_SET 43U
#define LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_CLEAR 44U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET 45U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET 46U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET 47U
#define LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_REQ 48U
#define LIBRESTEAM_SHIM_FRIENDS_GAME_INVITE 49U
#define LIBRESTEAM_SHIM_FRIENDS_COPLAY_CUNT_GET 50U
#define LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET 51U
#define LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_GET 52U
#define LIBRESTEAM_SHIM_FRIENDS_COPLAY_GAME_GET 53U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_JOIN 54U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_LEAVE 55U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET 56U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET 57U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_SEND 58U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_GET 59U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_ADMIN_GET 60U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET 61U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN 62U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE 63U
#define LIBRESTEAM_SHIM_FRIENDS_MSG_LISTEN_SET 64U
#define LIBRESTEAM_SHIM_FRIENDS_MSG_REPLY 65U
#define LIBRESTEAM_SHIM_FRIENDS_MSG_GET 66U
#define LIBRESTEAM_SHIM_FRIENDS_FOLLOWER_CUNT_REQ 67U
#define LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_REQ 68U
#define LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_LIST_REQ 69U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_PUBLIC_GET 70U
#define LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICIAL_GET 71U
#define LIBRESTEAM_SHIM_FRIENDS_UNREAD_CHAT_CUNT_GET 72U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE 73U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_PROTOCOL_REG 74U
#define LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE 75U
#define LIBRESTEAM_SHIM_FRIENDS_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_CALL_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_RESTRICTION_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_GROUP_INVALID 0xFFFFU
#define LIBRESTEAM_SHIM_FRIENDS_STEAM_LEVEL_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_MSG_LEN_NONE 0U
#define LIBRESTEAM_SHIM_FRIENDS_CHAT_ENTRY_TYPE_INVALID 0U
#define LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY ""
#define LIBRESTEAM_SHIM_FRIENDS_STR(str) ((str) ? (str) : LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY)
#define LIBRESTEAM_SHIM_FRIENDS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_friends_aok_is(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t libresteam_shim_friends_flag_get(uint32_t res, uint8_t flag)
{
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (flag == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static const char* libresteam_shim_friends_persona_name_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    res = libresteam_friends_persona_name_get(set, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET, res, "name: %s", LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static uint64_t libresteam_shim_friends_persona_name_set(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_SET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_SET, LDG_ERR_UNSUPPORTED, "name: %s", LIBRESTEAM_SHIM_FRIENDS_STR(name));
    if (res != LDG_ERR_AOK || !set) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
}

static uint32_t libresteam_shim_friends_persona_state_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_friends_persona_state_t state = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_STATE_GET, &set) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE; }

    res = libresteam_friends_persona_state_get(set, &state);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_PERSONA_STATE_GET, res, "state: %u32", (uint32_t)state);
    if (res != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE; }

    return (uint32_t)state;
}

static uint32_t libresteam_shim_friends_friend_cunt_get(void *self, uint32_t flags)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_friend_cunt_get(set, flags, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_CUNT_GET, res, "flags: %x32; cunt: %u32", flags, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_friend_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint32_t idx, uint32_t flags)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(id); }

    res = libresteam_friends_friend_get(set, idx, flags, &id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET, res, "idx: %u32; flags: %x32; id: %u64", idx, flags, id.all);
    if (res != LDG_ERR_AOK) { id.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(id);
}

static uint32_t libresteam_shim_friends_friend_rel_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    libresteam_friends_rel_t rel = LIBRESTEAM_FRIENDS_REL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_REL_GET, &set) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_REL_NONE; }

    id.all = raw;
    res = libresteam_friends_friend_rel_get(set, id, &rel);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_REL_GET, res, "id: %u64; rel: %u32", raw, (uint32_t)rel);
    if (res != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_REL_NONE; }

    return (uint32_t)rel;
}

static uint32_t libresteam_shim_friends_friend_persona_state_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    libresteam_friends_persona_state_t state = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_STATE_GET, &set) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE; }

    id.all = raw;
    res = libresteam_friends_friend_persona_state_get(set, id, &state);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_STATE_GET, res, "id: %u64; state: %u32", raw, (uint32_t)state);
    if (res != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE; }

    return (uint32_t)state;
}

static const char* libresteam_shim_friends_friend_persona_name_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    id.all = raw;
    res = libresteam_friends_friend_persona_name_get(set, id, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET, res, "id: %u64; name: %s", raw, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_friends_friend_game_played_get(void *self, uint64_t raw, libresteam_friends_game_info_t *info)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    libresteam_friends_game_info_t game = LDG_STRUCT_ZERO_INIT;
    uint8_t playing = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_GAME_PLAYED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    id.all = raw;
    res = libresteam_friends_friend_game_played_get(set, id, &game, &playing);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_GAME_PLAYED_GET, res, "id: %u64; playing: %u32; game: %u64; lobby: %u64", raw, (uint32_t)playing, game.game_id, game.lobby.all);
    if (res != LDG_ERR_AOK || playing != LDG_TRUTH_TRUE) { return LDG_TRUTH_FALSE; }

    if (info && ldg_mem_secure_cpy(info, &game, (uint64_t)sizeof(game)) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static const char* libresteam_shim_friends_friend_name_history_get(void *self, uint64_t raw, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    id.all = raw;
    res = libresteam_friends_friend_name_history_get(set, id, idx, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET, res, "id: %u64; idx: %u32; name: %s", raw, idx, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static uint32_t libresteam_shim_friends_friend_steam_level_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t level = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_STEAM_LEVEL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STEAM_LEVEL_NONE; }

    id.all = raw;
    res = libresteam_friends_friend_steam_level_get(set, id, &level);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_STEAM_LEVEL_GET, res, "id: %u64; level: %u32", raw, level);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STEAM_LEVEL_NONE; }

    return level;
}

static const char* libresteam_shim_friends_nickname_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET, &set) != LDG_ERR_AOK) { return 0x0; }

    id.all = raw;
    res = libresteam_friends_nickname_get(set, id, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET, res, "id: %u64; nickname: %s", raw, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK) { return 0x0; }

    return str;
}

static uint32_t libresteam_shim_friends_group_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_group_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static uint16_t libresteam_shim_friends_group_get(void *self, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    uint16_t group = LIBRESTEAM_SHIM_FRIENDS_GROUP_INVALID;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_GROUP_INVALID; }

    res = libresteam_friends_group_get(set, idx, &group);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_GET, res, "idx: %u32; group: %u32", idx, (uint32_t)group);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_GROUP_INVALID; }

    return group;
}

static const char* libresteam_shim_friends_group_name_get(void *self, uint16_t group)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET, &set) != LDG_ERR_AOK) { return 0x0; }

    res = libresteam_friends_group_name_get(set, group, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET, res, "group: %u32; name: %s", (uint32_t)group, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK) { return 0x0; }

    return str;
}

static uint32_t libresteam_shim_friends_group_member_cunt_get(void *self, uint16_t group)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBER_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_group_member_cunt_get(set, group, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBER_CUNT_GET, res, "group: %u32; cunt: %u32", (uint32_t)group, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static void libresteam_shim_friends_group_members_get(void *self, uint16_t group, libresteam_steam_id_t *member, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBERS_GET, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_group_members_get(set, group, member, cap);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBERS_GET, res, "group: %u32; cap: %u32; first: %u64", (uint32_t)group, cap, (member && cap != 0) ? member->all : (uint64_t)LIBRESTEAM_SHIM_FRIENDS_NONE);
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_friends_has_friend_get(void *self, uint64_t raw, uint32_t flags)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_HAS_FRIEND_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    id.all = raw;
    res = libresteam_friends_has_friend_get(set, id, flags, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_HAS_FRIEND_GET, res, "id: %u64; flags: %x32; has: %u32", raw, flags, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static uint32_t libresteam_shim_friends_clan_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_clan_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_clan_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(id); }

    res = libresteam_friends_clan_get(set, idx, &id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_GET, res, "idx: %u32; clan: %u64", idx, id.all);
    if (res != LDG_ERR_AOK) { id.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(id);
}

static const char* libresteam_shim_friends_clan_name_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    clan.all = raw;
    res = libresteam_friends_clan_name_get(set, clan, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET, res, "clan: %u64; name: %s", raw, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static const char* libresteam_shim_friends_clan_tag_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    clan.all = raw;
    res = libresteam_friends_clan_tag_get(set, clan, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET, res, "clan: %u64; tag: %s", raw, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_friends_clan_activity_get(void *self, uint64_t raw, uint32_t *online_out, uint32_t *in_game_out, uint32_t *chatting_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint32_t online = 0;
    uint32_t in_game = 0;
    uint32_t chatting = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    clan.all = raw;
    res = libresteam_friends_clan_activity_get(set, clan, &online, &in_game, &chatting);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_GET, res, "clan: %u64; online: %u32; in_game: %u32; chatting: %u32", raw, online, in_game, chatting);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (online_out) { *online_out = online; }

    if (in_game_out) { *in_game_out = in_game; }

    if (chatting_out) { *chatting_out = chatting; }

    return LDG_TRUTH_TRUE;
}

static uint64_t libresteam_shim_friends_clan_activity_req(void *self, libresteam_steam_id_t *clan, uint32_t cunt)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    res = libresteam_friends_clan_activity_req(set, clan, cunt, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_REQ, res, "cunt: %u32; first: %u64; call: %u64", cunt, (clan && cunt != 0) ? clan->all : (uint64_t)LIBRESTEAM_SHIM_FRIENDS_NONE, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static uint32_t libresteam_shim_friends_src_friend_cunt_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t src = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    src.all = raw;
    res = libresteam_friends_src_friend_cunt_get(set, src, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_CUNT_GET, res, "src: %u64; cunt: %u32", raw, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_src_friend_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t raw, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t src = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(id); }

    src.all = raw;
    res = libresteam_friends_src_friend_get(set, src, idx, &id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET, res, "src: %u64; idx: %u32; id: %u64", raw, idx, id.all);
    if (res != LDG_ERR_AOK) { id.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(id);
}

static uint8_t libresteam_shim_friends_src_user_in_get(void *self, uint64_t user_raw, uint64_t src_raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t src = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_SRC_USER_IN_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = user_raw;
    src.all = src_raw;
    res = libresteam_friends_src_user_in_get(set, user, src, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_SRC_USER_IN_GET, res, "user: %u64; src: %u64; in: %u32", user_raw, src_raw, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static void libresteam_shim_friends_voice_speaking_set(void *self, uint64_t raw, uint8_t speaking)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_VOICE_SPEAKING_SET, &set) != LDG_ERR_AOK) { return; }

    user.all = raw;
    if (speaking) { flag = LDG_TRUTH_TRUE; }

    res = libresteam_friends_voice_speaking_set(set, user, flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_VOICE_SPEAKING_SET, res, "user: %u64; speaking: %u32", raw, (uint32_t)flag);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_overlay_activate(void *self, const char *dialog)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_overlay_activate(set, dialog);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ACTIVATE, res, "dialog: %s", LIBRESTEAM_SHIM_FRIENDS_STR(dialog));
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_overlay_user_activate(void *self, const char *dialog, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_USER_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    user.all = raw;
    res = libresteam_friends_overlay_user_activate(set, dialog, user);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_USER_ACTIVATE, res, "dialog: %s; user: %u64", LIBRESTEAM_SHIM_FRIENDS_STR(dialog), raw);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_overlay_web_activate(void *self, const char *url, uint32_t mode)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_WEB_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_overlay_web_activate(set, url, mode);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_WEB_ACTIVATE, res, "mode: %u32", mode);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_overlay_st_activate(void *self, uint32_t app_id, uint32_t flag)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ST_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_overlay_st_activate(set, app_id, flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ST_ACTIVATE, res, "app_id: %u32; flag: %u32", app_id, flag);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_played_with_set(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_PLAYED_WITH_SET, &set) != LDG_ERR_AOK) { return; }

    id.all = raw;
    res = libresteam_friends_played_with_set(set, id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_PLAYED_WITH_SET, res, "id: %u64", raw);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_friends_overlay_invite_activate(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    lobby.all = raw;
    res = libresteam_friends_overlay_invite_activate(set, lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_ACTIVATE, res, "lobby: %u64", raw);
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_friends_avatar_small_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t avatar = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_SMALL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    id.all = raw;
    res = libresteam_friends_avatar_small_get(set, id, &avatar);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_SMALL_GET, res, "id: %u64; avatar: %u32", raw, avatar);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    return avatar;
}

static uint32_t libresteam_shim_friends_avatar_medium_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t avatar = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_MEDIUM_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    id.all = raw;
    res = libresteam_friends_avatar_medium_get(set, id, &avatar);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_MEDIUM_GET, res, "id: %u64; avatar: %u32", raw, avatar);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    return avatar;
}

static uint32_t libresteam_shim_friends_avatar_large_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t avatar = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_LARGE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    id.all = raw;
    res = libresteam_friends_avatar_large_get(set, id, &avatar);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_AVATAR_LARGE_GET, res, "id: %u64; avatar: %u32", raw, avatar);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_AVATAR_NONE; }

    return avatar;
}

static uint8_t libresteam_shim_friends_user_info_req(void *self, uint64_t raw, uint8_t name_only)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint8_t name_only_flag = LDG_TRUTH_FALSE;
    uint8_t pending = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_USER_INFO_REQ, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = raw;
    if (name_only) { name_only_flag = LDG_TRUTH_TRUE; }

    res = libresteam_friends_user_info_req(set, user, name_only_flag, &pending);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_USER_INFO_REQ, res, "user: %u64; name_only: %u32; pending: %u32", raw, (uint32_t)name_only_flag, (uint32_t)pending);

    return libresteam_shim_friends_flag_get(res, pending);
}

static uint64_t libresteam_shim_friends_clan_officer_req(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    clan.all = raw;
    res = libresteam_friends_clan_officer_req(set, clan, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_REQ, res, "clan: %u64; call: %u64", raw, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_clan_owner_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t owner = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(owner); }

    clan.all = raw;
    res = libresteam_friends_clan_owner_get(set, clan, &owner);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET, res, "clan: %u64; owner: %u64", raw, owner.all);
    if (res != LDG_ERR_AOK) { owner.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(owner);
}

static uint32_t libresteam_shim_friends_clan_officer_cunt_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    clan.all = raw;
    res = libresteam_friends_clan_officer_cunt_get(set, clan, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_CUNT_GET, res, "clan: %u64; cunt: %u32", raw, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_clan_officer_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t raw, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t officer = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(officer); }

    clan.all = raw;
    res = libresteam_friends_clan_officer_get(set, clan, idx, &officer);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET, res, "clan: %u64; idx: %u32; officer: %u64", raw, idx, officer.all);
    if (res != LDG_ERR_AOK) { officer.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(officer);
}

static uint32_t libresteam_shim_friends_user_restrictions_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_USER_RESTRICTIONS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_RESTRICTION_NONE; }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_USER_RESTRICTIONS_GET, LDG_ERR_UNSUPPORTED, "restrictions: %u32", LIBRESTEAM_SHIM_FRIENDS_RESTRICTION_NONE);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_RESTRICTION_NONE; }

    return LIBRESTEAM_SHIM_FRIENDS_RESTRICTION_NONE;
}

static uint8_t libresteam_shim_friends_rich_presence_set(void *self, const char *key, const char *val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_friends_rich_presence_set(set, key, val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_SET, res, "key: %s; val: %s", LIBRESTEAM_SHIM_FRIENDS_STR(key), LIBRESTEAM_SHIM_FRIENDS_STR(val));

    return libresteam_shim_friends_aok_is(res);
}

static void libresteam_shim_friends_rich_presence_clear(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_CLEAR, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_rich_presence_clear(set);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_CLEAR, res, "clear");
    if (res != LDG_ERR_AOK) { return; }
}

static const char* libresteam_shim_friends_friend_rich_presence_get(void *self, uint64_t raw, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    id.all = raw;
    res = libresteam_friends_friend_rich_presence_get(set, id, key, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET, res, "id: %u64; key: %s", raw, LIBRESTEAM_SHIM_FRIENDS_STR(key));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static uint32_t libresteam_shim_friends_friend_rich_presence_key_cunt_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    id.all = raw;
    res = libresteam_friends_friend_rich_presence_key_cunt_get(set, id, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET, res, "id: %u64; cunt: %u32", raw, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static const char* libresteam_shim_friends_friend_rich_presence_key_get(void *self, uint64_t raw, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    id.all = raw;
    res = libresteam_friends_friend_rich_presence_key_get(set, id, idx, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET, res, "id: %u64; idx: %u32; key: %s", raw, idx, LIBRESTEAM_SHIM_FRIENDS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_FRIENDS_STR_EMPTY; }

    return str;
}

static void libresteam_shim_friends_friend_rich_presence_req(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_REQ, &set) != LDG_ERR_AOK) { return; }

    id.all = raw;
    res = libresteam_friends_friend_rich_presence_req(set, id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_REQ, res, "id: %u64", raw);
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_friends_game_invite(void *self, uint64_t raw, const char *connect)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_GAME_INVITE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    id.all = raw;
    res = libresteam_friends_game_invite(set, id, connect);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_GAME_INVITE, res, "id: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint32_t libresteam_shim_friends_coplay_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_coplay_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_coplay_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(id); }

    res = libresteam_friends_coplay_get(set, idx, &id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET, res, "idx: %u32; id: %u64", idx, id.all);
    if (res != LDG_ERR_AOK) { id.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(id);
}

static uint32_t libresteam_shim_friends_coplay_time_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t time = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_NONE; }

    id.all = raw;
    res = libresteam_friends_coplay_time_get(set, id, &time);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_GET, res, "id: %u64; time: %u32", raw, time);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_NONE; }

    return time;
}

static uint32_t libresteam_shim_friends_coplay_game_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t app_id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_GAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    id.all = raw;
    res = libresteam_friends_coplay_game_get(set, id, &app_id);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_COPLAY_GAME_GET, res, "id: %u64; app_id: %u32", raw, app_id);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return app_id;
}

static uint64_t libresteam_shim_friends_clan_chat_join(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_JOIN, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    clan.all = raw;
    res = libresteam_friends_clan_chat_join(set, clan, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_JOIN, res, "clan: %u64; call: %u64", raw, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_friends_clan_chat_leave(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_LEAVE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    clan.all = raw;
    res = libresteam_friends_clan_chat_leave(set, clan);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_LEAVE, res, "clan: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint32_t libresteam_shim_friends_clan_chat_member_cunt_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    clan.all = raw;
    res = libresteam_friends_clan_chat_member_cunt_get(set, clan, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET, res, "clan: %u64; cunt: %u32", raw, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_friends_clan_chat_member_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t raw, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t member = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(member); }

    clan.all = raw;
    res = libresteam_friends_clan_chat_member_get(set, clan, idx, &member);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET, res, "clan: %u64; idx: %u32; member: %u64", raw, idx, member.all);
    if (res != LDG_ERR_AOK) { member.all = LIBRESTEAM_SHIM_FRIENDS_NONE; }

    LDG_CPP_STRUCT_RET(member);
}

static uint8_t libresteam_shim_friends_clan_chat_msg_send(void *self, uint64_t raw, const char *text)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_SEND, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    chat.all = raw;
    res = libresteam_friends_clan_chat_msg_send(set, chat, text);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_SEND, res, "chat: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint32_t libresteam_shim_friends_clan_chat_msg_get(void *self, uint64_t raw, uint32_t idx, void *buff, uint32_t cap, uint32_t *type_out, libresteam_steam_id_t *chatter_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t chatter = LDG_STRUCT_ZERO_INIT;
    uint32_t type = LIBRESTEAM_SHIM_FRIENDS_CHAT_ENTRY_TYPE_INVALID;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_MSG_LEN_NONE; }

    chat.all = raw;
    res = libresteam_friends_clan_chat_msg_get(set, chat, idx, buff, cap, &type, &chatter, &len);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_GET, res, "chat: %u64; idx: %u32; chatter: %u64; type: %u32; len: %u32", raw, idx, chatter.all, type, len);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_MSG_LEN_NONE; }

    if (type_out) { *type_out = type; }

    if (chatter_out) { chatter_out->all = chatter.all; }

    return len;
}

static uint8_t libresteam_shim_friends_clan_chat_admin_get(void *self, uint64_t chat_raw, uint64_t user_raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_ADMIN_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    chat.all = chat_raw;
    user.all = user_raw;
    res = libresteam_friends_clan_chat_admin_get(set, chat, user, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_ADMIN_GET, res, "chat: %u64; user: %u64; admin: %u32", chat_raw, user_raw, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static uint8_t libresteam_shim_friends_clan_chat_window_open_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    chat.all = raw;
    res = libresteam_friends_clan_chat_window_open_get(set, chat, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET, res, "chat: %u64; open: %u32", raw, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static uint8_t libresteam_shim_friends_clan_chat_window_open(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    chat.all = raw;
    res = libresteam_friends_clan_chat_window_open(set, chat);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN, res, "chat: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint8_t libresteam_shim_friends_clan_chat_window_close(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t chat = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    chat.all = raw;
    res = libresteam_friends_clan_chat_window_close(set, chat);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE, res, "chat: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint8_t libresteam_shim_friends_msg_listen_set(void *self, uint8_t en)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_MSG_LISTEN_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (en) { flag = LDG_TRUTH_TRUE; }

    res = libresteam_friends_msg_listen_set(set, flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_MSG_LISTEN_SET, res, "en: %u32", (uint32_t)flag);

    return libresteam_shim_friends_aok_is(res);
}

static uint8_t libresteam_shim_friends_msg_reply(void *self, uint64_t raw, const char *text)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_MSG_REPLY, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    id.all = raw;
    res = libresteam_friends_msg_reply(set, id, text);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_MSG_REPLY, res, "id: %u64", raw);

    return libresteam_shim_friends_aok_is(res);
}

static uint32_t libresteam_shim_friends_msg_get(void *self, uint64_t raw, uint32_t msg_id, void *buff, uint32_t cap, uint32_t *type_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t type = LIBRESTEAM_SHIM_FRIENDS_CHAT_ENTRY_TYPE_INVALID;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_MSG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_MSG_LEN_NONE; }

    id.all = raw;
    res = libresteam_friends_msg_get(set, id, msg_id, buff, cap, &type, &len);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_MSG_GET, res, "id: %u64; msg_id: %u32; type: %u32; len: %u32", raw, msg_id, type, len);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_MSG_LEN_NONE; }

    if (type_out) { *type_out = type; }

    return len;
}

static uint64_t libresteam_shim_friends_follower_cunt_req(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWER_CUNT_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    user.all = raw;
    res = libresteam_friends_follower_cunt_req(set, user, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWER_CUNT_REQ, res, "user: %u64; call: %u64", raw, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_friends_following_req(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    user.all = raw;
    res = libresteam_friends_following_req(set, user, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_REQ, res, "user: %u64; call: %u64", raw, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_friends_following_list_req(void *self, uint32_t start)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_FRIENDS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_LIST_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    res = libresteam_friends_following_list_req(set, start, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_LIST_REQ, res, "start: %u32; call: %u64", start, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_friends_clan_public_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_PUBLIC_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    clan.all = raw;
    res = libresteam_friends_clan_public_get(set, clan, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_PUBLIC_GET, res, "clan: %u64; public: %u32", raw, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static uint8_t libresteam_shim_friends_clan_official_get(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t clan = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICIAL_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    clan.all = raw;
    res = libresteam_friends_clan_official_get(set, clan, &flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICIAL_GET, res, "clan: %u64; official: %u32", raw, (uint32_t)flag);

    return libresteam_shim_friends_flag_get(res, flag);
}

static uint32_t libresteam_shim_friends_unread_chat_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_UNREAD_CHAT_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    res = libresteam_friends_unread_chat_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_UNREAD_CHAT_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FRIENDS_NONE; }

    return cunt;
}

static void libresteam_shim_friends_overlay_remote_play_invite_activate(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    lobby.all = raw;
    res = libresteam_friends_overlay_remote_play_invite_activate(set, lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE, res, "lobby: %u64", raw);
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_friends_overlay_protocol_reg(void *self, const char *proto)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_PROTOCOL_REG, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_friends_overlay_protocol_reg(set, proto);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_PROTOCOL_REG, res, "proto: %s", LIBRESTEAM_SHIM_FRIENDS_STR(proto));

    return libresteam_shim_friends_aok_is(res);
}

static void libresteam_shim_friends_overlay_invite_connect_activate(void *self, const char *connect)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_friends_overlay_invite_connect_activate(set, connect);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE, res, "connect_set: %u32", (uint32_t)(connect != 0x0));
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_friends_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET:
        { *out = "GetPersonaName";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_SET:
        { *out = "SetPersonaName";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_STATE_GET:
        { *out = "GetPersonaState";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_CUNT_GET:
        { *out = "GetFriendCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET:
        { *out = "GetFriendByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_REL_GET:
        { *out = "GetFriendRelationship";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_STATE_GET:
        { *out = "GetFriendPersonaState";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET:
        { *out = "GetFriendPersonaName";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_GAME_PLAYED_GET:
        { *out = "GetFriendGamePlayed";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET:
        { *out = "GetFriendPersonaNameHistory";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_STEAM_LEVEL_GET:
        { *out = "GetFriendSteamLevel";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET:
        { *out = "GetPlayerNickname";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_CUNT_GET:
        { *out = "GetFriendsGroupCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_GET:
        { *out = "GetFriendsGroupIDByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET:
        { *out = "GetFriendsGroupName";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBER_CUNT_GET:
        { *out = "GetFriendsGroupMembersCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBERS_GET:
        { *out = "GetFriendsGroupMembersList";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_HAS_FRIEND_GET:
        { *out = "HasFriend";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CUNT_GET:
        { *out = "GetClanCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_GET:
        { *out = "GetClanByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET:
        { *out = "GetClanName";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET:
        { *out = "GetClanTag";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_GET:
        { *out = "GetClanActivityCounts";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_REQ:
        { *out = "DownloadClanActivityCounts";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_CUNT_GET:
        { *out = "GetFriendCountFromSource";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET:
        { *out = "GetFriendFromSourceByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_SRC_USER_IN_GET:
        { *out = "IsUserInSource";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_VOICE_SPEAKING_SET:
        { *out = "SetInGameVoiceSpeaking";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ACTIVATE:
        { *out = "ActivateGameOverlay";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_USER_ACTIVATE:
        { *out = "ActivateGameOverlayToUser";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_WEB_ACTIVATE:
        { *out = "ActivateGameOverlayToWebPage";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ST_ACTIVATE:
        { *out = "ActivateGameOverlayToStore";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_PLAYED_WITH_SET:
        { *out = "SetPlayedWith";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_ACTIVATE:
        { *out = "ActivateGameOverlayInviteDialog";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_SMALL_GET:
        { *out = "GetSmallFriendAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_MEDIUM_GET:
        { *out = "GetMediumFriendAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_LARGE_GET:
        { *out = "GetLargeFriendAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_USER_INFO_REQ:
        { *out = "RequestUserInformation";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_REQ:
        { *out = "RequestClanOfficerList";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET:
        { *out = "GetClanOwner";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_CUNT_GET:
        { *out = "GetClanOfficerCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET:
        { *out = "GetClanOfficerByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_USER_RESTRICTIONS_GET:
        { *out = "GetUserRestrictions";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_SET:
        { *out = "SetRichPresence";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_CLEAR:
        { *out = "ClearRichPresence";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET:
        { *out = "GetFriendRichPresence";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET:
        { *out = "GetFriendRichPresenceKeyCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET:
        { *out = "GetFriendRichPresenceKeyByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_REQ:
        { *out = "RequestFriendRichPresence";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_GAME_INVITE:
        { *out = "InviteUserToGame";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_CUNT_GET:
        { *out = "GetCoplayFriendCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET:
        { *out = "GetCoplayFriend";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_GET:
        { *out = "GetFriendCoplayTime";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_GAME_GET:
        { *out = "GetFriendCoplayGame";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_JOIN:
        { *out = "JoinClanChatRoom";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_LEAVE:
        { *out = "LeaveClanChatRoom";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET:
        { *out = "GetClanChatMemberCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET:
        { *out = "GetChatMemberByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_SEND:
        { *out = "SendClanChatMessage";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_GET:
        { *out = "GetClanChatMessage";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_ADMIN_GET:
        { *out = "IsClanChatAdmin";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET:
        { *out = "IsClanChatWindowOpenInSteam";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN:
        { *out = "OpenClanChatWindowInSteam";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE:
        { *out = "CloseClanChatWindowInSteam";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_MSG_LISTEN_SET:
        { *out = "SetListenForFriendsMessages";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_MSG_REPLY:
        { *out = "ReplyToFriendMessage";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_MSG_GET:
        { *out = "GetFriendMessage";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWER_CUNT_REQ:
        { *out = "GetFollowerCount";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_REQ:
        { *out = "IsFollowing";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_LIST_REQ:
        { *out = "EnumerateFollowingList";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_PUBLIC_GET:
        { *out = "IsClanPublic";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICIAL_GET:
        { *out = "IsClanOfficialGameGroup";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_UNREAD_CHAT_CUNT_GET:
        { *out = "GetNumChatsWithUnreadPriorityMessages";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE:
        { *out = "ActivateGameOverlayRemotePlayTogetherInviteDialog";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_PROTOCOL_REG:
        { *out = "RegisterProtocolInOverlayBrowser";
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE:
        { *out = "ActivateGameOverlayInviteDialogConnectString";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_friends_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_FRIENDS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET:
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET:
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET:
        case LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET:
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET:
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET:
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_GET:
        case LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET:
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET:
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET:
        { *out = LIBRESTEAM_SHIM_RET_STRUCT;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_friends_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_persona_name_get)
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_NAME_SET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_persona_name_set)
        case LIBRESTEAM_SHIM_FRIENDS_PERSONA_STATE_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_persona_state_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_REL_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_rel_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_STATE_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_persona_state_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_PERSONA_NAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_persona_name_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_GAME_PLAYED_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_game_played_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_NAME_HISTORY_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_name_history_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_STEAM_LEVEL_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_steam_level_get)
        case LIBRESTEAM_SHIM_FRIENDS_NICKNAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_nickname_get)
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_group_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_group_get)
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_NAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_group_name_get)
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBER_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_group_member_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_GROUP_MEMBERS_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_group_members_get)
        case LIBRESTEAM_SHIM_FRIENDS_HAS_FRIEND_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_has_friend_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_NAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_name_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_TAG_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_tag_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_activity_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_ACTIVITY_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_activity_req)
        case LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_src_friend_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_SRC_FRIEND_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_src_friend_get)
        case LIBRESTEAM_SHIM_FRIENDS_SRC_USER_IN_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_src_user_in_get)
        case LIBRESTEAM_SHIM_FRIENDS_VOICE_SPEAKING_SET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_voice_speaking_set)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_activate)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_USER_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_user_activate)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_WEB_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_web_activate)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_ST_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_st_activate)
        case LIBRESTEAM_SHIM_FRIENDS_PLAYED_WITH_SET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_played_with_set)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_invite_activate)
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_SMALL_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_avatar_small_get)
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_MEDIUM_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_avatar_medium_get)
        case LIBRESTEAM_SHIM_FRIENDS_AVATAR_LARGE_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_avatar_large_get)
        case LIBRESTEAM_SHIM_FRIENDS_USER_INFO_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_user_info_req)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_officer_req)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OWNER_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_owner_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_officer_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICER_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_officer_get)
        case LIBRESTEAM_SHIM_FRIENDS_USER_RESTRICTIONS_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_user_restrictions_get)
        case LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_SET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_rich_presence_set)
        case LIBRESTEAM_SHIM_FRIENDS_RICH_PRESENCE_CLEAR:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_rich_presence_clear)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_rich_presence_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_rich_presence_key_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_rich_presence_key_get)
        case LIBRESTEAM_SHIM_FRIENDS_FRIEND_RICH_PRESENCE_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_friend_rich_presence_req)
        case LIBRESTEAM_SHIM_FRIENDS_GAME_INVITE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_game_invite)
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_coplay_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_coplay_get)
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_TIME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_coplay_time_get)
        case LIBRESTEAM_SHIM_FRIENDS_COPLAY_GAME_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_coplay_game_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_JOIN:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_join)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_LEAVE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_leave)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_member_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MEMBER_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_member_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_SEND:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_msg_send)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_MSG_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_msg_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_ADMIN_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_admin_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_window_open_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_OPEN:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_window_open)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_CHAT_WINDOW_CLOSE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_chat_window_close)
        case LIBRESTEAM_SHIM_FRIENDS_MSG_LISTEN_SET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_msg_listen_set)
        case LIBRESTEAM_SHIM_FRIENDS_MSG_REPLY:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_msg_reply)
        case LIBRESTEAM_SHIM_FRIENDS_MSG_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_msg_get)
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWER_CUNT_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_follower_cunt_req)
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_following_req)
        case LIBRESTEAM_SHIM_FRIENDS_FOLLOWING_LIST_REQ:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_following_list_req)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_PUBLIC_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_public_get)
        case LIBRESTEAM_SHIM_FRIENDS_CLAN_OFFICIAL_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_clan_official_get)
        case LIBRESTEAM_SHIM_FRIENDS_UNREAD_CHAT_CUNT_GET:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_unread_chat_cunt_get)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_REMOTE_PLAY_INVITE_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_remote_play_invite_activate)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_PROTOCOL_REG:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_protocol_reg)
        case LIBRESTEAM_SHIM_FRIENDS_OVERLAY_INVITE_CONNECT_ACTIVATE:
            LIBRESTEAM_SHIM_FRIENDS_IMPL(libresteam_shim_friends_overlay_invite_connect_activate)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_friends_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_FRIENDS_VERSION;
    out->name_get = libresteam_shim_friends_name_get;
    out->kind_get = libresteam_shim_friends_kind_get;
    out->impl_get = libresteam_shim_friends_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_FRIENDS_METHOD_CUNT;

    return LDG_ERR_AOK;
}
