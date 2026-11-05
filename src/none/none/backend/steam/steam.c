#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/io/path.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/secure.h>
#include <dangling/sys/lib.h>

#include <libresteam/backend/backend.h>
#include <libresteam/backend/steam/steam.h>
#include <libresteam/cb/msg.h>
#include <libresteam/cb/ops.h>

static void steam_name_id_fill(uint32_t *name_id, uint32_t cunt)
{
    uint32_t idx = 0;

    for (idx = 0; idx < cunt; idx++) { name_id[idx] = idx; }
}

static void steam_desc_fill(libresteam_backend_steam_t *steam)
{
    steam_name_id_fill(steam->client_name_id, LIBRESTEAM_BACKEND_STEAM_CLIENT_METHOD_CUNT);
    steam_name_id_fill(steam->user_stats_name_id, LIBRESTEAM_BACKEND_STEAM_USER_STATS_METHOD_CUNT);
    steam_name_id_fill(steam->friends_name_id, LIBRESTEAM_BACKEND_STEAM_FRIENDS_METHOD_CUNT);
    steam_name_id_fill(steam->cloud_name_id, LIBRESTEAM_BACKEND_STEAM_CLOUD_METHOD_CUNT);
    steam_name_id_fill(steam->user_name_id, LIBRESTEAM_BACKEND_STEAM_USER_METHOD_CUNT);
    steam_name_id_fill(steam->apps_name_id, LIBRESTEAM_BACKEND_STEAM_APPS_METHOD_CUNT);
    steam_name_id_fill(steam->utils_name_id, LIBRESTEAM_BACKEND_STEAM_UTILS_METHOD_CUNT);
    steam_name_id_fill(steam->music_name_id, LIBRESTEAM_BACKEND_STEAM_MUSIC_METHOD_CUNT);
    steam_name_id_fill(steam->timeline_name_id, LIBRESTEAM_BACKEND_STEAM_TIMELINE_METHOD_CUNT);
    steam_name_id_fill(steam->matchmaking_name_id, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_METHOD_CUNT);
    steam_name_id_fill(steam->matchmaking_srvs_name_id, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_METHOD_CUNT);
    steam_name_id_fill(steam->net_utils_name_id, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_METHOD_CUNT);
    steam_name_id_fill(steam->net_socks_name_id, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_METHOD_CUNT);
    steam_name_id_fill(steam->fake_udp_port_name_id, LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_METHOD_CUNT);
    steam_name_id_fill(steam->net_msgs_name_id, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_METHOD_CUNT);
    steam_name_id_fill(steam->wshop_name_id, LIBRESTEAM_BACKEND_STEAM_WSHOP_METHOD_CUNT);
    steam_name_id_fill(steam->inventory_name_id, LIBRESTEAM_BACKEND_STEAM_INVENTORY_METHOD_CUNT);
    steam_name_id_fill(steam->remote_play_name_id, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_METHOD_CUNT);

    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_FP32_GET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_INT_GET];
    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_FP32_SET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_INT_SET];
    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_STAT_FP32_GET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_STAT_INT_GET];
    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_FP64_GET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_INT_GET];
    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_HISTORY_INT_GET];
    steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_PROGRESS_LIMIT_FP32_GET] = steam->user_stats_name_id[LIBRESTEAM_BACKEND_STEAM_USER_STATS_PROGRESS_LIMIT_INT_GET];
    steam->wshop_name_id[LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ALL_CURSOR_CREATE] = steam->wshop_name_id[LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ALL_PAGE_CREATE];
    steam->wshop_name_id[LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_FIRST_KV_TAG_GET] = steam->wshop_name_id[LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_KV_TAG_GET];
    steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_BOOL_SET] = steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_STR_SET];
    steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_INT_SET] = steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_STR_SET];
    steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_FP32_SET] = steam->inventory_name_id[LIBRESTEAM_BACKEND_STEAM_INVENTORY_PROPERTY_STR_SET];

    steam->client_desc.name_id = steam->client_name_id;
    steam->client_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_CLIENT_METHOD_CUNT;
    steam->client_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->user_stats_desc.name_id = steam->user_stats_name_id;
    steam->user_stats_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_USER_STATS_METHOD_CUNT;
    steam->user_stats_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->friends_desc.name_id = steam->friends_name_id;
    steam->friends_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_FRIENDS_METHOD_CUNT;
    steam->friends_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->cloud_desc.name_id = steam->cloud_name_id;
    steam->cloud_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_CLOUD_METHOD_CUNT;
    steam->cloud_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->user_desc.name_id = steam->user_name_id;
    steam->user_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_USER_METHOD_CUNT;
    steam->user_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->apps_desc.name_id = steam->apps_name_id;
    steam->apps_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_APPS_METHOD_CUNT;
    steam->apps_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->utils_desc.name_id = steam->utils_name_id;
    steam->utils_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_UTILS_METHOD_CUNT;
    steam->utils_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->music_desc.name_id = steam->music_name_id;
    steam->music_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_MUSIC_METHOD_CUNT;
    steam->music_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->timeline_desc.name_id = steam->timeline_name_id;
    steam->timeline_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_TIMELINE_METHOD_CUNT;
    steam->timeline_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->matchmaking_desc.name_id = steam->matchmaking_name_id;
    steam->matchmaking_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_METHOD_CUNT;
    steam->matchmaking_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->matchmaking_srvs_desc.name_id = steam->matchmaking_srvs_name_id;
    steam->matchmaking_srvs_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_METHOD_CUNT;
    steam->matchmaking_srvs_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->net_utils_desc.name_id = steam->net_utils_name_id;
    steam->net_utils_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_NET_UTILS_METHOD_CUNT;
    steam->net_utils_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->net_socks_desc.name_id = steam->net_socks_name_id;
    steam->net_socks_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_METHOD_CUNT;
    steam->net_socks_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->fake_udp_port_desc.name_id = steam->fake_udp_port_name_id;
    steam->fake_udp_port_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_METHOD_CUNT;
    steam->fake_udp_port_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->net_msgs_desc.name_id = steam->net_msgs_name_id;
    steam->net_msgs_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_NET_MSGS_METHOD_CUNT;
    steam->net_msgs_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->wshop_desc.name_id = steam->wshop_name_id;
    steam->wshop_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_WSHOP_METHOD_CUNT;
    steam->wshop_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->inventory_desc.name_id = steam->inventory_name_id;
    steam->inventory_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_INVENTORY_METHOD_CUNT;
    steam->inventory_desc.dtor_idx = LDG_CPP_DTOR_NONE;

    steam->remote_play_desc.name_id = steam->remote_play_name_id;
    steam->remote_play_desc.method_cunt = LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_METHOD_CUNT;
    steam->remote_play_desc.dtor_idx = LDG_CPP_DTOR_NONE;
}

static uint32_t steam_client_path_resolve(libresteam_backend_steam_t *steam, const libresteam_backend_steam_conf_t *conf)
{
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (conf->client_path) { return ldg_io_path_normalize(conf->client_path, steam->client_path, LDG_IO_PATH_MAX); }

    while (res == LDG_ERR_AOK)
    {
        res = libresteam_backend_steam_path_default_get(idx, steam->client_path, LDG_IO_PATH_MAX);
        if (res == LDG_ERR_AOK && ldg_io_path_exists_is(steam->client_path) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        idx++;
    }

    steam->client_path[0] = LDG_STR_TERM;

    return res;
}

static uint32_t steam_sym_get(libresteam_backend_steam_t *steam)
{
    ldg_sys_lib_func_t func = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_sys_lib_sym_get(steam->lib, LIBRESTEAM_BACKEND_STEAM_SYM_CREATE_INTERFACE, &func);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(&steam->create_interface, &func, sizeof(ldg_sys_lib_func_t));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_sys_lib_sym_get(steam->lib, LIBRESTEAM_BACKEND_STEAM_SYM_CB_GET, &func);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(&steam->cb_get, &func, sizeof(ldg_sys_lib_func_t));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_sys_lib_sym_get(steam->lib, LIBRESTEAM_BACKEND_STEAM_SYM_CB_FREE, &func);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(&steam->cb_free, &func, sizeof(ldg_sys_lib_func_t));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_sys_lib_sym_get(steam->lib, LIBRESTEAM_BACKEND_STEAM_SYM_CALL_RES_GET, &func);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_mem_secure_cpy(&steam->call_res_get, &func, sizeof(ldg_sys_lib_func_t));
}

static uint32_t steam_teardown(libresteam_backend_steam_t *steam)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t first_res = LDG_ERR_AOK;
    uint8_t released = LDG_TRUTH_FALSE;

    if (steam->client && steam->user != 0)
    {
        cunt = 0;
        arg[cunt] = (ldg_cpp_arg_t) { .val = steam->pipe, .kind = LDG_CPP_ARG_KIND_INT };
        cunt++;
        arg[cunt] = (ldg_cpp_arg_t) { .val = steam->user, .kind = LDG_CPP_ARG_KIND_INT };
        cunt++;
        res = ldg_cpp_slot_get(&steam->client_desc, LIBRESTEAM_BACKEND_STEAM_CLIENT_USER_RELEASE, &slot);
        if (res == LDG_ERR_AOK) { res = ldg_cpp_void_call(steam->client, slot, arg, cunt); }

        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }

        steam->user = 0;
    }

    if (steam->client && steam->pipe != 0)
    {
        cunt = 0;
        arg[cunt] = (ldg_cpp_arg_t) { .val = steam->pipe, .kind = LDG_CPP_ARG_KIND_INT };
        cunt++;
        res = ldg_cpp_slot_get(&steam->client_desc, LIBRESTEAM_BACKEND_STEAM_CLIENT_PIPE_RELEASE, &slot);
        if (res == LDG_ERR_AOK) { res = ldg_cpp_uint8_call(steam->client, slot, arg, cunt, &released); }

        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }

        steam->pipe = 0;
    }

    steam->client = 0x0;
    steam->user_stats = 0x0;
    steam->friends = 0x0;
    steam->client_friends = 0x0;
    steam->apps = 0x0;
    steam->utils = 0x0;
    steam->music = 0x0;
    steam->timeline = 0x0;
    steam->matchmaking = 0x0;
    steam->matchmaking_srvs = 0x0;
    steam->net_utils = 0x0;
    steam->net_socks = 0x0;
    steam->net_msgs = 0x0;
    steam->wshop = 0x0;
    steam->inventory = 0x0;
    steam->remote_play = 0x0;
    steam->cloud = 0x0;
    steam->user_iface = 0x0;
    steam->create_interface = 0x0;
    steam->cb_get = 0x0;
    steam->cb_free = 0x0;
    steam->call_res_get = 0x0;

    if (steam->lib)
    {
        res = ldg_sys_lib_close(steam->lib);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }

        steam->lib = 0x0;
    }

    return first_res;
}

static uint32_t steam_init_fail(libresteam_backend_steam_t *steam, uint32_t err)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_teardown(steam);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "backend_steam: teardown after init failure failed"); }

    return err;
}

static uint32_t steam_iface_get(libresteam_backend_steam_t *steam, uint32_t method_idx, const char *version, void **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t iface = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0x0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = steam->user, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam->pipe, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = version, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = ldg_cpp_slot_get(&steam->client_desc, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->client, slot, arg, cunt, &iface);
    if (res != LDG_ERR_AOK) { return res; }

    if (iface == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (void *)(uintptr_t)iface;

    return LDG_ERR_AOK;
}

static uint32_t steam_pipe_iface_get(libresteam_backend_steam_t *steam, uint32_t method_idx, const char *version, void **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t iface = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0x0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = steam->pipe, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = version, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = ldg_cpp_slot_get(&steam->client_desc, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->client, slot, arg, cunt, &iface);
    if (res != LDG_ERR_AOK) { return res; }

    if (iface == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (void *)(uintptr_t)iface;

    return LDG_ERR_AOK;
}

static uint32_t steam_cb_pump(void *ctx, const libresteam_cb_sink_t *sink)
{
    libresteam_backend_steam_t *steam = 0x0;
    libresteam_cb_msg_t msg = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!ctx || !sink)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->cb_get || !steam->cb_free)) { return LDG_ERR_NOT_INIT; }

    while (steam->cb_get(steam->pipe, &msg) == LDG_TRUTH_TRUE)
    {
        sink->ops->msg_cb(sink->content, msg.user, msg.cb_id, msg.param, msg.param_size);
        steam->cb_free(steam->pipe);
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_call_res_get(void *ctx, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out)
{
    libresteam_backend_steam_t *steam = 0x0;

    if (LDG_UNLIKELY(!ctx || !buff || !failed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->call_res_get)) { return LDG_ERR_NOT_INIT; }

    if (steam->call_res_get(steam->pipe, call, buff, cap, cb_id_expected, failed_out) != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_cb_next_get(void *ctx, libresteam_cb_msg_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;

    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->cb_get)) { return LDG_ERR_NOT_INIT; }

    if (steam->cb_get(steam->pipe, out) != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_cb_last_free(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->cb_free)) { return LDG_ERR_NOT_INIT; }

    steam->cb_free(steam->pipe);

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_init(libresteam_backend_steam_t *steam, const libresteam_backend_steam_conf_t *conf, libresteam_backend_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t create_code = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->ops = 0x0;
    out->ctx = 0x0;

    if (LDG_UNLIKELY(!steam || !conf)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(steam, sizeof(libresteam_backend_steam_t));
    if (res != LDG_ERR_AOK) { return res; }

    steam_desc_fill(steam);

    res = steam_client_path_resolve(steam, conf);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_sys_lib_open(steam->client_path, &steam->lib);
    if (res != LDG_ERR_AOK) { return res; }

    res = steam_sym_get(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    steam->client = steam->create_interface(LIBRESTEAM_BACKEND_STEAM_CLIENT_VERSION, &create_code);
    if (!steam->client) { return steam_init_fail(steam, LDG_ERR_NOT_FOUND); }

    res = ldg_cpp_slot_get(&steam->client_desc, LIBRESTEAM_BACKEND_STEAM_CLIENT_PIPE_CREATE, &slot);
    if (res == LDG_ERR_AOK) { res = ldg_cpp_uint32_call(steam->client, slot, arg, cunt, &steam->pipe); }

    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    if (steam->pipe == 0) { return steam_init_fail(steam, LDG_ERR_NET_CONN); }

    cunt = 0;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam->pipe, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    res = ldg_cpp_slot_get(&steam->client_desc, LIBRESTEAM_BACKEND_STEAM_CLIENT_GLOB_USER_CONN, &slot);
    if (res == LDG_ERR_AOK) { res = ldg_cpp_uint32_call(steam->client, slot, arg, cunt, &steam->user); }

    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    if (steam->user == 0) { return steam_init_fail(steam, LDG_ERR_NET_CONN); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_USER_STATS_GET, LIBRESTEAM_BACKEND_STEAM_USER_STATS_VERSION, &steam->user_stats);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_FRIENDS_GET, LIBRESTEAM_BACKEND_STEAM_FRIENDS_VERSION, &steam->friends);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = ldg_mem_secure_cpy(&steam->client_friends, (const uint8_t *)steam->friends + LIBRESTEAM_BACKEND_STEAM_FRIENDS_CLIENT_OFF, (uint64_t)sizeof(steam->client_friends));
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_CLOUD_GET, LIBRESTEAM_BACKEND_STEAM_CLOUD_VERSION, &steam->cloud);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_APPS_GET, LIBRESTEAM_BACKEND_STEAM_APPS_VERSION, &steam->apps);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_pipe_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_UTILS_GET, LIBRESTEAM_BACKEND_STEAM_UTILS_VERSION, &steam->utils);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_MUSIC_GET, LIBRESTEAM_BACKEND_STEAM_MUSIC_VERSION, &steam->music);
    if (res != LDG_ERR_AOK) { steam->music = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_GENERIC_GET, LIBRESTEAM_BACKEND_STEAM_TIMELINE_VERSION, &steam->timeline);
    if (res != LDG_ERR_AOK) { steam->timeline = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_MATCHMAKING_GET, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_VERSION, &steam->matchmaking);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_MATCHMAKING_SRVS_GET, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_VERSION, &steam->matchmaking_srvs);
    if (res != LDG_ERR_AOK) { steam->matchmaking_srvs = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_GENERIC_GET, LIBRESTEAM_BACKEND_STEAM_NET_UTILS_VERSION, &steam->net_utils);
    if (res != LDG_ERR_AOK) { steam->net_utils = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_GENERIC_GET, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_VERSION, &steam->net_socks);
    if (res != LDG_ERR_AOK) { steam->net_socks = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_GENERIC_GET, LIBRESTEAM_BACKEND_STEAM_NET_MSGS_VERSION, &steam->net_msgs);
    if (res != LDG_ERR_AOK) { steam->net_msgs = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_WSHOP_GET, LIBRESTEAM_BACKEND_STEAM_WSHOP_VERSION, &steam->wshop);
    if (res != LDG_ERR_AOK) { steam->wshop = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_GENERIC_GET, LIBRESTEAM_BACKEND_STEAM_INVENTORY_VERSION, &steam->inventory);
    if (res != LDG_ERR_AOK) { steam->inventory = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_REMOTE_PLAY_GET, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_VERSION, &steam->remote_play);
    if (res != LDG_ERR_AOK) { steam->remote_play = 0x0; }

    res = steam_iface_get(steam, LIBRESTEAM_BACKEND_STEAM_CLIENT_USER_GET, LIBRESTEAM_BACKEND_STEAM_USER_VERSION, &steam->user_iface);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_user_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_apps_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_utils_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_music_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_lobby_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_srv_browser_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_net_utils_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_net_socks_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_net_msgs_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_wshop_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_inventory_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_timeline_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_user_stats_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_user_stats_ext_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_friends_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_friends_ext_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_cloud_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    res = libresteam_backend_steam_remote_play_ops_fill(steam);
    if (res != LDG_ERR_AOK) { return steam_init_fail(steam, res); }

    steam->cb_ops.pump = steam_cb_pump;
    steam->cb_ops.call_res_get = steam_call_res_get;
    steam->cb_ops.next_get = steam_cb_next_get;
    steam->cb_ops.last_free = steam_cb_last_free;
    steam->ops.cb = &steam->cb_ops;

    out->ops = &steam->ops;
    out->ctx = steam;

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_shutdown(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_teardown(steam);
}
