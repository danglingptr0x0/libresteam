#ifndef LIBRESTEAM_SRV_USER_H
#define LIBRESTEAM_SRV_USER_H

#include <stdint.h>

#include <libresteam/srv/srv.h>
#include <libresteam/wire/wire.h>

uint32_t libresteam_srv_user_stats_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr);

#endif
