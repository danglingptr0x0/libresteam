#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/game_id/game_id.h>

uint32_t libresteam_game_id_set(libresteam_game_id_t *id, uint32_t app, uint32_t mod, libresteam_game_id_type_t type)
{
    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(app > LIBRESTEAM_GAME_ID_APP_MASK)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(type >= LIBRESTEAM_GAME_ID_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    id->comp.app = app & LIBRESTEAM_GAME_ID_APP_MASK;
    id->comp.type = (uint32_t)type;
    id->comp.mod = mod;

    return LDG_ERR_AOK;
}

uint8_t libresteam_game_id_valid_is(libresteam_game_id_t id)
{
    uint8_t mod_flagged = LDG_TRUTH_FALSE;

    if ((id.comp.mod & LIBRESTEAM_GAME_ID_MOD_FLAG) != LIBRESTEAM_GAME_ID_MOD_NONE) { mod_flagged = LDG_TRUTH_TRUE; }

    switch (id.comp.type)
    {
        case LIBRESTEAM_GAME_ID_TYPE_APP:
            if (id.comp.app != LIBRESTEAM_GAME_ID_APP_INVALID) { return LDG_TRUTH_TRUE; }

            return LDG_TRUTH_FALSE;
        case LIBRESTEAM_GAME_ID_TYPE_GAME_MOD:
            if (id.comp.app != LIBRESTEAM_GAME_ID_APP_INVALID && mod_flagged == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

            return LDG_TRUTH_FALSE;
        case LIBRESTEAM_GAME_ID_TYPE_SHORTCUT:
            if (id.comp.app == LIBRESTEAM_GAME_ID_APP_INVALID && mod_flagged == LDG_TRUTH_TRUE && id.comp.mod >= (LIBRESTEAM_GAME_ID_LOCAL_APP_MAX | LIBRESTEAM_GAME_ID_MOD_FLAG)) { return LDG_TRUTH_TRUE; }

            return LDG_TRUTH_FALSE;
        case LIBRESTEAM_GAME_ID_TYPE_P2P:
            if (id.comp.app == LIBRESTEAM_GAME_ID_APP_INVALID && mod_flagged == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

            return LDG_TRUTH_FALSE;
        default:
            return LDG_TRUTH_FALSE;
    }
}
