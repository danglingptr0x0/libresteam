#ifndef LIBRESTEAM_SHIM_H
#define LIBRESTEAM_SHIM_H

#include <stdint.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/io/path.h>

#include <libresteam/backend/backend.h>
#include <libresteam/steam/steam.h>
#include <libresteam/cb/ops.h>
#include <libresteam/shim/flat_gen.h>

#define LIBRESTEAM_SHIM_SLOT_MAX 96U
#define LIBRESTEAM_SHIM_CALL_RES_CAP 128U
#define LIBRESTEAM_SHIM_CALL_RES_BUFF_MAX 16384U
#define LIBRESTEAM_SHIM_ADDR_STR_MAX 48U
#define LIBRESTEAM_SHIM_CB_CALL_COMPLETED 703U
#define LIBRESTEAM_SHIM_SLOT_FIRST 0U
#define LIBRESTEAM_SHIM_SLOT_STEP 1U
#define LIBRESTEAM_SHIM_SLOT_SECOND (LIBRESTEAM_SHIM_SLOT_FIRST + LIBRESTEAM_SHIM_SLOT_STEP)
#define LIBRESTEAM_SHIM_CB_CAP 256U
#define LIBRESTEAM_SHIM_LOG_PATH "libresteam.log"
#define LIBRESTEAM_SHIM_USER_HANDLE 1U
#define LIBRESTEAM_SHIM_PIPE_HANDLE 1U
#define LIBRESTEAM_SHIM_HANDLE_NONE 0U
#define LIBRESTEAM_SHIM_INSTANCE_STEP 1U
#define LIBRESTEAM_SHIM_HIT_STEP 1U
#define LIBRESTEAM_SHIM_HIT_FIRST 1U
#define LIBRESTEAM_SHIM_CB_FLAG_REG 0x01U
#define LIBRESTEAM_SHIM_CB_FLAG_GAME_SRV 0x02U
#define LIBRESTEAM_SHIM_CB_FLAGS_OFF 8U
#define LIBRESTEAM_SHIM_CB_ID_OFF 12U
#define LIBRESTEAM_SHIM_CB_RUN 0U
#define LIBRESTEAM_SHIM_CB_RUN_CALL 1U
#define LIBRESTEAM_SHIM_CB_SIZE_GET 2U
#define LIBRESTEAM_SHIM_CB_METHOD_CUNT 3U
#define LIBRESTEAM_SHIM_CB_RUN_ARG_CUNT 1U
#define LIBRESTEAM_SHIM_CTX_INIT_FN 0U
#define LIBRESTEAM_SHIM_CTX_INIT_CUNTR 1U
#define LIBRESTEAM_SHIM_CTX_INIT_PTR 2U
#define LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT 2U
#define LIBRESTEAM_SHIM_FAKE_UDP_PORT_MAX 8U
#define LIBRESTEAM_SHIM_FAKE_UDP_PORT_METHOD_CUNT 4U

typedef enum libresteam_shim_iface_id
{
    LIBRESTEAM_SHIM_IFACE_USER = 0,
    LIBRESTEAM_SHIM_IFACE_FRIENDS,
    LIBRESTEAM_SHIM_IFACE_USER_STATS,
    LIBRESTEAM_SHIM_IFACE_REMOTE_STORAGE,
    LIBRESTEAM_SHIM_IFACE_APPS,
    LIBRESTEAM_SHIM_IFACE_UTILS,
    LIBRESTEAM_SHIM_IFACE_MATCHMAKING,
    LIBRESTEAM_SHIM_IFACE_MATCHMAKING_SRVS,
    LIBRESTEAM_SHIM_IFACE_NETWORKING,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_SOCKS,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_UTILS,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_SOCKS_012,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_UTILS_004,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_MSGS,
    LIBRESTEAM_SHIM_IFACE_CLIENT,
    LIBRESTEAM_SHIM_IFACE_REMOTE_PLAY_001,
    LIBRESTEAM_SHIM_IFACE_REMOTE_PLAY_004,
    LIBRESTEAM_SHIM_IFACE_USER_020,
    LIBRESTEAM_SHIM_IFACE_CLIENT_019,
    LIBRESTEAM_SHIM_IFACE_USER_STATS_011,
    LIBRESTEAM_SHIM_IFACE_UTILS_009,
    LIBRESTEAM_SHIM_IFACE_NETWORKING_005,
    LIBRESTEAM_SHIM_IFACE_CONTROLLER_007,
    LIBRESTEAM_SHIM_IFACE_GEN_BASE,
    LIBRESTEAM_SHIM_IFACE_CUNT = LIBRESTEAM_SHIM_IFACE_GEN_BASE + LIBRESTEAM_SHIM_GEN_IFACE_CUNT
} libresteam_shim_iface_id_t;

typedef enum libresteam_shim_ret_kind
{
    LIBRESTEAM_SHIM_RET_INT = 0,
    LIBRESTEAM_SHIM_RET_STR,
    LIBRESTEAM_SHIM_RET_FP32,
    LIBRESTEAM_SHIM_RET_STRUCT
} libresteam_shim_ret_kind_t;

typedef uint32_t (*libresteam_shim_name_get_cb_t)(uint32_t idx, const char **out);
typedef uint32_t (*libresteam_shim_kind_get_cb_t)(uint32_t idx, uint32_t *out);
typedef uint32_t (*libresteam_shim_impl_get_cb_t)(uint32_t idx, void **out);

typedef struct libresteam_shim_iface_def
{
    const char *version;
    libresteam_shim_name_get_cb_t name_get;
    libresteam_shim_kind_get_cb_t kind_get;
    libresteam_shim_impl_get_cb_t impl_get;
    uint32_t method_cunt;
    uint8_t pudding[4];
} libresteam_shim_iface_def_t;

typedef struct libresteam_shim_flat_entry
{
    const char *version;
    const char *method;
    uint32_t def_idx;
    uint32_t slot;
} libresteam_shim_flat_entry_t;

typedef struct libresteam_shim_iface
{
    void **vtable;
    void *ctx;
    uint32_t id;
    uint8_t pudding[4];
} libresteam_shim_iface_t;

typedef struct libresteam_shim_fake_udp_port
{
    void **vtable;
    void *port;
    uint8_t in_use;
    uint8_t pudding[7];
} libresteam_shim_fake_udp_port_t;

typedef struct libresteam_shim_ctx
{
    libresteam_steam_t steam;
    libresteam_steam_conf_t steam_conf;
    libresteam_backend_t backend;
    libresteam_backend_set_t set;
    libresteam_cb_sink_ops_t sink_ops;
    libresteam_cb_sink_t sink;
    libresteam_shim_iface_t iface[LIBRESTEAM_SHIM_IFACE_CUNT];
    libresteam_shim_iface_def_t def[LIBRESTEAM_SHIM_IFACE_CUNT];
    ldg_cpp_class_desc_t desc[LIBRESTEAM_SHIM_IFACE_CUNT];
    void *vtable[LIBRESTEAM_SHIM_IFACE_CUNT][LIBRESTEAM_SHIM_SLOT_MAX];
    uint32_t name_id[LIBRESTEAM_SHIM_IFACE_CUNT][LIBRESTEAM_SHIM_SLOT_MAX];
    uint32_t hit[LIBRESTEAM_SHIM_IFACE_CUNT][LIBRESTEAM_SHIM_SLOT_MAX];
    void *cb_obj[LIBRESTEAM_SHIM_CB_CAP];
    ldg_cpp_class_desc_t cb_desc;
    libresteam_net_socks_sig_ops_t net_sig_ops;
    libresteam_net_socks_sig_recv_ops_t net_sig_recv_ops;
    ldg_cpp_class_desc_t net_sig_desc;
    ldg_cpp_class_desc_t net_sig_recv_desc;
    void *fake_udp_port_vtable[LIBRESTEAM_SHIM_FAKE_UDP_PORT_METHOD_CUNT];
    libresteam_shim_fake_udp_port_t fake_udp_port[LIBRESTEAM_SHIM_FAKE_UDP_PORT_MAX];
    uint32_t net_sig_name_id[LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT];
    uint32_t net_sig_recv_name_id[LIBRESTEAM_SHIM_NET_SIG_METHOD_CUNT];
    void *call_obj[LIBRESTEAM_SHIM_CALL_RES_CAP];
    uint64_t call_handle[LIBRESTEAM_SHIM_CALL_RES_CAP];
    uint8_t call_res_buff[LIBRESTEAM_SHIM_CALL_RES_BUFF_MAX];
    char install_path[LDG_IO_PATH_MAX];
    char addr_str[LIBRESTEAM_SHIM_ADDR_STR_MAX];
    uint64_t instance;
    uint32_t cb_name_id[LIBRESTEAM_SHIM_CB_METHOD_CUNT];
    uint32_t cb_cunt;
    uint32_t call_cunt;
    uint32_t cb_run_res;
    uint8_t steam_up;
    uint8_t manual_dispatch;
    uint8_t cb_run_seen;
    uint8_t pudding[5];
} libresteam_shim_ctx_t;

LDG_EXPORT uint8_t libresteam_shim_api_init(void);
LDG_EXPORT void libresteam_shim_api_shutdown(void);
LDG_EXPORT uint8_t libresteam_shim_api_app_restart_is(uint32_t app_id);
LDG_EXPORT uint32_t libresteam_shim_api_user_get(void);
LDG_EXPORT uint32_t libresteam_shim_api_pipe_get(void);
LDG_EXPORT void* libresteam_shim_api_ctx_init(void *data);
LDG_EXPORT void* libresteam_shim_api_user_iface_get(uint32_t user, const char *version);
LDG_EXPORT void* libresteam_shim_api_iface_create(const char *version);
LDG_EXPORT void* libresteam_shim_api_game_srv_iface_get(uint32_t user, const char *version);
LDG_EXPORT void libresteam_shim_api_cb_reg(void *obj, uint32_t cb_id);
LDG_EXPORT void libresteam_shim_api_cb_unreg(void *obj);
LDG_EXPORT void libresteam_shim_api_cb_run(void);
LDG_EXPORT uint8_t libresteam_shim_game_srv_init(uint32_t ip, uint16_t steam_port, uint16_t game_port, uint16_t query_port, uint32_t mode, const char *version);
LDG_EXPORT void libresteam_shim_game_srv_shutdown(void);
LDG_EXPORT void libresteam_shim_game_srv_cb_run(void);
LDG_EXPORT uint32_t libresteam_shim_game_srv_user_get(void);
LDG_EXPORT uint32_t libresteam_shim_game_srv_pipe_get(void);

uint32_t libresteam_shim_ctx_get(libresteam_shim_ctx_t **out);
uint32_t libresteam_shim_call_note(libresteam_shim_iface_t *iface, uint32_t idx);
uint32_t libresteam_shim_set_get(void *self, uint32_t idx, const libresteam_backend_set_t **out);
uint32_t libresteam_shim_arg_log(void *self, uint32_t idx, uint32_t res, const char *fmt, ...);
uint64_t libresteam_shim_stub_hit(void *self, uint32_t idx);
uint32_t libresteam_shim_stub_get(uint32_t idx, void **out);
uint32_t libresteam_shim_stub_sid_get(uint32_t idx, void **out);
uint32_t libresteam_shim_iface_def_get(uint32_t id, libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_iface_build(libresteam_shim_ctx_t *ctx);
uint32_t libresteam_shim_iface_find(libresteam_shim_ctx_t *ctx, const char *version, void **out);
uint32_t libresteam_shim_cb_reg(libresteam_shim_ctx_t *ctx, void *obj, uint32_t cb_id);
uint32_t libresteam_shim_cb_unreg(libresteam_shim_ctx_t *ctx, void *obj);
uint32_t libresteam_shim_cb_run(libresteam_shim_ctx_t *ctx);
uint32_t libresteam_shim_user_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_friends_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_user_stats_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_remote_storage_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_apps_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_utils_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_matchmaking_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_matchmaking_srvs_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_socks_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_utils_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_socks_012_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_utils_004_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_msgs_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_client_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_remote_play_001_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_remote_play_004_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_user_020_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_client_019_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_user_stats_011_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_utils_009_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_005_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_controller_007_def_get(libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_networking_socks_ctx_fill(libresteam_shim_ctx_t *ctx);
uint32_t libresteam_shim_gen_def_get(uint32_t idx, libresteam_shim_iface_def_t *out);
uint32_t libresteam_shim_flat_entry_get(uint32_t idx, libresteam_shim_flat_entry_t *out);
uint32_t libresteam_shim_flat_ck(libresteam_shim_ctx_t *ctx);
uint32_t libresteam_shim_flat_iface_get(uint32_t kind, const char *version, void **out);
uint32_t libresteam_shim_call_res_reg(libresteam_shim_ctx_t *ctx, void *obj, uint64_t call);
uint32_t libresteam_shim_call_res_unreg(libresteam_shim_ctx_t *ctx, void *obj, uint64_t call);
uint32_t libresteam_shim_ctx_ensure_get(libresteam_shim_ctx_t **out);

#endif
