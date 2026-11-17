#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/friends/friends.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>

static uint32_t friends_ext_ops_get(const libresteam_backend_set_t *set, const libresteam_friends_ext_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->friends || !primary->ops->friends->ext) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->friends->ext;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_friends_friend_name_history_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t idx, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_name_history_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_name_history_get(ctx, id, idx, out);
}

uint32_t libresteam_friends_friend_steam_level_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->friend_steam_level_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->friend_steam_level_get(ctx, id, out);
}

uint32_t libresteam_friends_nickname_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->nickname_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->nickname_get(ctx, id, out);
}

uint32_t libresteam_friends_group_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->group_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->group_cunt_get(ctx, out);
}

uint32_t libresteam_friends_group_get(const libresteam_backend_set_t *set, uint32_t idx, uint16_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->group_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->group_get(ctx, idx, out);
}

uint32_t libresteam_friends_group_name_get(const libresteam_backend_set_t *set, uint16_t group, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->group_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->group_name_get(ctx, group, out);
}

uint32_t libresteam_friends_group_member_cunt_get(const libresteam_backend_set_t *set, uint16_t group, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->group_member_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->group_member_cunt_get(ctx, group, out);
}

uint32_t libresteam_friends_group_members_get(const libresteam_backend_set_t *set, uint16_t group, libresteam_steam_id_t *member, uint32_t cap)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->group_members_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->group_members_get(ctx, group, member, cap);
}

uint32_t libresteam_friends_has_friend_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t flags, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->has_friend_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->has_friend_get(ctx, id, flags, out);
}

uint32_t libresteam_friends_clan_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_cunt_get(ctx, out);
}

uint32_t libresteam_friends_clan_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_get(ctx, idx, out);
}

uint32_t libresteam_friends_clan_name_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_name_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_tag_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_tag_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_tag_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_activity_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *online_out, uint32_t *in_game_out, uint32_t *chatting_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_activity_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_activity_get(ctx, clan, online_out, in_game_out, chatting_out);
}

uint32_t libresteam_friends_clan_activity_req(const libresteam_backend_set_t *set, libresteam_steam_id_t *clan, uint32_t cunt, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_activity_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_activity_req(ctx, clan, cunt, call_out);
}

uint32_t libresteam_friends_src_friend_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t src, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->src_friend_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->src_friend_cunt_get(ctx, src, out);
}

uint32_t libresteam_friends_src_friend_get(const libresteam_backend_set_t *set, libresteam_steam_id_t src, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->src_friend_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->src_friend_get(ctx, src, idx, out);
}

uint32_t libresteam_friends_src_user_in_get(const libresteam_backend_set_t *set, libresteam_steam_id_t user, libresteam_steam_id_t src, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->src_user_in_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->src_user_in_get(ctx, user, src, out);
}

uint32_t libresteam_friends_voice_speaking_set(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint8_t speaking)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->voice_speaking_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->voice_speaking_set(ctx, user, speaking);
}

uint32_t libresteam_friends_overlay_activate(const libresteam_backend_set_t *set, const char *dialog)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_activate(ctx, dialog);
}

uint32_t libresteam_friends_overlay_user_activate(const libresteam_backend_set_t *set, const char *dialog, libresteam_steam_id_t id)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_user_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_user_activate(ctx, dialog, id);
}

uint32_t libresteam_friends_overlay_web_activate(const libresteam_backend_set_t *set, const char *url, uint32_t mode)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_web_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_web_activate(ctx, url, mode);
}

uint32_t libresteam_friends_overlay_st_activate(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t flag)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_st_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_st_activate(ctx, app_id, flag);
}

uint32_t libresteam_friends_overlay_invite_activate(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_invite_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_invite_activate(ctx, lobby);
}

uint32_t libresteam_friends_avatar_small_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->avatar_small_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->avatar_small_get(ctx, id, out);
}

uint32_t libresteam_friends_avatar_medium_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->avatar_medium_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->avatar_medium_get(ctx, id, out);
}

uint32_t libresteam_friends_avatar_large_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->avatar_large_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->avatar_large_get(ctx, id, out);
}

uint32_t libresteam_friends_user_info_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint8_t name_only, uint8_t *pending_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_info_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_info_req(ctx, id, name_only, pending_out);
}

uint32_t libresteam_friends_clan_officer_req(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_officer_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_officer_req(ctx, clan, call_out);
}

uint32_t libresteam_friends_clan_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_owner_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_owner_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_officer_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_officer_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_officer_cunt_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_officer_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_officer_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_officer_get(ctx, clan, idx, out);
}

uint32_t libresteam_friends_game_invite(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *connect)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_invite) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_invite(ctx, id, connect);
}

uint32_t libresteam_friends_coplay_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->coplay_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->coplay_cunt_get(ctx, out);
}

uint32_t libresteam_friends_coplay_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->coplay_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->coplay_get(ctx, idx, out);
}

uint32_t libresteam_friends_coplay_time_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->coplay_time_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->coplay_time_get(ctx, id, out);
}

uint32_t libresteam_friends_coplay_game_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->coplay_game_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->coplay_game_get(ctx, id, out);
}

uint32_t libresteam_friends_clan_chat_join(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_join) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_join(ctx, clan, call_out);
}

uint32_t libresteam_friends_clan_chat_leave(const libresteam_backend_set_t *set, libresteam_steam_id_t clan)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_leave) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_leave(ctx, clan);
}

uint32_t libresteam_friends_clan_chat_member_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_member_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_member_cunt_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_chat_member_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_member_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_member_get(ctx, clan, idx, out);
}

uint32_t libresteam_friends_clan_chat_msg_send(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, const char *text)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_msg_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_msg_send(ctx, chat, text);
}

uint32_t libresteam_friends_clan_chat_msg_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, uint32_t idx, void *buff, uint32_t cap, uint32_t *type_out, libresteam_steam_id_t *chatter_out, uint32_t *len_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_msg_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_msg_get(ctx, chat, idx, buff, cap, type_out, chatter_out, len_out);
}

uint32_t libresteam_friends_clan_chat_admin_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, libresteam_steam_id_t user, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_admin_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_admin_get(ctx, chat, user, out);
}

uint32_t libresteam_friends_clan_chat_window_open_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_window_open_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_window_open_get(ctx, chat, out);
}

uint32_t libresteam_friends_clan_chat_window_open(const libresteam_backend_set_t *set, libresteam_steam_id_t chat)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_window_open) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_window_open(ctx, chat);
}

uint32_t libresteam_friends_clan_chat_window_close(const libresteam_backend_set_t *set, libresteam_steam_id_t chat)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_chat_window_close) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_chat_window_close(ctx, chat);
}

uint32_t libresteam_friends_msg_listen_set(const libresteam_backend_set_t *set, uint8_t en)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msg_listen_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->msg_listen_set(ctx, en);
}

uint32_t libresteam_friends_msg_reply(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *text)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msg_reply) { return LDG_ERR_UNSUPPORTED; }

    return ops->msg_reply(ctx, id, text);
}

uint32_t libresteam_friends_msg_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t msg_id, void *buff, uint32_t cap, uint32_t *type_out, uint32_t *len_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msg_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->msg_get(ctx, id, msg_id, buff, cap, type_out, len_out);
}

uint32_t libresteam_friends_follower_cunt_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->follower_cunt_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->follower_cunt_req(ctx, id, call_out);
}

uint32_t libresteam_friends_following_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->following_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->following_req(ctx, id, call_out);
}

uint32_t libresteam_friends_following_list_req(const libresteam_backend_set_t *set, uint32_t start, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->following_list_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->following_list_req(ctx, start, call_out);
}

uint32_t libresteam_friends_clan_public_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_public_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_public_get(ctx, clan, out);
}

uint32_t libresteam_friends_clan_official_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->clan_official_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->clan_official_get(ctx, clan, out);
}

uint32_t libresteam_friends_unread_chat_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->unread_chat_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->unread_chat_cunt_get(ctx, out);
}

uint32_t libresteam_friends_overlay_remote_play_invite_activate(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_remote_play_invite_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_remote_play_invite_activate(ctx, lobby);
}

uint32_t libresteam_friends_overlay_protocol_reg(const libresteam_backend_set_t *set, const char *proto)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_protocol_reg) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_protocol_reg(ctx, proto);
}

uint32_t libresteam_friends_overlay_invite_connect_activate(const libresteam_backend_set_t *set, const char *connect)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_invite_connect_activate) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_invite_connect_activate(ctx, connect);
}

uint32_t libresteam_friends_profile_items_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->profile_items_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->profile_items_req(ctx, id, call_out);
}

uint32_t libresteam_friends_profile_item_has_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint8_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->profile_item_has_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->profile_item_has_get(ctx, id, type, out);
}

uint32_t libresteam_friends_profile_item_str_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint32_t prop, const char **out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->profile_item_str_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->profile_item_str_get(ctx, id, type, prop, out);
}

uint32_t libresteam_friends_profile_item_u32_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint32_t prop, uint32_t *out)
{
    const libresteam_friends_ext_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = friends_ext_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->profile_item_u32_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->profile_item_u32_get(ctx, id, type, prop, out);
}
