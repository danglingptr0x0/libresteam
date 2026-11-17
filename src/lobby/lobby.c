#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/lobby/lobby.h>
#include <libresteam/lobby/ops.h>
#include <libresteam/steam_id/steam_id.h>

static uint32_t lobby_ops_get(const libresteam_backend_set_t *set, const libresteam_lobby_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->lobby) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->lobby;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_lobby_favorite_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_cunt_get(ctx, out);
}

uint32_t libresteam_lobby_favorite_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_lobby_favorite_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_get(ctx, idx, out);
}

uint32_t libresteam_lobby_favorite_add(const libresteam_backend_set_t *set, const libresteam_lobby_favorite_t *fav, uint32_t *idx_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *idx_out = 0;

    if (LDG_UNLIKELY(!set || !fav)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_add(ctx, fav, idx_out);
}

uint32_t libresteam_lobby_favorite_remove(const libresteam_backend_set_t *set, const libresteam_lobby_favorite_t *fav)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !fav)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_remove(ctx, fav);
}

uint32_t libresteam_lobby_list_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_req(ctx, call_out);
}

uint32_t libresteam_lobby_list_str_filter_add(const libresteam_backend_set_t *set, const char *key, const char *val, uint32_t cmp)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_str_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_str_filter_add(ctx, key, val, cmp);
}

uint32_t libresteam_lobby_list_numeric_filter_add(const libresteam_backend_set_t *set, const char *key, uint32_t val, uint32_t cmp)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_numeric_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_numeric_filter_add(ctx, key, val, cmp);
}

uint32_t libresteam_lobby_list_near_filter_add(const libresteam_backend_set_t *set, const char *key, uint32_t val)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_near_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_near_filter_add(ctx, key, val);
}

uint32_t libresteam_lobby_list_slot_filter_add(const libresteam_backend_set_t *set, uint32_t slot_cunt)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_slot_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_slot_filter_add(ctx, slot_cunt);
}

uint32_t libresteam_lobby_list_dist_filter_add(const libresteam_backend_set_t *set, libresteam_lobby_dist_t dist)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(dist >= LIBRESTEAM_LOBBY_DIST_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_dist_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_dist_filter_add(ctx, dist);
}

uint32_t libresteam_lobby_list_res_cunt_filter_add(const libresteam_backend_set_t *set, uint32_t res_cunt_max)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_res_cunt_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_res_cunt_filter_add(ctx, res_cunt_max);
}

uint32_t libresteam_lobby_list_compatible_member_filter_add(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_compatible_member_filter_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_compatible_member_filter_add(ctx, lobby);
}

uint32_t libresteam_lobby_list_lobby_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_lobby_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_lobby_get(ctx, idx, out);
}

uint32_t libresteam_lobby_create(const libresteam_backend_set_t *set, libresteam_lobby_type_t type, uint32_t member_max, uint64_t *call_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(type >= LIBRESTEAM_LOBBY_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->create) { return LDG_ERR_UNSUPPORTED; }

    return ops->create(ctx, type, member_max, call_out);
}

uint32_t libresteam_lobby_join(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint64_t *call_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->join) { return LDG_ERR_UNSUPPORTED; }

    return ops->join(ctx, lobby, call_out);
}

uint32_t libresteam_lobby_leave(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->leave) { return LDG_ERR_UNSUPPORTED; }

    return ops->leave(ctx, lobby);
}

uint32_t libresteam_lobby_invite(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t invitee)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->invite) { return LDG_ERR_UNSUPPORTED; }

    return ops->invite(ctx, lobby, invitee);
}

uint32_t libresteam_lobby_member_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_cunt_get(ctx, lobby, out);
}

uint32_t libresteam_lobby_member_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_get(ctx, lobby, idx, out);
}

uint32_t libresteam_lobby_data_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char **out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_get(ctx, lobby, key, out);
}

uint32_t libresteam_lobby_data_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char *val)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_set(ctx, lobby, key, val);
}

uint32_t libresteam_lobby_data_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_cunt_get(ctx, lobby, out);
}

uint32_t libresteam_lobby_data_idx_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t idx, char *key_out, uint32_t key_cap, char *val_out, uint32_t val_cap)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key_out || !val_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(key_cap == 0 || val_cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    key_out[0] = LDG_STR_TERM;
    val_out[0] = LDG_STR_TERM;

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_idx_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_idx_get(ctx, lobby, idx, key_out, key_cap, val_out, val_cap);
}

uint32_t libresteam_lobby_data_delete(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_delete) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_delete(ctx, lobby, key);
}

uint32_t libresteam_lobby_member_data_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t user, const char *key, const char **out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_data_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_data_get(ctx, lobby, user, key, out);
}

uint32_t libresteam_lobby_member_data_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char *val)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_data_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_data_set(ctx, lobby, key, val);
}

uint32_t libresteam_lobby_chat_msg_send(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const void *body, uint32_t len)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !body)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->chat_msg_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->chat_msg_send(ctx, lobby, body, len);
}

uint32_t libresteam_lobby_chat_entry_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t chat_id, libresteam_steam_id_t *user_out, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *entry_type_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!user_out || !len_out || !entry_type_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    user_out->all = 0;
    *len_out = 0;
    *entry_type_out = 0;

    if (LDG_UNLIKELY(!set || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->chat_entry_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->chat_entry_get(ctx, lobby, chat_id, user_out, buff, cap, len_out, entry_type_out);
}

uint32_t libresteam_lobby_data_req(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->data_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->data_req(ctx, lobby);
}

uint32_t libresteam_lobby_game_srv_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t ip, uint16_t port, libresteam_steam_id_t srv)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_srv_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_srv_set(ctx, lobby, ip, port, srv);
}

uint32_t libresteam_lobby_game_srv_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *ip_out, uint16_t *port_out, libresteam_steam_id_t *srv_out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ip_out || !port_out || !srv_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ip_out = 0;
    *port_out = 0;
    srv_out->all = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_srv_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_srv_get(ctx, lobby, ip_out, port_out, srv_out);
}

uint32_t libresteam_lobby_member_limit_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t member_max)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_limit_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_limit_set(ctx, lobby, member_max);
}

uint32_t libresteam_lobby_member_limit_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->member_limit_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->member_limit_get(ctx, lobby, out);
}

uint32_t libresteam_lobby_type_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_lobby_type_t type)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(type >= LIBRESTEAM_LOBBY_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->type_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->type_set(ctx, lobby, type);
}

uint32_t libresteam_lobby_joinable_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint8_t joinable)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->joinable_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->joinable_set(ctx, lobby, joinable);
}

uint32_t libresteam_lobby_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t *out)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->owner_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->owner_get(ctx, lobby, out);
}

uint32_t libresteam_lobby_owner_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t owner)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->owner_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->owner_set(ctx, lobby, owner);
}

uint32_t libresteam_lobby_linked_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t dependent)
{
    const libresteam_lobby_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = lobby_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->linked_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->linked_set(ctx, lobby, dependent);
}
