#ifndef LIBRESTEAM_BACKEND_SRV_USER_H
#define LIBRESTEAM_BACKEND_SRV_USER_H

#include <stdint.h>

#include <libresteam/backend/srv/srv.h>

uint32_t libresteam_backend_srv_user_ops_fill(libresteam_backend_srv_t *srv);
uint32_t libresteam_backend_srv_user_stats_ops_fill(libresteam_backend_srv_t *srv);

#endif
