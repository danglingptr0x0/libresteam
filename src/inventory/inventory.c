#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/inventory/cb.h>
#include <libresteam/inventory/inventory.h>
#include <libresteam/inventory/ops.h>
#include <libresteam/inventory/types.h>
#include <libresteam/steam_id/steam_id.h>

#define INVENTORY_LAYOUT_PASS 1
#define INVENTORY_LAYOUT_FAIL 0

typedef struct { uint32_t ck : (sizeof(libresteam_inventory_item_t) == LIBRESTEAM_INVENTORY_ITEM_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_item_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_inventory_cb_res_ready_t) == LIBRESTEAM_INVENTORY_CB_RES_READY_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_cb_res_ready_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_inventory_cb_full_update_t) == LIBRESTEAM_INVENTORY_CB_FULL_UPDATE_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_cb_full_update_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_inventory_cb_prices_res_t) == LIBRESTEAM_INVENTORY_CB_PRICES_RES_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_cb_prices_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_inventory_cb_eligible_promo_item_defs_t) == LIBRESTEAM_INVENTORY_CB_ELIGIBLE_PROMO_ITEM_DEFS_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_cb_eligible_promo_item_defs_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_inventory_cb_purchase_start_res_t) == LIBRESTEAM_INVENTORY_CB_PURCHASE_START_RES_SIZE) ? INVENTORY_LAYOUT_PASS : INVENTORY_LAYOUT_FAIL;
} inventory_cb_purchase_start_res_size_ck_t;

static uint32_t inventory_ops_get(const libresteam_backend_set_t *set, const libresteam_inventory_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->inventory) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->inventory;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_inventory_res_status_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t *eres_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_status_get(ctx, res, eres_out);
}

uint32_t libresteam_inventory_res_items_get(const libresteam_backend_set_t *set, uint32_t res, libresteam_inventory_item_t *item, uint32_t *item_cunt)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!item_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_items_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_items_get(ctx, res, item, item_cunt);
}

uint32_t libresteam_inventory_res_item_property_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t item_idx, const char *name, char *buff, uint32_t *buff_size)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_item_property_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_item_property_get(ctx, res, item_idx, name, buff, buff_size);
}

uint32_t libresteam_inventory_res_timestamp_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t *out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_timestamp_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_timestamp_get(ctx, res, out);
}

uint32_t libresteam_inventory_res_owner_match_get(const libresteam_backend_set_t *set, uint32_t res, libresteam_steam_id_t expected, uint8_t *out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_owner_match_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_owner_match_get(ctx, res, expected, out);
}

uint32_t libresteam_inventory_res_destroy(const libresteam_backend_set_t *set, uint32_t res)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_destroy) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_destroy(ctx, res);
}

uint32_t libresteam_inventory_all_items_get(const libresteam_backend_set_t *set, uint32_t *res_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->all_items_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->all_items_get(ctx, res_out);
}

uint32_t libresteam_inventory_items_by_id_get(const libresteam_backend_set_t *set, uint32_t *res_out, const uint64_t *item_id, uint32_t item_id_cunt)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_by_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_by_id_get(ctx, res_out, item_id, item_id_cunt);
}

uint32_t libresteam_inventory_res_serialize(const libresteam_backend_set_t *set, uint32_t res, void *buff, uint32_t *buff_size)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_serialize) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_serialize(ctx, res, buff, buff_size);
}

uint32_t libresteam_inventory_res_deserialize(const libresteam_backend_set_t *set, uint32_t *res_out, const void *buff, uint32_t buff_size)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->res_deserialize) { return LDG_ERR_UNSUPPORTED; }

    return ops->res_deserialize(ctx, res_out, buff, buff_size);
}

uint32_t libresteam_inventory_items_generate(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *def, const uint32_t *quantity, uint32_t len)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_generate) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_generate(ctx, res_out, def, quantity, len);
}

uint32_t libresteam_inventory_promo_items_grant(const libresteam_backend_set_t *set, uint32_t *res_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->promo_items_grant) { return LDG_ERR_UNSUPPORTED; }

    return ops->promo_items_grant(ctx, res_out);
}

uint32_t libresteam_inventory_promo_item_add(const libresteam_backend_set_t *set, uint32_t *res_out, uint32_t def)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->promo_item_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->promo_item_add(ctx, res_out, def);
}

uint32_t libresteam_inventory_promo_items_add(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *def, uint32_t len)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->promo_items_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->promo_items_add(ctx, res_out, def, len);
}

uint32_t libresteam_inventory_item_consume(const libresteam_backend_set_t *set, uint32_t *res_out, uint64_t item_id, uint32_t quantity)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_consume) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_consume(ctx, res_out, item_id, quantity);
}

uint32_t libresteam_inventory_items_xchg(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *generate_def, const uint32_t *generate_quantity, uint32_t generate_len, const uint64_t *destroy_item_id, const uint32_t *destroy_quantity, uint32_t destroy_len)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_xchg) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_xchg(ctx, res_out, generate_def, generate_quantity, generate_len, destroy_item_id, destroy_quantity, destroy_len);
}

uint32_t libresteam_inventory_item_quantity_xfer(const libresteam_backend_set_t *set, uint32_t *res_out, uint64_t item_src, uint32_t quantity, uint64_t item_dst)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_quantity_xfer) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_quantity_xfer(ctx, res_out, item_src, quantity, item_dst);
}

uint32_t libresteam_inventory_item_drop_heartbeat_send(const libresteam_backend_set_t *set)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_drop_heartbeat_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_drop_heartbeat_send(ctx);
}

uint32_t libresteam_inventory_item_drop_trigger(const libresteam_backend_set_t *set, uint32_t *res_out, uint32_t drop_list_def)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_drop_trigger) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_drop_trigger(ctx, res_out, drop_list_def);
}

uint32_t libresteam_inventory_items_trade(const libresteam_backend_set_t *set, uint32_t *res_out, libresteam_steam_id_t partner, const uint64_t *give_item_id, const uint32_t *give_quantity, uint32_t give_len, const uint64_t *get_item_id, const uint32_t *get_quantity, uint32_t get_len)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_trade) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_trade(ctx, res_out, partner, give_item_id, give_quantity, give_len, get_item_id, get_quantity, get_len);
}

uint32_t libresteam_inventory_item_defs_ld(const libresteam_backend_set_t *set)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_defs_ld) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_defs_ld(ctx);
}

uint32_t libresteam_inventory_item_def_ids_get(const libresteam_backend_set_t *set, uint32_t *def, uint32_t *def_cunt)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!def_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_def_ids_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_def_ids_get(ctx, def, def_cunt);
}

uint32_t libresteam_inventory_item_def_property_get(const libresteam_backend_set_t *set, uint32_t def, const char *name, char *buff, uint32_t *buff_size)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_def_property_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_def_property_get(ctx, def, name, buff, buff_size);
}

uint32_t libresteam_inventory_eligible_promo_item_defs_req(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint64_t *call_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->eligible_promo_item_defs_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->eligible_promo_item_defs_req(ctx, user, call_out);
}

uint32_t libresteam_inventory_eligible_promo_item_def_ids_get(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint32_t *def, uint32_t *def_cunt)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!def_cunt)) { return LDG_ERR_FUNC_ARG_NULL; }

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->eligible_promo_item_def_ids_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->eligible_promo_item_def_ids_get(ctx, user, def, def_cunt);
}

uint32_t libresteam_inventory_purchase_start(const libresteam_backend_set_t *set, const uint32_t *def, const uint32_t *quantity, uint32_t len, uint64_t *call_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->purchase_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->purchase_start(ctx, def, quantity, len, call_out);
}

uint32_t libresteam_inventory_prices_req(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->prices_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->prices_req(ctx, call_out);
}

uint32_t libresteam_inventory_items_with_prices_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_with_prices_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_with_prices_cunt_get(ctx, out);
}

uint32_t libresteam_inventory_items_with_prices_get(const libresteam_backend_set_t *set, uint32_t *def, uint64_t *cur_price, uint64_t *base_price, uint32_t len)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->items_with_prices_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_with_prices_get(ctx, def, cur_price, base_price, len);
}

uint32_t libresteam_inventory_item_price_get(const libresteam_backend_set_t *set, uint32_t def, uint64_t *cur_price_out, uint64_t *base_price_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cur_price_out || !base_price_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cur_price_out = 0;
    *base_price_out = 0;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_price_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_price_get(ctx, def, cur_price_out, base_price_out);
}

uint32_t libresteam_inventory_property_update_start(const libresteam_backend_set_t *set, uint64_t *update_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!update_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *update_out = LIBRESTEAM_INVENTORY_UPDATE_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_update_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_update_start(ctx, update_out);
}

uint32_t libresteam_inventory_property_remove(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_remove(ctx, update, item_id, name);
}

uint32_t libresteam_inventory_property_str_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, const char *val)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_str_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_str_set(ctx, update, item_id, name, val);
}

uint32_t libresteam_inventory_property_bool_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, uint8_t val)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_bool_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_bool_set(ctx, update, item_id, name, val);
}

uint32_t libresteam_inventory_property_int_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, uint64_t val)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_int_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_int_set(ctx, update, item_id, name, val);
}

uint32_t libresteam_inventory_property_fp32_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, libresteam_fp32_t val)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_fp32_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_fp32_set(ctx, update, item_id, name, val);
}

uint32_t libresteam_inventory_property_update_submit(const libresteam_backend_set_t *set, uint64_t update, uint32_t *res_out)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->property_update_submit) { return LDG_ERR_UNSUPPORTED; }

    return ops->property_update_submit(ctx, update, res_out);
}

uint32_t libresteam_inventory_item_inspect(const libresteam_backend_set_t *set, uint32_t *res_out, const char *tok)
{
    const libresteam_inventory_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t ret = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!res_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *res_out = LIBRESTEAM_INVENTORY_RES_INVALID;

    ret = inventory_ops_get(set, &ops, &ctx);
    if (ret != LDG_ERR_AOK) { return ret; }

    if (!ops->item_inspect) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_inspect(ctx, res_out, tok);
}
