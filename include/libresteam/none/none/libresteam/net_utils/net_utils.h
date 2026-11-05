#ifndef LIBRESTEAM_NET_UTILS_H
#define LIBRESTEAM_NET_UTILS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>
#include <libresteam/net_utils/ops.h>

LDG_EXPORT uint32_t libresteam_net_utils_msg_alloc(const libresteam_backend_set_t *set, uint32_t size, libresteam_net_msg_t **out);
LDG_EXPORT uint32_t libresteam_net_utils_relay_status_get(const libresteam_backend_set_t *set, libresteam_net_relay_status_t *details, uint32_t *avail_out);
LDG_EXPORT uint32_t libresteam_net_utils_local_ping_location_get(const libresteam_backend_set_t *set, libresteam_net_ping_location_t *out, libresteam_fp32_t *age_out);
LDG_EXPORT uint32_t libresteam_net_utils_ping_between_estimate(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *lhs, const libresteam_net_ping_location_t *rhs, uint32_t *ms_out);
LDG_EXPORT uint32_t libresteam_net_utils_ping_from_local_estimate(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *remote, uint32_t *ms_out);
LDG_EXPORT uint32_t libresteam_net_utils_ping_location_to_str(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *location, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_net_utils_ping_location_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ping_location_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_ping_data_up_to_date_get(const libresteam_backend_set_t *set, libresteam_fp32_t max_age_sec, uint8_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_ping_to_data_center_get(const libresteam_backend_set_t *set, uint32_t pop, uint32_t *via_relay_pop_out, uint32_t *ms_out);
LDG_EXPORT uint32_t libresteam_net_utils_direct_ping_to_pop_get(const libresteam_backend_set_t *set, uint32_t pop, uint32_t *ms_out);
LDG_EXPORT uint32_t libresteam_net_utils_pop_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_pop_list_get(const libresteam_backend_set_t *set, uint32_t *list, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_net_utils_local_timestamp_get(const libresteam_backend_set_t *set, uint64_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_debug_output_set(const libresteam_backend_set_t *set, uint32_t detail_lvl, libresteam_net_utils_debug_cb_t cb);
LDG_EXPORT uint32_t libresteam_net_utils_ipv4_fake_ip_type_get(const libresteam_backend_set_t *set, uint32_t ipv4, uint32_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_real_ident_for_fake_ip_get(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *fake_ip, libresteam_net_ident_t *out, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_utils_conf_val_set(const libresteam_backend_set_t *set, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg);
LDG_EXPORT uint32_t libresteam_net_utils_conf_val_get(const libresteam_backend_set_t *set, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t *data_type_out, void *res, uint64_t *res_size, uint32_t *get_res_out);
LDG_EXPORT uint32_t libresteam_net_utils_conf_val_info_get(const libresteam_backend_set_t *set, uint32_t val, uint32_t *data_type_out, uint32_t *scope_out, const char **name_out);
LDG_EXPORT uint32_t libresteam_net_utils_editable_conf_val_iterate(const libresteam_backend_set_t *set, uint32_t cur, uint8_t dev_vars_en, uint32_t *next_out);
LDG_EXPORT uint32_t libresteam_net_utils_ip_addr_to_str(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, char *buff, uint64_t cap, uint8_t port_en);
LDG_EXPORT uint32_t libresteam_net_utils_ip_addr_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ip_addr_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_ip_addr_fake_ip_type_get(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, uint32_t *out);
LDG_EXPORT uint32_t libresteam_net_utils_ident_to_str(const libresteam_backend_set_t *set, const libresteam_net_ident_t *ident, char *buff, uint64_t cap);
LDG_EXPORT uint32_t libresteam_net_utils_ident_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ident_t *out);

#endif
