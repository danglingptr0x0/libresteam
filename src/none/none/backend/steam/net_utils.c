#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/mem/secure.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/net/types.h>
#include <libresteam/net_utils/ops.h>

#define STEAM_NET_UTILS_ARG_NONE 0U
#define STEAM_NET_UTILS_SIGN_BIT 0x80000000U
#define STEAM_NET_UTILS_PING_FAILED 0xFFFFFFFFU
#define STEAM_NET_UTILS_PING_UNKNOWN 0xFFFFFFFEU
#define STEAM_NET_UTILS_CONF_GET_BAD_VAL 0xFFFFFFFFU
#define STEAM_NET_UTILS_CONF_GET_BAD_SCOPE_OBJ 0xFFFFFFFEU
#define STEAM_NET_UTILS_CONF_GET_BUFF_TOO_SMALL 0xFFFFFFFDU

static uint32_t steam_net_utils_slot_get(void *ctx, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->net_utils)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->net_utils;

    return ldg_cpp_slot_get(&steam->net_utils_desc, method_idx, slot_out);
}

static uint32_t steam_net_utils_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_utils_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(obj, slot, arg, arg_cunt);
}

static uint32_t steam_net_utils_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    *out = LDG_TRUTH_FALSE;

    res = steam_net_utils_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_net_utils_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_net_utils_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_net_utils_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_net_utils_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_net_utils_ping_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *ms_out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_utils_u32_call(ctx, method_idx, arg, arg_cunt, ms_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*ms_out == STEAM_NET_UTILS_PING_UNKNOWN)
    {
        *ms_out = 0;
        return LDG_ERR_AGAIN;
    }

    if (*ms_out == STEAM_NET_UTILS_PING_FAILED || (*ms_out & STEAM_NET_UTILS_SIGN_BIT) != 0)
    {
        *ms_out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_cunt_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_utils_u32_call(ctx, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_NET_UTILS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_msg_alloc(void *ctx, uint32_t size, libresteam_net_msg_t **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t msg = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_net_utils_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_MSG_ALLOC, arg, cunt, &msg);
    if (res != LDG_ERR_AOK) { return res; }

    if (msg == 0) { return LDG_ERR_ALLOC_NULL; }

    *out = (libresteam_net_msg_t *)(uintptr_t)msg;

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_relay_status_get(void *ctx, libresteam_net_relay_status_t *details, uint32_t *avail_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *avail_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = details, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_RELAY_STATUS_GET, arg, cunt, avail_out);
}

static uint32_t steam_net_utils_local_ping_location_get(void *ctx, libresteam_net_ping_location_t *out, libresteam_fp32_t *age_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out || !age_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *age_out = 0;

    res = steam_net_utils_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_LOCAL_PING_LOCATION_GET, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = ldg_cpp_fp32_call(obj, slot, arg, cunt, age_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*age_out < 0)
    {
        *age_out = 0;
        return LDG_ERR_AGAIN;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_ping_between_estimate(void *ctx, const libresteam_net_ping_location_t *lhs, const libresteam_net_ping_location_t *rhs, uint32_t *ms_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    if (LDG_UNLIKELY(!lhs || !rhs)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = lhs, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = rhs, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_ping_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_BETWEEN_ESTIMATE, arg, cunt, ms_out);
}

static uint32_t steam_net_utils_ping_from_local_estimate(void *ctx, const libresteam_net_ping_location_t *remote, uint32_t *ms_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_ping_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_FROM_LOCAL_ESTIMATE, arg, cunt, ms_out);
}

static uint32_t steam_net_utils_ping_location_to_str(void *ctx, const libresteam_net_ping_location_t *location, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!location || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = location, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_LOCATION_TO_STR, arg, cunt);
}

static uint32_t steam_net_utils_ping_location_parse(void *ctx, const char *str, libresteam_net_ping_location_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = str, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_LOCATION_PARSE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_utils_ping_data_up_to_date_get(void *ctx, libresteam_fp32_t max_age_sec, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = max_age_sec, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_net_utils_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_DATA_UP_TO_DATE_GET, arg, cunt, out);
}

static uint32_t steam_net_utils_ping_to_data_center_get(void *ctx, uint32_t pop, uint32_t *via_relay_pop_out, uint32_t *ms_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!via_relay_pop_out || !ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *via_relay_pop_out = 0;
    *ms_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = pop, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = via_relay_pop_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_ping_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_PING_TO_DATA_CENTER_GET, arg, cunt, ms_out);
}

static uint32_t steam_net_utils_direct_ping_to_pop_get(void *ctx, uint32_t pop, uint32_t *ms_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ms_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *ms_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = pop, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_ping_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_DIRECT_PING_TO_POP_GET, arg, cunt, ms_out);
}

static uint32_t steam_net_utils_pop_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    return steam_net_utils_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_POP_CUNT_GET, 0x0, STEAM_NET_UTILS_ARG_NONE, out);
}

static uint32_t steam_net_utils_pop_list_get(void *ctx, uint32_t *list, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!list)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = list, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_POP_LIST_GET, arg, cunt, cunt_out);
}

static uint32_t steam_net_utils_local_timestamp_get(void *ctx, uint64_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    return steam_net_utils_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_LOCAL_TIMESTAMP_GET, 0x0, STEAM_NET_UTILS_ARG_NONE, out);
}

static uint32_t steam_net_utils_debug_output_set(void *ctx, uint32_t detail_lvl, libresteam_net_utils_debug_cb_t cb)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t cb_addr = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_secure_cpy(&cb_addr, &cb, (uint64_t)sizeof(cb));
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = detail_lvl, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cb_addr, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_DEBUG_OUTPUT_SET, arg, cunt);
}

static uint32_t steam_net_utils_ipv4_fake_ip_type_get(void *ctx, uint32_t ipv4, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = ipv4, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IPV4_FAKE_IP_TYPE_GET, arg, cunt, out);
}

static uint32_t steam_net_utils_real_ident_for_fake_ip_get(void *ctx, const libresteam_net_ip_addr_t *fake_ip, libresteam_net_ident_t *out, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!fake_ip || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = fake_ip, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_REAL_IDENT_FOR_FAKE_IP_GET, arg, cunt, eres_out);
}

static uint32_t steam_net_utils_conf_val_set(void *ctx, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg_val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = scope, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = scope_obj, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = data_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = arg_val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_CONF_VAL_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_utils_conf_val_get(void *ctx, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t *data_type_out, void *res_buff, uint64_t *res_size, uint32_t *get_res_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t get_res = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!get_res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *get_res_out = 0;

    if (LDG_UNLIKELY(!res_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = scope, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = scope_obj, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data_type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_CONF_VAL_GET, arg, cunt, &get_res);
    if (res != LDG_ERR_AOK) { return res; }

    if (get_res == STEAM_NET_UTILS_CONF_GET_BAD_VAL) { return LDG_ERR_NOT_FOUND; }

    if (get_res == STEAM_NET_UTILS_CONF_GET_BAD_SCOPE_OBJ) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (get_res == STEAM_NET_UTILS_CONF_GET_BUFF_TOO_SMALL) { return LDG_ERR_OVERFLOW; }

    if ((get_res & STEAM_NET_UTILS_SIGN_BIT) != 0) { return LDG_ERR_INVALID; }

    *get_res_out = get_res;

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_conf_val_info_get(void *ctx, uint32_t val, uint32_t *data_type_out, uint32_t *scope_out, const char **name_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t name = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *name_out = 0x0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data_type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = scope_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_net_utils_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_CONF_VAL_INFO_GET, arg, cunt, &name);
    if (res != LDG_ERR_AOK) { return res; }

    if (name == 0) { return LDG_ERR_NOT_FOUND; }

    *name_out = (const char *)(uintptr_t)name;

    return LDG_ERR_AOK;
}

static uint32_t steam_net_utils_editable_conf_val_iterate(void *ctx, uint32_t cur, uint8_t dev_vars_en, uint32_t *next_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!next_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *next_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = cur, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = dev_vars_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_EDITABLE_CONF_VAL_ITERATE, arg, cunt, next_out);
}

static uint32_t steam_net_utils_ip_addr_to_str(void *ctx, const libresteam_net_ip_addr_t *addr, char *buff, uint64_t cap, uint8_t port_en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!addr || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = addr, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IP_ADDR_TO_STR, arg, cunt);
}

static uint32_t steam_net_utils_ip_addr_parse(void *ctx, const char *str, libresteam_net_ip_addr_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = str, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IP_ADDR_PARSE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_utils_ip_addr_fake_ip_type_get(void *ctx, const libresteam_net_ip_addr_t *addr, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!addr)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = addr, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IP_ADDR_FAKE_IP_TYPE_GET, arg, cunt, out);
}

static uint32_t steam_net_utils_ident_to_str(void *ctx, const libresteam_net_ident_t *ident, char *buff, uint64_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ident || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ident, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IDENT_TO_STR, arg, cunt);
}

static uint32_t steam_net_utils_ident_parse(void *ctx, const char *str, libresteam_net_ident_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!str || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = str, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_IDENT_PARSE, arg, cunt, LDG_ERR_INVALID);
}

uint32_t libresteam_backend_steam_net_utils_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_net_utils_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->net_utils_ops;
    ops->msg_alloc = steam_net_utils_msg_alloc;
    ops->relay_status_get = steam_net_utils_relay_status_get;
    ops->local_ping_location_get = steam_net_utils_local_ping_location_get;
    ops->ping_between_estimate = steam_net_utils_ping_between_estimate;
    ops->ping_from_local_estimate = steam_net_utils_ping_from_local_estimate;
    ops->ping_location_to_str = steam_net_utils_ping_location_to_str;
    ops->ping_location_parse = steam_net_utils_ping_location_parse;
    ops->ping_data_up_to_date_get = steam_net_utils_ping_data_up_to_date_get;
    ops->ping_to_data_center_get = steam_net_utils_ping_to_data_center_get;
    ops->direct_ping_to_pop_get = steam_net_utils_direct_ping_to_pop_get;
    ops->pop_cunt_get = steam_net_utils_pop_cunt_get;
    ops->pop_list_get = steam_net_utils_pop_list_get;
    ops->local_timestamp_get = steam_net_utils_local_timestamp_get;
    ops->debug_output_set = steam_net_utils_debug_output_set;
    ops->ipv4_fake_ip_type_get = steam_net_utils_ipv4_fake_ip_type_get;
    ops->real_ident_for_fake_ip_get = steam_net_utils_real_ident_for_fake_ip_get;
    ops->conf_val_set = steam_net_utils_conf_val_set;
    ops->conf_val_get = steam_net_utils_conf_val_get;
    ops->conf_val_info_get = steam_net_utils_conf_val_info_get;
    ops->editable_conf_val_iterate = steam_net_utils_editable_conf_val_iterate;
    ops->ip_addr_to_str = steam_net_utils_ip_addr_to_str;
    ops->ip_addr_parse = steam_net_utils_ip_addr_parse;
    ops->ip_addr_fake_ip_type_get = steam_net_utils_ip_addr_fake_ip_type_get;
    ops->ident_to_str = steam_net_utils_ident_to_str;
    ops->ident_parse = steam_net_utils_ident_parse;
    steam->ops.net_utils = &steam->net_utils_ops;

    return LDG_ERR_AOK;
}
