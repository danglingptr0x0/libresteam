#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/net_socks/net_socks.h>
#include <libresteam/net_socks/ops.h>

static uint32_t net_socks_ops_get(const libresteam_backend_set_t *set, const libresteam_net_socks_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->net_socks) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->net_socks;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_net_socks_listen_sock_ip_create(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *local, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!local)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->listen_sock_ip_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->listen_sock_ip_create(ctx, local, conf_val_cunt, conf_val, sock_out);
}

uint32_t libresteam_net_socks_ip_addr_connect(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!addr)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ip_addr_connect) { return LDG_ERR_UNSUPPORTED; }

    return ops->ip_addr_connect(ctx, addr, conf_val_cunt, conf_val, conn_out);
}

uint32_t libresteam_net_socks_listen_sock_p2p_create(const libresteam_backend_set_t *set, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->listen_sock_p2p_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->listen_sock_p2p_create(ctx, local_virtual_port, conf_val_cunt, conf_val, sock_out);
}

uint32_t libresteam_net_socks_p2p_connect(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->p2p_connect) { return LDG_ERR_UNSUPPORTED; }

    return ops->p2p_connect(ctx, remote, remote_virtual_port, conf_val_cunt, conf_val, conn_out);
}

uint32_t libresteam_net_socks_conn_accept(const libresteam_backend_set_t *set, uint32_t conn, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_accept) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_accept(ctx, conn, eres_out);
}

uint32_t libresteam_net_socks_conn_close(const libresteam_backend_set_t *set, uint32_t conn, uint32_t reason, const char *debug, uint8_t linger_en)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_close) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_close(ctx, conn, reason, debug, linger_en);
}

uint32_t libresteam_net_socks_listen_sock_close(const libresteam_backend_set_t *set, uint32_t sock)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->listen_sock_close) { return LDG_ERR_UNSUPPORTED; }

    return ops->listen_sock_close(ctx, sock);
}

uint32_t libresteam_net_socks_conn_user_data_set(const libresteam_backend_set_t *set, uint32_t conn, uint64_t user_data)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_user_data_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_user_data_set(ctx, conn, user_data);
}

uint32_t libresteam_net_socks_conn_user_data_get(const libresteam_backend_set_t *set, uint32_t conn, uint64_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_user_data_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_user_data_get(ctx, conn, out);
}

uint32_t libresteam_net_socks_conn_name_set(const libresteam_backend_set_t *set, uint32_t conn, const char *name)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_name_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_name_set(ctx, conn, name);
}

uint32_t libresteam_net_socks_conn_name_get(const libresteam_backend_set_t *set, uint32_t conn, char *buff, uint32_t cap)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_name_get(ctx, conn, buff, cap);
}

uint32_t libresteam_net_socks_conn_msg_send(const libresteam_backend_set_t *set, uint32_t conn, const void *data, uint32_t size, uint32_t flags, uint64_t *msg_num_out, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_msg_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_msg_send(ctx, conn, data, size, flags, msg_num_out, eres_out);
}

uint32_t libresteam_net_socks_msgs_send(const libresteam_backend_set_t *set, uint32_t msg_cunt, libresteam_net_msg_t *const *msg, uint64_t *msg_num_or_res_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msgs_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->msgs_send(ctx, msg_cunt, msg, msg_num_or_res_out);
}

uint32_t libresteam_net_socks_conn_msgs_flush(const libresteam_backend_set_t *set, uint32_t conn, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_msgs_flush) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_msgs_flush(ctx, conn, eres_out);
}

uint32_t libresteam_net_socks_conn_msgs_recv(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_msgs_recv) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_msgs_recv(ctx, conn, msg_out, cap, cunt_out);
}

uint32_t libresteam_net_socks_conn_info_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_conn_info_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_info_get(ctx, conn, out);
}

uint32_t libresteam_net_socks_conn_real_time_status_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_real_time_status_t *status, uint32_t lane_cunt, libresteam_net_lane_status_t *lane, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!lane && lane_cunt != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_real_time_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_real_time_status_get(ctx, conn, status, lane_cunt, lane, eres_out);
}

uint32_t libresteam_net_socks_conn_detailed_status_get(const libresteam_backend_set_t *set, uint32_t conn, char *buff, uint32_t cap, uint32_t *needed_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!needed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *needed_out = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_detailed_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_detailed_status_get(ctx, conn, buff, cap, needed_out);
}

uint32_t libresteam_net_socks_listen_sock_addr_get(const libresteam_backend_set_t *set, uint32_t sock, libresteam_net_ip_addr_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->listen_sock_addr_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->listen_sock_addr_get(ctx, sock, out);
}

uint32_t libresteam_net_socks_sock_pair_create(const libresteam_backend_set_t *set, uint32_t *conn_lhs_out, uint32_t *conn_rhs_out, uint8_t loopback_en, const libresteam_net_ident_t *ident_lhs, const libresteam_net_ident_t *ident_rhs)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_lhs_out || !conn_rhs_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_lhs_out = LIBRESTEAM_NET_HANDLE_INVALID;
    *conn_rhs_out = LIBRESTEAM_NET_HANDLE_INVALID;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sock_pair_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->sock_pair_create(ctx, conn_lhs_out, conn_rhs_out, loopback_en, ident_lhs, ident_rhs);
}

uint32_t libresteam_net_socks_conn_lane_conf_set(const libresteam_backend_set_t *set, uint32_t conn, uint32_t lane_cunt, const uint32_t *prio, const uint16_t *weight, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_lane_conf_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_lane_conf_set(ctx, conn, lane_cunt, prio, weight, eres_out);
}

uint32_t libresteam_net_socks_ident_get(const libresteam_backend_set_t *set, libresteam_net_ident_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ident_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ident_get(ctx, out);
}

uint32_t libresteam_net_socks_authentication_init(const libresteam_backend_set_t *set, uint32_t *avail_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *avail_out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->authentication_init) { return LDG_ERR_UNSUPPORTED; }

    return ops->authentication_init(ctx, avail_out);
}

uint32_t libresteam_net_socks_authentication_status_get(const libresteam_backend_set_t *set, libresteam_net_avail_status_t *details, uint32_t *avail_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *avail_out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->authentication_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->authentication_status_get(ctx, details, avail_out);
}

uint32_t libresteam_net_socks_poll_group_create(const libresteam_backend_set_t *set, uint32_t *group_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!group_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *group_out = LIBRESTEAM_NET_HANDLE_INVALID;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->poll_group_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->poll_group_create(ctx, group_out);
}

uint32_t libresteam_net_socks_poll_group_destroy(const libresteam_backend_set_t *set, uint32_t group)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->poll_group_destroy) { return LDG_ERR_UNSUPPORTED; }

    return ops->poll_group_destroy(ctx, group);
}

uint32_t libresteam_net_socks_conn_poll_group_set(const libresteam_backend_set_t *set, uint32_t conn, uint32_t group)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_poll_group_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_poll_group_set(ctx, conn, group);
}

uint32_t libresteam_net_socks_poll_group_msgs_recv(const libresteam_backend_set_t *set, uint32_t group, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->poll_group_msgs_recv) { return LDG_ERR_UNSUPPORTED; }

    return ops->poll_group_msgs_recv(ctx, group, msg_out, cap, cunt_out);
}

uint32_t libresteam_net_socks_relay_authentication_tkt_receive(const libresteam_backend_set_t *set, const void *tkt, uint32_t size, void *parsed_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!tkt)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->relay_authentication_tkt_receive) { return LDG_ERR_UNSUPPORTED; }

    return ops->relay_authentication_tkt_receive(ctx, tkt, size, parsed_out);
}

uint32_t libresteam_net_socks_relay_authentication_tkt_find(const libresteam_backend_set_t *set, const libresteam_net_ident_t *game_srv, uint32_t remote_virtual_port, void *parsed_out, uint32_t *expiry_sec_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!expiry_sec_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *expiry_sec_out = 0;

    if (LDG_UNLIKELY(!game_srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->relay_authentication_tkt_find) { return LDG_ERR_UNSUPPORTED; }

    return ops->relay_authentication_tkt_find(ctx, game_srv, remote_virtual_port, parsed_out, expiry_sec_out);
}

uint32_t libresteam_net_socks_hosted_dedicated_srv_connect(const libresteam_backend_set_t *set, const libresteam_net_ident_t *tgt, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!tgt)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->hosted_dedicated_srv_connect) { return LDG_ERR_UNSUPPORTED; }

    return ops->hosted_dedicated_srv_connect(ctx, tgt, remote_virtual_port, conf_val_cunt, conf_val, conn_out);
}

uint32_t libresteam_net_socks_hosted_dedicated_srv_port_get(const libresteam_backend_set_t *set, uint16_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->hosted_dedicated_srv_port_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->hosted_dedicated_srv_port_get(ctx, out);
}

uint32_t libresteam_net_socks_hosted_dedicated_srv_pop_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->hosted_dedicated_srv_pop_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->hosted_dedicated_srv_pop_get(ctx, out);
}

uint32_t libresteam_net_socks_hosted_dedicated_srv_addr_get(const libresteam_backend_set_t *set, void *routing, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!routing)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->hosted_dedicated_srv_addr_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->hosted_dedicated_srv_addr_get(ctx, routing, eres_out);
}

uint32_t libresteam_net_socks_hosted_dedicated_srv_listen_sock_create(const libresteam_backend_set_t *set, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->hosted_dedicated_srv_listen_sock_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->hosted_dedicated_srv_listen_sock_create(ctx, local_virtual_port, conf_val_cunt, conf_val, sock_out);
}

uint32_t libresteam_net_socks_game_coordinator_srv_login_get(const libresteam_backend_set_t *set, void *login_info, uint32_t *blob_size, void *blob, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!login_info || !blob_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_coordinator_srv_login_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_coordinator_srv_login_get(ctx, login_info, blob_size, blob, eres_out);
}

uint32_t libresteam_net_socks_p2p_custom_sig_connect(const libresteam_backend_set_t *set, const libresteam_net_socks_sig_t *sig, const libresteam_net_ident_t *peer, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!sig || !sig->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->p2p_custom_sig_connect) { return LDG_ERR_UNSUPPORTED; }

    return ops->p2p_custom_sig_connect(ctx, sig, peer, remote_virtual_port, conf_val_cunt, conf_val, conn_out);
}

uint32_t libresteam_net_socks_p2p_custom_sig_receive(const libresteam_backend_set_t *set, const void *msg, uint32_t size, const libresteam_net_socks_sig_recv_t *recv)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!msg || !recv || !recv->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->p2p_custom_sig_receive) { return LDG_ERR_UNSUPPORTED; }

    return ops->p2p_custom_sig_receive(ctx, msg, size, recv);
}

uint32_t libresteam_net_socks_certificate_req_get(const libresteam_backend_set_t *set, uint32_t *blob_size, void *blob, char *fail_msg)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!blob_size || !fail_msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    fail_msg[0] = LDG_STR_TERM;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->certificate_req_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->certificate_req_get(ctx, blob_size, blob, fail_msg);
}

uint32_t libresteam_net_socks_certificate_set(const libresteam_backend_set_t *set, const void *certificate, uint32_t size, char *fail_msg)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!certificate || !fail_msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    fail_msg[0] = LDG_STR_TERM;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->certificate_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->certificate_set(ctx, certificate, size, fail_msg);
}

uint32_t libresteam_net_socks_ident_rst(const libresteam_backend_set_t *set, const libresteam_net_ident_t *ident)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ident_rst) { return LDG_ERR_UNSUPPORTED; }

    return ops->ident_rst(ctx, ident);
}

uint32_t libresteam_net_socks_cb_run(const libresteam_backend_set_t *set)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->cb_run) { return LDG_ERR_UNSUPPORTED; }

    return ops->cb_run(ctx);
}

uint32_t libresteam_net_socks_fake_ip_req_begin(const libresteam_backend_set_t *set, uint32_t port_cunt)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_ip_req_begin) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_ip_req_begin(ctx, port_cunt);
}

uint32_t libresteam_net_socks_fake_ip_get(const libresteam_backend_set_t *set, uint32_t first_port_idx, libresteam_net_fake_ip_res_t *out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_ip_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_ip_get(ctx, first_port_idx, out);
}

uint32_t libresteam_net_socks_listen_sock_p2p_fake_ip_create(const libresteam_backend_set_t *set, uint32_t fake_port_idx, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->listen_sock_p2p_fake_ip_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->listen_sock_p2p_fake_ip_create(ctx, fake_port_idx, conf_val_cunt, conf_val, sock_out);
}

uint32_t libresteam_net_socks_conn_remote_fake_ip_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_ip_addr_t *out, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conn_remote_fake_ip_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conn_remote_fake_ip_get(ctx, conn, out, eres_out);
}

uint32_t libresteam_net_socks_fake_udp_port_create(const libresteam_backend_set_t *set, uint32_t fake_srv_port_idx, void **port_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!port_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *port_out = 0x0;

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_udp_port_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_udp_port_create(ctx, fake_srv_port_idx, port_out);
}

uint32_t libresteam_net_socks_fake_udp_port_destroy(const libresteam_backend_set_t *set, void *port)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!port)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_udp_port_destroy) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_udp_port_destroy(ctx, port);
}

uint32_t libresteam_net_socks_fake_udp_port_msg_send(const libresteam_backend_set_t *set, void *port, const libresteam_net_ip_addr_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t *eres_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!port || !remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_udp_port_msg_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_udp_port_msg_send(ctx, port, remote, data, size, flags, eres_out);
}

uint32_t libresteam_net_socks_fake_udp_port_msgs_recv(const libresteam_backend_set_t *set, void *port, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!port || !msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_udp_port_msgs_recv) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_udp_port_msgs_recv(ctx, port, msg_out, cap, cunt_out);
}

uint32_t libresteam_net_socks_fake_udp_port_cleanup_schedule(const libresteam_backend_set_t *set, void *port, const libresteam_net_ip_addr_t *remote)
{
    const libresteam_net_socks_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!port || !remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_socks_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->fake_udp_port_cleanup_schedule) { return LDG_ERR_UNSUPPORTED; }

    return ops->fake_udp_port_cleanup_schedule(ctx, port, remote);
}
