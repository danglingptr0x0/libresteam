#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/srv/srv.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

#define SRV_FRIENDS_STR_EMPTY ""

static uint32_t srv_friends_id_req(libresteam_backend_srv_t *srv, libresteam_steam_id_t id, libresteam_wire_wr_t *wr)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_backend_srv_req_begin(srv, wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u64_put(wr, id.all);
}

static uint32_t srv_friends_str_reply(libresteam_backend_srv_t *srv, libresteam_wire_rd_t *rd, const char **out)
{
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_str_get(rd, srv->str, LIBRESTEAM_BACKEND_SRV_STR_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    *out = srv->str;

    return LDG_ERR_AOK;
}

static uint32_t srv_friends_persona_state_reply(libresteam_wire_rd_t *rd, libresteam_friends_persona_state_t *out)
{
    uint32_t state = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u32_get(rd, &state);
    if (res != LDG_ERR_AOK) { return res; }

    if (state >= LIBRESTEAM_FRIENDS_PERSONA_STATE_MAX) { return LDG_ERR_RANGE; }

    *out = (libresteam_friends_persona_state_t)state;

    return LDG_ERR_AOK;
}

static uint32_t srv_friends_persona_name_get(void *ctx, const char **out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_PERSONA_NAME_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_str_reply((libresteam_backend_srv_t *)ctx, &rd, out);
}

static uint32_t srv_friends_persona_state_get(void *ctx, libresteam_friends_persona_state_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_PERSONA_STATE_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_persona_state_reply(&rd, out);
}

static uint32_t srv_friends_friend_cunt_get(void *ctx, uint32_t flags, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, flags);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_CUNT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_friends_friend_get(void *ctx, uint32_t idx, uint32_t flags, libresteam_steam_id_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint64_t id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, idx);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, flags);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u64_get(&rd, &id);
    if (res != LDG_ERR_AOK) { return res; }

    out->all = id;

    return LDG_ERR_AOK;
}

static uint32_t srv_friends_friend_rel_get(void *ctx, libresteam_steam_id_t id, libresteam_friends_rel_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t rel = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_REL_NONE;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_REL_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(&rd, &rel);
    if (res != LDG_ERR_AOK) { return res; }

    if (rel >= LIBRESTEAM_FRIENDS_REL_MAX) { return LDG_ERR_RANGE; }

    *out = (libresteam_friends_rel_t)rel;

    return LDG_ERR_AOK;
}

static uint32_t srv_friends_friend_persona_state_get(void *ctx, libresteam_steam_id_t id, libresteam_friends_persona_state_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_PERSONA_STATE_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_persona_state_reply(&rd, out);
}

static uint32_t srv_friends_friend_persona_name_get(void *ctx, libresteam_steam_id_t id, const char **out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_PERSONA_NAME_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_str_reply((libresteam_backend_srv_t *)ctx, &rd, out);
}

static uint32_t srv_friends_rich_presence_set(void *ctx, const char *key, const char *val)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_put(&wr, key);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_put(&wr, val ? val : SRV_FRIENDS_STR_EMPTY);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_RICH_PRESENCE_SET, &wr, &rd);
}

static uint32_t srv_friends_rich_presence_clear(void *ctx)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_RICH_PRESENCE_CLEAR, &wr, &rd);
}

static uint32_t srv_friends_friend_rich_presence_get(void *ctx, libresteam_steam_id_t id, const char *key, const char **out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_put(&wr, key);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_str_reply((libresteam_backend_srv_t *)ctx, &rd, out);
}

static uint32_t srv_friends_friend_rich_presence_key_cunt_get(void *ctx, libresteam_steam_id_t id, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_KEY_CUNT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_friends_friend_rich_presence_key_get(void *ctx, libresteam_steam_id_t id, uint32_t idx, const char **out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, idx);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_KEY_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_friends_str_reply((libresteam_backend_srv_t *)ctx, &rd, out);
}

static uint32_t srv_friends_friend_rich_presence_req(void *ctx, libresteam_steam_id_t id)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_FRIEND_RICH_PRESENCE_REQ, &wr, &rd);
}

static uint32_t srv_friends_played_with_set(void *ctx, libresteam_steam_id_t id)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_friends_id_req((libresteam_backend_srv_t *)ctx, id, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_FRIENDS, LIBRESTEAM_WIRE_OP_FRIENDS_PLAYED_WITH_SET, &wr, &rd);
}

uint32_t libresteam_backend_srv_friends_ops_fill(libresteam_backend_srv_t *srv)
{
    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->friends_ops.persona_name_get = srv_friends_persona_name_get;
    srv->friends_ops.persona_state_get = srv_friends_persona_state_get;
    srv->friends_ops.friend_cunt_get = srv_friends_friend_cunt_get;
    srv->friends_ops.friend_get = srv_friends_friend_get;
    srv->friends_ops.friend_rel_get = srv_friends_friend_rel_get;
    srv->friends_ops.friend_persona_state_get = srv_friends_friend_persona_state_get;
    srv->friends_ops.friend_persona_name_get = srv_friends_friend_persona_name_get;
    srv->friends_ops.rich_presence_set = srv_friends_rich_presence_set;
    srv->friends_ops.rich_presence_clear = srv_friends_rich_presence_clear;
    srv->friends_ops.friend_rich_presence_get = srv_friends_friend_rich_presence_get;
    srv->friends_ops.friend_rich_presence_key_cunt_get = srv_friends_friend_rich_presence_key_cunt_get;
    srv->friends_ops.friend_rich_presence_key_get = srv_friends_friend_rich_presence_key_get;
    srv->friends_ops.friend_rich_presence_req = srv_friends_friend_rich_presence_req;
    srv->friends_ops.played_with_set = srv_friends_played_with_set;
    srv->friends_ops.ext = 0x0;
    srv->ops.friends = &srv->friends_ops;

    return LDG_ERR_AOK;
}
