#ifndef LIBRESTEAM_SHIM_FLAT_H
#define LIBRESTEAM_SHIM_FLAT_H

#include <stdint.h>

#include <libresteam/cb/msg.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>
#include <libresteam/srv_browser/ops.h>

uint8_t libresteam_shim_flat_init_anon(void);
uint8_t libresteam_shim_flat_steam_running_is(void);
const char* libresteam_shim_flat_install_path_get(void);
void libresteam_shim_flat_thread_mem_release(void);
void libresteam_shim_flat_mini_dump_wr(uint32_t code, void *info, uint32_t build);
void libresteam_shim_flat_mini_dump_comment_set(const char *msg);
void libresteam_shim_flat_breakpad_app_id_set(uint32_t app_id);
void libresteam_shim_flat_breakpad_use(uint8_t en);
void libresteam_shim_flat_try_catch_set(uint8_t en);
void libresteam_shim_flat_call_res_reg(void *obj, uint64_t call);
void libresteam_shim_flat_call_res_unreg(void *obj, uint64_t call);
void libresteam_shim_flat_manual_dispatch_init(void);
void libresteam_shim_flat_manual_dispatch_frame_run(uint32_t pipe);
uint8_t libresteam_shim_flat_manual_dispatch_next_get(uint32_t pipe, libresteam_cb_msg_t *out);
void libresteam_shim_flat_manual_dispatch_last_free(uint32_t pipe);
uint8_t libresteam_shim_flat_manual_dispatch_call_res_get(uint32_t pipe, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out);
void* libresteam_shim_flat_steam_client_get(void);
uint8_t libresteam_shim_flat_game_srv_secure_is(void);
uint64_t libresteam_shim_flat_game_srv_steam_id_get(void);
uint32_t libresteam_shim_flat_game_srv_ipc_call_cunt_get(void);
uint8_t libresteam_shim_flat_ip_addr_t_set_is(const void *self);
void libresteam_shim_flat_kv_construct(void *self);
void libresteam_shim_flat_netadr_construct(libresteam_srv_browser_netadr_t *self);
void libresteam_shim_flat_netadr_init(libresteam_srv_browser_netadr_t *self, uint32_t ip, uint16_t query_port, uint16_t conn_port);
uint16_t libresteam_shim_flat_netadr_query_port_get(const libresteam_srv_browser_netadr_t *self);
void libresteam_shim_flat_netadr_query_port_set(libresteam_srv_browser_netadr_t *self, uint16_t port);
uint16_t libresteam_shim_flat_netadr_conn_port_get(const libresteam_srv_browser_netadr_t *self);
void libresteam_shim_flat_netadr_conn_port_set(libresteam_srv_browser_netadr_t *self, uint16_t port);
uint32_t libresteam_shim_flat_netadr_ip_get(const libresteam_srv_browser_netadr_t *self);
void libresteam_shim_flat_netadr_ip_set(libresteam_srv_browser_netadr_t *self, uint32_t ip);
const char* libresteam_shim_flat_netadr_conn_str_get(const libresteam_srv_browser_netadr_t *self);
const char* libresteam_shim_flat_netadr_query_str_get(const libresteam_srv_browser_netadr_t *self);
uint8_t libresteam_shim_flat_netadr_less_is(const libresteam_srv_browser_netadr_t *self, const libresteam_srv_browser_netadr_t *rhs);
void libresteam_shim_flat_netadr_assign(libresteam_srv_browser_netadr_t *self, const libresteam_srv_browser_netadr_t *rhs);
void libresteam_shim_flat_srv_item_construct(libresteam_srv_browser_item_t *self);
const char* libresteam_shim_flat_srv_item_name_get(const libresteam_srv_browser_item_t *self);
void libresteam_shim_flat_srv_item_name_set(libresteam_srv_browser_item_t *self, const char *name);
void libresteam_shim_flat_ip_clear(libresteam_net_ip_addr_t *self);
uint8_t libresteam_shim_flat_ip_ipv6_zero_is(const libresteam_net_ip_addr_t *self);
void libresteam_shim_flat_ip_ipv6_set(libresteam_net_ip_addr_t *self, const uint8_t *ipv6, uint16_t port);
void libresteam_shim_flat_ip_ipv4_set(libresteam_net_ip_addr_t *self, uint32_t ip, uint16_t port);
uint8_t libresteam_shim_flat_ip_ipv4_is(const libresteam_net_ip_addr_t *self);
uint32_t libresteam_shim_flat_ip_ipv4_get(const libresteam_net_ip_addr_t *self);
void libresteam_shim_flat_ip_ipv6_localhost_set(libresteam_net_ip_addr_t *self, uint16_t port);
uint8_t libresteam_shim_flat_ip_localhost_is(const libresteam_net_ip_addr_t *self);
void libresteam_shim_flat_ip_to_str(const libresteam_net_ip_addr_t *self, char *buff, uint32_t cap, uint8_t port_en);
uint8_t libresteam_shim_flat_ip_parse(libresteam_net_ip_addr_t *self, const char *str);
uint8_t libresteam_shim_flat_ip_equal_is(const libresteam_net_ip_addr_t *self, const libresteam_net_ip_addr_t *rhs);
uint32_t libresteam_shim_flat_ip_fake_type_get(const libresteam_net_ip_addr_t *self);
uint8_t libresteam_shim_flat_ip_fake_is(const libresteam_net_ip_addr_t *self);
void libresteam_shim_flat_ident_clear(libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_invalid_is(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_steam_id_set(libresteam_net_ident_t *self, uint64_t id);
uint64_t libresteam_shim_flat_ident_steam_id_get(const libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_xbox_set(libresteam_net_ident_t *self, const char *str);
const char* libresteam_shim_flat_ident_xbox_get(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_psn_set(libresteam_net_ident_t *self, uint64_t id);
uint64_t libresteam_shim_flat_ident_psn_get(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_stadia_set(libresteam_net_ident_t *self, uint64_t id);
uint64_t libresteam_shim_flat_ident_stadia_get(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_ip_set(libresteam_net_ident_t *self, const libresteam_net_ip_addr_t *addr);
const libresteam_net_ip_addr_t* libresteam_shim_flat_ident_ip_get(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_ipv4_set(libresteam_net_ident_t *self, uint32_t ip, uint16_t port);
uint32_t libresteam_shim_flat_ident_ipv4_get(const libresteam_net_ident_t *self);
uint32_t libresteam_shim_flat_ident_fake_type_get(const libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_fake_is(const libresteam_net_ident_t *self);
void libresteam_shim_flat_ident_localhost_set(libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_localhost_is(const libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_generic_str_set(libresteam_net_ident_t *self, const char *str);
const char* libresteam_shim_flat_ident_generic_str_get(const libresteam_net_ident_t *self);
uint8_t libresteam_shim_flat_ident_generic_bytes_set(libresteam_net_ident_t *self, const void *data, uint32_t len);
const uint8_t* libresteam_shim_flat_ident_generic_bytes_get(const libresteam_net_ident_t *self, uint32_t *len_out);
uint8_t libresteam_shim_flat_ident_equal_is(const libresteam_net_ident_t *self, const libresteam_net_ident_t *rhs);
void libresteam_shim_flat_ident_to_str(const libresteam_net_ident_t *self, char *buff, uint32_t cap);
uint8_t libresteam_shim_flat_ident_parse(libresteam_net_ident_t *self, const char *str);
void libresteam_shim_flat_msg_release(libresteam_net_msg_t *self);
void libresteam_shim_flat_conf_int32_set(libresteam_net_conf_val_t *self, uint32_t val, uint32_t data);
void libresteam_shim_flat_conf_int64_set(libresteam_net_conf_val_t *self, uint32_t val, uint64_t data);
void libresteam_shim_flat_conf_fp32_set(libresteam_net_conf_val_t *self, uint32_t val, libresteam_fp32_t data);
void libresteam_shim_flat_conf_ptr_set(libresteam_net_conf_val_t *self, uint32_t val, void *data);
void libresteam_shim_flat_conf_str_set(libresteam_net_conf_val_t *self, uint32_t val, const char *data);
void libresteam_shim_flat_net_utils_relay_init(void *self);
uint8_t libresteam_shim_flat_net_utils_fake_ipv4_is(void *self, uint32_t ip);
uint8_t libresteam_shim_flat_net_utils_glob_int32_set(void *self, uint32_t val, uint32_t data);
uint8_t libresteam_shim_flat_net_utils_glob_fp32_set(void *self, uint32_t val, libresteam_fp32_t data);
uint8_t libresteam_shim_flat_net_utils_glob_str_set(void *self, uint32_t val, const char *data);
uint8_t libresteam_shim_flat_net_utils_glob_ptr_set(void *self, uint32_t val, void *data);
uint8_t libresteam_shim_flat_net_utils_conn_int32_set(void *self, uint32_t conn, uint32_t val, uint32_t data);
uint8_t libresteam_shim_flat_net_utils_conn_fp32_set(void *self, uint32_t conn, uint32_t val, libresteam_fp32_t data);
uint8_t libresteam_shim_flat_net_utils_conn_str_set(void *self, uint32_t conn, uint32_t val, const char *data);
uint8_t libresteam_shim_flat_net_utils_cb_conn_status_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_cb_auth_status_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_cb_relay_status_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_cb_fake_ip_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_cb_msgs_sesh_req_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_cb_msgs_sesh_failed_set(void *self, void *fn);
uint8_t libresteam_shim_flat_net_utils_conf_struct_set(void *self, const libresteam_net_conf_val_t *opt, uint32_t scope, uint64_t scope_obj);
void libresteam_shim_flat_hosted_addr_clear(void *self);
uint32_t libresteam_shim_flat_hosted_addr_pop_get(const void *self);
void libresteam_shim_flat_hosted_addr_dev_set(void *self, uint32_t ip, uint16_t port, uint32_t pop);

#endif
