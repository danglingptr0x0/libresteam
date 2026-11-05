#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/logging/logging.h>

#include <libresteam/backend/backend.h>
#include <libresteam/cloud/cloud.h>
#include <libresteam/cloud/ops.h>

static uint32_t cloud_primary_ops_get(const libresteam_backend_set_t *set, const libresteam_cloud_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->cloud) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->cloud;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

static uint32_t cloud_tee_ops_get(const libresteam_backend_set_t *set, uint32_t idx, const libresteam_cloud_ops_t **ops_out, void **ctx_out)
{
    *ops_out = set->backend[idx].ops->cloud;
    *ctx_out = set->backend[idx].ctx;

    if (!*ops_out) { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_cloud_file_rd(const libresteam_backend_set_t *set, const char *name, void *buff, uint32_t cap, uint32_t *out_len)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out_len)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out_len = 0;

    if (LDG_UNLIKELY(!set || !name || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_rd) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_rd(ctx, name, buff, cap, out_len);
}

uint32_t libresteam_cloud_file_exists_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_exists_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_exists_get(ctx, name, out);
}

uint32_t libresteam_cloud_file_persisted_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_persisted_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_persisted_get(ctx, name, out);
}

uint32_t libresteam_cloud_file_size_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_size_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_size_get(ctx, name, out);
}

uint32_t libresteam_cloud_file_timestamp_get(const libresteam_backend_set_t *set, const char *name, uint64_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_timestamp_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_timestamp_get(ctx, name, out);
}

uint32_t libresteam_cloud_file_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_cunt_get(ctx, out);
}

uint32_t libresteam_cloud_file_name_size_get(const libresteam_backend_set_t *set, uint32_t idx, const char **name_out, uint32_t *size_out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name_out || !size_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *name_out = 0x0;
    *size_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->file_name_size_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->file_name_size_get(ctx, idx, name_out, size_out);
}

uint32_t libresteam_cloud_quota_get(const libresteam_backend_set_t *set, uint64_t *total_out, uint64_t *avail_out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!total_out || !avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *total_out = 0;
    *avail_out = 0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->quota_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->quota_get(ctx, total_out, avail_out);
}

uint32_t libresteam_cloud_account_en_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->account_en_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->account_en_get(ctx, out);
}

uint32_t libresteam_cloud_app_en_get(const libresteam_backend_set_t *set, uint8_t *out)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cloud_primary_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_en_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_en_get(ctx, out);
}

uint32_t libresteam_cloud_file_wr(const libresteam_backend_set_t *set, const char *name, const void *data, uint32_t len)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = cloud_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->file_wr) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->file_wr(ctx, name, data, len); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "cloud: tee backend file_wr failed"); }
    }

    return primary_res;
}

uint32_t libresteam_cloud_file_forget(const libresteam_backend_set_t *set, const char *name)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = cloud_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->file_forget) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->file_forget(ctx, name); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "cloud: tee backend file_forget failed"); }
    }

    return primary_res;
}

uint32_t libresteam_cloud_file_delete(const libresteam_backend_set_t *set, const char *name)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = cloud_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->file_delete) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->file_delete(ctx, name); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "cloud: tee backend file_delete failed"); }
    }

    return primary_res;
}

uint32_t libresteam_cloud_app_en_set(const libresteam_backend_set_t *set, uint8_t en)
{
    const libresteam_cloud_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t primary_res = LDG_ERR_NOT_INIT;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend)) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < set->backend_cunt; idx++)
    {
        res = cloud_tee_ops_get(set, idx, &ops, &ctx);
        if (res == LDG_ERR_AOK && !ops->app_en_set) { res = LDG_ERR_UNSUPPORTED; }

        if (res == LDG_ERR_AOK) { res = ops->app_en_set(ctx, en); }

        if (idx == set->primary_idx) { primary_res = res; }
        else if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_UNSUPPORTED)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "cloud: tee backend app_en_set failed"); }
    }

    return primary_res;
}
