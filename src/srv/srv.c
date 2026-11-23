#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/core/types.h>
#include <dangling/io/dir.h>
#include <dangling/logging/logging.h>
#include <dangling/io/path.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/net/sock.h>
#include <dangling/proto/emiemi.h>

#include <libresteam/srv/srv.h>
#include <libresteam/srv/user/user.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/wire/wire.h>

#define SRV_HOST_V4 "127.0.0.1"
#define SRV_HOST_V6 "::1"
#define SRV_POOL_VAR 0U
#define SRV_ALIGN_SLACK LIBRESTEAM_SRV_DIO_BLK
#define SRV_ALLOC_CUNT (4U + LIBRESTEAM_SRV_CLIENT_MAX)
#define SRV_SLOT_LISTENER UINT32_MAX
#define SRV_FLAGS_REPLY LIBRESTEAM_WIRE_FLAG_REPLY
#define SRV_CB_ID_NONE 0U
#define SRV_ACCEPT_POLL 0U

static uint32_t srv_aligned_alloc(ldg_mem_pool_t *pool, uint64_t size, uint8_t **out)
{
    void *alloc = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0x0;

    res = ldg_mem_pool_alloc(pool, size + SRV_ALIGN_SLACK, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    *out = (uint8_t *)(((uintptr_t)alloc + LIBRESTEAM_SRV_DIO_MASK) & ~(uintptr_t)LIBRESTEAM_SRV_DIO_MASK);

    return ldg_mem_secure_zero(*out, size);
}

static uint32_t srv_client_rd_byte_cb(void *ctx, uint8_t *out)
{
    libresteam_srv_client_t *client = 0x0;
    uint64_t bytes_rd = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    client = (libresteam_srv_client_t *)ctx;

    res = ldg_net_sock_rd(&client->sock, out, (uint64_t)sizeof(uint8_t), LIBRESTEAM_SRV_TIMEOUT, &bytes_rd);
    if (res != LDG_ERR_AOK) { return res; }

    if (bytes_rd != sizeof(uint8_t)) { return LDG_ERR_NET_RECV; }

    return LDG_ERR_AOK;
}

static uint32_t srv_client_wr_cb(const ldg_byte_t *data, ldg_dword_t len, void *ctx)
{
    libresteam_srv_client_t *client = 0x0;
    uint64_t off = 0;
    uint64_t bytes_wr = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    client = (libresteam_srv_client_t *)ctx;

    while (off < len)
    {
        bytes_wr = 0;
        res = ldg_net_sock_wr(&client->sock, &data[off], len - off, LIBRESTEAM_SRV_TIMEOUT, &bytes_wr);
        if (res != LDG_ERR_AOK) { return res; }

        if (bytes_wr == 0) { return LDG_ERR_NET_SEND; }

        off += bytes_wr;
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_client_drop(libresteam_srv_client_t *client)
{
    uint32_t res = LDG_ERR_AOK;

    if (ldg_net_sock_live_is(&client->sock) == LDG_TRUTH_TRUE) { res = ldg_net_sock_close(&client->sock, LIBRESTEAM_SRV_TIMEOUT); }

    client->live = LDG_TRUTH_FALSE;
    client->hello_done = LDG_TRUTH_FALSE;
    client->presence_cunt = 0;
    client->account = LIBRESTEAM_SRV_ACCOUNT_NONE;
    client->app_id = 0;
    client->img_len = 0;
    client->name[0] = LDG_STR_TERM;

    return res;
}

static uint32_t srv_data_dir_init(libresteam_srv_t *srv)
{
    uint32_t res = LDG_ERR_AOK;

    res = ldg_io_path_data_get(srv->path, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path) != LDG_TRUTH_TRUE)
    {
        res = ldg_io_dir_create(srv->path, LIBRESTEAM_SRV_DIR_ACCESS);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = ldg_io_path_join(srv->path, LIBRESTEAM_SRV_DIR_NAME, srv->data_dir, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->data_dir) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    return ldg_io_dir_create(srv->data_dir, LIBRESTEAM_SRV_DIR_ACCESS);
}

static uint32_t srv_pool_init(libresteam_srv_t *srv)
{
    uint64_t cap = 0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    cap = (uint64_t)LIBRESTEAM_SRV_FRAME_CAP + LIBRESTEAM_SRV_FRAME_CAP + LIBRESTEAM_SRV_DIO_BLK + LIBRESTEAM_SRV_IMG_CAP + (uint64_t)LIBRESTEAM_SRV_CLIENT_MAX * LIBRESTEAM_SRV_IMG_CAP + (uint64_t)SRV_ALLOC_CUNT * SRV_ALIGN_SLACK;

    res = ldg_mem_pool_create(SRV_POOL_VAR, cap, &srv->pool);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_aligned_alloc(srv->pool, LIBRESTEAM_SRV_FRAME_CAP, &srv->frame);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_aligned_alloc(srv->pool, LIBRESTEAM_SRV_FRAME_CAP, &srv->reply);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_aligned_alloc(srv->pool, LIBRESTEAM_SRV_DIO_BLK, &srv->blk);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_aligned_alloc(srv->pool, LIBRESTEAM_SRV_IMG_CAP, &srv->compact);
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < LIBRESTEAM_SRV_CLIENT_MAX; idx++)
    {
        res = srv_aligned_alloc(srv->pool, LIBRESTEAM_SRV_IMG_CAP, &srv->client[idx].img);
        if (res != LDG_ERR_AOK) { return res; }

        srv->client[idx].io.rd_byte = srv_client_rd_byte_cb;
        srv->client[idx].io.wr = srv_client_wr_cb;
        srv->client[idx].io.ctx = &srv->client[idx];
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_listen(libresteam_srv_t *srv)
{
    uint32_t res = LDG_ERR_AOK;

    srv->listener_cunt = 0;

    res = ldg_net_sock_listen(SRV_HOST_V4, srv->port, &srv->listener[srv->listener_cunt]);
    if (res == LDG_ERR_AOK) { srv->listener_cunt++; }
    else { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: listen on loopback v4 failed"); }

    res = ldg_net_sock_listen(SRV_HOST_V6, srv->port, &srv->listener[srv->listener_cunt]);
    if (res == LDG_ERR_AOK) { srv->listener_cunt++; }
    else { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: listen on loopback v6 failed"); }

    if (srv->listener_cunt == 0) { return LDG_ERR_NET_CONN; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_init(libresteam_srv_t *srv)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(srv, sizeof(libresteam_srv_t));
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_data_dir_init(srv);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_pool_init(srv);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_srv_conf_rd(srv);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_srv_account_next_scan(srv);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_net_sock_init();
    if (res != LDG_ERR_AOK) { return res; }

    return srv_listen(srv);
}

static uint32_t srv_hello(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_str_get(rd, client->name, LIBRESTEAM_SRV_NAME_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &client->app_id);
    if (res != LDG_ERR_AOK) { return res; }

    if (client->app_id == 0) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_srv_account_name_valid_is(client->name) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_srv_account_open(srv, client);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_steam_id_set(&id, client->account, LIBRESTEAM_STEAM_ID_UNIVERSE_PUBLIC, LIBRESTEAM_STEAM_ID_ACCOUNT_TYPE_INDIVIDUAL);
    if (res != LDG_ERR_AOK) { return res; }

    client->hello_done = LDG_TRUTH_TRUE;

    return libresteam_wire_u64_put(wr, id.all);
}

static uint32_t srv_dispatch(libresteam_srv_t *srv, libresteam_srv_client_t *client, const libresteam_wire_hdr_t *hdr, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    if (client->hello_done != LDG_TRUTH_TRUE)
    {
        if (hdr->service != LIBRESTEAM_WIRE_SERVICE_SESH || hdr->op != LIBRESTEAM_WIRE_SESH_OP_HELLO) { return LDG_ERR_DENIED; }

        return srv_hello(srv, client, rd, wr);
    }

    switch (hdr->service)
    {
        case LIBRESTEAM_WIRE_SERVICE_USER_STATS:
            return libresteam_srv_user_stats_handle(srv, client, hdr->op, rd, wr);
        case LIBRESTEAM_WIRE_SERVICE_FRIENDS:
            return libresteam_srv_friends_handle(srv, client, hdr->op, rd, wr);
        case LIBRESTEAM_WIRE_SERVICE_CLOUD:
            return libresteam_srv_cloud_handle(srv, client, hdr->op, rd, wr);
        case LIBRESTEAM_WIRE_SERVICE_SESH:
            return LDG_ERR_INVALID;
        default:
            return LDG_ERR_UNSUPPORTED;
    }
}

static uint32_t srv_client_serve(libresteam_srv_t *srv, libresteam_srv_client_t *client)
{
    libresteam_wire_hdr_t hdr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_hdr_t reply = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    ldg_dword_t len = 0;
    ldg_dword_t checksum = 0;
    uint32_t status = LDG_ERR_AOK;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_emiemi_recv(&client->io, srv->frame, LIBRESTEAM_WIRE_PAYLOAD_MAX, &len, &checksum);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_rd_init(&rd, srv->frame, len, &hdr);
    if (res != LDG_ERR_AOK) { return res; }

    if ((hdr.flags & (LIBRESTEAM_WIRE_FLAG_REPLY | LIBRESTEAM_WIRE_FLAG_PUSH)) != 0) { return LDG_ERR_NET_PROTOCOL; }

    res = libresteam_wire_wr_init(&wr, srv->reply, LIBRESTEAM_WIRE_PAYLOAD_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    status = srv_dispatch(srv, client, &hdr, &rd, &wr);
    if (status != LDG_ERR_AOK)
    {
        res = libresteam_wire_wr_init(&wr, srv->reply, LIBRESTEAM_WIRE_PAYLOAD_MAX);
        if (res != LDG_ERR_AOK) { return res; }
    }

    reply.flags = SRV_FLAGS_REPLY;
    reply.seq = hdr.seq;
    reply.status = status;
    reply.cb_id = SRV_CB_ID_NONE;
    reply.service = hdr.service;
    reply.op = hdr.op;

    res = libresteam_wire_hdr_finish(&wr, &reply);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_emiemi_send(&client->io, srv->reply, wr.len, &checksum);
}

static uint32_t srv_accept(libresteam_srv_t *srv, uint32_t listener_idx)
{
    ldg_net_sock_t sock = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_net_sock_accept(&srv->listener[listener_idx], SRV_ACCEPT_POLL, &sock);
    if (res == LDG_ERR_TIMEOUT) { return LDG_ERR_AOK; }

    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < LIBRESTEAM_SRV_CLIENT_MAX; idx++)
    {
        if (srv->client[idx].live == LDG_TRUTH_TRUE) { continue; }

        res = srv_client_drop(&srv->client[idx]);
        if (res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: stale client slot close failed"); }

        srv->client[idx].sock = sock;
        srv->client[idx].live = LDG_TRUTH_TRUE;

        return LDG_ERR_AOK;
    }

    ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: client limit reached; connection refused");

    return ldg_net_sock_close(&sock, LIBRESTEAM_SRV_TIMEOUT);
}

uint32_t libresteam_srv_run(libresteam_srv_t *srv)
{
    uint32_t cunt = 0;
    uint32_t idx = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    while (srv->listener_cunt > 0)
    {
        cunt = 0;

        for (idx = 0; idx < srv->listener_cunt; idx++)
        {
            srv->wait_sock[cunt] = srv->listener[idx];
            srv->wait_client[cunt] = SRV_SLOT_LISTENER;
            srv->wait_ready[cunt] = LDG_TRUTH_FALSE;
            cunt++;
        }

        for (idx = 0; idx < LIBRESTEAM_SRV_CLIENT_MAX; idx++)
        {
            if (srv->client[idx].live != LDG_TRUTH_TRUE) { continue; }

            srv->wait_sock[cunt] = srv->client[idx].sock;
            srv->wait_client[cunt] = idx;
            srv->wait_ready[cunt] = LDG_TRUTH_FALSE;
            cunt++;
        }

        res = ldg_net_sock_wait(srv->wait_sock, cunt, LIBRESTEAM_SRV_TIMEOUT, srv->wait_ready);
        if (res == LDG_ERR_TIMEOUT || res == LDG_ERR_INTERRUPTED) { continue; }

        if (res != LDG_ERR_AOK) { return res; }

        for (idx = 0; idx < cunt; idx++)
        {
            if (srv->wait_ready[idx] != LDG_TRUTH_TRUE) { continue; }

            slot = srv->wait_client[idx];
            if (slot == SRV_SLOT_LISTENER)
            {
                res = srv_accept(srv, idx);
                if (res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: accept failed"); }

                continue;
            }

            res = srv_client_serve(srv, &srv->client[slot]);
            if (res == LDG_ERR_AOK) { continue; }

            res = srv_client_drop(&srv->client[slot]);
            if (res != LDG_ERR_AOK) { ldg_logging_ln_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "srv: client close failed"); }
        }
    }

    return LDG_ERR_NET_CONN;
}

uint32_t libresteam_srv_shutdown(libresteam_srv_t *srv)
{
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t first_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (idx = 0; idx < LIBRESTEAM_SRV_CLIENT_MAX; idx++)
    {
        if (srv->client[idx].live != LDG_TRUTH_TRUE) { continue; }

        res = srv_client_drop(&srv->client[idx]);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }
    }

    for (idx = 0; idx < srv->listener_cunt; idx++)
    {
        res = ldg_net_sock_close(&srv->listener[idx], LIBRESTEAM_SRV_TIMEOUT);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }
    }

    srv->listener_cunt = 0;

    res = ldg_net_sock_shutdown();
    if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }

    if (srv->pool)
    {
        res = ldg_mem_pool_destroy(&srv->pool);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }
    }

    return first_res;
}
