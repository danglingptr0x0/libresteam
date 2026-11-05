#ifndef LIBRESTEAM_SRV_BROWSER_H
#define LIBRESTEAM_SRV_BROWSER_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/srv_browser/ops.h>

LDG_EXPORT uint32_t libresteam_srv_browser_list_req(const libresteam_backend_set_t *set, libresteam_srv_browser_list_type_t type, uint32_t app, const libresteam_srv_browser_kv_t *const *filter, uint32_t filter_cunt, const libresteam_srv_browser_list_resp_t *resp, void **req_out);
LDG_EXPORT uint32_t libresteam_srv_browser_req_release(const libresteam_backend_set_t *set, void *req);
LDG_EXPORT uint32_t libresteam_srv_browser_srv_details_get(const libresteam_backend_set_t *set, void *req, uint32_t srv_idx, const libresteam_srv_browser_item_t **out);
LDG_EXPORT uint32_t libresteam_srv_browser_query_cancel(const libresteam_backend_set_t *set, void *req);
LDG_EXPORT uint32_t libresteam_srv_browser_query_refresh(const libresteam_backend_set_t *set, void *req);
LDG_EXPORT uint32_t libresteam_srv_browser_refreshing_get(const libresteam_backend_set_t *set, void *req, uint8_t *out);
LDG_EXPORT uint32_t libresteam_srv_browser_srv_cunt_get(const libresteam_backend_set_t *set, void *req, uint32_t *out);
LDG_EXPORT uint32_t libresteam_srv_browser_srv_refresh(const libresteam_backend_set_t *set, void *req, uint32_t srv_idx);
LDG_EXPORT uint32_t libresteam_srv_browser_ping_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_ping_resp_t *resp, uint32_t *query_out);
LDG_EXPORT uint32_t libresteam_srv_browser_player_details_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_player_resp_t *resp, uint32_t *query_out);
LDG_EXPORT uint32_t libresteam_srv_browser_rule_req(const libresteam_backend_set_t *set, uint32_t ip, uint16_t port, const libresteam_srv_browser_rule_resp_t *resp, uint32_t *query_out);
LDG_EXPORT uint32_t libresteam_srv_browser_srv_query_cancel(const libresteam_backend_set_t *set, uint32_t query);
LDG_EXPORT uint32_t libresteam_srv_browser_favorite_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_srv_browser_favorite_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *app_out, uint32_t *ip_out, uint16_t *conn_port_out, uint16_t *query_port_out, uint32_t *flags_out, uint32_t *time_last_played_out);
LDG_EXPORT uint32_t libresteam_srv_browser_favorite_add(const libresteam_backend_set_t *set, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint32_t time_last_played, uint32_t *idx_out);
LDG_EXPORT uint32_t libresteam_srv_browser_favorite_remove(const libresteam_backend_set_t *set, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint8_t *removed_out);

#endif
