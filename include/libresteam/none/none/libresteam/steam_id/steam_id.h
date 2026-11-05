#ifndef LIBRESTEAM_STEAM_ID_H
#define LIBRESTEAM_STEAM_ID_H

#include <stdint.h>
#include <dangling/core/macros.h>

#define LIBRESTEAM_STEAM_ID_ACCOUNT_INVALID 0U
#define LIBRESTEAM_STEAM_ID_ACCOUNT_MASK 0xFFFFFFFFU
#define LIBRESTEAM_STEAM_ID_INST_MASK 0x000FFFFFU
#define LIBRESTEAM_STEAM_ID_INST_NONE 0U
#define LIBRESTEAM_STEAM_ID_INST_DESKTOP 1U

typedef enum libresteam_steam_id_universe
{
    LIBRESTEAM_STEAM_ID_UNIVERSE_INVALID = 0,
    LIBRESTEAM_STEAM_ID_UNIVERSE_PUBLIC,
    LIBRESTEAM_STEAM_ID_UNIVERSE_BETA,
    LIBRESTEAM_STEAM_ID_UNIVERSE_INTERNAL,
    LIBRESTEAM_STEAM_ID_UNIVERSE_DEVELOPMENT,
    LIBRESTEAM_STEAM_ID_UNIVERSE_MAX
} libresteam_steam_id_universe_t;

typedef enum libresteam_steam_id_account_type
{
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INVALID = 0,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INDIVIDUAL,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_MULTISEAT,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_GAME_SRV,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_ANONYMOUS_GAME_SRV,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_PENDING,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CONTENT_SRV,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CLAN,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CHAT,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CONSOLE_USER,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_ANONYMOUS_USER,
    LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_MAX
} libresteam_steam_id_account_type_t;

typedef struct libresteam_steam_id_comp
{
    uint32_t account : 32;
    uint32_t inst : 20;
    uint32_t account_type : 4;
    uint32_t universe : 8;
} LDG_PACKED libresteam_steam_id_comp_t;

typedef union libresteam_steam_id
{
    libresteam_steam_id_comp_t comp;
    uint64_t all;
} LDG_PACKED libresteam_steam_id_t;

LDG_EXPORT uint32_t libresteam_steam_id_set(libresteam_steam_id_t *id, uint32_t account, libresteam_steam_id_universe_t universe, libresteam_steam_id_account_type_t account_type);
LDG_EXPORT uint32_t libresteam_steam_id_inst_set(libresteam_steam_id_t *id, uint32_t account, uint32_t inst, libresteam_steam_id_universe_t universe, libresteam_steam_id_account_type_t account_type);
LDG_EXPORT uint32_t libresteam_steam_id_clear(libresteam_steam_id_t *id);
LDG_EXPORT uint8_t libresteam_steam_id_valid_is(libresteam_steam_id_t id);

#endif
