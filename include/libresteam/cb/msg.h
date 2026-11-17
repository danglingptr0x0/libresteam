#ifndef LIBRESTEAM_CB_MSG_H
#define LIBRESTEAM_CB_MSG_H

#include <stdint.h>
#include <dangling/core/macros.h>

#define LIBRESTEAM_CB_MSG_SIZE 24U

typedef struct libresteam_cb_msg
{
    uint32_t user;
    uint32_t cb_id;
    uint8_t *param;
    uint32_t param_size;
    uint8_t pudding[4];
} LDG_PACKED libresteam_cb_msg_t;

#endif
