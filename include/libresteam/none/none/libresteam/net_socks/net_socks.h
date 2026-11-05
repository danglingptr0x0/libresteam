#ifndef LIBRESTEAM_NET_SOCKS_H
#define LIBRESTEAM_NET_SOCKS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/net_socks/ops.h>

LDG_EXPORT uint32_t libresteam_net_socks_listen_sock_ip_create(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *local, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
LDG_EXPORT uint32_t libresteam_net_socks_ip_addr_connect(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
LDG_EXPORT uint32_t libresteam_net_socks_listen_sock_p2p_create(const libresteam_backend_set_t *set, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
LDG_EXPORT uint32_t libresteam_net_socks_p2p_connect(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_accept(const libresteam_backend_set_t *set, uint32_t conn, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_close(const libresteam_backend_set_t *set, uint32_t conn, uint32_t reason, const char *debug, uint8_t linger_en);
LDG_EXPORT uint32_t libresteam_net_socks_listen_sock_close(const libresteam_backend_set_t *set, uint32_t sock);
LDG_EXPORT uint32_t libresteam_net_socks_conn_user_data_set(const libresteam_backend_set_t *set, uint32_t conn, uint64_t user_data);
LDG_EXPORT uint32_t libresteam_net_socks_conn_user_data_get(const libresteam_backend_set_t *set, uint32_t conn, uint64_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_name_set(const libresteam_backend_set_t *set, uint32_t conn, const char *name);
LDG_EXPORT uint32_t libresteam_net_socks_conn_name_get(const libresteam_backend_set_t *set, uint32_t conn, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_net_socks_conn_msg_send(const libresteam_backend_set_t *set, uint32_t conn, const void *data, uint32_t size, uint32_t flags, uint64_t *msg_num_out, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_msgs_send(const libresteam_backend_set_t *set, uint32_t msg_cunt, libresteam_net_msg_t *const *msg, uint64_t *msg_num_or_res_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_msgs_flush(const libresteam_backend_set_t *set, uint32_t conn, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_msgs_recv(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_info_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_conn_info_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_real_time_status_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_real_time_status_t *status, uint32_t lane_cunt, libresteam_net_lane_status_t *lane, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_detailed_status_get(const libresteam_backend_set_t *set, uint32_t conn, char *buff, uint32_t cap, uint32_t *needed_out);
LDG_EXPORT uint32_t libresteam_net_socks_listen_sock_addr_get(const libresteam_backend_set_t *set, uint32_t sock, libresteam_net_ip_addr_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_sock_pair_create(const libresteam_backend_set_t *set, uint32_t *conn_lhs_out, uint32_t *conn_rhs_out, uint8_t loopback_en, const libresteam_net_ident_t *ident_lhs, const libresteam_net_ident_t *ident_rhs);
LDG_EXPORT uint32_t libresteam_net_socks_conn_lane_conf_set(const libresteam_backend_set_t *set, uint32_t conn, uint32_t lane_cunt, const uint32_t *prio, const uint16_t *weight, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_ident_get(const libresteam_backend_set_t *set, libresteam_net_ident_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_authentication_init(const libresteam_backend_set_t *set, uint32_t *avail_out);
LDG_EXPORT uint32_t libresteam_net_socks_authentication_status_get(const libresteam_backend_set_t *set, libresteam_net_avail_status_t *details, uint32_t *avail_out);
LDG_EXPORT uint32_t libresteam_net_socks_poll_group_create(const libresteam_backend_set_t *set, uint32_t *group_out);
LDG_EXPORT uint32_t libresteam_net_socks_poll_group_destroy(const libresteam_backend_set_t *set, uint32_t group);
LDG_EXPORT uint32_t libresteam_net_socks_conn_poll_group_set(const libresteam_backend_set_t *set, uint32_t conn, uint32_t group);
LDG_EXPORT uint32_t libresteam_net_socks_poll_group_msgs_recv(const libresteam_backend_set_t *set, uint32_t group, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_net_socks_relay_authentication_tkt_receive(const libresteam_backend_set_t *set, const void *tkt, uint32_t size, void *parsed_out);
LDG_EXPORT uint32_t libresteam_net_socks_relay_authentication_tkt_find(const libresteam_backend_set_t *set, const libresteam_net_ident_t *game_srv, uint32_t remote_virtual_port, void *parsed_out, uint32_t *expiry_sec_out);
LDG_EXPORT uint32_t libresteam_net_socks_hosted_dedicated_srv_connect(const libresteam_backend_set_t *set, const libresteam_net_ident_t *tgt, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
LDG_EXPORT uint32_t libresteam_net_socks_hosted_dedicated_srv_port_get(const libresteam_backend_set_t *set, uint16_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_hosted_dedicated_srv_pop_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_hosted_dedicated_srv_addr_get(const libresteam_backend_set_t *set, void *routing, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_hosted_dedicated_srv_listen_sock_create(const libresteam_backend_set_t *set, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
LDG_EXPORT uint32_t libresteam_net_socks_game_coordinator_srv_login_get(const libresteam_backend_set_t *set, void *login_info, uint32_t *blob_size, void *blob, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_p2p_custom_sig_connect(const libresteam_backend_set_t *set, const libresteam_net_socks_sig_t *sig, const libresteam_net_ident_t *peer, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out);
LDG_EXPORT uint32_t libresteam_net_socks_p2p_custom_sig_receive(const libresteam_backend_set_t *set, const void *msg, uint32_t size, const libresteam_net_socks_sig_recv_t *recv);
LDG_EXPORT uint32_t libresteam_net_socks_certificate_req_get(const libresteam_backend_set_t *set, uint32_t *blob_size, void *blob, char *fail_msg);
LDG_EXPORT uint32_t libresteam_net_socks_certificate_set(const libresteam_backend_set_t *set, const void *certificate, uint32_t size, char *fail_msg);
LDG_EXPORT uint32_t libresteam_net_socks_ident_rst(const libresteam_backend_set_t *set, const libresteam_net_ident_t *ident);
LDG_EXPORT uint32_t libresteam_net_socks_cb_run(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_net_socks_fake_ip_req_begin(const libresteam_backend_set_t *set, uint32_t port_cunt);
LDG_EXPORT uint32_t libresteam_net_socks_fake_ip_get(const libresteam_backend_set_t *set, uint32_t first_port_idx, libresteam_net_fake_ip_res_t *out);
LDG_EXPORT uint32_t libresteam_net_socks_listen_sock_p2p_fake_ip_create(const libresteam_backend_set_t *set, uint32_t fake_port_idx, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out);
LDG_EXPORT uint32_t libresteam_net_socks_conn_remote_fake_ip_get(const libresteam_backend_set_t *set, uint32_t conn, libresteam_net_ip_addr_t *out, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_fake_udp_port_create(const libresteam_backend_set_t *set, uint32_t fake_srv_port_idx, void **port_out);
LDG_EXPORT uint32_t libresteam_net_socks_fake_udp_port_destroy(const libresteam_backend_set_t *set, void *port);
LDG_EXPORT uint32_t libresteam_net_socks_fake_udp_port_msg_send(const libresteam_backend_set_t *set, void *port, const libresteam_net_ip_addr_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_socks_fake_udp_port_msgs_recv(const libresteam_backend_set_t *set, void *port, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_net_socks_fake_udp_port_cleanup_schedule(const libresteam_backend_set_t *set, void *port, const libresteam_net_ip_addr_t *remote);

#endif
