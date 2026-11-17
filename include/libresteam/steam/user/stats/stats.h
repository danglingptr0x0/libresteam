#ifndef LIBRESTEAM_STEAM_USER_STATS_H
#define LIBRESTEAM_STEAM_USER_STATS_H

#include <stdint.h>

#include <libresteam/steam/steam.h>

uint32_t libresteam_steam_user_stats_ops_fill(libresteam_steam_t *steam);
uint32_t libresteam_steam_user_stats_ext_ops_fill(libresteam_steam_t *steam);

#endif
