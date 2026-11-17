#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/bits.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/io/console.h>
#include <dangling/io/file.h>
#include <dangling/io/ipc.h>
#include <dangling/io/path.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>
#include <dangling/sys/proc.h>
#include <dangling/sys/tty.h>
#include <dangling/thread/yield.h>

#include <libresteam/ext/repl/repl.h>
#include <libresteam/ext/repl/mcp.h>

#define CLIENT_PID_DIGITS_MAX 10U
#define CLIENT_PIPE_CAP 16U
#define CLIENT_TGT_CUNT 1U
#define CLIENT_LINE_END "\r\n"
#define CLIENT_HELLO_POLL_MS 20U
#define CLIENT_HELLO_TRIES 250U
#define CLIENT_KEY_BASE 0x100U
#define CLIENT_KEY_ESC (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_ESC)
#define CLIENT_KEY_ENTER (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_ENTER)
#define CLIENT_KEY_TAB (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_TAB)
#define CLIENT_KEY_BACKSPACE (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_BACKSPACE)
#define CLIENT_KEY_UP (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_UP)
#define CLIENT_KEY_DOWN (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_DOWN)
#define CLIENT_KEY_LHS (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_LHS)
#define CLIENT_KEY_RHS (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_RHS)
#define CLIENT_KEY_HOME (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_HOME)
#define CLIENT_KEY_END (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_END)
#define CLIENT_KEY_PAGE_UP (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_PAGE_UP)
#define CLIENT_KEY_PAGE_DOWN (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_PAGE_DOWN)
#define CLIENT_KEY_DELETE (CLIENT_KEY_BASE + LDG_IO_CONSOLE_KEY_DELETE)
#define CLIENT_CTRL_BYTE_MAX 0x1FU
#define CLIENT_CTRL_BYTE_OFF 0x40U
#define CLIENT_CASE_OFF 0x20U
#define CLIENT_ERASE_BELOW "\r\x1B[J"
#define CLIENT_ROW_UP_FMT "\x1B[%u32A"
#define CLIENT_COL_FMT "\x1B[%u32G"
#define CLIENT_COL_BASE 1U
#define CLIENT_SEARCH_SEP_COLS 2U
#define CLIENT_WIDTH_MIN 2U
#define CLIENT_LOCK_SUFFIX ".lock"
#define CLIENT_GHOST_ON "\x1B[90m"
#define CLIENT_GHOST_OFF "\x1B[39m"
#define CLIENT_ALIAS_CAP 32U
#define CLIENT_ALIAS_NAME_CAP 32U
#define CLIENT_VAR_CAP 32U
#define CLIENT_VAR_NAME_CAP 32U
#define CLIENT_HEX_ROW_CAP 16U
#define CLIENT_HEX_BYTE_COLS 3U
#define CLIENT_HEX_ADDR_DIGITS 16U
#define CLIENT_HEX_SEPARATOR_COLS 2U
#define CLIENT_TERMINAL_FALLBACK_COLS 132U
#define CLIENT_FILE_ALIGN 4096U
#define CLIENT_FILE_ACCESS 0600U
#define CLIENT_ADDR_CAP 64U
#define CLIENT_ADDR_DIGITS_MIN 5U
#define CLIENT_CANDIDATE_ROWS 10U
#define CLIENT_PROMPT "repl> "
#define CLIENT_PROMPT_COLOR "\x1b[38;2;80;0;255mrepl> \x1b[39m"
#define CLIENT_SCREEN_CLEAR "\x1b[2J\x1b[H"

#define CLIENT_HISTORY_CAP 1000U
#define CLIENT_HISTORY_PATH_CAP 1024U
#define CLIENT_HISTORY_KEY_CAP 200U
#define CLIENT_IMAGE_CAP 32768U
#define CLIENT_HISTORY_HEADER "LIBRESTEAM_REPL_HISTORY_1\n"
#define CLIENT_HISTORY_BYTES (CLIENT_HISTORY_CAP * (LIBRESTEAM_EXT_REPL_CMD_CAP + 3U) + CLIENT_FILE_ALIGN)
#define CLIENT_HISTORY_COMMAND 'C'
#define CLIENT_HISTORY_ALIAS 'A'
#define CLIENT_SEARCH_PROMPT "(reverse-i-search) "

typedef struct client_history_entry
{
    char text[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint8_t kind;
    uint8_t pudding[3];
} client_history_entry_t;

typedef struct client_alias
{
    char name[CLIENT_ALIAS_NAME_CAP];
    char text[LIBRESTEAM_EXT_REPL_CMD_CAP];
} client_alias_t;

typedef struct client_var
{
    char name[CLIENT_VAR_NAME_CAP];
    char val[LIBRESTEAM_EXT_REPL_CMD_CAP];
} client_var_t;

typedef struct client_ctx
{
    void *mcp;
    uint32_t command_res;
    uint32_t mcp_res;
    uint16_t mcp_port;
    uint8_t tee;
    uint8_t closing;
    ldg_io_ipc_t pipe;
    ldg_io_ipc_name_t names[CLIENT_PIPE_CAP];
    libresteam_ext_repl_hello_t hello;
    libresteam_ext_repl_resp_t resp;
    client_alias_t aliases[CLIENT_ALIAS_CAP];
    client_var_t vars[CLIENT_VAR_CAP];
    char candidates[LIBRESTEAM_EXT_REPL_COMPLETE_CAP][LIBRESTEAM_EXT_REPL_CMD_CAP];
    char labels[LIBRESTEAM_EXT_REPL_COMPLETE_CAP][LIBRESTEAM_EXT_REPL_NAME_CAP];
    char addresses[CLIENT_ADDR_CAP][LIBRESTEAM_EXT_REPL_CMD_CAP];
    char complete_last[LIBRESTEAM_EXT_REPL_CMD_CAP];
    char complete_orig[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint64_t previous;
    uint32_t addr_next;
    uint32_t candidate_cunt;
    uint32_t candidate_start;
    uint32_t candidate_end;
    uint32_t candidate_sel;
    uint8_t candidate_on;
    uint8_t candidate_more;
    uint8_t candidate_space;
    uint8_t candidate_redraw;
    uint32_t candidate_rows;
    uint32_t candidate_width;
    uint8_t candidate_drawn;
    uint8_t candidate_pudding[3];
    char cmd[LIBRESTEAM_EXT_REPL_CMD_CAP];
    char path[LIBRESTEAM_EXT_REPL_NAME_CAP];
    char line[512];
    ldg_io_file_t *file;
    uint64_t file_len;
    uint32_t file_used;
    uint32_t file_res;
    uint8_t output_line_open;
    uint8_t ansi;
    uint8_t interactive;
    uint8_t pudding[1];
    client_history_entry_t history[CLIENT_HISTORY_CAP];
    uint32_t history_cunt;
    uint32_t history_sel;
    uint32_t edit_cursor;
    uint32_t edit_row;
    uint32_t edit_width;
    uint8_t edit_drawn;
    uint8_t edit_origin_pudding[3];
    uint32_t search_len;
    uint32_t search_sel;
    uint32_t draft_cursor;
    uint8_t searching;
    uint8_t history_loading;
    uint8_t history_alias;
    uint8_t history_up;
    char draft[LIBRESTEAM_EXT_REPL_CMD_CAP];
    char search[LIBRESTEAM_EXT_REPL_CMD_CAP];
    char alias_record[LIBRESTEAM_EXT_REPL_CMD_CAP];
    char history_path[CLIENT_HISTORY_PATH_CAP];
    char history_tmp[CLIENT_HISTORY_PATH_CAP];
    char history_key[CLIENT_HISTORY_KEY_CAP];
    char img[CLIENT_HISTORY_PATH_CAP];
    char search_draft[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint32_t search_cursor;
    uint32_t candidate_cursor;
    uint32_t ghost_sel;
    uint8_t search_failed;
    uint8_t ghost_hidden;
    uint8_t edit_pudding[2];
    uint8_t history_storage[CLIENT_HISTORY_BYTES + CLIENT_FILE_ALIGN];
    ldg_io_file_t *history_lock;
    const char *stage;
    char file_path[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint8_t file_storage[CLIENT_FILE_ALIGN * 2U];
} client_ctx_t;

static uint32_t client_file_wr(client_ctx_t *ctx, const char *text, uint32_t len)
{
    uint8_t *buff = (uint8_t *)(((uintptr_t)ctx->file_storage + CLIENT_FILE_ALIGN - 1U) & ~((uintptr_t)CLIENT_FILE_ALIGN - 1U));
    uint64_t done = 0x0;
    uint32_t part = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(UINT64_MAX - ctx->file_len < len)) { return LDG_ERR_OVERFLOW; }

    while (len)
    {
        part = CLIENT_FILE_ALIGN - ctx->file_used;
        if (part > len) { part = len; }

        res = ldg_mem_secure_cpy(buff + ctx->file_used, text, part);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        ctx->file_used += part;
        ctx->file_len += part;
        text += part;
        len -= part;
        if (ctx->file_used == CLIENT_FILE_ALIGN)
        {
            res = ldg_io_file_wr(ctx->file, buff, CLIENT_FILE_ALIGN, &done);
            if (res == LDG_ERR_AOK && done != CLIENT_FILE_ALIGN) { res = LDG_ERR_IO_WR; }

            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { ctx->file_res = res; return res; }

            ctx->file_used = 0;
        }
    }
    return LDG_ERR_AOK;
}

static uint32_t client_file_close(client_ctx_t *ctx)
{
    uint8_t *buff = (uint8_t *)(((uintptr_t)ctx->file_storage + CLIENT_FILE_ALIGN - 1U) & ~((uintptr_t)CLIENT_FILE_ALIGN - 1U));
    uint64_t done = 0x0;
    uint32_t res = ctx->file_res;
    uint32_t close_res = LDG_ERR_AOK;

    if (!ctx->file) { return LDG_ERR_AOK; }

    if (res == LDG_ERR_AOK && ctx->file_used && ctx->file_used < CLIENT_FILE_ALIGN)
    {
        res = ldg_mem_secure_zero(buff + ctx->file_used, CLIENT_FILE_ALIGN - ctx->file_used);
        if (res == LDG_ERR_AOK) { res = ldg_io_file_wr(ctx->file, buff, CLIENT_FILE_ALIGN, &done); }

        if (res == LDG_ERR_AOK && done != CLIENT_FILE_ALIGN) { res = LDG_ERR_IO_WR; }
    }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_truncate(ctx->file, ctx->file_len); }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_commit(ctx->file); }

    close_res = ldg_io_file_close(ctx->file);
    ctx->file = 0x0;
    ctx->file_used = 0;
    ctx->file_len = 0;
    ctx->file_res = LDG_ERR_AOK;
    return res == LDG_ERR_AOK ? close_res : res;
}

static uint32_t client_wr(client_ctx_t *ctx, const char *text, uint32_t len)
{
    uint32_t start = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->tee && ctx->mcp_res == LDG_ERR_AOK) { ctx->mcp_res = libresteam_ext_repl_mcp_wr(ctx->mcp, text, len); }

    if (ctx->file)
    {
        res = client_file_wr(ctx, text, len);
        if (res != LDG_ERR_AOK || !ctx->tee) { return res; }
    }

    if (!len) { return LDG_ERR_AOK; }

    if (!ctx->interactive) { return ldg_io_console_wr((const uint8_t *)text, len) == LDG_ERR_AOK ? LDG_ERR_AOK : LDG_ERR_IO_WR; }

    for (idx = 0; idx < len; idx++)
    {
        if (text[idx] != '\n' || (idx && text[idx - LDG_STR_TERM_SIZE] == '\r')) { continue; }

        if (idx > start && ldg_io_console_wr((const uint8_t *)text + start, idx - start) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }

        if (ldg_io_console_wr((const uint8_t *)CLIENT_LINE_END, sizeof(CLIENT_LINE_END) - LDG_STR_TERM_SIZE) != LDG_ERR_AOK) { return LDG_ERR_IO_WR; }

        start = idx + LDG_STR_TERM_SIZE;
    }
    if (start == len) { return LDG_ERR_AOK; }

    return ldg_io_console_wr((const uint8_t *)text + start, len - start) == LDG_ERR_AOK ? LDG_ERR_AOK : LDG_ERR_IO_WR;
}

static uint32_t client_text(client_ctx_t *ctx, const char *text)
{
    uint64_t len = 0x0;
    uint32_t res = ldg_str_len_get(text, LIBRESTEAM_EXT_REPL_RESP_CAP, &len);

    return res == LDG_ERR_AOK ? client_wr(ctx, text, (uint32_t)len) : res;
}

static uint32_t client_hex_parse(const char *text, uint32_t len, uint64_t *addr, uint8_t *bytes, uint32_t *cunt)
{
    uint32_t pos = 2U;
    uint32_t res = LDG_ERR_AOK;
    uint8_t hi = 0x0;
    uint8_t lo = 0x0;

    *cunt = 0;
    if (len < 4U || text[0] != '0' || text[1] != 'x') { return LDG_ERR_STR_INVALID; }

    while (pos < len && ldg_char_hex_is((uint8_t)text[pos]) == LDG_TRUTH_TRUE) { pos++; }
    if (pos == 2U || pos - 2U > CLIENT_HEX_ADDR_DIGITS || pos >= len || text[pos] != ':') { return LDG_ERR_STR_INVALID; }

    res = ldg_str_uint_parse(text + 2U, LDG_BASE_HEX, pos - 2U, addr);
    if (res != LDG_ERR_AOK) { return res; }

    pos++;
    while (pos < len)
    {
        if (text[pos++] != ' ') { return LDG_ERR_STR_INVALID; }

        if (pos == len) { break; }

        if (len - pos < 2U || *cunt == CLIENT_HEX_ROW_CAP) { return LDG_ERR_STR_INVALID; }

        res = ldg_hex_to_nipple(text[pos], &hi);
        if (res == LDG_ERR_AOK) { res = ldg_hex_to_nipple(text[pos + 1U], &lo); }

        if (res != LDG_ERR_AOK) { return res; }

        bytes[(*cunt)++] = (uint8_t)((hi << LDG_NIBBLE_BITS) | lo);
        pos += 2U;
    }
    return *cunt && *addr <= UINT64_MAX - (*cunt - 1U) ? LDG_ERR_AOK : LDG_ERR_STR_INVALID;
}

static uint32_t client_hex_wr(client_ctx_t *ctx, uint64_t addr, const uint8_t *bytes, uint32_t cunt, uint32_t width)
{
    uint64_t prefix = 0x0;
    uint32_t offset = 0x0;
    uint32_t idx = 0x0;
    uint32_t pos = 0x0;
    uint32_t cols = 0x0;
    uint32_t row = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (width > sizeof(ctx->line) - LDG_STR_TERM_SIZE) { width = sizeof(ctx->line) - LDG_STR_TERM_SIZE; }

    while (offset < cunt)
    {
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &prefix, "0x%x64: ", addr + offset);
        if (res != LDG_ERR_AOK) { return res; }

        cols = width / 2U;
        row = cols > prefix + CLIENT_HEX_SEPARATOR_COLS ? (cols - (uint32_t)prefix - CLIENT_HEX_SEPARATOR_COLS) / CLIENT_HEX_BYTE_COLS : 1U;
        if (!row) { row = 1U; }

        if (row > CLIENT_HEX_ROW_CAP) { row = CLIENT_HEX_ROW_CAP; }

        if (row > cunt - offset) { row = cunt - offset; }

        pos = (uint32_t)prefix;
        for (idx = 0; idx < row; idx++)
        {
            res = ldg_byte_to_hex(bytes[offset + idx], ctx->line + pos);
            if (res != LDG_ERR_AOK) { return res; }

            pos += 2U;
            ctx->line[pos++] = ' ';
        }
        while (pos < cols) { ctx->line[pos++] = ' '; }
        ctx->line[pos++] = '|';
        ctx->line[pos++] = ' ';
        for (idx = 0; idx < row; idx++) { ctx->line[pos++] = bytes[offset + idx] >= LDG_ASCII_PRINTABLE_MIN && bytes[offset + idx] <= LDG_ASCII_PRINTABLE_MAX ? (char)bytes[offset + idx] : '.'; }
        ctx->line[pos++] = '\n';
        res = client_wr(ctx, ctx->line, pos);
        if (res != LDG_ERR_AOK) { return res; }

        offset += row;
    }
    return LDG_ERR_AOK;
}

static uint32_t client_resp_wr(client_ctx_t *ctx)
{
    uint8_t bytes[CLIENT_HEX_ROW_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t addr = 0x0;
    uint32_t pos = 0x0;
    uint32_t start = 0x0;
    uint32_t len = 0x0;
    uint32_t cunt = 0x0;
    uint32_t width = 0x0;
    uint32_t res = ldg_sys_tty_width_get(&width);

    if (res != LDG_ERR_AOK || !width) { width = CLIENT_TERMINAL_FALLBACK_COLS; }

    while (pos < ctx->resp.len)
    {
        start = pos;
        while (pos < ctx->resp.len && ctx->resp.text[pos] != '\n') { pos++; }
        len = pos - start;
        if (len && ctx->resp.text[start + len - 1U] == '\r') { len--; }

        if (pos < ctx->resp.len) { pos++; }

        res = ctx->output_line_open ? LDG_ERR_STR_INVALID : client_hex_parse(ctx->resp.text + start, len, &addr, bytes, &cunt);
        if (res == LDG_ERR_AOK) { res = client_hex_wr(ctx, addr, bytes, cunt, width); }
        else { res = client_wr(ctx, ctx->resp.text + start, pos - start); }

        if (res != LDG_ERR_AOK) { return res; }

        ctx->output_line_open = ctx->resp.text[pos - 1U] != '\n' ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    }
    return LDG_ERR_AOK;
}

static uint8_t client_eq_is(const char *lhs, const char *rhs)
{
    uint32_t cmp = 0x0;

    if (ldg_str_cmp(lhs, rhs, LIBRESTEAM_EXT_REPL_CMD_CAP, &cmp) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return cmp == LDG_ARITH_ORD_EQ ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t client_history_name_len(const char *text)
{
    uint32_t idx = 0x0;

    if (text[0] != '_' && ldg_char_alpha_is((uint8_t)text[0]) != LDG_TRUTH_TRUE) { return 0; }

    while (idx < CLIENT_ALIAS_NAME_CAP && (text[idx] == '_' || ldg_char_alnum_is((uint8_t)text[idx]) == LDG_TRUTH_TRUE)) { idx++; }
    return idx && idx < CLIENT_ALIAS_NAME_CAP && text[idx] == '=' && text[idx + 1U] != '=' ? idx : 0;
}

static uint8_t client_history_pinned_is(client_ctx_t *ctx, uint32_t idx)
{
    uint32_t size = 0x0;
    uint32_t later = 0x0;
    uint32_t pos = 0x0;
    const char *text = ctx->history[idx].text;

    if (ctx->history[idx].kind != CLIENT_HISTORY_ALIAS) { return LDG_TRUTH_FALSE; }

    size = client_history_name_len(text);
    if (!size || !text[size + 1U]) { return LDG_TRUTH_FALSE; }

    for (later = idx + 1U; later < ctx->history_cunt; later++)
    {
        if (ctx->history[later].kind != CLIENT_HISTORY_ALIAS || client_history_name_len(ctx->history[later].text) != size) { continue; }

        for (pos = 0; pos < size && text[pos] == ctx->history[later].text[pos]; pos++) {}
        if (pos == size) { return LDG_TRUTH_FALSE; }
    }
    return LDG_TRUTH_TRUE;
}

static uint32_t client_history_add(client_ctx_t *ctx, const char *text, uint8_t kind)
{
    uint64_t len = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = ldg_str_len_get(text, LIBRESTEAM_EXT_REPL_CMD_CAP, &len);

    if (res != LDG_ERR_AOK) { return res; }

    if (!len) { return LDG_ERR_AOK; }

    for (idx = 0; idx < len; idx++) { if (((uint8_t)text[idx] < LDG_ASCII_PRINTABLE_MIN && text[idx] != '\t') || (uint8_t)text[idx] > LDG_ASCII_PRINTABLE_MAX) { return LDG_ERR_STR_INVALID; } }
    if (ctx->history_cunt == CLIENT_HISTORY_CAP)
    {
        for (idx = 0; idx < ctx->history_cunt && client_history_pinned_is(ctx, idx) == LDG_TRUTH_TRUE; idx++) {}
        if (idx == ctx->history_cunt) { return LDG_ERR_FULL; }

        for (; idx + 1U < ctx->history_cunt; idx++) { ctx->history[idx] = ctx->history[idx + 1U]; }
        ctx->history_cunt--;
    }

    res = ldg_str_rbrcpy(ctx->history[ctx->history_cunt].text, text, sizeof(ctx->history[0].text));
    if (res != LDG_ERR_AOK) { return res; }

    ctx->history[ctx->history_cunt++].kind = kind;
    return LDG_ERR_AOK;
}

static uint32_t client_history_encode(client_ctx_t *ctx, uint8_t *buff, uint64_t *len)
{
    uint64_t size = 0x0;
    uint64_t pos = sizeof(CLIENT_HISTORY_HEADER) - LDG_STR_TERM_SIZE;
    uint32_t idx = 0x0;
    uint32_t res = ldg_mem_secure_zero(buff, CLIENT_HISTORY_BYTES);

    *len = 0;
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(buff, CLIENT_HISTORY_HEADER, pos); }

    for (idx = 0; res == LDG_ERR_AOK && idx < ctx->history_cunt; idx++)
    {
        res = ldg_str_len_get(ctx->history[idx].text, sizeof(ctx->history[idx].text), &size);
        if (res != LDG_ERR_AOK) { return res; }

        if (pos + size + 3U > CLIENT_HISTORY_BYTES) { return LDG_ERR_FULL; }

        buff[pos++] = ctx->history[idx].kind;
        buff[pos++] = '\t';
        res = ldg_mem_secure_cpy(buff + pos, ctx->history[idx].text, size);
        pos += size;
        buff[pos++] = '\n';
    }
    if (res == LDG_ERR_AOK) { *len = pos; }

    return res;
}

static uint32_t client_history_save(client_ctx_t *ctx)
{
    uint8_t *buff = (uint8_t *)(((uintptr_t)ctx->history_storage + CLIENT_FILE_ALIGN - 1U) & ~((uintptr_t)CLIENT_FILE_ALIGN - 1U));
    ldg_io_file_t *file = 0x0;
    uint64_t len = 0x0;
    uint64_t padded = 0x0;
    uint64_t done = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (!ctx->history_up || ctx->history_loading) { return LDG_ERR_AOK; }

    res = client_history_encode(ctx, buff, &len);
    if (res != LDG_ERR_AOK) { return res; }

    padded = ((len + CLIENT_FILE_ALIGN - 1U) & ~((uint64_t)CLIENT_FILE_ALIGN - 1U));
    res = ldg_io_file_open(ctx->history_tmp, LDG_IO_WR_ONLY | LDG_IO_CREATE | LDG_IO_TRUNCATE | LDG_IO_DIRECT, CLIENT_FILE_ACCESS, &file);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_file_wr(file, buff, padded, &done);
    if (res == LDG_ERR_AOK && done != padded) { res = LDG_ERR_IO_WR; }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_truncate(file, len); }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_commit(file); }

    cleanup = ldg_io_file_close(file);
    if (res == LDG_ERR_AOK) { res = cleanup; }

    if (res == LDG_ERR_AOK && ldg_io_path_rename(ctx->history_tmp, ctx->history_path) != LDG_ERR_AOK) { res = LDG_ERR_IO_WR; }

    return res;
}

static uint32_t client_history_key_get(client_ctx_t *ctx, const char *img)
{
    const char hex[] = "0123456789abcdef";
    uint32_t start = 0x0;
    uint32_t idx = 0x0;
    uint32_t used = 0x0;
    uint32_t ch = 0x0;
    uint32_t shift = 0x0;

    for (idx = 0; img[idx]; idx++) { if (img[idx] == '\\' || img[idx] == '/') { start = idx + 1U; } }
    if (!img[start]) { return LDG_ERR_STR_INVALID; }

    for (idx = start; img[idx]; idx++)
    {
        ch = (uint8_t)img[idx];
        if (ch >= 'A' && ch <= 'Z') { ch += 'a' - 'A'; }

        if (used + 6U >= sizeof(ctx->history_key)) { return LDG_ERR_STR_TRUNC; }

        if ((ch >= 'a' && ch <= 'z') || (ch >= '0' && ch <= '9') || ch == '.' || ch == '-') { ctx->history_key[used++] = (char)ch; }
        else
        {
            ctx->history_key[used++] = '_';
            for (shift = sizeof(uint16_t) * LDG_BYTE_BITS; shift; shift -= LDG_NIBBLE_BITS) { ctx->history_key[used++] = hex[(ch >> (shift - LDG_NIBBLE_BITS)) & LDG_NIBBLE_MASK]; }
        }
    }
    ctx->history_key[used] = LDG_STR_TERM;
    return LDG_ERR_AOK;
}

static uint32_t client_history_paths_get(client_ctx_t *ctx)
{
    ldg_sys_proc_t proc = LDG_STRUCT_ZERO_INIT;
    uint64_t len = 0x0;
    uint32_t dir = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (ctx->hello.pid > UINT32_MAX) { return LDG_ERR_RANGE; }

    if (ldg_sys_proc_open((uint32_t)ctx->hello.pid, &proc) != LDG_ERR_AOK) { return LDG_ERR_DENIED; }

    res = ldg_sys_proc_img_path_get(&proc, ctx->img, sizeof(ctx->img));
    cleanup = ldg_sys_proc_close(&proc);
    if (res == LDG_ERR_AOK) { res = cleanup; }

    if (res == LDG_ERR_AOK) { res = client_history_key_get(ctx, ctx->img); }

    if (res == LDG_ERR_AOK) { res = ldg_io_path_prog_get(ctx->history_path, sizeof(ctx->history_path)); }

    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; ctx->history_path[idx]; idx++) { if (ctx->history_path[idx] == '\\' || ctx->history_path[idx] == '/') { dir = idx + LDG_STR_TERM_SIZE; } }
    if (!dir) { return LDG_ERR_STR_INVALID; }

    res = ldg_str_fmt(ctx->history_path + dir, sizeof(ctx->history_path) - dir, &len, "repl-history-%s.txt", ctx->history_key);
    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->history_tmp, sizeof(ctx->history_tmp), &len, "%s.tmp", ctx->history_path); }

    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &len, "%s" CLIENT_LOCK_SUFFIX, ctx->history_path); }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_open(ctx->line, LDG_IO_WR_ONLY | LDG_IO_CREATE | LDG_IO_DIRECT, CLIENT_FILE_ACCESS, &ctx->history_lock); }

    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_file_lock(ctx->history_lock) != LDG_ERR_AOK)
    {
        cleanup = ldg_io_file_close(ctx->history_lock);
        ctx->history_lock = 0x0;
        return cleanup == LDG_ERR_AOK ? LDG_ERR_BUSY : cleanup;
    }

    return LDG_ERR_AOK;
}

static uint32_t client_literal_end(const char *text, uint32_t len, uint32_t start, uint32_t *end)
{
    uint32_t idx = start + 1U;
    uint32_t probe = 0x0;
    uint32_t equal = 0x0;
    uint8_t quote = (uint8_t)text[start];

    *end = start;
    if (quote == '\'' || quote == '"')
    {
        while (idx < len)
        {
            if (text[idx] == '\\' && idx + 1U < len) { idx += 2U; continue; }

            if ((uint8_t)text[idx++] == quote) { *end = idx; return LDG_ERR_AOK; }
        }
        return LDG_ERR_STR_INVALID;
    }

    if (quote != '[') { return LDG_ERR_AOK; }

    while (idx < len && text[idx] == '=') { idx++; }
    if (idx == len || text[idx] != '[') { return LDG_ERR_AOK; }

    equal = idx - start - 1U;
    for (idx++; idx < len; idx++)
    {
        if (text[idx] != ']') { continue; }

        probe = idx + 1U;
        while (probe < len && text[probe] == '=') { probe++; }
        if (probe < len && probe - idx - 1U == equal && text[probe] == ']') { *end = probe + 1U; return LDG_ERR_AOK; }
    }
    return LDG_ERR_STR_INVALID;
}

static uint32_t client_var_name_validate(const char *name, uint32_t len)
{
    uint32_t idx = 0x0;

    if (LDG_UNLIKELY(!len || len >= CLIENT_VAR_NAME_CAP)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(name[0] != '_' && ldg_char_alpha_is((uint8_t)name[0]) != LDG_TRUTH_TRUE)) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 1; idx < len; idx++) { if (LDG_UNLIKELY(name[idx] != '_' && ldg_char_alnum_is((uint8_t)name[idx]) != LDG_TRUTH_TRUE)) { return LDG_ERR_FUNC_ARG_INVALID; } }
    return LDG_ERR_AOK;
}

static uint32_t client_var_find(client_ctx_t *ctx, const char *name, uint32_t len, uint32_t *out)
{
    uint32_t slot = UINT32_MAX;
    uint32_t idx = 0x0;

    *out = UINT32_MAX;
    for (slot = 0; slot < CLIENT_VAR_CAP; slot++)
    {
        for (idx = 0; idx < len && ctx->vars[slot].name[idx] == name[idx]; idx++) {}
        if (idx == len && ctx->vars[slot].name[idx] == LDG_STR_TERM) { *out = slot; return LDG_ERR_AOK; }
    }
    return LDG_ERR_NOT_FOUND;
}

static uint32_t client_var_val_validate(const char *text, uint32_t len)
{
    uint32_t idx = 0x0;
    uint32_t digits = 0x0;
    uint32_t end = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (client_eq_is(text, "true") == LDG_TRUTH_TRUE || client_eq_is(text, "false") == LDG_TRUTH_TRUE || client_eq_is(text, "nil") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    res = client_literal_end(text, len, 0, &end);
    if (res != LDG_ERR_AOK || end) { return res == LDG_ERR_AOK && end == len ? LDG_ERR_AOK : LDG_ERR_STR_INVALID; }

    if (text[idx] == '+' || text[idx] == '-') { idx++; }

    if (idx + 2U <= len && text[idx] == '0' && (text[idx + 1U] == 'x' || text[idx + 1U] == 'X'))
    {
        idx += 2U;
        digits = idx;
        while (idx < len && ldg_char_hex_is((uint8_t)text[idx]) == LDG_TRUTH_TRUE) { idx++; }
        return idx == len && idx > digits ? LDG_ERR_AOK : LDG_ERR_STR_INVALID;
    }

    while (idx < len && ldg_char_digit_is((uint8_t)text[idx]) == LDG_TRUTH_TRUE) { idx++; digits++; }
    if (idx < len && text[idx] == '.')
    {
        idx++;
        while (idx < len && ldg_char_digit_is((uint8_t)text[idx]) == LDG_TRUTH_TRUE) { idx++; digits++; }
    }

    if (!digits) { return LDG_ERR_STR_INVALID; }

    if (idx < len && (text[idx] == 'e' || text[idx] == 'E'))
    {
        idx++;
        if (idx < len && (text[idx] == '+' || text[idx] == '-')) { idx++; }

        digits = idx;
        while (idx < len && ldg_char_digit_is((uint8_t)text[idx]) == LDG_TRUTH_TRUE) { idx++; }
        if (idx == digits) { return LDG_ERR_STR_INVALID; }
    }

    return idx == len ? LDG_ERR_AOK : LDG_ERR_STR_INVALID;
}

static uint32_t client_var_expand(client_ctx_t *ctx, uint32_t *len)
{
    const char *part = 0x0;
    uint32_t src = 0x0;
    uint32_t dst = 0x0;
    uint32_t end = 0x0;
    uint32_t start = 0x0;
    uint32_t slot = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    uint64_t part_len = 0x0;
    uint8_t braced = LDG_TRUTH_FALSE;

    while (src < *len)
    {
        end = src;
        res = client_literal_end(ctx->cmd, *len, src, &end);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (end == src && ctx->cmd[src] == '-' && src + 1U < *len && ctx->cmd[src + 1U] == '-')
        {
            if (src + 2U < *len && ctx->cmd[src + 2U] == '[') { res = client_literal_end(ctx->cmd, *len, src + 2U, &end); }

            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            if (end <= src + 2U || ctx->cmd[src + 2U] != '[') { end = *len; }
        }

        part = ctx->cmd + src;
        part_len = end > src ? end - src : 1U;
        if (end == src && ctx->cmd[src] == '$')
        {
            start = src + 1U;
            if (start < *len && ctx->cmd[start] == '$') { end = start + 1U; }
            else
            {
                braced = start < *len && ctx->cmd[start] == '{';
                if (braced) { start++; }

                end = start;
                while (end < *len && (ctx->cmd[end] == '_' || ldg_char_alnum_is((uint8_t)ctx->cmd[end]) == LDG_TRUTH_TRUE)) { end++; }
                res = client_var_name_validate(ctx->cmd + start, end - start);
                if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

                if (LDG_UNLIKELY(braced && (end == *len || ctx->cmd[end] != '}'))) { return LDG_ERR_STR_INVALID; }

                res = client_var_find(ctx, ctx->cmd + start, end - start, &slot);
                if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

                if (braced) { end++; }

                part = ctx->vars[slot].val;
                res = ldg_str_len_get(part, sizeof(ctx->vars[slot].val), &part_len);
                if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
            }
        }

        if (LDG_UNLIKELY(part_len >= sizeof(ctx->cmd) - dst)) { return LDG_ERR_STR_TRUNC; }

        res = ldg_mem_secure_cpy(ctx->line + dst, part, part_len);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        dst += (uint32_t)part_len;
        src = end > src ? end : src + 1U;
    }
    ctx->line[dst] = LDG_STR_TERM;
    res = ldg_str_rbrcpy(ctx->cmd, ctx->line, sizeof(ctx->cmd));
    if (res == LDG_ERR_AOK) { *len = dst; }

    return res;
}

static uint32_t client_var_list(client_ctx_t *ctx)
{
    uint32_t idx = UINT32_MAX;
    uint32_t cunt = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint64_t len = 0x0;

    for (idx = 0; idx < CLIENT_VAR_CAP; idx++)
    {
        if (!ctx->vars[idx].name[0]) { continue; }

        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &len, "%s=%s\n", ctx->vars[idx].name, ctx->vars[idx].val);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)len); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        cunt++;
    }
    return cunt ? LDG_ERR_AOK : client_text(ctx, "No variables defined. Use set name=value.\n");
}

static uint32_t client_var_cmd(client_ctx_t *ctx, uint32_t *len, uint8_t *handled)
{
    uint32_t start = 0x0;
    uint32_t end = 0x0;
    uint32_t equal = 0x0;
    uint32_t val = 0x0;
    uint32_t slot = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    uint64_t text_len = 0x0;
    uint8_t remove = LDG_TRUTH_FALSE;

    *handled = LDG_TRUTH_FALSE;
    res = client_var_expand(ctx, len);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (client_eq_is(ctx->cmd, "vars") == LDG_TRUTH_TRUE) { *handled = LDG_TRUTH_TRUE; return client_var_list(ctx); }

    if (*len > 3U && ctx->cmd[0] == 's' && ctx->cmd[1] == 'e' && ctx->cmd[2] == 't' && ldg_char_space_is((uint8_t)ctx->cmd[3]) == LDG_TRUTH_TRUE) { start = 4U; }
    else if (*len >= 5U && ctx->cmd[0] == 'u' && ctx->cmd[1] == 'n' && ctx->cmd[2] == 's' && ctx->cmd[3] == 'e' && ctx->cmd[4] == 't' && (*len == 5U || ldg_char_space_is((uint8_t)ctx->cmd[5]) == LDG_TRUTH_TRUE)) { start = *len == 5U ? 5U : 6U; remove = LDG_TRUTH_TRUE; }
    else { return LDG_ERR_AOK; }

    while (start < *len && ldg_char_space_is((uint8_t)ctx->cmd[start]) == LDG_TRUTH_TRUE) { start++; }
    end = start;
    while (end < *len && ctx->cmd[end] != '=' && ldg_char_space_is((uint8_t)ctx->cmd[end]) != LDG_TRUTH_TRUE) { end++; }
    equal = end;
    while (equal < *len && ldg_char_space_is((uint8_t)ctx->cmd[equal]) == LDG_TRUTH_TRUE) { equal++; }
    if (!remove && (equal == *len || ctx->cmd[equal] != '=')) { return LDG_ERR_AOK; }

    *handled = LDG_TRUTH_TRUE;
    res = client_var_name_validate(ctx->cmd + start, end - start);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = client_var_find(ctx, ctx->cmd + start, end - start, &slot);
    if (remove)
    {
        if (LDG_UNLIKELY(equal != *len)) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        ctx->vars[slot].name[0] = LDG_STR_TERM;
        ctx->vars[slot].val[0] = LDG_STR_TERM;
        return client_text(ctx, "Variable removed.\n");
    }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK && res != LDG_ERR_NOT_FOUND)) { return res; }

    val = equal + 1U;
    while (val < *len && ldg_char_space_is((uint8_t)ctx->cmd[val]) == LDG_TRUTH_TRUE) { val++; }
    res = client_var_val_validate(ctx->cmd + val, *len - val);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (slot == UINT32_MAX) { for (slot = 0; slot < CLIENT_VAR_CAP && ctx->vars[slot].name[0]; slot++) {} }

    if (LDG_UNLIKELY(slot == CLIENT_VAR_CAP)) { return LDG_ERR_FULL; }

    res = ldg_str_rbrcpy(ctx->vars[slot].val, ctx->cmd + val, sizeof(ctx->vars[slot].val));
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(ctx->vars[slot].name, ctx->cmd + start, end - start); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    ctx->vars[slot].name[end - start] = LDG_STR_TERM;
    res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "variable saved: %s=%s\n", ctx->vars[slot].name, ctx->vars[slot].val);
    return res == LDG_ERR_AOK ? client_wr(ctx, ctx->line, (uint32_t)text_len) : res;
}

static uint32_t client_alias_list(client_ctx_t *ctx)
{
    uint64_t len = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t cunt = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (idx = 0; idx < CLIENT_ALIAS_CAP; idx++)
    {
        if (ctx->aliases[idx].name[0] == LDG_STR_TERM) { continue; }

        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &len, "%s=%s\n", ctx->aliases[idx].name, ctx->aliases[idx].text);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)len); }

        if (res != LDG_ERR_AOK) { return res; }

        cunt++;
    }
    return cunt ? LDG_ERR_AOK : client_text(ctx, "No aliases defined. Use name=command to create one.\n");
}

static uint32_t client_alias_set(client_ctx_t *ctx, uint32_t equal, uint32_t len)
{
    uint32_t name_len = equal;
    uint32_t value_start = equal + LDG_STR_TERM_SIZE;
    uint32_t idx = UINT32_MAX;
    uint32_t slot = UINT32_MAX;
    uint32_t vacant = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    uint64_t text_len = 0x0;

    while (name_len && ldg_char_space_is((uint8_t)ctx->cmd[name_len - 1U]) == LDG_TRUTH_TRUE) { name_len--; }
    while (value_start < len && ldg_char_space_is((uint8_t)ctx->cmd[value_start]) == LDG_TRUTH_TRUE) { value_start++; }
    if (!name_len || name_len >= CLIENT_ALIAS_NAME_CAP) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (ctx->cmd[0] != '_' && ldg_char_alpha_is((uint8_t)ctx->cmd[0]) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; idx < name_len; idx++) { if (ctx->cmd[idx] != '_' && ldg_char_alnum_is((uint8_t)ctx->cmd[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; } }
    ctx->cmd[name_len] = LDG_STR_TERM;
    if (client_eq_is(ctx->cmd, "quit") == LDG_TRUTH_TRUE || client_eq_is(ctx->cmd, "help") == LDG_TRUTH_TRUE || client_eq_is(ctx->cmd, "aliases") == LDG_TRUTH_TRUE || client_eq_is(ctx->cmd, "vars") == LDG_TRUTH_TRUE || client_eq_is(ctx->cmd, "unset") == LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

    for (idx = 0; idx < CLIENT_ALIAS_CAP; idx++)
    {
        if (ctx->aliases[idx].name[0] == LDG_STR_TERM)
        {
            if (vacant == UINT32_MAX) { vacant = idx; }

            continue;
        }

        if (client_eq_is(ctx->aliases[idx].name, ctx->cmd) == LDG_TRUTH_TRUE) { slot = idx; break; }
    }
    if (value_start == len)
    {
        if (slot == UINT32_MAX) { return LDG_ERR_NOT_FOUND; }

        ctx->aliases[slot].name[0] = LDG_STR_TERM;
        ctx->aliases[slot].text[0] = LDG_STR_TERM;
        res = ldg_str_fmt(ctx->alias_record, sizeof(ctx->alias_record), &text_len, "%s=", ctx->cmd);
        if (res != LDG_ERR_AOK) { return res; }

        ctx->history_alias = LDG_TRUTH_TRUE;
        return ctx->history_loading ? LDG_ERR_AOK : client_text(ctx, "Alias removed.\n");
    }

    if (slot == UINT32_MAX) { slot = vacant; }

    if (slot == UINT32_MAX) { return LDG_ERR_FULL; }

    res = ldg_str_rbrcpy(ctx->aliases[slot].text, ctx->cmd + value_start, sizeof(ctx->aliases[slot].text));
    if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(ctx->aliases[slot].name, ctx->cmd, sizeof(ctx->aliases[slot].name)); }

    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->alias_record, sizeof(ctx->alias_record), &text_len, "%s=%s", ctx->aliases[slot].name, ctx->aliases[slot].text); }

    if (res != LDG_ERR_AOK) { return res; }

    ctx->history_alias = LDG_TRUTH_TRUE;
    if (ctx->history_loading) { return LDG_ERR_AOK; }

    res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "alias saved: %s=%s\n", ctx->aliases[slot].name, ctx->aliases[slot].text);

    return res == LDG_ERR_AOK ? client_wr(ctx, ctx->line, (uint32_t)text_len) : res;
}

static uint32_t client_history_decode(client_ctx_t *ctx, const uint8_t *buff, uint64_t len)
{
    uint64_t pos = 0x0;
    uint32_t size = 0x0;
    uint32_t equal = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t kind = 0x0;

    if (len < sizeof(CLIENT_HISTORY_HEADER) - LDG_STR_TERM_SIZE) { return LDG_ERR_INVALID; }

    for (pos = 0; pos < sizeof(CLIENT_HISTORY_HEADER) - LDG_STR_TERM_SIZE; pos++) { if (buff[pos] != (uint8_t)CLIENT_HISTORY_HEADER[pos]) { return LDG_ERR_INVALID; } }
    ctx->history_cunt = 0;
    res = ldg_mem_secure_zero(ctx->aliases, sizeof(ctx->aliases));
    if (res != LDG_ERR_AOK) { return res; }

    ctx->history_loading = LDG_TRUTH_TRUE;
    while (pos < len && res == LDG_ERR_AOK)
    {
        if (ctx->history_cunt == CLIENT_HISTORY_CAP || len - pos < 4U) { res = LDG_ERR_INVALID; break; }

        kind = buff[pos++];
        if ((kind != CLIENT_HISTORY_COMMAND && kind != CLIENT_HISTORY_ALIAS) || buff[pos++] != '\t') { res = LDG_ERR_INVALID; break; }

        size = 0;
        while (pos < len && buff[pos] != '\n')
        {
            if (size + LDG_STR_TERM_SIZE >= sizeof(ctx->cmd) || (buff[pos] < LDG_ASCII_PRINTABLE_MIN && buff[pos] != '\t') || buff[pos] > LDG_ASCII_PRINTABLE_MAX) { res = LDG_ERR_INVALID; break; }

            ctx->cmd[size++] = (char)buff[pos++];
        }
        if (res != LDG_ERR_AOK) { break; }

        if (pos == len || !size) { res = LDG_ERR_INVALID; break; }

        pos++;
        ctx->cmd[size] = LDG_STR_TERM;
        res = client_history_add(ctx, ctx->cmd, kind);
        if (res != LDG_ERR_AOK || kind != CLIENT_HISTORY_ALIAS) { continue; }

        equal = client_history_name_len(ctx->cmd);
        if (!equal) { res = LDG_ERR_INVALID; break; }

        res = client_alias_set(ctx, equal, size);
        if (res == LDG_ERR_NOT_FOUND && size == equal + 1U) { res = LDG_ERR_AOK; }
    }
    ctx->cmd[0] = LDG_STR_TERM;
    ctx->history_loading = LDG_TRUTH_FALSE;
    ctx->history_alias = LDG_TRUTH_FALSE;
    return res;
}

static uint32_t client_history_load(client_ctx_t *ctx)
{
    uint8_t *buff = (uint8_t *)(((uintptr_t)ctx->history_storage + CLIENT_FILE_ALIGN - 1U) & ~((uintptr_t)CLIENT_FILE_ALIGN - 1U));
    ldg_io_file_t *file = 0x0;
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    uint64_t padded = 0x0;
    uint64_t done = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (ldg_io_path_exists_is(ctx->history_path) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    res = ldg_io_file_open(ctx->history_path, LDG_IO_RD_ONLY | LDG_IO_DIRECT, 0, &file);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_file_info_get(file, &info);
    if (res == LDG_ERR_AOK && (!info.size || info.size > CLIENT_HISTORY_BYTES - CLIENT_FILE_ALIGN)) { res = LDG_ERR_INVALID; }

    if (res == LDG_ERR_AOK)
    {
        padded = (info.size + CLIENT_FILE_ALIGN - 1U) & ~((uint64_t)CLIENT_FILE_ALIGN - 1U);
        res = ldg_io_file_rd(file, buff, padded, &done);
        if (res == LDG_ERR_AOK && done != info.size) { res = LDG_ERR_IO_RD; }
    }

    cleanup = ldg_io_file_close(file);
    if (res == LDG_ERR_AOK) { res = cleanup; }

    return res == LDG_ERR_AOK ? client_history_decode(ctx, buff, done) : res;
}

static uint32_t client_history_alias_save(client_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->history_alias || !ctx->history_cunt) { return LDG_ERR_AOK; }

    res = ldg_str_rbrcpy(ctx->history[ctx->history_cunt - 1U].text, ctx->alias_record, sizeof(ctx->history[0].text));
    if (res != LDG_ERR_AOK) { return res; }

    ctx->history[ctx->history_cunt - 1U].kind = CLIENT_HISTORY_ALIAS;
    return client_history_save(ctx);
}

static uint32_t client_alias_run(client_ctx_t *ctx, uint32_t *len, uint8_t *handled)
{
    uint64_t equal = UINT64_MAX;
    uint64_t expanded_len = 0x0;
    uint32_t start = 0x0;
    uint32_t end = *len;
    uint32_t idx = UINT32_MAX;
    uint32_t depth = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *handled = LDG_TRUTH_FALSE;
    while (start < end && ldg_char_space_is((uint8_t)ctx->cmd[start]) == LDG_TRUTH_TRUE) { start++; }
    while (end > start && ldg_char_space_is((uint8_t)ctx->cmd[end - 1U]) == LDG_TRUTH_TRUE) { end--; }
    *len = end - start;
    if (!*len) { *handled = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

    res = ldg_mem_secure_cpy(ctx->line, ctx->cmd + start, *len);
    if (res != LDG_ERR_AOK) { return res; }

    ctx->line[*len] = LDG_STR_TERM;
    res = ldg_str_rbrcpy(ctx->cmd, ctx->line, sizeof(ctx->cmd));
    if (res != LDG_ERR_AOK) { return res; }

    equal = 0;
    if (ctx->cmd[0] == '_' || ldg_char_alpha_is((uint8_t)ctx->cmd[0]) == LDG_TRUTH_TRUE)
    {
        while (equal < *len && (ctx->cmd[equal] == '_' || ldg_char_alnum_is((uint8_t)ctx->cmd[equal]) == LDG_TRUTH_TRUE)) { equal++; }
        while (equal < *len && ldg_char_space_is((uint8_t)ctx->cmd[equal]) == LDG_TRUTH_TRUE) { equal++; }
        if (equal < *len && ctx->cmd[equal] == '=' && ctx->cmd[equal + 1U] != '=')
        {
            *handled = LDG_TRUTH_TRUE;
            return client_alias_set(ctx, (uint32_t)equal, *len);
        }
    }

    for (depth = 0; depth <= CLIENT_ALIAS_CAP; depth++)
    {
        if (client_eq_is(ctx->cmd, "aliases") == LDG_TRUTH_TRUE)
        {
            *handled = LDG_TRUTH_TRUE;
            return client_alias_list(ctx);
        }

        if (client_eq_is(ctx->cmd, "help") == LDG_TRUTH_TRUE) { return client_text(ctx, "Client aliases: name=command saves; name runs; name= removes; aliases lists. Variables: set name=value; $name or ${name} substitutes; vars lists; unset name removes. Values: decimal/hex numbers, booleans, nil, quoted strings. Substitution skips quoted text and Lua comments; $$ is a literal dollar. Up to 32 aliases and 32 variables, names up to 31 characters. Aliases persist in the target executable's history; variables last until this client closes.\n"); }

        for (idx = 0; idx < CLIENT_ALIAS_CAP; idx++) { if (ctx->aliases[idx].name[0] != LDG_STR_TERM && client_eq_is(ctx->aliases[idx].name, ctx->cmd) == LDG_TRUTH_TRUE) { break; } }
        if (idx == CLIENT_ALIAS_CAP) { return LDG_ERR_AOK; }

        if (depth == CLIENT_ALIAS_CAP) { return LDG_ERR_INVALID; }

        res = ldg_str_rbrcpy(ctx->cmd, ctx->aliases[idx].text, sizeof(ctx->cmd));
        if (res == LDG_ERR_AOK) { res = ldg_str_len_get(ctx->cmd, sizeof(ctx->cmd), &expanded_len); }

        if (res != LDG_ERR_AOK) { return res; }

        *len = (uint32_t)expanded_len;
    }
    return LDG_ERR_INVALID;
}

static uint32_t client_path_get(client_ctx_t *ctx, const char *pid)
{
    uint64_t len = 0x0;
    uint64_t cunt = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (pid)
    {
        for (idx = 0; pid[idx] != LDG_STR_TERM; idx++) { if (idx >= CLIENT_PID_DIGITS_MAX || ldg_char_digit_is((uint8_t)pid[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; } }
        if (!idx) { return LDG_ERR_FUNC_ARG_INVALID; }

        return ldg_str_fmt(ctx->path, sizeof(ctx->path), &len, LIBRESTEAM_EXT_REPL_PIPE_PREFIX "%s", pid);
    }

    res = ldg_io_ipc_list(LIBRESTEAM_EXT_REPL_PIPE_PREFIX, ctx->names, CLIENT_PIPE_CAP, &cunt);
    if (res != LDG_ERR_AOK) { return res; }

    if (cunt == 0) { return LDG_ERR_NOT_FOUND; }

    if (cunt > CLIENT_PIPE_CAP) { cunt = CLIENT_PIPE_CAP; }

    for (idx = 0; idx < cunt; idx++)
    {
        res = client_text(ctx, ctx->names[idx].text);
        if (res == LDG_ERR_AOK) { res = client_text(ctx, "\n"); }

        if (res != LDG_ERR_AOK) { return res; }
    }

    if (cunt != CLIENT_TGT_CUNT)
    {
        res = client_text(ctx, "Multiple games found. Run libresteam-repl.exe <PID> using the name suffix above.\n");
        return res == LDG_ERR_AOK ? LDG_ERR_BUSY : res;
    }

    return ldg_str_rbrcpy(ctx->path, ctx->names[0].text, sizeof(ctx->path));
}

static uint32_t client_addr_add(client_ctx_t *ctx, const char *text, uint32_t len)
{
    uint32_t idx = 0x0;
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!len || len >= LIBRESTEAM_EXT_REPL_CMD_CAP) { return LDG_ERR_AOK; }

    for (idx = 0; idx < CLIENT_ADDR_CAP; idx++)
    {
        for (pos = 0; pos < len && ctx->addresses[idx][pos] == text[pos]; pos++) {}
        if (pos == len && !ctx->addresses[idx][pos]) { return LDG_ERR_AOK; }
    }
    res = ldg_mem_secure_cpy(ctx->addresses[ctx->addr_next], text, len);
    if (res != LDG_ERR_AOK) { return res; }

    ctx->addresses[ctx->addr_next][len] = LDG_STR_TERM;
    ctx->addr_next = (ctx->addr_next + 1U) % CLIENT_ADDR_CAP;
    return res;
}

static uint32_t client_resp_rd(client_ctx_t *ctx, uint64_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    for (;;)
    {
        res = ldg_io_ipc_rd_timed(&ctx->pipe, (ldg_byte_t *)&ctx->resp, sizeof(ctx->resp), CLIENT_HELLO_POLL_MS, out);
        if (res == LDG_ERR_TIMEOUT) { continue; }

        if (res != LDG_ERR_AOK || *out) { return res; }

        res = ldg_thread_yield((uint64_t)CLIENT_HELLO_POLL_MS * LDG_NS_PER_MS);
        if (res != LDG_ERR_AOK) { return res; }
    }
}

static uint32_t client_addr_capture(client_ctx_t *ctx, const char *text, uint32_t len)
{
    uint32_t idx = 0x0;
    uint32_t end = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (idx = 0; idx + 2U < len; idx++)
    {
        if (text[idx] != '0' || (text[idx + 1U] != 'x' && text[idx + 1U] != 'X')) { continue; }

        if (idx && (text[idx - 1U] == '_' || ldg_char_alnum_is((uint8_t)text[idx - 1U]) == LDG_TRUTH_TRUE)) { continue; }

        end = idx + 2U;
        while (end < len && ldg_char_hex_is((uint8_t)text[end]) == LDG_TRUTH_TRUE) { end++; }
        if (end - idx - 2U >= CLIENT_ADDR_DIGITS_MIN && end - idx - 2U <= CLIENT_HEX_ADDR_DIGITS && (end == len || ldg_char_alnum_is((uint8_t)text[end]) != LDG_TRUTH_TRUE))
        {
            res = client_addr_add(ctx, text + idx, end - idx);
            if (res != LDG_ERR_AOK) { return res; }
        }

        idx = end - 1U;
    }
    return res;
}

static uint32_t client_request(client_ctx_t *ctx, const char *cmd, uint32_t len, uint8_t quiet)
{
    uint64_t bytes = 0x0;
    uint64_t id = 0x0;
    uint64_t text_len = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t streamed = LDG_TRUTH_FALSE;

    ctx->output_line_open = LDG_TRUTH_FALSE;
    res = ldg_io_ipc_wr(&ctx->pipe, (const ldg_byte_t *)cmd, len + LDG_STR_TERM_SIZE, &bytes);
    if (res != LDG_ERR_AOK || bytes != len + LDG_STR_TERM_SIZE) { return LDG_ERR_IO_WR; }

    do
    {
        if (client_resp_rd(ctx, &bytes) != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (bytes < LIBRESTEAM_EXT_REPL_RESP_HDR_SIZE + LDG_STR_TERM_SIZE || ctx->resp.len >= sizeof(ctx->resp.text) || bytes != LIBRESTEAM_EXT_REPL_RESP_HDR_SIZE + ctx->resp.len + LDG_STR_TERM_SIZE || ctx->resp.text[ctx->resp.len] != LDG_STR_TERM || ctx->resp.id <= ctx->previous || (id && id != ctx->resp.id)) { return LDG_ERR_INVALID; }

        id = ctx->resp.id;
        if (!quiet)
        {
            res = client_addr_capture(ctx, ctx->resp.text, ctx->resp.len);
            if (res == LDG_ERR_AOK) { res = client_resp_wr(ctx); }

            if (res != LDG_ERR_AOK) { return res; }
        }

        if (ctx->resp.res == LIBRESTEAM_EXT_REPL_RESP_MORE) { streamed = LDG_TRUTH_TRUE; }
    } while (ctx->resp.res == LIBRESTEAM_EXT_REPL_RESP_MORE);
    ctx->previous = id;
    if (!quiet) { ctx->command_res = ctx->resp.res; }

    if (quiet)
    {
        if (streamed) { ctx->resp.res = LDG_ERR_FULL; }

        return LDG_ERR_AOK;
    }

    if (ctx->resp.res != LDG_ERR_AOK)
    {
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "command failed; res: %u32\n", ctx->resp.res);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }
    }

    return res;
}

static uint32_t client_candidate_add(client_ctx_t *ctx, const char *value, const char *label)
{
    uint32_t idx = 0x0;
    uint32_t prefix = ctx->candidate_end - ctx->candidate_start;
    uint32_t res = LDG_ERR_AOK;

    for (idx = 0; idx < prefix; idx++) { if (value[idx] != ctx->cmd[ctx->candidate_start + idx]) { return LDG_ERR_AOK; } }
    for (idx = 0; idx < ctx->candidate_cunt; idx++) { if (client_eq_is(value, ctx->candidates[idx]) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; } }
    if (ctx->candidate_cunt == LIBRESTEAM_EXT_REPL_COMPLETE_CAP) { ctx->candidate_more = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

    res = ldg_str_rbrcpy(ctx->candidates[ctx->candidate_cunt], value, sizeof(ctx->candidates[0]));
    if (res == LDG_ERR_AOK)
    {
        for (idx = 0; idx + LDG_STR_TERM_SIZE < sizeof(ctx->labels[0]) && label[idx]; idx++) { ctx->labels[ctx->candidate_cunt][idx] = (uint8_t)label[idx] >= LDG_ASCII_PRINTABLE_MIN && (uint8_t)label[idx] <= LDG_ASCII_PRINTABLE_MAX ? label[idx] : ' '; }
        ctx->labels[ctx->candidate_cunt][idx] = LDG_STR_TERM;
    }

    if (res == LDG_ERR_AOK) { ctx->candidate_cunt++; }

    return res;
}

static uint32_t client_candidate_parse(client_ctx_t *ctx, uint32_t cursor)
{
    const char *header = "complete: ";
    char *value = 0x0;
    char *label = 0x0;
    uint64_t replace = 0x0;
    uint32_t idx = 0x0;
    uint32_t start = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->resp.res != LDG_ERR_AOK) { return LDG_ERR_NOT_FOUND; }

    while (header[idx])
    {
        if (idx >= ctx->resp.len || ctx->resp.text[idx] != header[idx]) { return LDG_ERR_INVALID; }

        idx++;
    }
    start = idx;
    while (idx < ctx->resp.len && ldg_char_digit_is((uint8_t)ctx->resp.text[idx]) == LDG_TRUTH_TRUE) { idx++; }
    if (idx == start || idx + 3U > ctx->resp.len || ctx->resp.text[idx] != ' ' || (ctx->resp.text[idx + 1U] != '0' && ctx->resp.text[idx + 1U] != '1') || ctx->resp.text[idx + 2U] != '\n') { return LDG_ERR_INVALID; }

    res = ldg_str_uint_parse(ctx->resp.text + start, LDG_BASE_DEC, idx - start, &replace);
    if (res != LDG_ERR_AOK || replace > cursor) { return LDG_ERR_INVALID; }

    ctx->candidate_start = cursor - (uint32_t)replace;
    ctx->candidate_end = cursor;
    ctx->candidate_space = ctx->resp.text[idx + 1U] == '1';
    idx += 3U;
    while (idx < ctx->resp.len)
    {
        if (ctx->resp.text[idx] == '!' && idx + 1U < ctx->resp.len && ctx->resp.text[idx + 1U] == '\n') { ctx->candidate_more = LDG_TRUTH_TRUE; idx += 2U; continue; }

        value = ctx->resp.text + idx;
        while (idx < ctx->resp.len && ctx->resp.text[idx] != '\t')
        {
            if ((uint8_t)ctx->resp.text[idx] < LDG_ASCII_PRINTABLE_MIN || (uint8_t)ctx->resp.text[idx] > LDG_ASCII_PRINTABLE_MAX) { return LDG_ERR_INVALID; }

            idx++;
        }
        if (idx == ctx->resp.len) { return LDG_ERR_INVALID; }

        ctx->resp.text[idx++] = LDG_STR_TERM;
        label = ctx->resp.text + idx;
        while (idx < ctx->resp.len && ctx->resp.text[idx] != '\n')
        {
            if ((uint8_t)ctx->resp.text[idx] < LDG_ASCII_PRINTABLE_MIN || (uint8_t)ctx->resp.text[idx] > LDG_ASCII_PRINTABLE_MAX) { return LDG_ERR_INVALID; }

            idx++;
        }
        if (idx == ctx->resp.len) { return LDG_ERR_INVALID; }

        ctx->resp.text[idx++] = LDG_STR_TERM;
        res = client_candidate_add(ctx, value, label);
        if (res != LDG_ERR_AOK) { return res; }
    }
    return res;
}

static uint32_t client_candidate_local(client_ctx_t *ctx, uint32_t cursor, uint8_t *handled)
{
    const char *commands[] = { "aliases", "vars", "unset", "set", "quit" };
    uint32_t idx = 0x0;
    uint32_t end = 0x0;
    uint32_t mark = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;
    uint64_t text_len = 0x0;
    uint8_t braced = LDG_TRUTH_FALSE;

    *handled = LDG_TRUTH_FALSE;
    ctx->candidate_start = cursor;
    ctx->candidate_end = cursor;
    ctx->candidate_space = LDG_TRUTH_TRUE;
    for (idx = 0; idx < cursor; idx++)
    {
        if (ctx->cmd[idx] == '-' && idx + 1U < cursor && ctx->cmd[idx + 1U] == '-')
        {
            res = client_literal_end(ctx->cmd, cursor, idx + 2U, &end);
            if (res != LDG_ERR_AOK || end == idx + 2U) { *handled = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

            idx = end - 1U;
            continue;
        }

        res = client_literal_end(ctx->cmd, cursor, idx, &end);
        if (res != LDG_ERR_AOK) { *handled = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

        if (end != idx) { idx = end - 1U; continue; }

        if (ctx->cmd[idx] == '$')
        {
            if (idx + 1U < cursor && ctx->cmd[idx + 1U] == '$') { idx++; continue; }

            mark = idx;
        }
    }
    if (mark != UINT32_MAX)
    {
        idx = mark + 1U;
        if (idx < cursor && ctx->cmd[idx] == '{') { braced = LDG_TRUTH_TRUE; idx++; }

        for (; idx < cursor; idx++) { if (ctx->cmd[idx] != '_' && ldg_char_alnum_is((uint8_t)ctx->cmd[idx]) != LDG_TRUTH_TRUE) { break; } }
        if (idx == cursor)
        {
            *handled = LDG_TRUTH_TRUE;
            ctx->candidate_start = mark;
            ctx->candidate_space = LDG_TRUTH_FALSE;
            for (idx = 0; idx < CLIENT_VAR_CAP; idx++)
            {
                if (!ctx->vars[idx].name[0]) { continue; }

                res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, braced ? "$" "{%s}" : "$%s", ctx->vars[idx].name);
                if (res == LDG_ERR_AOK) { res = client_candidate_add(ctx, ctx->line, ctx->vars[idx].val); }

                if (res != LDG_ERR_AOK) { return res; }
            }
            return res;
        }
    }

    while (ctx->candidate_start && ctx->cmd[ctx->candidate_start - 1U] != ' ' && ctx->cmd[ctx->candidate_start - 1U] != '\t') { ctx->candidate_start--; }
    idx = cursor;
    while (idx && ldg_char_hex_is((uint8_t)ctx->cmd[idx - 1U]) == LDG_TRUTH_TRUE) { idx--; }
    if (idx >= 2U && ctx->cmd[idx - 1U] == 'x' && ctx->cmd[idx - 2U] == '0' && (idx == 2U || (ctx->cmd[idx - 3U] != '_' && ldg_char_alnum_is((uint8_t)ctx->cmd[idx - 3U]) != LDG_TRUTH_TRUE)))
    {
        *handled = LDG_TRUTH_TRUE;
        ctx->candidate_start = idx - 2U;
        ctx->candidate_space = LDG_TRUTH_FALSE;
        for (idx = 0; idx < CLIENT_ADDR_CAP; idx++)
        {
            if (!ctx->addresses[idx][0]) { continue; }

            res = client_candidate_add(ctx, ctx->addresses[idx], "observed address");
            if (res != LDG_ERR_AOK) { return res; }
        }
        for (idx = 0; idx < CLIENT_VAR_CAP; idx++)
        {
            if (ctx->vars[idx].val[0] != '0' || ctx->vars[idx].val[1] != 'x') { continue; }

            res = client_candidate_add(ctx, ctx->vars[idx].val, ctx->vars[idx].name);
            if (res != LDG_ERR_AOK) { return res; }
        }
        return res;
    }

    if (ctx->candidate_start == 0)
    {
        for (idx = 0; res == LDG_ERR_AOK && idx < sizeof(commands) / sizeof(commands[0]); idx++) { res = client_candidate_add(ctx, commands[idx], "client"); }
        for (idx = 0; res == LDG_ERR_AOK && idx < CLIENT_ALIAS_CAP; idx++) { if (ctx->aliases[idx].name[0]) { res = client_candidate_add(ctx, ctx->aliases[idx].name, ctx->aliases[idx].text); } }
        return res;
    }

    if ((ctx->candidate_start == 6U && ctx->cmd[0] == 'u' && ctx->cmd[1] == 'n' && ctx->cmd[2] == 's' && ctx->cmd[3] == 'e' && ctx->cmd[4] == 't') || (ctx->candidate_start == 4U && ctx->cmd[0] == 's' && ctx->cmd[1] == 'e' && ctx->cmd[2] == 't'))
    {
        /* Numeric IL2CPP set arguments continue through backend completion. */
        if (ctx->candidate_start == 4U && cursor > 4U && ldg_char_digit_is((uint8_t)ctx->cmd[4]) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        *handled = LDG_TRUTH_TRUE;
        ctx->candidate_space = LDG_TRUTH_FALSE;
        for (idx = 0; idx < CLIENT_VAR_CAP; idx++)
        {
            if (!ctx->vars[idx].name[0]) { continue; }

            res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, ctx->candidate_start == 4U ? "%s=" : "%s", ctx->vars[idx].name);
            if (res == LDG_ERR_AOK) { res = client_candidate_add(ctx, ctx->line, ctx->vars[idx].val); }

            if (res != LDG_ERR_AOK) { return res; }
        }
    }

    return res;
}

static uint32_t client_candidate_gather(client_ctx_t *ctx, uint32_t cursor)
{
    char request[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    char held[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    char label[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t request_len = 0x0;
    uint32_t prefix_len = cursor;
    uint32_t local_cunt = 0x0;
    uint32_t local_start = 0x0;
    uint32_t idx = 0x0;
    uint32_t prior = 0x0;
    uint32_t cmp = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t handled = LDG_TRUTH_FALSE;
    uint8_t local_space = LDG_TRUTH_FALSE;

    ctx->candidate_cunt = 0;
    ctx->candidate_more = LDG_TRUTH_FALSE;
    ctx->candidate_sel = UINT32_MAX;
    res = client_candidate_local(ctx, cursor, &handled);
    if (res != LDG_ERR_AOK) { return res; }

    if (!handled)
    {
        local_cunt = ctx->candidate_cunt;
        local_start = ctx->candidate_start;
        local_space = ctx->candidate_space;
        res = ldg_str_rbrcpy(ctx->complete_orig, ctx->cmd, sizeof(ctx->complete_orig));
        if (res != LDG_ERR_AOK) { return res; }

        ctx->cmd[cursor] = LDG_STR_TERM;
        res = client_var_expand(ctx, &prefix_len);
        if (res == LDG_ERR_AOK) { res = ldg_str_fmt(request, sizeof(request), &request_len, "complete %s", ctx->cmd); }

        if (ldg_str_rbrcpy(ctx->cmd, ctx->complete_orig, sizeof(ctx->cmd)) != LDG_ERR_AOK) { return LDG_ERR_INVALID; }

        if (res == LDG_ERR_AOK)
        {
            res = client_request(ctx, request, (uint32_t)request_len, LDG_TRUTH_TRUE);
            if (res != LDG_ERR_AOK) { return res; }

            res = client_candidate_parse(ctx, cursor);
            if (res != LDG_ERR_AOK) { ctx->candidate_cunt = local_cunt; ctx->candidate_start = local_start; ctx->candidate_end = cursor; ctx->candidate_more = LDG_TRUTH_FALSE; ctx->candidate_space = local_space; }
        }
    }

    for (idx = 1; idx < ctx->candidate_cunt; idx++)
    {
        res = ldg_str_rbrcpy(held, ctx->candidates[idx], sizeof(held));
        if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(label, ctx->labels[idx], sizeof(label)); }

        if (res != LDG_ERR_AOK) { return res; }

        prior = idx;
        while (prior)
        {
            res = ldg_str_cmp(ctx->candidates[prior - 1U], held, sizeof(held), &cmp);
            if (res != LDG_ERR_AOK) { return res; }

            if (cmp != LDG_ARITH_ORD_GT) { break; }

            res = ldg_str_rbrcpy(ctx->candidates[prior], ctx->candidates[prior - 1U], sizeof(ctx->candidates[0]));
            if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(ctx->labels[prior], ctx->labels[prior - 1U], sizeof(ctx->labels[0])); }

            if (res != LDG_ERR_AOK) { return res; }

            prior--;
        }
        res = ldg_str_rbrcpy(ctx->candidates[prior], held, sizeof(held));
        if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(ctx->labels[prior], label, sizeof(label)); }

        if (res != LDG_ERR_AOK) { return res; }
    }
    return LDG_ERR_AOK;
}

static uint32_t client_candidate_put(client_ctx_t *ctx, const char *value, uint32_t size, uint32_t *len, uint8_t space)
{
    char result[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0x0;
    uint32_t dst = 0x0;
    uint32_t suffix = ctx->candidate_end;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->candidate_start > ctx->candidate_end || ctx->candidate_end > *len) { return LDG_ERR_BOUNDS; }

    if (size && value[size - 1U] == '}' && ctx->cmd[suffix] == '}') { suffix++; }

    if (space && (suffix < *len || !size || value[size - 1U] == '.' || value[size - 1U] == '=')) { space = LDG_TRUTH_FALSE; }

    if (ctx->candidate_start + size + *len - suffix >= sizeof(result)) { return LDG_ERR_FULL; }

    if (ctx->candidate_start + size + space + *len - suffix >= sizeof(result)) { space = LDG_TRUTH_FALSE; }

    for (idx = 0; idx < ctx->candidate_start; idx++) { result[dst++] = ctx->cmd[idx]; }
    for (idx = 0; idx < size; idx++) { result[dst++] = value[idx]; }
    ctx->candidate_end = dst;
    if (space) { result[dst++] = ' '; }

    ctx->candidate_cursor = dst;
    for (idx = suffix; idx < *len; idx++) { result[dst++] = ctx->cmd[idx]; }
    result[dst] = LDG_STR_TERM;
    res = ldg_str_rbrcpy(ctx->cmd, result, sizeof(ctx->cmd));
    if (res != LDG_ERR_AOK) { return res; }

    *len = dst;
    return ldg_str_rbrcpy(ctx->complete_last, ctx->cmd, sizeof(ctx->complete_last));
}

static uint32_t client_candidate_tab(client_ctx_t *ctx, uint32_t *len, uint32_t cursor, uint8_t back)
{
    uint64_t size = 0x0;
    uint32_t common = 0x0;
    uint32_t idx = 0x0;
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;

    ctx->candidate_redraw = LDG_TRUTH_FALSE;
    if (ctx->candidate_on && client_eq_is(ctx->cmd, ctx->complete_last) == LDG_TRUTH_TRUE && cursor == ctx->candidate_cursor && ctx->candidate_cunt)
    {
        ctx->candidate_redraw = LDG_TRUTH_TRUE;
        if (ctx->candidate_sel == UINT32_MAX) { ctx->candidate_sel = back ? ctx->candidate_cunt - 1U : 0; }
        else { ctx->candidate_sel = back ? (ctx->candidate_sel + ctx->candidate_cunt - 1U) % ctx->candidate_cunt : (ctx->candidate_sel + 1U) % ctx->candidate_cunt; }

        res = ldg_str_len_get(ctx->candidates[ctx->candidate_sel], sizeof(ctx->candidates[0]), &size);
        return res == LDG_ERR_AOK ? client_candidate_put(ctx, ctx->candidates[ctx->candidate_sel], (uint32_t)size, len, LDG_TRUTH_FALSE) : res;
    }

    ctx->candidate_on = LDG_TRUTH_FALSE;
    ctx->candidate_cursor = cursor;
    res = client_candidate_gather(ctx, cursor);
    if (res != LDG_ERR_AOK || !ctx->candidate_cunt) { return res; }

    res = ldg_str_len_get(ctx->candidates[0], sizeof(ctx->candidates[0]), &size);
    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->candidate_cunt == 1U && !ctx->candidate_more) { return client_candidate_put(ctx, ctx->candidates[0], (uint32_t)size, len, ctx->candidate_space); }

    common = ctx->candidate_more ? ctx->candidate_end - ctx->candidate_start : (uint32_t)size;
    for (idx = 1; idx < ctx->candidate_cunt; idx++)
    {
        for (pos = 0; pos < common && ctx->candidates[0][pos] == ctx->candidates[idx][pos]; pos++) {}
        common = pos;
    }
    if (common > ctx->candidate_end - ctx->candidate_start) { res = client_candidate_put(ctx, ctx->candidates[0], common, len, LDG_TRUTH_FALSE); }

    if (res != LDG_ERR_AOK) { return res; }

    ctx->candidate_on = LDG_TRUTH_TRUE;
    return ldg_str_rbrcpy(ctx->complete_last, ctx->cmd, sizeof(ctx->complete_last));
}

static uint32_t client_width_get(void)
{
    uint32_t cols = 0x0;

    if (ldg_sys_tty_width_get(&cols) != LDG_ERR_AOK || cols < CLIENT_WIDTH_MIN) { return CLIENT_TERMINAL_FALLBACK_COLS; }

    return cols;
}

static uint32_t client_rows_up(client_ctx_t *ctx, uint32_t rows)
{
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!rows) { return LDG_ERR_AOK; }

    res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &len, CLIENT_ROW_UP_FMT, rows);
    return res == LDG_ERR_AOK ? client_wr(ctx, ctx->line, (uint32_t)len) : res;
}

static uint32_t client_edit_clear(client_ctx_t *ctx)
{
    uint32_t res = ctx->edit_drawn ? client_rows_up(ctx, ctx->edit_row) : LDG_ERR_AOK;

    ctx->edit_row = 0;
    ctx->edit_drawn = LDG_TRUTH_FALSE;
    return res == LDG_ERR_AOK ? client_text(ctx, CLIENT_ERASE_BELOW) : res;
}

static uint32_t client_candidate_draw(client_ctx_t *ctx)
{
    uint64_t text_len = 0x0;
    uint32_t idx = 0x0;
    uint32_t start = 0x0;
    uint32_t rows = 0x0;
    uint32_t width = client_width_get();
    uint32_t room = width - CLIENT_COL_BASE;
    uint32_t shown = 0x0;
    uint32_t res = client_edit_clear(ctx);

    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->candidate_redraw && ctx->candidate_drawn && ctx->candidate_width == width)
    {
        res = client_rows_up(ctx, ctx->candidate_rows);
        if (res == LDG_ERR_AOK) { res = client_text(ctx, CLIENT_ERASE_BELOW); }
    }
    else { res = client_text(ctx, "\n"); }

    ctx->candidate_drawn = LDG_TRUTH_FALSE;
    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->candidate_on)
    {
        if (ctx->candidate_sel != UINT32_MAX) { start = (ctx->candidate_sel / CLIENT_CANDIDATE_ROWS) * CLIENT_CANDIDATE_ROWS; }

        for (idx = start; res == LDG_ERR_AOK && idx < ctx->candidate_cunt && idx < start + CLIENT_CANDIDATE_ROWS; idx++)
        {
            res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "%s%s\x20\x20%s", idx == ctx->candidate_sel ? "> " : "\x20\x20", ctx->candidates[idx], ctx->labels[idx]);
            shown = text_len < room ? (uint32_t)text_len : room;
            if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, shown); }

            if (res == LDG_ERR_AOK) { res = client_text(ctx, "\n"); }

            rows++;
        }
        if (res == LDG_ERR_AOK)
        {
            res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "Tab/Shift+Tab: cycle; Enter: accept; Esc: close; matches: %u32%s", ctx->candidate_cunt, ctx->candidate_more ? "+" : "");
            shown = text_len < room ? (uint32_t)text_len : room;
            if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, shown); }

            if (res == LDG_ERR_AOK) { res = client_text(ctx, "\n"); }

            rows++;
        }
    }

    if (res == LDG_ERR_AOK) { res = client_text(ctx, CLIENT_PROMPT_COLOR); }

    rows += (sizeof(CLIENT_PROMPT) - LDG_STR_TERM_SIZE) / width;
    if (res == LDG_ERR_AOK)
    {
        ctx->candidate_rows = rows;
        ctx->candidate_width = width;
        ctx->candidate_drawn = LDG_TRUTH_TRUE;
    }

    return res;
}

static uint32_t client_edit_put(client_ctx_t *ctx, const char *text, uint32_t *len)
{
    uint64_t size = 0x0;
    uint32_t res = ldg_str_len_get(text, sizeof(ctx->cmd), &size);

    if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(ctx->cmd, text, sizeof(ctx->cmd)); }

    if (res != LDG_ERR_AOK) { return res; }

    *len = (uint32_t)size;
    ctx->edit_cursor = *len;
    return LDG_ERR_AOK;
}

static uint32_t client_edit_insert(client_ctx_t *ctx, uint32_t *len, uint32_t ch)
{
    uint32_t idx = *len;

    if (ctx->edit_cursor > *len) { return LDG_ERR_BOUNDS; }

    if (*len + LDG_STR_TERM_SIZE >= sizeof(ctx->cmd)) { return LDG_ERR_FULL; }

    if (ch < LDG_ASCII_PRINTABLE_MIN || ch > LDG_ASCII_PRINTABLE_MAX) { return LDG_ERR_STR_INVALID; }

    for (; idx > ctx->edit_cursor; idx--) { ctx->cmd[idx] = ctx->cmd[idx - 1U]; }
    ctx->cmd[ctx->edit_cursor++] = (char)ch;
    ctx->cmd[++(*len)] = LDG_STR_TERM;
    return LDG_ERR_AOK;
}

static uint32_t client_edit_drop(client_ctx_t *ctx, uint32_t *len, uint32_t start, uint32_t end)
{
    uint32_t idx = start;

    if (start > end || end > *len) { return LDG_ERR_BOUNDS; }

    for (; end <= *len; end++) { ctx->cmd[idx++] = ctx->cmd[end]; }
    *len -= end - idx;
    ctx->edit_cursor = start;
    return LDG_ERR_AOK;
}

static uint32_t client_edit_word_get(client_ctx_t *ctx, uint32_t len, uint8_t back, uint32_t *pos)
{
    *pos = ctx->edit_cursor;
    if (*pos > len) { return LDG_ERR_BOUNDS; }

    if (back)
    {
        while (*pos && ctx->cmd[*pos - 1U] == ' ') { (*pos)--; }
        while (*pos && ctx->cmd[*pos - 1U] != ' ') { (*pos)--; }
    }
    else
    {
        while (*pos < len && ctx->cmd[*pos] == ' ') { (*pos)++; }
        while (*pos < len && ctx->cmd[*pos] != ' ') { (*pos)++; }
    }

    return LDG_ERR_AOK;
}

static uint32_t client_history_walk(client_ctx_t *ctx, uint32_t *len, uint8_t older)
{
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->history_cunt) { return LDG_ERR_AOK; }

    if (older)
    {
        if (!ctx->history_sel) { return LDG_ERR_AOK; }

        if (ctx->history_sel == ctx->history_cunt)
        {
            res = ldg_str_rbrcpy(ctx->draft, ctx->cmd, sizeof(ctx->draft));
            if (res != LDG_ERR_AOK) { return res; }

            ctx->draft_cursor = ctx->edit_cursor;
        }

        ctx->history_sel--;
    }
    else
    {
        if (ctx->history_sel >= ctx->history_cunt) { return LDG_ERR_AOK; }

        ctx->history_sel++;
    }

    if (ctx->history_sel < ctx->history_cunt) { return client_edit_put(ctx, ctx->history[ctx->history_sel].text, len); }

    res = client_edit_put(ctx, ctx->draft, len);
    if (res == LDG_ERR_AOK) { ctx->edit_cursor = ctx->draft_cursor; }

    return res;
}

static uint8_t client_search_match_is(const char *text, const char *needle)
{
    uint32_t pos = 0x0;
    uint32_t idx = 0x0;

    if (!needle[0]) { return LDG_TRUTH_TRUE; }

    for (pos = 0; text[pos]; pos++)
    {
        for (idx = 0; needle[idx] && text[pos + idx] && needle[idx] == text[pos + idx]; idx++) {}
        if (!needle[idx]) { return LDG_TRUTH_TRUE; }
    }
    return LDG_TRUTH_FALSE;
}

static uint32_t client_search_step(client_ctx_t *ctx, uint32_t *len, uint8_t newer, uint8_t restart)
{
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->searching)
    {
        res = ldg_str_rbrcpy(ctx->search_draft, ctx->cmd, sizeof(ctx->search_draft));
        if (res != LDG_ERR_AOK) { return res; }

        ctx->search_cursor = ctx->edit_cursor;
        ctx->searching = LDG_TRUTH_TRUE;
        ctx->search[0] = LDG_STR_TERM;
        ctx->search_len = 0;
        ctx->search_sel = UINT32_MAX;
        restart = LDG_TRUTH_TRUE;
    }

    idx = restart || ctx->search_sel == UINT32_MAX ? (newer ? 0 : ctx->history_cunt) : (newer ? ctx->search_sel + 1U : ctx->search_sel);
    ctx->search_failed = LDG_TRUTH_TRUE;
    while (newer ? idx < ctx->history_cunt : idx > 0)
    {
        if (!newer) { idx--; }

        if (client_search_match_is(ctx->history[idx].text, ctx->search) == LDG_TRUTH_TRUE)
        {
            ctx->search_sel = idx;
            ctx->search_failed = LDG_TRUTH_FALSE;
            return client_edit_put(ctx, ctx->history[idx].text, len);
        }

        if (newer) { idx++; }
    }
    return LDG_ERR_AOK;
}

static uint32_t client_ghost_refresh(client_ctx_t *ctx, uint32_t len)
{
    uint32_t idx = ctx->history_cunt;
    uint32_t pos = 0x0;

    ctx->ghost_sel = UINT32_MAX;
    if (!len || ctx->edit_cursor != len || ctx->searching || ctx->candidate_on || ctx->ghost_hidden) { return LDG_ERR_AOK; }

    while (idx)
    {
        idx--;
        for (pos = 0; pos < len && ctx->cmd[pos] == ctx->history[idx].text[pos]; pos++) {}
        if (pos == len && ctx->history[idx].text[len]) { ctx->ghost_sel = idx; break; }
    }
    return LDG_ERR_AOK;
}

static uint32_t client_ghost_accept(client_ctx_t *ctx, uint32_t *len, uint8_t word)
{
    const char *text = 0x0;
    uint32_t end = *len;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->ghost_sel == UINT32_MAX || ctx->edit_cursor != *len) { return LDG_ERR_AOK; }

    text = ctx->history[ctx->ghost_sel].text;
    if (word)
    {
        while (text[end] == ' ') { end++; }
        while (text[end] && text[end] != ' ' && text[end] != '/' && text[end] != '\\') { end++; }
        if (text[end] == '/' || text[end] == '\\') { end++; }
    }
    else { while (text[end]) { end++; } }

    for (; *len < end && res == LDG_ERR_AOK;) { res = client_edit_insert(ctx, len, (uint8_t)text[*len]); }
    return res;
}

static uint32_t client_edit_key(client_ctx_t *ctx, uint32_t *len, uint32_t key, uint32_t ch, uint8_t ctrl, uint8_t shift, uint8_t *submit)
{
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *submit = LDG_TRUTH_FALSE;
    ctx->ghost_hidden = key == CLIENT_KEY_ESC;
    if (ctrl && key == 'D') { return LDG_ERR_EOF; }

    if (ctrl && key == 'C')
    {
        ctx->cmd[0] = LDG_STR_TERM;
        *len = 0;
        ctx->edit_cursor = 0;
        ctx->searching = LDG_TRUTH_FALSE;
        ctx->candidate_on = LDG_TRUTH_FALSE;
        ctx->ghost_sel = UINT32_MAX;
        ctx->history_sel = ctx->history_cunt;
        return LDG_ERR_AOK;
    }

    if (ctrl && key == 'R') { return client_search_step(ctx, len, shift, LDG_TRUTH_FALSE); }

    if (ctx->searching)
    {
        if (key == CLIENT_KEY_ESC || (ctrl && key == 'G'))
        {
            ctx->searching = LDG_TRUTH_FALSE;
            res = client_edit_put(ctx, ctx->search_draft, len);
            if (res == LDG_ERR_AOK) { ctx->edit_cursor = ctx->search_cursor; }

            return res;
        }

        if (key == CLIENT_KEY_ENTER)
        {
            ctx->searching = LDG_TRUTH_FALSE;
            if (!ctx->search_failed && ctx->search_sel != UINT32_MAX) { *submit = LDG_TRUTH_TRUE; }

            return LDG_ERR_AOK;
        }

        if (key == CLIENT_KEY_BACKSPACE && ctx->search_len) { ctx->search[--ctx->search_len] = LDG_STR_TERM; }
        else if (ctrl && key == 'W')
        {
            while (ctx->search_len && ctx->search[ctx->search_len - 1U] == ' ') { ctx->search_len--; }
            while (ctx->search_len && ctx->search[ctx->search_len - 1U] != ' ') { ctx->search_len--; }
            ctx->search[ctx->search_len] = LDG_STR_TERM;
        }
        else if (!ctrl && ch >= LDG_ASCII_PRINTABLE_MIN && ch <= LDG_ASCII_PRINTABLE_MAX)
        {
            if (ctx->search_len + LDG_STR_TERM_SIZE >= sizeof(ctx->search)) { return LDG_ERR_FULL; }

            ctx->search[ctx->search_len++] = (char)ch;
            ctx->search[ctx->search_len] = LDG_STR_TERM;
        }
        else { return LDG_ERR_AOK; }

        return client_search_step(ctx, len, LDG_TRUTH_FALSE, LDG_TRUTH_TRUE);
    }

    if (key == CLIENT_KEY_ENTER) { *submit = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

    if (key == CLIENT_KEY_UP || key == CLIENT_KEY_PAGE_UP) { return client_history_walk(ctx, len, LDG_TRUTH_TRUE); }

    if (key == CLIENT_KEY_DOWN || key == CLIENT_KEY_PAGE_DOWN) { return client_history_walk(ctx, len, LDG_TRUTH_FALSE); }

    if (key == CLIENT_KEY_HOME || (ctrl && key == 'A')) { ctx->edit_cursor = 0; return LDG_ERR_AOK; }

    if (key == CLIENT_KEY_END || (ctrl && key == 'E'))
    {
        if (key == CLIENT_KEY_END && ctx->edit_cursor == *len) { res = client_ghost_accept(ctx, len, LDG_TRUTH_FALSE); }

        ctx->edit_cursor = *len;
        return res;
    }

    if (key == CLIENT_KEY_LHS || key == CLIENT_KEY_RHS)
    {
        if (key == CLIENT_KEY_RHS && ctx->edit_cursor == *len) { return client_ghost_accept(ctx, len, ctrl); }

        if (ctrl) { return client_edit_word_get(ctx, *len, key == CLIENT_KEY_LHS, &ctx->edit_cursor); }

        if (key == CLIENT_KEY_LHS && ctx->edit_cursor) { ctx->edit_cursor--; }

        if (key == CLIENT_KEY_RHS && ctx->edit_cursor < *len) { ctx->edit_cursor++; }

        return LDG_ERR_AOK;
    }

    if (ctrl && key == 'U') { return client_edit_drop(ctx, len, 0, ctx->edit_cursor); }

    if (ctrl && key == 'K') { return client_edit_drop(ctx, len, ctx->edit_cursor, *len); }

    if (ctrl && (key == 'W' || key == CLIENT_KEY_BACKSPACE))
    {
        res = client_edit_word_get(ctx, *len, LDG_TRUTH_TRUE, &pos);
        return res == LDG_ERR_AOK ? client_edit_drop(ctx, len, pos, ctx->edit_cursor) : res;
    }

    if (key == CLIENT_KEY_BACKSPACE && ctx->edit_cursor) { return client_edit_drop(ctx, len, ctx->edit_cursor - 1U, ctx->edit_cursor); }

    if (key == CLIENT_KEY_DELETE && ctx->edit_cursor < *len) { return client_edit_drop(ctx, len, ctx->edit_cursor, ctx->edit_cursor + 1U); }

    if (!ctrl && ch >= LDG_ASCII_PRINTABLE_MIN) { return client_edit_insert(ctx, len, ch); }

    return LDG_ERR_AOK;
}

static uint32_t client_edit_draw(client_ctx_t *ctx, uint32_t len)
{
    uint64_t ghost_len = 0x0;
    uint64_t text_len = 0x0;
    uint32_t width = client_width_get();
    uint32_t cells = 0x0;
    uint32_t cursor_cell = 0x0;
    uint32_t res = client_edit_clear(ctx);

    if (res != LDG_ERR_AOK) { return res; }

    ctx->edit_width = width;
    if (ctx->searching)
    {
        res = client_text(ctx, CLIENT_SEARCH_PROMPT);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->search, ctx->search_len); }

        cursor_cell = sizeof(CLIENT_SEARCH_PROMPT) - LDG_STR_TERM_SIZE + ctx->search_len;
        if (res == LDG_ERR_AOK) { res = client_text(ctx, ctx->search_failed ? "! " : ": "); }

        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->cmd, len); }

        cells = cursor_cell + CLIENT_SEARCH_SEP_COLS + len;
    }
    else
    {
        res = client_text(ctx, CLIENT_PROMPT_COLOR);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->cmd, len); }

        cells = sizeof(CLIENT_PROMPT) - LDG_STR_TERM_SIZE + len;
        cursor_cell = sizeof(CLIENT_PROMPT) - LDG_STR_TERM_SIZE + ctx->edit_cursor;
        if (res == LDG_ERR_AOK && ctx->ghost_sel != UINT32_MAX)
        {
            res = ldg_str_len_get(ctx->history[ctx->ghost_sel].text, sizeof(ctx->history[ctx->ghost_sel].text), &ghost_len);
            if (res == LDG_ERR_AOK && ghost_len < len) { res = LDG_ERR_INVALID; }

            if (res == LDG_ERR_AOK) { res = client_text(ctx, CLIENT_GHOST_ON); }

            if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->history[ctx->ghost_sel].text + len, (uint32_t)ghost_len - len); }

            if (res == LDG_ERR_AOK) { res = client_text(ctx, CLIENT_GHOST_OFF); }

            if (res == LDG_ERR_AOK) { cells += (uint32_t)ghost_len - len; }
        }
    }

    if (res == LDG_ERR_AOK && cells % width == 0) { res = client_text(ctx, "\r\n"); }

    if (res == LDG_ERR_AOK) { res = client_rows_up(ctx, cells / width - cursor_cell / width); }

    if (res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, CLIENT_COL_FMT, cursor_cell % width + CLIENT_COL_BASE); }

    if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }

    if (res != LDG_ERR_AOK) { return res; }

    ctx->edit_row = cursor_cell / width;
    ctx->edit_drawn = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t client_edit_finish(client_ctx_t *ctx, uint32_t len)
{
    uint32_t res = LDG_ERR_AOK;

    ctx->ghost_sel = UINT32_MAX;
    ctx->searching = LDG_TRUTH_FALSE;
    ctx->edit_cursor = len;
    res = client_edit_draw(ctx, len);
    if (res == LDG_ERR_AOK && (sizeof(CLIENT_PROMPT) - LDG_STR_TERM_SIZE + len) % ctx->edit_width) { res = client_text(ctx, "\r\n"); }

    ctx->edit_drawn = LDG_TRUTH_FALSE;
    return res;
}

static uint32_t client_redirect_parse(client_ctx_t *ctx, uint32_t *len)
{
    uint32_t idx = 0x0;
    uint32_t mark = UINT32_MAX;
    uint32_t start = 0x0;
    uint32_t end = 0x0;
    uint32_t depth = 0x0;
    uint32_t long_eq = UINT32_MAX;
    uint32_t probe = 0x0;
    uint8_t quote = 0x0;
    uint8_t ch = 0x0;

    ctx->file_path[0] = LDG_STR_TERM;
    for (idx = 0; idx < *len; idx++)
    {
        ch = (uint8_t)ctx->cmd[idx];
        if (long_eq != UINT32_MAX)
        {
            if (ch != ']') { continue; }

            probe = idx + 1U;
            while (probe < *len && ctx->cmd[probe] == '=') { probe++; }
            if (probe < *len && probe - idx - 1U == long_eq && ctx->cmd[probe] == ']') { idx = probe; long_eq = UINT32_MAX; }

            continue;
        }

        if (quote)
        {
            if (ch == '\\' && idx + 1U < *len) { idx++; }
            else if (ch == quote) { quote = 0; }

            continue;
        }

        if (ch == '\'' || ch == '"') { quote = ch; continue; }

        if (ch == '[')
        {
            probe = idx + 1U;
            while (probe < *len && ctx->cmd[probe] == '=') { probe++; }
            if (probe < *len && ctx->cmd[probe] == '[') { long_eq = probe - idx - 1U; idx = probe; continue; }
        }

        if (ch == '(' || ch == '{' || ch == '[') { depth++; continue; }

        if (ch == ')' || ch == '}' || ch == ']')
        {
            if (depth) { depth--; }

            continue;
        }

        if (ch == '>' && !depth && idx + 1U < *len && ctx->cmd[idx + 1U] == '=') { idx++; continue; }

        if (ch == '>' && !depth) { mark = idx; break; }
    }
    if (mark == UINT32_MAX) { return LDG_ERR_AOK; }

    start = mark + 1U;
    while (start < *len && ldg_char_space_is((uint8_t)ctx->cmd[start]) == LDG_TRUTH_TRUE) { start++; }
    if (start == *len || ctx->cmd[start] == '>') { return LDG_ERR_FUNC_ARG_INVALID; }

    end = start;
    if (ctx->cmd[start] == '\'' || ctx->cmd[start] == '"')
    {
        quote = (uint8_t)ctx->cmd[start++];
        end = start;
        while (end < *len && (uint8_t)ctx->cmd[end] != quote) { end++; }
        if (end == *len) { return LDG_ERR_STR_INVALID; }

        probe = end + 1U;
    }
    else
    {
        while (end < *len && ldg_char_space_is((uint8_t)ctx->cmd[end]) != LDG_TRUTH_TRUE) { end++; }
        probe = end;
    }

    while (probe < *len && ldg_char_space_is((uint8_t)ctx->cmd[probe]) == LDG_TRUTH_TRUE) { probe++; }
    if (probe != *len || end == start) { return LDG_ERR_STR_INVALID; }

    for (idx = start; idx < end; idx++) { ctx->file_path[idx - start] = ctx->cmd[idx]; }
    ctx->file_path[end - start] = LDG_STR_TERM;
    while (mark && ldg_char_space_is((uint8_t)ctx->cmd[mark - 1U]) == LDG_TRUTH_TRUE) { mark--; }
    if (!mark) { return LDG_ERR_FUNC_ARG_INVALID; }

    ctx->cmd[mark] = LDG_STR_TERM;
    *len = mark;
    return LDG_ERR_AOK;
}

static uint32_t client_execute(client_ctx_t *ctx, uint32_t len)
{
    uint32_t cmp = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;
    uint64_t text_len = 0;
    uint8_t handled = LDG_TRUTH_FALSE;

    ctx->command_res = LDG_ERR_AOK;
    ctx->history_alias = LDG_TRUTH_FALSE;
    res = client_history_add(ctx, ctx->cmd, CLIENT_HISTORY_COMMAND);
    if (res == LDG_ERR_AOK) { res = client_history_save(ctx); }

    if (res != LDG_ERR_AOK) { return res; }

    res = client_redirect_parse(ctx, &len);
    if (res == LDG_ERR_AOK && ctx->file_path[0]) { res = ldg_io_file_open(ctx->file_path, LDG_IO_WR_ONLY | LDG_IO_CREATE | LDG_IO_TRUNCATE | LDG_IO_DIRECT, CLIENT_FILE_ACCESS, &ctx->file); }

    if (res != LDG_ERR_AOK)
    {
        ctx->command_res = res;
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "redirection failed; res: %u32; command not executed\n", res);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }

        return res;
    }

    res = client_alias_run(ctx, &len, &handled);
    close_res = client_history_alias_save(ctx);
    if (close_res != LDG_ERR_AOK) { return close_res; }

    if (res != LDG_ERR_AOK)
    {
        ctx->command_res = res;
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "alias failed; res: %u32; names: letters/digits/underscore, starting with a letter or underscore; reserved: help, quit, aliases, vars, unset; cycles are rejected\n", res);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }

        close_res = client_file_close(ctx);
        if (close_res != LDG_ERR_AOK) { return close_res; }

        return res;
    }

    if (handled)
    {
        res = client_file_close(ctx);
        return res;
    }

    res = client_var_cmd(ctx, &len, &handled);
    if (res != LDG_ERR_AOK)
    {
        ctx->command_res = res;
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "variable failed; res: %u32; command not executed; use set name=value, $name, vars, unset name; values must be literals and expanded commands must fit 255 characters\n", res);
        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }

        close_res = client_file_close(ctx);
        if (close_res != LDG_ERR_AOK) { return close_res; }

        return res;
    }

    if (handled)
    {
        res = client_file_close(ctx);
        return res;
    }

    res = ldg_str_cmp(ctx->cmd, "quit", sizeof(ctx->cmd), &cmp);
    if (res != LDG_ERR_AOK) { return res; }

    if (cmp == LDG_ARITH_ORD_EQ) { ctx->closing = LDG_TRUTH_TRUE; return client_file_close(ctx); }

    res = client_request(ctx, ctx->cmd, len, LDG_TRUTH_FALSE);
    close_res = client_file_close(ctx);
    if (res == LDG_ERR_AOK) { res = close_res; }

    return res;
}

static uint32_t client_mcp_tick(client_ctx_t *ctx, uint32_t len)
{
    char cmd[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    char draft[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t size = 0;
    uint32_t cursor = ctx->edit_cursor;
    uint32_t res = LDG_ERR_AOK;
    uint32_t result = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;
    uint8_t ready = LDG_TRUTH_FALSE;
    uint8_t searching = ctx->searching;

    if (!ctx->mcp) { return LDG_ERR_AOK; }

    res = libresteam_ext_repl_mcp_poll(ctx->mcp, cmd, sizeof(cmd), &ready);
    if (res != LDG_ERR_AOK || !ready) { return res; }

    res = ldg_str_rbrcpy(draft, ctx->cmd, sizeof(draft));
    if (res == LDG_ERR_AOK && ctx->candidate_drawn)
    {
        ctx->candidate_on = LDG_TRUTH_FALSE;
        ctx->candidate_redraw = LDG_TRUTH_TRUE;
        res = client_candidate_draw(ctx);
    }

    ctx->candidate_on = LDG_TRUTH_FALSE;
    ctx->candidate_drawn = LDG_TRUTH_FALSE;
    if (res == LDG_ERR_AOK) { res = client_edit_clear(ctx); }

    if (res == LDG_ERR_AOK) { res = client_text(ctx, "[MCP] "); }

    if (res == LDG_ERR_AOK) { res = client_text(ctx, CLIENT_PROMPT); }

    if (res == LDG_ERR_AOK) { res = client_text(ctx, cmd); }

    if (res == LDG_ERR_AOK) { res = client_text(ctx, "\r\n"); }

    if (res == LDG_ERR_AOK) { res = ldg_str_rbrcpy(ctx->cmd, cmd, sizeof(ctx->cmd)); }

    if (res == LDG_ERR_AOK) { res = ldg_str_len_get(cmd, sizeof(cmd), &size); }

    ctx->mcp_res = LDG_ERR_AOK;
    ctx->tee = LDG_TRUTH_TRUE;
    if (res == LDG_ERR_AOK) { res = client_execute(ctx, (uint32_t)size); }

    result = res == LDG_ERR_AOK ? ctx->command_res : res;
    if (res != LDG_ERR_AOK)
    {
        cleanup = ldg_str_fmt(ctx->line, sizeof(ctx->line), &size, "command transport failed; res: %u32\n", res);
        if (cleanup == LDG_ERR_AOK) { cleanup = client_wr(ctx, ctx->line, (uint32_t)size); }

        if (cleanup != LDG_ERR_AOK) { result = cleanup; }
    }

    ctx->tee = LDG_TRUTH_FALSE;
    cleanup = libresteam_ext_repl_mcp_finish(ctx->mcp, result);
    if (cleanup != LDG_ERR_AOK || ctx->mcp_res != LDG_ERR_AOK)
    {
        cleanup = client_text(ctx, "MCP caller disconnected; command was not retried.\n");
        if (res == LDG_ERR_AOK) { res = cleanup; }
    }

    cleanup = client_file_close(ctx);
    if (res == LDG_ERR_AOK) { res = cleanup; }

    cleanup = ldg_str_rbrcpy(ctx->cmd, draft, sizeof(ctx->cmd));
    if (res == LDG_ERR_AOK) { res = cleanup; }

    ctx->edit_cursor = cursor;
    ctx->searching = searching;
    ctx->history_sel = ctx->history_cunt;
    ctx->search_sel = UINT32_MAX;
    ctx->edit_drawn = LDG_TRUTH_FALSE;
    ctx->ghost_sel = UINT32_MAX;
    if (ctx->closing) { return res == LDG_ERR_AOK ? LDG_ERR_EOF : res; }

    if (res == LDG_ERR_AOK) { res = client_ghost_refresh(ctx, len); }

    if (res == LDG_ERR_AOK) { res = client_edit_draw(ctx, len); }

    return res;
}

static void client_key_get(const ldg_io_console_key_t *src, uint32_t *key, uint32_t *ch, uint8_t *ctrl, uint8_t *shift)
{
    *ctrl = (src->mods & LDG_IO_CONSOLE_MOD_CTRL) != 0 ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    *shift = (src->mods & LDG_IO_CONSOLE_MOD_SHIFT) != 0 ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    *ch = 0;
    *key = CLIENT_KEY_BASE + src->kind;
    if (src->kind != LDG_IO_CONSOLE_KEY_CHAR) { return; }

    *key = src->codepoint;
    *ch = src->codepoint;
    if (*key <= CLIENT_CTRL_BYTE_MAX)
    {
        *ctrl = LDG_TRUTH_TRUE;
        *key += CLIENT_CTRL_BYTE_OFF;
        *ch = 0;
    }
    else if (*key >= 'a' && *key <= 'z') { *key -= CLIENT_CASE_OFF; }
}

static uint32_t client_console_line_get(client_ctx_t *ctx, uint32_t *len)
{
    ldg_io_console_key_t event = LDG_STRUCT_ZERO_INIT;
    uint32_t key = 0x0;
    uint32_t ch = 0x0;
    uint32_t finish = LDG_ERR_AOK;
    uint32_t res = LDG_ERR_AOK;
    uint8_t submit = LDG_TRUTH_FALSE;
    uint8_t ctrl = LDG_TRUTH_FALSE;
    uint8_t shift = LDG_TRUTH_FALSE;
    uint8_t picked = LDG_TRUTH_FALSE;

    *len = 0;
    ctx->cmd[0] = LDG_STR_TERM;
    ctx->candidate_on = LDG_TRUTH_FALSE;
    ctx->candidate_drawn = LDG_TRUTH_FALSE;
    ctx->searching = LDG_TRUTH_FALSE;
    ctx->edit_cursor = 0;
    ctx->history_sel = ctx->history_cunt;
    ctx->edit_drawn = LDG_TRUTH_FALSE;
    ctx->ghost_sel = UINT32_MAX;
    ctx->ghost_hidden = LDG_TRUTH_FALSE;
    res = client_edit_draw(ctx, *len);
    while (res == LDG_ERR_AOK)
    {
        res = client_mcp_tick(ctx, *len);
        if (res != LDG_ERR_AOK) { return res; }

        if (ctx->mcp && ldg_io_console_rd_ready_is() != LDG_TRUTH_TRUE)
        {
            res = ldg_thread_yield((uint64_t)LIBRESTEAM_EXT_REPL_MCP_POLL_MS * LDG_NS_PER_MS);
            continue;
        }

        res = ldg_io_console_key_rd(&event);
        if (res == LDG_ERR_EOF) { return LDG_ERR_EOF; }

        if (res != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (!event.down) { continue; }

        client_key_get(&event, &key, &ch, &ctrl, &shift);
        if (key == CLIENT_KEY_TAB && !ctx->searching)
        {
            res = client_candidate_tab(ctx, len, ctx->edit_cursor, shift);
            if (res == LDG_ERR_AOK) { ctx->edit_cursor = ctx->candidate_cursor; res = client_candidate_draw(ctx); }
        }
        else
        {
            picked = ctx->candidate_on && ctx->candidate_sel != UINT32_MAX;
            if (ctx->candidate_drawn)
            {
                ctx->candidate_on = LDG_TRUTH_FALSE;
                ctx->candidate_redraw = LDG_TRUTH_TRUE;
                res = client_candidate_draw(ctx);
                ctx->candidate_drawn = LDG_TRUTH_FALSE;
            }

            ctx->candidate_on = LDG_TRUTH_FALSE;
            if (res == LDG_ERR_AOK && ctrl && key == 'L') { ctx->edit_drawn = LDG_TRUTH_FALSE; res = client_text(ctx, CLIENT_SCREEN_CLEAR); }
            else if (res == LDG_ERR_AOK && !(picked && key == CLIENT_KEY_ENTER)) { res = client_edit_key(ctx, len, key, ch, ctrl, shift, &submit); }
        }

        if (res == LDG_ERR_EOF)
        {
            finish = client_edit_finish(ctx, *len);
            return finish == LDG_ERR_AOK ? res : finish;
        }

        if (res == LDG_ERR_FULL || res == LDG_ERR_STR_TRUNC || res == LDG_ERR_STR_INVALID) { res = client_text(ctx, "\a"); }

        if (res != LDG_ERR_AOK) { return res; }

        if (submit) { return client_edit_finish(ctx, *len); }

        res = client_ghost_refresh(ctx, *len);
        if (res == LDG_ERR_AOK) { res = client_edit_draw(ctx, *len); }
    }
    return res;
}

static uint32_t client_line_get(client_ctx_t *ctx, uint32_t *len)
{
    char ch = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t invalid = LDG_TRUTH_FALSE;

    if (ctx->interactive) { return client_console_line_get(ctx, len); }

    *len = 0;
    for (;;)
    {
        res = ldg_io_console_byte_rd((uint8_t *)&ch);
        if (res == LDG_ERR_EOF) { return LDG_ERR_EOF; }

        if (res != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (ch == '\n') { break; }

        if (ch == '\r') { continue; }

        if (*len + LDG_STR_TERM_SIZE >= sizeof(ctx->cmd) || (uint8_t)ch > LDG_ASCII_PRINTABLE_MAX || ((uint8_t)ch < LDG_ASCII_PRINTABLE_MIN && ch != '\t'))
        {
            invalid = LDG_TRUTH_TRUE;
            continue;
        }

        ctx->cmd[(*len)++] = ch;
    }
    ctx->cmd[*len] = LDG_STR_TERM;
    return invalid ? LDG_ERR_STR_INVALID : LDG_ERR_AOK;
}

static uint32_t client_loop(client_ctx_t *ctx)
{
    uint32_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = client_text(ctx, "Connected to LibreSteam REPL; commands execute on the host's selected callback thread; status reports its ID\n"
        "Help: help lists backends; help native lists engine-independent tools; quit or Ctrl-D disconnects; Ctrl-C clears the line\n"
        "History: 1000 entries per target executable; Up/Down or PgUp/PgDn recalls; Ctrl-R searches older, Ctrl-Shift-R newer; Right/End accepts a dimmed suggestion, Ctrl-Right accepts one word\n"
        "Editing: Ctrl-W deletes a word; Ctrl-Left/Right moves by word; Ctrl-L clears the screen\n"
        "Aliases and variables: name=command for aliases; set name=value and $name for variables\n"
        "Completion: Tab completes; Tab/Shift+Tab cycles matches; Enter accepts a selected match, then Enter executes; Esc closes suggestions\n"
        "Output: append > file.txt to redirect; quote paths containing spaces; parenthesize Lua > comparisons\n");
    while (res == LDG_ERR_AOK)
    {
        res = ctx->interactive ? LDG_ERR_AOK : client_text(ctx, ctx->ansi ? CLIENT_PROMPT_COLOR : CLIENT_PROMPT);
        if (res != LDG_ERR_AOK) { return res; }

        res = client_line_get(ctx, &len);
        if (res == LDG_ERR_EOF) { return LDG_ERR_AOK; }

        if (res == LDG_ERR_STR_INVALID)
        {
            res = client_text(ctx, "Use ASCII commands of at most 255 characters.\n");
            continue;
        }

        if (res != LDG_ERR_AOK) { return res; }

        if (!len) { continue; }

        res = client_execute(ctx, len);
        if (ctx->closing) { return res; }
    }
    return res;
}

static uint32_t client_hello_rd(client_ctx_t *ctx)
{
    const char magic[] = LIBRESTEAM_EXT_REPL_HELLO_MAGIC;
    uint64_t bytes = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (idx = 0; idx < CLIENT_HELLO_TRIES && !bytes; idx++)
    {
        res = ldg_io_ipc_rd_timed(&ctx->pipe, (ldg_byte_t *)&ctx->resp, sizeof(ctx->resp), CLIENT_HELLO_POLL_MS, &bytes);
        if (res == LDG_ERR_TIMEOUT) { continue; }

        if (res != LDG_ERR_AOK) { return res; }

        if (!bytes) { res = ldg_thread_yield((uint64_t)CLIENT_HELLO_POLL_MS * LDG_NS_PER_MS); }

        if (res != LDG_ERR_AOK) { return res; }
    }
    if (!bytes) { return LDG_ERR_TIMEOUT; }

    ctx->stage = "hello length";
    if (bytes < sizeof(ctx->hello)) { return LDG_ERR_INVALID; }

    res = ldg_mem_secure_cpy(&ctx->hello, &ctx->resp, sizeof(ctx->hello));
    if (res != LDG_ERR_AOK) { return res; }

    ctx->stage = "hello magic";
    for (idx = 0; idx < sizeof(ctx->hello.magic); idx++) { if (ctx->hello.magic[idx] != magic[idx]) { return LDG_ERR_INVALID; } }
    ctx->stage = "hello version";
    if (ctx->hello.version != LIBRESTEAM_EXT_REPL_PROTO_VERSION) { return LDG_ERR_UNSUPPORTED; }

    ctx->stage = "hello size field";
    return ctx->hello.size == bytes ? LDG_ERR_AOK : LDG_ERR_INVALID;
}

static uint32_t client_run(client_ctx_t *ctx, const char *pid)
{
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;
    uint64_t text_len = 0;

    ctx->stage = "endpoint name";
    res = client_path_get(ctx, pid);
    if (res != LDG_ERR_AOK) { return res; }

    ctx->stage = "connect";
    res = ldg_io_ipc_connect(ctx->path, &ctx->pipe);
    if (res != LDG_ERR_AOK) { return res; }

    ctx->stage = "hello";
    res = client_hello_rd(ctx);

    if (res == LDG_ERR_AOK && ldg_sys_tty_stdout_is() == LDG_TRUTH_TRUE)
    {
        ctx->stage = "terminal mode";
        if (ldg_sys_tty_vt_en_set(LDG_TRUTH_TRUE) != LDG_ERR_AOK) { res = LDG_ERR_UNSUPPORTED; }
        else { ctx->ansi = LDG_TRUTH_TRUE; }
    }

    if (res == LDG_ERR_AOK && ctx->ansi && ldg_io_console_raw_set(LDG_TRUTH_TRUE) == LDG_ERR_AOK) { ctx->interactive = LDG_TRUTH_TRUE; }

    if (res == LDG_ERR_AOK)
    {
        ctx->stage = "history paths";
        res = client_history_paths_get(ctx);
    }

    if (res == LDG_ERR_AOK)
    {
        ctx->stage = "history file";
        res = client_history_load(ctx);
    }

    if (res == LDG_ERR_AOK)
    {
        ctx->stage = "session";
        ctx->history_up = LDG_TRUTH_TRUE;
        res = client_text(ctx, "History file: ");
        if (res == LDG_ERR_AOK) { res = client_text(ctx, ctx->history_path); }

        if (res == LDG_ERR_AOK) { res = client_text(ctx, "\n"); }
    }

    if (res == LDG_ERR_AOK && ctx->interactive)
    {
        close_res = libresteam_ext_repl_mcp_init(&ctx->mcp, ctx->mcp_port);
        if (close_res == LDG_ERR_AOK) { res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "MCP: http://127.0.0.1:%u32/mcp; tool: repl_execute; commands and output are visible here\n", (uint32_t)ctx->mcp_port); }
        else { res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &text_len, "MCP listener unavailable; port: %u32; res: %u32; prompt remains available; use --mcp-port PORT for another listener\n", (uint32_t)ctx->mcp_port, close_res); }

        if (res == LDG_ERR_AOK) { res = client_wr(ctx, ctx->line, (uint32_t)text_len); }
    }

    if (res == LDG_ERR_AOK) { res = client_loop(ctx); }

    if (ctx->mcp)
    {
        close_res = libresteam_ext_repl_mcp_stop(ctx->mcp);
        ctx->mcp = 0x0;
        if (res == LDG_ERR_AOK) { res = close_res; }
    }

    if (ctx->interactive && ldg_io_console_raw_set(LDG_TRUTH_FALSE) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    if (ctx->ansi && ldg_sys_tty_vt_en_set(LDG_TRUTH_FALSE) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    ctx->interactive = LDG_TRUTH_FALSE;
    ctx->ansi = LDG_TRUTH_FALSE;
    close_res = client_file_close(ctx);
    if (res == LDG_ERR_AOK) { res = close_res; }

    if (ctx->history_lock)
    {
        if (ldg_io_file_unlock(ctx->history_lock) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

        if (ldg_io_file_close(ctx->history_lock) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

        ctx->history_lock = 0x0;
    }

    if (ldg_io_ipc_close(&ctx->pipe) != LDG_ERR_AOK) { return LDG_ERR_IO_CLOSE; }

    return res;
}

int main(int argc, char **argv)
{
    client_ctx_t *ctx = 0x0;
    void *mem = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint64_t port = LIBRESTEAM_EXT_REPL_MCP_PORT;
    uint32_t idx = 0;
    const char *pid = 0x0;
    uint8_t port_set = LDG_TRUTH_FALSE;

    if (argc < 1 || argc > 4) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 1; idx < (uint32_t)argc; idx++)
    {
        if (client_eq_is(argv[idx], "--mcp-port") == LDG_TRUTH_TRUE)
        {
            if (port_set || ++idx >= (uint32_t)argc) { return LDG_ERR_FUNC_ARG_INVALID; }

            res = ldg_str_len_get(argv[idx], CLIENT_PID_DIGITS_MAX, &len);
            if (res == LDG_ERR_AOK) { res = ldg_str_uint_parse(argv[idx], LDG_BASE_DEC, (uint32_t)len, &port); }

            if (res != LDG_ERR_AOK || !port || port > UINT16_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

            port_set = LDG_TRUTH_TRUE;
        }
        else if (!pid) { pid = argv[idx]; }
        else { return LDG_ERR_FUNC_ARG_INVALID; }
    }

    res = ldg_mem_init();
    if (res == LDG_ERR_AOK) { res = ldg_mem_alloc(sizeof(*ctx), &mem); }

    if (res != LDG_ERR_AOK) { return (int)res; }

    ctx = (client_ctx_t *)mem;
    res = ldg_mem_secure_zero(ctx, sizeof(*ctx));

    ctx->mcp_port = (uint16_t)port;
    if (res == LDG_ERR_AOK) { res = client_run(ctx, pid); }

    if (res != LDG_ERR_AOK)
    {
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &len, "REPL disconnected or unavailable; res: %u32; stage: %s. Start the game with repl.dangext in extensions, then retry.\n", res, ctx->stage ? ctx->stage : "startup");
        if (res != LDG_ERR_AOK) { return res; }

        res = client_wr(ctx, ctx->line, (uint32_t)len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = ldg_mem_dealloc(ctx);
    return res;
}
