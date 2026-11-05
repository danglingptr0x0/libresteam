#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/inventory/ops.h>
#include <libresteam/inventory/types.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_INVENTORY_ARG_NONE 0U
#define STEAM_INVENTORY_CALL_INVALID 0U
#define STEAM_INVENTORY_ID_SIZE ((uint32_t)sizeof(libresteam_steam_id_t))

static uint32_t steam_inventory_slot_get(void *ctx, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->inventory)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->inventory;

    return ldg_cpp_slot_get(&steam->inventory_desc, method_idx, slot_out);
}

static uint32_t steam_inventory_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t ret = LDG_ERR_AOK;

    ret = steam_inventory_slot_get(ctx, method_idx, &obj, &slot);
    if (ret != LDG_ERR_AOK) { return ret; }

    return ldg_cpp_void_call(obj, slot, arg, arg_cunt);
}

static uint32_t steam_inventory_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t ret = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    *out = LDG_TRUTH_FALSE;

    ret = steam_inventory_slot_get(ctx, method_idx, &obj, &slot);
    if (ret != LDG_ERR_AOK) { return ret; }

    ret = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_inventory_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t ret = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    ret = steam_inventory_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_inventory_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t ret = LDG_ERR_AOK;

    *out = 0;

    ret = steam_inventory_slot_get(ctx, method_idx, &obj, &slot);
    if (ret != LDG_ERR_AOK) { return ret; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_inventory_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t ret = LDG_ERR_AOK;

    *out = 0;

    ret = steam_inventory_slot_get(ctx, method_idx, &obj, &slot);
    if (ret != LDG_ERR_AOK) { return ret; }

    return ldg_cpp_uint64_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_inventory_res_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *res_out)
{
    uint32_t ret = LDG_ERR_AOK;

    ret = steam_inventory_bool_call(ctx, method_idx, arg, arg_cunt, LDG_ERR_INVALID);
    if (ret != LDG_ERR_AOK)
    {
        *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;
        return ret;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_inventory_async_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *call_out)
{
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = steam_inventory_u64_call(ctx, method_idx, arg, arg_cunt, call_out);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (*call_out == STEAM_INVENTORY_CALL_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_inventory_res_status_get(void *ctx, uint32_t res, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_STATUS_GET, arg, cunt, eres_out);
}

static uint32_t steam_inventory_res_items_get(void *ctx, uint32_t res, libresteam_inventory_item_t *item, uint32_t *item_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!item_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = item, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = item_cunt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_ITEMS_GET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_res_item_property_get(void *ctx, uint32_t res, uint32_t item_idx, const char *name, char *buff, uint32_t *buff_size)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_ITEM_PROPERTY_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_res_timestamp_get(void *ctx, uint32_t res, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_TIMESTAMP_GET, arg, cunt, out);
}

static uint32_t steam_inventory_res_owner_match_get(void *ctx, uint32_t res, libresteam_steam_id_t expected, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &expected, .size = STEAM_INVENTORY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_inventory_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_OWNER_MATCH_GET, arg, cunt, out);
}

static uint32_t steam_inventory_res_destroy(void *ctx, uint32_t res)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_DESTROY, arg, cunt);
}

static uint32_t steam_inventory_all_items_get(void *ctx, uint32_t *res_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ALL_ITEMS_GET, arg, cunt, res_out);
}

static uint32_t steam_inventory_items_by_id_get(void *ctx, uint32_t *res_out, const uint64_t *item_id, uint32_t item_id_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !item_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = item_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_BY_ID_GET, arg, cunt, res_out);
}

static uint32_t steam_inventory_res_serialize(void *ctx, uint32_t res, void *buff, uint32_t *buff_size)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = res, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_SERIALIZE, arg, cunt, LDG_ERR_OVERFLOW);
}

static uint32_t steam_inventory_res_deserialize(void *ctx, uint32_t *res_out, const void *buff, uint32_t buff_size)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = buff_size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = LDG_TRUTH_FALSE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_RES_DESERIALIZE, arg, cunt, res_out);
}

static uint32_t steam_inventory_items_generate(void *ctx, uint32_t *res_out, const uint32_t *def, const uint32_t *quantity, uint32_t len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !def)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_GENERATE, arg, cunt, res_out);
}

static uint32_t steam_inventory_promo_items_grant(void *ctx, uint32_t *res_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROMO_ITEMS_GRANT, arg, cunt, res_out);
}

static uint32_t steam_inventory_promo_item_add(void *ctx, uint32_t *res_out, uint32_t def)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = def, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROMO_ITEM_ADD, arg, cunt, res_out);
}

static uint32_t steam_inventory_promo_items_add(void *ctx, uint32_t *res_out, const uint32_t *def, uint32_t len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !def)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROMO_ITEMS_ADD, arg, cunt, res_out);
}

static uint32_t steam_inventory_item_consume(void *ctx, uint32_t *res_out, uint64_t item_id, uint32_t quantity)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = quantity, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_CONSUME, arg, cunt, res_out);
}

static uint32_t steam_inventory_items_xchg(void *ctx, uint32_t *res_out, const uint32_t *generate_def, const uint32_t *generate_quantity, uint32_t generate_len, const uint64_t *destroy_item_id, const uint32_t *destroy_quantity, uint32_t destroy_len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !generate_def || !destroy_item_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = generate_def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = generate_quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = generate_len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = destroy_item_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = destroy_quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = destroy_len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_XCHG, arg, cunt, res_out);
}

static uint32_t steam_inventory_item_quantity_xfer(void *ctx, uint32_t *res_out, uint64_t item_src, uint32_t quantity, uint64_t item_dst)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_src, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = quantity, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_dst, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_QUANTITY_XFER, arg, cunt, res_out);
}

static uint32_t steam_inventory_item_drop_heartbeat_send(void *ctx)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t ret = LDG_ERR_AOK;

    ret = steam_inventory_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_DROP_HEARTBEAT_SEND, &obj, &slot);
    if (ret != LDG_ERR_AOK) { return ret; }

    return ldg_cpp_void_call(obj, slot, 0x0, STEAM_INVENTORY_ARG_NONE);
}

static uint32_t steam_inventory_item_drop_trigger(void *ctx, uint32_t *res_out, uint32_t drop_list_def)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = drop_list_def, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_DROP_TRIGGER, arg, cunt, res_out);
}

static uint32_t steam_inventory_items_trade(void *ctx, uint32_t *res_out, libresteam_steam_id_t partner, const uint64_t *give_item_id, const uint32_t *give_quantity, uint32_t give_len, const uint64_t *get_item_id, const uint32_t *get_quantity, uint32_t get_len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &partner, .size = STEAM_INVENTORY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = give_item_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = give_quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = give_len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = get_item_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = get_quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = get_len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_TRADE, arg, cunt, res_out);
}

static uint32_t steam_inventory_item_defs_ld(void *ctx)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_DEFS_LD, 0x0, STEAM_INVENTORY_ARG_NONE, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_item_def_ids_get(void *ctx, uint32_t *def, uint32_t *def_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!def_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def_cunt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_DEF_IDS_GET, arg, cunt, def ? LDG_ERR_OVERFLOW : LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_item_def_property_get(void *ctx, uint32_t def, const char *name, char *buff, uint32_t *buff_size)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = def, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_DEF_PROPERTY_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_eligible_promo_item_defs_req(void *ctx, libresteam_steam_id_t user, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &user, .size = STEAM_INVENTORY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_inventory_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ELIGIBLE_PROMO_ITEM_DEFS_REQ, arg, cunt, call_out);
}

static uint32_t steam_inventory_eligible_promo_item_def_ids_get(void *ctx, libresteam_steam_id_t user, uint32_t *def, uint32_t *def_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!def_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &user, .size = STEAM_INVENTORY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def_cunt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ELIGIBLE_PROMO_ITEM_DEF_IDS_GET, arg, cunt, def ? LDG_ERR_OVERFLOW : LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_purchase_start(void *ctx, const uint32_t *def, const uint32_t *quantity, uint32_t len, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!def || !quantity)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = quantity, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PURCHASE_START, arg, cunt, call_out);
}

static uint32_t steam_inventory_prices_req(void *ctx, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_inventory_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PRICES_REQ, 0x0, STEAM_INVENTORY_ARG_NONE, call_out);
}

static uint32_t steam_inventory_items_with_prices_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_inventory_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_WITH_PRICES_CUNT_GET, 0x0, STEAM_INVENTORY_ARG_NONE, out);
}

static uint32_t steam_inventory_items_with_prices_get(void *ctx, uint32_t *def, uint64_t *cur_price, uint64_t *base_price, uint32_t len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!def || !cur_price || !base_price)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = def, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = cur_price, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = base_price, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEMS_WITH_PRICES_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_item_price_get(void *ctx, uint32_t def, uint64_t *cur_price_out, uint64_t *base_price_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cur_price_out || !base_price_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cur_price_out = 0;
    *base_price_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = def, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = cur_price_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = base_price_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_PRICE_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_inventory_property_update_start(void *ctx, uint64_t *update_out)
{
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!update_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = steam_inventory_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_UPDATE_START, 0x0, STEAM_INVENTORY_ARG_NONE, update_out);
    if (ret != LDG_ERR_AOK)
    {
        *update_out = LIBRESTEAM_INVENTORY_UPDATE_INVALID;
        return ret;
    }

    if (*update_out == LIBRESTEAM_INVENTORY_UPDATE_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_inventory_property_remove(void *ctx, uint64_t update, uint64_t item_id, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_REMOVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_property_str_set(void *ctx, uint64_t update, uint64_t item_id, const char *name, const char *val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_STR_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_property_bool_set(void *ctx, uint64_t update, uint64_t item_id, const char *name, uint8_t val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_BOOL_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_property_int_set(void *ctx, uint64_t update, uint64_t item_id, const char *name, uint64_t val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_INT_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_property_fp32_set(void *ctx, uint64_t update, uint64_t item_id, const char *name, libresteam_fp32_t val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = item_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = val, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_inventory_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_FP32_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_inventory_property_update_submit(void *ctx, uint64_t update, uint32_t *res_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_UPDATE_SUBMIT, arg, cunt, res_out);
}

static uint32_t steam_inventory_item_inspect(void *ctx, uint32_t *res_out, const char *tok)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!res_out || !tok)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tok, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_inventory_res_call(ctx, LIBRESTEAM_BACKEND_STEAM_INVENTORY_ITEM_INSPECT, arg, cunt, res_out);
}

uint32_t libresteam_backend_steam_inventory_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_inventory_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->inventory_ops;
    ops->res_status_get = steam_inventory_res_status_get;
    ops->res_items_get = steam_inventory_res_items_get;
    ops->res_item_property_get = steam_inventory_res_item_property_get;
    ops->res_timestamp_get = steam_inventory_res_timestamp_get;
    ops->res_owner_match_get = steam_inventory_res_owner_match_get;
    ops->res_destroy = steam_inventory_res_destroy;
    ops->all_items_get = steam_inventory_all_items_get;
    ops->items_by_id_get = steam_inventory_items_by_id_get;
    ops->res_serialize = steam_inventory_res_serialize;
    ops->res_deserialize = steam_inventory_res_deserialize;
    ops->items_generate = steam_inventory_items_generate;
    ops->promo_items_grant = steam_inventory_promo_items_grant;
    ops->promo_item_add = steam_inventory_promo_item_add;
    ops->promo_items_add = steam_inventory_promo_items_add;
    ops->item_consume = steam_inventory_item_consume;
    ops->items_xchg = steam_inventory_items_xchg;
    ops->item_quantity_xfer = steam_inventory_item_quantity_xfer;
    ops->item_drop_heartbeat_send = steam_inventory_item_drop_heartbeat_send;
    ops->item_drop_trigger = steam_inventory_item_drop_trigger;
    ops->items_trade = steam_inventory_items_trade;
    ops->item_defs_ld = steam_inventory_item_defs_ld;
    ops->item_def_ids_get = steam_inventory_item_def_ids_get;
    ops->item_def_property_get = steam_inventory_item_def_property_get;
    ops->eligible_promo_item_defs_req = steam_inventory_eligible_promo_item_defs_req;
    ops->eligible_promo_item_def_ids_get = steam_inventory_eligible_promo_item_def_ids_get;
    ops->purchase_start = steam_inventory_purchase_start;
    ops->prices_req = steam_inventory_prices_req;
    ops->items_with_prices_cunt_get = steam_inventory_items_with_prices_cunt_get;
    ops->items_with_prices_get = steam_inventory_items_with_prices_get;
    ops->item_price_get = steam_inventory_item_price_get;
    ops->property_update_start = steam_inventory_property_update_start;
    ops->property_remove = steam_inventory_property_remove;
    ops->property_str_set = steam_inventory_property_str_set;
    ops->property_bool_set = steam_inventory_property_bool_set;
    ops->property_int_set = steam_inventory_property_int_set;
    ops->property_fp32_set = steam_inventory_property_fp32_set;
    ops->property_update_submit = steam_inventory_property_update_submit;
    ops->item_inspect = steam_inventory_item_inspect;
    steam->ops.inventory = &steam->inventory_ops;

    return LDG_ERR_AOK;
}
