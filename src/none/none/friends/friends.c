#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/logging/logging.h>

#include <libresteam/backend/backend.h>
#include <libresteam/friends/friends.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>

static uint32_t friends_primary_ops_get(const libresteam_backend_set_t *set, const libresteam_friends_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->friends) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->friends;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

static uint32_t friends_tee_ops_get(const libresteam_backend_set_t *set, uint32_t idx, const libresteam_friends_ops_t **ops_out, void **ctx_out)
{
    *ops_out = set->backend[idx].ops->friends;
    *ctx_out = set->backend[idx].ctx;

    if (!*ops_out) { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_friends_persona_name_get(const libresteam_backend_set_t *set, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->persona_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->persona_name_get(ctx, out);
}

uint32_t libresteam_friends_persona_state_get(const libresteam_backend_set_t *set, libresteam_friends_persona_state_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->persona_state_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->persona_state_get(ctx, out);
}

uint32_t libresteam_friends_friend_cunt_get(const libresteam_backend_set_t *set, uint32_t flags, uint32_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_cunt_get(ctx, flags, out);
}

uint32_t libresteam_friends_friend_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t flags, libresteam_steam_id_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_get(ctx, idx, flags, out);
}

uint32_t libresteam_friends_friend_rel_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_rel_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_REL_NONE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_rel_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_rel_get(ctx, id, out);
}

uint32_t libresteam_friends_friend_persona_state_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_persona_state_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_persona_state_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_persona_state_get(ctx, id, out);
}

uint32_t libresteam_friends_friend_persona_name_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_persona_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_persona_name_get(ctx, id, out);
}

uint32_t libresteam_friends_friend_rich_presence_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *key, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_rich_presence_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_rich_presence_get(ctx, id, key, out);
}

uint32_t libresteam_friends_friend_rich_presence_key_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_rich_presence_key_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_rich_presence_key_cunt_get(ctx, id, out);
}

uint32_t libresteam_friends_friend_rich_presence_key_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t idx, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_rich_presence_key_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_rich_presence_key_get(ctx, id, idx, out);
}

uint32_t libresteam_friends_friend_rich_presence_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_rich_presence_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_rich_presence_req(ctx, id);
}

uint32_t libresteam_friends_rich_presence_set(const libresteam_backend_set_t *set, const char *key, const char *val)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = friends_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->rich_presence_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->rich_presence_set(ctx, key, val); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "friends: tee backend rich_presence_set failed"); }
    }

    return primary_res;
}

uint32_t libresteam_friends_rich_presence_clear(const libresteam_backend_set_t *set)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = friends_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->rich_presence_clear) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->rich_presence_clear(ctx); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "friends: tee backend rich_presence_clear failed"); }
    }

    return primary_res;
}

uint32_t libresteam_friends_played_with_set(const libresteam_backend_set_t *set, libresteam_steam_id_t id)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = friends_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->played_with_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->played_with_set(ctx, id); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "friends: tee backend played_with_set failed"); }
    }

    return primary_res;
}

uint32_t libresteam_friends_friend_game_played_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_game_info_t *info_out, uint8_t *playing_out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!info_out || !playing_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *playing_out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_game_played_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_game_played_get(ctx, id, info_out, playing_out);
}

uint32_t libresteam_friends_friend_app_rich_presence_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, const char *key, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_app_rich_presence_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_app_rich_presence_get(ctx, id, app_id, key, out);
}

uint32_t libresteam_friends_friend_app_rich_presence_key_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t *out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_app_rich_presence_key_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_app_rich_presence_key_cunt_get(ctx, id, app_id, out);
}

uint32_t libresteam_friends_friend_app_rich_presence_key_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t idx, const char **out)
{
    const libresteam_friends_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = friends_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_app_rich_presence_key_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_app_rich_presence_key_get(ctx, id, app_id, idx, out);
}
