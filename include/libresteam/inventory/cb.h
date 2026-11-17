#ifndef LIBRESTEAM_INVENTORY_CB_H
#define LIBRESTEAM_INVENTORY_CB_H

#include <stdint.h>
#include <dangling/core/macros.h>

#define LIBRESTEAM_INVENTORY_CB_RES_READY 4700U
#define LIBRESTEAM_INVENTORY_CB_FULL_UPDATE 4701U
#define LIBRESTEAM_INVENTORY_CB_DEF_UPDATE 4702U
#define LIBRESTEAM_INVENTORY_CB_ELIGIBLE_PROMO_ITEM_DEFS 4703U
#define LIBRESTEAM_INVENTORY_CB_PURCHASE_START_RES 4704U
#define LIBRESTEAM_INVENTORY_CB_PRICES_RES 4705U

#define LIBRESTEAM_INVENTORY_CB_CURRENCY_MAX 4U
#define LIBRESTEAM_INVENTORY_CB_RES_READY_SIZE 8U
#define LIBRESTEAM_INVENTORY_CB_FULL_UPDATE_SIZE 4U
#define LIBRESTEAM_INVENTORY_CB_PRICES_RES_SIZE 8U

typedef struct libresteam_inventory_cb_res_ready
{
    uint32_t res;
    uint32_t eres;
} LDG_PACKED libresteam_inventory_cb_res_ready_t;

typedef struct libresteam_inventory_cb_full_update
{
    uint32_t res;
} LDG_PACKED libresteam_inventory_cb_full_update_t;

typedef struct libresteam_inventory_cb_prices_res
{
    uint32_t eres;
    char currency[LIBRESTEAM_INVENTORY_CB_CURRENCY_MAX];
} LDG_PACKED libresteam_inventory_cb_prices_res_t;

#include <libresteam/inventory/cb_pform.h>

#endif
