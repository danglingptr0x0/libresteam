#ifndef LIBRESTEAM_LOBBY_H
#define LIBRESTEAM_LOBBY_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/lobby/ops.h>
#include <libresteam/steam_id/steam_id.h>

LDG_EXPORT uint32_t libresteam_lobby_favorite_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_lobby_favorite_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_lobby_favorite_t *out);
LDG_EXPORT uint32_t libresteam_lobby_favorite_add(const libresteam_backend_set_t *set, const libresteam_lobby_favorite_t *fav, uint32_t *idx_out);
LDG_EXPORT uint32_t libresteam_lobby_favorite_remove(const libresteam_backend_set_t *set, const libresteam_lobby_favorite_t *fav);
LDG_EXPORT uint32_t libresteam_lobby_list_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_lobby_list_str_filter_add(const libresteam_backend_set_t *set, const char *key, const char *val, uint32_t cmp);
LDG_EXPORT uint32_t libresteam_lobby_list_numeric_filter_add(const libresteam_backend_set_t *set, const char *key, uint32_t val, uint32_t cmp);
LDG_EXPORT uint32_t libresteam_lobby_list_near_filter_add(const libresteam_backend_set_t *set, const char *key, uint32_t val);
LDG_EXPORT uint32_t libresteam_lobby_list_slot_filter_add(const libresteam_backend_set_t *set, uint32_t slot_cunt);
LDG_EXPORT uint32_t libresteam_lobby_list_dist_filter_add(const libresteam_backend_set_t *set, libresteam_lobby_dist_t dist);
LDG_EXPORT uint32_t libresteam_lobby_list_res_cunt_filter_add(const libresteam_backend_set_t *set, uint32_t res_cunt_max);
LDG_EXPORT uint32_t libresteam_lobby_list_compatible_member_filter_add(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby);
LDG_EXPORT uint32_t libresteam_lobby_list_lobby_get(const libresteam_backend_set_t *set, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_lobby_create(const libresteam_backend_set_t *set, libresteam_lobby_type_t type, uint32_t member_max, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_lobby_join(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_lobby_leave(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby);
LDG_EXPORT uint32_t libresteam_lobby_invite(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t invitee);
LDG_EXPORT uint32_t libresteam_lobby_member_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out);
LDG_EXPORT uint32_t libresteam_lobby_member_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t idx, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_lobby_data_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_lobby_data_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char *val);
LDG_EXPORT uint32_t libresteam_lobby_data_cunt_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out);
LDG_EXPORT uint32_t libresteam_lobby_data_idx_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t idx, char *key_out, uint32_t key_cap, char *val_out, uint32_t val_cap);
LDG_EXPORT uint32_t libresteam_lobby_data_delete(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key);
LDG_EXPORT uint32_t libresteam_lobby_member_data_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t user, const char *key, const char **out);
LDG_EXPORT uint32_t libresteam_lobby_member_data_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const char *key, const char *val);
LDG_EXPORT uint32_t libresteam_lobby_chat_msg_send(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, const void *body, uint32_t len);
LDG_EXPORT uint32_t libresteam_lobby_chat_entry_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t chat_id, libresteam_steam_id_t *user_out, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *entry_type_out);
LDG_EXPORT uint32_t libresteam_lobby_data_req(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby);
LDG_EXPORT uint32_t libresteam_lobby_game_srv_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t ip, uint16_t port, libresteam_steam_id_t srv);
LDG_EXPORT uint32_t libresteam_lobby_game_srv_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *ip_out, uint16_t *port_out, libresteam_steam_id_t *srv_out);
LDG_EXPORT uint32_t libresteam_lobby_member_limit_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t member_max);
LDG_EXPORT uint32_t libresteam_lobby_member_limit_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint32_t *out);
LDG_EXPORT uint32_t libresteam_lobby_type_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_lobby_type_t type);
LDG_EXPORT uint32_t libresteam_lobby_joinable_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, uint8_t joinable);
LDG_EXPORT uint32_t libresteam_lobby_owner_get(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t *out);
LDG_EXPORT uint32_t libresteam_lobby_owner_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t owner);
LDG_EXPORT uint32_t libresteam_lobby_linked_set(const libresteam_backend_set_t *set, libresteam_steam_id_t lobby, libresteam_steam_id_t dependent);

#endif
