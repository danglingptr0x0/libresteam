#ifndef LIBRESTEAM_FRIENDS_OPS_H
#define LIBRESTEAM_FRIENDS_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_FRIENDS_FLAG_NONE 0x0000U
#define LIBRESTEAM_FRIENDS_FLAG_BLK 0x0001U
#define LIBRESTEAM_FRIENDS_FLAG_REQ_RECEIVED 0x0002U
#define LIBRESTEAM_FRIENDS_FLAG_IMM 0x0004U
#define LIBRESTEAM_FRIENDS_FLAG_CLAN_MEMBER 0x0008U
#define LIBRESTEAM_FRIENDS_FLAG_ON_GAME_SRV 0x0010U
#define LIBRESTEAM_FRIENDS_FLAG_REQ_SENT 0x0080U
#define LIBRESTEAM_FRIENDS_FLAG_REQ_INFO 0x0100U
#define LIBRESTEAM_FRIENDS_FLAG_IGNORED 0x0200U
#define LIBRESTEAM_FRIENDS_FLAG_IGNORED_FRIEND 0x0400U
#define LIBRESTEAM_FRIENDS_FLAG_CHAT_MEMBER 0x1000U
#define LIBRESTEAM_FRIENDS_FLAG_ALL 0xFFFFU

#define LIBRESTEAM_FRIENDS_PERSONA_NAME_MAX 128U
#define LIBRESTEAM_FRIENDS_RICH_PRESENCE_KEY_CUNT_MAX 30U
#define LIBRESTEAM_FRIENDS_RICH_PRESENCE_KEY_LEN_MAX 64U
#define LIBRESTEAM_FRIENDS_RICH_PRESENCE_VAL_LEN_MAX 256U

typedef enum libresteam_friends_rel
{
    LIBRESTEAM_FRIENDS_REL_NONE = 0,
    LIBRESTEAM_FRIENDS_REL_BLK,
    LIBRESTEAM_FRIENDS_REL_REQ_RECIPIENT,
    LIBRESTEAM_FRIENDS_REL_FRIEND,
    LIBRESTEAM_FRIENDS_REL_REQ_INITIATOR,
    LIBRESTEAM_FRIENDS_REL_IGNORED,
    LIBRESTEAM_FRIENDS_REL_IGNORED_FRIEND,
    LIBRESTEAM_FRIENDS_REL_SUGGESTED_DEPRECATED,
    LIBRESTEAM_FRIENDS_REL_MAX
} libresteam_friends_rel_t;

typedef enum libresteam_friends_persona_state
{
    LIBRESTEAM_FRIENDS_PERSONA_STATE_OFFLINE = 0,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_ONLINE,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_BUSY,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_AWAY,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_SNOOZE,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_LOOKING_TO_TRADE,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_LOOKING_TO_PLAY,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_INVISIBLE,
    LIBRESTEAM_FRIENDS_PERSONA_STATE_MAX
} libresteam_friends_persona_state_t;

#define LIBRESTEAM_FRIENDS_GAME_ID_APP_MASK 0x00FFFFFFU

typedef struct libresteam_friends_game_info
{
    uint64_t game_id;
    uint32_t game_ip;
    uint16_t game_port;
    uint16_t query_port;
    libresteam_steam_id_t lobby;
} LDG_PACKED libresteam_friends_game_info_t;

typedef struct libresteam_friends_ext_ops
{
    uint32_t (*friend_name_history_get)(void *ctx, libresteam_steam_id_t id, uint32_t idx, const char **out);
    uint32_t (*friend_steam_level_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*nickname_get)(void *ctx, libresteam_steam_id_t id, const char **out);
    uint32_t (*group_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*group_get)(void *ctx, uint32_t idx, uint16_t *out);
    uint32_t (*group_name_get)(void *ctx, uint16_t group, const char **out);
    uint32_t (*group_member_cunt_get)(void *ctx, uint16_t group, uint32_t *out);
    uint32_t (*group_members_get)(void *ctx, uint16_t group, libresteam_steam_id_t *member, uint32_t cap);
    uint32_t (*has_friend_get)(void *ctx, libresteam_steam_id_t id, uint32_t flags, uint8_t *out);
    uint32_t (*clan_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*clan_get)(void *ctx, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*clan_name_get)(void *ctx, libresteam_steam_id_t clan, const char **out);
    uint32_t (*clan_tag_get)(void *ctx, libresteam_steam_id_t clan, const char **out);
    uint32_t (*clan_activity_get)(void *ctx, libresteam_steam_id_t clan, uint32_t *online_out, uint32_t *in_game_out, uint32_t *chatting_out);
    uint32_t (*clan_activity_req)(void *ctx, libresteam_steam_id_t *clan, uint32_t cunt, uint64_t *call_out);
    uint32_t (*src_friend_cunt_get)(void *ctx, libresteam_steam_id_t src, uint32_t *out);
    uint32_t (*src_friend_get)(void *ctx, libresteam_steam_id_t src, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*src_user_in_get)(void *ctx, libresteam_steam_id_t user, libresteam_steam_id_t src, uint8_t *out);
    uint32_t (*voice_speaking_set)(void *ctx, libresteam_steam_id_t user, uint8_t speaking);
    uint32_t (*overlay_activate)(void *ctx, const char *dialog);
    uint32_t (*overlay_user_activate)(void *ctx, const char *dialog, libresteam_steam_id_t id);
    uint32_t (*overlay_web_activate)(void *ctx, const char *url, uint32_t mode);
    uint32_t (*overlay_st_activate)(void *ctx, uint32_t app_id, uint32_t flag);
    uint32_t (*overlay_invite_activate)(void *ctx, libresteam_steam_id_t lobby);
    uint32_t (*avatar_small_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*avatar_medium_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*avatar_large_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*user_info_req)(void *ctx, libresteam_steam_id_t id, uint8_t name_only, uint8_t *pending_out);
    uint32_t (*clan_officer_req)(void *ctx, libresteam_steam_id_t clan, uint64_t *call_out);
    uint32_t (*clan_owner_get)(void *ctx, libresteam_steam_id_t clan, libresteam_steam_id_t *out);
    uint32_t (*clan_officer_cunt_get)(void *ctx, libresteam_steam_id_t clan, uint32_t *out);
    uint32_t (*clan_officer_get)(void *ctx, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*game_invite)(void *ctx, libresteam_steam_id_t id, const char *connect);
    uint32_t (*coplay_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*coplay_get)(void *ctx, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*coplay_time_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*coplay_game_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*clan_chat_join)(void *ctx, libresteam_steam_id_t clan, uint64_t *call_out);
    uint32_t (*clan_chat_leave)(void *ctx, libresteam_steam_id_t clan);
    uint32_t (*clan_chat_member_cunt_get)(void *ctx, libresteam_steam_id_t clan, uint32_t *out);
    uint32_t (*clan_chat_member_get)(void *ctx, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*clan_chat_msg_send)(void *ctx, libresteam_steam_id_t chat, const char *text);
    uint32_t (*clan_chat_msg_get)(void *ctx, libresteam_steam_id_t chat, uint32_t idx, void *buff, uint32_t cap, uint32_t *type_out, libresteam_steam_id_t *chatter_out, uint32_t *len_out);
    uint32_t (*clan_chat_admin_get)(void *ctx, libresteam_steam_id_t chat, libresteam_steam_id_t user, uint8_t *out);
    uint32_t (*clan_chat_window_open_get)(void *ctx, libresteam_steam_id_t chat, uint8_t *out);
    uint32_t (*clan_chat_window_open)(void *ctx, libresteam_steam_id_t chat);
    uint32_t (*clan_chat_window_close)(void *ctx, libresteam_steam_id_t chat);
    uint32_t (*msg_listen_set)(void *ctx, uint8_t en);
    uint32_t (*msg_reply)(void *ctx, libresteam_steam_id_t id, const char *text);
    uint32_t (*msg_get)(void *ctx, libresteam_steam_id_t id, uint32_t msg_id, void *buff, uint32_t cap, uint32_t *type_out, uint32_t *len_out);
    uint32_t (*follower_cunt_req)(void *ctx, libresteam_steam_id_t id, uint64_t *call_out);
    uint32_t (*following_req)(void *ctx, libresteam_steam_id_t id, uint64_t *call_out);
    uint32_t (*following_list_req)(void *ctx, uint32_t start, uint64_t *call_out);
    uint32_t (*clan_public_get)(void *ctx, libresteam_steam_id_t clan, uint8_t *out);
    uint32_t (*clan_official_get)(void *ctx, libresteam_steam_id_t clan, uint8_t *out);
    uint32_t (*unread_chat_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*overlay_remote_play_invite_activate)(void *ctx, libresteam_steam_id_t lobby);
    uint32_t (*overlay_protocol_reg)(void *ctx, const char *proto);
    uint32_t (*overlay_invite_connect_activate)(void *ctx, const char *connect);
    uint32_t (*profile_items_req)(void *ctx, libresteam_steam_id_t id, uint64_t *call_out);
    uint32_t (*profile_item_has_get)(void *ctx, libresteam_steam_id_t id, uint32_t type, uint8_t *out);
    uint32_t (*profile_item_str_get)(void *ctx, libresteam_steam_id_t id, uint32_t type, uint32_t prop, const char **out);
    uint32_t (*profile_item_u32_get)(void *ctx, libresteam_steam_id_t id, uint32_t type, uint32_t prop, uint32_t *out);
} libresteam_friends_ext_ops_t;

typedef struct libresteam_friends_ops
{
    uint32_t (*persona_name_get)(void *ctx, const char **out);
    uint32_t (*persona_state_get)(void *ctx, libresteam_friends_persona_state_t *out);
    uint32_t (*friend_cunt_get)(void *ctx, uint32_t flags, uint32_t *out);
    uint32_t (*friend_get)(void *ctx, uint32_t idx, uint32_t flags, libresteam_steam_id_t *out);
    uint32_t (*friend_rel_get)(void *ctx, libresteam_steam_id_t id, libresteam_friends_rel_t *out);
    uint32_t (*friend_persona_state_get)(void *ctx, libresteam_steam_id_t id, libresteam_friends_persona_state_t *out);
    uint32_t (*friend_persona_name_get)(void *ctx, libresteam_steam_id_t id, const char **out);
    uint32_t (*rich_presence_set)(void *ctx, const char *key, const char *val);
    uint32_t (*rich_presence_clear)(void *ctx);
    uint32_t (*friend_rich_presence_get)(void *ctx, libresteam_steam_id_t id, const char *key, const char **out);
    uint32_t (*friend_rich_presence_key_cunt_get)(void *ctx, libresteam_steam_id_t id, uint32_t *out);
    uint32_t (*friend_rich_presence_key_get)(void *ctx, libresteam_steam_id_t id, uint32_t idx, const char **out);
    uint32_t (*friend_rich_presence_req)(void *ctx, libresteam_steam_id_t id);
    uint32_t (*played_with_set)(void *ctx, libresteam_steam_id_t id);
    uint32_t (*friend_game_played_get)(void *ctx, libresteam_steam_id_t id, libresteam_friends_game_info_t *info_out, uint8_t *playing_out);
    uint32_t (*friend_app_rich_presence_get)(void *ctx, libresteam_steam_id_t id, uint32_t app_id, const char *key, const char **out);
    uint32_t (*friend_app_rich_presence_key_cunt_get)(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t *out);
    uint32_t (*friend_app_rich_presence_key_get)(void *ctx, libresteam_steam_id_t id, uint32_t app_id, uint32_t idx, const char **out);
    const libresteam_friends_ext_ops_t *ext;
} libresteam_friends_ops_t;

#endif
