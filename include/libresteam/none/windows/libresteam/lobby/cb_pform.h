#ifndef LIBRESTEAM_LOBBY_CB_PFORM_H
#define LIBRESTEAM_LOBBY_CB_PFORM_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_lobby_cb_enter
{
    libresteam_steam_id_t lobby;
    uint32_t chat_perm;
    uint8_t locked;
    uint8_t pudding0[3];
    uint32_t resp;
    uint8_t pudding[4];
} LDG_PACKED libresteam_lobby_cb_enter_t;

typedef struct libresteam_lobby_cb_data_update
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t member;
    uint8_t success;
    uint8_t pudding[7];
} LDG_PACKED libresteam_lobby_cb_data_update_t;

typedef struct libresteam_lobby_cb_chat_update
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t changed;
    libresteam_steam_id_t making_change;
    uint32_t state_change;
    uint8_t pudding[4];
} LDG_PACKED libresteam_lobby_cb_chat_update_t;

typedef struct libresteam_lobby_cb_kicked
{
    libresteam_steam_id_t lobby;
    libresteam_steam_id_t admin;
    uint8_t disconnect;
    uint8_t pudding[7];
} LDG_PACKED libresteam_lobby_cb_kicked_t;

typedef struct libresteam_lobby_cb_created
{
    uint32_t res;
    uint8_t pudding0[4];
    libresteam_steam_id_t lobby;
} LDG_PACKED libresteam_lobby_cb_created_t;

#endif
