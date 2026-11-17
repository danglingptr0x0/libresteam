#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/net/net.h>
#include <libresteam/net/types.h>

#define NET_LAYOUT_PASS 1
#define NET_LAYOUT_FAIL 0

typedef struct { uint32_t ck : (sizeof(libresteam_net_ip_addr_t) == LIBRESTEAM_NET_IP_ADDR_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_ip_addr_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_ident_t) == LIBRESTEAM_NET_IDENT_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_ident_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_conn_info_t) == LIBRESTEAM_NET_CONN_INFO_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_conn_info_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_real_time_status_t) == LIBRESTEAM_NET_REAL_TIME_STATUS_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_real_time_status_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_lane_status_t) == LIBRESTEAM_NET_LANE_STATUS_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_lane_status_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_msg_t) == LIBRESTEAM_NET_MSG_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_msg_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_ping_location_t) == LIBRESTEAM_NET_PING_LOCATION_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_ping_location_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_conf_val_t) == LIBRESTEAM_NET_CONF_VAL_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_conf_val_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_avail_status_t) == LIBRESTEAM_NET_AVAIL_STATUS_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_avail_status_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_relay_status_t) == LIBRESTEAM_NET_RELAY_STATUS_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_relay_status_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_net_fake_ip_res_t) == LIBRESTEAM_NET_FAKE_IP_RES_SIZE) ? NET_LAYOUT_PASS : NET_LAYOUT_FAIL;
} net_fake_ip_res_size_ck_t;

uint32_t libresteam_net_msg_release(libresteam_net_msg_t *msg)
{
    if (LDG_UNLIKELY(!msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!msg->release_cb)) { return LDG_ERR_INVALID; }

    msg->release_cb(msg);

    return LDG_ERR_AOK;
}
