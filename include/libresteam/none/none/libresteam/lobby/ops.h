#ifndef LIBRESTEAM_LOBBY_OPS_H
#define LIBRESTEAM_LOBBY_OPS_H

#include <stdint.h>

#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_LOBBY_KEY_LEN_MAX 255U

#define LIBRESTEAM_LOBBY_CMP_EQUAL_OR_LESS 0xFFFFFFFEU
#define LIBRESTEAM_LOBBY_CMP_LESS 0xFFFFFFFFU
#define LIBRESTEAM_LOBBY_CMP_EQUAL 0x00000000U
#define LIBRESTEAM_LOBBY_CMP_GREATER 0x00000001U
#define LIBRESTEAM_LOBBY_CMP_EQUAL_OR_GREATER 0x00000002U
#define LIBRESTEAM_LOBBY_CMP_NOT_EQUAL 0x00000003U

typedef enum libresteam_lobby_type
{
    LIBRESTEAM_LOBBY_TYPE_PRIVATE = 0,
    LIBRESTEAM_LOBBY_TYPE_FRIENDS_ONLY,
    LIBRESTEAM_LOBBY_TYPE_PUBLIC,
    LIBRESTEAM_LOBBY_TYPE_INVISIBLE,
    LIBRESTEAM_LOBBY_TYPE_PRIVATE_UNIQUE,
    LIBRESTEAM_LOBBY_TYPE_MAX
} libresteam_lobby_type_t;

typedef enum libresteam_lobby_dist
{
    LIBRESTEAM_LOBBY_DIST_CLOSE = 0,
    LIBRESTEAM_LOBBY_DIST_DEFAULT,
    LIBRESTEAM_LOBBY_DIST_FAR,
    LIBRESTEAM_LOBBY_DIST_WORLDWIDE,
    LIBRESTEAM_LOBBY_DIST_MAX
} libresteam_lobby_dist_t;

typedef struct libresteam_lobby_favorite
{
    uint32_t app_id;
    uint32_t ip;
    uint16_t conn_port;
    uint16_t query_port;
    uint32_t flags;
    uint32_t last_played;
    uint8_t pudding[4];
} libresteam_lobby_favorite_t;

typedef struct libresteam_lobby_ops
{
    uint32_t (*favorite_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*favorite_get)(void *ctx, uint32_t idx, libresteam_lobby_favorite_t *out);
    uint32_t (*favorite_add)(void *ctx, const libresteam_lobby_favorite_t *fav, uint32_t *idx_out);
    uint32_t (*favorite_remove)(void *ctx, const libresteam_lobby_favorite_t *fav);
    uint32_t (*list_req)(void *ctx, uint64_t *call_out);
    uint32_t (*list_str_filter_add)(void *ctx, const char *key, const char *val, uint32_t cmp);
    uint32_t (*list_numeric_filter_add)(void *ctx, const char *key, uint32_t val, uint32_t cmp);
    uint32_t (*list_near_filter_add)(void *ctx, const char *key, uint32_t val);
    uint32_t (*list_slot_filter_add)(void *ctx, uint32_t slot_cunt);
    uint32_t (*list_dist_filter_add)(void *ctx, libresteam_lobby_dist_t dist);
    uint32_t (*list_res_cunt_filter_add)(void *ctx, uint32_t res_cunt_max);
    uint32_t (*list_compatible_member_filter_add)(void *ctx, libresteam_steam_id_t lobby);
    uint32_t (*list_lobby_get)(void *ctx, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*create)(void *ctx, libresteam_lobby_type_t type, uint32_t member_max, uint64_t *call_out);
    uint32_t (*join)(void *ctx, libresteam_steam_id_t lobby, uint64_t *call_out);
    uint32_t (*leave)(void *ctx, libresteam_steam_id_t lobby);
    uint32_t (*invite)(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t invitee);
    uint32_t (*member_cunt_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t *out);
    uint32_t (*member_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t idx, libresteam_steam_id_t *out);
    uint32_t (*data_get)(void *ctx, libresteam_steam_id_t lobby, const char *key, const char **out);
    uint32_t (*data_set)(void *ctx, libresteam_steam_id_t lobby, const char *key, const char *val);
    uint32_t (*data_cunt_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t *out);
    uint32_t (*data_idx_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t idx, char *key_out, uint32_t key_cap, char *val_out, uint32_t val_cap);
    uint32_t (*data_delete)(void *ctx, libresteam_steam_id_t lobby, const char *key);
    uint32_t (*member_data_get)(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t user, const char *key, const char **out);
    uint32_t (*member_data_set)(void *ctx, libresteam_steam_id_t lobby, const char *key, const char *val);
    uint32_t (*chat_msg_send)(void *ctx, libresteam_steam_id_t lobby, const void *body, uint32_t len);
    uint32_t (*chat_entry_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t chat_id, libresteam_steam_id_t *user_out, void *buff, uint32_t cap, uint32_t *len_out, uint32_t *entry_type_out);
    uint32_t (*data_req)(void *ctx, libresteam_steam_id_t lobby);
    uint32_t (*game_srv_set)(void *ctx, libresteam_steam_id_t lobby, uint32_t ip, uint16_t port, libresteam_steam_id_t srv);
    uint32_t (*game_srv_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t *ip_out, uint16_t *port_out, libresteam_steam_id_t *srv_out);
    uint32_t (*member_limit_set)(void *ctx, libresteam_steam_id_t lobby, uint32_t member_max);
    uint32_t (*member_limit_get)(void *ctx, libresteam_steam_id_t lobby, uint32_t *out);
    uint32_t (*type_set)(void *ctx, libresteam_steam_id_t lobby, libresteam_lobby_type_t type);
    uint32_t (*joinable_set)(void *ctx, libresteam_steam_id_t lobby, uint8_t joinable);
    uint32_t (*owner_get)(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t *out);
    uint32_t (*owner_set)(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t owner);
    uint32_t (*linked_set)(void *ctx, libresteam_steam_id_t lobby, libresteam_steam_id_t dependent);
} libresteam_lobby_ops_t;

#endif
