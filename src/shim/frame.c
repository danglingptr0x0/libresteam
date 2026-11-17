#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/time/time.h>

#include <libresteam/shim/shim.h>

#define FRAME_INIT_RETRY_MS 2000U
#define FRAME_INIT_RETRY_NS ((uint64_t)FRAME_INIT_RETRY_MS * LDG_NS_PER_MS)

static uint32_t frame_il2cpp_tick(libresteam_shim_ctx_t *ctx)
{
    uint64_t now = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->il2cpp.ready || ctx->il2cpp.failed) { return LDG_ERR_AOK; }

    res = ldg_time_monotonic_get(&now);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (now < ctx->next_tick_ns) { return LDG_ERR_AOK; }

    if (LDG_UNLIKELY(now > UINT64_MAX - FRAME_INIT_RETRY_NS)) { return LDG_ERR_OVERFLOW; }

    if (ctx->next_tick_ns == 0) { ctx->next_tick_ns = now + FRAME_INIT_RETRY_NS; return LDG_ERR_AOK; }

    ctx->next_tick_ns = now + FRAME_INIT_RETRY_NS;
    res = libresteam_backends_il2cpp_init(&ctx->il2cpp);
    if (res == LDG_ERR_NOT_INIT || ctx->il2cpp.failed) { return LDG_ERR_AOK; }

    return res;
}

uint32_t libresteam_shim_frame_tick(libresteam_shim_ctx_t *ctx)
{
    uint32_t mod_res = LDG_ERR_AOK;
    uint32_t il2cpp_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (ctx->frame_active || !ctx->steam_up) { return LDG_ERR_AOK; }

    ctx->frame_active = LDG_TRUTH_TRUE;
    mod_res = libresteam_mod_tick(&ctx->mods);
    if (ctx->steam_up) { il2cpp_res = frame_il2cpp_tick(ctx); }

    ctx->frame_active = LDG_TRUTH_FALSE;
    if (LDG_UNLIKELY(mod_res != LDG_ERR_AOK)) { return mod_res; }

    return il2cpp_res;
}
