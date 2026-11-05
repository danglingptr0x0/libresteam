#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>

uint32_t libresteam_backend_set_init(libresteam_backend_set_t *set, libresteam_backend_t *backend, uint32_t backend_cunt, uint32_t primary_idx)
{
    uint32_t idx = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    set->backend = 0x0;
    set->backend_cunt = 0;
    set->primary_idx = UINT32_MAX;

    if (LDG_UNLIKELY(!backend)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(backend_cunt == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(primary_idx >= backend_cunt)) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; idx < backend_cunt; idx++) { if (LDG_UNLIKELY(!backend[idx].ops)) { return LDG_ERR_FUNC_ARG_NULL; } }

    set->backend = backend;
    set->backend_cunt = backend_cunt;
    set->primary_idx = primary_idx;

    return LDG_ERR_AOK;
}
