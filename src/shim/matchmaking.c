#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/lobby/lobby.h>
#include <libresteam/lobby/ops.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_SHIM_MATCHMAKING_VERSION "SteamMatchMaking009"
#define LIBRESTEAM_SHIM_MATCHMAKING_METHOD_CUNT 39U
#define LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_CUNT_GET 0U
#define LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_GET 1U
#define LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_ADD 2U
#define LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_REMOVE 3U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_REQ 4U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_STR_FILTER_ADD 5U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD 6U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_NEAR_FILTER_ADD 7U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_SLOT_FILTER_ADD 8U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_DIST_FILTER_ADD 9U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD 10U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD 11U
#define LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET 12U
#define LIBRESTEAM_SHIM_MATCHMAKING_CREATE 13U
#define LIBRESTEAM_SHIM_MATCHMAKING_JOIN 14U
#define LIBRESTEAM_SHIM_MATCHMAKING_LEAVE 15U
#define LIBRESTEAM_SHIM_MATCHMAKING_INVITE 16U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_CUNT_GET 17U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET 18U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET 19U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_SET 20U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_CUNT_GET 21U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_IDX_GET 22U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_DELETE 23U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET 24U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_SET 25U
#define LIBRESTEAM_SHIM_MATCHMAKING_CHAT_MSG_SEND 26U
#define LIBRESTEAM_SHIM_MATCHMAKING_CHAT_ENTRY_GET 27U
#define LIBRESTEAM_SHIM_MATCHMAKING_DATA_REQ 28U
#define LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_SET 29U
#define LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_GET 30U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_SET 31U
#define LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_GET 32U
#define LIBRESTEAM_SHIM_MATCHMAKING_TYPE_SET 33U
#define LIBRESTEAM_SHIM_MATCHMAKING_JOINABLE_SET 34U
#define LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET 35U
#define LIBRESTEAM_SHIM_MATCHMAKING_OWNER_SET 36U
#define LIBRESTEAM_SHIM_MATCHMAKING_LINKED_SET 37U
#define LIBRESTEAM_SHIM_MATCHMAKING_PSN_INVITE_CK 38U
#define LIBRESTEAM_SHIM_MATCHMAKING_NONE 0U
#define LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE 0U
#define LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_IDX_NONE 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY ""
#define LIBRESTEAM_SHIM_MATCHMAKING_STR(str) ((str) ? (str) : LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY)
#define LIBRESTEAM_SHIM_MATCHMAKING_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_matchmaking_aok_is(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_matchmaking_favorite_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    res = libresteam_lobby_favorite_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    return cunt;
}

static uint8_t libresteam_shim_matchmaking_favorite_get(void *self, uint32_t idx, uint32_t *app_id_out, uint32_t *ip_out, uint16_t *conn_port_out, uint16_t *query_port_out, uint32_t *flags_out, uint32_t *last_played_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_lobby_favorite_t fav = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_lobby_favorite_get(set, idx, &fav);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_GET, res, "idx: %u32; app_id: %u32; flags: %x32; last_played: %u32", idx, fav.app_id, fav.flags, fav.last_played);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (app_id_out) { *app_id_out = fav.app_id; }

    if (ip_out) { *ip_out = fav.ip; }

    if (conn_port_out) { *conn_port_out = fav.conn_port; }

    if (query_port_out) { *query_port_out = fav.query_port; }

    if (flags_out) { *flags_out = fav.flags; }

    if (last_played_out) { *last_played_out = fav.last_played; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_matchmaking_favorite_add(void *self, uint32_t app_id, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint32_t last_played)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_lobby_favorite_t fav = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_IDX_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_ADD, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_IDX_NONE; }

    fav.app_id = app_id;
    fav.ip = ip;
    fav.conn_port = conn_port;
    fav.query_port = query_port;
    fav.flags = flags;
    fav.last_played = last_played;
    res = libresteam_lobby_favorite_add(set, &fav, &idx);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_ADD, res, "app_id: %u32; flags: %x32; last_played: %u32; idx: %u32", app_id, flags, last_played, idx);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_IDX_NONE; }

    return idx;
}

static uint8_t libresteam_shim_matchmaking_favorite_remove(void *self, uint32_t app_id, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_lobby_favorite_t fav = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_REMOVE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    fav.app_id = app_id;
    fav.ip = ip;
    fav.conn_port = conn_port;
    fav.query_port = query_port;
    fav.flags = flags;
    res = libresteam_lobby_favorite_remove(set, &fav);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_REMOVE, res, "app_id: %u32; flags: %x32", app_id, flags);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint64_t libresteam_shim_matchmaking_list_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    res = libresteam_lobby_list_req(set, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_REQ, res, "call: %u64", call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    return call;
}

static void libresteam_shim_matchmaking_list_str_filter_add(void *self, const char *key, const char *val, uint32_t cmp)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_STR_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_lobby_list_str_filter_add(set, key, val, cmp);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_STR_FILTER_ADD, res, "key: %s; val: %s; cmp: %i32", LIBRESTEAM_SHIM_MATCHMAKING_STR(key), LIBRESTEAM_SHIM_MATCHMAKING_STR(val), cmp);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_numeric_filter_add(void *self, const char *key, uint32_t val, uint32_t cmp)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_lobby_list_numeric_filter_add(set, key, val, cmp);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD, res, "key: %s; val: %i32; cmp: %i32", LIBRESTEAM_SHIM_MATCHMAKING_STR(key), val, cmp);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_near_filter_add(void *self, const char *key, uint32_t val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_NEAR_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_lobby_list_near_filter_add(set, key, val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_NEAR_FILTER_ADD, res, "key: %s; val: %i32", LIBRESTEAM_SHIM_MATCHMAKING_STR(key), val);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_slot_filter_add(void *self, uint32_t slot_cunt)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_SLOT_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_lobby_list_slot_filter_add(set, slot_cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_SLOT_FILTER_ADD, res, "slots: %u32", slot_cunt);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_dist_filter_add(void *self, uint32_t dist)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_RANGE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_DIST_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    if (dist < (uint32_t)LIBRESTEAM_LOBBY_DIST_MAX) { res = libresteam_lobby_list_dist_filter_add(set, (libresteam_lobby_dist_t)dist); }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_DIST_FILTER_ADD, res, "dist: %u32", dist);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_res_cunt_filter_add(void *self, uint32_t res_cunt_max)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_lobby_list_res_cunt_filter_add(set, res_cunt_max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD, res, "max: %u32", res_cunt_max);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_matchmaking_list_compatible_filter_add(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD, &set) != LDG_ERR_AOK) { return; }

    lobby.all = lobby_id;
    res = libresteam_lobby_list_compatible_member_filter_add(set, lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD, res, "lobby: %u64", lobby_id);
    if (res != LDG_ERR_AOK) { return; }
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_matchmaking_list_lobby_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(lobby); }

    res = libresteam_lobby_list_lobby_get(set, idx, &lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET, res, "idx: %u32; lobby: %u64", idx, lobby.all);
    if (res != LDG_ERR_AOK) { lobby.all = LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    LDG_CPP_STRUCT_RET(lobby);
}

static uint64_t libresteam_shim_matchmaking_create(void *self, uint32_t type, uint32_t member_max)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE;
    uint32_t res = LDG_ERR_RANGE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    if (type < (uint32_t)LIBRESTEAM_LOBBY_TYPE_MAX) { res = libresteam_lobby_create(set, (libresteam_lobby_type_t)type, member_max, &call); }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_CREATE, res, "type: %u32; max: %u32; call: %u64", type, member_max, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_matchmaking_join(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_JOIN, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_join(set, lobby, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_JOIN, res, "lobby: %u64; call: %u64", lobby_id, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_CALL_NONE; }

    return call;
}

static void libresteam_shim_matchmaking_leave(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LEAVE, &set) != LDG_ERR_AOK) { return; }

    lobby.all = lobby_id;
    res = libresteam_lobby_leave(set, lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LEAVE, res, "lobby: %u64", lobby_id);
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_matchmaking_invite(void *self, uint64_t lobby_id, uint64_t invitee_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t invitee = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_INVITE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    invitee.all = invitee_id;
    res = libresteam_lobby_invite(set, lobby, invitee);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_INVITE, res, "lobby: %u64; invitee: %u64", lobby_id, invitee_id);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint32_t libresteam_shim_matchmaking_member_cunt_get(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_member_cunt_get(set, lobby, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_CUNT_GET, res, "lobby: %u64; cunt: %u32", lobby_id, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    return cunt;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_matchmaking_member_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t lobby_id, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t member = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(member); }

    lobby.all = lobby_id;
    res = libresteam_lobby_member_get(set, lobby, idx, &member);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET, res, "lobby: %u64; idx: %u32; member: %u64", lobby_id, idx, member.all);
    if (res != LDG_ERR_AOK) { member.all = LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    LDG_CPP_STRUCT_RET(member);
}

static const char* libresteam_shim_matchmaking_data_get(void *self, uint64_t lobby_id, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_get(set, lobby, key, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET, res, "lobby: %u64; key: %s", lobby_id, LIBRESTEAM_SHIM_MATCHMAKING_STR(key));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_matchmaking_data_set(void *self, uint64_t lobby_id, const char *key, const char *val)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_set(set, lobby, key, val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_SET, res, "lobby: %u64; key: %s; val: %s", lobby_id, LIBRESTEAM_SHIM_MATCHMAKING_STR(key), LIBRESTEAM_SHIM_MATCHMAKING_STR(val));

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint32_t libresteam_shim_matchmaking_data_cunt_get(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_cunt_get(set, lobby, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_CUNT_GET, res, "lobby: %u64; cunt: %u32", lobby_id, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    return cunt;
}

static uint8_t libresteam_shim_matchmaking_data_idx_get(void *self, uint64_t lobby_id, uint32_t idx, char *key_out, uint32_t key_cap, char *val_out, uint32_t val_cap)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_IDX_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_idx_get(set, lobby, idx, key_out, key_cap, val_out, val_cap);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_IDX_GET, res, "lobby: %u64; idx: %u32; key: %s; val: %s", lobby_id, idx, (res == LDG_ERR_AOK) ? LIBRESTEAM_SHIM_MATCHMAKING_STR(key_out) : LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY, (res == LDG_ERR_AOK) ? LIBRESTEAM_SHIM_MATCHMAKING_STR(val_out) : LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint8_t libresteam_shim_matchmaking_data_delete(void *self, uint64_t lobby_id, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_DELETE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_delete(set, lobby, key);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_DELETE, res, "lobby: %u64; key: %s", lobby_id, LIBRESTEAM_SHIM_MATCHMAKING_STR(key));

    return libresteam_shim_matchmaking_aok_is(res);
}

static const char* libresteam_shim_matchmaking_member_data_get(void *self, uint64_t lobby_id, uint64_t user_id, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY; }

    lobby.all = lobby_id;
    user.all = user_id;
    res = libresteam_lobby_member_data_get(set, lobby, user, key, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET, res, "lobby: %u64; user: %u64; key: %s", lobby_id, user_id, LIBRESTEAM_SHIM_MATCHMAKING_STR(key));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_MATCHMAKING_STR_EMPTY; }

    return str;
}

static void libresteam_shim_matchmaking_member_data_set(void *self, uint64_t lobby_id, const char *key, const char *val)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_SET, &set) != LDG_ERR_AOK) { return; }

    lobby.all = lobby_id;
    res = libresteam_lobby_member_data_set(set, lobby, key, val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_SET, res, "lobby: %u64; key: %s; val: %s", lobby_id, LIBRESTEAM_SHIM_MATCHMAKING_STR(key), LIBRESTEAM_SHIM_MATCHMAKING_STR(val));
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_matchmaking_chat_msg_send(void *self, uint64_t lobby_id, const void *body, uint32_t len)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_CHAT_MSG_SEND, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_chat_msg_send(set, lobby, body, len);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_CHAT_MSG_SEND, res, "lobby: %u64; len: %u32", lobby_id, len);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint32_t libresteam_shim_matchmaking_chat_entry_get(void *self, uint64_t lobby_id, uint32_t chat_id, libresteam_steam_id_t *user_out, void *buff, uint32_t cap, uint32_t *entry_type_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint32_t len = 0;
    uint32_t entry_type = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_CHAT_ENTRY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_chat_entry_get(set, lobby, chat_id, &user, buff, cap, &len, &entry_type);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_CHAT_ENTRY_GET, res, "lobby: %u64; chat_id: %u32; user: %u64; len: %u32; type: %u32", lobby_id, chat_id, user.all, len, entry_type);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    if (user_out) { user_out->all = user.all; }

    if (entry_type_out) { *entry_type_out = entry_type; }

    return len;
}

static uint8_t libresteam_shim_matchmaking_data_req(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_REQ, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_data_req(set, lobby);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_DATA_REQ, res, "lobby: %u64", lobby_id);

    return libresteam_shim_matchmaking_aok_is(res);
}

static void libresteam_shim_matchmaking_game_srv_set(void *self, uint64_t lobby_id, uint32_t ip, uint16_t port, uint64_t srv_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t srv = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_SET, &set) != LDG_ERR_AOK) { return; }

    lobby.all = lobby_id;
    srv.all = srv_id;
    res = libresteam_lobby_game_srv_set(set, lobby, ip, port, srv);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_SET, res, "lobby: %u64; srv: %u64", lobby_id, srv_id);
    if (res != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_matchmaking_game_srv_get(void *self, uint64_t lobby_id, uint32_t *ip_out, uint16_t *port_out, libresteam_steam_id_t *srv_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t srv = LDG_STRUCT_ZERO_INIT;
    uint32_t ip = 0;
    uint16_t port = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_game_srv_get(set, lobby, &ip, &port, &srv);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_GET, res, "lobby: %u64; srv: %u64", lobby_id, srv.all);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (ip_out) { *ip_out = ip; }

    if (port_out) { *port_out = port; }

    if (srv_out) { srv_out->all = srv.all; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_matchmaking_member_limit_set(void *self, uint64_t lobby_id, uint32_t member_max)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_member_limit_set(set, lobby, member_max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_SET, res, "lobby: %u64; max: %u32", lobby_id, member_max);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint32_t libresteam_shim_matchmaking_member_limit_get(void *self, uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t limit = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    lobby.all = lobby_id;
    res = libresteam_lobby_member_limit_get(set, lobby, &limit);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_GET, res, "lobby: %u64; max: %u32", lobby_id, limit);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    return limit;
}

static uint8_t libresteam_shim_matchmaking_type_set(void *self, uint64_t lobby_id, uint32_t type)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_RANGE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_TYPE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    if (type < (uint32_t)LIBRESTEAM_LOBBY_TYPE_MAX) { res = libresteam_lobby_type_set(set, lobby, (libresteam_lobby_type_t)type); }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_TYPE_SET, res, "lobby: %u64; type: %u32", lobby_id, type);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint8_t libresteam_shim_matchmaking_joinable_set(void *self, uint64_t lobby_id, uint8_t joinable)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_JOINABLE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    if (joinable) { flag = LDG_TRUTH_TRUE; }

    res = libresteam_lobby_joinable_set(set, lobby, flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_JOINABLE_SET, res, "lobby: %u64; joinable: %u32", lobby_id, (uint32_t)flag);

    return libresteam_shim_matchmaking_aok_is(res);
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_matchmaking_owner_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint64_t lobby_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t owner = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(owner); }

    lobby.all = lobby_id;
    res = libresteam_lobby_owner_get(set, lobby, &owner);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET, res, "lobby: %u64; owner: %u64", lobby_id, owner.all);
    if (res != LDG_ERR_AOK) { owner.all = LIBRESTEAM_SHIM_MATCHMAKING_NONE; }

    LDG_CPP_STRUCT_RET(owner);
}

static uint8_t libresteam_shim_matchmaking_owner_set(void *self, uint64_t lobby_id, uint64_t owner_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t owner = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_OWNER_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    owner.all = owner_id;
    res = libresteam_lobby_owner_set(set, lobby, owner);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_OWNER_SET, res, "lobby: %u64; owner: %u64", lobby_id, owner_id);

    return libresteam_shim_matchmaking_aok_is(res);
}

static uint8_t libresteam_shim_matchmaking_linked_set(void *self, uint64_t lobby_id, uint64_t dependent_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t lobby = LDG_STRUCT_ZERO_INIT;
    libresteam_steam_id_t dependent = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_LINKED_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    lobby.all = lobby_id;
    dependent.all = dependent_id;
    res = libresteam_lobby_linked_set(set, lobby, dependent);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_LINKED_SET, res, "lobby: %u64; dependent: %u64", lobby_id, dependent_id);

    return libresteam_shim_matchmaking_aok_is(res);
}

static void libresteam_shim_matchmaking_psn_invite_ck(void *self, uint32_t attrs)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_MATCHMAKING_PSN_INVITE_CK, &set) != LDG_ERR_AOK) { return; }

    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_MATCHMAKING_PSN_INVITE_CK, LDG_ERR_UNSUPPORTED, "attrs: %x32", attrs);
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_matchmaking_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_CUNT_GET:
        { *out = "GetFavoriteGameCount";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_GET:
        { *out = "GetFavoriteGame";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_ADD:
        { *out = "AddFavoriteGame";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_REMOVE:
        { *out = "RemoveFavoriteGame";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_REQ:
        { *out = "RequestLobbyList";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_STR_FILTER_ADD:
        { *out = "AddRequestLobbyListStringFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD:
        { *out = "AddRequestLobbyListNumericalFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_NEAR_FILTER_ADD:
        { *out = "AddRequestLobbyListNearValueFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_SLOT_FILTER_ADD:
        { *out = "AddRequestLobbyListFilterSlotsAvailable";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_DIST_FILTER_ADD:
        { *out = "AddRequestLobbyListDistanceFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD:
        { *out = "AddRequestLobbyListResultCountFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD:
        { *out = "AddRequestLobbyListCompatibleMembersFilter";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET:
        { *out = "GetLobbyByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_CREATE:
        { *out = "CreateLobby";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_JOIN:
        { *out = "JoinLobby";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LEAVE:
        { *out = "LeaveLobby";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_INVITE:
        { *out = "InviteUserToLobby";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_CUNT_GET:
        { *out = "GetNumLobbyMembers";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET:
        { *out = "GetLobbyMemberByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET:
        { *out = "GetLobbyData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_SET:
        { *out = "SetLobbyData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_CUNT_GET:
        { *out = "GetLobbyDataCount";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_IDX_GET:
        { *out = "GetLobbyDataByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_DELETE:
        { *out = "DeleteLobbyData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET:
        { *out = "GetLobbyMemberData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_SET:
        { *out = "SetLobbyMemberData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_CHAT_MSG_SEND:
        { *out = "SendLobbyChatMsg";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_CHAT_ENTRY_GET:
        { *out = "GetLobbyChatEntry";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_REQ:
        { *out = "RequestLobbyData";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_SET:
        { *out = "SetLobbyGameServer";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_GET:
        { *out = "GetLobbyGameServer";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_SET:
        { *out = "SetLobbyMemberLimit";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_GET:
        { *out = "GetLobbyMemberLimit";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_TYPE_SET:
        { *out = "SetLobbyType";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_JOINABLE_SET:
        { *out = "SetLobbyJoinable";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET:
        { *out = "GetLobbyOwner";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_OWNER_SET:
        { *out = "SetLobbyOwner";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LINKED_SET:
        { *out = "SetLinkedLobby";
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_PSN_INVITE_CK:
        { *out = "CheckForPSNGameBootInvite";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_matchmaking_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_MATCHMAKING_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET:
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET:
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET:
        case LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET:
        { *out = LIBRESTEAM_SHIM_RET_STRUCT;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_matchmaking_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_CUNT_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_favorite_cunt_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_favorite_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_favorite_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_FAVORITE_REMOVE:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_favorite_remove)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_REQ:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_req)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_STR_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_str_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_NUMERIC_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_numeric_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_NEAR_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_near_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_SLOT_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_slot_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_DIST_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_dist_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_RES_CUNT_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_res_cunt_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_COMPATIBLE_FILTER_ADD:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_compatible_filter_add)
        case LIBRESTEAM_SHIM_MATCHMAKING_LIST_LOBBY_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_list_lobby_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_CREATE:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_create)
        case LIBRESTEAM_SHIM_MATCHMAKING_JOIN:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_join)
        case LIBRESTEAM_SHIM_MATCHMAKING_LEAVE:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_leave)
        case LIBRESTEAM_SHIM_MATCHMAKING_INVITE:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_invite)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_CUNT_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_cunt_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_CUNT_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_cunt_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_IDX_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_idx_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_DELETE:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_delete)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_data_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_DATA_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_data_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_CHAT_MSG_SEND:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_chat_msg_send)
        case LIBRESTEAM_SHIM_MATCHMAKING_CHAT_ENTRY_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_chat_entry_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_DATA_REQ:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_data_req)
        case LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_game_srv_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_GAME_SRV_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_game_srv_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_limit_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_MEMBER_LIMIT_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_member_limit_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_TYPE_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_type_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_JOINABLE_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_joinable_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_OWNER_GET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_owner_get)
        case LIBRESTEAM_SHIM_MATCHMAKING_OWNER_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_owner_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_LINKED_SET:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_linked_set)
        case LIBRESTEAM_SHIM_MATCHMAKING_PSN_INVITE_CK:
            LIBRESTEAM_SHIM_MATCHMAKING_IMPL(libresteam_shim_matchmaking_psn_invite_ck)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_matchmaking_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_MATCHMAKING_VERSION;
    out->name_get = libresteam_shim_matchmaking_name_get;
    out->kind_get = libresteam_shim_matchmaking_kind_get;
    out->impl_get = libresteam_shim_matchmaking_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_MATCHMAKING_METHOD_CUNT;

    return LDG_ERR_AOK;
}
