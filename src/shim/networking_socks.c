#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/mem/secure.h>

#include <libresteam/steam/steam.h>
#include <libresteam/net/types.h>
#include <libresteam/net/socks/socks.h>
#include <libresteam/net/socks/ops.h>
#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_VERSION "SteamNetworkingSo" "ckets009"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_METHOD_CUNT 40U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_VERSION "SteamNetworkingSo" "ckets012"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_METHOD_CUNT 47U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_LISTEN "CreateListenSo" "cketP2P"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CONN "ConnectP2P"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CUSTOM_CONN "ConnectP2PCustomSignaling"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CUSTOM_SIG "ReceivedP2PCustomSignal"
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_QUICK_STATUS 16U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_CB_RUN 39U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_SHIFT 1U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL 2U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_EOK 1U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE 0U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_LANE_NONE 0U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_SEND 0U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_RELEASE 1U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_CONNECT_REQ 0U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_REJECTION_SEND 1U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_DESTROY 0U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_MSG_SEND 1U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_MSGS_RECV 2U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_CLEANUP_SCHEDULE 3U
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }
#define LIBRESTEAM_SHIM_NETWORKING_SOCKS_VFUNC_CPY(dst, fn) { __typeof__(&fn) cb = 0x0; cb = fn; res = ldg_mem_secure_cpy(&(dst), &cb, (uint64_t)sizeof(cb)); if (res != LDG_ERR_AOK) { return res; } }

static uint32_t libresteam_shim_networking_socks_set_get(void *self, uint32_t canon, const libresteam_backend_set_t **out)
{
    const libresteam_shim_iface_t *iface = (const libresteam_shim_iface_t *)self;
    uint32_t idx = canon;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!iface)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (iface->id == LIBRESTEAM_SHIM_IFACE_NETWORKING_SOCKS)
    {
        if (canon == LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET || canon == LIBRESTEAM_STEAM_NET_SOCKS_IDENT_RST || canon > LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN) { return LDG_ERR_RANGE; }

        if (canon == LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN) { idx = LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_CB_RUN; }
        else if (canon >= LIBRESTEAM_STEAM_NET_SOCKS_IDENT_GET) { idx = canon - LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_SHIFT; }
    }

    return libresteam_shim_set_get(self, idx, out);
}

static uint8_t libresteam_shim_networking_socks_flag(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_networking_socks_listen_sock_ip_create(void *self, const libresteam_net_ip_addr_t *local, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t sock = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_IP_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_listen_sock_ip_create(set, local, conf_val_cunt, conf_val, &sock) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return sock;
}

static uint32_t libresteam_shim_networking_socks_ip_addr_connect(void *self, const libresteam_net_ip_addr_t *addr, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t conn = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_IP_ADDR_CONNECT, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_ip_addr_connect(set, addr, conf_val_cunt, conf_val, &conn) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return conn;
}

static uint32_t libresteam_shim_networking_socks_listen_sock_p2p_create(void *self, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t sock = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_listen_sock_p2p_create(set, local_virtual_port, conf_val_cunt, conf_val, &sock) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return sock;
}

static uint32_t libresteam_shim_networking_socks_p2p_connect(void *self, const libresteam_net_ident_t *remote, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t conn = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_P2P_CONNECT, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_p2p_connect(set, remote, remote_virtual_port, conf_val_cunt, conf_val, &conn) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return conn;
}

static uint32_t libresteam_shim_networking_socks_conn_accept(void *self, uint32_t conn)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_ACCEPT, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_accept(set, conn, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint8_t libresteam_shim_networking_socks_conn_close(void *self, uint32_t conn, uint32_t reason, const char *debug, uint8_t linger_en)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_CLOSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_conn_close(set, conn, reason, debug, linger_en));
}

static uint8_t libresteam_shim_networking_socks_listen_sock_close(void *self, uint32_t sock)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_CLOSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_listen_sock_close(set, sock));
}

static uint8_t libresteam_shim_networking_socks_conn_user_data_set(void *self, uint32_t conn, uint64_t user_data)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_USER_DATA_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_conn_user_data_set(set, conn, user_data));
}

static uint64_t libresteam_shim_networking_socks_conn_user_data_get(void *self, uint32_t conn)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t user_data = UINT64_MAX;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_USER_DATA_GET, &set) != LDG_ERR_AOK) { return UINT64_MAX; }

    if (libresteam_net_socks_conn_user_data_get(set, conn, &user_data) != LDG_ERR_AOK) { return UINT64_MAX; }

    return user_data;
}

static void libresteam_shim_networking_socks_conn_name_set(void *self, uint32_t conn, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_NAME_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_conn_name_set(set, conn, name) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_networking_socks_conn_name_get(void *self, uint32_t conn, char *buff, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_NAME_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_conn_name_get(set, conn, buff, cap));
}

static uint32_t libresteam_shim_networking_socks_conn_msg_send(void *self, uint32_t conn, const void *data, uint32_t size, uint32_t flags, uint64_t *msg_num_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSG_SEND, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_msg_send(set, conn, data, size, flags, msg_num_out, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static void libresteam_shim_networking_socks_msgs_send(void *self, uint32_t msg_cunt, libresteam_net_msg_t *const *msg, uint64_t *msg_num_or_res_out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_MSGS_SEND, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_msgs_send(set, msg_cunt, msg, msg_num_or_res_out) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_socks_conn_msgs_flush(void *self, uint32_t conn)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSGS_FLUSH, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_msgs_flush(set, conn, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_socks_conn_msgs_recv(void *self, uint32_t conn, libresteam_net_msg_t **msg_out, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSGS_RECV, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    if (libresteam_net_socks_conn_msgs_recv(set, conn, msg_out, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    return cunt;
}

static uint8_t libresteam_shim_networking_socks_conn_info_get(void *self, uint32_t conn, libresteam_net_conn_info_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_INFO_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_conn_info_get(set, conn, out));
}

static uint32_t libresteam_shim_networking_socks_conn_real_time_status_get(void *self, uint32_t conn, libresteam_net_real_time_status_t *status, uint32_t lane_cunt, libresteam_net_lane_status_t *lane)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_real_time_status_get(set, conn, status, lane_cunt, lane, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint8_t libresteam_shim_networking_socks_quick_status_get(void *self, uint32_t conn, libresteam_net_real_time_status_t *status)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_socks_conn_real_time_status_get(set, conn, status, LIBRESTEAM_SHIM_NETWORKING_SOCKS_LANE_NONE, 0x0, &eres) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (eres != LIBRESTEAM_SHIM_NETWORKING_SOCKS_EOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_networking_socks_conn_detailed_status_get(void *self, uint32_t conn, char *buff, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t needed = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_DETAILED_STATUS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    res = libresteam_net_socks_conn_detailed_status_get(set, conn, buff, cap, &needed);
    if (res == LDG_ERR_OVERFLOW) { return needed; }

    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE;
}

static uint8_t libresteam_shim_networking_socks_listen_sock_addr_get(void *self, uint32_t sock, libresteam_net_ip_addr_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_ADDR_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_listen_sock_addr_get(set, sock, out));
}

static uint8_t libresteam_shim_networking_socks_sock_pair_create(void *self, uint32_t *conn_lhs_out, uint32_t *conn_rhs_out, uint8_t loopback_en, const libresteam_net_ident_t *ident_lhs, const libresteam_net_ident_t *ident_rhs)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_SOCK_PAIR_CREATE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_sock_pair_create(set, conn_lhs_out, conn_rhs_out, loopback_en, ident_lhs, ident_rhs));
}

static uint32_t libresteam_shim_networking_socks_conn_lane_conf_set(void *self, uint32_t conn, uint32_t lane_cunt, const uint32_t *prio, const uint16_t *weight)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_lane_conf_set(set, conn, lane_cunt, prio, weight, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint8_t libresteam_shim_networking_socks_ident_get(void *self, libresteam_net_ident_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_IDENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_ident_get(set, out));
}

static uint32_t libresteam_shim_networking_socks_authentication_init(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t avail = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_AUTHENTICATION_INIT, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_net_socks_authentication_init(set, &avail) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    return avail;
}

static uint32_t libresteam_shim_networking_socks_authentication_status_get(void *self, libresteam_net_avail_status_t *details)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t avail = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_AUTHENTICATION_STATUS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_net_socks_authentication_status_get(set, details, &avail) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    return avail;
}

static uint32_t libresteam_shim_networking_socks_poll_group_create(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t group = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_poll_group_create(set, &group) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return group;
}

static uint8_t libresteam_shim_networking_socks_poll_group_destroy(void *self, uint32_t group)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_DESTROY, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_poll_group_destroy(set, group));
}

static uint8_t libresteam_shim_networking_socks_conn_poll_group_set(void *self, uint32_t conn, uint32_t group)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_POLL_GROUP_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_conn_poll_group_set(set, conn, group));
}

static uint32_t libresteam_shim_networking_socks_poll_group_msgs_recv(void *self, uint32_t group, libresteam_net_msg_t **msg_out, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_MSGS_RECV, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    if (libresteam_net_socks_poll_group_msgs_recv(set, group, msg_out, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    return cunt;
}

static uint8_t libresteam_shim_networking_socks_relay_authentication_tkt_receive(void *self, const void *tkt, uint32_t size, void *parsed_out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_RECEIVE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_relay_authentication_tkt_receive(set, tkt, size, parsed_out));
}

static uint32_t libresteam_shim_networking_socks_relay_authentication_tkt_find(void *self, const libresteam_net_ident_t *game_srv, uint32_t remote_virtual_port, void *parsed_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t expiry_sec = 0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_FIND, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_net_socks_relay_authentication_tkt_find(set, game_srv, remote_virtual_port, parsed_out, &expiry_sec) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    return expiry_sec;
}

static uint32_t libresteam_shim_networking_socks_hosted_dedicated_srv_connect(void *self, const libresteam_net_ident_t *tgt, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t conn = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_CONNECT, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_hosted_dedicated_srv_connect(set, tgt, remote_virtual_port, conf_val_cunt, conf_val, &conn) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return conn;
}

static uint16_t libresteam_shim_networking_socks_hosted_dedicated_srv_port_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint16_t port = 0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_PORT_GET, &set) != LDG_ERR_AOK) { return port; }

    if (libresteam_net_socks_hosted_dedicated_srv_port_get(set, &port) != LDG_ERR_AOK) { port = 0; }

    return port;
}

static uint32_t libresteam_shim_networking_socks_hosted_dedicated_srv_pop_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t pop = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_POP_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_net_socks_hosted_dedicated_srv_pop_get(set, &pop) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    return pop;
}

static uint32_t libresteam_shim_networking_socks_hosted_dedicated_srv_addr_get(void *self, void *routing)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_ADDR_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_hosted_dedicated_srv_addr_get(set, routing, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_socks_hosted_dedicated_srv_listen_sock_create(void *self, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t sock = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_LISTEN_SOCK_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_hosted_dedicated_srv_listen_sock_create(set, local_virtual_port, conf_val_cunt, conf_val, &sock) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return sock;
}

static uint32_t libresteam_shim_networking_socks_game_coordinator_srv_login_get(void *self, void *login_info, uint32_t *blob_size, void *blob)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_GAME_COORDINATOR_SRV_LOGIN_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_game_coordinator_srv_login_get(set, login_info, blob_size, blob, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_socks_sig_send_cb(void *content, uint32_t conn, const libresteam_net_conn_info_t *info, const void *msg, uint32_t size)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!content)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_ctx_get(&ctx);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_slot_get(&ctx->net_sig_desc, LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_SEND, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = info, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = ldg_cpp_uint8_call(content, slot, arg, cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_DENIED; }

    return LDG_ERR_AOK;
}

static void libresteam_shim_networking_socks_sig_release_cb(void *content)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t slot = 0;

    if (LDG_UNLIKELY(!content)) { return; }

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return; }

    if (ldg_cpp_slot_get(&ctx->net_sig_desc, LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_RELEASE, &slot) != LDG_ERR_AOK) { return; }

    if (ldg_cpp_void_call(content, slot, 0x0, LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_socks_sig_connect_req_cb(void *content, uint32_t conn, const libresteam_net_ident_t *peer, uint32_t local_virtual_port, libresteam_net_socks_sig_t *sig_out)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t obj = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!content || !sig_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_ctx_get(&ctx);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_slot_get(&ctx->net_sig_recv_desc, LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_CONNECT_REQ, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = peer, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = local_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = ldg_cpp_uint64_call(content, slot, arg, cunt, &obj);
    if (res != LDG_ERR_AOK) { return res; }

    if (obj == 0) { return LDG_ERR_NOT_FOUND; }

    sig_out->ops = &ctx->net_sig_ops;
    sig_out->content = (void *)(uintptr_t)obj;

    return LDG_ERR_AOK;
}

static void libresteam_shim_networking_socks_sig_rejection_send_cb(void *content, const libresteam_net_ident_t *peer, const void *msg, uint32_t size)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!content)) { return; }

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return; }

    if (ldg_cpp_slot_get(&ctx->net_sig_recv_desc, LIBRESTEAM_SHIM_NETWORKING_SOCKS_SIG_REJECTION_SEND, &slot) != LDG_ERR_AOK) { return; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = peer, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    if (ldg_cpp_void_call(content, slot, arg, cunt) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_socks_p2p_custom_sig_connect(void *self, void *sig_obj, const libresteam_net_ident_t *peer, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_shim_ctx_t *ctx = 0x0;
    libresteam_net_socks_sig_t sig = LDG_STRUCT_ZERO_INIT;
    uint32_t conn = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_CONNECT, &set) != LDG_ERR_AOK || libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK)
    {
        libresteam_shim_networking_socks_sig_release_cb(sig_obj);
        return LIBRESTEAM_NET_HANDLE_INVALID;
    }

    sig.ops = &ctx->net_sig_ops;
    sig.content = sig_obj;

    if (libresteam_net_socks_p2p_custom_sig_connect(set, &sig, peer, remote_virtual_port, conf_val_cunt, conf_val, &conn) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return conn;
}

static uint8_t libresteam_shim_networking_socks_p2p_custom_sig_receive(void *self, const void *msg, uint32_t size, void *recv_obj)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_shim_ctx_t *ctx = 0x0;
    libresteam_net_socks_sig_recv_t recv = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_RECEIVE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    recv.ops = &ctx->net_sig_recv_ops;
    recv.content = recv_obj;

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_p2p_custom_sig_receive(set, msg, size, &recv));
}

static uint8_t libresteam_shim_networking_socks_certificate_req_get(void *self, uint32_t *blob_size, void *blob, char *fail_msg)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CERTIFICATE_REQ_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_certificate_req_get(set, blob_size, blob, fail_msg));
}

static uint8_t libresteam_shim_networking_socks_certificate_set(void *self, const void *certificate, uint32_t size, char *fail_msg)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CERTIFICATE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_certificate_set(set, certificate, size, fail_msg));
}

static void libresteam_shim_networking_socks_ident_rst(void *self, const libresteam_net_ident_t *ident)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_IDENT_RST, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_ident_rst(set, ident) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_networking_socks_cb_run(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_cb_run(set) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_networking_socks_fake_ip_req_begin(void *self, uint32_t port_cunt)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_REQ_BEGIN, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_socks_flag(libresteam_net_socks_fake_ip_req_begin(set, port_cunt));
}

static void libresteam_shim_networking_socks_fake_ip_get(void *self, uint32_t first_port_idx, libresteam_net_fake_ip_res_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_GET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_fake_ip_get(set, first_port_idx, out) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_socks_listen_sock_p2p_fake_ip_create(void *self, uint32_t fake_port_idx, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t sock = LIBRESTEAM_NET_HANDLE_INVALID;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_FAKE_IP_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    if (libresteam_net_socks_listen_sock_p2p_fake_ip_create(set, fake_port_idx, conf_val_cunt, conf_val, &sock) != LDG_ERR_AOK) { return LIBRESTEAM_NET_HANDLE_INVALID; }

    return sock;
}

static uint32_t libresteam_shim_networking_socks_conn_remote_fake_ip_get(void *self, uint32_t conn, libresteam_net_ip_addr_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_CONN_REMOTE_FAKE_IP_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_conn_remote_fake_ip_get(set, conn, out, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_socks_port_get(void *self, libresteam_shim_fake_udp_port_t **port_out, const libresteam_backend_set_t **set_out)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    libresteam_shim_fake_udp_port_t *port = (libresteam_shim_fake_udp_port_t *)self;
    uint32_t res = LDG_ERR_AOK;

    *port_out = 0x0;
    *set_out = 0x0;

    if (LDG_UNLIKELY(!port)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_ctx_get(&ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (LDG_UNLIKELY(ctx->steam_up != LDG_TRUTH_TRUE)) { return LDG_ERR_NOT_INIT; }

    if (LDG_UNLIKELY(port->in_use != LDG_TRUTH_TRUE || !port->port)) { return LDG_ERR_INVALID; }

    *port_out = port;
    *set_out = &ctx->set;

    return LDG_ERR_AOK;
}

static void libresteam_shim_networking_socks_port_destroy(void *self)
{
    libresteam_shim_fake_udp_port_t *port = 0x0;
    const libresteam_backend_set_t *set = 0x0;
    void *steam_port = 0x0;

    if (libresteam_shim_networking_socks_port_get(self, &port, &set) != LDG_ERR_AOK) { return; }

    steam_port = port->port;
    port->port = 0x0;
    port->in_use = LDG_TRUTH_FALSE;

    if (libresteam_net_socks_fake_udp_port_destroy(set, steam_port) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_networking_socks_port_msg_send(void *self, const libresteam_net_ip_addr_t *remote, const void *data, uint32_t size, uint32_t flags)
{
    libresteam_shim_fake_udp_port_t *port = 0x0;
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL;

    if (libresteam_shim_networking_socks_port_get(self, &port, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    if (libresteam_net_socks_fake_udp_port_msg_send(set, port->port, remote, data, size, flags, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_socks_port_msgs_recv(void *self, libresteam_net_msg_t **msg_out, uint32_t cap)
{
    libresteam_shim_fake_udp_port_t *port = 0x0;
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_networking_socks_port_get(self, &port, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    if (libresteam_net_socks_fake_udp_port_msgs_recv(set, port->port, msg_out, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_CUNT_INVALID; }

    return cunt;
}

static void libresteam_shim_networking_socks_port_cleanup_schedule(void *self, const libresteam_net_ip_addr_t *remote)
{
    libresteam_shim_fake_udp_port_t *port = 0x0;
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_networking_socks_port_get(self, &port, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_socks_fake_udp_port_cleanup_schedule(set, port->port, remote) != LDG_ERR_AOK) { return; }
}

static uint64_t libresteam_shim_networking_socks_fake_udp_port_create(void *self, uint32_t fake_srv_port_idx)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_shim_ctx_t *ctx = 0x0;
    libresteam_shim_fake_udp_port_t *free_port = 0x0;
    void *steam_port = 0x0;
    uint32_t idx = 0;

    if (libresteam_shim_networking_socks_set_get(self, LIBRESTEAM_STEAM_NET_SOCKS_FAKE_UDP_PORT_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    if (libresteam_net_socks_fake_udp_port_create(set, fake_srv_port_idx, &steam_port) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

    for (idx = 0; idx < LIBRESTEAM_SHIM_FAKE_UDP_PORT_MAX; idx++)
    {
        if (ctx->fake_udp_port[idx].in_use == LDG_TRUTH_TRUE && ctx->fake_udp_port[idx].port == steam_port) { return (uint64_t)(uintptr_t)&ctx->fake_udp_port[idx]; }

        if (!free_port && ctx->fake_udp_port[idx].in_use == LDG_TRUTH_FALSE) { free_port = &ctx->fake_udp_port[idx]; }
    }

    if (!free_port)
    {
        if (libresteam_net_socks_fake_udp_port_destroy(set, steam_port) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE; }

        return LIBRESTEAM_SHIM_NETWORKING_SOCKS_NONE;
    }

    free_port->port = steam_port;
    free_port->in_use = LDG_TRUTH_TRUE;

    return (uint64_t)(uintptr_t)free_port;
}

static uint32_t libresteam_shim_networking_socks_canon_impl_get(uint32_t canon, void **out)
{
    switch (canon)
    {
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_IP_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_listen_sock_ip_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_IP_ADDR_CONNECT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_ip_addr_connect)
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_listen_sock_p2p_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_P2P_CONNECT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_p2p_connect)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_ACCEPT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_accept)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_CLOSE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_close)
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_CLOSE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_listen_sock_close)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_USER_DATA_SET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_user_data_set)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_USER_DATA_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_user_data_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_NAME_SET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_name_set)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_NAME_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_name_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSG_SEND:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_msg_send)
        case LIBRESTEAM_STEAM_NET_SOCKS_MSGS_SEND:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_msgs_send)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSGS_FLUSH:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_msgs_flush)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_MSGS_RECV:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_msgs_recv)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_INFO_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_info_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_real_time_status_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_DETAILED_STATUS_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_detailed_status_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_ADDR_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_listen_sock_addr_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_SOCK_PAIR_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_sock_pair_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_lane_conf_set)
        case LIBRESTEAM_STEAM_NET_SOCKS_IDENT_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_ident_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_AUTHENTICATION_INIT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_authentication_init)
        case LIBRESTEAM_STEAM_NET_SOCKS_AUTHENTICATION_STATUS_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_authentication_status_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_poll_group_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_DESTROY:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_poll_group_destroy)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_POLL_GROUP_SET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_poll_group_set)
        case LIBRESTEAM_STEAM_NET_SOCKS_POLL_GROUP_MSGS_RECV:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_poll_group_msgs_recv)
        case LIBRESTEAM_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_RECEIVE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_relay_authentication_tkt_receive)
        case LIBRESTEAM_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_FIND:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_relay_authentication_tkt_find)
        case LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_CONNECT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_hosted_dedicated_srv_connect)
        case LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_PORT_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_hosted_dedicated_srv_port_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_POP_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_hosted_dedicated_srv_pop_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_ADDR_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_hosted_dedicated_srv_addr_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_LISTEN_SOCK_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_hosted_dedicated_srv_listen_sock_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_GAME_COORDINATOR_SRV_LOGIN_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_game_coordinator_srv_login_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_CONNECT:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_p2p_custom_sig_connect)
        case LIBRESTEAM_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_RECEIVE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_p2p_custom_sig_receive)
        case LIBRESTEAM_STEAM_NET_SOCKS_CERTIFICATE_REQ_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_certificate_req_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_CERTIFICATE_SET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_certificate_set)
        case LIBRESTEAM_STEAM_NET_SOCKS_IDENT_RST:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_ident_rst)
        case LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_cb_run)
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_REQ_BEGIN:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_fake_ip_req_begin)
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_fake_ip_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_FAKE_IP_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_listen_sock_p2p_fake_ip_create)
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_REMOTE_FAKE_IP_GET:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_conn_remote_fake_ip_get)
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_UDP_PORT_CREATE:
            LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_fake_udp_port_create)
        default: { return LDG_ERR_RANGE;
        }
    }
}

static uint32_t libresteam_shim_networking_socks_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_SLOT_FIRST:
        { *out = "CreateListenSocketIP";
          break;
        }
        case LIBRESTEAM_SHIM_SLOT_SECOND:
        { *out = "ConnectByIPAddress";
          break;
        }
        case 2:
        { *out = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_LISTEN;
          break;
        }
        case 3:
        { *out = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CONN;
          break;
        }
        case 4:
        { *out = "AcceptConnection";
          break;
        }
        case 5:
        { *out = "CloseConnection";
          break;
        }
        case 6:
        { *out = "CloseListenSocket";
          break;
        }
        case 7:
        { *out = "SetConnectionUserData";
          break;
        }
        case 8:
        { *out = "GetConnectionUserData";
          break;
        }
        case 9:
        { *out = "SetConnectionName";
          break;
        }
        case 10:
        { *out = "GetConnectionName";
          break;
        }
        case 11:
        { *out = "SendMessageToConnection";
          break;
        }
        case 12:
        { *out = "SendMessages";
          break;
        }
        case 13:
        { *out = "FlushMessagesOnConnection";
          break;
        }
        case 14:
        { *out = "ReceiveMessagesOnConnection";
          break;
        }
        case 15:
        { *out = "GetConnectionInfo";
          break;
        }
        case 16:
        { *out = "GetQuickConnectionStatus";
          break;
        }
        case 17:
        { *out = "GetDetailedConnectionStatus";
          break;
        }
        case 18:
        { *out = "GetListenSocketAddress";
          break;
        }
        case 19:
        { *out = "CreateSocketPair";
          break;
        }
        case 20:
        { *out = "GetIdentity";
          break;
        }
        case 21:
        { *out = "InitAuthentication";
          break;
        }
        case 22:
        { *out = "GetAuthenticationStatus";
          break;
        }
        case 23:
        { *out = "CreatePollGroup";
          break;
        }
        case 24:
        { *out = "DestroyPollGroup";
          break;
        }
        case 25:
        { *out = "SetConnectionPollGroup";
          break;
        }
        case 26:
        { *out = "ReceiveMessagesOnPollGroup";
          break;
        }
        case 27:
        { *out = "ReceivedRelayAuthTicket";
          break;
        }
        case 28:
        { *out = "FindRelayAuthTicketForServer";
          break;
        }
        case 29:
        { *out = "ConnectToHostedDedicatedServer";
          break;
        }
        case 30:
        { *out = "GetHostedDedicatedServerPort";
          break;
        }
        case 31:
        { *out = "GetHostedDedicatedServerPOPID";
          break;
        }
        case 32:
        { *out = "GetHostedDedicatedServerAddress";
          break;
        }
        case 33:
        { *out = "CreateHostedDedicatedServerListenSocket";
          break;
        }
        case 34:
        { *out = "GetGameCoordinatorServerLogin";
          break;
        }
        case 35:
        { *out = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CUSTOM_CONN;
          break;
        }
        case 36:
        { *out = LIBRESTEAM_SHIM_NETWORKING_SOCKS_NAME_CUSTOM_SIG;
          break;
        }
        case 37:
        { *out = "GetCertificateRequest";
          break;
        }
        case 38:
        { *out = "SetCertificate";
          break;
        }
        case 39:
        { *out = "RunCallbacks";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_socks_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_SOCKS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_socks_impl_get(uint32_t idx, void **out)
{
    uint32_t canon = idx;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_SOCKS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_QUICK_STATUS)
        LIBRESTEAM_SHIM_NETWORKING_SOCKS_IMPL(libresteam_shim_networking_socks_quick_status_get)

        if (idx == LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_CB_RUN) { canon = LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN; }
        else if (idx >= LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET) { canon = idx + LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_SHIFT; }

    return libresteam_shim_networking_socks_canon_impl_get(canon, out);
}

uint32_t libresteam_shim_networking_socks_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_SOCKS_VERSION;
    out->name_get = libresteam_shim_networking_socks_name_get;
    out->kind_get = libresteam_shim_networking_socks_kind_get;
    out->impl_get = libresteam_shim_networking_socks_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_SOCKS_METHOD_CUNT;

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_socks_012_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET:
        { *out = "GetConnectionRealTimeStatus";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_DETAILED_STATUS_GET:
        { *out = "GetDetailedConnectionStatus";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_ADDR_GET:
        { *out = "GetListenSocketAddress";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_SOCK_PAIR_CREATE:
        { *out = "CreateSocketPair";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET:
        { *out = "ConfigureConnectionLanes";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_IDENT_RST:
        { *out = "ResetIdentity";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_CB_RUN:
        { *out = "RunCallbacks";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_REQ_BEGIN:
        { *out = "BeginAsyncRequestFakeIP";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_IP_GET:
        { *out = "GetFakeIP";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_FAKE_IP_CREATE:
        { *out = "CreateListenSocketP2PFakeIP";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_CONN_REMOTE_FAKE_IP_GET:
        { *out = "GetRemoteFakeIPForConnection";
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_STEAM_NET_SOCKS_FAKE_UDP_PORT_CREATE:
        { *out = "CreateFakeUDPPort";
          return LDG_ERR_AOK;
        }
        default: { break;
        }
    }

    if (idx < LIBRESTEAM_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET) { return libresteam_shim_networking_socks_name_get(idx, out); }

    if (idx > LIBRESTEAM_STEAM_NET_SOCKS_CONN_LANE_CONF_SET && idx < LIBRESTEAM_STEAM_NET_SOCKS_IDENT_RST) { return libresteam_shim_networking_socks_name_get(idx - LIBRESTEAM_SHIM_NETWORKING_SOCKS_009_SHIFT, out); }

    return LDG_ERR_RANGE;
}

static uint32_t libresteam_shim_networking_socks_012_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_socks_012_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return libresteam_shim_networking_socks_canon_impl_get(idx, out);
}

uint32_t libresteam_shim_networking_socks_012_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_VERSION;
    out->name_get = libresteam_shim_networking_socks_012_name_get;
    out->kind_get = libresteam_shim_networking_socks_012_kind_get;
    out->impl_get = libresteam_shim_networking_socks_012_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_SOCKS_012_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_networking_socks_ctx_fill(libresteam_shim_ctx_t *ctx)
{
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    ctx->net_sig_ops.sig_send_cb = libresteam_shim_networking_socks_sig_send_cb;
    ctx->net_sig_ops.release_cb = libresteam_shim_networking_socks_sig_release_cb;
    ctx->net_sig_recv_ops.connect_req_cb = libresteam_shim_networking_socks_sig_connect_req_cb;
    ctx->net_sig_recv_ops.rejection_send_cb = libresteam_shim_networking_socks_sig_rejection_send_cb;

    for (idx = 0; idx < LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT; idx++)
    {
        ctx->net_sig_name_id[idx] = idx;
        ctx->net_sig_recv_name_id[idx] = idx;
    }

    ctx->net_sig_desc.name_id = ctx->net_sig_name_id;
    ctx->net_sig_desc.method_cunt = LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT;
    ctx->net_sig_desc.dtor_idx = LDG_CPP_DTOR_NONE;
    ctx->net_sig_recv_desc.name_id = ctx->net_sig_recv_name_id;
    ctx->net_sig_recv_desc.method_cunt = LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT;
    ctx->net_sig_recv_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    LIBRESTEAM_SHIM_NETWORKING_SOCKS_VFUNC_CPY(ctx->fake_udp_port_vtable[LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_DESTROY], libresteam_shim_networking_socks_port_destroy)
    LIBRESTEAM_SHIM_NETWORKING_SOCKS_VFUNC_CPY(ctx->fake_udp_port_vtable[LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_MSG_SEND], libresteam_shim_networking_socks_port_msg_send)
    LIBRESTEAM_SHIM_NETWORKING_SOCKS_VFUNC_CPY(ctx->fake_udp_port_vtable[LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_MSGS_RECV], libresteam_shim_networking_socks_port_msgs_recv)
    LIBRESTEAM_SHIM_NETWORKING_SOCKS_VFUNC_CPY(ctx->fake_udp_port_vtable[LIBRESTEAM_SHIM_NETWORKING_SOCKS_PORT_CLEANUP_SCHEDULE], libresteam_shim_networking_socks_port_cleanup_schedule)

    for (idx = 0; idx < LIBRESTEAM_SHIM_FAKE_UDP_PORT_MAX; idx++)
    {
        ctx->fake_udp_port[idx].vtable = ctx->fake_udp_port_vtable;
        ctx->fake_udp_port[idx].port = 0x0;
        ctx->fake_udp_port[idx].in_use = LDG_TRUTH_FALSE;
    }

    return LDG_ERR_AOK;
}
