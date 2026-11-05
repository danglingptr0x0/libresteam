#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/cb/cb.h>
#include <libresteam/cb/ops.h>

static uint32_t cb_ops_get(const libresteam_backend_set_t *set, const libresteam_cb_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->cb) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->cb;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_cb_pump(const libresteam_backend_set_t *set, const libresteam_cb_sink_t *sink)
{
    const libresteam_cb_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set || !sink || !sink->ops || !sink->ops->msg_cb)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cb_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->pump) { return LDG_ERR_UNSUPPORTED; }

    return ops->pump(ctx, sink);
}

uint32_t libresteam_cb_call_res_get(const libresteam_backend_set_t *set, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out)
{
    const libresteam_cb_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!failed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *failed_out = LDG_TRUTH_TRUE;

    if (LDG_UNLIKELY(!set || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cb_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->call_res_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->call_res_get(ctx, call, buff, cap, cb_id_expected, failed_out);
}

uint32_t libresteam_cb_next_get(const libresteam_backend_set_t *set, libresteam_cb_msg_t *out)
{
    const libresteam_cb_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = (libresteam_cb_msg_t)LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cb_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->next_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->next_get(ctx, out);
}

uint32_t libresteam_cb_last_free(const libresteam_backend_set_t *set)
{
    const libresteam_cb_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cb_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->last_free) { return LDG_ERR_UNSUPPORTED; }

    return ops->last_free(ctx);
}
