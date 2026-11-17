#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>
#include <libresteam/net/utils/utils.h>
#include <libresteam/net/utils/ops.h>
#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_NETWORKING_UTILS_VERSION "SteamNetworkingUtils003"
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_METHOD_CUNT 22U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_004_VERSION "SteamNetworkingUtils004"
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_004_METHOD_CUNT 25U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_LOCATION_GET 2U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_AGE_NONE (-1.0F)
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_SHARED_LAST 13U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_SET 14U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_GET 15U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_INFO_GET 16U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_FIRST_GET 17U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_TO_STR 18U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_PARSE 19U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_TO_STR 20U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_PARSE 21U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE UINT32_MAX
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_UNKNOWN 0xFFFFFFFEU
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BAD_VAL 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BAD_SCOPE_OBJ 0xFFFFFFFEU
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BUFF_TOO_SMALL 0xFFFFFFFDU
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID 0U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_EFAIL 2U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE 0U
#define LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint32_t libresteam_shim_networking_utils_set_get(void *self, uint32_t idx_003, uint32_t idx_004, const libresteam_backend_set_t **out)
{
    const libresteam_shim_iface_t *iface = (const libresteam_shim_iface_t *)self;
    uint32_t idx = idx_004;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!iface)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (iface->id == LIBRESTEAM_SHIM_IFACE_NETWORKING_UTILS) { idx = idx_003; }

    if (idx == LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE) { return LDG_ERR_RANGE; }

    return libresteam_shim_set_get(self, idx, out);
}

static uint32_t libresteam_shim_networking_utils_ping(uint32_t res, uint32_t ms)
{
    if (res == LDG_ERR_AOK) { return ms; }

    if (res == LDG_ERR_AGAIN) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_UNKNOWN; }

    return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED;
}

static uint8_t libresteam_shim_networking_utils_flag(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint64_t libresteam_shim_networking_utils_msg_alloc(void *self, uint32_t size)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_net_msg_t *msg = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_MSG_ALLOC, LIBRESTEAM_STEAM_NET_UTILS_MSG_ALLOC, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_msg_alloc(set, size, &msg) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return (uint64_t)(uintptr_t)msg;
}

static uint32_t libresteam_shim_networking_utils_relay_status_get(void *self, libresteam_net_relay_status_t *details)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t avail = LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_RELAY_STATUS_GET, LIBRESTEAM_STEAM_NET_UTILS_RELAY_STATUS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_relay_status_get(set, details, &avail) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return avail;
}

static libresteam_fp32_t libresteam_shim_networking_utils_ping_location_get(void *self, libresteam_net_ping_location_t *location)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_fp32_t age = LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_AGE_NONE;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_LOCATION_GET, LIBRESTEAM_STEAM_NET_UTILS_LOCAL_PING_LOCATION_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_AGE_NONE; }

    if (libresteam_net_utils_local_ping_location_get(set, location, &age) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_AGE_NONE; }

    return age;
}

static uint32_t libresteam_shim_networking_utils_ping_between_estimate(void *self, const libresteam_net_ping_location_t *lhs, const libresteam_net_ping_location_t *rhs)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t ms = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_BETWEEN_ESTIMATE, LIBRESTEAM_STEAM_NET_UTILS_PING_BETWEEN_ESTIMATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED; }

    return libresteam_shim_networking_utils_ping(libresteam_net_utils_ping_between_estimate(set, lhs, rhs, &ms), ms);
}

static uint32_t libresteam_shim_networking_utils_ping_from_local_estimate(void *self, const libresteam_net_ping_location_t *remote)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t ms = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_FROM_LOCAL_ESTIMATE, LIBRESTEAM_STEAM_NET_UTILS_PING_FROM_LOCAL_ESTIMATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED; }

    return libresteam_shim_networking_utils_ping(libresteam_net_utils_ping_from_local_estimate(set, remote, &ms), ms);
}

static void libresteam_shim_networking_utils_ping_location_to_str(void *self, const libresteam_net_ping_location_t *location, char *buff, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_TO_STR, LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_TO_STR, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ping_location_to_str(set, location, buff, cap) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_networking_utils_ping_location_parse(void *self, const char *str, libresteam_net_ping_location_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_PARSE, LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_PARSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_utils_flag(libresteam_net_utils_ping_location_parse(set, str, out));
}

static uint8_t libresteam_shim_networking_utils_ping_data_up_to_date_get(void *self, libresteam_fp32_t max_age_sec)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_DATA_UP_TO_DATE_GET, LIBRESTEAM_STEAM_NET_UTILS_PING_DATA_UP_TO_DATE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_ping_data_up_to_date_get(set, max_age_sec, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint32_t libresteam_shim_networking_utils_ping_to_data_center_get(void *self, uint32_t pop, uint32_t *via_relay_pop_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t via = 0;
    uint32_t ms = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_PING_TO_DATA_CENTER_GET, LIBRESTEAM_STEAM_NET_UTILS_PING_TO_DATA_CENTER_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED; }

    res = libresteam_net_utils_ping_to_data_center_get(set, pop, &via, &ms);
    if (via_relay_pop_out) { *via_relay_pop_out = via; }

    return libresteam_shim_networking_utils_ping(res, ms);
}

static uint32_t libresteam_shim_networking_utils_direct_ping_to_pop_get(void *self, uint32_t pop)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t ms = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_DIRECT_PING_TO_POP_GET, LIBRESTEAM_STEAM_NET_UTILS_DIRECT_PING_TO_POP_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_FAILED; }

    return libresteam_shim_networking_utils_ping(libresteam_net_utils_direct_ping_to_pop_get(set, pop, &ms), ms);
}

static uint32_t libresteam_shim_networking_utils_pop_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_POP_CUNT_GET, LIBRESTEAM_STEAM_NET_UTILS_POP_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_pop_cunt_get(set, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return cunt;
}

static uint32_t libresteam_shim_networking_utils_pop_list_get(void *self, uint32_t *list, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_POP_LIST_GET, LIBRESTEAM_STEAM_NET_UTILS_POP_LIST_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_pop_list_get(set, list, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return cunt;
}

static uint64_t libresteam_shim_networking_utils_local_timestamp_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t timestamp = 0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_LOCAL_TIMESTAMP_GET, LIBRESTEAM_STEAM_NET_UTILS_LOCAL_TIMESTAMP_GET, &set) != LDG_ERR_AOK) { return timestamp; }

    if (libresteam_net_utils_local_timestamp_get(set, &timestamp) != LDG_ERR_AOK) { timestamp = 0; }

    return timestamp;
}

static void libresteam_shim_networking_utils_debug_output_set(void *self, uint32_t detail_lvl, libresteam_net_utils_debug_cb_t cb)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_STEAM_NET_UTILS_DEBUG_OUTPUT_SET, LIBRESTEAM_STEAM_NET_UTILS_DEBUG_OUTPUT_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_debug_output_set(set, detail_lvl, cb) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_utils_ipv4_fake_ip_type_get(void *self, uint32_t ipv4)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t type = LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, LIBRESTEAM_STEAM_NET_UTILS_IPV4_FAKE_IP_TYPE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_ipv4_fake_ip_type_get(set, ipv4, &type) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return type;
}

static uint32_t libresteam_shim_networking_utils_real_ident_for_fake_ip_get(void *self, const libresteam_net_ip_addr_t *fake_ip, libresteam_net_ident_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_UTILS_EFAIL;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, LIBRESTEAM_STEAM_NET_UTILS_REAL_IDENT_FOR_FAKE_IP_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_EFAIL; }

    if (libresteam_net_utils_real_ident_for_fake_ip_get(set, fake_ip, out, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_EFAIL; }

    return eres;
}

static uint8_t libresteam_shim_networking_utils_conf_val_set(void *self, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_SET, LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_utils_flag(libresteam_net_utils_conf_val_set(set, val, scope, scope_obj, data_type, arg));
}

static uint32_t libresteam_shim_networking_utils_conf_val_get(void *self, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t *data_type_out, void *buff, uint64_t *buff_size)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t status = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_GET, LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BAD_VAL; }

    res = libresteam_net_utils_conf_val_get(set, val, scope, scope_obj, data_type_out, buff, buff_size, &status);
    if (res == LDG_ERR_AOK) { return status; }

    if (res == LDG_ERR_FUNC_ARG_INVALID) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BAD_SCOPE_OBJ; }

    if (res == LDG_ERR_OVERFLOW) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BUFF_TOO_SMALL; }

    return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_GET_BAD_VAL;
}

static uint64_t libresteam_shim_networking_utils_conf_val_info_get(void *self, uint32_t val, uint32_t *data_type_out, uint32_t *scope_out)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *name = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_INFO_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_conf_val_info_get(set, val, data_type_out, scope_out, &name) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return (uint64_t)(uintptr_t)name;
}

static uint8_t libresteam_shim_networking_utils_003_conf_val_info_get(void *self, uint32_t val, const char **name_out, uint32_t *data_type_out, uint32_t *scope_out, uint32_t *next_out)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *name = 0x0;
    uint32_t next = LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_INFO_GET, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_conf_val_info_get(set, val, data_type_out, scope_out, &name) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_editable_conf_val_iterate(set, val, LDG_TRUTH_TRUE, &next) != LDG_ERR_AOK) { next = LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID; }

    if (name_out) { *name_out = name; }

    if (next_out) { *next_out = next; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_networking_utils_003_conf_val_first_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t next = LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_FIRST_GET, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID; }

    if (libresteam_net_utils_editable_conf_val_iterate(set, LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID, LDG_TRUTH_TRUE, &next) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID; }

    return next;
}

static uint32_t libresteam_shim_networking_utils_editable_conf_val_iterate(void *self, uint32_t cur, uint8_t dev_vars_en)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t next = LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, LIBRESTEAM_STEAM_NET_UTILS_EDITABLE_CONF_VAL_ITERATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID; }

    if (libresteam_net_utils_editable_conf_val_iterate(set, cur, dev_vars_en, &next) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_CONF_INVALID; }

    return next;
}

static void libresteam_shim_networking_utils_ip_addr_to_str(void *self, const libresteam_net_ip_addr_t *addr, char *buff, uint64_t cap, uint8_t port_en)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_TO_STR, LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_TO_STR, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ip_addr_to_str(set, addr, buff, cap, port_en) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_networking_utils_ip_addr_parse(void *self, libresteam_net_ip_addr_t *out, const char *str)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_PARSE, LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_PARSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_utils_flag(libresteam_net_utils_ip_addr_parse(set, str, out));
}

static uint32_t libresteam_shim_networking_utils_ip_addr_fake_ip_type_get(void *self, const libresteam_net_ip_addr_t *addr)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t type = LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_IDX_NONE, LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_FAKE_IP_TYPE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    if (libresteam_net_utils_ip_addr_fake_ip_type_get(set, addr, &type) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_UTILS_NONE; }

    return type;
}

static void libresteam_shim_networking_utils_ident_to_str(void *self, const libresteam_net_ident_t *ident, char *buff, uint64_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_TO_STR, LIBRESTEAM_STEAM_NET_UTILS_IDENT_TO_STR, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ident_to_str(set, ident, buff, cap) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_networking_utils_ident_parse(void *self, libresteam_net_ident_t *out, const char *str)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_utils_set_get(self, LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_PARSE, LIBRESTEAM_STEAM_NET_UTILS_IDENT_PARSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_utils_flag(libresteam_net_utils_ident_parse(set, str, out));
}

static uint32_t libresteam_shim_networking_utils_shared_impl_get(uint32_t idx, void **out)
{
    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_UTILS_MSG_ALLOC:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_msg_alloc)
        case LIBRESTEAM_STEAM_NET_UTILS_RELAY_STATUS_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_relay_status_get)
        case LIBRESTEAM_STEAM_NET_UTILS_LOCAL_PING_LOCATION_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_location_get)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_BETWEEN_ESTIMATE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_between_estimate)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_FROM_LOCAL_ESTIMATE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_from_local_estimate)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_TO_STR:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_location_to_str)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_LOCATION_PARSE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_location_parse)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_DATA_UP_TO_DATE_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_data_up_to_date_get)
        case LIBRESTEAM_STEAM_NET_UTILS_PING_TO_DATA_CENTER_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ping_to_data_center_get)
        case LIBRESTEAM_STEAM_NET_UTILS_DIRECT_PING_TO_POP_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_direct_ping_to_pop_get)
        case LIBRESTEAM_STEAM_NET_UTILS_POP_CUNT_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_pop_cunt_get)
        case LIBRESTEAM_STEAM_NET_UTILS_POP_LIST_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_pop_list_get)
        case LIBRESTEAM_STEAM_NET_UTILS_LOCAL_TIMESTAMP_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_local_timestamp_get)
        case LIBRESTEAM_STEAM_NET_UTILS_DEBUG_OUTPUT_SET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_debug_output_set)
        default: { return LDG_ERR_RANGE;
        }
    }
}

static uint32_t libresteam_shim_networking_utils_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_SLOT_FIRST:
        { *out = "AllocateMessage";
          break;
        }
        case LIBRESTEAM_SHIM_SLOT_SECOND:
        { *out = "GetRelayNetworkStatus";
          break;
        }
        case 2:
        { *out = "GetLocalPingLocation";
          break;
        }
        case 3:
        { *out = "EstimatePingTimeBetweenTwoLocations";
          break;
        }
        case 4:
        { *out = "EstimatePingTimeFromLocalHost";
          break;
        }
        case 5:
        { *out = "ConvertPingLocationToString";
          break;
        }
        case 6:
        { *out = "ParsePingLocationString";
          break;
        }
        case 7:
        { *out = "CheckPingDataUpToDate";
          break;
        }
        case 8:
        { *out = "GetPingToDataCenter";
          break;
        }
        case 9:
        { *out = "GetDirectPingToPOP";
          break;
        }
        case 10:
        { *out = "GetPOPCount";
          break;
        }
        case 11:
        { *out = "GetPOPList";
          break;
        }
        case 12:
        { *out = "GetLocalTimestamp";
          break;
        }
        case 13:
        { *out = "SetDebugOutputFunction";
          break;
        }
        case 14:
        { *out = "SetConfigValue";
          break;
        }
        case 15:
        { *out = "GetConfigValue";
          break;
        }
        case 16:
        { *out = "GetConfigValueInfo";
          break;
        }
        case 17:
        { *out = "GetFirstConfigValue";
          break;
        }
        case 18:
        { *out = "SteamNetworkingIPAddr_ToString";
          break;
        }
        case 19:
        { *out = "SteamNetworkingIPAddr_ParseString";
          break;
        }
        case 20:
        { *out = "SteamNetworkingIdentity_ToString";
          break;
        }
        case 21:
        { *out = "SteamNetworkingIdentity_ParseString";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_utils_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_UTILS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_SHIM_NETWORKING_UTILS_PING_LOCATION_GET) { *out = LIBRESTEAM_SHIM_RET_FP32; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_utils_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_UTILS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx <= LIBRESTEAM_SHIM_NETWORKING_UTILS_SHARED_LAST) { return libresteam_shim_networking_utils_shared_impl_get(idx, out); }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_SET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_conf_val_set)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_conf_val_get)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_INFO_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_003_conf_val_info_get)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_CONF_VAL_FIRST_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_003_conf_val_first_get)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_TO_STR:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ip_addr_to_str)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IP_ADDR_PARSE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ip_addr_parse)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_TO_STR:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ident_to_str)
        case LIBRESTEAM_SHIM_NETWORKING_UTILS_003_IDENT_PARSE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ident_parse)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_networking_utils_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_UTILS_VERSION;
    out->name_get = libresteam_shim_networking_utils_name_get;
    out->kind_get = libresteam_shim_networking_utils_kind_get;
    out->impl_get = libresteam_shim_networking_utils_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_UTILS_METHOD_CUNT;

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_utils_004_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (idx <= LIBRESTEAM_SHIM_NETWORKING_UTILS_SHARED_LAST) { return libresteam_shim_networking_utils_name_get(idx, out); }

    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_UTILS_IPV4_FAKE_IP_TYPE_GET:
        { *out = "GetIPv4FakeIPType";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_REAL_IDENT_FOR_FAKE_IP_GET:
        { *out = "GetRealIdentityForFakeIP";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_SET:
        { *out = "SetConfigValue";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_GET:
        { *out = "GetConfigValue";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_INFO_GET:
        { *out = "GetConfigValueInfo";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_EDITABLE_CONF_VAL_ITERATE:
        { *out = "IterateGenericEditableConfigValues";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_TO_STR:
        { *out = "SteamNetworkingIPAddr_ToString";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_PARSE:
        { *out = "SteamNetworkingIPAddr_ParseString";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_FAKE_IP_TYPE_GET:
        { *out = "SteamNetworkingIPAddr_GetFakeIPType";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_IDENT_TO_STR:
        { *out = "SteamNetworkingIdentity_ToString";
          break;
        }
        case LIBRESTEAM_STEAM_NET_UTILS_IDENT_PARSE:
        { *out = "SteamNetworkingIdentity_ParseString";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_utils_004_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_UTILS_004_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_STEAM_NET_UTILS_LOCAL_PING_LOCATION_GET) { *out = LIBRESTEAM_SHIM_RET_FP32; }

    if (idx == LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_INFO_GET) { *out = LIBRESTEAM_SHIM_RET_STR; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_utils_004_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_UTILS_004_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx <= LIBRESTEAM_SHIM_NETWORKING_UTILS_SHARED_LAST) { return libresteam_shim_networking_utils_shared_impl_get(idx, out); }

    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_UTILS_IPV4_FAKE_IP_TYPE_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ipv4_fake_ip_type_get)
        case LIBRESTEAM_STEAM_NET_UTILS_REAL_IDENT_FOR_FAKE_IP_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_real_ident_for_fake_ip_get)
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_SET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_conf_val_set)
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_conf_val_get)
        case LIBRESTEAM_STEAM_NET_UTILS_CONF_VAL_INFO_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_conf_val_info_get)
        case LIBRESTEAM_STEAM_NET_UTILS_EDITABLE_CONF_VAL_ITERATE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_editable_conf_val_iterate)
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_TO_STR:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ip_addr_to_str)
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_PARSE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ip_addr_parse)
        case LIBRESTEAM_STEAM_NET_UTILS_IP_ADDR_FAKE_IP_TYPE_GET:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ip_addr_fake_ip_type_get)
        case LIBRESTEAM_STEAM_NET_UTILS_IDENT_TO_STR:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ident_to_str)
        case LIBRESTEAM_STEAM_NET_UTILS_IDENT_PARSE:
            LIBRESTEAM_SHIM_NETWORKING_UTILS_IMPL(libresteam_shim_networking_utils_ident_parse)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_networking_utils_004_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_UTILS_004_VERSION;
    out->name_get = libresteam_shim_networking_utils_004_name_get;
    out->kind_get = libresteam_shim_networking_utils_004_kind_get;
    out->impl_get = libresteam_shim_networking_utils_004_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_UTILS_004_METHOD_CUNT;

    return LDG_ERR_AOK;
}
