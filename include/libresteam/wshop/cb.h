#ifndef LIBRESTEAM_WSHOP_CB_H
#define LIBRESTEAM_WSHOP_CB_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/wshop/types.h>

#define LIBRESTEAM_WSHOP_CB_SUBSCRIBE_RES 1313U
#define LIBRESTEAM_WSHOP_CB_UNSUBSCRIBE_RES 1315U
#define LIBRESTEAM_WSHOP_CB_QUERY_COMPLETED 3401U
#define LIBRESTEAM_WSHOP_CB_DETAILS_RES 3402U
#define LIBRESTEAM_WSHOP_CB_CREATE_RES 3403U
#define LIBRESTEAM_WSHOP_CB_SUBMIT_RES 3404U
#define LIBRESTEAM_WSHOP_CB_INSTALLED 3405U
#define LIBRESTEAM_WSHOP_CB_DOWNLOAD_RES 3406U
#define LIBRESTEAM_WSHOP_CB_FAVORITE_LIST_CHANGED 3407U
#define LIBRESTEAM_WSHOP_CB_VOTE_SET_RES 3408U
#define LIBRESTEAM_WSHOP_CB_VOTE_GET_RES 3409U
#define LIBRESTEAM_WSHOP_CB_PLAYTIME_START_RES 3410U
#define LIBRESTEAM_WSHOP_CB_PLAYTIME_STOP_RES 3411U
#define LIBRESTEAM_WSHOP_CB_DEPENDENCY_ADD_RES 3412U
#define LIBRESTEAM_WSHOP_CB_DEPENDENCY_REMOVE_RES 3413U
#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_ADD_RES 3414U
#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_REMOVE_RES 3415U
#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCIES_RES 3416U
#define LIBRESTEAM_WSHOP_CB_DELETE_RES 3417U
#define LIBRESTEAM_WSHOP_CB_SUBSCRIBED_LIST_CHANGED 3418U
#define LIBRESTEAM_WSHOP_CB_EULA_STATUS 3420U

#define LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_MAX 32U
#define LIBRESTEAM_WSHOP_CB_QUERY_COMPLETED_SIZE 280U
#define LIBRESTEAM_WSHOP_CB_SUBMIT_RES_SIZE 16U
#define LIBRESTEAM_WSHOP_CB_FAVORITE_LIST_CHANGED_SIZE 16U
#define LIBRESTEAM_WSHOP_CB_VOTE_SET_RES_SIZE 16U
#define LIBRESTEAM_WSHOP_CB_VOTE_GET_RES_SIZE 16U
#define LIBRESTEAM_WSHOP_CB_PLAYTIME_RES_SIZE 4U
#define LIBRESTEAM_WSHOP_CB_SUBSCRIBED_LIST_CHANGED_SIZE 4U
#define LIBRESTEAM_WSHOP_CB_EULA_STATUS_SIZE 20U

typedef struct libresteam_wshop_cb_query_completed
{
    uint64_t query;
    uint32_t eres;
    uint32_t res_cunt;
    uint32_t total_matching_cunt;
    uint8_t cached;
    char next_cursor[LIBRESTEAM_WSHOP_URL_MAX];
    uint8_t pudding[3];
} LDG_PACKED libresteam_wshop_cb_query_completed_t;

typedef struct libresteam_wshop_cb_submit_res
{
    uint32_t eres;
    uint8_t legal_agreement_needed;
    uint8_t pudding0[3];
    uint64_t file_id;
} LDG_PACKED libresteam_wshop_cb_submit_res_t;

typedef struct libresteam_wshop_cb_favorite_list_changed
{
    uint64_t file_id;
    uint32_t eres;
    uint8_t was_add_req;
    uint8_t pudding[3];
} LDG_PACKED libresteam_wshop_cb_favorite_list_changed_t;

typedef struct libresteam_wshop_cb_vote_set_res
{
    uint64_t file_id;
    uint32_t eres;
    uint8_t vote_up;
    uint8_t pudding[3];
} LDG_PACKED libresteam_wshop_cb_vote_set_res_t;

typedef struct libresteam_wshop_cb_vote_get_res
{
    uint64_t file_id;
    uint32_t eres;
    uint8_t voted_up;
    uint8_t voted_down;
    uint8_t vote_skipped;
    uint8_t pudding[1];
} LDG_PACKED libresteam_wshop_cb_vote_get_res_t;

typedef struct libresteam_wshop_cb_playtime_res
{
    uint32_t eres;
} LDG_PACKED libresteam_wshop_cb_playtime_res_t;

typedef struct libresteam_wshop_cb_subscribed_list_changed
{
    uint32_t app;
} LDG_PACKED libresteam_wshop_cb_subscribed_list_changed_t;

typedef struct libresteam_wshop_cb_eula_status
{
    uint32_t eres;
    uint32_t app;
    uint32_t version;
    uint32_t time_action;
    uint8_t accepted;
    uint8_t needs_action;
    uint8_t pudding[2];
} LDG_PACKED libresteam_wshop_cb_eula_status_t;

#include <libresteam/wshop/cb_pform.h>

#endif
