#ifndef LIBRESTEAM_BACKEND_H
#define LIBRESTEAM_BACKEND_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/apps/ops.h>
#include <libresteam/cb/ops.h>
#include <libresteam/cloud/ops.h>
#include <libresteam/friends/ops.h>
#include <libresteam/inventory/ops.h>
#include <libresteam/lobby/ops.h>
#include <libresteam/music/ops.h>
#include <libresteam/net_msgs/ops.h>
#include <libresteam/net_socks/ops.h>
#include <libresteam/net_utils/ops.h>
#include <libresteam/remote_play/ops.h>
#include <libresteam/srv_browser/ops.h>
#include <libresteam/timeline/ops.h>
#include <libresteam/user/ops.h>
#include <libresteam/user_stats/ops.h>
#include <libresteam/utils/ops.h>
#include <libresteam/wshop/ops.h>

typedef struct libresteam_backend_ops
{
    const libresteam_user_stats_ops_t *user_stats;
    const libresteam_friends_ops_t *friends;
    const libresteam_cloud_ops_t *cloud;
    const libresteam_lobby_ops_t *lobby;
    const libresteam_srv_browser_ops_t *srv_browser;
    const libresteam_cb_ops_t *cb;
    const libresteam_user_ops_t *user;
    const libresteam_apps_ops_t *apps;
    const libresteam_utils_ops_t *utils;
    const libresteam_music_ops_t *music;
    const libresteam_timeline_ops_t *timeline;
    const libresteam_net_utils_ops_t *net_utils;
    const libresteam_net_socks_ops_t *net_socks;
    const libresteam_net_msgs_ops_t *net_msgs;
    const libresteam_wshop_ops_t *wshop;
    const libresteam_inventory_ops_t *inventory;
    const libresteam_remote_play_ops_t *remote_play;
} libresteam_backend_ops_t;

typedef struct libresteam_backend
{
    const libresteam_backend_ops_t *ops;
    void *ctx;
} libresteam_backend_t;

typedef struct libresteam_backend_set
{
    libresteam_backend_t *backend;
    uint32_t backend_cunt;
    uint32_t primary_idx;
} libresteam_backend_set_t;

LDG_EXPORT uint32_t libresteam_backend_set_init(libresteam_backend_set_t *set, libresteam_backend_t *backend, uint32_t backend_cunt, uint32_t primary_idx);

#endif
