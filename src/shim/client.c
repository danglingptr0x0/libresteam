#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/shim/shim.h>
#include <libresteam/utils/utils.h>

#define LIBRESTEAM_SHIM_CLIENT_VERSION "SteamClient020"
#define LIBRESTEAM_SHIM_CLIENT_METHOD_CUNT 42U
#define LIBRESTEAM_SHIM_CLIENT_019_VERSION "SteamClient019"
#define LIBRESTEAM_SHIM_CLIENT_019_METHOD_CUNT 41U
#define LIBRESTEAM_SHIM_CLIENT_PIPE_CREATE 0U
#define LIBRESTEAM_SHIM_CLIENT_PIPE_RELEASE 1U
#define LIBRESTEAM_SHIM_CLIENT_GLOB_USER_CONN 2U
#define LIBRESTEAM_SHIM_CLIENT_LOCAL_USER_CREATE 3U
#define LIBRESTEAM_SHIM_CLIENT_USER_RELEASE 4U
#define LIBRESTEAM_SHIM_CLIENT_USER_GET 5U
#define LIBRESTEAM_SHIM_CLIENT_GAME_SRV_GET 6U
#define LIBRESTEAM_SHIM_CLIENT_LOCAL_IP_BINDING_SET 7U
#define LIBRESTEAM_SHIM_CLIENT_FRIENDS_GET 8U
#define LIBRESTEAM_SHIM_CLIENT_UTILS_GET 9U
#define LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_GET 10U
#define LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_SRVS_GET 11U
#define LIBRESTEAM_SHIM_CLIENT_GENERIC_GET 12U
#define LIBRESTEAM_SHIM_CLIENT_USER_STATS_GET 13U
#define LIBRESTEAM_SHIM_CLIENT_GAME_SRV_STATS_GET 14U
#define LIBRESTEAM_SHIM_CLIENT_APPS_GET 15U
#define LIBRESTEAM_SHIM_CLIENT_NETWORKING_GET 16U
#define LIBRESTEAM_SHIM_CLIENT_REMOTE_STORAGE_GET 17U
#define LIBRESTEAM_SHIM_CLIENT_SCREENSHOTS_GET 18U
#define LIBRESTEAM_SHIM_CLIENT_GAME_SEARCH_GET 19U
#define LIBRESTEAM_SHIM_CLIENT_FRAME_RUN 20U
#define LIBRESTEAM_SHIM_CLIENT_IPC_CALL_CUNT_GET 21U
#define LIBRESTEAM_SHIM_CLIENT_WARNING_HOOK_SET 22U
#define LIBRESTEAM_SHIM_CLIENT_PIPES_CLOSED_SHUTDOWN 23U
#define LIBRESTEAM_SHIM_CLIENT_HTTP_GET 24U
#define LIBRESTEAM_SHIM_CLIENT_UNIFIED_MSGS_GET 25U
#define LIBRESTEAM_SHIM_CLIENT_CONTROLLER_GET 26U
#define LIBRESTEAM_SHIM_CLIENT_UGC_GET 27U
#define LIBRESTEAM_SHIM_CLIENT_APP_LIST_GET 28U
#define LIBRESTEAM_SHIM_CLIENT_MUSIC_GET 29U
#define LIBRESTEAM_SHIM_CLIENT_MUSIC_REMOTE_GET 30U
#define LIBRESTEAM_SHIM_CLIENT_HTML_SURFACE_GET 31U
#define LIBRESTEAM_SHIM_CLIENT_POST_API_RES_SET 32U
#define LIBRESTEAM_SHIM_CLIENT_POST_API_RES_REMOVE 33U
#define LIBRESTEAM_SHIM_CLIENT_CB_REGISTERED_CK_SET 34U
#define LIBRESTEAM_SHIM_CLIENT_INVENTORY_GET 35U
#define LIBRESTEAM_SHIM_CLIENT_VIDEO_GET 36U
#define LIBRESTEAM_SHIM_CLIENT_PARENTAL_GET 37U
#define LIBRESTEAM_SHIM_CLIENT_INPUT_GET 38U
#define LIBRESTEAM_SHIM_CLIENT_PARTIES_GET 39U
#define LIBRESTEAM_SHIM_CLIENT_REMOTE_PLAY_GET 40U
#define LIBRESTEAM_SHIM_CLIENT_IFACES_DESTROY 41U
#define LIBRESTEAM_SHIM_CLIENT_NONE 0U
#define LIBRESTEAM_SHIM_CLIENT_STR_EMPTY ""
#define LIBRESTEAM_SHIM_CLIENT_STR(str) ((str) ? (str) : LIBRESTEAM_SHIM_CLIENT_STR_EMPTY)
#define LIBRESTEAM_SHIM_CLIENT_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint32_t libresteam_shim_client_handles_ck(uint32_t user, uint32_t pipe)
{
    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (user != LIBRESTEAM_SHIM_USER_HANDLE) { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static void* libresteam_shim_client_iface_resolve(void *self, uint32_t idx, uint32_t res, const char *version, uint32_t user, uint32_t pipe)
{
    libresteam_shim_iface_t *iface = (libresteam_shim_iface_t *)self;
    void *found = 0x0;

    if (res == LDG_ERR_AOK && !version) { res = LDG_ERR_FUNC_ARG_NULL; }

    if (res == LDG_ERR_AOK) { res = libresteam_shim_iface_find((libresteam_shim_ctx_t *)iface->ctx, version, &found); }

    if (res != LDG_ERR_AOK) { found = 0x0; }

    if (libresteam_shim_arg_log(self, idx, res, "user: %u32; pipe: %u32; version: %s", user, pipe, LIBRESTEAM_SHIM_CLIENT_STR(version)) != LDG_ERR_AOK) { return found; }

    return found;
}

static void* libresteam_shim_client_user_iface_get(void *self, uint32_t idx, uint32_t user, uint32_t pipe, const char *version)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, idx, &set) != LDG_ERR_AOK) { return 0x0; }

    return libresteam_shim_client_iface_resolve(self, idx, libresteam_shim_client_handles_ck(user, pipe), version, user, pipe);
}

static void* libresteam_shim_client_game_srv_iface_get(void *self, uint32_t idx, uint32_t user, uint32_t pipe, const char *version)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, idx, &set) != LDG_ERR_AOK) { return 0x0; }

    if (libresteam_shim_arg_log(self, idx, LDG_ERR_UNSUPPORTED, "user: %u32; pipe: %u32; version: %s", user, pipe, LIBRESTEAM_SHIM_CLIENT_STR(version)) != LDG_ERR_AOK) { return 0x0; }

    return 0x0;
}

static void* libresteam_shim_client_user_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_USER_GET, user, pipe, version);
}

static void* libresteam_shim_client_friends_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_FRIENDS_GET, user, pipe, version);
}

static void* libresteam_shim_client_matchmaking_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_GET, user, pipe, version);
}

static void* libresteam_shim_client_matchmaking_srvs_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_SRVS_GET, user, pipe, version);
}

static void* libresteam_shim_client_generic_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_GENERIC_GET, user, pipe, version);
}

static void* libresteam_shim_client_user_stats_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_USER_STATS_GET, user, pipe, version);
}

static void* libresteam_shim_client_apps_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_APPS_GET, user, pipe, version);
}

static void* libresteam_shim_client_networking_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_NETWORKING_GET, user, pipe, version);
}

static void* libresteam_shim_client_remote_storage_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_REMOTE_STORAGE_GET, user, pipe, version);
}

static void* libresteam_shim_client_screenshots_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_SCREENSHOTS_GET, user, pipe, version);
}

static void* libresteam_shim_client_game_search_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_GAME_SEARCH_GET, user, pipe, version);
}

static void* libresteam_shim_client_http_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_HTTP_GET, user, pipe, version);
}

static void* libresteam_shim_client_unified_msgs_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_UNIFIED_MSGS_GET, user, pipe, version);
}

static void* libresteam_shim_client_controller_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_CONTROLLER_GET, user, pipe, version);
}

static void* libresteam_shim_client_ugc_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_UGC_GET, user, pipe, version);
}

static void* libresteam_shim_client_app_list_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_APP_LIST_GET, user, pipe, version);
}

static void* libresteam_shim_client_music_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_MUSIC_GET, user, pipe, version);
}

static void* libresteam_shim_client_music_remote_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_MUSIC_REMOTE_GET, user, pipe, version);
}

static void* libresteam_shim_client_html_surface_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_HTML_SURFACE_GET, user, pipe, version);
}

static void* libresteam_shim_client_inventory_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_INVENTORY_GET, user, pipe, version);
}

static void* libresteam_shim_client_video_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_VIDEO_GET, user, pipe, version);
}

static void* libresteam_shim_client_parental_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_PARENTAL_GET, user, pipe, version);
}

static void* libresteam_shim_client_input_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_INPUT_GET, user, pipe, version);
}

static void* libresteam_shim_client_parties_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_PARTIES_GET, user, pipe, version);
}

static void* libresteam_shim_client_remote_play_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_user_iface_get(self, LIBRESTEAM_SHIM_CLIENT_REMOTE_PLAY_GET, user, pipe, version);
}

static void* libresteam_shim_client_game_srv_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_game_srv_iface_get(self, LIBRESTEAM_SHIM_CLIENT_GAME_SRV_GET, user, pipe, version);
}

static void* libresteam_shim_client_game_srv_stats_get(void *self, uint32_t user, uint32_t pipe, const char *version)
{
    if (!self) { return 0x0; }

    return libresteam_shim_client_game_srv_iface_get(self, LIBRESTEAM_SHIM_CLIENT_GAME_SRV_STATS_GET, user, pipe, version);
}

static void* libresteam_shim_client_utils_get(void *self, uint32_t pipe, const char *version)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_UTILS_GET, &set) != LDG_ERR_AOK) { return 0x0; }

    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { res = LDG_ERR_FUNC_ARG_INVALID; }

    return libresteam_shim_client_iface_resolve(self, LIBRESTEAM_SHIM_CLIENT_UTILS_GET, res, version, LIBRESTEAM_SHIM_USER_HANDLE, pipe);
}

static uint32_t libresteam_shim_client_pipe_create(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_PIPE_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_PIPE_CREATE, LDG_ERR_AOK, "pipe: %u32", (uint32_t)LIBRESTEAM_SHIM_PIPE_HANDLE) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_PIPE_HANDLE; }

    return LIBRESTEAM_SHIM_PIPE_HANDLE;
}

static uint8_t libresteam_shim_client_pipe_release(void *self, uint32_t pipe)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_PIPE_RELEASE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { res = LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_PIPE_RELEASE, res, "pipe: %u32", pipe) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_client_glob_user_conn(void *self, uint32_t pipe)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t user = LIBRESTEAM_SHIM_USER_HANDLE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_GLOB_USER_CONN, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { res = LDG_ERR_FUNC_ARG_INVALID; user = LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_GLOB_USER_CONN, res, "pipe: %u32; user: %u32", pipe, user) != LDG_ERR_AOK) { return user; }

    return user;
}

static uint32_t libresteam_shim_client_local_user_create(void *self, uint32_t *pipe_out, uint32_t account_type)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_LOCAL_USER_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (pipe_out) { *pipe_out = LIBRESTEAM_SHIM_HANDLE_NONE; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_LOCAL_USER_CREATE, LDG_ERR_UNSUPPORTED, "account type: %u32", account_type) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_HANDLE_NONE; }

    return LIBRESTEAM_SHIM_HANDLE_NONE;
}

static void libresteam_shim_client_user_release(void *self, uint32_t pipe, uint32_t user)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_USER_RELEASE, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_USER_RELEASE, libresteam_shim_client_handles_ck(user, pipe), "pipe: %u32; user: %u32", pipe, user) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_client_local_ip_binding_set(void *self, const void *ip, uint16_t port)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_LOCAL_IP_BINDING_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_CLIENT_LOCAL_IP_BINDING_SET, LDG_ERR_UNSUPPORTED, "ip: %p; port: %u32", ip, (uint32_t)port) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_client_frame_run(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_FRAME_RUN, &set) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_client_ipc_call_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_IPC_CALL_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_CLIENT_NONE; }

    if (libresteam_utils_ipc_call_cunt_get(set, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_CLIENT_NONE; }

    return cunt;
}

static void libresteam_shim_client_warning_hook_set(void *self, void *hook)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_WARNING_HOOK_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_utils_warning_hook_set(set, hook) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_client_pipes_closed_shutdown(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_PIPES_CLOSED_SHUTDOWN, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

static void libresteam_shim_client_hook_note(void *self, uint32_t idx, void *fn)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, idx, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_arg_log(self, idx, LDG_ERR_AOK, "fn: %p", fn) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_client_post_api_res_set(void *self, void *fn)
{
    if (!self) { return; }

    libresteam_shim_client_hook_note(self, LIBRESTEAM_SHIM_CLIENT_POST_API_RES_SET, fn);
}

static void libresteam_shim_client_post_api_res_remove(void *self, void *fn)
{
    if (!self) { return; }

    libresteam_shim_client_hook_note(self, LIBRESTEAM_SHIM_CLIENT_POST_API_RES_REMOVE, fn);
}

static void libresteam_shim_client_cb_registered_ck_set(void *self, void *fn)
{
    if (!self) { return; }

    libresteam_shim_client_hook_note(self, LIBRESTEAM_SHIM_CLIENT_CB_REGISTERED_CK_SET, fn);
}

static void libresteam_shim_client_ifaces_destroy(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_CLIENT_IFACES_DESTROY, &set) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_client_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_CLIENT_PIPE_CREATE:
        { *out = "CreateSteamPipe";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_PIPE_RELEASE:
        { *out = "BReleaseSteamPipe";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_GLOB_USER_CONN:
        { *out = "ConnectToGlobalUser";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_LOCAL_USER_CREATE:
        { *out = "CreateLocalUser";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_USER_RELEASE:
        { *out = "ReleaseUser";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_USER_GET:
        { *out = "GetISteamUser";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_GAME_SRV_GET:
        { *out = "GetISteamGameServer";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_LOCAL_IP_BINDING_SET:
        { *out = "SetLocalIPBinding";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_FRIENDS_GET:
        { *out = "GetISteamFriends";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_UTILS_GET:
        { *out = "GetISteamUtils";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_GET:
        { *out = "GetISteamMatchmaking";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_SRVS_GET:
        { *out = "GetISteamMatchmakingServers";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_GENERIC_GET:
        { *out = "GetISteamGenericInterface";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_USER_STATS_GET:
        { *out = "GetISteamUserStats";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_GAME_SRV_STATS_GET:
        { *out = "GetISteamGameServerStats";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_APPS_GET:
        { *out = "GetISteamApps";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_NETWORKING_GET:
        { *out = "GetISteamNetworking";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_REMOTE_STORAGE_GET:
        { *out = "GetISteamRemoteStorage";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_SCREENSHOTS_GET:
        { *out = "GetISteamScreenshots";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_GAME_SEARCH_GET:
        { *out = "GetISteamGameSearch";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_FRAME_RUN:
        { *out = "RunFrame";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_IPC_CALL_CUNT_GET:
        { *out = "GetIPCCallCount";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_WARNING_HOOK_SET:
        { *out = "SetWarningMessageHook";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_PIPES_CLOSED_SHUTDOWN:
        { *out = "BShutdownIfAllPipesClosed";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_HTTP_GET:
        { *out = "GetISteamHTTP";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_UNIFIED_MSGS_GET:
        { *out = "DEPRECATED_GetISteamUnifiedMessages";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_CONTROLLER_GET:
        { *out = "GetISteamController";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_UGC_GET:
        { *out = "GetISteamUGC";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_APP_LIST_GET:
        { *out = "GetISteamAppList";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_MUSIC_GET:
        { *out = "GetISteamMusic";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_MUSIC_REMOTE_GET:
        { *out = "GetISteamMusicRemote";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_HTML_SURFACE_GET:
        { *out = "GetISteamHTMLSurface";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_POST_API_RES_SET:
        { *out = "DEPRECATED_Set_SteamAPI_CPostAPIResultInProcess";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_POST_API_RES_REMOVE:
        { *out = "DEPRECATED_Remove_SteamAPI_CPostAPIResultInProcess";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_CB_REGISTERED_CK_SET:
        { *out = "Set_SteamAPI_CCheckCallbackRegisteredInProcess";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_INVENTORY_GET:
        { *out = "GetISteamInventory";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_VIDEO_GET:
        { *out = "GetISteamVideo";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_PARENTAL_GET:
        { *out = "GetISteamParentalSettings";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_INPUT_GET:
        { *out = "GetISteamInput";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_PARTIES_GET:
        { *out = "GetISteamParties";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_REMOTE_PLAY_GET:
        { *out = "GetISteamRemotePlay";
          break;
        }
        case LIBRESTEAM_SHIM_CLIENT_IFACES_DESTROY:
        { *out = "DestroyAllInterfaces";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_client_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_CLIENT_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_client_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_CLIENT_PIPE_CREATE:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_pipe_create)
        case LIBRESTEAM_SHIM_CLIENT_PIPE_RELEASE:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_pipe_release)
        case LIBRESTEAM_SHIM_CLIENT_GLOB_USER_CONN:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_glob_user_conn)
        case LIBRESTEAM_SHIM_CLIENT_LOCAL_USER_CREATE:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_local_user_create)
        case LIBRESTEAM_SHIM_CLIENT_USER_RELEASE:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_user_release)
        case LIBRESTEAM_SHIM_CLIENT_USER_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_user_get)
        case LIBRESTEAM_SHIM_CLIENT_GAME_SRV_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_game_srv_get)
        case LIBRESTEAM_SHIM_CLIENT_LOCAL_IP_BINDING_SET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_local_ip_binding_set)
        case LIBRESTEAM_SHIM_CLIENT_FRIENDS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_friends_get)
        case LIBRESTEAM_SHIM_CLIENT_UTILS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_utils_get)
        case LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_matchmaking_get)
        case LIBRESTEAM_SHIM_CLIENT_MATCHMAKING_SRVS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_matchmaking_srvs_get)
        case LIBRESTEAM_SHIM_CLIENT_GENERIC_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_generic_get)
        case LIBRESTEAM_SHIM_CLIENT_USER_STATS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_user_stats_get)
        case LIBRESTEAM_SHIM_CLIENT_GAME_SRV_STATS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_game_srv_stats_get)
        case LIBRESTEAM_SHIM_CLIENT_APPS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_apps_get)
        case LIBRESTEAM_SHIM_CLIENT_NETWORKING_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_networking_get)
        case LIBRESTEAM_SHIM_CLIENT_REMOTE_STORAGE_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_remote_storage_get)
        case LIBRESTEAM_SHIM_CLIENT_SCREENSHOTS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_screenshots_get)
        case LIBRESTEAM_SHIM_CLIENT_GAME_SEARCH_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_game_search_get)
        case LIBRESTEAM_SHIM_CLIENT_FRAME_RUN:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_frame_run)
        case LIBRESTEAM_SHIM_CLIENT_IPC_CALL_CUNT_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_ipc_call_cunt_get)
        case LIBRESTEAM_SHIM_CLIENT_WARNING_HOOK_SET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_warning_hook_set)
        case LIBRESTEAM_SHIM_CLIENT_PIPES_CLOSED_SHUTDOWN:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_pipes_closed_shutdown)
        case LIBRESTEAM_SHIM_CLIENT_HTTP_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_http_get)
        case LIBRESTEAM_SHIM_CLIENT_UNIFIED_MSGS_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_unified_msgs_get)
        case LIBRESTEAM_SHIM_CLIENT_CONTROLLER_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_controller_get)
        case LIBRESTEAM_SHIM_CLIENT_UGC_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_ugc_get)
        case LIBRESTEAM_SHIM_CLIENT_APP_LIST_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_app_list_get)
        case LIBRESTEAM_SHIM_CLIENT_MUSIC_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_music_get)
        case LIBRESTEAM_SHIM_CLIENT_MUSIC_REMOTE_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_music_remote_get)
        case LIBRESTEAM_SHIM_CLIENT_HTML_SURFACE_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_html_surface_get)
        case LIBRESTEAM_SHIM_CLIENT_POST_API_RES_SET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_post_api_res_set)
        case LIBRESTEAM_SHIM_CLIENT_POST_API_RES_REMOVE:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_post_api_res_remove)
        case LIBRESTEAM_SHIM_CLIENT_CB_REGISTERED_CK_SET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_cb_registered_ck_set)
        case LIBRESTEAM_SHIM_CLIENT_INVENTORY_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_inventory_get)
        case LIBRESTEAM_SHIM_CLIENT_VIDEO_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_video_get)
        case LIBRESTEAM_SHIM_CLIENT_PARENTAL_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_parental_get)
        case LIBRESTEAM_SHIM_CLIENT_INPUT_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_input_get)
        case LIBRESTEAM_SHIM_CLIENT_PARTIES_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_parties_get)
        case LIBRESTEAM_SHIM_CLIENT_REMOTE_PLAY_GET:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_remote_play_get)
        case LIBRESTEAM_SHIM_CLIENT_IFACES_DESTROY:
            LIBRESTEAM_SHIM_CLIENT_IMPL(libresteam_shim_client_ifaces_destroy)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_client_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_CLIENT_VERSION;
    out->name_get = libresteam_shim_client_name_get;
    out->kind_get = libresteam_shim_client_kind_get;
    out->impl_get = libresteam_shim_client_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_CLIENT_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_client_019_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_CLIENT_019_VERSION;
    out->name_get = libresteam_shim_client_name_get;
    out->kind_get = libresteam_shim_client_kind_get;
    out->impl_get = libresteam_shim_client_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_CLIENT_019_METHOD_CUNT;

    return LDG_ERR_AOK;
}
