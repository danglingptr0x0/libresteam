#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/mem/secure.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/net/types.h>
#include <libresteam/net_socks/ops.h>

#define STEAM_NET_SOCKS_ARG_NONE 0U
#define STEAM_NET_SOCKS_SIGN_BIT 0x80000000U
#define STEAM_NET_SOCKS_SIG_REJECTED 0U
#define STEAM_NET_SOCKS_SIG_VFUNC_SEND 0U
#define STEAM_NET_SOCKS_SIG_VFUNC_RELEASE 1U
#define STEAM_NET_SOCKS_SIG_RECV_VFUNC_CONNECT_REQ 0U
#define STEAM_NET_SOCKS_SIG_RECV_VFUNC_REJECTION_SEND 1U
#define STEAM_NET_SOCKS_VFUNC_SIZE ((uint64_t)sizeof(libresteam_backend_steam_vfunc_t))

typedef uint8_t (*steam_net_socks_sig_send_cb_t)(void *self, uint32_t conn, const libresteam_net_conn_info_t *info, const void *msg, uint32_t size);
typedef void (*steam_net_socks_self_cb_t)(void *self);
typedef uint64_t (*steam_net_socks_connect_req_cb_t)(void *self, uint32_t conn, const libresteam_net_ident_t *peer, uint32_t local_virtual_port);
typedef void (*steam_net_socks_rejection_send_cb_t)(void *self, const libresteam_net_ident_t *peer, const void *msg, uint32_t size);

typedef struct steam_net_socks_recv_ctx
{
    const libresteam_backend_steam_vfunc_t *vtbl;
    libresteam_backend_steam_t *steam;
    libresteam_net_socks_sig_recv_t recv;
} steam_net_socks_recv_ctx_t;

static void steam_net_socks_sig_drop(const libresteam_net_socks_sig_t *sig)
{
    if (!sig->ops || !sig->ops->release_cb) { return; }

    sig->ops->release_cb(sig->content);
}

static void steam_net_socks_sig_slot_free(libresteam_backend_steam_net_socks_sig_slot_t *sig_slot)
{
    sig_slot->sig.ops = 0x0;
    sig_slot->sig.content = 0x0;
    sig_slot->in_use = LDG_TRUTH_FALSE;
}

static uint32_t steam_net_socks_sig_slot_claim(libresteam_backend_steam_t *steam, const libresteam_net_socks_sig_t *sig, libresteam_backend_steam_net_socks_sig_slot_t **out)
{
    libresteam_backend_steam_net_socks_sig_slot_t *sig_slot = 0x0;
    uint32_t idx = 0;

    *out = 0x0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_SIG_MAX; idx++)
    {
        sig_slot = &steam->net_socks_sig_slot[idx];
        if (sig_slot->in_use == LDG_TRUTH_FALSE)
        {
            sig_slot->sig = *sig;
            sig_slot->in_use = LDG_TRUTH_TRUE;
            *out = sig_slot;
            return LDG_ERR_AOK;
        }
    }

    steam_net_socks_sig_drop(sig);

    return LDG_ERR_FULL;
}

static uint8_t steam_net_socks_sig_send_cb(void *self, uint32_t conn, const libresteam_net_conn_info_t *info, const void *msg, uint32_t size)
{
    libresteam_backend_steam_net_socks_sig_slot_t *sig_slot = (libresteam_backend_steam_net_socks_sig_slot_t *)self;

    if (LDG_UNLIKELY(!sig_slot || !sig_slot->sig.ops || !sig_slot->sig.ops->sig_send_cb)) { return LDG_TRUTH_FALSE; }

    if (sig_slot->sig.ops->sig_send_cb(sig_slot->sig.content, conn, info, msg, size) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static void steam_net_socks_sig_release_cb(void *self)
{
    libresteam_backend_steam_net_socks_sig_slot_t *sig_slot = (libresteam_backend_steam_net_socks_sig_slot_t *)self;
    libresteam_net_socks_sig_t sig = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!sig_slot)) { return; }

    sig = sig_slot->sig;
    steam_net_socks_sig_slot_free(sig_slot);
    steam_net_socks_sig_drop(&sig);
}

static uint64_t steam_net_socks_recv_connect_req_cb(void *self, uint32_t conn, const libresteam_net_ident_t *peer, uint32_t local_virtual_port)
{
    steam_net_socks_recv_ctx_t *recv_ctx = (steam_net_socks_recv_ctx_t *)self;
    libresteam_backend_steam_net_socks_sig_slot_t *sig_slot = 0x0;
    libresteam_net_socks_sig_t sig = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!recv_ctx || !recv_ctx->steam || !recv_ctx->recv.ops || !recv_ctx->recv.ops->connect_req_cb)) { return STEAM_NET_SOCKS_SIG_REJECTED; }

    res = recv_ctx->recv.ops->connect_req_cb(recv_ctx->recv.content, conn, peer, local_virtual_port, &sig);
    if (res != LDG_ERR_AOK || !sig.ops) { return STEAM_NET_SOCKS_SIG_REJECTED; }

    res = steam_net_socks_sig_slot_claim(recv_ctx->steam, &sig, &sig_slot);
    if (res != LDG_ERR_AOK) { return STEAM_NET_SOCKS_SIG_REJECTED; }

    return (uint64_t)(uintptr_t)sig_slot;
}

static void steam_net_socks_recv_rejection_send_cb(void *self, const libresteam_net_ident_t *peer, const void *msg, uint32_t size)
{
    steam_net_socks_recv_ctx_t *recv_ctx = (steam_net_socks_recv_ctx_t *)self;

    if (LDG_UNLIKELY(!recv_ctx || !recv_ctx->recv.ops || !recv_ctx->recv.ops->rejection_send_cb)) { return; }

    recv_ctx->recv.ops->rejection_send_cb(recv_ctx->recv.content, peer, msg, size);
}

static uint32_t steam_net_socks_tgt_get(void *ctx, void *port, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (port)
    {
        *obj_out = port;
        return ldg_cpp_slot_get(&steam->fake_udp_port_desc, method_idx, slot_out);
    }

    if (LDG_UNLIKELY(!steam->net_socks)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->net_socks;

    return ldg_cpp_slot_get(&steam->net_socks_desc, method_idx, slot_out);
}

static uint32_t steam_net_socks_void_call(void *ctx, void *port, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_socks_tgt_get(ctx, port, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(obj, slot, arg, arg_cunt);
}

static uint32_t steam_net_socks_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_net_socks_tgt_get(ctx, 0x0, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_u32_call(void *ctx, void *port, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_net_socks_tgt_get(ctx, port, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_net_socks_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_net_socks_tgt_get(ctx, 0x0, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_net_socks_handle_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_socks_u32_call(ctx, 0x0, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == LIBRESTEAM_NET_HANDLE_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_conf_handle_call(void *ctx, uint32_t method_idx, ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *out)
{
    uint32_t cunt = arg_cunt;

    if (LDG_UNLIKELY(!conf_val && conf_val_cunt != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conf_val_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = conf_val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_handle_call(ctx, method_idx, arg, cunt, out);
}

static uint32_t steam_net_socks_cunt_call(void *ctx, void *port, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_socks_u32_call(ctx, port, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_NET_SOCKS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_INVALID;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_listen_sock_ip_create(void *ctx, const libresteam_net_ip_addr_t *local, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!local)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = local, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_LISTEN_SOCK_IP_CREATE, arg, cunt, conf_val_cunt, conf_val, sock_out);
}

static uint32_t steam_net_socks_ip_addr_connect(void *ctx, const libresteam_net_ip_addr_t *addr, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!addr)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = addr, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_IP_ADDR_CONNECT, arg, cunt, conf_val_cunt, conf_val, conn_out);
}

static uint32_t steam_net_socks_listen_sock_p2p_create(void *ctx, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    arg[cunt] = (ldg_cpp_arg_t) { .val = local_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_CREATE, arg, cunt, conf_val_cunt, conf_val, sock_out);
}

static uint32_t steam_net_socks_p2p_connect(void *ctx, const libresteam_net_ident_t *remote, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = remote_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_P2P_CONNECT, arg, cunt, conf_val_cunt, conf_val, conn_out);
}

static uint32_t steam_net_socks_conn_accept(void *ctx, uint32_t conn, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_ACCEPT, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_conn_close(void *ctx, uint32_t conn, uint32_t reason, const char *debug, uint8_t linger_en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = reason, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = debug, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = linger_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_CLOSE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_listen_sock_close(void *ctx, uint32_t sock)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sock, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_LISTEN_SOCK_CLOSE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_user_data_set(void *ctx, uint32_t conn, uint64_t user_data)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = user_data, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_USER_DATA_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_user_data_get(void *ctx, uint32_t conn, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_USER_DATA_GET, arg, cunt, out);
}

static uint32_t steam_net_socks_conn_name_set(void *ctx, uint32_t conn, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_void_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_NAME_SET, arg, cunt);
}

static uint32_t steam_net_socks_conn_name_get(void *ctx, uint32_t conn, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_NAME_GET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_msg_send(void *ctx, uint32_t conn, const void *data, uint32_t size, uint32_t flags, uint64_t *msg_num_out, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (msg_num_out) { *msg_num_out = 0; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_num_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_MSG_SEND, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_msgs_send(void *ctx, uint32_t msg_cunt, libresteam_net_msg_t *const *msg, uint64_t *msg_num_or_res_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = msg_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_num_or_res_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_void_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_MSGS_SEND, arg, cunt);
}

static uint32_t steam_net_socks_conn_msgs_flush(void *ctx, uint32_t conn, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_MSGS_FLUSH, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_conn_msgs_recv(void *ctx, uint32_t conn, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_cunt_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_MSGS_RECV, arg, cunt, cunt_out);
}

static uint32_t steam_net_socks_conn_info_get(void *ctx, uint32_t conn, libresteam_net_conn_info_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_INFO_GET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_real_time_status_get(void *ctx, uint32_t conn, libresteam_net_real_time_status_t *status, uint32_t lane_cunt, libresteam_net_lane_status_t *lane, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!lane && lane_cunt != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = status, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = lane_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = lane, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_REAL_TIME_STATUS_GET, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_conn_detailed_status_get(void *ctx, uint32_t conn, char *buff, uint32_t cap, uint32_t *needed_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t needed = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!needed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *needed_out = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_DETAILED_STATUS_GET, arg, cunt, &needed);
    if (res != LDG_ERR_AOK) { return res; }

    if ((needed & STEAM_NET_SOCKS_SIGN_BIT) != 0) { return LDG_ERR_INVALID; }

    if (needed != 0)
    {
        *needed_out = needed;
        return LDG_ERR_OVERFLOW;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_listen_sock_addr_get(void *ctx, uint32_t sock, libresteam_net_ip_addr_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = sock, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_LISTEN_SOCK_ADDR_GET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_sock_pair_create(void *ctx, uint32_t *conn_lhs_out, uint32_t *conn_rhs_out, uint8_t loopback_en, const libresteam_net_ident_t *ident_lhs, const libresteam_net_ident_t *ident_rhs)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!conn_lhs_out || !conn_rhs_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_lhs_out = LIBRESTEAM_NET_HANDLE_INVALID;
    *conn_rhs_out = LIBRESTEAM_NET_HANDLE_INVALID;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = conn_lhs_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = conn_rhs_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = loopback_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ident_lhs, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ident_rhs, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_SOCK_PAIR_CREATE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_lane_conf_set(void *ctx, uint32_t conn, uint32_t lane_cunt, const uint32_t *prio, const uint16_t *weight, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = lane_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = prio, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = weight, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_LANE_CONF_SET, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_ident_get(void *ctx, libresteam_net_ident_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_IDENT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_net_socks_authentication_init(void *ctx, uint32_t *avail_out)
{
    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_AUTHENTICATION_INIT, 0x0, STEAM_NET_SOCKS_ARG_NONE, avail_out);
}

static uint32_t steam_net_socks_authentication_status_get(void *ctx, libresteam_net_avail_status_t *details, uint32_t *avail_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = details, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_AUTHENTICATION_STATUS_GET, arg, cunt, avail_out);
}

static uint32_t steam_net_socks_poll_group_create(void *ctx, uint32_t *group_out)
{
    if (LDG_UNLIKELY(!group_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_net_socks_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_POLL_GROUP_CREATE, 0x0, STEAM_NET_SOCKS_ARG_NONE, group_out);
}

static uint32_t steam_net_socks_poll_group_destroy(void *ctx, uint32_t group)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_POLL_GROUP_DESTROY, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_conn_poll_group_set(void *ctx, uint32_t conn, uint32_t group)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_POLL_GROUP_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_poll_group_msgs_recv(void *ctx, uint32_t group, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = group, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_cunt_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_POLL_GROUP_MSGS_RECV, arg, cunt, cunt_out);
}

static uint32_t steam_net_socks_relay_authentication_tkt_receive(void *ctx, const void *tkt, uint32_t size, void *parsed_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!tkt)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tkt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = parsed_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_RECEIVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_relay_authentication_tkt_find(void *ctx, const libresteam_net_ident_t *game_srv, uint32_t remote_virtual_port, void *parsed_out, uint32_t *expiry_sec_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!expiry_sec_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *expiry_sec_out = 0;

    if (LDG_UNLIKELY(!game_srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = game_srv, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = remote_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = parsed_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_RELAY_AUTHENTICATION_TKT_FIND, arg, cunt, expiry_sec_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*expiry_sec_out == 0 || (*expiry_sec_out & STEAM_NET_SOCKS_SIGN_BIT) != 0)
    {
        *expiry_sec_out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_hosted_dedicated_srv_connect(void *ctx, const libresteam_net_ident_t *tgt, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!tgt)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tgt, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = remote_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_CONNECT, arg, cunt, conf_val_cunt, conf_val, conn_out);
}

static uint32_t steam_net_socks_hosted_dedicated_srv_port_get(void *ctx, uint16_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_net_socks_tgt_get(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_PORT_GET, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint16_call(obj, slot, 0x0, STEAM_NET_SOCKS_ARG_NONE, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_hosted_dedicated_srv_pop_get(void *ctx, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_POP_GET, 0x0, STEAM_NET_SOCKS_ARG_NONE, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_hosted_dedicated_srv_addr_get(void *ctx, void *routing, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!routing)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = routing, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_ADDR_GET, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_hosted_dedicated_srv_listen_sock_create(void *ctx, uint32_t local_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    arg[cunt] = (ldg_cpp_arg_t) { .val = local_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_HOSTED_DEDICATED_SRV_LISTEN_SOCK_CREATE, arg, cunt, conf_val_cunt, conf_val, sock_out);
}

static uint32_t steam_net_socks_game_coordinator_srv_login_get(void *ctx, void *login_info, uint32_t *blob_size, void *blob, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!login_info || !blob_size)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = login_info, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = blob_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = blob, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_GAME_COORDINATOR_SRV_LOGIN_GET, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_p2p_custom_sig_connect(void *ctx, const libresteam_net_socks_sig_t *sig, const libresteam_net_ident_t *peer, uint32_t remote_virtual_port, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *conn_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    libresteam_backend_steam_net_socks_sig_slot_t *sig_slot = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!conn_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;

    if (LDG_UNLIKELY(!sig || !sig->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam || (!conf_val && conf_val_cunt != 0)))
    {
        steam_net_socks_sig_drop(sig);
        return LDG_ERR_FUNC_ARG_NULL;
    }

    res = steam_net_socks_tgt_get(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_CONNECT, &obj, &slot);
    if (res != LDG_ERR_AOK)
    {
        steam_net_socks_sig_drop(sig);
        return res;
    }

    res = steam_net_socks_sig_slot_claim(steam, sig, &sig_slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = sig_slot, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = peer, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = remote_virtual_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = conf_val_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = conf_val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = ldg_cpp_uint32_call(obj, slot, arg, cunt, conn_out);
    if (res != LDG_ERR_AOK)
    {
        steam_net_socks_sig_slot_free(sig_slot);
        steam_net_socks_sig_drop(sig);
        *conn_out = LIBRESTEAM_NET_HANDLE_INVALID;
        return res;
    }

    if (*conn_out == LIBRESTEAM_NET_HANDLE_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_p2p_custom_sig_receive(void *ctx, const void *msg, uint32_t size, const libresteam_net_socks_sig_recv_t *recv)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    steam_net_socks_recv_ctx_t recv_ctx = LDG_STRUCT_ZERO_INIT;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!steam || !msg || !recv || !recv->ops)) { return LDG_ERR_FUNC_ARG_NULL; }

    recv_ctx.vtbl = steam->net_socks_sig_recv_vtbl;
    recv_ctx.steam = steam;
    recv_ctx.recv = *recv;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &recv_ctx, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_P2P_CUSTOM_SIG_RECEIVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_certificate_req_get(void *ctx, uint32_t *blob_size, void *blob, char *fail_msg)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!blob_size || !fail_msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    fail_msg[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = blob_size, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = blob, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = fail_msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CERTIFICATE_REQ_GET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_certificate_set(void *ctx, const void *certificate, uint32_t size, char *fail_msg)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!certificate || !fail_msg)) { return LDG_ERR_FUNC_ARG_NULL; }

    fail_msg[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = certificate, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = fail_msg, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CERTIFICATE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_net_socks_ident_rst(void *ctx, const libresteam_net_ident_t *ident)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ident, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_void_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_IDENT_RST, arg, cunt);
}

static uint32_t steam_net_socks_cb_run(void *ctx)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_net_socks_tgt_get(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CB_RUN, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(obj, slot, 0x0, STEAM_NET_SOCKS_ARG_NONE);
}

static uint32_t steam_net_socks_fake_ip_req_begin(void *ctx, uint32_t port_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = port_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_FAKE_IP_REQ_BEGIN, arg, cunt, LDG_ERR_BUSY);
}

static uint32_t steam_net_socks_fake_ip_get(void *ctx, uint32_t first_port_idx, libresteam_net_fake_ip_res_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = first_port_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_void_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_FAKE_IP_GET, arg, cunt);
}

static uint32_t steam_net_socks_listen_sock_p2p_fake_ip_create(void *ctx, uint32_t fake_port_idx, uint32_t conf_val_cunt, const libresteam_net_conf_val_t *conf_val, uint32_t *sock_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!sock_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *sock_out = LIBRESTEAM_NET_HANDLE_INVALID;

    arg[cunt] = (ldg_cpp_arg_t) { .val = fake_port_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_conf_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_LISTEN_SOCK_P2P_FAKE_IP_CREATE, arg, cunt, conf_val_cunt, conf_val, sock_out);
}

static uint32_t steam_net_socks_conn_remote_fake_ip_get(void *ctx, uint32_t conn, libresteam_net_ip_addr_t *out, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = conn, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_u32_call(ctx, 0x0, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_CONN_REMOTE_FAKE_IP_GET, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_fake_udp_port_create(void *ctx, uint32_t fake_srv_port_idx, void **port_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t port = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!port_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *port_out = 0x0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = fake_srv_port_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_net_socks_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_FAKE_UDP_PORT_CREATE, arg, cunt, &port);
    if (res != LDG_ERR_AOK) { return res; }

    if (port == 0) { return LDG_ERR_INVALID; }

    *port_out = (void *)(uintptr_t)port;

    return LDG_ERR_AOK;
}

static uint32_t steam_net_socks_fake_udp_port_destroy(void *ctx, void *port)
{
    if (LDG_UNLIKELY(!port)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_net_socks_void_call(ctx, port, LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_DESTROY, 0x0, STEAM_NET_SOCKS_ARG_NONE);
}

static uint32_t steam_net_socks_fake_udp_port_msg_send(void *ctx, void *port, const libresteam_net_ip_addr_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t *eres_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!eres_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *eres_out = 0;

    if (LDG_UNLIKELY(!port || !remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!data && size != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = size, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_u32_call(ctx, port, LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_MSG_SEND, arg, cunt, eres_out);
}

static uint32_t steam_net_socks_fake_udp_port_msgs_recv(void *ctx, void *port, libresteam_net_msg_t **msg_out, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!port || !msg_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = msg_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_net_socks_cunt_call(ctx, port, LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_MSGS_RECV, arg, cunt, cunt_out);
}

static uint32_t steam_net_socks_fake_udp_port_cleanup_schedule(void *ctx, void *port, const libresteam_net_ip_addr_t *remote)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!port || !remote)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = remote, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_net_socks_void_call(ctx, port, LIBRESTEAM_BACKEND_STEAM_FAKE_UDP_PORT_CLEANUP_SCHEDULE, arg, cunt);
}

static uint32_t steam_net_socks_vtbl_fill(libresteam_backend_steam_t *steam)
{
    steam_net_socks_sig_send_cb_t sig_send_cb = 0x0;
    steam_net_socks_self_cb_t self_cb = 0x0;
    steam_net_socks_connect_req_cb_t connect_req_cb = 0x0;
    steam_net_socks_rejection_send_cb_t rejection_send_cb = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    sig_send_cb = steam_net_socks_sig_send_cb;
    res = ldg_mem_secure_cpy(&steam->net_socks_sig_vtbl[STEAM_NET_SOCKS_SIG_VFUNC_SEND], &sig_send_cb, STEAM_NET_SOCKS_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_net_socks_sig_release_cb;
    res = ldg_mem_secure_cpy(&steam->net_socks_sig_vtbl[STEAM_NET_SOCKS_SIG_VFUNC_RELEASE], &self_cb, STEAM_NET_SOCKS_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    connect_req_cb = steam_net_socks_recv_connect_req_cb;
    res = ldg_mem_secure_cpy(&steam->net_socks_sig_recv_vtbl[STEAM_NET_SOCKS_SIG_RECV_VFUNC_CONNECT_REQ], &connect_req_cb, STEAM_NET_SOCKS_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    rejection_send_cb = steam_net_socks_recv_rejection_send_cb;
    res = ldg_mem_secure_cpy(&steam->net_socks_sig_recv_vtbl[STEAM_NET_SOCKS_SIG_RECV_VFUNC_REJECTION_SEND], &rejection_send_cb, STEAM_NET_SOCKS_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_NET_SOCKS_SIG_MAX; idx++)
    {
        steam->net_socks_sig_slot[idx].vtbl = steam->net_socks_sig_vtbl;
        steam_net_socks_sig_slot_free(&steam->net_socks_sig_slot[idx]);
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_net_socks_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_net_socks_ops_t *ops = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_net_socks_vtbl_fill(steam);
    if (res != LDG_ERR_AOK) { return res; }

    ops = &steam->net_socks_ops;
    ops->listen_sock_ip_create = steam_net_socks_listen_sock_ip_create;
    ops->ip_addr_connect = steam_net_socks_ip_addr_connect;
    ops->listen_sock_p2p_create = steam_net_socks_listen_sock_p2p_create;
    ops->p2p_connect = steam_net_socks_p2p_connect;
    ops->conn_accept = steam_net_socks_conn_accept;
    ops->conn_close = steam_net_socks_conn_close;
    ops->listen_sock_close = steam_net_socks_listen_sock_close;
    ops->conn_user_data_set = steam_net_socks_conn_user_data_set;
    ops->conn_user_data_get = steam_net_socks_conn_user_data_get;
    ops->conn_name_set = steam_net_socks_conn_name_set;
    ops->conn_name_get = steam_net_socks_conn_name_get;
    ops->conn_msg_send = steam_net_socks_conn_msg_send;
    ops->msgs_send = steam_net_socks_msgs_send;
    ops->conn_msgs_flush = steam_net_socks_conn_msgs_flush;
    ops->conn_msgs_recv = steam_net_socks_conn_msgs_recv;
    ops->conn_info_get = steam_net_socks_conn_info_get;
    ops->conn_real_time_status_get = steam_net_socks_conn_real_time_status_get;
    ops->conn_detailed_status_get = steam_net_socks_conn_detailed_status_get;
    ops->listen_sock_addr_get = steam_net_socks_listen_sock_addr_get;
    ops->sock_pair_create = steam_net_socks_sock_pair_create;
    ops->conn_lane_conf_set = steam_net_socks_conn_lane_conf_set;
    ops->ident_get = steam_net_socks_ident_get;
    ops->authentication_init = steam_net_socks_authentication_init;
    ops->authentication_status_get = steam_net_socks_authentication_status_get;
    ops->poll_group_create = steam_net_socks_poll_group_create;
    ops->poll_group_destroy = steam_net_socks_poll_group_destroy;
    ops->conn_poll_group_set = steam_net_socks_conn_poll_group_set;
    ops->poll_group_msgs_recv = steam_net_socks_poll_group_msgs_recv;
    ops->relay_authentication_tkt_receive = steam_net_socks_relay_authentication_tkt_receive;
    ops->relay_authentication_tkt_find = steam_net_socks_relay_authentication_tkt_find;
    ops->hosted_dedicated_srv_connect = steam_net_socks_hosted_dedicated_srv_connect;
    ops->hosted_dedicated_srv_port_get = steam_net_socks_hosted_dedicated_srv_port_get;
    ops->hosted_dedicated_srv_pop_get = steam_net_socks_hosted_dedicated_srv_pop_get;
    ops->hosted_dedicated_srv_addr_get = steam_net_socks_hosted_dedicated_srv_addr_get;
    ops->hosted_dedicated_srv_listen_sock_create = steam_net_socks_hosted_dedicated_srv_listen_sock_create;
    ops->game_coordinator_srv_login_get = steam_net_socks_game_coordinator_srv_login_get;
    ops->p2p_custom_sig_connect = steam_net_socks_p2p_custom_sig_connect;
    ops->p2p_custom_sig_receive = steam_net_socks_p2p_custom_sig_receive;
    ops->certificate_req_get = steam_net_socks_certificate_req_get;
    ops->certificate_set = steam_net_socks_certificate_set;
    ops->ident_rst = steam_net_socks_ident_rst;
    ops->cb_run = steam_net_socks_cb_run;
    ops->fake_ip_req_begin = steam_net_socks_fake_ip_req_begin;
    ops->fake_ip_get = steam_net_socks_fake_ip_get;
    ops->listen_sock_p2p_fake_ip_create = steam_net_socks_listen_sock_p2p_fake_ip_create;
    ops->conn_remote_fake_ip_get = steam_net_socks_conn_remote_fake_ip_get;
    ops->fake_udp_port_create = steam_net_socks_fake_udp_port_create;
    ops->fake_udp_port_destroy = steam_net_socks_fake_udp_port_destroy;
    ops->fake_udp_port_msg_send = steam_net_socks_fake_udp_port_msg_send;
    ops->fake_udp_port_msgs_recv = steam_net_socks_fake_udp_port_msgs_recv;
    ops->fake_udp_port_cleanup_schedule = steam_net_socks_fake_udp_port_cleanup_schedule;
    steam->ops.net_socks = &steam->net_socks_ops;

    return LDG_ERR_AOK;
}
