#ifndef LIBRESTEAM_NET_MSGS_H
#define LIBRESTEAM_NET_MSGS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/net/types.h>
#include <libresteam/net_msgs/ops.h>

LDG_EXPORT uint32_t libresteam_net_msgs_msg_send(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t remote_channel, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_net_msgs_channel_msgs_recv(const libresteam_backend_set_t *set, uint32_t local_channel, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_net_msgs_sesh_accept(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote);
LDG_EXPORT uint32_t libresteam_net_msgs_sesh_close(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote);
LDG_EXPORT uint32_t libresteam_net_msgs_channel_close(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, uint32_t local_channel);
LDG_EXPORT uint32_t libresteam_net_msgs_sesh_conn_info_get(const libresteam_backend_set_t *set, const libresteam_net_ident_t *remote, libresteam_net_conn_info_t *info, libresteam_net_real_time_status_t *status, uint32_t *state_out);

#endif
