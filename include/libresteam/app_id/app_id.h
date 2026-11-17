#ifndef LIBRESTEAM_APP_ID_H
#define LIBRESTEAM_APP_ID_H

#include <stdint.h>
#include <dangling/core/macros.h>

#define LIBRESTEAM_APP_ID_NONE 0U
#define LIBRESTEAM_APP_ID_FILE_NAME "steam_appid.txt"

LDG_EXPORT uint32_t libresteam_app_id_resolve(uint32_t conf_app_id, uint32_t *out);

#endif
