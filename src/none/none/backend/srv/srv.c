#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/core/types.h>
#include <dangling/logging/logging.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/mem.h>
#include <dangling/mem/secure.h>
#include <dangling/net/sock.h>
#include <dangling/proto/emiemi.h>
#include <dangling/str/str.h>
#include <dangling/time/time.h>

#include <libresteam/backend/backend.h>
#include <libresteam/backend/srv/srv.h>
#include <libresteam/cb/ops.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/wire/wire.h>

#define SRV_STR_EQUAL LDG_ARITH_ORD_EQ
#define SRV_POOL_VAR 0U
#define SRV_POOL_SLACK (8U * LDG_MEM_POOL_VAR_ALIGN)
#define SRV_WAIT_SOCK_CUNT 1U
#define SRV_WAIT_POLL 0U
#define SRV_FLAGS_REQ 0U
#define SRV_STATUS_REQ 0U
#define SRV_CB_ID_NONE 0U
#define SRV_DEADLINE_HEADROOM 2U

static uint8_t srv_host_loopback_is(const char *host)
{
    uint32_t cmp = UINT32_MAX;

    if (ldg_str_cmp(host, LIBRESTEAM_BACKEND_SRV_HOST_LOOPBACK_V4, LIBRESTEAM_BACKEND_SRV_HOST_MAX, &cmp) == LDG_ERR_AOK && cmp == SRV_STR_EQUAL) { return LDG_TRUTH_TRUE; }

    if (ldg_str_cmp(host, LIBRESTEAM_BACKEND_SRV_HOST_LOOPBACK_V6, LIBRESTEAM_BACKEND_SRV_HOST_MAX, &cmp) == LDG_ERR_AOK && cmp == SRV_STR_EQUAL) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t srv_rd_byte_cb(void *ctx, uint8_t *out)
{
    libresteam_backend_srv_t *srv = 0x0;
    uint64_t bytes_rd = 0;
    uint64_t now = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;

    res = ldg_time_monotonic_get(&now);
    if (res != LDG_ERR_AOK) { return res; }

    if (now > srv->deadline_ns) { return LDG_ERR_TIMEOUT; }

    res = ldg_net_sock_rd(&srv->sock, out, (uint64_t)sizeof(uint8_t), srv->timeout, &bytes_rd);
    if (res != LDG_ERR_AOK) { return res; }

    if (bytes_rd != sizeof(uint8_t)) { return LDG_ERR_NET_RECV; }

    return LDG_ERR_AOK;
}

static uint32_t srv_fail_close(libresteam_backend_srv_t *srv, uint32_t err)
{
    if (ldg_net_sock_live_is(&srv->sock) == LDG_TRUTH_TRUE && ldg_net_sock_close(&srv->sock, srv->timeout) != LDG_ERR_AOK) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "backend_srv: close after stream failure failed"); }

    return err;
}

static uint32_t srv_wr_cb(const ldg_byte_t *data, ldg_dword_t len, void *ctx)
{
    libresteam_backend_srv_t *srv = 0x0;
    uint64_t off = 0;
    uint64_t bytes_wr = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;

    while (off < len)
    {
        bytes_wr = 0;
        res = ldg_net_sock_wr(&srv->sock, &data[off], len - off, srv->timeout, &bytes_wr);
        if (res != LDG_ERR_AOK) { return res; }

        if (bytes_wr == 0) { return LDG_ERR_NET_SEND; }

        off += bytes_wr;
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_push_enqueue(libresteam_backend_srv_t *srv, uint32_t cb_id, const uint8_t *body, uint32_t len)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (srv->push_cunt >= srv->push_slot_cunt) { return LDG_ERR_FULL; }

    if (len > srv->push_slot_size) { return LDG_ERR_OVERFLOW; }

    slot = srv->push_hd + srv->push_cunt;
    if (slot >= srv->push_slot_cunt) { slot -= srv->push_slot_cunt; }

    if (len != 0)
    {
        res = ldg_mem_secure_cpy(&srv->push_buff[(uint64_t)slot * srv->push_slot_size], body, len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    srv->push_len[slot] = len;
    srv->push_cb_id[slot] = cb_id;
    srv->push_cunt++;

    return LDG_ERR_AOK;
}

static uint32_t srv_frame_recv(libresteam_backend_srv_t *srv, libresteam_wire_rd_t *rd, libresteam_wire_hdr_t *hdr)
{
    ldg_dword_t len = 0;
    ldg_dword_t checksum = 0;
    uint64_t now = 0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_time_monotonic_get(&now);
    if (res != LDG_ERR_AOK) { return res; }

    srv->deadline_ns = now + srv->timeout * LDG_NS_PER_MS;

    res = ldg_emiemi_recv(&srv->io, srv->frame, srv->frame_cap, &len, &checksum);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_rd_init(rd, srv->frame, len, hdr);
}

uint32_t libresteam_backend_srv_req_begin(libresteam_backend_srv_t *srv, libresteam_wire_wr_t *wr)
{
    if (LDG_UNLIKELY(!srv || !wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!srv->frame)) { return LDG_ERR_NOT_INIT; }

    return libresteam_wire_wr_init(wr, srv->frame, srv->frame_cap);
}

uint32_t libresteam_backend_srv_call(libresteam_backend_srv_t *srv, uint8_t service, uint8_t op, libresteam_wire_wr_t *wr, libresteam_wire_rd_t *rd_out)
{
    libresteam_wire_hdr_t hdr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_hdr_t reply = LDG_STRUCT_ZERO_INIT;
    ldg_dword_t checksum = 0;
    uint32_t pushes = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !wr || !rd_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(ldg_net_sock_live_is(&srv->sock) != LDG_TRUTH_TRUE)) { return LDG_ERR_NET_DISCONNECTED; }

    srv->seq++;
    if (srv->seq == LIBRESTEAM_WIRE_SEQ_PUSH) { srv->seq++; }

    hdr.flags = SRV_FLAGS_REQ;
    hdr.seq = srv->seq;
    hdr.status = SRV_STATUS_REQ;
    hdr.cb_id = SRV_CB_ID_NONE;
    hdr.service = service;
    hdr.op = op;

    res = libresteam_wire_hdr_finish(wr, &hdr);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_emiemi_send(&srv->io, srv->frame, wr->len, &checksum);
    if (res != LDG_ERR_AOK) { return srv_fail_close(srv, res); }

    while (res == LDG_ERR_AOK)
    {
        res = srv_frame_recv(srv, rd_out, &reply);
        if (res != LDG_ERR_AOK) { return srv_fail_close(srv, res); }

        if ((reply.flags & LIBRESTEAM_WIRE_FLAG_PUSH) != 0)
        {
            pushes++;
            if (LDG_UNLIKELY(pushes > srv->push_slot_cunt)) { return srv_fail_close(srv, LDG_ERR_NET_PROTOCOL); }

            res = srv_push_enqueue(srv, reply.cb_id, &srv->frame[LIBRESTEAM_WIRE_HDR_SIZE], reply.body_len);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "backend_srv: push dropped while waiting for reply"); }

            res = LDG_ERR_AOK;
            continue;
        }

        if ((reply.flags & LIBRESTEAM_WIRE_FLAG_REPLY) == 0 || reply.seq != hdr.seq) { return srv_fail_close(srv, LDG_ERR_NET_PROTOCOL); }

        if (reply.status != LDG_ERR_AOK) { return reply.status; }

        return LDG_ERR_AOK;
    }

    return res;
}

static uint32_t srv_pump(void *ctx, const libresteam_cb_sink_t *sink)
{
    libresteam_backend_srv_t *srv = 0x0;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_hdr_t hdr = LDG_STRUCT_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t len = 0;
    uint32_t cb_id = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t ready = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !sink)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;
    if (LDG_UNLIKELY(!srv->frame || !srv->push_buff || !srv->push_len || !srv->push_cb_id || !srv->deliver)) { return LDG_ERR_NOT_INIT; }

    if (LDG_UNLIKELY(srv->pumping == LDG_TRUTH_TRUE)) { return LDG_ERR_BUSY; }

    srv->pumping = LDG_TRUTH_TRUE;

    while (res == LDG_ERR_AOK && srv->push_cunt < srv->push_slot_cunt && ldg_net_sock_live_is(&srv->sock) == LDG_TRUTH_TRUE)
    {
        ready = LDG_TRUTH_FALSE;
        res = ldg_net_sock_wait(&srv->sock, SRV_WAIT_SOCK_CUNT, SRV_WAIT_POLL, &ready);
        if (res == LDG_ERR_TIMEOUT || (res == LDG_ERR_AOK && ready != LDG_TRUTH_TRUE)) { res = LDG_ERR_AOK; break; }

        if (res != LDG_ERR_AOK) { break; }

        res = srv_frame_recv(srv, &rd, &hdr);
        if (res != LDG_ERR_AOK) { res = srv_fail_close(srv, res); break; }

        if ((hdr.flags & LIBRESTEAM_WIRE_FLAG_PUSH) == 0) { res = srv_fail_close(srv, LDG_ERR_NET_PROTOCOL); break; }

        if (srv_push_enqueue(srv, hdr.cb_id, &srv->frame[LIBRESTEAM_WIRE_HDR_SIZE], hdr.body_len) != LDG_ERR_AOK) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "backend_srv: push dropped while pumping"); }
    }

    while (srv->push_cunt > 0)
    {
        slot = srv->push_hd;
        len = srv->push_len[slot];
        cb_id = srv->push_cb_id[slot];
        if (len != 0 && ldg_mem_secure_cpy(srv->deliver, &srv->push_buff[(uint64_t)slot * srv->push_slot_size], len) != LDG_ERR_AOK) { len = 0; }

        srv->push_hd++;
        if (srv->push_hd == srv->push_slot_cunt) { srv->push_hd = 0; }

        srv->push_cunt--;
        sink->ops->msg_cb(sink->content, LIBRESTEAM_BACKEND_SRV_USER_NONE, cb_id, srv->deliver, len);
    }

    srv->pumping = LDG_TRUTH_FALSE;

    return res;
}

static uint32_t srv_teardown(libresteam_backend_srv_t *srv)
{
    uint32_t res = LDG_ERR_AOK;
    uint32_t first_res = LDG_ERR_AOK;

    if (ldg_net_sock_live_is(&srv->sock) == LDG_TRUTH_TRUE)
    {
        res = ldg_net_sock_close(&srv->sock, srv->timeout);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }
    }

    if (srv->sock_up == LDG_TRUTH_TRUE)
    {
        res = ldg_net_sock_shutdown();
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }

        srv->sock_up = LDG_TRUTH_FALSE;
    }

    if (srv->pool)
    {
        res = ldg_mem_pool_destroy(&srv->pool);
        if (res != LDG_ERR_AOK && first_res == LDG_ERR_AOK) { first_res = res; }
    }

    srv->frame = 0x0;
    srv->push_buff = 0x0;
    srv->push_len = 0x0;
    srv->push_cb_id = 0x0;
    srv->str = 0x0;
    srv->deliver = 0x0;
    srv->push_cunt = 0;
    srv->push_hd = 0;
    srv->pumping = LDG_TRUTH_FALSE;

    return first_res;
}

static uint32_t srv_init_fail(libresteam_backend_srv_t *srv, uint32_t err)
{
    uint32_t res = LDG_ERR_AOK;

    res = srv_teardown(srv);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { ldg_logging_line_wr(LDG_LOGGING_CON, LDG_LOGGING_LVL_WARN, "backend_srv: teardown after init failure failed"); }

    return err;
}

static uint32_t srv_pool_alloc(libresteam_backend_srv_t *srv)
{
    void *alloc = 0x0;
    uint64_t push_bytes = 0;
    uint64_t slot_bytes = 0;
    uint64_t cap = 0;
    uint32_t res = LDG_ERR_AOK;

    push_bytes = (uint64_t)srv->push_slot_cunt * srv->push_slot_size;
    slot_bytes = (uint64_t)srv->push_slot_cunt * sizeof(uint32_t);
    cap = srv->frame_cap + push_bytes + slot_bytes + slot_bytes + LIBRESTEAM_BACKEND_SRV_STR_MAX + srv->push_slot_size + SRV_POOL_SLACK;

    res = ldg_mem_pool_create(SRV_POOL_VAR, cap, &srv->pool);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_pool_alloc(srv->pool, srv->frame_cap, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->frame = (uint8_t *)alloc;

    res = ldg_mem_pool_alloc(srv->pool, push_bytes, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->push_buff = (uint8_t *)alloc;

    res = ldg_mem_pool_alloc(srv->pool, slot_bytes, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->push_len = (uint32_t *)alloc;

    res = ldg_mem_pool_alloc(srv->pool, slot_bytes, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->push_cb_id = (uint32_t *)alloc;

    res = ldg_mem_pool_alloc(srv->pool, LIBRESTEAM_BACKEND_SRV_STR_MAX, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->str = (char *)alloc;

    res = ldg_mem_pool_alloc(srv->pool, srv->push_slot_size, &alloc);
    if (res != LDG_ERR_AOK) { return res; }

    srv->deliver = (uint8_t *)alloc;

    return LDG_ERR_AOK;
}

static uint32_t srv_hello(libresteam_backend_srv_t *srv, const char *account_name)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint64_t steam_id = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backend_srv_req_begin(srv, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_put(&wr, account_name);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, srv->app_id);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call(srv, LIBRESTEAM_WIRE_SERVICE_SESH, LIBRESTEAM_WIRE_SESH_OP_HELLO, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u64_get(&rd, &steam_id);
    if (res != LDG_ERR_AOK) { return res; }

    srv->steam_id.all = steam_id;
    if (libresteam_steam_id_valid_is(srv->steam_id) != LDG_TRUTH_TRUE) { srv->steam_id.all = 0; return LDG_ERR_NET_PROTOCOL; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_srv_init(libresteam_backend_srv_t *srv, const libresteam_backend_srv_conf_t *conf, libresteam_backend_t *out)
{
    const char *host = LIBRESTEAM_BACKEND_SRV_HOST_LOOPBACK_V4;
    uint64_t name_len = 0;
    uint16_t port = LIBRESTEAM_WIRE_DEFAULT_PORT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->ops = 0x0;
    out->ctx = 0x0;

    if (LDG_UNLIKELY(!srv || !conf || !conf->account_name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(conf->push_slot_cunt == 0 || conf->push_slot_size == 0 || conf->push_slot_size > LIBRESTEAM_WIRE_BODY_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(conf->app_id == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(ldg_str_len_get(conf->account_name, LIBRESTEAM_WIRE_ACCOUNT_NAME_MAX + LDG_STR_TERM_SIZE, &name_len) != LDG_ERR_AOK || name_len == 0 || name_len > LIBRESTEAM_WIRE_ACCOUNT_NAME_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (conf->host) { host = conf->host; }

    if (conf->port != LIBRESTEAM_BACKEND_SRV_PORT_DEFAULT) { port = conf->port; }

    if (srv_host_loopback_is(host) != LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

    res = ldg_mem_secure_zero(srv, sizeof(libresteam_backend_srv_t));
    if (res != LDG_ERR_AOK) { return res; }

    srv->timeout = conf->timeout;
    if (srv->timeout == 0) { srv->timeout = LDG_NET_SOCK_TIMEOUT_DEFAULT; }

    if (LDG_UNLIKELY(srv->timeout > UINT64_MAX / LDG_NS_PER_MS / SRV_DEADLINE_HEADROOM)) { return LDG_ERR_FUNC_ARG_INVALID; }

    srv->app_id = conf->app_id;
    srv->push_slot_cunt = conf->push_slot_cunt;
    srv->push_slot_size = conf->push_slot_size;
    srv->frame_cap = LIBRESTEAM_WIRE_PAYLOAD_MAX;

    res = srv_pool_alloc(srv);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    res = ldg_net_sock_init();
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    srv->sock_up = LDG_TRUTH_TRUE;

    res = ldg_net_sock_open(host, port, srv->timeout, &srv->sock);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    srv->io.rd_byte = srv_rd_byte_cb;
    srv->io.wr = srv_wr_cb;
    srv->io.ctx = srv;

    res = srv_hello(srv, conf->account_name);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    res = libresteam_backend_srv_user_stats_ops_fill(srv);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    res = libresteam_backend_srv_friends_ops_fill(srv);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    res = libresteam_backend_srv_cloud_ops_fill(srv);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    res = libresteam_backend_srv_user_ops_fill(srv);
    if (res != LDG_ERR_AOK) { return srv_init_fail(srv, res); }

    srv->cb_ops.pump = srv_pump;
    srv->ops.cb = &srv->cb_ops;

    out->ops = &srv->ops;
    out->ctx = srv;

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_srv_shutdown(libresteam_backend_srv_t *srv)
{
    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_teardown(srv);
}

uint32_t libresteam_backend_srv_steam_id_get(const libresteam_backend_srv_t *srv, libresteam_steam_id_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = srv->steam_id;

    return LDG_ERR_AOK;
}
