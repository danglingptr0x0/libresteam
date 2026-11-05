#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/net/sock.h>

#include <libresteam/backend/srv/srv.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/ops.h>

static uint32_t srv_user_steam_id_get(void *ctx, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = ((libresteam_backend_srv_t *)ctx)->steam_id;

    return LDG_ERR_AOK;
}

static uint32_t srv_user_logged_on_get(void *ctx, uint8_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (ldg_net_sock_live_is(&((libresteam_backend_srv_t *)ctx)->sock) == LDG_TRUTH_TRUE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_srv_user_ops_fill(libresteam_backend_srv_t *srv)
{
    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->user_ops.steam_id_get = srv_user_steam_id_get;
    srv->user_ops.logged_on_get = srv_user_logged_on_get;
    srv->ops.user = &srv->user_ops;

    return LDG_ERR_AOK;
}
