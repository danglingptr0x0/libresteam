#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/net/sock.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>
#include <dangling/time/time.h>
#include <libresteam/ext/repl/mcp.h>

#define MCP_BIND_ADDR "127.0.0.1"
#define MCP_NO_WAIT_MS 0U

#define MCP_REQ_CAP 16384U
#define MCP_HDR_CAP 4096U
#define MCP_TOK_CAP 256U
#define MCP_DEPTH_CAP 16U
#define MCP_ID_CAP 256U
#define MCP_CMD_CAP 256U
#define MCP_ESCAPE_CAP 2048U
#define MCP_REQUEST_MS 5000U
#define MCP_SEND_MS 1000U
#define MCP_JSON_STRING 1U
#define MCP_JSON_NUMBER 2U
#define MCP_JSON_OBJECT 3U
#define MCP_JSON_ARRAY 4U
#define MCP_JSON_LITERAL 5U
#define MCP_HEX_RADIX 16U
#define MCP_DEC_RADIX 10U
#define MCP_HTTP_OK 200U
#define MCP_HTTP_ACCEPTED 202U
#define MCP_HTTP_BAD 400U
#define MCP_HTTP_DENIED 403U
#define MCP_HTTP_NOT_FOUND 404U
#define MCP_HTTP_METHOD 405U
#define MCP_HTTP_LARGE 413U

typedef struct mcp_tok
{
    uint32_t start;
    uint32_t end;
    uint32_t next;
    uint32_t kind;
} mcp_tok_t;

typedef struct mcp_ctx
{
    ldg_net_sock_t listener;
    ldg_net_sock_t peer;
    uint64_t deadline;
    uint8_t listener_up;
    uint8_t peer_up;
    uint8_t pudding[6];
    uint32_t used;
    uint32_t body;
    uint32_t length;
    uint32_t pos;
    uint32_t toks;
    uint32_t send_res;
    uint16_t port;
    uint8_t executing;
    uint8_t startup;
    char req[MCP_REQ_CAP];
    char id[MCP_ID_CAP];
    char scratch[MCP_ESCAPE_CAP];
    mcp_tok_t tok[MCP_TOK_CAP];
} mcp_ctx_t;

static uint8_t mcp_equal_is(const char *lhs, const char *rhs)
{
    uint32_t idx = 0;

    while (lhs[idx] && lhs[idx] == rhs[idx]) { idx++; }
    return lhs[idx] == rhs[idx] ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint64_t mcp_ms_get(void)
{
    uint64_t ns = 0x0;

    if (ldg_time_monotonic_ns_get(&ns) != LDG_ERR_AOK) { return UINT64_MAX; }

    return ns / LDG_NS_PER_MS;
}

static uint8_t mcp_idle_is(uint32_t res)
{
    return res == LDG_ERR_TIMEOUT || res == LDG_ERR_WOULD_BLOCK || res == LDG_ERR_AGAIN ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t mcp_peer_close(mcp_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (ctx->peer_up && ldg_net_sock_close(&ctx->peer, MCP_SEND_MS) != LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    ctx->peer_up = LDG_TRUTH_FALSE;
    ctx->used = 0;
    ctx->body = 0;
    ctx->length = 0;
    ctx->executing = LDG_TRUTH_FALSE;
    return res;
}

static uint32_t mcp_send(mcp_ctx_t *ctx, const char *text, uint32_t len)
{
    uint64_t done = 0x0;
    uint64_t deadline = mcp_ms_get() + MCP_SEND_MS;
    uint32_t pos = 0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->send_res != LDG_ERR_AOK) { return ctx->send_res; }

    while (pos < len)
    {
        res = ldg_net_sock_wr(&ctx->peer, (const ldg_byte_t *)text + pos, len - pos, MCP_SEND_MS, &done);
        if ((res == LDG_ERR_AOK || mcp_idle_is(res) == LDG_TRUTH_TRUE) && done <= len - pos && mcp_ms_get() < deadline)
        {
            pos += (uint32_t)done;
            continue;
        }

        if (res == LDG_ERR_AOK && done == len - pos) { break; }

        ctx->send_res = LDG_ERR_IO_WR;
        return ctx->send_res;
    }
    return LDG_ERR_AOK;
}

static uint32_t mcp_text(mcp_ctx_t *ctx, const char *text)
{
    uint64_t len = 0;
    uint32_t res = ldg_str_len_get(text, MCP_REQ_CAP, &len);

    return res == LDG_ERR_AOK ? mcp_send(ctx, text, (uint32_t)len) : res;
}

static uint32_t mcp_chunk(mcp_ctx_t *ctx, const char *text, uint32_t len)
{
    char hdr[32] = LDG_ARR_ZERO_INIT;
    uint64_t size = 0;
    uint32_t res = LDG_ERR_AOK;

    if (!len) { return LDG_ERR_AOK; }

    res = ldg_str_fmt(hdr, sizeof(hdr), &size, "%x32\r\n", len);
    if (res == LDG_ERR_AOK) { res = mcp_send(ctx, hdr, (uint32_t)size); }

    if (res == LDG_ERR_AOK) { res = mcp_send(ctx, text, len); }

    if (res == LDG_ERR_AOK) { res = mcp_text(ctx, "\r\n"); }

    return res;
}

static uint32_t mcp_chunk_text(mcp_ctx_t *ctx, const char *text)
{
    uint64_t len = 0;
    uint32_t res = ldg_str_len_get(text, MCP_REQ_CAP, &len);

    return res == LDG_ERR_AOK ? mcp_chunk(ctx, text, (uint32_t)len) : res;
}

static uint32_t mcp_http(mcp_ctx_t *ctx, uint32_t status)
{
    char hdr[256] = LDG_ARR_ZERO_INIT;
    uint64_t len = 0;
    uint32_t res = ldg_str_fmt(hdr, sizeof(hdr), &len, "HTTP/1.1 %u32 %s\r\nConnection: close\r\nCache-Control: no-store\r\nX-Content-Type-Options: nosniff\r\n%s\r\n", status, status == MCP_HTTP_OK ? "OK" : "Response", status == MCP_HTTP_OK ? "Content-Type: application/json\r\nTransfer-Encoding: chunked\r\n" : status == MCP_HTTP_METHOD ? "Allow: POST\r\nContent-Length: 0\r\n" : "Content-Length: 0\r\n");

    return res == LDG_ERR_AOK ? mcp_send(ctx, hdr, (uint32_t)len) : res;
}

static uint32_t mcp_envelope(mcp_ctx_t *ctx)
{
    uint32_t res = mcp_http(ctx, MCP_HTTP_OK);

    if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, "{\"jsonrpc\":\"2.0\",\"id\":"); }

    if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, ctx->id); }

    return res;
}

static uint32_t mcp_reply(mcp_ctx_t *ctx, const char *result)
{
    uint32_t res = mcp_envelope(ctx);

    if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, result); }

    if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, "}"); }

    if (res == LDG_ERR_AOK) { res = mcp_text(ctx, "0\r\n\r\n"); }

    return res;
}

static uint32_t mcp_hex_get(mcp_ctx_t *ctx, uint32_t *out)
{
    uint32_t idx = 0;
    uint8_t digit = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    if (ctx->used - ctx->pos < 4U) { return LDG_ERR_STR_INVALID; }

    for (idx = 0; idx < 4U; idx++)
    {
        res = ldg_hex_to_nipple(ctx->req[ctx->pos++], &digit);
        if (res != LDG_ERR_AOK) { return LDG_ERR_STR_INVALID; }

        *out = *out * MCP_HEX_RADIX + digit;
    }
    return LDG_ERR_AOK;
}

static uint32_t mcp_string_read(mcp_ctx_t *ctx, char *out, uint32_t cap)
{
    uint32_t len = 0;
    uint32_t ch = 0;
    uint32_t low = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t remaining = 0;
    uint32_t minimum = 0;

    if (ctx->pos >= ctx->used || ctx->req[ctx->pos++] != '"') { return LDG_ERR_STR_INVALID; }

    while (ctx->pos < ctx->used)
    {
        ch = (uint8_t)ctx->req[ctx->pos++];
        if (ch == '"')
        {
            if (out) { out[len] = 0; }

            return LDG_ERR_AOK;
        }

        if (ch < LDG_ASCII_SPACE) { return LDG_ERR_STR_INVALID; }

        if (ch >= 0x80U)
        {
            if (ch >= 0xc2U && ch <= 0xdfU) { remaining = 1; minimum = 0x80U; ch &= 0x1fU; }
            else if (ch >= 0xe0U && ch <= 0xefU) { remaining = 2; minimum = 0x800U; ch &= 0x0fU; }
            else if (ch >= 0xf0U && ch <= 0xf4U) { remaining = 3; minimum = 0x10000U; ch &= 0x07U; }
            else { return LDG_ERR_STR_INVALID; }

            while (remaining)
            {
                if (ctx->pos >= ctx->used) { return LDG_ERR_STR_INVALID; }

                low = (uint8_t)ctx->req[ctx->pos++];
                if ((low & 0xc0U) != 0x80U) { return LDG_ERR_STR_INVALID; }

                ch = (ch << 6U) | (low & 0x3fU);
                remaining--;
            }
            if (ch < minimum || ch > 0x10ffffU || (ch >= 0xd800U && ch <= 0xdfffU)) { return LDG_ERR_STR_INVALID; }
        }

        if (ch == '\\')
        {
            if (ctx->pos == ctx->used) { return LDG_ERR_STR_INVALID; }

            ch = (uint8_t)ctx->req[ctx->pos++];
            if (ch == 'u')
            {
                res = mcp_hex_get(ctx, &ch);
                if (res != LDG_ERR_AOK) { return res; }

                if (ch >= 0xd800U && ch <= 0xdbffU)
                {
                    if (ctx->used - ctx->pos < 6U || ctx->req[ctx->pos++] != '\\' || ctx->req[ctx->pos++] != 'u') { return LDG_ERR_STR_INVALID; }

                    res = mcp_hex_get(ctx, &low);
                    if (res != LDG_ERR_AOK || low < 0xdc00U || low > 0xdfffU) { return LDG_ERR_STR_INVALID; }

                    ch = 0x10000U;
                }
                else if (ch >= 0xdc00U && ch <= 0xdfffU) { return LDG_ERR_STR_INVALID; }
            }
            else if (ch == 'n') { ch = '\n'; }
            else if (ch == 'r') { ch = '\r'; }
            else if (ch == 't') { ch = '\t'; }
            else if (ch == 'b') { ch = '\b'; }
            else if (ch == 'f') { ch = '\f'; }
            else if (ch != '"' && ch != '\\' && ch != '/') { return LDG_ERR_STR_INVALID; }
        }

        if (out)
        {
            if (!ch || ch > LDG_ASCII_PRINTABLE_MAX || len + 1U >= cap) { return LDG_ERR_STR_INVALID; }

            out[len++] = (char)ch;
        }
    }
    return LDG_ERR_STR_INVALID;
}

static uint32_t mcp_space_skip(mcp_ctx_t *ctx)
{
    while (ctx->pos < ctx->used && (ctx->req[ctx->pos] == ' ' || ctx->req[ctx->pos] == '\t' || ctx->req[ctx->pos] == '\r' || ctx->req[ctx->pos] == '\n')) { ctx->pos++; }
    return LDG_ERR_AOK;
}

static uint32_t mcp_value_read(mcp_ctx_t *ctx, uint32_t depth)
{
    uint32_t idx = ctx->toks;
    uint32_t res = mcp_space_skip(ctx);
    uint32_t start = 0;
    char ch = 0;
    char close = 0;
    const char *literal = 0x0;

    if (res != LDG_ERR_AOK || depth > MCP_DEPTH_CAP || idx == MCP_TOK_CAP || ctx->pos >= ctx->used) { return LDG_ERR_STR_INVALID; }

    ctx->toks++;
    ctx->tok[idx].start = ctx->pos;
    ch = ctx->req[ctx->pos];
    if (ch == '"') { ctx->tok[idx].kind = MCP_JSON_STRING; res = mcp_string_read(ctx, 0x0, 0); }
    else if (ch == '{' || ch == '[')
    {
        ctx->tok[idx].kind = ch == '{' ? MCP_JSON_OBJECT : MCP_JSON_ARRAY;
        close = ch == '{' ? '}' : ']';
        ctx->pos++;
        res = mcp_space_skip(ctx);
        if (ctx->pos < ctx->used && ctx->req[ctx->pos] != close)
        {
            for (;;)
            {
                if (ch == '{')
                {
                    if (ctx->pos >= ctx->used || ctx->req[ctx->pos] != '"') { return LDG_ERR_STR_INVALID; }

                    res = mcp_value_read(ctx, depth + 1U);
                    if (res == LDG_ERR_AOK) { res = mcp_space_skip(ctx); }

                    if (res != LDG_ERR_AOK || ctx->pos >= ctx->used || ctx->req[ctx->pos++] != ':') { return LDG_ERR_STR_INVALID; }
                }

                res = mcp_value_read(ctx, depth + 1U);
                if (res == LDG_ERR_AOK) { res = mcp_space_skip(ctx); }

                if (res != LDG_ERR_AOK || ctx->pos >= ctx->used) { return LDG_ERR_STR_INVALID; }

                if (ctx->req[ctx->pos] == close) { break; }

                if (ctx->req[ctx->pos++] != ',') { return LDG_ERR_STR_INVALID; }

                res = mcp_space_skip(ctx);
                if (res != LDG_ERR_AOK) { return res; }
            }
        }

        if (ctx->pos >= ctx->used || ctx->req[ctx->pos++] != close) { return LDG_ERR_STR_INVALID; }
    }
    else if (ch == '-' || (ch >= '0' && ch <= '9'))
    {
        ctx->tok[idx].kind = MCP_JSON_NUMBER;
        if (ch == '-') { ctx->pos++; }

        if (ctx->pos >= ctx->used) { return LDG_ERR_STR_INVALID; }

        if (ctx->req[ctx->pos] == '0') { ctx->pos++; }
        else
        {
            start = ctx->pos;
            while (ctx->pos < ctx->used && ctx->req[ctx->pos] >= '0' && ctx->req[ctx->pos] <= '9') { ctx->pos++; }
            if (start == ctx->pos) { return LDG_ERR_STR_INVALID; }
        }

        if (ctx->pos < ctx->used && ctx->req[ctx->pos] == '.')
        {
            start = ++ctx->pos;
            while (ctx->pos < ctx->used && ctx->req[ctx->pos] >= '0' && ctx->req[ctx->pos] <= '9') { ctx->pos++; }
            if (start == ctx->pos) { return LDG_ERR_STR_INVALID; }
        }

        if (ctx->pos < ctx->used && (ctx->req[ctx->pos] == 'e' || ctx->req[ctx->pos] == 'E'))
        {
            ctx->pos++;
            if (ctx->pos < ctx->used && (ctx->req[ctx->pos] == '+' || ctx->req[ctx->pos] == '-')) { ctx->pos++; }

            start = ctx->pos;
            while (ctx->pos < ctx->used && ctx->req[ctx->pos] >= '0' && ctx->req[ctx->pos] <= '9') { ctx->pos++; }
            if (start == ctx->pos) { return LDG_ERR_STR_INVALID; }
        }
    }
    else
    {
        ctx->tok[idx].kind = MCP_JSON_LITERAL;
        literal = ch == 't' ? "true" : ch == 'f' ? "false" : "null";
        for (start = 0; literal[start]; start++) { if (ctx->pos >= ctx->used || ctx->req[ctx->pos++] != literal[start]) { return LDG_ERR_STR_INVALID; } }
    }

    ctx->tok[idx].end = ctx->pos;
    ctx->tok[idx].next = ctx->toks;
    return res;
}

static uint32_t mcp_string_get(mcp_ctx_t *ctx, uint32_t idx, char *out, uint32_t cap)
{
    if (idx >= ctx->toks || ctx->tok[idx].kind != MCP_JSON_STRING) { return LDG_ERR_STR_INVALID; }

    ctx->pos = ctx->tok[idx].start;
    return mcp_string_read(ctx, out, cap);
}

static uint32_t mcp_member_get(mcp_ctx_t *ctx, uint32_t object, const char *name, uint32_t *out)
{
    char key[MCP_CMD_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = UINT32_MAX;
    if (object >= ctx->toks || ctx->tok[object].kind != MCP_JSON_OBJECT) { return LDG_ERR_STR_INVALID; }

    for (idx = object + 1U; idx < ctx->tok[object].next; idx = ctx->tok[idx + 1U].next)
    {
        res = mcp_string_get(ctx, idx, key, sizeof(key));
        if (res != LDG_ERR_AOK) { return res; }

        if (mcp_equal_is(key, name) == LDG_TRUTH_TRUE)
        {
            if (*out != UINT32_MAX) { return LDG_ERR_STR_INVALID; }

            *out = idx + 1U;
        }
    }
    return LDG_ERR_AOK;
}

static uint32_t mcp_rpc(mcp_ctx_t *ctx, char *cmd, uint32_t cap, uint8_t *ready)
{
    char method[MCP_CMD_CAP] = LDG_ARR_ZERO_INIT;
    char name[MCP_CMD_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = UINT32_MAX;
    uint32_t params = UINT32_MAX;
    uint32_t args = UINT32_MAX;
    uint32_t id = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    uint32_t pos = 0;

    ctx->pos = ctx->body;
    ctx->toks = 0;
    res = ldg_str_rbrcpy(ctx->id, "null", sizeof(ctx->id));
    if (res == LDG_ERR_AOK) { res = mcp_value_read(ctx, 0); }

    if (res == LDG_ERR_AOK) { res = mcp_space_skip(ctx); }

    if (res != LDG_ERR_AOK || ctx->pos != ctx->used) { return mcp_reply(ctx, ",\"error\":{\"code\":-32700,\"message\":\"Invalid JSON\"}"); }

    res = mcp_member_get(ctx, 0, "id", &id);
    if (res == LDG_ERR_AOK && id != UINT32_MAX) { if ((ctx->tok[id].kind != MCP_JSON_NUMBER && ctx->tok[id].kind != MCP_JSON_STRING) || ctx->tok[id].end - ctx->tok[id].start >= sizeof(ctx->id)) { res = LDG_ERR_STR_INVALID; }
        else
        {
            for (pos = ctx->tok[id].start; pos < ctx->tok[id].end; pos++) { ctx->id[pos - ctx->tok[id].start] = ctx->req[pos]; }
            ctx->id[pos - ctx->tok[id].start] = 0;
        } }

    if (res == LDG_ERR_AOK) { res = mcp_member_get(ctx, 0, "jsonrpc", &idx); }

    if (res == LDG_ERR_AOK) { res = mcp_string_get(ctx, idx, name, sizeof(name)); }

    if (res == LDG_ERR_AOK && mcp_equal_is(name, "2.0") != LDG_TRUTH_TRUE) { res = LDG_ERR_STR_INVALID; }

    if (res == LDG_ERR_AOK) { res = mcp_member_get(ctx, 0, "method", &idx); }

    if (res == LDG_ERR_AOK) { res = mcp_string_get(ctx, idx, method, sizeof(method)); }

    if (res == LDG_ERR_AOK) { res = mcp_member_get(ctx, 0, "params", &params); }

    if (res != LDG_ERR_AOK) { return mcp_reply(ctx, ",\"error\":{\"code\":-32600,\"message\":\"Invalid request\"}"); }

    if (id == UINT32_MAX) { return mcp_http(ctx, MCP_HTTP_ACCEPTED); }

    if (mcp_equal_is(method, "initialize") == LDG_TRUTH_TRUE)
    {
        res = mcp_member_get(ctx, params, "protocolVersion", &idx);
        if (res == LDG_ERR_AOK) { res = mcp_string_get(ctx, idx, name, sizeof(name)); }

        if (res != LDG_ERR_AOK) { return mcp_reply(ctx, ",\"error\":{\"code\":-32602,\"message\":\"Missing protocolVersion\"}"); }

        if (mcp_equal_is(name, "2025-06-18") != LDG_TRUTH_TRUE && mcp_equal_is(name, "2025-11-25") != LDG_TRUTH_TRUE) { res = ldg_str_rbrcpy(name, "2025-03-26", sizeof(name)); }

        if (res == LDG_ERR_AOK) { res = mcp_envelope(ctx); }

        if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, ",\"result\":{\"protocolVersion\":\""); }

        if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, name); }

        if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, "\",\"capabilities\":{\"tools\":{}},\"serverInfo\":{\"name\":\"libresteam-repl\",\"version\":\"1.0\"},\"instructions\":\"Commands execute visibly in the connected user prompt. State, aliases and variables are shared. Do not retry a disconnected mutating call automatically.\"}}"); }

        if (res == LDG_ERR_AOK) { res = mcp_text(ctx, "0\r\n\r\n"); }

        return res;
    }

    if (mcp_equal_is(method, "ping") == LDG_TRUTH_TRUE) { return mcp_reply(ctx, ",\"result\":{}"); }

    if (mcp_equal_is(method, "tools/list") == LDG_TRUTH_TRUE) { return mcp_reply(ctx, ",\"result\":{\"tools\":[{\"name\":\"repl_execute\",\"description\":\"Execute one command through the visible LibreSteam prompt. Supports every REPL command, aliases, variables and selected backend. Output is also printed in the terminal. Commands can modify the target process.\",\"inputSchema\":{\"type\":\"object\",\"properties\":{\"command\":{\"type\":\"string\",\"minLength\":1,\"maxLength\":255}},\"required\":[\"command\"],\"additionalProperties\":false},\"annotations\":{\"readOnlyHint\":false,\"destructiveHint\":true,\"idempotentHint\":false}}]}"); }

    if (mcp_equal_is(method, "tools/call") != LDG_TRUTH_TRUE) { return mcp_reply(ctx, ",\"error\":{\"code\":-32601,\"message\":\"Method not found\"}"); }

    res = mcp_member_get(ctx, params, "name", &idx);
    if (res == LDG_ERR_AOK) { res = mcp_string_get(ctx, idx, name, sizeof(name)); }

    if (res == LDG_ERR_AOK && mcp_equal_is(name, "repl_execute") != LDG_TRUTH_TRUE) { res = LDG_ERR_NOT_FOUND; }

    if (res == LDG_ERR_AOK) { res = mcp_member_get(ctx, params, "arguments", &args); }

    if (res == LDG_ERR_AOK) { res = mcp_member_get(ctx, args, "command", &idx); }

    if (res == LDG_ERR_AOK && ctx->tok[args].next != args + 3U) { res = LDG_ERR_STR_INVALID; }

    if (res == LDG_ERR_AOK) { res = mcp_string_get(ctx, idx, cmd, cap); }

    if (res == LDG_ERR_AOK && !cmd[0]) { res = LDG_ERR_STR_INVALID; }

    for (pos = 0; res == LDG_ERR_AOK && cmd[pos]; pos++) { if ((uint8_t)cmd[pos] < LDG_ASCII_PRINTABLE_MIN && cmd[pos] != '\t') { res = LDG_ERR_STR_INVALID; } }
    if (res != LDG_ERR_AOK) { return mcp_reply(ctx, ",\"error\":{\"code\":-32602,\"message\":\"Expected repl_execute with one ASCII command of 1 to 255 characters\"}"); }

    res = mcp_envelope(ctx);
    if (res == LDG_ERR_AOK) { res = mcp_chunk_text(ctx, ",\"result\":{\"content\":[{\"type\":\"text\",\"text\":\""); }

    if (res == LDG_ERR_AOK) { ctx->executing = LDG_TRUTH_TRUE; *ready = LDG_TRUTH_TRUE; }

    return res;
}

static uint32_t mcp_headers(mcp_ctx_t *ctx, uint32_t *status)
{
    char host[64] = LDG_ARR_ZERO_INIT;
    char local[64] = LDG_ARR_ZERO_INIT;
    uint64_t formatted = 0;
    uint64_t length = 0;
    uint32_t pos = 0;
    uint32_t start = 0;
    uint32_t colon = 0;
    uint32_t end = 0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t have_host = LDG_TRUTH_FALSE;
    uint8_t have_length = LDG_TRUTH_FALSE;
    uint8_t have_type = LDG_TRUTH_FALSE;
    char *value = 0x0;

    *status = MCP_HTTP_BAD;
    while (pos + 1U < ctx->body && !(ctx->req[pos] == '\r' && ctx->req[pos + 1U] == '\n')) { pos++; }
    ctx->req[pos] = 0;
    if (mcp_equal_is(ctx->req, "POST /mcp HTTP/1.1") != LDG_TRUTH_TRUE)
    {
        *status = mcp_equal_is(ctx->req, "GET /mcp HTTP/1.1") == LDG_TRUTH_TRUE || mcp_equal_is(ctx->req, "DELETE /mcp HTTP/1.1") == LDG_TRUTH_TRUE ? MCP_HTTP_METHOD : MCP_HTTP_NOT_FOUND;
        return LDG_ERR_AOK;
    }

    res = ldg_str_fmt(host, sizeof(host), &formatted, "127.0.0.1:%u32", (uint32_t)ctx->port);
    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(local, sizeof(local), &formatted, "localhost:%u32", (uint32_t)ctx->port); }

    if (res != LDG_ERR_AOK) { return res; }

    pos += 2U;
    while (pos + 2U < ctx->body)
    {
        start = pos;
        while (pos + 1U < ctx->body && !(ctx->req[pos] == '\r' && ctx->req[pos + 1U] == '\n')) { pos++; }
        end = pos;
        pos += 2U;
        colon = start;
        while (colon < end && ctx->req[colon] != ':') { colon++; }
        if (colon == start || colon == end) { return LDG_ERR_AOK; }

        for (idx = start; idx < colon; idx++) { if (ctx->req[idx] >= 'A' && ctx->req[idx] <= 'Z') { ctx->req[idx] += 'a' - 'A'; }
            else if ((ctx->req[idx] < 'a' || ctx->req[idx] > 'z') && ctx->req[idx] != '-') { return LDG_ERR_AOK; } }
        ctx->req[colon++] = 0;
        while (colon < end && (ctx->req[colon] == ' ' || ctx->req[colon] == '\t')) { colon++; }
        while (end > colon && (ctx->req[end - 1U] == ' ' || ctx->req[end - 1U] == '\t')) { end--; }
        ctx->req[end] = 0;
        value = ctx->req + colon;
        if (mcp_equal_is(ctx->req + start, "origin") == LDG_TRUTH_TRUE) { *status = MCP_HTTP_DENIED; return LDG_ERR_AOK; }

        if (mcp_equal_is(ctx->req + start, "transfer-encoding") == LDG_TRUTH_TRUE || mcp_equal_is(ctx->req + start, "expect") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        if (mcp_equal_is(ctx->req + start, "host") == LDG_TRUTH_TRUE)
        {
            if (have_host || (mcp_equal_is(value, host) != LDG_TRUTH_TRUE && mcp_equal_is(value, local) != LDG_TRUTH_TRUE)) { *status = MCP_HTTP_DENIED; return LDG_ERR_AOK; }

            have_host = LDG_TRUTH_TRUE;
        }

        if (mcp_equal_is(ctx->req + start, "content-length") == LDG_TRUTH_TRUE)
        {
            if (have_length || colon == end) { return LDG_ERR_AOK; }

            for (idx = colon; idx < end; idx++)
            {
                if (ctx->req[idx] < '0' || ctx->req[idx] > '9') { return LDG_ERR_AOK; }

                length = length * MCP_DEC_RADIX + (uint32_t)(ctx->req[idx] - '0');
                if (length >= MCP_REQ_CAP - ctx->body) { *status = MCP_HTTP_LARGE; return LDG_ERR_AOK; }
            }
            ctx->length = (uint32_t)length;
            have_length = LDG_TRUTH_TRUE;
        }

        if (mcp_equal_is(ctx->req + start, "content-type") == LDG_TRUTH_TRUE)
        {
            if (have_type) { return LDG_ERR_AOK; }

            for (idx = colon; idx < end && ctx->req[idx] != ';'; idx++) {}
            while (idx > colon && ctx->req[idx - 1U] == ' ') { idx--; }
            ctx->req[idx] = 0;
            if (mcp_equal_is(value, "application/json") != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

            have_type = LDG_TRUTH_TRUE;
        }

        if (mcp_equal_is(ctx->req + start, "mcp-protocol-version") == LDG_TRUTH_TRUE) { if (mcp_equal_is(value, "2025-03-26") != LDG_TRUTH_TRUE && mcp_equal_is(value, "2025-06-18") != LDG_TRUTH_TRUE && mcp_equal_is(value, "2025-11-25") != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; } }
    }
    if (have_host && have_length && have_type && ctx->length) { *status = MCP_HTTP_OK; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_mcp_poll(void *opaque, char *cmd, uint32_t cap, uint8_t *ready)
{
    mcp_ctx_t *ctx = opaque;
    uint64_t done = 0x0;
    uint64_t room = 0x0;
    uint32_t idx = 0;
    uint32_t status = MCP_HTTP_OK;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !cmd || !ready || cap < MCP_CMD_CAP)) { return LDG_ERR_FUNC_ARG_INVALID; }

    *ready = LDG_TRUTH_FALSE;
    if (ctx->executing) { return LDG_ERR_BUSY; }

    if (!ctx->peer_up)
    {
        res = ldg_net_sock_accept(&ctx->listener, MCP_NO_WAIT_MS, &ctx->peer);
        if (mcp_idle_is(res) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        if (res != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        ctx->peer_up = LDG_TRUTH_TRUE;
        ctx->send_res = LDG_ERR_AOK;
        ctx->deadline = mcp_ms_get() + MCP_REQUEST_MS;
    }

    if (mcp_ms_get() >= ctx->deadline) { return mcp_peer_close(ctx); }

    room = sizeof(ctx->req) - ctx->used - LDG_STR_TERM_SIZE;
    res = ldg_net_sock_rd(&ctx->peer, (ldg_byte_t *)ctx->req + ctx->used, room, MCP_NO_WAIT_MS, &done);
    if (mcp_idle_is(res) == LDG_TRUTH_TRUE || (res == LDG_ERR_AOK && !done)) { return LDG_ERR_AOK; }

    if (res != LDG_ERR_AOK || done > room) { return mcp_peer_close(ctx); }

    res = LDG_ERR_AOK;
    ctx->used += (uint32_t)done;
    ctx->req[ctx->used] = 0;
    if (!ctx->body)
    {
        for (idx = 0; idx + 3U < ctx->used; idx++)
        {
            if (!ctx->req[idx]) { status = MCP_HTTP_BAD; break; }

            if (ctx->req[idx] == '\r' && ctx->req[idx + 1U] == '\n' && ctx->req[idx + 2U] == '\r' && ctx->req[idx + 3U] == '\n') { ctx->body = idx + 4U; break; }
        }
        if (ctx->body > MCP_HDR_CAP || (!ctx->body && ctx->used >= MCP_HDR_CAP)) { status = MCP_HTTP_LARGE; }

        if (status == MCP_HTTP_OK && ctx->body) { res = mcp_headers(ctx, &status); }
    }

    if (res != LDG_ERR_AOK) { status = MCP_HTTP_BAD; }

    if (status != MCP_HTTP_OK)
    {
        res = mcp_http(ctx, status);
        if (res != LDG_ERR_AOK) { ctx->send_res = res; }

        return mcp_peer_close(ctx);
    }

    if (!ctx->body || ctx->used < ctx->body + ctx->length) { return LDG_ERR_AOK; }

    if (ctx->used != ctx->body + ctx->length) { res = mcp_http(ctx, MCP_HTTP_BAD); }
    else { res = mcp_rpc(ctx, cmd, cap, ready); }

    if (res != LDG_ERR_AOK || !*ready) { return mcp_peer_close(ctx); }

    return res;
}

uint32_t libresteam_ext_repl_mcp_wr(void *opaque, const char *text, uint32_t len)
{
    mcp_ctx_t *ctx = opaque;
    uint32_t idx = 0;
    uint32_t used = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t ch = 0;

    if (LDG_UNLIKELY(!ctx || !text)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->executing) { return LDG_ERR_NOT_INIT; }

    for (idx = 0; idx < len; idx++)
    {
        ch = (uint8_t)text[idx];
        if (used + 6U > sizeof(ctx->scratch))
        {
            res = mcp_chunk(ctx, ctx->scratch, used);
            if (res != LDG_ERR_AOK) { return res; }

            used = 0;
        }

        if (ch == '"' || ch == '\\') { ctx->scratch[used++] = '\\'; ctx->scratch[used++] = (char)ch; }
        else if (ch < LDG_ASCII_SPACE || ch > LDG_ASCII_PRINTABLE_MAX)
        {
            ctx->scratch[used++] = '\\';
            ctx->scratch[used++] = 'u';
            ctx->scratch[used++] = '0';
            ctx->scratch[used++] = '0';
            res = ldg_byte_to_hex(ch, ctx->scratch + used);
            if (res != LDG_ERR_AOK) { return res; }

            used += 2U;
        }
        else { ctx->scratch[used++] = (char)ch; }
    }
    return mcp_chunk(ctx, ctx->scratch, used);
}

uint32_t libresteam_ext_repl_mcp_finish(void *opaque, uint32_t result)
{
    mcp_ctx_t *ctx = opaque;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->executing) { return LDG_ERR_NOT_INIT; }

    res = mcp_chunk_text(ctx, result == LDG_ERR_AOK ? "\"}],\"isError\":false}}" : "\"}],\"isError\":true}}");
    if (res == LDG_ERR_AOK) { res = mcp_text(ctx, "0\r\n\r\n"); }

    cleanup = mcp_peer_close(ctx);
    return res == LDG_ERR_AOK ? cleanup : res;
}

uint32_t libresteam_ext_repl_mcp_stop(void *opaque)
{
    mcp_ctx_t *ctx = opaque;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = mcp_peer_close(ctx);
    if (ctx->listener_up && ldg_net_sock_close(&ctx->listener, MCP_SEND_MS) != LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    if (ctx->startup && ldg_net_sock_shutdown() != LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    cleanup = ldg_mem_dealloc(ctx);
    return res == LDG_ERR_AOK ? cleanup : res;
}

uint32_t libresteam_ext_repl_mcp_init(void **out, uint16_t port)
{
    mcp_ctx_t *ctx = 0x0;
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;
    if (!port) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = ldg_mem_alloc(sizeof(*ctx), &mem);
    if (res != LDG_ERR_AOK) { return res; }

    ctx = mem;
    res = ldg_mem_secure_zero(ctx, sizeof(*ctx));
    ctx->port = port;
    if (res == LDG_ERR_AOK && ldg_net_sock_init() != LDG_ERR_AOK) { res = LDG_ERR_NOT_INIT; }

    if (res == LDG_ERR_AOK)
    {
        ctx->startup = LDG_TRUTH_TRUE;
        if (ldg_net_sock_listen(MCP_BIND_ADDR, port, &ctx->listener) != LDG_ERR_AOK) { res = LDG_ERR_IO_OPEN; }
        else { ctx->listener_up = LDG_TRUTH_TRUE; }
    }

    if (res != LDG_ERR_AOK) { cleanup = libresteam_ext_repl_mcp_stop(ctx); return cleanup == LDG_ERR_AOK ? res : cleanup; }

    *out = ctx;
    return LDG_ERR_AOK;
}
