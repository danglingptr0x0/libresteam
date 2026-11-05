#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/net/types.h>
#include <libresteam/net_msgs/ops.h>

#define STEAM_NET_MSGS_SIGN_BIT 0x80000000U
#define STEAM_NET_MSGS_CONN_STATE_NONE 0U

static uint32_t steam_net_msgs_slot_get(void *ctx, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->net_msgs)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->net_msgs;

    return ldg_cpp_slot_get(&steam->net_msgs_desc, method_idx, slot_out);
}

static uint32_t steam_net_msgs_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_net_msgs_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_msgs_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_net_msgs_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_net_msgs_msg_send(void *ctx, const libresteam_net_ident_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t remote_channel, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = remote_channel, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_msgs_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_MSG_SEND, arg, cunt, eres_out);
}

static uint32_t steam_net_msgs_channel_msgs_recv(void *ctx, uint32_t local_channel, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = local_channel, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_net_msgs_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_CHANNEL_MSGS_RECV, arg, cunt, cunt_out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*cunt_out & STEAM_NET_MSGS_SIGN_BIT) != 0)
    {
        *cunt_out = 0;
        return LDG_ERR_INVALID;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_msgs_sesh_accept(void *ctx, const libresteam_net_ident_t *remote)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_msgs_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_SESH_ACCEPT, arg, cunt);
}

static uint32_t steam_net_msgs_sesh_close(void *ctx, const libresteam_net_ident_t *remote)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_msgs_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_SESH_CLOSE, arg, cunt);
}

static uint32_t steam_net_msgs_channel_close(void *ctx, const libresteam_net_ident_t *remote, uint32_t local_channel)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = local_channel, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_msgs_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_CHANNEL_CLOSE, arg, cunt);
}

static uint32_t steam_net_msgs_sesh_conn_info_get(void *ctx, const libresteam_net_ident_t *remote, libresteam_net_conn_info_t *info, libresteam_net_real_time_status_t *status, uint32_t *state_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!state_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *state_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = info, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = status, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_net_msgs_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_SESH_CONN_INFO_GET, arg, cunt, state_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*state_out == STEAM_NET_MSGS_CONN_STATE_NONE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_net_msgs_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_net_msgs_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->net_msgs_ops;
    ops->msg_send = steam_net_msgs_msg_send;
    ops->channel_msgs_recv = steam_net_msgs_channel_msgs_recv;
    ops->sesh_accept = steam_net_msgs_sesh_accept;
    ops->sesh_close = steam_net_msgs_sesh_close;
    ops->channel_close = steam_net_msgs_channel_close;
    ops->sesh_conn_info_get = steam_net_msgs_sesh_conn_info_get;
    steam->ops.net_msgs = &steam->net_msgs_ops;

    return LDG_ERR_AOK;
}
