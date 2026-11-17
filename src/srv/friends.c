#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/str/str.h>

#include <libresteam/friends/ops.h>
#include <libresteam/srv/srv.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

#define SRV_FRIENDS_STR_EMPTY ""
#define SRV_FRIENDS_STR_EQUAL LDG_ARITH_ORD_EQ
#define SRV_FRIENDS_FRIEND_NONE 0U
#define SRV_FRIENDS_ID_SIZE 8U
#define SRV_FRIENDS_VAL_NONE_LEN 0U

static uint32_t srv_friends_peer_get(libresteam_srv_t *srv, uint64_t id_bits, libresteam_srv_client_t **out)
{
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0;

    *out = 0x0;
    id.all = id_bits;

    for (idx = 0; idx < LIBRESTEAM_SRV_CLIENT_MAX; idx++)
    {
        if (srv->client[idx].live != LDG_TRUTH_TRUE || srv->client[idx].hello_done != LDG_TRUTH_TRUE) { continue; }

        if (srv->client[idx].account != id.comp.account) { continue; }

        *out = &srv->client[idx];
        return LDG_ERR_AOK;
    }

    return LDG_ERR_NOT_FOUND;
}

static uint32_t srv_friends_presence_find(const libresteam_srv_client_t *client, const char *key, uint32_t *idx_out)
{
    uint32_t idx = 0;
    uint32_t cmp = UINT32_MAX;

    *idx_out = UINT32_MAX;

    for (idx = 0; idx < client->presence_cunt; idx++)
    {
        if (ldg_str_cmp(client->presence[idx].key, key, LIBRESTEAM_SRV_PRESENCE_KEY_CAP, &cmp) != LDG_ERR_AOK) { continue; }

        if (cmp != SRV_FRIENDS_STR_EQUAL) { continue; }

        *idx_out = idx;
        return LDG_ERR_AOK;
    }

    return LDG_ERR_NOT_FOUND;
}

static uint32_t srv_friends_presence_set(libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    char key[LIBRESTEAM_SRV_PRESENCE_KEY_CAP] = LDG_ARR_ZERO_INIT;
    char val[LIBRESTEAM_SRV_PRESENCE_VAL_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = UINT32_MAX;
    uint32_t last = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_str_get(rd, key, LIBRESTEAM_SRV_PRESENCE_KEY_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_get(rd, val, LIBRESTEAM_SRV_PRESENCE_VAL_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_friends_presence_find(client, key, &idx);

    if (val[0] == LDG_STR_TERM)
    {
        if (res != LDG_ERR_AOK) { return LDG_ERR_AOK; }

        client->presence_cunt--;
        last = client->presence_cunt;
        if (idx != last) { client->presence[idx] = client->presence[last]; }

        return LDG_ERR_AOK;
    }

    if (res != LDG_ERR_AOK)
    {
        if (client->presence_cunt >= LIBRESTEAM_FRIENDS_RICH_PRESENCE_KEY_CUNT_MAX) { return LDG_ERR_FULL; }

        idx = client->presence_cunt;
        client->presence_cunt++;
        res = ldg_str_rbrcpy(client->presence[idx].key, key, LIBRESTEAM_SRV_PRESENCE_KEY_CAP);
        if (res != LDG_ERR_AOK) { return res; }
    }

    return ldg_str_rbrcpy(client->presence[idx].val, val, LIBRESTEAM_SRV_PRESENCE_VAL_CAP);
}

static uint32_t srv_friends_peer_presence_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_srv_client_t **peer_out)
{
    uint64_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    *peer_out = 0x0;

    res = libresteam_wire_u64_get(rd, &id);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_friends_peer_get(srv, id, peer_out);
    if (res != LDG_ERR_AOK || (*peer_out)->app_id != client->app_id) { *peer_out = 0x0; }

    return LDG_ERR_AOK;
}

static uint32_t srv_friends_rich_presence_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_client_t *peer = 0x0;
    char key[LIBRESTEAM_SRV_PRESENCE_KEY_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    res = srv_friends_peer_presence_get(srv, client, rd, &peer);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_get(rd, key, LIBRESTEAM_SRV_PRESENCE_KEY_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    if (!peer || srv_friends_presence_find(peer, key, &idx) != LDG_ERR_AOK) { return libresteam_wire_str_put(wr, SRV_FRIENDS_STR_EMPTY); }

    return libresteam_wire_str_put(wr, peer->presence[idx].val);
}

static uint32_t srv_friends_rich_presence_key_cunt_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_client_t *peer = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_friends_peer_presence_get(srv, client, rd, &peer);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_put(wr, peer ? peer->presence_cunt : SRV_FRIENDS_FRIEND_NONE);
}

static uint32_t srv_friends_rich_presence_key_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_client_t *peer = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_friends_peer_presence_get(srv, client, rd, &peer);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &idx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!peer || idx >= peer->presence_cunt) { return LDG_ERR_NOT_FOUND; }

    return libresteam_wire_str_put(wr, peer->presence[idx].key);
}

static uint32_t srv_friends_peer_state_get(libresteam_srv_t *srv, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_client_t *peer = 0x0;
    uint64_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u64_get(rd, &id);
    if (res != LDG_ERR_AOK) { return res; }

    if (srv_friends_peer_get(srv, id, &peer) == LDG_ERR_AOK) { return libresteam_wire_u32_put(wr, LIBRESTEAM_FRIENDS_PERSONA_STATE_ONLINE); }

    return libresteam_wire_u32_put(wr, LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE);
}

static uint32_t srv_friends_peer_name_get(libresteam_srv_t *srv, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_client_t *peer = 0x0;
    uint64_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u64_get(rd, &id);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_friends_peer_get(srv, id, &peer);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_str_put(wr, peer->name);
}

static uint32_t srv_friends_played_with_set(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    libresteam_wire_wr_t key = LDG_STRUCT_ZERO_INIT;
    uint8_t key_buff[SRV_FRIENDS_ID_SIZE] = LDG_ARR_ZERO_INIT;
    uint64_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u64_get(rd, &id);
    if (res != LDG_ERR_AOK) { return res; }

    key.buff = key_buff;
    key.cap = SRV_FRIENDS_ID_SIZE;

    res = libresteam_wire_u64_put(&key, id);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_srv_account_rec_append(srv, client, LIBRESTEAM_SRV_REC_KIND_PLAYED_WITH, key.buff, key.len, 0x0, SRV_FRIENDS_VAL_NONE_LEN);
}

uint32_t libresteam_srv_friends_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    uint64_t id = 0;
    uint32_t idx = 0;
    uint32_t flags = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !client || !rd || !wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    switch (op)
    {
        case LIBRESTEAM_WIRE_OP_FRIENDS_PERSONA_NAME_GET:
            return libresteam_wire_str_put(wr, client->name);
        case LIBRESTEAM_WIRE_OP_FRIENDS_PERSONA_STATE_GET:
            return libresteam_wire_u32_put(wr, LIBRESTEAM_FRIENDS_PERSONA_STATE_ONLINE);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_CUNT_GET:
            res = libresteam_wire_u32_get(rd, &flags);
            if (res != LDG_ERR_AOK) { return res; }

            return libresteam_wire_u32_put(wr, SRV_FRIENDS_FRIEND_NONE);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_GET:
            res = libresteam_wire_u32_get(rd, &idx);
            if (res != LDG_ERR_AOK) { return res; }

            res = libresteam_wire_u32_get(rd, &flags);
            if (res != LDG_ERR_AOK) { return res; }

            return LDG_ERR_NOT_FOUND;
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_REL_GET:
            res = libresteam_wire_u64_get(rd, &id);
            if (res != LDG_ERR_AOK) { return res; }

            return libresteam_wire_u32_put(wr, LIBRESTEAM_FRIENDS_REL_NONE);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_PERSONA_STATE_GET:
            return srv_friends_peer_state_get(srv, rd, wr);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_PERSONA_NAME_GET:
            return srv_friends_peer_name_get(srv, rd, wr);
        case LIBRESTEAM_WIRE_OP_FRIENDS_RICH_PRESENCE_SET:
            return srv_friends_presence_set(client, rd);
        case LIBRESTEAM_WIRE_OP_FRIENDS_RICH_PRESENCE_CLEAR:
            client->presence_cunt = 0;
            return LDG_ERR_AOK;
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_GET:
            return srv_friends_rich_presence_get(srv, client, rd, wr);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET:
            return srv_friends_rich_presence_key_cunt_get(srv, client, rd, wr);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET:
            return srv_friends_rich_presence_key_get(srv, client, rd, wr);
        case LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_REQ:
            return libresteam_wire_u64_get(rd, &id);
        case LIBRESTEAM_WIRE_OP_FRIENDS_PLAYED_WITH_SET:
            return srv_friends_played_with_set(srv, client, rd);
        default:
            return LDG_ERR_UNSUPPORTED;
    }
}
