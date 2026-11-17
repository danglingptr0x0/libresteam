#ifndef LIBRESTEAM_FRIENDS_H
#define LIBRESTEAM_FRIENDS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>

LDG_EXPORT uint32_t libresteam_friends_persona_name_get(const libresteam_backend_set_t *set, const char **out);
LDG_EXPORT uint32_t libresteam_friends_persona_state_get(const libresteam_backend_set_t *set, libresteam_friends_persona_state_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_cunt_get(const libresteam_backend_set_t *set, uint32_t flags, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t flags, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_rel_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_rel_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_persona_state_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_persona_state_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_persona_name_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char **out);
LDG_EXPORT uint32_t libresteam_friends_rich_presence_set(const libresteam_backend_set_t *set, const char *key, const char *val);
LDG_EXPORT uint32_t libresteam_friends_rich_presence_clear(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_friends_friend_rich_presence_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_friends_friend_rich_presence_key_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_rich_presence_key_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t idx, const char **out);
LDG_EXPORT uint32_t libresteam_friends_friend_rich_presence_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id);
LDG_EXPORT uint32_t libresteam_friends_played_with_set(const libresteam_backend_set_t *set, libresteam_steam_id_t id);
LDG_EXPORT uint32_t libresteam_friends_friend_game_played_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, libresteam_friends_game_info_t *info_out, uint8_t *playing_out);
LDG_EXPORT uint32_t libresteam_friends_friend_app_rich_presence_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_friends_friend_app_rich_presence_key_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_name_history_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t idx, const char **out);
LDG_EXPORT uint32_t libresteam_friends_friend_steam_level_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_nickname_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char **out);
LDG_EXPORT uint32_t libresteam_friends_group_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_group_get(const libresteam_backend_set_t *set, uint32_t idx, uint16_t *out);
LDG_EXPORT uint32_t libresteam_friends_group_name_get(const libresteam_backend_set_t *set, uint16_t group, const char **out);
LDG_EXPORT uint32_t libresteam_friends_group_member_cunt_get(const libresteam_backend_set_t *set, uint16_t group, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_group_members_get(const libresteam_backend_set_t *set, uint16_t group, libresteam_steam_id_t *member, uint32_t cap);
LDG_EXPORT uint32_t libresteam_friends_has_friend_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t flags, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_name_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, const char **out);
LDG_EXPORT uint32_t libresteam_friends_clan_tag_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, const char **out);
LDG_EXPORT uint32_t libresteam_friends_clan_activity_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *online_out, uint32_t *in_game_out, uint32_t *chatting_out);
LDG_EXPORT uint32_t libresteam_friends_clan_activity_req(const libresteam_backend_set_t *set, libresteam_steam_id_t *clan, uint32_t cunt, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_src_friend_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t src, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_src_friend_get(const libresteam_backend_set_t *set, libresteam_steam_id_t src, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_src_user_in_get(const libresteam_backend_set_t *set, libresteam_steam_id_t user, libresteam_steam_id_t src, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_voice_speaking_set(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint8_t speaking);
LDG_EXPORT uint32_t libresteam_friends_overlay_activate(const libresteam_backend_set_t *set, const char *dialog);
LDG_EXPORT uint32_t libresteam_friends_overlay_user_activate(const libresteam_backend_set_t *set, const char *dialog, libresteam_steam_id_t id);
LDG_EXPORT uint32_t libresteam_friends_overlay_web_activate(const libresteam_backend_set_t *set, const char *url, uint32_t mode);
LDG_EXPORT uint32_t libresteam_friends_overlay_st_activate(const libresteam_backend_set_t *set, uint32_t app_id, uint32_t flag);
LDG_EXPORT uint32_t libresteam_friends_overlay_invite_activate(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby);
LDG_EXPORT uint32_t libresteam_friends_avatar_small_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_avatar_medium_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_avatar_large_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_user_info_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint8_t name_only, uint8_t *pending_out);
LDG_EXPORT uint32_t libresteam_friends_clan_officer_req(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_clan_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_officer_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_officer_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_game_invite(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *connect);
LDG_EXPORT uint32_t libresteam_friends_coplay_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_coplay_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_coplay_time_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_coplay_game_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_join(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_leave(const libresteam_backend_set_t *set, libresteam_steam_id_t clan);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_member_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_member_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_msg_send(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, const char *text);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_msg_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, uint32_t idx, void *buff, uint32_t cap, uint32_t *type_out, libresteam_steam_id_t *chatter_out, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_admin_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, libresteam_steam_id_t user, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_window_open_get(const libresteam_backend_set_t *set, libresteam_steam_id_t chat, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_window_open(const libresteam_backend_set_t *set, libresteam_steam_id_t chat);
LDG_EXPORT uint32_t libresteam_friends_clan_chat_window_close(const libresteam_backend_set_t *set, libresteam_steam_id_t chat);
LDG_EXPORT uint32_t libresteam_friends_msg_listen_set(const libresteam_backend_set_t *set, uint8_t en);
LDG_EXPORT uint32_t libresteam_friends_msg_reply(const libresteam_backend_set_t *set, libresteam_steam_id_t id, const char *text);
LDG_EXPORT uint32_t libresteam_friends_msg_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t msg_id, void *buff, uint32_t cap, uint32_t *type_out, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_friends_follower_cunt_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_following_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_following_list_req(const libresteam_backend_set_t *set, uint32_t start, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_clan_public_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_clan_official_get(const libresteam_backend_set_t *set, libresteam_steam_id_t clan, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_unread_chat_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_overlay_remote_play_invite_activate(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby);
LDG_EXPORT uint32_t libresteam_friends_overlay_protocol_reg(const libresteam_backend_set_t *set, const char *proto);
LDG_EXPORT uint32_t libresteam_friends_overlay_invite_connect_activate(const libresteam_backend_set_t *set, const char *connect);
LDG_EXPORT uint32_t libresteam_friends_profile_items_req(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_friends_profile_item_has_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint8_t *out);
LDG_EXPORT uint32_t libresteam_friends_profile_item_str_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint32_t prop, const char **out);
LDG_EXPORT uint32_t libresteam_friends_profile_item_u32_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t type, uint32_t prop, uint32_t *out);
LDG_EXPORT uint32_t libresteam_friends_friend_app_rich_presence_key_get(const libresteam_backend_set_t *set, libresteam_steam_id_t id, uint32_t app_id, uint32_t idx, const char **out);

#endif
