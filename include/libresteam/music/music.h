#ifndef LIBRESTEAM_MUSIC_H
#define LIBRESTEAM_MUSIC_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/music/ops.h>

LDG_EXPORT uint32_t libresteam_music_en_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_music_playing_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_music_status_get(const libresteam_backend_set_t *set, libresteam_music_status_t *out);
LDG_EXPORT uint32_t libresteam_music_play(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_music_pause(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_music_prev_play(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_music_next_play(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_music_vol_set(const libresteam_backend_set_t *set, libresteam_fp32_t vol);
LDG_EXPORT uint32_t libresteam_music_vol_get(const libresteam_backend_set_t *set, libresteam_fp32_t *out);

#endif
