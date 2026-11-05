#ifndef LIBRESTEAM_NET_SOCKS_OPS_H
#define LIBRESTEAM_NET_SOCKS_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/net/types.h>

#define LIBRESTEAM_NET_SOCKS_FAKE_PORT_CLIENT 0xFFFFFFFFU

typedef struct libresteam_net_socks_sig_ops
{
    uint32_t (*sig_send_cb)(void *content, uint32_t conn, const libresteam_net_conn_info_t *info, const void *msg, uint32_t size);
    void (*release_cb)(void *content);
} libresteam_net_socks_sig_ops_t;

typedef struct libresteam_net_socks_sig
{
    const libresteam_net_socks_sig_ops_t *ops;
    void *content;
} libresteam_net_socks_sig_t;

typedef struct libresteam_net_socks_sig_recv_ops
{
    uint32_t (*connect_req_cb)(void *content, uint32_t conn, const libresteam_net_ident_t *peer, uint32_t local_virtual_port, libresteam_net_socks_sig_t *sig_out);
    void (*rejection_send_cb)(void *content, const libresteam_net_ident_t *peer, const void *msg, uint32_t size);
} libresteam_net_socks_sig_recv_ops_t;

typedef struct libresteam_net_socks_sig_recv
{
    const libresteam_net_socks_sig_recv_ops_t *ops;
    void *content;
} libresteam_net_socks_sig_recv_t;

typedef struct libresteam_net_socks_ops
{
    uint32_t (*listen_sock_ip_create)(void *ctx, const libresteam_net_ip_addr_t *local, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
    uint32_t (*ip_addr_connect)(void *ctx, const libresteam_net_ip_addr_t *addr, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
    uint32_t (*listen_sock_p2p_create)(void *ctx, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
    uint32_t (*p2p_connect)(void *ctx, const libresteam_net_ident_t *remote, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
    uint32_t (*conn_accept)(void *ctx, uint32_t conn, uint32_t *eres_out);
    uint32_t (*conn_close)(void *ctx, uint32_t conn, uint32_t reason, const char *debug, uint8_t linger_en);
    uint32_t (*listen_sock_close)(void *ctx, uint32_t sock);
    uint32_t (*conn_user_data_set)(void *ctx, uint32_t conn, uint64_t user_data);
    uint32_t (*conn_user_data_get)(void *ctx, uint32_t conn, uint64_t *out);
    uint32_t (*conn_name_set)(void *ctx, uint32_t conn, const char *name);
    uint32_t (*conn_name_get)(void *ctx, uint32_t conn, char *buff, uint32_t cap);
    uint32_t (*conn_msg_send)(void *ctx, uint32_t conn, const void *data, uint32_t size, uint32_t flags, uint64_t *msg_num_out, uint32_t *eres_out);
    uint32_t (*msgs_send)(void *ctx, uint32_t msg_cunt, libresteam_net_msg_t *const *msg, uint64_t *msg_num_or_res_out);
    uint32_t (*conn_msgs_flush)(void *ctx, uint32_t conn, uint32_t *eres_out);
    uint32_t (*conn_msgs_recv)(void *ctx, uint32_t conn, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*conn_info_get)(void *ctx, uint32_t conn, libresteam_net_conn_info_t *out);
    uint32_t (*conn_real_time_status_get)(void *ctx, uint32_t conn, libresteam_net_real_time_status_t *status, uint32_t lane_cunt, libresteam_net_lane_status_t *lane, uint32_t *eres_out);
    uint32_t (*conn_detailed_status_get)(void *ctx, uint32_t conn, char *buff, uint32_t cap, uint32_t *needed_out);
    uint32_t (*listen_sock_addr_get)(void *ctx, uint32_t sock, libresteam_net_ip_addr_t *out);
    uint32_t (*sock_pair_create)(void *ctx, uint32_t *conn_lhs_out, uint32_t *conn_rhs_out, uint8_t loopback_en, const libresteam_net_ident_t *ident_lhs, const libresteam_net_ident_t *ident_rhs);
    uint32_t (*conn_lane_conf_set)(void *ctx, uint32_t conn, uint32_t lane_cunt, const uint32_t *prio, const uint16_t *weight, uint32_t *eres_out);
    uint32_t (*ident_get)(void *ctx, libresteam_net_ident_t *out);
    uint32_t (*authentication_init)(void *ctx, uint32_t *avail_out);
    uint32_t (*authentication_status_get)(void *ctx, libresteam_net_avail_status_t *details, uint32_t *avail_out);
    uint32_t (*poll_group_create)(void *ctx, uint32_t *group_out);
    uint32_t (*poll_group_destroy)(void *ctx, uint32_t group);
    uint32_t (*conn_poll_group_set)(void *ctx, uint32_t conn, uint32_t group);
    uint32_t (*poll_group_msgs_recv)(void *ctx, uint32_t group, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*relay_authentication_tkt_receive)(void *ctx, const void *tkt, uint32_t size, void *parsed_out);
    uint32_t (*relay_authentication_tkt_find)(void *ctx, const libresteam_net_ident_t *game_srv, uint32_t remote_virtual_port, void *parsed_out, uint32_t *expiry_sec_out);
    uint32_t (*hosted_dedicated_srv_connect)(void *ctx, const libresteam_net_ident_t *tgt, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
    uint32_t (*hosted_dedicated_srv_port_get)(void *ctx, uint16_t *out);
    uint32_t (*hosted_dedicated_srv_pop_get)(void *ctx, uint32_t *out);
    uint32_t (*hosted_dedicated_srv_addr_get)(void *ctx, void *routing, uint32_t *eres_out);
    uint32_t (*hosted_dedicated_srv_listen_sock_create)(void *ctx, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
    uint32_t (*game_coordinator_srv_login_get)(void *ctx, void *login_info, uint32_t *blob_size, void *blob, uint32_t *eres_out);
    uint32_t (*p2p_custom_sig_connect)(void *ctx, const libresteam_net_socks_sig_t *sig, const libresteam_net_ident_t *peer, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
    uint32_t (*p2p_custom_sig_receive)(void *ctx, const void *msg, uint32_t size, const libresteam_net_socks_sig_recv_t *recv);
    uint32_t (*certificate_req_get)(void *ctx, uint32_t *blob_size, void *blob, char *fail_msg);
    uint32_t (*certificate_set)(void *ctx, const void *certificate, uint32_t size, char *fail_msg);
    uint32_t (*ident_rst)(void *ctx, const libresteam_net_ident_t *ident);
    uint32_t (*cb_run)(void *ctx);
    uint32_t (*fake_ip_req_begin)(void *ctx, uint32_t port_cunt);
    uint32_t (*fake_ip_get)(void *ctx, uint32_t first_port_idx, libresteam_net_fake_ip_res_t *out);
    uint32_t (*listen_sock_p2p_fake_ip_create)(void *ctx, uint32_t fake_port_idx, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
    uint32_t (*conn_remote_fake_ip_get)(void *ctx, uint32_t conn, libresteam_net_ip_addr_t *out, uint32_t *eres_out);
    uint32_t (*fake_udp_port_create)(void *ctx, uint32_t fake_srv_port_idx, void **port_out);
    uint32_t (*fake_udp_port_destroy)(void *ctx, void *port);
    uint32_t (*fake_udp_port_msg_send)(void *ctx, void *port, const libresteam_net_ip_addr_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t *eres_out);
    uint32_t (*fake_udp_port_msgs_recv)(void *ctx, void *port, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*fake_udp_port_cleanup_schedule)(void *ctx, void *port, const libresteam_net_ip_addr_t *remote);
} libresteam_net_socks_ops_t;

#endif
