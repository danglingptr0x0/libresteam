#ifndef LIBRESTEAM_WSHOP_CB_PFORM_H
#define LIBRESTEAM_WSHOP_CB_PFORM_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/wshop/pform.h>
#include <libresteam/wshop/types.h>

#define LIBRESTEAM_WSHOP_CB_DETAILS_RES_SIZE 9792U
#define LIBRESTEAM_WSHOP_CB_CREATE_RES_SIZE 24U
#define LIBRESTEAM_WSHOP_CB_INSTALLED_SIZE 32U
#define LIBRESTEAM_WSHOP_CB_DOWNLOAD_RES_SIZE 24U
#define LIBRESTEAM_WSHOP_CB_DEPENDENCY_RES_SIZE 24U
#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_RES_SIZE 24U
#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCIES_RES_SIZE 152U
#define LIBRESTEAM_WSHOP_CB_FILE_RES_SIZE 16U

typedef struct libresteam_wshop_cb_details_res
{
    libresteam_wshop_details_t details;
    uint8_t cached;
    uint8_t pudding[7];
} libresteam_wshop_cb_details_res_t;

typedef struct libresteam_wshop_cb_create_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint8_t legal_agreement_needed;
    uint8_t pudding[7];
} LDG_PACKED libresteam_wshop_cb_create_res_t;

typedef struct libresteam_wshop_cb_installed
{
    uint32_t app;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint64_t legacy_content;
    uint64_t manifest;
} LDG_PACKED libresteam_wshop_cb_installed_t;

typedef struct libresteam_wshop_cb_download_res
{
    uint32_t app;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint32_t eres;
    uint8_t pudding[4];
} LDG_PACKED libresteam_wshop_cb_download_res_t;

typedef struct libresteam_wshop_cb_dependency_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint64_t child;
} LDG_PACKED libresteam_wshop_cb_dependency_res_t;

typedef struct libresteam_wshop_cb_app_dependency_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint32_t app;
    uint8_t pudding[4];
} LDG_PACKED libresteam_wshop_cb_app_dependency_res_t;

typedef struct libresteam_wshop_cb_app_dependencies_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t file_id;
    uint32_t app[LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_MAX];
    uint32_t app_cunt;
    uint32_t total_app_cunt;
} LDG_PACKED libresteam_wshop_cb_app_dependencies_res_t;

typedef struct libresteam_wshop_cb_file_res
{
    uint32_t eres;
    uint8_t pudding0[4];
    uint64_t file_id;
} LDG_PACKED libresteam_wshop_cb_file_res_t;

#endif
