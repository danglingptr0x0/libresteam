#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/steam_id/steam_id.h>

uint32_t libresteam_steam_id_inst_set(libresteam_steam_id_t *id, uint32_t account, uint32_t inst, libresteam_steam_id_universe_t universe, libresteam_steam_id_account_type_t account_type)
{
    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(universe >= LIBRESTEAM_STEAM_ID_UNIVERSE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(account_type >= LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(inst > LIBRESTEAM_STEAM_ID_INST_MASK)) { return LDG_ERR_FUNC_ARG_INVALID; }

    id->comp.account = account;
    id->comp.inst = inst & LIBRESTEAM_STEAM_ID_INST_MASK;
    id->comp.account_type = (uint32_t)account_type;
    id->comp.universe = (uint32_t)universe;

    return LDG_ERR_AOK;
}

uint32_t libresteam_steam_id_set(libresteam_steam_id_t *id, uint32_t account, libresteam_steam_id_universe_t universe, libresteam_steam_id_account_type_t account_type)
{
    uint32_t inst = LIBRESTEAM_STEAM_ID_INST_DESKTOP;

    if (account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CLAN || account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_GAME_SRV) { inst = LIBRESTEAM_STEAM_ID_INST_NONE; }

    return libresteam_steam_id_inst_set(id, account, inst, universe, account_type);
}

uint32_t libresteam_steam_id_clear(libresteam_steam_id_t *id)
{
    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    id->comp.account = LIBRESTEAM_STEAM_ID_ACCOUNT_INVALID;
    id->comp.inst = LIBRESTEAM_STEAM_ID_INST_NONE;
    id->comp.account_type = LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INVALID;
    id->comp.universe = LIBRESTEAM_STEAM_ID_UNIVERSE_INVALID;

    return LDG_ERR_AOK;
}

uint8_t libresteam_steam_id_valid_is(libresteam_steam_id_t id)
{
    if (id.comp.account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INVALID || id.comp.account_type >= LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_MAX) { return LDG_TRUTH_FALSE; }

    if (id.comp.universe == LIBRESTEAM_STEAM_ID_UNIVERSE_INVALID || id.comp.universe >= LIBRESTEAM_STEAM_ID_UNIVERSE_MAX) { return LDG_TRUTH_FALSE; }

    if (id.comp.account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INDIVIDUAL) { if (id.comp.account == LIBRESTEAM_STEAM_ID_ACCOUNT_INVALID || id.comp.inst != LIBRESTEAM_STEAM_ID_INST_DESKTOP) { return LDG_TRUTH_FALSE; } }

    if (id.comp.account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_CLAN) { if (id.comp.account == LIBRESTEAM_STEAM_ID_ACCOUNT_INVALID || id.comp.inst != LIBRESTEAM_STEAM_ID_INST_NONE) { return LDG_TRUTH_FALSE; } }

    if (id.comp.account_type == LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_GAME_SRV) { if (id.comp.account == LIBRESTEAM_STEAM_ID_ACCOUNT_INVALID) { return LDG_TRUTH_FALSE; } }

    return LDG_TRUTH_TRUE;
}
