#ifndef LIBRESTEAM_STEAM_NET_H
#define LIBRESTEAM_STEAM_NET_H

#include <stdint.h>

#include <libresteam/steam/steam.h>

uint32_t libresteam_steam_net_utils_ops_fill(libresteam_steam_t *steam);
uint32_t libresteam_steam_net_socks_ops_fill(libresteam_steam_t *steam);
uint32_t libresteam_steam_net_msgs_ops_fill(libresteam_steam_t *steam);

#endif
