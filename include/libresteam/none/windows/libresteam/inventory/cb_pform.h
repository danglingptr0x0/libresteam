#ifndef LIBRESTEAM_INVENTORY_CB_PFORM_H
#define LIBRESTEAM_INVENTORY_CB_PFORM_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_INVENTORY_CB_ELIGIBLE_PROMO_ITEM_DEFS_SIZE 24U
#define LIBRESTEAM_INVENTORY_CB_PURCHASE_START_RES_SIZE 24U

typedef struct libresteam_inventory_cb_eligible_promo_item_defs
{
    uint32_t eres;
    uint8_t pudding0[4];
    libresteam_steam_id_t user;
    uint32_t def_cunt;
    uint8_t cached;
    uint8_t pudding[3];
} LDG_PACKED libresteam_inventory_cb_eligible_promo_item_defs_t;

typedef struct libresteam_inventory_cb_purchase_start_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t order_id;
    uint64_t transaction_id;
} LDG_PACKED libresteam_inventory_cb_purchase_start_res_t;

#endif
