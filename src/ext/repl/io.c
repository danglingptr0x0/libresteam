#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/arch/amd64/atomic.h>
#include <dangling/core/macros.h>
#include <dangling/io/ipc.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/sys/info.h>
#include <dangling/thread/yield.h>
#include <dangling/time/time.h>

#include <libresteam/ext/repl/repl.h>

#define IO_POLL_MS 20U
#define IO_OUTPUT_TIMEOUT_MS 5000U
#define IO_ID_NONE 0U

typedef struct io_ctx
{
    ldg_io_ipc_t listener;
    ldg_io_ipc_t peer;
    libresteam_ext_repl_req_t req;
    libresteam_ext_repl_resp_t resp;
    libresteam_ext_repl_hello_t hello;
    uint64_t serial;
    uint64_t active;
    uint64_t stop;
    uint8_t listening;
    uint8_t connected;
    uint8_t pudding[6];
} io_ctx_t;

static uint32_t io_pause(io_ctx_t *io)
{
    if (LDG_LD_ACQUIRE(io->stop) != LDG_TRUTH_FALSE) { return LDG_ERR_CANCELED; }

    return ldg_thread_yield((uint64_t)IO_POLL_MS * LDG_NS_PER_MS);
}

static uint32_t io_flush(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    io_ctx_t *io = ctx->io;
    uint64_t start = 0x0;
    uint64_t now = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_time_monotonic_ns_get(&start);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->resp.res = LIBRESTEAM_EXT_REPL_RESP_MORE;
    for (;;)
    {
        if (LDG_UNLIKELY(LDG_LD_ACQUIRE(io->active) != ctx->resp.id)) { return LDG_ERR_CANCELED; }

        res = ldg_spsc_push(&ctx->resp_queue, &ctx->resp);
        if (res != LDG_ERR_FULL) { break; }

        res = ldg_time_monotonic_ns_get(&now);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (now - start >= (uint64_t)IO_OUTPUT_TIMEOUT_MS * LDG_NS_PER_MS) { return LDG_ERR_TIMEOUT; }

        res = io_pause(io);
        if (res != LDG_ERR_AOK) { return res; }
    }
    if (res == LDG_ERR_AOK)
    {
        ctx->resp.len = 0;
        ctx->resp.text[0] = LDG_STR_TERM;
    }

    return res;
}

uint32_t libresteam_ext_repl_io_init(libresteam_ext_repl_ctx_t *ctx)
{
    io_ctx_t *io = 0x0;
    void *mem = 0x0;
    uint64_t pid = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_alloc(sizeof(*io), &mem);
    if (res != LDG_ERR_AOK) { return res; }

    io = (io_ctx_t *)mem;
    res = ldg_mem_secure_zero(io, sizeof(*io));
    if (res != LDG_ERR_AOK)
    {
        res = ldg_mem_dealloc(io);
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    ctx->io = io;
    ctx->flush = io_flush;
    res = ldg_sys_pid_get(&pid);
    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->pipe_name, sizeof(ctx->pipe_name), &len, LIBRESTEAM_EXT_REPL_PIPE_PREFIX "%u64", pid); }

    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(io->hello.magic, LIBRESTEAM_EXT_REPL_HELLO_MAGIC, sizeof(io->hello.magic)); }

    if (res != LDG_ERR_AOK) { return res; }

    io->hello.version = LIBRESTEAM_EXT_REPL_PROTO_VERSION;
    io->hello.size = (uint32_t)sizeof(io->hello);
    io->hello.pid = pid;
    io->hello.req_cap = LIBRESTEAM_EXT_REPL_CMD_CAP;
    io->hello.resp_cap = LIBRESTEAM_EXT_REPL_RESP_CAP;
    res = ldg_io_ipc_listen(ctx->pipe_name, &io->listener);
    if (res != LDG_ERR_AOK) { return res; }

    io->listening = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_io_stop(libresteam_ext_repl_ctx_t *ctx)
{
    io_ctx_t *io = 0x0;

    if (LDG_UNLIKELY(!ctx || !ctx->io)) { return LDG_ERR_FUNC_ARG_NULL; }

    io = (io_ctx_t *)ctx->io;
    LDG_ST_RELEASE(io->stop, LDG_TRUTH_TRUE);
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_io_shutdown(libresteam_ext_repl_ctx_t *ctx)
{
    io_ctx_t *io = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->io) { return LDG_ERR_AOK; }

    io = (io_ctx_t *)ctx->io;
    if (io->connected) { res |= ldg_io_ipc_close(&io->peer); }

    if (io->listening) { res |= ldg_io_ipc_close(&io->listener); }

    res |= ldg_mem_dealloc(io);
    ctx->io = 0x0;
    return res;
}

static uint32_t io_hello_wr(libresteam_ext_repl_ctx_t *ctx, io_ctx_t *io)
{
    uint64_t sent = 0x0;
    uint32_t res = LDG_ERR_AOK;

    io->hello.thread = LDG_RD_ONCE(ctx->main_thread);
    res = ldg_io_ipc_wr(&io->peer, (const ldg_byte_t *)&io->hello, sizeof(io->hello), &sent);
    if (res != LDG_ERR_AOK) { return res; }

    return sent == sizeof(io->hello) ? LDG_ERR_AOK : LDG_ERR_IO_WR;
}

static uint32_t io_req_rd(io_ctx_t *io, uint64_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    for (;;)
    {
        res = ldg_io_ipc_rd_timed(&io->peer, (ldg_byte_t *)io->req.text, sizeof(io->req.text), IO_POLL_MS, out);
        if (res != LDG_ERR_TIMEOUT) { return res; }

        if (LDG_LD_ACQUIRE(io->stop) != LDG_TRUTH_FALSE) { return LDG_ERR_CANCELED; }
    }
}

static uint32_t io_xchg(libresteam_ext_repl_ctx_t *ctx, io_ctx_t *io)
{
    uint64_t bytes = 0x0;
    uint64_t size = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = io_req_rd(io, &bytes);
    if (res != LDG_ERR_AOK) { return res; }

    if (!bytes || bytes > sizeof(io->req.text) || io->req.text[bytes - LDG_STR_TERM_SIZE] != LDG_STR_TERM) { return LDG_ERR_STR_INVALID; }

    for (idx = 0; idx + LDG_STR_TERM_SIZE < bytes; idx++) { if (io->req.text[idx] == LDG_STR_TERM) { return LDG_ERR_STR_INVALID; } }
    if (io->serial == UINT64_MAX) { return LDG_ERR_OVERFLOW; }

    io->req.id = ++io->serial;
    LDG_ST_RELEASE(io->active, io->req.id);
    while ((res = ldg_spsc_push(&ctx->req_queue, &io->req)) == LDG_ERR_FULL)
    {
        res = io_pause(io);
        if (res != LDG_ERR_AOK) { return res; }
    }
    if (res != LDG_ERR_AOK) { return res; }

    for (;;)
    {
        res = ldg_spsc_pop(&ctx->resp_queue, &io->resp);
        if (res == LDG_ERR_AOK && io->resp.id == io->req.id)
        {
            if (io->resp.len >= sizeof(io->resp.text)) { return LDG_ERR_BOUNDS; }

            size = LIBRESTEAM_EXT_REPL_RESP_HDR_SIZE + io->resp.len + LDG_STR_TERM_SIZE;
            res = ldg_io_ipc_wr(&io->peer, (const ldg_byte_t *)&io->resp, size, &bytes);
            if (res != LDG_ERR_AOK) { return res; }

            if (bytes != size) { return LDG_ERR_IO_WR; }

            if (io->resp.res != LIBRESTEAM_EXT_REPL_RESP_MORE) { return LDG_ERR_AOK; }

            continue;
        }

        if (res != LDG_ERR_AOK && res != LDG_ERR_EMPTY) { return res; }

        res = io_pause(io);
        if (res != LDG_ERR_AOK) { return res; }
    }
}

uint32_t libresteam_ext_repl_io_worker_cb(uint32_t worker_id, void *arg)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)arg;
    io_ctx_t *io = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->io)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (worker_id != 0) { return LDG_ERR_FUNC_ARG_INVALID; }

    io = (io_ctx_t *)ctx->io;
    while (LDG_LD_ACQUIRE(io->stop) == LDG_TRUTH_FALSE)
    {
        res = ldg_io_ipc_accept_timed(&io->listener, IO_POLL_MS, &io->peer);
        if (res == LDG_ERR_TIMEOUT) { continue; }

        if (res != LDG_ERR_AOK) { return res == LDG_ERR_CANCELED ? LDG_ERR_AOK : res; }

        io->connected = LDG_TRUTH_TRUE;
        res = io_hello_wr(ctx, io);
        while (res == LDG_ERR_AOK) { res = io_xchg(ctx, io); }
        LDG_ST_RELEASE(io->active, IO_ID_NONE);
        io->connected = LDG_TRUTH_FALSE;
        close_res = ldg_io_ipc_close(&io->peer);
        if (res == LDG_ERR_CANCELED) { return LDG_ERR_AOK; }

        if (close_res != LDG_ERR_AOK) { return close_res; }
    }
    return LDG_ERR_AOK;
}
