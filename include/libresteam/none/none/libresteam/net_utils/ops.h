#ifndef LIBRESTEAM_NET_UTILS_OPS_H
#define LIBRESTEAM_NET_UTILS_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>

#define LIBRESTEAM_NET_UTILS_CONF_GET_OK 1U
#define LIBRESTEAM_NET_UTILS_CONF_GET_OK_INHERITED 2U

typedef void (*libresteam_net_utils_debug_cb_t)(uint32_t type, const char *msg);

typedef struct libresteam_net_utils_ops
{
    uint32_t (*msg_alloc)(void *ctx, uint32_t size, libresteam_net_msg_t **out);
    uint32_t (*relay_status_get)(void *ctx, libresteam_net_relay_status_t *details, uint32_t *avail_out);
    uint32_t (*local_ping_location_get)(void *ctx, libresteam_net_ping_location_t *out, libresteam_fp32_t *age_out);
    uint32_t (*ping_between_estimate)(void *ctx, const libresteam_net_ping_location_t *lhs, const libresteam_net_ping_location_t *rhs, uint32_t *ms_out);
    uint32_t (*ping_from_local_estimate)(void *ctx, const libresteam_net_ping_location_t *remote, uint32_t *ms_out);
    uint32_t (*ping_location_to_str)(void *ctx, const libresteam_net_ping_location_t *location, char *buff, uint32_t cap);
    uint32_t (*ping_location_parse)(void *ctx, const char *str, libresteam_net_ping_location_t *out);
    uint32_t (*ping_data_up_to_date_get)(void *ctx, libresteam_fp32_t max_age_sec, uint8_t *out);
    uint32_t (*ping_to_data_center_get)(void *ctx, uint32_t pop, uint32_t *via_relay_pop_out, uint32_t *ms_out);
    uint32_t (*direct_ping_to_pop_get)(void *ctx, uint32_t pop, uint32_t *ms_out);
    uint32_t (*pop_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*pop_list_get)(void *ctx, uint32_t *list, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*local_timestamp_get)(void *ctx, uint64_t *out);
    uint32_t (*debug_output_set)(void *ctx, uint32_t detail_lvl, libresteam_net_utils_debug_cb_t cb);
    uint32_t (*ipv4_fake_ip_type_get)(void *ctx, uint32_t ipv4, uint32_t *out);
    uint32_t (*real_ident_for_fake_ip_get)(void *ctx, const libresteam_net_ip_addr_t *fake_ip, libresteam_net_ident_t *out, uint32_t *eres_out);
    uint32_t (*conf_val_set)(void *ctx, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg);
    uint32_t (*conf_val_get)(void *ctx, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t *data_type_out, void *res, uint64_t *res_size, uint32_t *get_res_out);
    uint32_t (*conf_val_info_get)(void *ctx, uint32_t val, uint32_t *data_type_out, uint32_t *scope_out, const char **name_out);
    uint32_t (*editable_conf_val_iterate)(void *ctx, uint32_t cur, uint8_t dev_vars_en, uint32_t *next_out);
    uint32_t (*ip_addr_to_str)(void *ctx, const libresteam_net_ip_addr_t *addr, char *buff, uint64_t cap, uint8_t port_en);
    uint32_t (*ip_addr_parse)(void *ctx, const char *str, libresteam_net_ip_addr_t *out);
    uint32_t (*ip_addr_fake_ip_type_get)(void *ctx, const libresteam_net_ip_addr_t *addr, uint32_t *out);
    uint32_t (*ident_to_str)(void *ctx, const libresteam_net_ident_t *ident, char *buff, uint64_t cap);
    uint32_t (*ident_parse)(void *ctx, const char *str, libresteam_net_ident_t *out);
} libresteam_net_utils_ops_t;

#endif
