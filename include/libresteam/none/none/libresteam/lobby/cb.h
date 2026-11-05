#ifndef LIBRESTEAM_LOBBY_CB_H
#define LIBRESTEAM_LOBBY_CB_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/lobby/cb_pform.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_LOBBY_CB_JOIN_REQ 333U
#define LIBRESTEAM_LOBBY_CB_RICH_PRESENCE_JOIN_REQ 337U
#define LIBRESTEAM_LOBBY_CB_INVITE 503U
#define LIBRESTEAM_LOBBY_CB_ENTER 504U
#define LIBRESTEAM_LOBBY_CB_DATA_UPDATE 505U
#define LIBRESTEAM_LOBBY_CB_CHAT_UPDATE 506U
#define LIBRESTEAM_LOBBY_CB_CHAT_MSG 507U
#define LIBRESTEAM_LOBBY_CB_GAME_CREATED 509U
#define LIBRESTEAM_LOBBY_CB_MATCH_LIST 510U
#define LIBRESTEAM_LOBBY_CB_KICKED 512U
#define LIBRESTEAM_LOBBY_CB_CREATED 513U
#define LIBRESTEAM_LOBBY_CB_CONNECT_MAX 256U

typedef struct libresteam_lobby_cb_join_req
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t friend_id;
} LDG_PACKED libresteam_lobby_cb_join_req_t;

typedef struct libresteam_lobby_cb_rich_presence_join_req
{
    libresteam_steam_id_t friend_id;
    char connect[LIBRESTEAM_LOBBY_CB_CONNECT_MAX];
} LDG_PACKED libresteam_lobby_cb_rich_presence_join_req_t;

typedef struct libresteam_lobby_cb_invite
{
    libresteam_steam_id_t user;
    libresteam_steam_id_t lobby;
    uint64_t game_id;
} LDG_PACKED libresteam_lobby_cb_invite_t;

typedef struct libresteam_lobby_cb_chat_msg
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t user;
    uint8_t entry_type;
    uint8_t pudding0[3];
    uint32_t chat_id;
} LDG_PACKED libresteam_lobby_cb_chat_msg_t;

typedef struct libresteam_lobby_cb_game_created
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t srv;
    uint32_t ip;
    uint16_t port;
    uint8_t pudding[2];
} LDG_PACKED libresteam_lobby_cb_game_created_t;

typedef struct libresteam_lobby_cb_match_list
{
    uint32_t lobby_cunt;
} LDG_PACKED libresteam_lobby_cb_match_list_t;

#endif
