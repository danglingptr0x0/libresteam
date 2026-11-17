#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/io/path.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/secure.h>
#include <dangling/parse/parse.h>
#include <dangling/str/fmt.h>

#include <libresteam/cb/cb.h>
#include <libresteam/net/types.h>
#include <libresteam/net/utils/utils.h>
#include <libresteam/shim/flat.h>
#include <libresteam/shim/shim.h>
#include <libresteam/srv_browser/ops.h>

#define LIBRESTEAM_SHIM_FLAT_LOG_LVL LDG_LOGGING_LVL_INFO
#define LIBRESTEAM_SHIM_FLAT_KIND_USER 0U
#define LIBRESTEAM_SHIM_FLAT_KIND_GAME_SRV 1U
#define LIBRESTEAM_SHIM_FLAT_CLIENT_VERSION "SteamClient020"
#define LIBRESTEAM_SHIM_FLAT_STR_EMPTY ""
#define LIBRESTEAM_SHIM_FLAT_STR(str) ((str) ? (str) : LIBRESTEAM_SHIM_FLAT_STR_EMPTY)
#define LIBRESTEAM_SHIM_FLAT_NONE 0U
#define LIBRESTEAM_SHIM_FLAT_IPV4_PREFIX_ZERO_LEN 10U
#define LIBRESTEAM_SHIM_FLAT_IPV4_MARK 0xFFU
#define LIBRESTEAM_SHIM_FLAT_IPV4_MARK_FIRST 10U
#define LIBRESTEAM_SHIM_FLAT_IPV4_MARK_SECOND 11U
#define LIBRESTEAM_SHIM_FLAT_IPV4_FIRST 12U
#define LIBRESTEAM_SHIM_FLAT_IPV4_SECOND 13U
#define LIBRESTEAM_SHIM_FLAT_IPV4_THIRD 14U
#define LIBRESTEAM_SHIM_FLAT_IPV4_FOURTH 15U
#define LIBRESTEAM_SHIM_FLAT_IPV6_LAST 15U
#define LIBRESTEAM_SHIM_FLAT_IPV6_LOOPBACK 1U
#define LIBRESTEAM_SHIM_FLAT_IPV4_LOOPBACK_NET 127U
#define LIBRESTEAM_SHIM_FLAT_SHIFT_HIGH 24U
#define LIBRESTEAM_SHIM_FLAT_SHIFT_MID 16U
#define LIBRESTEAM_SHIM_FLAT_SHIFT_LOW 8U
#define LIBRESTEAM_SHIM_FLAT_BYTE_MASK 0xFFU
#define LIBRESTEAM_SHIM_FLAT_IP_TYPE_V4 0U
#define LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID 0U
#define LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_NOT_FAKE 1U
#define LIBRESTEAM_SHIM_FLAT_IDENT_TYPE_STADIA 19U
#define LIBRESTEAM_SHIM_FLAT_IDENT_ID_SIZE 8U
#define LIBRESTEAM_SHIM_FLAT_IDENT_XBOX_MAX 33U
#define LIBRESTEAM_SHIM_FLAT_IDENT_GENERIC_STR_MAX 32U
#define LIBRESTEAM_SHIM_FLAT_IDENT_GENERIC_BYTES_MAX 32U
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT32 1U
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT64 2U
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_FP32 3U
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_STR 4U
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_PTR 5U
#define LIBRESTEAM_SHIM_FLAT_KV_VAL_OFF 256U
#define LIBRESTEAM_SHIM_FLAT_HOSTED_ADDR_SIZE 132U
#define LIBRESTEAM_SHIM_FLAT_POP_NONE 0U
#define LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB 1U
#define LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_CONN 4U
#define LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE 0U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_CONN_STATUS 201U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_AUTH_STATUS 202U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_RELAY_STATUS 203U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_MSGS_SESH_REQ 204U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_MSGS_SESH_FAILED 205U
#define LIBRESTEAM_SHIM_FLAT_CONF_CB_FAKE_IP 207U
#define LIBRESTEAM_SHIM_FLAT_RELAY_INIT_AGE 1e10F
#define LIBRESTEAM_SHIM_FLAT_CONF_DATA_OFF 8U

typedef struct libresteam_shim_flat_ip
{
    uint8_t ipv6[LIBRESTEAM_NET_IPV6_SIZE];
    uint32_t type;
} libresteam_shim_flat_ip_t;

static uint32_t libresteam_shim_flat_log(const char *what, const char *detail)
{
    return ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; %s; %s", what, LIBRESTEAM_SHIM_FLAT_STR(detail));
}

static uint32_t libresteam_shim_flat_set_get(const libresteam_backend_set_t **out)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0x0;

    res = libresteam_shim_ctx_get(&ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_INIT; }

    *out = &ctx->set;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_flat_iface_get(uint32_t kind, const char *version, void **out)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t log_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!version)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_shim_ctx_get(&ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_INIT; }

    if (kind == LIBRESTEAM_SHIM_FLAT_KIND_GAME_SRV) { res = LDG_ERR_UNSUPPORTED; }
    else { res = libresteam_shim_iface_find(ctx, version, out); }

    log_res = ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat iface; kind: %u32; version: %s; res: %u32", kind, version, res);
    if (log_res != LDG_ERR_AOK) { return res; }

    return res;
}

uint32_t libresteam_shim_flat_ck(libresteam_shim_ctx_t *ctx)
{
    libresteam_shim_flat_entry_t entry = LDG_STRUCT_ZERO_INIT;
    libresteam_shim_iface_t *iface = 0x0;
    void *found = 0x0;
    const char *name = 0x0;
    uint32_t slot = 0;
    uint32_t idx = 0;
    uint32_t bad = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (idx = 0; idx < LIBRESTEAM_SHIM_FLAT_ENTRY_CUNT; idx++)
    {
        res = libresteam_shim_flat_entry_get(idx, &entry);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_shim_iface_find(ctx, entry.version, &found);
        if (res != LDG_ERR_AOK)
        {
            bad++;
            if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat ck; version missing; version: %s; method: %s", entry.version, entry.method) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }

            continue;
        }

        iface = (libresteam_shim_iface_t *)found;
        if (entry.def_idx >= ctx->def[iface->id].method_cunt)
        {
            bad++;
            if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat ck; idx out of range; version: %s; method: %s; idx: %u32; cunt: %u32", entry.version, entry.method, entry.def_idx, ctx->def[iface->id].method_cunt) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }

            continue;
        }

        res = ctx->def[iface->id].name_get(entry.def_idx, &name);
        if (res != LDG_ERR_AOK) { name = LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

        res = ldg_cpp_slot_get(&ctx->desc[iface->id], entry.def_idx, &slot);
        if (res != LDG_ERR_AOK) { slot = UINT32_MAX; }

        if (ldg_parse_streq_is(name, entry.method) == LDG_TRUTH_TRUE && slot == entry.slot) { continue; }

        bad++;
        if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat ck; mismatch; version: %s; method: %s; have: %s; slot: %u32; want slot: %u32", entry.version, entry.method, name, slot, entry.slot) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }
    }

    res = ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat ck; entries: %u32; bad: %u32", (uint32_t)LIBRESTEAM_SHIM_FLAT_ENTRY_CUNT, bad);
    if (LDG_UNLIKELY(bad != 0)) { return LDG_ERR_INVALID; }

    return res;
}

uint8_t libresteam_shim_flat_init_anon(void)
{
    if (libresteam_shim_flat_log("init anonymous user", "unsupported") != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

uint8_t libresteam_shim_flat_steam_running_is(void)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

const char* libresteam_shim_flat_install_path_get(void)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    if (ldg_io_path_parent_get(ctx->steam.client_path, ctx->install_path, (uint64_t)sizeof(ctx->install_path)) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    return ctx->install_path;
}

void libresteam_shim_flat_thread_mem_release(void)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_mini_dump_wr(uint32_t code, void *info, uint32_t build)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; mini dump; code: %x32; info: %p; build: %u32", code, info, build) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_mini_dump_comment_set(const char *msg)
{
    if (libresteam_shim_flat_log("mini dump comment", msg) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_breakpad_app_id_set(uint32_t app_id)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; breakpad app id; app_id: %u32", app_id) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_breakpad_use(uint8_t en)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; breakpad handler; en: %u32", (uint32_t)en) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_try_catch_set(uint8_t en)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; try catch cbs; en: %u32", (uint32_t)en) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_call_res_reg(void *obj, uint64_t call)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (!obj) { return; }

    if (libresteam_shim_ctx_ensure_get(&ctx) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_call_res_reg(ctx, obj, call) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_call_res_unreg(void *obj, uint64_t call)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (!obj) { return; }

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return; }

    if (libresteam_shim_call_res_unreg(ctx, obj, call) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_manual_dispatch_init(void)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (libresteam_shim_ctx_ensure_get(&ctx) != LDG_ERR_AOK) { return; }

    ctx->manual_dispatch = LDG_TRUTH_TRUE;

    if (libresteam_shim_flat_log("manual dispatch init", LIBRESTEAM_SHIM_FLAT_STR_EMPTY) != LDG_ERR_AOK) { return; }
}

void libresteam_shim_flat_manual_dispatch_frame_run(uint32_t pipe)
{
    libresteam_shim_ctx_t *ctx = 0x0;

    if (libresteam_shim_ctx_get(&ctx) != LDG_ERR_AOK) { return; }

    if (ctx->manual_dispatch != LDG_TRUTH_TRUE || pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { return; }

    if (ctx->steam_up != LDG_TRUTH_TRUE) { return; }
}

uint8_t libresteam_shim_flat_manual_dispatch_next_get(uint32_t pipe, libresteam_cb_msg_t *out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (!out || pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_cb_next_get(set, out) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_manual_dispatch_last_free(uint32_t pipe)
{
    const libresteam_backend_set_t *set = 0x0;

    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { return; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return; }

    if (libresteam_cb_last_free(set) != LDG_ERR_AOK) { return; }
}

uint8_t libresteam_shim_flat_manual_dispatch_call_res_get(uint32_t pipe, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t failed = LDG_TRUTH_TRUE;
    uint32_t res = LDG_ERR_AOK;

    if (failed_out) { *failed_out = LDG_TRUTH_TRUE; }

    if (pipe != LIBRESTEAM_SHIM_PIPE_HANDLE) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_cb_call_res_get(set, call, buff, cap, cb_id_expected, &failed);
    if (failed_out) { *failed_out = failed; }

    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

void* libresteam_shim_flat_steam_client_get(void)
{
    void *iface = 0x0;

    if (libresteam_shim_flat_iface_get(LIBRESTEAM_SHIM_FLAT_KIND_USER, LIBRESTEAM_SHIM_FLAT_CLIENT_VERSION, &iface) != LDG_ERR_AOK) { return 0x0; }

    return iface;
}

uint8_t libresteam_shim_flat_game_srv_secure_is(void)
{
    if (libresteam_shim_flat_log("game srv secure", "unsupported") != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

uint64_t libresteam_shim_flat_game_srv_steam_id_get(void)
{
    if (libresteam_shim_flat_log("game srv steam id", "unsupported") != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return LIBRESTEAM_SHIM_FLAT_NONE;
}

uint32_t libresteam_shim_flat_game_srv_ipc_call_cunt_get(void)
{
    if (libresteam_shim_flat_log("game srv ipc call cunt", "unsupported") != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return LIBRESTEAM_SHIM_FLAT_NONE;
}

uint8_t libresteam_shim_flat_ip_addr_t_set_is(const void *addr)
{
    const libresteam_shim_flat_ip_t *self = (const libresteam_shim_flat_ip_t *)addr;
    uint32_t idx = 0;

    if (!self) { return LDG_TRUTH_FALSE; }

    if (self->type == LIBRESTEAM_SHIM_FLAT_IP_TYPE_V4)
    {
        for (idx = 0; idx < sizeof(uint32_t); idx++) { if (self->ipv6[idx] != 0) { return LDG_TRUTH_TRUE; } }
        return LDG_TRUTH_FALSE;
    }

    for (idx = 0; idx < LIBRESTEAM_NET_IPV6_SIZE; idx++) { if (self->ipv6[idx] != 0) { return LDG_TRUTH_TRUE; } }

    return LDG_TRUTH_FALSE;
}

void libresteam_shim_flat_kv_construct(void *self)
{
    char *kv = (char *)self;

    if (!kv) { return; }

    kv[LIBRESTEAM_SHIM_FLAT_NONE] = LDG_STR_TERM;
    kv[LIBRESTEAM_SHIM_FLAT_KV_VAL_OFF] = LDG_STR_TERM;
}

void libresteam_shim_flat_netadr_construct(libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return; }

    *self = (libresteam_srv_browser_netadr_t)LDG_STRUCT_ZERO_INIT;
}

void libresteam_shim_flat_netadr_init(libresteam_srv_browser_netadr_t *self, uint32_t ip, uint16_t query_port, uint16_t conn_port)
{
    if (!self) { return; }

    self->ip = ip;
    self->query_port = query_port;
    self->conn_port = conn_port;
}

uint16_t libresteam_shim_flat_netadr_query_port_get(const libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return self->query_port;
}

void libresteam_shim_flat_netadr_query_port_set(libresteam_srv_browser_netadr_t *self, uint16_t port)
{
    if (!self) { return; }

    self->query_port = port;
}

uint16_t libresteam_shim_flat_netadr_conn_port_get(const libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return self->conn_port;
}

void libresteam_shim_flat_netadr_conn_port_set(libresteam_srv_browser_netadr_t *self, uint16_t port)
{
    if (!self) { return; }

    self->conn_port = port;
}

uint32_t libresteam_shim_flat_netadr_ip_get(const libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return self->ip;
}

void libresteam_shim_flat_netadr_ip_set(libresteam_srv_browser_netadr_t *self, uint32_t ip)
{
    if (!self) { return; }

    self->ip = ip;
}

static const char* libresteam_shim_flat_netadr_str(uint32_t ip, uint16_t port)
{
    libresteam_shim_ctx_t *ctx = 0x0;
    uint64_t len = 0;

    if (libresteam_shim_ctx_ensure_get(&ctx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    if (ldg_str_fmt(ctx->addr_str, (uint64_t)sizeof(ctx->addr_str), &len, "%u32.%u32.%u32.%u32:%u32", (ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_HIGH) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK, (ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_MID) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK, (ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_LOW) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK, ip & LIBRESTEAM_SHIM_FLAT_BYTE_MASK, (uint32_t)port) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    return ctx->addr_str;
}

const char* libresteam_shim_flat_netadr_conn_str_get(const libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    return libresteam_shim_flat_netadr_str(self->ip, self->conn_port);
}

const char* libresteam_shim_flat_netadr_query_str_get(const libresteam_srv_browser_netadr_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    return libresteam_shim_flat_netadr_str(self->ip, self->query_port);
}

uint8_t libresteam_shim_flat_netadr_less_is(const libresteam_srv_browser_netadr_t *self, const libresteam_srv_browser_netadr_t *rhs)
{
    if (!self || !rhs) { return LDG_TRUTH_FALSE; }

    if (self->ip < rhs->ip) { return LDG_TRUTH_TRUE; }

    if (self->ip == rhs->ip && self->query_port < rhs->query_port) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

void libresteam_shim_flat_netadr_assign(libresteam_srv_browser_netadr_t *self, const libresteam_srv_browser_netadr_t *rhs)
{
    if (!self || !rhs) { return; }

    *self = *rhs;
}

void libresteam_shim_flat_srv_item_construct(libresteam_srv_browser_item_t *self)
{
    if (!self) { return; }

    if (ldg_mem_secure_zero(self, (uint64_t)sizeof(*self)) != LDG_ERR_AOK) { return; }
}

const char* libresteam_shim_flat_srv_item_name_get(const libresteam_srv_browser_item_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_STR_EMPTY; }

    if (self->name[LIBRESTEAM_SHIM_FLAT_NONE] != LDG_STR_TERM) { return self->name; }

    return libresteam_shim_flat_netadr_str(self->netadr.ip, self->netadr.conn_port);
}

void libresteam_shim_flat_srv_item_name_set(libresteam_srv_browser_item_t *self, const char *name)
{
    uint32_t idx = 0;

    if (!self) { return; }

    for (idx = 0; name && idx + LDG_STR_TERM_SIZE < LIBRESTEAM_SRV_BROWSER_NAME_MAX && name[idx] != LDG_STR_TERM; idx++) { self->name[idx] = name[idx]; }

    self->name[idx] = LDG_STR_TERM;
}

void libresteam_shim_flat_ip_clear(libresteam_net_ip_addr_t *self)
{
    if (!self) { return; }

    *self = (libresteam_net_ip_addr_t)LDG_STRUCT_ZERO_INIT;
}

uint8_t libresteam_shim_flat_ip_ipv6_zero_is(const libresteam_net_ip_addr_t *self)
{
    uint32_t idx = 0;

    if (!self) { return LDG_TRUTH_FALSE; }

    for (idx = 0; idx < LIBRESTEAM_NET_IPV6_SIZE; idx++) { if (self->ipv6[idx] != 0) { return LDG_TRUTH_FALSE; } }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_ip_ipv6_set(libresteam_net_ip_addr_t *self, const uint8_t *ipv6, uint16_t port)
{
    if (!self || !ipv6) { return; }

    if (ldg_mem_secure_cpy(self->ipv6, ipv6, (uint64_t)LIBRESTEAM_NET_IPV6_SIZE) != LDG_ERR_AOK) { return; }

    self->port = port;
}

void libresteam_shim_flat_ip_ipv4_set(libresteam_net_ip_addr_t *self, uint32_t ip, uint16_t port)
{
    uint32_t idx = 0;

    if (!self) { return; }

    for (idx = 0; idx < LIBRESTEAM_SHIM_FLAT_IPV4_PREFIX_ZERO_LEN; idx++) { self->ipv6[idx] = 0; }

    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_MARK_FIRST] = LIBRESTEAM_SHIM_FLAT_IPV4_MARK;
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_MARK_SECOND] = LIBRESTEAM_SHIM_FLAT_IPV4_MARK;
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_FIRST] = (uint8_t)((ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_HIGH) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK);
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_SECOND] = (uint8_t)((ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_MID) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK);
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_THIRD] = (uint8_t)((ip >> LIBRESTEAM_SHIM_FLAT_SHIFT_LOW) & LIBRESTEAM_SHIM_FLAT_BYTE_MASK);
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_FOURTH] = (uint8_t)(ip & LIBRESTEAM_SHIM_FLAT_BYTE_MASK);
    self->port = port;
}

uint8_t libresteam_shim_flat_ip_ipv4_is(const libresteam_net_ip_addr_t *self)
{
    uint32_t idx = 0;

    if (!self) { return LDG_TRUTH_FALSE; }

    for (idx = 0; idx < LIBRESTEAM_SHIM_FLAT_IPV4_PREFIX_ZERO_LEN; idx++) { if (self->ipv6[idx] != 0) { return LDG_TRUTH_FALSE; } }
    if (self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_MARK_FIRST] != LIBRESTEAM_SHIM_FLAT_IPV4_MARK) { return LDG_TRUTH_FALSE; }

    if (self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_MARK_SECOND] != LIBRESTEAM_SHIM_FLAT_IPV4_MARK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

uint32_t libresteam_shim_flat_ip_ipv4_get(const libresteam_net_ip_addr_t *self)
{
    if (libresteam_shim_flat_ip_ipv4_is(self) != LDG_TRUTH_TRUE) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return ((uint32_t)self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_FIRST] << LIBRESTEAM_SHIM_FLAT_SHIFT_HIGH) | ((uint32_t)self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_SECOND] << LIBRESTEAM_SHIM_FLAT_SHIFT_MID) | ((uint32_t)self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_THIRD] << LIBRESTEAM_SHIM_FLAT_SHIFT_LOW) | (uint32_t)self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_FOURTH];
}

void libresteam_shim_flat_ip_ipv6_localhost_set(libresteam_net_ip_addr_t *self, uint16_t port)
{
    if (!self) { return; }

    *self = (libresteam_net_ip_addr_t)LDG_STRUCT_ZERO_INIT;
    self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV6_LAST] = LIBRESTEAM_SHIM_FLAT_IPV6_LOOPBACK;
    self->port = port;
}

uint8_t libresteam_shim_flat_ip_localhost_is(const libresteam_net_ip_addr_t *self)
{
    uint32_t idx = 0;

    if (!self) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_ip_ipv4_is(self) == LDG_TRUTH_TRUE) { return self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV4_FIRST] == LIBRESTEAM_SHIM_FLAT_IPV4_LOOPBACK_NET ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE; }

    for (idx = 0; idx < LIBRESTEAM_SHIM_FLAT_IPV6_LAST; idx++) { if (self->ipv6[idx] != 0) { return LDG_TRUTH_FALSE; } }
    if (self->ipv6[LIBRESTEAM_SHIM_FLAT_IPV6_LAST] != LIBRESTEAM_SHIM_FLAT_IPV6_LOOPBACK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_ip_to_str(const libresteam_net_ip_addr_t *self, char *buff, uint32_t cap, uint8_t port_en)
{
    const libresteam_backend_set_t *set = 0x0;

    if (!buff || cap == 0) { return; }

    *buff = LDG_STR_TERM;

    if (!self) { return; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ip_addr_to_str(set, self, buff, (uint64_t)cap, port_en ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE) != LDG_ERR_AOK) { *buff = LDG_STR_TERM; }
}

uint8_t libresteam_shim_flat_ip_parse(libresteam_net_ip_addr_t *self, const char *str)
{
    const libresteam_backend_set_t *set = 0x0;

    if (!self || !str) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_ip_addr_parse(set, str, self) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

uint8_t libresteam_shim_flat_ip_equal_is(const libresteam_net_ip_addr_t *self, const libresteam_net_ip_addr_t *rhs)
{
    uint32_t idx = 0;

    if (!self || !rhs) { return LDG_TRUTH_FALSE; }

    for (idx = 0; idx < LIBRESTEAM_NET_IPV6_SIZE; idx++) { if (self->ipv6[idx] != rhs->ipv6[idx]) { return LDG_TRUTH_FALSE; } }
    if (self->port != rhs->port) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

uint32_t libresteam_shim_flat_ip_fake_type_get(const libresteam_net_ip_addr_t *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t type = LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID;

    if (!self) { return LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID; }

    if (libresteam_net_utils_ip_addr_fake_ip_type_get(set, self, &type) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID; }

    return type;
}

uint8_t libresteam_shim_flat_ip_fake_is(const libresteam_net_ip_addr_t *self)
{
    if (libresteam_shim_flat_ip_fake_type_get(self) > LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_NOT_FAKE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint64_t libresteam_shim_flat_ident_id_get(const libresteam_net_ident_t *self, uint32_t type)
{
    uint64_t id = 0;

    if (!self || self->type != type) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    if (ldg_mem_secure_cpy(&id, self->data, (uint64_t)sizeof(id)) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return id;
}

static void libresteam_shim_flat_ident_id_set(libresteam_net_ident_t *self, uint32_t type, uint64_t id)
{
    if (!self) { return; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    self->type = type;
    self->size = LIBRESTEAM_SHIM_FLAT_IDENT_ID_SIZE;

    if (ldg_mem_secure_cpy(self->data, &id, (uint64_t)sizeof(id)) != LDG_ERR_AOK) { self->type = LIBRESTEAM_NET_IDENT_TYPE_INVALID; }
}

static uint8_t libresteam_shim_flat_ident_str_set(libresteam_net_ident_t *self, uint32_t type, const char *str, uint32_t cap)
{
    uint32_t len = 0;

    if (!self || !str) { return LDG_TRUTH_FALSE; }

    for (len = 0; len < cap && str[len] != LDG_STR_TERM; len++) {}
    if (len >= cap) { return LDG_TRUTH_FALSE; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    if (ldg_mem_secure_cpy(self->data, str, (uint64_t)len) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    self->type = type;
    self->size = len + LDG_STR_TERM_SIZE;

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_ident_clear(libresteam_net_ident_t *self)
{
    if (!self) { return; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
}

uint8_t libresteam_shim_flat_ident_invalid_is(const libresteam_net_ident_t *self)
{
    if (!self || self->type == LIBRESTEAM_NET_IDENT_TYPE_INVALID) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

void libresteam_shim_flat_ident_steam_id_set(libresteam_net_ident_t *self, uint64_t id)
{
    if (!self) { return; }

    libresteam_shim_flat_ident_id_set(self, LIBRESTEAM_NET_IDENT_TYPE_STEAM_ID, id);
}

uint64_t libresteam_shim_flat_ident_steam_id_get(const libresteam_net_ident_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return libresteam_shim_flat_ident_id_get(self, LIBRESTEAM_NET_IDENT_TYPE_STEAM_ID);
}

uint8_t libresteam_shim_flat_ident_xbox_set(libresteam_net_ident_t *self, const char *str)
{
    if (!self || !str) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_ident_str_set(self, LIBRESTEAM_NET_IDENT_TYPE_XBOX_PAIRWISE_ID, str, LIBRESTEAM_SHIM_FLAT_IDENT_XBOX_MAX);
}

const char* libresteam_shim_flat_ident_xbox_get(const libresteam_net_ident_t *self)
{
    if (!self || self->type != LIBRESTEAM_NET_IDENT_TYPE_XBOX_PAIRWISE_ID) { return 0x0; }

    return (const char *)self->data;
}

void libresteam_shim_flat_ident_psn_set(libresteam_net_ident_t *self, uint64_t id)
{
    if (!self) { return; }

    libresteam_shim_flat_ident_id_set(self, LIBRESTEAM_NET_IDENT_TYPE_SONY_PSN, id);
}

uint64_t libresteam_shim_flat_ident_psn_get(const libresteam_net_ident_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return libresteam_shim_flat_ident_id_get(self, LIBRESTEAM_NET_IDENT_TYPE_SONY_PSN);
}

void libresteam_shim_flat_ident_stadia_set(libresteam_net_ident_t *self, uint64_t id)
{
    if (!self) { return; }

    libresteam_shim_flat_ident_id_set(self, LIBRESTEAM_SHIM_FLAT_IDENT_TYPE_STADIA, id);
}

uint64_t libresteam_shim_flat_ident_stadia_get(const libresteam_net_ident_t *self)
{
    if (!self) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return libresteam_shim_flat_ident_id_get(self, LIBRESTEAM_SHIM_FLAT_IDENT_TYPE_STADIA);
}

void libresteam_shim_flat_ident_ip_set(libresteam_net_ident_t *self, const libresteam_net_ip_addr_t *addr)
{
    if (!self || !addr) { return; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    if (ldg_mem_secure_cpy(self->data, addr, (uint64_t)sizeof(*addr)) != LDG_ERR_AOK) { return; }

    self->type = LIBRESTEAM_NET_IDENT_TYPE_IP_ADDR;
    self->size = (uint32_t)sizeof(*addr);
}

const libresteam_net_ip_addr_t* libresteam_shim_flat_ident_ip_get(const libresteam_net_ident_t *self)
{
    if (!self || self->type != LIBRESTEAM_NET_IDENT_TYPE_IP_ADDR) { return 0x0; }

    return (const libresteam_net_ip_addr_t *)(const void *)self->data;
}

void libresteam_shim_flat_ident_ipv4_set(libresteam_net_ident_t *self, uint32_t ip, uint16_t port)
{
    if (!self) { return; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    self->type = LIBRESTEAM_NET_IDENT_TYPE_IP_ADDR;
    self->size = (uint32_t)sizeof(libresteam_net_ip_addr_t);

    libresteam_shim_flat_ip_ipv4_set((libresteam_net_ip_addr_t *)(void *)self->data, ip, port);
}

uint32_t libresteam_shim_flat_ident_ipv4_get(const libresteam_net_ident_t *self)
{
    const libresteam_net_ip_addr_t *addr = libresteam_shim_flat_ident_ip_get(self);

    if (!addr) { return LIBRESTEAM_SHIM_FLAT_NONE; }

    return libresteam_shim_flat_ip_ipv4_get(addr);
}

uint32_t libresteam_shim_flat_ident_fake_type_get(const libresteam_net_ident_t *self)
{
    const libresteam_net_ip_addr_t *addr = libresteam_shim_flat_ident_ip_get(self);

    if (!addr) { return LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID; }

    return libresteam_shim_flat_ip_fake_type_get(addr);
}

uint8_t libresteam_shim_flat_ident_fake_is(const libresteam_net_ident_t *self)
{
    if (libresteam_shim_flat_ident_fake_type_get(self) > LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_NOT_FAKE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

void libresteam_shim_flat_ident_localhost_set(libresteam_net_ident_t *self)
{
    if (!self) { return; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    self->type = LIBRESTEAM_NET_IDENT_TYPE_IP_ADDR;
    self->size = (uint32_t)sizeof(libresteam_net_ip_addr_t);

    libresteam_shim_flat_ip_ipv6_localhost_set((libresteam_net_ip_addr_t *)(void *)self->data, LIBRESTEAM_SHIM_FLAT_NONE);
}

uint8_t libresteam_shim_flat_ident_localhost_is(const libresteam_net_ident_t *self)
{
    const libresteam_net_ip_addr_t *addr = libresteam_shim_flat_ident_ip_get(self);

    if (!addr) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_ip_localhost_is(addr);
}

uint8_t libresteam_shim_flat_ident_generic_str_set(libresteam_net_ident_t *self, const char *str)
{
    if (!self || !str) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_ident_str_set(self, LIBRESTEAM_NET_IDENT_TYPE_GENERIC_STR, str, LIBRESTEAM_SHIM_FLAT_IDENT_GENERIC_STR_MAX);
}

const char* libresteam_shim_flat_ident_generic_str_get(const libresteam_net_ident_t *self)
{
    if (!self || self->type != LIBRESTEAM_NET_IDENT_TYPE_GENERIC_STR) { return 0x0; }

    return (const char *)self->data;
}

uint8_t libresteam_shim_flat_ident_generic_bytes_set(libresteam_net_ident_t *self, const void *data, uint32_t len)
{
    if (!self || (!data && len != 0)) { return LDG_TRUTH_FALSE; }

    if (len > LIBRESTEAM_SHIM_FLAT_IDENT_GENERIC_BYTES_MAX) { return LDG_TRUTH_FALSE; }

    *self = (libresteam_net_ident_t)LDG_STRUCT_ZERO_INIT;
    if (len != 0 && ldg_mem_secure_cpy(self->data, data, (uint64_t)len) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    self->type = LIBRESTEAM_NET_IDENT_TYPE_GENERIC_BYTES;
    self->size = len;

    return LDG_TRUTH_TRUE;
}

const uint8_t* libresteam_shim_flat_ident_generic_bytes_get(const libresteam_net_ident_t *self, uint32_t *len_out)
{
    if (len_out) { *len_out = LIBRESTEAM_SHIM_FLAT_NONE; }

    if (!self || self->type != LIBRESTEAM_NET_IDENT_TYPE_GENERIC_BYTES) { return 0x0; }

    if (len_out) { *len_out = self->size; }

    return self->data;
}

uint8_t libresteam_shim_flat_ident_equal_is(const libresteam_net_ident_t *self, const libresteam_net_ident_t *rhs)
{
    uint32_t idx = 0;

    if (!self || !rhs) { return LDG_TRUTH_FALSE; }

    if (self->type != rhs->type || self->size != rhs->size) { return LDG_TRUTH_FALSE; }

    if (self->size > LIBRESTEAM_NET_IDENT_DATA_SIZE) { return LDG_TRUTH_FALSE; }

    for (idx = 0; idx < self->size; idx++) { if (self->data[idx] != rhs->data[idx]) { return LDG_TRUTH_FALSE; } }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_ident_to_str(const libresteam_net_ident_t *self, char *buff, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (!buff || cap == 0) { return; }

    *buff = LDG_STR_TERM;

    if (!self) { return; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ident_to_str(set, self, buff, (uint64_t)cap) != LDG_ERR_AOK) { *buff = LDG_STR_TERM; }
}

uint8_t libresteam_shim_flat_ident_parse(libresteam_net_ident_t *self, const char *str)
{
    const libresteam_backend_set_t *set = 0x0;

    if (!self || !str) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_ident_parse(set, str, self) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_msg_release(libresteam_net_msg_t *self)
{
    if (!self || !self->release_cb) { return; }

    self->release_cb(self);
}

static void libresteam_shim_flat_conf_set(libresteam_net_conf_val_t *self, uint32_t val, uint32_t data_type, uint64_t data)
{
    if (!self) { return; }

    self->val = val;
    self->data_type = data_type;
    self->data = data;
}

void libresteam_shim_flat_conf_int32_set(libresteam_net_conf_val_t *self, uint32_t val, uint32_t data)
{
    if (!self) { return; }

    libresteam_shim_flat_conf_set(self, val, LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT32, (uint64_t)data);
}

void libresteam_shim_flat_conf_int64_set(libresteam_net_conf_val_t *self, uint32_t val, uint64_t data)
{
    if (!self) { return; }

    libresteam_shim_flat_conf_set(self, val, LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT64, data);
}

void libresteam_shim_flat_conf_fp32_set(libresteam_net_conf_val_t *self, uint32_t val, libresteam_fp32_t data)
{
    uint32_t bits = 0;

    if (ldg_mem_secure_cpy(&bits, &data, (uint64_t)sizeof(bits)) != LDG_ERR_AOK) { return; }

    libresteam_shim_flat_conf_set(self, val, LIBRESTEAM_SHIM_FLAT_CONF_DATA_FP32, (uint64_t)bits);
}

void libresteam_shim_flat_conf_ptr_set(libresteam_net_conf_val_t *self, uint32_t val, void *data)
{
    if (!self) { return; }

    libresteam_shim_flat_conf_set(self, val, LIBRESTEAM_SHIM_FLAT_CONF_DATA_PTR, (uint64_t)(uintptr_t)data);
}

void libresteam_shim_flat_conf_str_set(libresteam_net_conf_val_t *self, uint32_t val, const char *data)
{
    if (!self) { return; }

    libresteam_shim_flat_conf_set(self, val, LIBRESTEAM_SHIM_FLAT_CONF_DATA_STR, (uint64_t)(uintptr_t)data);
}

static uint8_t libresteam_shim_flat_net_utils_conf_put(void *self, uint32_t val, uint32_t scope, uint64_t scope_obj, uint32_t data_type, const void *arg)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!self) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_net_utils_conf_val_set(set, val, scope, scope_obj, data_type, arg);
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; net conf set; val: %u32; scope: %u32; obj: %u64; type: %u32; res: %u32", val, scope, scope_obj, data_type, res) != LDG_ERR_AOK) { return res == LDG_ERR_AOK ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE; }

    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

void libresteam_shim_flat_net_utils_relay_init(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t up_to_date = LDG_TRUTH_FALSE;

    if (!self) { return; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return; }

    if (libresteam_net_utils_ping_data_up_to_date_get(set, LIBRESTEAM_SHIM_FLAT_RELAY_INIT_AGE, &up_to_date) != LDG_ERR_AOK) { return; }
}

uint8_t libresteam_shim_flat_net_utils_fake_ipv4_is(void *self, uint32_t ip)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t type = LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_INVALID;

    if (!self) { return LDG_TRUTH_FALSE; }

    if (libresteam_shim_flat_set_get(&set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_net_utils_ipv4_fake_ip_type_get(set, ip, &type) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (type > LIBRESTEAM_SHIM_FLAT_FAKE_IP_TYPE_NOT_FAKE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

uint8_t libresteam_shim_flat_net_utils_glob_int32_set(void *self, uint32_t val, uint32_t data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE, LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT32, &data);
}

uint8_t libresteam_shim_flat_net_utils_glob_fp32_set(void *self, uint32_t val, libresteam_fp32_t data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE, LIBRESTEAM_SHIM_FLAT_CONF_DATA_FP32, &data);
}

uint8_t libresteam_shim_flat_net_utils_glob_str_set(void *self, uint32_t val, const char *data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE, LIBRESTEAM_SHIM_FLAT_CONF_DATA_STR, data);
}

uint8_t libresteam_shim_flat_net_utils_glob_ptr_set(void *self, uint32_t val, void *data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE, LIBRESTEAM_SHIM_FLAT_CONF_DATA_PTR, &data);
}

uint8_t libresteam_shim_flat_net_utils_conn_int32_set(void *self, uint32_t conn, uint32_t val, uint32_t data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_CONN, (uint64_t)conn, LIBRESTEAM_SHIM_FLAT_CONF_DATA_INT32, &data);
}

uint8_t libresteam_shim_flat_net_utils_conn_fp32_set(void *self, uint32_t conn, uint32_t val, libresteam_fp32_t data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_CONN, (uint64_t)conn, LIBRESTEAM_SHIM_FLAT_CONF_DATA_FP32, &data);
}

uint8_t libresteam_shim_flat_net_utils_conn_str_set(void *self, uint32_t conn, uint32_t val, const char *data)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_CONN, (uint64_t)conn, LIBRESTEAM_SHIM_FLAT_CONF_DATA_STR, data);
}

static uint8_t libresteam_shim_flat_net_utils_cb_put(void *self, uint32_t val, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_conf_put(self, val, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_GLOB, LIBRESTEAM_SHIM_FLAT_CONF_SCOPE_OBJ_NONE, LIBRESTEAM_SHIM_FLAT_CONF_DATA_PTR, &fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_conn_status_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_CONN_STATUS, fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_auth_status_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_AUTH_STATUS, fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_relay_status_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_RELAY_STATUS, fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_fake_ip_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_FAKE_IP, fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_msgs_sesh_req_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_MSGS_SESH_REQ, fn);
}

uint8_t libresteam_shim_flat_net_utils_cb_msgs_sesh_failed_set(void *self, void *fn)
{
    if (!self) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_flat_net_utils_cb_put(self, LIBRESTEAM_SHIM_FLAT_CONF_CB_MSGS_SESH_FAILED, fn);
}

uint8_t libresteam_shim_flat_net_utils_conf_struct_set(void *self, const libresteam_net_conf_val_t *opt, uint32_t scope, uint64_t scope_obj)
{
    const void *arg = 0x0;

    if (!self || !opt) { return LDG_TRUTH_FALSE; }

    arg = (const uint8_t *)(const void *)opt + LIBRESTEAM_SHIM_FLAT_CONF_DATA_OFF;
    if (opt->data_type == LIBRESTEAM_SHIM_FLAT_CONF_DATA_STR) { arg = (const void *)(uintptr_t)opt->data; }

    return libresteam_shim_flat_net_utils_conf_put(self, opt->val, scope, scope_obj, opt->data_type, arg);
}

void libresteam_shim_flat_hosted_addr_clear(void *self)
{
    if (!self) { return; }

    if (ldg_mem_secure_zero(self, (uint64_t)LIBRESTEAM_SHIM_FLAT_HOSTED_ADDR_SIZE) != LDG_ERR_AOK) { return; }
}

uint32_t libresteam_shim_flat_hosted_addr_pop_get(const void *self)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; hosted addr pop get unsupported; addr: %p", self) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_FLAT_POP_NONE; }

    return LIBRESTEAM_SHIM_FLAT_POP_NONE;
}

void libresteam_shim_flat_hosted_addr_dev_set(void *self, uint32_t ip, uint16_t port, uint32_t pop)
{
    if (ldg_logging_line_wr(LIBRESTEAM_SHIM_LOG_PATH, LIBRESTEAM_SHIM_FLAT_LOG_LVL, "flat; hosted addr dev set unsupported; addr: %p; ip: %x32; port: %u32; pop: %x32", self, ip, (uint32_t)port, pop) != LDG_ERR_AOK) { return; }
}
