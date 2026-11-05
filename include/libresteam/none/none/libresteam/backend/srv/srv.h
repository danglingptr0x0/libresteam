#ifndef LIBRESTEAM_BACKEND_SRV_H
#define LIBRESTEAM_BACKEND_SRV_H

#include <stdint.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/net/sock.h>
#include <dangling/proto/emiemi.h>

#include <libresteam/backend/backend.h>
#include <libresteam/cb/ops.h>
#include <libresteam/cloud/ops.h>
#include <libresteam/friends/ops.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/ops.h>
#include <libresteam/user_stats/ops.h>
#include <libresteam/wire/wire.h>

#define LIBRESTEAM_BACKEND_SRV_HOST_LOOPBACK_V4 "127.0.0.1"
#define LIBRESTEAM_BACKEND_SRV_HOST_LOOPBACK_V6 "::1"
#define LIBRESTEAM_BACKEND_SRV_HOST_MAX 64U
#define LIBRESTEAM_BACKEND_SRV_STR_MAX 4096U
#define LIBRESTEAM_BACKEND_SRV_USER_NONE 0U
#define LIBRESTEAM_BACKEND_SRV_PORT_DEFAULT 0U

typedef struct libresteam_backend_srv_conf
{
    const char *host;
    const char *account_name;
    uint64_t timeout;
    uint32_t app_id;
    uint32_t push_slot_cunt;
    uint32_t push_slot_size;
    uint16_t port;
    uint8_t pudding[2];
} libresteam_backend_srv_conf_t;

typedef struct libresteam_backend_srv
{
    libresteam_backend_ops_t ops;
    libresteam_user_stats_ops_t user_stats_ops;
    libresteam_friends_ops_t friends_ops;
    libresteam_cloud_ops_t cloud_ops;
    libresteam_cb_ops_t cb_ops;
    libresteam_user_ops_t user_ops;
    ldg_net_sock_t sock;
    ldg_emiemi_io_ctx_t io;
    ldg_mem_pool_t *pool;
    uint8_t *frame;
    uint8_t *push_buff;
    uint32_t *push_len;
    uint32_t *push_cb_id;
    char *str;
    uint8_t *deliver;
    libresteam_steam_id_t steam_id;
    uint64_t timeout;
    uint64_t deadline_ns;
    uint32_t frame_cap;
    uint32_t seq;
    uint32_t push_slot_cunt;
    uint32_t push_slot_size;
    uint32_t push_hd;
    uint32_t push_cunt;
    uint32_t app_id;
    uint8_t sock_up;
    uint8_t pumping;
    uint8_t pudding[2];
} libresteam_backend_srv_t;

LDG_EXPORT uint32_t libresteam_backend_srv_init(libresteam_backend_srv_t *srv, const libresteam_backend_srv_conf_t *conf, libresteam_backend_t *out);
LDG_EXPORT uint32_t libresteam_backend_srv_shutdown(libresteam_backend_srv_t *srv);
LDG_EXPORT uint32_t libresteam_backend_srv_steam_id_get(const libresteam_backend_srv_t *srv, libresteam_steam_id_t *out);

uint32_t libresteam_backend_srv_req_begin(libresteam_backend_srv_t *srv, libresteam_wire_wr_t *wr);
uint32_t libresteam_backend_srv_call(libresteam_backend_srv_t *srv, uint8_t service, uint8_t op, libresteam_wire_wr_t *wr, libresteam_wire_rd_t *rd_out);
uint32_t libresteam_backend_srv_user_stats_ops_fill(libresteam_backend_srv_t *srv);
uint32_t libresteam_backend_srv_friends_ops_fill(libresteam_backend_srv_t *srv);
uint32_t libresteam_backend_srv_cloud_ops_fill(libresteam_backend_srv_t *srv);
uint32_t libresteam_backend_srv_user_ops_fill(libresteam_backend_srv_t *srv);

#endif
