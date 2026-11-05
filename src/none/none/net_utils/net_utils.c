#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>
#include <libresteam/net_utils/net_utils.h>
#include <libresteam/net_utils/ops.h>

static uint32_t net_utils_ops_get(const libresteam_backend_set_t *set, const libresteam_net_utils_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->net_utils) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->net_utils;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_net_utils_msg_alloc(const libresteam_backend_set_t *set, uint32_t size, libresteam_net_msg_t **out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->msg_alloc) { return LDG_ERR_UNSUPPORTED; }

    return ops->msg_alloc(ctx, size, out);
}

uint32_t libresteam_net_utils_relay_status_get(const libresteam_backend_set_t *set, libresteam_net_relay_status_t *details, uint32_t *avail_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *avail_out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->relay_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->relay_status_get(ctx, details, avail_out);
}

uint32_t libresteam_net_utils_local_ping_location_get(const libresteam_backend_set_t *set, libresteam_net_ping_location_t *out, libresteam_fp32_t *age_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out || !age_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->local_ping_location_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->local_ping_location_get(ctx, out, age_out);
}

uint32_t libresteam_net_utils_ping_between_estimate(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *lhs, const libresteam_net_ping_location_t *rhs, uint32_t *ms_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    if (LDG_UNLIKELY(!lhs || !rhs)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_between_estimate) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_between_estimate(ctx, lhs, rhs, ms_out);
}

uint32_t libresteam_net_utils_ping_from_local_estimate(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *remote, uint32_t *ms_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_from_local_estimate) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_from_local_estimate(ctx, remote, ms_out);
}

uint32_t libresteam_net_utils_ping_location_to_str(const libresteam_backend_set_t *set, const libresteam_net_ping_location_t *location, char *buff, uint32_t cap)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!location || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_location_to_str) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_location_to_str(ctx, location, buff, cap);
}

uint32_t libresteam_net_utils_ping_location_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ping_location_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_location_parse) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_location_parse(ctx, str, out);
}

uint32_t libresteam_net_utils_ping_data_up_to_date_get(const libresteam_backend_set_t *set, libresteam_fp32_t max_age_sec, uint8_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_data_up_to_date_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_data_up_to_date_get(ctx, max_age_sec, out);
}

uint32_t libresteam_net_utils_ping_to_data_center_get(const libresteam_backend_set_t *set, uint32_t pop, uint32_t *via_relay_pop_out, uint32_t *ms_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!via_relay_pop_out || !ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *via_relay_pop_out = 0;
    *ms_out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ping_to_data_center_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ping_to_data_center_get(ctx, pop, via_relay_pop_out, ms_out);
}

uint32_t libresteam_net_utils_direct_ping_to_pop_get(const libresteam_backend_set_t *set, uint32_t pop, uint32_t *ms_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->direct_ping_to_pop_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->direct_ping_to_pop_get(ctx, pop, ms_out);
}

uint32_t libresteam_net_utils_pop_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->pop_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->pop_cunt_get(ctx, out);
}

uint32_t libresteam_net_utils_pop_list_get(const libresteam_backend_set_t *set, uint32_t *list, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!list)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->pop_list_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->pop_list_get(ctx, list, cap, cunt_out);
}

uint32_t libresteam_net_utils_local_timestamp_get(const libresteam_backend_set_t *set, uint64_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->local_timestamp_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->local_timestamp_get(ctx, out);
}

uint32_t libresteam_net_utils_debug_output_set(const libresteam_backend_set_t *set, uint32_t detail_lvl, libresteam_net_utils_debug_cb_t cb)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->debug_output_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->debug_output_set(ctx, detail_lvl, cb);
}

uint32_t libresteam_net_utils_ipv4_fake_ip_type_get(const libresteam_backend_set_t *set, uint32_t ipv4, uint32_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ipv4_fake_ip_type_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ipv4_fake_ip_type_get(ctx, ipv4, out);
}

uint32_t libresteam_net_utils_real_ident_for_fake_ip_get(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *fake_ip, libresteam_net_ident_t *out, uint32_t *eres_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!fake_ip || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->real_ident_for_fake_ip_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->real_ident_for_fake_ip_get(ctx, fake_ip, out, eres_out);
}

uint32_t libresteam_net_utils_conf_val_set(const libresteam_backend_set_t *set, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conf_val_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->conf_val_set(ctx, val, scope, scope_obj, data_type, arg);
}

uint32_t libresteam_net_utils_conf_val_get(const libresteam_backend_set_t *set, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t *data_type_out, void *res, uint64_t *res_size, uint32_t *get_res_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!get_res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *get_res_out = 0;

    if (LDG_UNLIKELY(!res_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = net_utils_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->conf_val_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conf_val_get(ctx, val, scope, scope_obj, data_type_out, res, res_size, get_res_out);
}

uint32_t libresteam_net_utils_conf_val_info_get(const libresteam_backend_set_t *set, uint32_t val, uint32_t *data_type_out, uint32_t *scope_out, const char **name_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *name_out = 0x0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->conf_val_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->conf_val_info_get(ctx, val, data_type_out, scope_out, name_out);
}

uint32_t libresteam_net_utils_editable_conf_val_iterate(const libresteam_backend_set_t *set, uint32_t cur, uint8_t dev_vars_en, uint32_t *next_out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!next_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *next_out = 0;

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->editable_conf_val_iterate) { return LDG_ERR_UNSUPPORTED; }

    return ops->editable_conf_val_iterate(ctx, cur, dev_vars_en, next_out);
}

uint32_t libresteam_net_utils_ip_addr_to_str(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, char *buff, uint64_t cap, uint8_t port_en)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!addr || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ip_addr_to_str) { return LDG_ERR_UNSUPPORTED; }

    return ops->ip_addr_to_str(ctx, addr, buff, cap, port_en);
}

uint32_t libresteam_net_utils_ip_addr_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ip_addr_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ip_addr_parse) { return LDG_ERR_UNSUPPORTED; }

    return ops->ip_addr_parse(ctx, str, out);
}

uint32_t libresteam_net_utils_ip_addr_fake_ip_type_get(const libresteam_backend_set_t *set, const libresteam_net_ip_addr_t *addr, uint32_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!addr)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ip_addr_fake_ip_type_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->ip_addr_fake_ip_type_get(ctx, addr, out);
}

uint32_t libresteam_net_utils_ident_to_str(const libresteam_backend_set_t *set, const libresteam_net_ident_t *ident, char *buff, uint64_t cap)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ident || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ident_to_str) { return LDG_ERR_UNSUPPORTED; }

    return ops->ident_to_str(ctx, ident, buff, cap);
}

uint32_t libresteam_net_utils_ident_parse(const libresteam_backend_set_t *set, const char *str, libresteam_net_ident_t *out)
{
    const libresteam_net_utils_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = net_utils_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ident_parse) { return LDG_ERR_UNSUPPORTED; }

    return ops->ident_parse(ctx, str, out);
}
