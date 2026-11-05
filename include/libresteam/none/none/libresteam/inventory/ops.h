#ifndef LIBRESTEAM_INVENTORY_OPS_H
#define LIBRESTEAM_INVENTORY_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/inventory/types.h>
#include <libresteam/steam_id/steam_id.h>

typedef struct libresteam_inventory_ops
{
    uint32_t (*res_status_get)(void *ctx, uint32_t res, uint32_t *eres_out);
    uint32_t (*res_items_get)(void *ctx, uint32_t res, libresteam_inventory_item_t *item, uint32_t *item_cunt);
    uint32_t (*res_item_property_get)(void *ctx, uint32_t res, uint32_t item_idx, const char *name, char *buff, uint32_t *buff_size);
    uint32_t (*res_timestamp_get)(void *ctx, uint32_t res, uint32_t *out);
    uint32_t (*res_owner_match_get)(void *ctx, uint32_t res, libresteam_steam_id_t expected, uint8_t *out);
    uint32_t (*res_destroy)(void *ctx, uint32_t res);
    uint32_t (*all_items_get)(void *ctx, uint32_t *res_out);
    uint32_t (*items_by_id_get)(void *ctx, uint32_t *res_out, const uint64_t *item_id, uint32_t item_id_cunt);
    uint32_t (*res_serialize)(void *ctx, uint32_t res, void *buff, uint32_t *buff_size);
    uint32_t (*res_deserialize)(void *ctx, uint32_t *res_out, const void *buff, uint32_t buff_size);
    uint32_t (*items_generate)(void *ctx, uint32_t *res_out, const uint32_t *def, const uint32_t *quantity, uint32_t len);
    uint32_t (*promo_items_grant)(void *ctx, uint32_t *res_out);
    uint32_t (*promo_item_add)(void *ctx, uint32_t *res_out, uint32_t def);
    uint32_t (*promo_items_add)(void *ctx, uint32_t *res_out, const uint32_t *def, uint32_t len);
    uint32_t (*item_consume)(void *ctx, uint32_t *res_out, uint64_t item_id, uint32_t quantity);
    uint32_t (*items_xchg)(void *ctx, uint32_t *res_out, const uint32_t *generate_def, const uint32_t *generate_quantity, uint32_t generate_len, const uint64_t *destroy_item_id, const uint32_t *destroy_quantity, uint32_t destroy_len);
    uint32_t (*item_quantity_xfer)(void *ctx, uint32_t *res_out, uint64_t item_src, uint32_t quantity, uint64_t item_dst);
    uint32_t (*item_drop_heartbeat_send)(void *ctx);
    uint32_t (*item_drop_trigger)(void *ctx, uint32_t *res_out, uint32_t drop_list_def);
    uint32_t (*items_trade)(void *ctx, uint32_t *res_out, libresteam_steam_id_t partner, const uint64_t *give_item_id, const uint32_t *give_quantity, uint32_t give_len, const uint64_t *get_item_id, const uint32_t *get_quantity, uint32_t get_len);
    uint32_t (*item_defs_ld)(void *ctx);
    uint32_t (*item_def_ids_get)(void *ctx, uint32_t *def, uint32_t *def_cunt);
    uint32_t (*item_def_property_get)(void *ctx, uint32_t def, const char *name, char *buff, uint32_t *buff_size);
    uint32_t (*eligible_promo_item_defs_req)(void *ctx, libresteam_steam_id_t user, uint64_t *call_out);
    uint32_t (*eligible_promo_item_def_ids_get)(void *ctx, libresteam_steam_id_t user, uint32_t *def, uint32_t *def_cunt);
    uint32_t (*purchase_start)(void *ctx, const uint32_t *def, const uint32_t *quantity, uint32_t len, uint64_t *call_out);
    uint32_t (*prices_req)(void *ctx, uint64_t *call_out);
    uint32_t (*items_with_prices_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*items_with_prices_get)(void *ctx, uint32_t *def, uint64_t *cur_price, uint64_t *base_price, uint32_t len);
    uint32_t (*item_price_get)(void *ctx, uint32_t def, uint64_t *cur_price_out, uint64_t *base_price_out);
    uint32_t (*property_update_start)(void *ctx, uint64_t *update_out);
    uint32_t (*property_remove)(void *ctx, uint64_t update, uint64_t item_id, const char *name);
    uint32_t (*property_str_set)(void *ctx, uint64_t update, uint64_t item_id, const char *name, const char *val);
    uint32_t (*property_bool_set)(void *ctx, uint64_t update, uint64_t item_id, const char *name, uint8_t val);
    uint32_t (*property_int_set)(void *ctx, uint64_t update, uint64_t item_id, const char *name, uint64_t val);
    uint32_t (*property_fp32_set)(void *ctx, uint64_t update, uint64_t item_id, const char *name, libresteam_fp32_t val);
    uint32_t (*property_update_submit)(void *ctx, uint64_t update, uint32_t *res_out);
    uint32_t (*item_inspect)(void *ctx, uint32_t *res_out, const char *tok);
} libresteam_inventory_ops_t;

#endif
