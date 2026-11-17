#ifndef LIBRESTEAM_INVENTORY_H
#define LIBRESTEAM_INVENTORY_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/inventory/ops.h>
#include <libresteam/inventory/types.h>
#include <libresteam/steam_id/steam_id.h>

LDG_EXPORT uint32_t libresteam_inventory_res_status_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t *eres_out);
LDG_EXPORT uint32_t libresteam_inventory_res_items_get(const libresteam_backend_set_t *set, uint32_t res, libresteam_inventory_item_t *item, uint32_t *item_cunt);
LDG_EXPORT uint32_t libresteam_inventory_res_item_property_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t item_idx, const char *name, char *buff, uint32_t *buff_size);
LDG_EXPORT uint32_t libresteam_inventory_res_timestamp_get(const libresteam_backend_set_t *set, uint32_t res, uint32_t *out);
LDG_EXPORT uint32_t libresteam_inventory_res_owner_match_get(const libresteam_backend_set_t *set, uint32_t res, libresteam_steam_id_t expected, uint8_t *out);
LDG_EXPORT uint32_t libresteam_inventory_res_destroy(const libresteam_backend_set_t *set, uint32_t res);
LDG_EXPORT uint32_t libresteam_inventory_all_items_get(const libresteam_backend_set_t *set, uint32_t *res_out);
LDG_EXPORT uint32_t libresteam_inventory_items_by_id_get(const libresteam_backend_set_t *set, uint32_t *res_out, const uint64_t *item_id, uint32_t item_id_cunt);
LDG_EXPORT uint32_t libresteam_inventory_res_serialize(const libresteam_backend_set_t *set, uint32_t res, void *buff, uint32_t *buff_size);
LDG_EXPORT uint32_t libresteam_inventory_res_deserialize(const libresteam_backend_set_t *set, uint32_t *res_out, const void *buff, uint32_t buff_size);
LDG_EXPORT uint32_t libresteam_inventory_items_generate(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *def, const uint32_t *quantity, uint32_t len);
LDG_EXPORT uint32_t libresteam_inventory_promo_items_grant(const libresteam_backend_set_t *set, uint32_t *res_out);
LDG_EXPORT uint32_t libresteam_inventory_promo_item_add(const libresteam_backend_set_t *set, uint32_t *res_out, uint32_t def);
LDG_EXPORT uint32_t libresteam_inventory_promo_items_add(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *def, uint32_t len);
LDG_EXPORT uint32_t libresteam_inventory_item_consume(const libresteam_backend_set_t *set, uint32_t *res_out, uint64_t item_id, uint32_t quantity);
LDG_EXPORT uint32_t libresteam_inventory_items_xchg(const libresteam_backend_set_t *set, uint32_t *res_out, const uint32_t *generate_def, const uint32_t *generate_quantity, uint32_t generate_len, const uint64_t *destroy_item_id, const uint32_t *destroy_quantity, uint32_t destroy_len);
LDG_EXPORT uint32_t libresteam_inventory_item_quantity_xfer(const libresteam_backend_set_t *set, uint32_t *res_out, uint64_t item_src, uint32_t quantity, uint64_t item_dst);
LDG_EXPORT uint32_t libresteam_inventory_item_drop_heartbeat_send(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_inventory_item_drop_trigger(const libresteam_backend_set_t *set, uint32_t *res_out, uint32_t drop_list_def);
LDG_EXPORT uint32_t libresteam_inventory_items_trade(const libresteam_backend_set_t *set, uint32_t *res_out, libresteam_steam_id_t partner, const uint64_t *give_item_id, const uint32_t *give_quantity, uint32_t give_len, const uint64_t *get_item_id, const uint32_t *get_quantity, uint32_t get_len);
LDG_EXPORT uint32_t libresteam_inventory_item_defs_ld(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_inventory_item_def_ids_get(const libresteam_backend_set_t *set, uint32_t *def, uint32_t *def_cunt);
LDG_EXPORT uint32_t libresteam_inventory_item_def_property_get(const libresteam_backend_set_t *set, uint32_t def, const char *name, char *buff, uint32_t *buff_size);
LDG_EXPORT uint32_t libresteam_inventory_eligible_promo_item_defs_req(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_inventory_eligible_promo_item_def_ids_get(const libresteam_backend_set_t *set, libresteam_steam_id_t user, uint32_t *def, uint32_t *def_cunt);
LDG_EXPORT uint32_t libresteam_inventory_purchase_start(const libresteam_backend_set_t *set, const uint32_t *def, const uint32_t *quantity, uint32_t len, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_inventory_prices_req(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_inventory_items_with_prices_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_inventory_items_with_prices_get(const libresteam_backend_set_t *set, uint32_t *def, uint64_t *cur_price, uint64_t *base_price, uint32_t len);
LDG_EXPORT uint32_t libresteam_inventory_item_price_get(const libresteam_backend_set_t *set, uint32_t def, uint64_t *cur_price_out, uint64_t *base_price_out);
LDG_EXPORT uint32_t libresteam_inventory_property_update_start(const libresteam_backend_set_t *set, uint64_t *update_out);
LDG_EXPORT uint32_t libresteam_inventory_property_remove(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name);
LDG_EXPORT uint32_t libresteam_inventory_property_str_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, const char *val);
LDG_EXPORT uint32_t libresteam_inventory_property_bool_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, uint8_t val);
LDG_EXPORT uint32_t libresteam_inventory_property_int_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, uint64_t val);
LDG_EXPORT uint32_t libresteam_inventory_property_fp32_set(const libresteam_backend_set_t *set, uint64_t update, uint64_t item_id, const char *name, libresteam_fp32_t val);
LDG_EXPORT uint32_t libresteam_inventory_property_update_submit(const libresteam_backend_set_t *set, uint64_t update, uint32_t *res_out);
LDG_EXPORT uint32_t libresteam_inventory_item_inspect(const libresteam_backend_set_t *set, uint32_t *res_out, const char *tok);

#endif
