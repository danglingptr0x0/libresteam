#ifndef LIBRESTEAM_SRV_BROWSER_OPS_H
#define LIBRESTEAM_SRV_BROWSER_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_SRV_BROWSER_GAME_DIR_MAX 32U
#define LIBRESTEAM_SRV_BROWSER_MAP_MAX 32U
#define LIBRESTEAM_SRV_BROWSER_GAME_DESC_MAX 64U
#define LIBRESTEAM_SRV_BROWSER_NAME_MAX 64U
#define LIBRESTEAM_SRV_BROWSER_TAG_MAX 128U
#define LIBRESTEAM_SRV_BROWSER_KV_MAX 256U
#define LIBRESTEAM_SRV_BROWSER_ITEM_SIZE 372U
#define LIBRESTEAM_SRV_BROWSER_QUERY_INVALID 0xFFFFFFFFU

typedef enum libresteam_srv_browser_list_type
{
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_INTERNET = 0,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_LAN,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_FRIENDS,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_FAVORITES,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_HISTORY,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_SPECTATOR,
    LIBRESTEAM_SRV_BROWSER_LIST_TYPE_MAX
} libresteam_srv_browser_list_type_t;

typedef enum libresteam_srv_browser_resp
{
    LIBRESTEAM_SRV_BROWSER_RESP_RESPONDED = 0,
    LIBRESTEAM_SRV_BROWSER_RESP_FAILED_TO_RESPOND,
    LIBRESTEAM_SRV_BROWSER_RESP_NO_SRV_LISTED,
    LIBRESTEAM_SRV_BROWSER_RESP_MAX
} libresteam_srv_browser_resp_t;

typedef struct libresteam_srv_browser_netadr
{
    uint16_t conn_port;
    uint16_t query_port;
    uint32_t ip;
} LDG_PACKED libresteam_srv_browser_netadr_t;

typedef struct libresteam_srv_browser_item
{
    libresteam_srv_browser_netadr_t netadr;
    uint32_t ping;
    uint8_t had_successful_resp;
    uint8_t do_not_refresh;
    char game_dir[LIBRESTEAM_SRV_BROWSER_GAME_DIR_MAX];
    char map[LIBRESTEAM_SRV_BROWSER_MAP_MAX];
    char game_desc[LIBRESTEAM_SRV_BROWSER_GAME_DESC_MAX];
    uint8_t pudding0[2];
    uint32_t app_id;
    uint32_t player_cunt;
    uint32_t player_max;
    uint32_t bot_cunt;
    uint8_t password;
    uint8_t secure;
    uint8_t pudding1[2];
    uint32_t time_last_played;
    uint32_t srv_version;
    char name[LIBRESTEAM_SRV_BROWSER_NAME_MAX];
    char tag[LIBRESTEAM_SRV_BROWSER_TAG_MAX];
    libresteam_steam_id_t steam_id;
} LDG_PACKED libresteam_srv_browser_item_t;

typedef struct libresteam_srv_browser_kv
{
    char key[LIBRESTEAM_SRV_BROWSER_KV_MAX];
    char val[LIBRESTEAM_SRV_BROWSER_KV_MAX];
} LDG_PACKED libresteam_srv_browser_kv_t;

typedef struct libresteam_srv_browser_list_resp_ops
{
    void (*srv_responded_cb)(void *content, void *req, uint32_t srv_idx);
    void (*srv_failed_cb)(void *content, void *req, uint32_t srv_idx);
    void (*refresh_complete_cb)(void *content, void *req, libresteam_srv_browser_resp_t resp);
} libresteam_srv_browser_list_resp_ops_t;

typedef struct libresteam_srv_browser_list_resp
{
    const libresteam_srv_browser_list_resp_ops_t *ops;
    void *content;
} libresteam_srv_browser_list_resp_t;

typedef struct libresteam_srv_browser_ping_resp_ops
{
    void (*srv_responded_cb)(void *content, const libresteam_srv_browser_item_t *item);
    void (*srv_failed_cb)(void *content);
} libresteam_srv_browser_ping_resp_ops_t;

typedef struct libresteam_srv_browser_ping_resp
{
    const libresteam_srv_browser_ping_resp_ops_t *ops;
    void *content;
} libresteam_srv_browser_ping_resp_t;

typedef struct libresteam_srv_browser_player_resp_ops
{
    void (*player_add_cb)(void *content, const char *name, uint32_t score, double time_played);
    void (*player_failed_cb)(void *content);
    void (*player_refresh_complete_cb)(void *content);
} libresteam_srv_browser_player_resp_ops_t;

typedef struct libresteam_srv_browser_player_resp
{
    const libresteam_srv_browser_player_resp_ops_t *ops;
    void *content;
} libresteam_srv_browser_player_resp_t;

typedef struct libresteam_srv_browser_rule_resp_ops
{
    void (*rule_responded_cb)(void *content, const char *rule, const char *val);
    void (*rule_failed_cb)(void *content);
    void (*rule_refresh_complete_cb)(void *content);
} libresteam_srv_browser_rule_resp_ops_t;

typedef struct libresteam_srv_browser_rule_resp
{
    const libresteam_srv_browser_rule_resp_ops_t *ops;
    void *content;
} libresteam_srv_browser_rule_resp_t;

typedef struct libresteam_srv_browser_ops
{
    uint32_t (*list_req)(void *ctx, libresteam_srv_browser_list_type_t type, uint32_t app, const libresteam_srv_browser_kv_t *const *filter, uint32_t filter_cunt, const libresteam_srv_browser_list_resp_t *resp, void **req_out);
    uint32_t (*req_release)(void *ctx, void *req);
    uint32_t (*srv_details_get)(void *ctx, void *req, uint32_t srv_idx, const libresteam_srv_browser_item_t **out);
    uint32_t (*query_cancel)(void *ctx, void *req);
    uint32_t (*query_refresh)(void *ctx, void *req);
    uint32_t (*refreshing_get)(void *ctx, void *req, uint8_t *out);
    uint32_t (*srv_cunt_get)(void *ctx, void *req, uint32_t *out);
    uint32_t (*srv_refresh)(void *ctx, void *req, uint32_t srv_idx);
    uint32_t (*ping_req)(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_ping_resp_t *resp, uint32_t *query_out);
    uint32_t (*player_details_req)(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_player_resp_t *resp, uint32_t *query_out);
    uint32_t (*rule_req)(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_rule_resp_t *resp, uint32_t *query_out);
    uint32_t (*srv_query_cancel)(void *ctx, uint32_t query);
    uint32_t (*favorite_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*favorite_get)(void *ctx, uint32_t idx, uint32_t *app_out, uint32_t *ip_out, uint16_t *conn_port_out, uint16_t *query_port_out, uint32_t *flags_out, uint32_t *time_last_played_out);
    uint32_t (*favorite_add)(void *ctx, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint32_t time_last_played, uint32_t *idx_out);
    uint32_t (*favorite_remove)(void *ctx, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint8_t *removed_out);
} libresteam_srv_browser_ops_t;

#endif
