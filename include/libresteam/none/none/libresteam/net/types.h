#ifndef LIBRESTEAM_NET_TYPES_H
#define LIBRESTEAM_NET_TYPES_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/fp32/fp32.h>

#define LIBRESTEAM_NET_HANDLE_INVALID 0U
#define LIBRESTEAM_NET_ERES_OK 1U
#define LIBRESTEAM_NET_IPV6_SIZE 16U
#define LIBRESTEAM_NET_IP_ADDR_SIZE 18U
#define LIBRESTEAM_NET_IP_ADDR_STR_MAX 48U
#define LIBRESTEAM_NET_IDENT_DATA_SIZE 128U
#define LIBRESTEAM_NET_IDENT_SIZE 136U
#define LIBRESTEAM_NET_IDENT_STR_MAX 128U
#define LIBRESTEAM_NET_CONN_CLOSE_REASON_MAX 128U
#define LIBRESTEAM_NET_CONN_DESC_MAX 128U
#define LIBRESTEAM_NET_CONN_INFO_RESERVED_CUNT 63U
#define LIBRESTEAM_NET_CONN_INFO_SIZE 696U
#define LIBRESTEAM_NET_REAL_TIME_RESERVED_CUNT 15U
#define LIBRESTEAM_NET_REAL_TIME_STATUS_SIZE 120U
#define LIBRESTEAM_NET_LANE_RESERVED_CUNT 10U
#define LIBRESTEAM_NET_LANE_STATUS_SIZE 64U
#define LIBRESTEAM_NET_MSG_SIZE 216U
#define LIBRESTEAM_NET_PING_LOCATION_SIZE 512U
#define LIBRESTEAM_NET_PING_LOCATION_STR_MAX 1024U
#define LIBRESTEAM_NET_CONF_VAL_SIZE 16U
#define LIBRESTEAM_NET_DEBUG_MSG_MAX 256U
#define LIBRESTEAM_NET_AVAIL_STATUS_SIZE 260U
#define LIBRESTEAM_NET_RELAY_STATUS_SIZE 272U
#define LIBRESTEAM_NET_FAKE_IP_PORT_MAX 8U
#define LIBRESTEAM_NET_FAKE_IP_RES_SIZE 160U
#define LIBRESTEAM_NET_ERR_MSG_MAX 1024U
#define LIBRESTEAM_NET_SEND_MSG_MAX 524288U

#define LIBRESTEAM_NET_SEND_UNRELIABLE 0U
#define LIBRESTEAM_NET_SEND_NO_NAGLE 1U
#define LIBRESTEAM_NET_SEND_NO_DELAY 4U
#define LIBRESTEAM_NET_SEND_RELIABLE 8U
#define LIBRESTEAM_NET_SEND_USE_CURRENT_THREAD 16U
#define LIBRESTEAM_NET_SEND_AUTO_RESTART_BROKEN_SESH 32U

#define LIBRESTEAM_NET_IDENT_TYPE_INVALID 0U
#define LIBRESTEAM_NET_IDENT_TYPE_IP_ADDR 1U
#define LIBRESTEAM_NET_IDENT_TYPE_GENERIC_STR 2U
#define LIBRESTEAM_NET_IDENT_TYPE_GENERIC_BYTES 3U
#define LIBRESTEAM_NET_IDENT_TYPE_UNKNOWN 4U
#define LIBRESTEAM_NET_IDENT_TYPE_STEAM_ID 16U
#define LIBRESTEAM_NET_IDENT_TYPE_XBOX_PAIRWISE_ID 17U
#define LIBRESTEAM_NET_IDENT_TYPE_SONY_PSN 18U

typedef struct libresteam_net_ip_addr
{
    uint8_t ipv6[LIBRESTEAM_NET_IPV6_SIZE];
    uint16_t port;
} LDG_PACKED libresteam_net_ip_addr_t;

typedef struct libresteam_net_ident
{
    uint32_t type;
    uint32_t size;
    uint8_t data[LIBRESTEAM_NET_IDENT_DATA_SIZE];
} LDG_PACKED libresteam_net_ident_t;

typedef struct libresteam_net_conn_info
{
    libresteam_net_ident_t ident_remote;
    uint64_t user_data;
    uint32_t listen_sock;
    libresteam_net_ip_addr_t addr_remote;
    uint8_t pudding0[2];
    uint32_t pop_remote;
    uint32_t pop_relay;
    uint32_t state;
    uint32_t end_reason;
    char end_debug[LIBRESTEAM_NET_CONN_CLOSE_REASON_MAX];
    char conn_desc[LIBRESTEAM_NET_CONN_DESC_MAX];
    uint32_t flags;
    uint32_t reserved[LIBRESTEAM_NET_CONN_INFO_RESERVED_CUNT];
} LDG_PACKED libresteam_net_conn_info_t;

typedef struct libresteam_net_real_time_status
{
    uint32_t state;
    uint32_t ping;
    libresteam_fp32_t quality_local;
    libresteam_fp32_t quality_remote;
    libresteam_fp32_t out_packets_per_sec;
    libresteam_fp32_t out_bytes_per_sec;
    libresteam_fp32_t in_packets_per_sec;
    libresteam_fp32_t in_bytes_per_sec;
    uint32_t send_rate_bytes_per_sec;
    uint32_t pending_unreliable;
    uint32_t pending_reliable;
    uint32_t sent_unacked_reliable;
    uint64_t queue_time_usec;
    uint32_t max_jitter_usec;
    uint32_t reserved[LIBRESTEAM_NET_REAL_TIME_RESERVED_CUNT];
} LDG_PACKED libresteam_net_real_time_status_t;

typedef struct libresteam_net_lane_status
{
    uint32_t pending_unreliable;
    uint32_t pending_reliable;
    uint32_t sent_unacked_reliable;
    uint8_t pudding0[4];
    uint64_t queue_time_usec;
    uint32_t reserved[LIBRESTEAM_NET_LANE_RESERVED_CUNT];
} LDG_PACKED libresteam_net_lane_status_t;

typedef struct libresteam_net_msg
{
    void *data;
    uint32_t size;
    uint32_t conn;
    libresteam_net_ident_t ident_peer;
    uint64_t conn_user_data;
    uint64_t time_received_usec;
    uint64_t msg_num;
    void (*data_free_cb)(struct libresteam_net_msg *msg);
    void (*release_cb)(struct libresteam_net_msg *msg);
    uint32_t channel;
    uint32_t flags;
    uint64_t user_data;
    uint16_t lane_idx;
    uint8_t pudding[6];
} LDG_PACKED libresteam_net_msg_t;

typedef struct libresteam_net_ping_location
{
    uint8_t data[LIBRESTEAM_NET_PING_LOCATION_SIZE];
} libresteam_net_ping_location_t;

typedef struct libresteam_net_conf_val
{
    uint32_t val;
    uint32_t data_type;
    uint64_t data;
} LDG_PACKED libresteam_net_conf_val_t;

typedef struct libresteam_net_avail_status
{
    uint32_t avail;
    char debug_msg[LIBRESTEAM_NET_DEBUG_MSG_MAX];
} LDG_PACKED libresteam_net_avail_status_t;

typedef struct libresteam_net_relay_status
{
    uint32_t avail;
    uint32_t ping_measurement_in_progress;
    uint32_t avail_net_conf;
    uint32_t avail_any_relay;
    char debug_msg[LIBRESTEAM_NET_DEBUG_MSG_MAX];
} LDG_PACKED libresteam_net_relay_status_t;

typedef struct libresteam_net_fake_ip_res
{
    uint32_t res;
    libresteam_net_ident_t ident;
    uint32_t ip;
    uint16_t port[LIBRESTEAM_NET_FAKE_IP_PORT_MAX];
} LDG_PACKED libresteam_net_fake_ip_res_t;

#endif
