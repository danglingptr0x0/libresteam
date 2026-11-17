#ifndef LIBRESTEAM_NET_MSGS_OPS_H
#define LIBRESTEAM_NET_MSGS_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/net/types.h>

typedef struct libresteam_net_msgs_ops
{
    uint32_t (*msg_send)(void *ctx, const libresteam_net_ident_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t remote_channel, uint32_t *eres_out);
    uint32_t (*channel_msgs_recv)(void *ctx, uint32_t local_channel, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*sesh_accept)(void *ctx, const libresteam_net_ident_t *remote);
    uint32_t (*sesh_close)(void *ctx, const libresteam_net_ident_t *remote);
    uint32_t (*channel_close)(void *ctx, const libresteam_net_ident_t *remote, uint32_t local_channel);
    uint32_t (*sesh_conn_info_get)(void *ctx, const libresteam_net_ident_t *remote, libresteam_net_conn_info_t *info, libresteam_net_real_time_status_t *status, uint32_t *state_out);
} libresteam_net_msgs_ops_t;

#endif
