#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/secure.h>

#include <libresteam/cb/cb.h>
#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_CB_LOG_LVL LDG_LOGGING_LVL_INFO
#define LIBRESTEAM_SHIM_CALL_COMPLETED_HANDLE_OFF 0U
#define LIBRESTEAM_SHIM_CALL_COMPLETED_CB_ID_OFF 8U
#define LIBRESTEAM_SHIM_CALL_COMPLETED_SIZE_OFF 12U
#define LIBRESTEAM_SHIM_CALL_COMPLETED_SIZE 16U
#define LIBRESTEAM_SHIM_CB_RUN_CALL_ARG_CUNT 3U

static uint32_t libresteam_shim_cb_hdr_get(void *obj, uint8_t *flags_out, uint32_t *cb_id_out)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!flags_out || !cb_id_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *flags_out = 0;
    *cb_id_out = 0;

    if (LDG_UNLIKELY(!obj)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_cpy(flags_out, (const uint8_t *)obj + LIBRESTEAM_SHIM_CB_FLAGS_OFF, (uint64_t)sizeof(*flags_out));
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_mem_secure_cpy(cb_id_out, (const uint8_t *)obj + LIBRESTEAM_SHIM_CB_ID_OFF, (uint64_t)sizeof(*cb_id_out));
}

uint32_t libresteam_shim_cb_reg(libresteam_shim_ctx_t *ctx, void *obj, uint32_t cb_id)
{
    uint8_t flags = 0;
    uint32_t old_id = 0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !obj)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_cb_hdr_get(obj, &flags, &old_id);
    if (res != LDG_ERR_AOK) { return res; }

    flags = (uint8_t)(flags | LIBRESTEAM_SHIM_CB_FLAG_REG);

    res = ldg_mem_secure_cpy((uint8_t *)obj + LIBRESTEAM_SHIM_CB_FLAGS_OFF, &flags, (uint64_t)sizeof(flags));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy((uint8_t *)obj + LIBRESTEAM_SHIM_CB_ID_OFF, &cb_id, (uint64_t)sizeof(cb_id));
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < ctx->cb_cunt; idx++) { if (ctx->cb_obj[idx] == obj) { return LDG_ERR_AOK; } }

    if (LDG_UNLIKELY(ctx->cb_cunt >= LIBRESTEAM_SHIM_CB_CAP)) { return LDG_ERR_FULL; }

    ctx->cb_obj[ctx->cb_cunt] = obj;
    ctx->cb_cunt++;

    return ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "cb reg; cb_id: %u32; flags: %u32; cunt: %u32", cb_id, (uint32_t)flags, ctx->cb_cunt);
}

uint32_t libresteam_shim_cb_unreg(libresteam_shim_ctx_t *ctx, void *obj)
{
    uint8_t flags = 0;
    uint32_t cb_id = 0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !obj)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_cb_hdr_get(obj, &flags, &cb_id);
    if (res != LDG_ERR_AOK) { return res; }

    flags = (uint8_t)(flags & (uint8_t) ~LIBRESTEAM_SHIM_CB_FLAG_REG);

    res = ldg_mem_secure_cpy((uint8_t *)obj + LIBRESTEAM_SHIM_CB_FLAGS_OFF, &flags, (uint64_t)sizeof(flags));
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < ctx->cb_cunt; idx++)
    {
        if (ctx->cb_obj[idx] != obj) { continue; }

        ctx->cb_cunt--;
        ctx->cb_obj[idx] = ctx->cb_obj[ctx->cb_cunt];
        ctx->cb_obj[ctx->cb_cunt] = 0x0;
        return LDG_ERR_AOK;
    }

    return LDG_ERR_NOT_FOUND;
}

uint32_t libresteam_shim_call_res_reg(libresteam_shim_ctx_t *ctx, void *obj, uint64_t call)
{
    uint32_t idx = 0;

    if (LDG_UNLIKELY(!ctx || !obj)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (idx = 0; idx < ctx->call_cunt; idx++)
    {
        if (ctx->call_obj[idx] != obj) { continue; }

        ctx->call_handle[idx] = call;
        return LDG_ERR_AOK;
    }

    if (LDG_UNLIKELY(ctx->call_cunt >= LIBRESTEAM_SHIM_CALL_RES_CAP)) { return LDG_ERR_FULL; }

    ctx->call_obj[ctx->call_cunt] = obj;
    ctx->call_handle[ctx->call_cunt] = call;
    ctx->call_cunt++;

    return ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "call res reg; call: %u64; cunt: %u32", call, ctx->call_cunt);
}

uint32_t libresteam_shim_call_res_unreg(libresteam_shim_ctx_t *ctx, void *obj, uint64_t call)
{
    uint32_t idx = 0;

    if (LDG_UNLIKELY(!ctx || !obj)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (idx = 0; idx < ctx->call_cunt; idx++)
    {
        if (ctx->call_obj[idx] != obj || ctx->call_handle[idx] != call) { continue; }

        ctx->call_cunt--;
        ctx->call_obj[idx] = ctx->call_obj[ctx->call_cunt];
        ctx->call_handle[idx] = ctx->call_handle[ctx->call_cunt];
        ctx->call_obj[ctx->call_cunt] = 0x0;
        ctx->call_handle[ctx->call_cunt] = 0;
        return LDG_ERR_AOK;
    }

    return LDG_ERR_NOT_FOUND;
}

static uint32_t libresteam_shim_call_res_dispatch(libresteam_shim_ctx_t *ctx, const uint8_t *param, uint32_t param_size)
{
    void *obj = 0x0;
    uint64_t call = 0;
    uint32_t cb_id = 0;
    uint32_t size = 0;
    uint32_t slot = 0;
    uint32_t idx = 0;
    uint8_t failed = LDG_TRUTH_TRUE;
    uint32_t res = LDG_ERR_AOK;
    uint32_t log_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(param_size < LIBRESTEAM_SHIM_CALL_COMPLETED_SIZE)) { return LDG_ERR_RANGE; }

    res = ldg_mem_secure_cpy(&call, param + LIBRESTEAM_SHIM_CALL_COMPLETED_HANDLE_OFF, (uint64_t)sizeof(call));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(&cb_id, param + LIBRESTEAM_SHIM_CALL_COMPLETED_CB_ID_OFF, (uint64_t)sizeof(cb_id));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(&size, param + LIBRESTEAM_SHIM_CALL_COMPLETED_SIZE_OFF, (uint64_t)sizeof(size));
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < ctx->call_cunt; idx++) { if (ctx->call_handle[idx] == call) { obj = ctx->call_obj[idx]; break; } }
    if (!obj) { return LDG_ERR_NOT_FOUND; }

    res = libresteam_shim_call_res_unreg(ctx, obj, call);
    if (res != LDG_ERR_AOK) { return res; }

    if (LDG_UNLIKELY(size > LIBRESTEAM_SHIM_CALL_RES_BUFF_MAX)) { return LDG_ERR_OVERFLOW; }

    res = libresteam_cb_call_res_get(&ctx->set, call, ctx->call_res_buff, size, cb_id, &failed);
    if (res != LDG_ERR_AOK) { failed = LDG_TRUTH_TRUE; }

    res = ldg_cpp_slot_get(&ctx->cb_desc, LIBRESTEAM_SHIM_CB_RUN_CALL, &slot);
    if (res != LDG_ERR_AOK)
    {
        log_res = ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "call res slot failed; res: %u32; call: %u64; cb_id: %u32", res, call, cb_id);
        if (log_res != LDG_ERR_AOK) { return log_res; }

        return res;
    }

    res = ldg_cpp_void_call(obj, slot, (ldg_cpp_arg_t[]) { { .ptr = ctx->call_res_buff, .kind = LDG_CPP_ARG_KIND_PTR }, { .val = failed, .kind = LDG_CPP_ARG_KIND_INT }, { .val = call, .kind = LDG_CPP_ARG_KIND_INT } }, LIBRESTEAM_SHIM_CB_RUN_CALL_ARG_CUNT);
    if (res != LDG_ERR_AOK)
    {
        log_res = ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "call res run failed; res: %u32; call: %u64; cb_id: %u32; slot: %u32; obj: %p", res, call, cb_id, slot, obj);
        if (log_res != LDG_ERR_AOK) { return log_res; }

        return res;
    }

    return ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "call res run; call: %u64; cb_id: %u32; size: %u32; failed: %u32", call, cb_id, size, (uint32_t)failed);
}

static void libresteam_shim_cb_msg_cb(void *content, uint32_t user, uint32_t cb_id, const uint8_t *param, uint32_t param_size)
{
    libresteam_shim_ctx_t *ctx = (libresteam_shim_ctx_t *)content;
    uint8_t flags = 0;
    uint32_t obj_id = 0;
    uint32_t slot = 0;
    uint32_t idx = 0;
    uint32_t delivered = 0;
    uint32_t res = LDG_ERR_AOK;

    if (!ctx || !param) { return; }

    if (cb_id == LIBRESTEAM_SHIM_CB_CALL_COMPLETED)
    {
        res = LDG_ERR_EMPTY;
        if (ctx->call_cunt != 0) { res = libresteam_shim_call_res_dispatch(ctx, param, param_size); }

        if (res == LDG_ERR_AOK) { delivered++; }
        else if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_CB_LOG_LVL, "call completed undelivered; res: %u32; size: %u32; pending: %u32", res, param_size, ctx->call_cunt) != LDG_ERR_AOK) { return; }
    }

    res = ldg_cpp_slot_get(&ctx->cb_desc, LIBRESTEAM_SHIM_CB_RUN, &slot);
    if (res != LDG_ERR_AOK) { return; }

    for (idx = 0; idx < ctx->cb_cunt; idx++)
    {
        res = libresteam_shim_cb_hdr_get(ctx->cb_obj[idx], &flags, &obj_id);
        if (res != LDG_ERR_AOK) { continue; }

        if (obj_id != cb_id) { continue; }

        if ((flags & LIBRESTEAM_SHIM_CB_FLAG_REG) != LIBRESTEAM_SHIM_CB_FLAG_REG) { continue; }

        if ((flags & LIBRESTEAM_SHIM_CB_FLAG_GAME_SRV) == LIBRESTEAM_SHIM_CB_FLAG_GAME_SRV) { continue; }

        res = ldg_cpp_void_call(ctx->cb_obj[idx], slot, (ldg_cpp_arg_t[]) { { .ptr = param, .kind = LDG_CPP_ARG_KIND_PTR } }, LIBRESTEAM_SHIM_CB_RUN_ARG_CUNT);
        if (res == LDG_ERR_AOK) { delivered++; }
    }

    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LDG_LOGGING_LVL_DEBUG, "cb; user: %u32; cb_id: %u32; size: %u32; delivered: %u32", user, cb_id, param_size, delivered) != LDG_ERR_AOK) { return; }
}

uint32_t libresteam_shim_cb_run(libresteam_shim_ctx_t *ctx)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(ctx->steam_up != LDG_TRUTH_TRUE)) { return LDG_ERR_NOT_INIT; }

    ctx->sink_ops.msg_cb = libresteam_shim_cb_msg_cb;
    ctx->sink.ops = &ctx->sink_ops;
    ctx->sink.content = ctx;

    return libresteam_cb_pump(&ctx->set, &ctx->sink);
}
