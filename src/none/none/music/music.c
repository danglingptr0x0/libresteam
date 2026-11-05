#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/music/music.h>
#include <libresteam/music/ops.h>

static uint32_t music_ops_get(const libresteam_backend_set_t *set, const libresteam_music_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->music) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->music;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_music_en_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->en_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->en_get(ctx, out);
}

uint32_t libresteam_music_playing_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->playing_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->playing_get(ctx, out);
}

uint32_t libresteam_music_status_get(const libresteam_backend_set_t *set, libresteam_music_status_t *out)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->status_get(ctx, out);
}

uint32_t libresteam_music_play(const libresteam_backend_set_t *set)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->play) { return LDG_ERR_UNSUPPORTED; }

    return ops->play(ctx);
}

uint32_t libresteam_music_pause(const libresteam_backend_set_t *set)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->pause) { return LDG_ERR_UNSUPPORTED; }

    return ops->pause(ctx);
}

uint32_t libresteam_music_prev_play(const libresteam_backend_set_t *set)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->prev_play) { return LDG_ERR_UNSUPPORTED; }

    return ops->prev_play(ctx);
}

uint32_t libresteam_music_next_play(const libresteam_backend_set_t *set)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->next_play) { return LDG_ERR_UNSUPPORTED; }

    return ops->next_play(ctx);
}

uint32_t libresteam_music_vol_set(const libresteam_backend_set_t *set, libresteam_fp32_t vol)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vol_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->vol_set(ctx, vol);
}

uint32_t libresteam_music_vol_get(const libresteam_backend_set_t *set, libresteam_fp32_t *out)
{
    const libresteam_music_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = music_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->vol_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->vol_get(ctx, out);
}
