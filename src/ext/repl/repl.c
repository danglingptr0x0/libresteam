#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>

#include <libresteam/ext/repl/repl.h>

#define REPL_WORKERS 1U

static uint32_t repl_stop(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->pool_running) { return LDG_ERR_AOK; }

    res = libresteam_ext_repl_io_stop(ctx);
    if (res == LDG_ERR_AOK) { res = ldg_thread_pool_stop(&ctx->pool); }

    if (res == LDG_ERR_AOK) { ctx->pool_running = LDG_TRUTH_FALSE; }

    return res;
}

static void repl_cleanup_cb(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    const libresteam_mod_api_t *api = 0x0;
    void *mod = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return; }

    api = ctx->api;
    mod = ctx->mod;
    res = libresteam_ext_repl_trace_shutdown(ctx);
    if (res != LDG_ERR_AOK)
    {
        if (api->log(mod, "repl trace shutdown failed; context retained; res: %u32", res) != LDG_ERR_AOK) { return; }

        return;
    }

    res = repl_stop(ctx);
    if (res != LDG_ERR_AOK)
    {
        if (api->log(mod, "repl worker stop failed; context retained; res: %u32", res) != LDG_ERR_AOK) { return; }

        return;
    }

    if (ctx->pool_up) { res = ldg_thread_pool_shutdown(&ctx->pool); }

    res |= libresteam_ext_repl_io_shutdown(ctx);
    if (ctx->req_up) { res |= ldg_spsc_shutdown(&ctx->req_queue); }

    if (ctx->resp_up) { res |= ldg_spsc_shutdown(&ctx->resp_queue); }

    for (idx = 0; idx < ctx->backend_cunt; idx++) { if (ctx->backend[idx].shutdown) { res |= ctx->backend[idx].shutdown(ctx); } }
    res |= ldg_mem_dealloc(ctx);
    if (api->log(mod, "repl shutdown; res: %u32", res) != LDG_ERR_AOK) { return; }
}

static void repl_tick_cb(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    uint32_t thread = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return; }

    res = libresteam_ext_repl_thread_get(&thread);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return; }

    if (!ctx->main_thread) { ctx->main_thread = thread; }

    if (ctx->main_thread != thread) { return; }

    for (idx = 0; idx < ctx->backend_cunt; idx++) { if (ctx->backend[idx].tick) { ctx->backend[idx].last_res = ctx->backend[idx].tick(ctx); } }
    if (ctx->response_ready)
    {
        res = ldg_spsc_push(&ctx->resp_queue, &ctx->resp);
        if (res == LDG_ERR_AOK) { ctx->response_ready = LDG_TRUTH_FALSE; }

        return;
    }

    if (ctx->searching)
    {
        if (ctx->pending_backend >= ctx->backend_cunt || !ctx->backend[ctx->pending_backend].cont) { res = LDG_ERR_INVALID; }
        else { res = ctx->backend[ctx->pending_backend].cont(ctx); }

        if (res != LDG_ERR_AOK) { ctx->searching = LDG_TRUTH_FALSE; }
    }
    else
    {
        res = ldg_spsc_pop(&ctx->req_queue, &ctx->req);
        if (res == LDG_ERR_EMPTY) { return; }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return; }

        ctx->resp.id = ctx->req.id;
        ctx->resp.len = 0;
        ctx->resp.text[0] = LDG_STR_TERM;
        res = libresteam_ext_repl_cmd_run(ctx);
    }

    ctx->resp.res = res;
    if (!ctx->searching) { ctx->pending_backend = UINT32_MAX; ctx->response_ready = LDG_TRUTH_TRUE; }
}

static uint32_t repl_resources_init(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res = ldg_spsc_init(&ctx->req_queue, sizeof(ctx->req), LIBRESTEAM_EXT_REPL_QUEUE_CAP);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->req_up = LDG_TRUTH_TRUE;
    res = ldg_spsc_init(&ctx->resp_queue, sizeof(ctx->resp), LIBRESTEAM_EXT_REPL_QUEUE_CAP);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->resp_up = LDG_TRUTH_TRUE;
    res = ldg_thread_pool_init(&ctx->pool, REPL_WORKERS);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->pool_up = LDG_TRUTH_TRUE;
    res = libresteam_ext_repl_native_init(ctx);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_java_init(ctx); }

    if (res == LDG_ERR_AOK)
    {
        res = libresteam_ext_repl_trace_init(ctx);
        if (res != LDG_ERR_AOK) { res = ctx->api->log(ctx->mod, "repl trace unavailable; res: %u32", res); }
    }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_io_init(ctx); }

    return res;
}

uint32_t libresteam_ext_repl_init(const libresteam_mod_api_t *api, void *mod)
{
    libresteam_ext_repl_ctx_t *ctx = 0x0;
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!api || !mod)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (api->version != LIBRESTEAM_MOD_API_VERSION || api->size < sizeof(*api)) { return LDG_ERR_UNSUPPORTED; }

    if (!api->log || !api->tick_reg || !api->cleanup_reg) { return LDG_ERR_UNSUPPORTED; }

    res = ldg_mem_init();
    if (res == LDG_ERR_AOK) { res = ldg_mem_alloc(sizeof(*ctx), &mem); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx = (libresteam_ext_repl_ctx_t *)mem;
    res = ldg_mem_secure_zero(ctx, sizeof(*ctx));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        res = ldg_mem_dealloc(ctx);
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    ctx->api = api;
    ctx->mod = mod;
    res = libresteam_ext_repl_backend_init(ctx);
    if (res == LDG_ERR_AOK) { res = repl_resources_init(ctx); }

    if (res == LDG_ERR_AOK) { res = api->cleanup_reg(mod, repl_cleanup_cb, ctx); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        repl_cleanup_cb(ctx);
        return res;
    }

    res = api->tick_reg(mod, repl_tick_cb, ctx);
    if (res == LDG_ERR_AOK) { res = api->log(mod, "repl init; pipe: %s; backends: native, il2cpp, lua, java; commands run on callback thread", ctx->pipe_name); }

    if (res == LDG_ERR_AOK) { res = ldg_thread_pool_start(&ctx->pool, libresteam_ext_repl_io_worker_cb, ctx); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->pool_running = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}
