#ifndef LIBRESTEAM_NET_H
#define LIBRESTEAM_NET_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/net/types.h>

LDG_EXPORT uint32_t libresteam_net_msg_release(libresteam_net_msg_t *msg);

#endif
