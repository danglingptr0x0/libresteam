#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/music/ops.h>

#define STEAM_MUSIC_ARG_NONE 0U

static uint32_t steam_music_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->music)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->music_desc, method_idx, out);
}

static uint32_t steam_music_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->music, slot, arg, arg_cunt);
}

static uint32_t steam_music_en_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_EN_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_music_playing_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_PLAYING_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_music_status_get(void *ctx, libresteam_music_status_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t status = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_MUSIC_STATUS_UNDEFINED;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_STATUS_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE, &status);
    if (res != LDG_ERR_AOK) { return res; }

    if (status > (uint32_t)LIBRESTEAM_MUSIC_STATUS_IDLE) { return LDG_ERR_RANGE; }

    *out = (libresteam_music_status_t)status;

    return LDG_ERR_AOK;
}

static uint32_t steam_music_play(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_PLAY, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE);
}

static uint32_t steam_music_pause(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_PAUSE, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE);
}

static uint32_t steam_music_prev_play(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_PREV_PLAY, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE);
}

static uint32_t steam_music_next_play(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_NEXT_PLAY, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE);
}

static uint32_t steam_music_vol_set(void *ctx, libresteam_fp32_t vol)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = vol, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_music_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_MUSIC_VOL_SET, arg, cunt);
}

static uint32_t steam_music_vol_get(void *ctx, libresteam_fp32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = (libresteam_fp32_t)0.0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_music_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_MUSIC_VOL_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_fp32_call(steam->music, slot, 0x0, STEAM_MUSIC_ARG_NONE, out);
}

uint32_t libresteam_backend_steam_music_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->music_ops.en_get = steam_music_en_get;
    steam->music_ops.playing_get = steam_music_playing_get;
    steam->music_ops.status_get = steam_music_status_get;
    steam->music_ops.play = steam_music_play;
    steam->music_ops.pause = steam_music_pause;
    steam->music_ops.prev_play = steam_music_prev_play;
    steam->music_ops.next_play = steam_music_next_play;
    steam->music_ops.vol_set = steam_music_vol_set;
    steam->music_ops.vol_get = steam_music_vol_get;
    steam->ops.music = &steam->music_ops;

    return LDG_ERR_AOK;
}
