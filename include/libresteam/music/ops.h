#ifndef LIBRESTEAM_MUSIC_OPS_H
#define LIBRESTEAM_MUSIC_OPS_H

#include <stdint.h>

#include <libresteam/fp32/fp32.h>

typedef enum libresteam_music_status
{
    LIBRESTEAM_MUSIC_STATUS_UNDEFINED = 0,
    LIBRESTEAM_MUSIC_STATUS_PLAYING,
    LIBRESTEAM_MUSIC_STATUS_PAUSED,
    LIBRESTEAM_MUSIC_STATUS_IDLE
} libresteam_music_status_t;

typedef struct libresteam_music_ops
{
    uint32_t (*en_get)(void *ctx, uint8_t *out);
    uint32_t (*playing_get)(void *ctx, uint8_t *out);
    uint32_t (*status_get)(void *ctx, libresteam_music_status_t *out);
    uint32_t (*play)(void *ctx);
    uint32_t (*pause)(void *ctx);
    uint32_t (*prev_play)(void *ctx);
    uint32_t (*next_play)(void *ctx);
    uint32_t (*vol_set)(void *ctx, libresteam_fp32_t vol);
    uint32_t (*vol_get)(void *ctx, libresteam_fp32_t *out);
} libresteam_music_ops_t;

#endif
