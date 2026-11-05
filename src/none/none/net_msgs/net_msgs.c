#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/net_msgs/net_msgs.h>
#include <libresteam/net_msgs/ops.h>

static uint32_t net_msgs_ops_get(const libresteam_backend_set_t *set, const libresteam_net_msgs_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->net_msgs) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->net_msgs;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_net_msgs_msg_send(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t remote_channel, uint32_t *eres_out)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msg_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->msg_send(ctx, remote, data, size, flags, remote_channel, eres_out);
}

uint32_t libresteam_net_msgs_channel_msgs_recv(const libresteam_backend_set_t *set, uint32_t local_channel, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->channel_msgs_recv) { return LDG_ERR_UNSUPPORTED; }

    return ops->channel_msgs_recv(ctx, local_channel, msg_out, cap, cunt_out);
}

uint32_t libresteam_net_msgs_sesh_accept(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sesh_accept) { return LDG_ERR_UNSUPPORTED; }

    return ops->sesh_accept(ctx, remote);
}

uint32_t libresteam_net_msgs_sesh_close(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sesh_close) { return LDG_ERR_UNSUPPORTED; }

    return ops->sesh_close(ctx, remote);
}

uint32_t libresteam_net_msgs_channel_close(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, uint32_t local_channel)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->channel_close) { return LDG_ERR_UNSUPPORTED; }

    return ops->channel_close(ctx, remote, local_channel);
}

uint32_t libresteam_net_msgs_sesh_conn_info_get(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, libresteam_net_conn_info_t *info, libresteam_net_real_time_status_t *status, uint32_t *state_out)
{
    const libresteam_net_msgs_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!state_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *state_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_msgs_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sesh_conn_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->sesh_conn_info_get(ctx, remote, info, status, state_out);
}
