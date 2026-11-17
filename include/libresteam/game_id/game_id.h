#ifndef LIBRESTEAM_GAME_ID_H
#define LIBRESTEAM_GAME_ID_H

#include <stdint.h>
#include <dangling/core/macros.h>

#define LIBRESTEAM_GAME_ID_APP_INVALID 0U
#define LIBRESTEAM_GAME_ID_MOD_NONE 0U
#define LIBRESTEAM_GAME_ID_MOD_FLAG 0x80000000U
#define LIBRESTEAM_GAME_ID_LOCAL_APP_MAX 5000U
#define LIBRESTEAM_GAME_ID_APP_MASK 0x00FFFFFFU

typedef enum libresteam_game_id_type
{
    LIBRESTEAM_GAME_ID_TYPE_APP = 0,
    LIBRESTEAM_GAME_ID_TYPE_GAME_MOD,
    LIBRESTEAM_GAME_ID_TYPE_SHORTCUT,
    LIBRESTEAM_GAME_ID_TYPE_P2P,
    LIBRESTEAM_GAME_ID_TYPE_MAX
} libresteam_game_id_type_t;

typedef struct libresteam_game_id_comp
{
    uint32_t app : 24;
    uint32_t type : 8;
    uint32_t mod : 32;
} LDG_PACKED libresteam_game_id_comp_t;

typedef union libresteam_game_id
{
    uint64_t all;
    libresteam_game_id_comp_t comp;
} LDG_PACKED libresteam_game_id_t;

LDG_EXPORT uint32_t libresteam_game_id_set(libresteam_game_id_t *id, uint32_t app, uint32_t mod, libresteam_game_id_type_t type);
LDG_EXPORT uint8_t libresteam_game_id_valid_is(libresteam_game_id_t id);

#endif
