#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/srv_browser/ops.h>
#include <libresteam/srv_browser/srv_browser.h>

#define SRV_BROWSER_LAYOUT_PASS 1
#define SRV_BROWSER_LAYOUT_FAIL 0

typedef struct { uint32_t ck : (sizeof(libresteam_srv_browser_item_t) == LIBRESTEAM_SRV_BROWSER_ITEM_SIZE) ? SRV_BROWSER_LAYOUT_PASS : SRV_BROWSER_LAYOUT_FAIL;
} srv_browser_item_size_ck_t;

static uint32_t srv_browser_ops_get(const libresteam_backend_set_t *set, const libresteam_srv_browser_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->srv_browser) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->srv_browser;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_browser_list_req(const libresteam_backend_set_t *set, libresteam_srv_browser_list_type_t type, uint32_t app, const libresteam_srv_browser_kv_t *const *filter, uint32_t filter_cunt, const libresteam_srv_browser_list_resp_t *resp, void **req_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!req_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *req_out = 0x0;

    if (LDG_UNLIKELY(!set || !resp || !resp->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(filter_cunt != 0 && !filter)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(type >= LIBRESTEAM_SRV_BROWSER_LIST_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->list_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->list_req(ctx, type, app, filter, filter_cunt, resp, req_out);
}

uint32_t libresteam_srv_browser_req_release(const libresteam_backend_set_t *set, void *req)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->req_release) { return LDG_ERR_UNSUPPORTED; }

    return ops->req_release(ctx, req);
}

uint32_t libresteam_srv_browser_srv_details_get(const libresteam_backend_set_t *set, void *req, uint32_t srv_idx, const libresteam_srv_browser_item_t **out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->srv_details_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->srv_details_get(ctx, req, srv_idx, out);
}

uint32_t libresteam_srv_browser_query_cancel(const libresteam_backend_set_t *set, void *req)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_cancel) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_cancel(ctx, req);
}

uint32_t libresteam_srv_browser_query_refresh(const libresteam_backend_set_t *set, void *req)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_refresh) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_refresh(ctx, req);
}

uint32_t libresteam_srv_browser_refreshing_get(const libresteam_backend_set_t *set, void *req, uint8_t *out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->refreshing_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->refreshing_get(ctx, req, out);
}

uint32_t libresteam_srv_browser_srv_cunt_get(const libresteam_backend_set_t *set, void *req, uint32_t *out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->srv_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->srv_cunt_get(ctx, req, out);
}

uint32_t libresteam_srv_browser_srv_refresh(const libresteam_backend_set_t *set, void *req, uint32_t srv_idx)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->srv_refresh) { return LDG_ERR_UNSUPPORTED; }

    return ops->srv_refresh(ctx, req, srv_idx);
}

uint32_t libresteam_srv_browser_ping_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_ping_resp_t *resp, uint32_t *query_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!set || !resp || !resp->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_req(ctx, ip, port, resp, query_out);
}

uint32_t libresteam_srv_browser_player_details_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_player_resp_t *resp, uint32_t *query_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!set || !resp || !resp->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->player_details_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->player_details_req(ctx, ip, port, resp, query_out);
}

uint32_t libresteam_srv_browser_rule_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_rule_resp_t *resp, uint32_t *query_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!set || !resp || !resp->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->rule_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->rule_req(ctx, ip, port, resp, query_out);
}

uint32_t libresteam_srv_browser_srv_query_cancel(const libresteam_backend_set_t *set, uint32_t query)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(query == LIBRESTEAM_SRV_BROWSER_QUERY_INVALID)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->srv_query_cancel) { return LDG_ERR_UNSUPPORTED; }

    return ops->srv_query_cancel(ctx, query);
}

uint32_t libresteam_srv_browser_favorite_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_cunt_get(ctx, out);
}

uint32_t libresteam_srv_browser_favorite_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *app_out, uint32_t *ip_out, uint16_t *conn_port_out, uint16_t *query_port_out, uint32_t *flags_out, uint32_t *time_last_played_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!app_out || !ip_out || !conn_port_out || !query_port_out || !flags_out || !time_last_played_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *app_out = 0;
    *ip_out = 0;
    *conn_port_out = 0;
    *query_port_out = 0;
    *flags_out = 0;
    *time_last_played_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_get(ctx, idx, app_out, ip_out, conn_port_out, query_port_out, flags_out, time_last_played_out);
}

uint32_t libresteam_srv_browser_favorite_add(const libresteam_backend_set_t *set, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint32_t time_last_played, uint32_t *idx_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *idx_out = UINT32_MAX;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_add(ctx, app, ip, conn_port, query_port, flags, time_last_played, idx_out);
}

uint32_t libresteam_srv_browser_favorite_remove(const libresteam_backend_set_t *set, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint8_t *removed_out)
{
    const libresteam_srv_browser_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!removed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *removed_out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = srv_browser_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_remove(ctx, app, ip, conn_port, query_port, flags, removed_out);
}
