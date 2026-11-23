#include <stdarg.h>
#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/repl.h>

#define CMD_ESCAPE_SIZE 4U
#define CMD_ARGC_SINGLE 1U
#define CMD_ARGC_PAIR 2U
#define CMD_ARG_IDX 1U

uint32_t libresteam_ext_repl_output_wr(libresteam_ext_repl_ctx_t *ctx, const char *text, uint64_t len)
{
    uint64_t part = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !text)) { return LDG_ERR_FUNC_ARG_NULL; }

    while (len)
    {
        if (LDG_UNLIKELY(ctx->resp.len >= sizeof(ctx->resp.text))) { return LDG_ERR_BOUNDS; }

        part = sizeof(ctx->resp.text) - LDG_STR_TERM_SIZE - ctx->resp.len;
        if (!part)
        {
            if (LDG_UNLIKELY(!ctx->flush)) { return LDG_ERR_FULL; }

            res = ctx->flush(ctx);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            continue;
        }

        if (part > len) { part = len; }

        res = ldg_mem_secure_cpy(ctx->resp.text + ctx->resp.len, text, part);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        ctx->resp.len += (uint32_t)part;
        ctx->resp.text[ctx->resp.len] = LDG_STR_TERM;
        text += part;
        len -= part;
    }
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_emit(libresteam_ext_repl_ctx_t *ctx, const char *fmt, ...)
{
    va_list args;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !fmt)) { return LDG_ERR_FUNC_ARG_NULL; }

    va_start(args, fmt);
    res = ldg_str_var_fmt(ctx->line, sizeof(ctx->line), &len, fmt, args);
    va_end(args);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (LDG_UNLIKELY(ctx->resp.len >= sizeof(ctx->resp.text))) { return LDG_ERR_BOUNDS; }

    if (len + LDG_STR_TERM_SIZE >= sizeof(ctx->resp.text) - ctx->resp.len && ctx->flush)
    {
        res = ctx->flush(ctx);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }

    res = libresteam_ext_repl_output_wr(ctx, ctx->line, len);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    return libresteam_ext_repl_output_wr(ctx, "\n", 1U);
}

uint32_t libresteam_ext_repl_name_get(const char *src, char *out, uint32_t cap)
{
    char hex[3] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0x0;
    uint32_t dst = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out || cap == 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_STR_TERM;
    if (!src) { return ldg_str_rbrcpy(out, "-", cap); }

    for (idx = 0; src[idx] != LDG_STR_TERM; idx++)
    {
        if ((uint8_t)src[idx] >= LDG_ASCII_PRINTABLE_MIN && (uint8_t)src[idx] <= LDG_ASCII_PRINTABLE_MAX)
        {
            if (dst + LDG_STR_TERM_SIZE >= cap) { break; }

            out[dst++] = src[idx];
        }
        else
        {
            if (dst + CMD_ESCAPE_SIZE >= cap) { break; }

            res = ldg_byte_to_hex((uint8_t)src[idx], hex);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            out[dst++] = '\\';
            out[dst++] = 'x';
            out[dst++] = hex[0];
            out[dst++] = hex[1];
        }
    }
    out[dst] = LDG_STR_TERM;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_num_get(const char *text, uint64_t *out)
{
    uint32_t base = LDG_BASE_DEC;
    uint32_t idx = 0x0;

    if (LDG_UNLIKELY(!text || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;
    if (text[0] == '0' && (text[1] == 'x' || text[1] == 'X')) { base = LDG_BASE_HEX; text += 2; }

    if (*text == LDG_STR_TERM) { return LDG_ERR_STR_INVALID; }

    for (idx = 0; text[idx] != LDG_STR_TERM; idx++)
    {
        if (base == LDG_BASE_DEC && ldg_char_digit_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }

        if (base == LDG_BASE_HEX && ldg_char_hex_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }
    }
    return ldg_str_uint_parse(text, base, idx, out);
}

uint8_t libresteam_ext_repl_cmd_eq_is(const char *lhs, const char *rhs)
{
    uint32_t cmp = 0x0;

    if (ldg_str_cmp(lhs, rhs, LIBRESTEAM_EXT_REPL_CMD_CAP, &cmp) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return cmp == LDG_ARITH_ORD_EQ ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t cmd_args_get(libresteam_ext_repl_ctx_t *ctx, char **args, uint32_t *cunt)
{
    uint64_t cursor = 0x0;
    uint64_t start = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t idx = UINT32_MAX;

    *cunt = 0;
    for (idx = 0; idx < sizeof(ctx->req.text); idx++)
    {
        if (ctx->req.text[idx] == LDG_STR_TERM) { break; }

        if ((uint8_t)ctx->req.text[idx] > LDG_ASCII_PRINTABLE_MAX || ((uint8_t)ctx->req.text[idx] < LDG_ASCII_PRINTABLE_MIN && ctx->req.text[idx] != '\t')) { return LDG_ERR_STR_INVALID; }
    }
    if (idx == sizeof(ctx->req.text)) { return LDG_ERR_STR_TRUNC; }

    while ((res = ldg_str_tok_get(ctx->req.text, " \t", &cursor, sizeof(ctx->req.text), &start, &len)) == LDG_ERR_AOK)
    {
        if (*cunt == LIBRESTEAM_EXT_REPL_ARG_CAP) { return LDG_ERR_FULL; }

        args[(*cunt)++] = ctx->req.text + start;
        if (ctx->req.text[start + len] != LDG_STR_TERM)
        {
            ctx->req.text[start + len] = LDG_STR_TERM;
            cursor++;
        }

        if ((*cunt == 1U && (libresteam_ext_repl_cmd_eq_is(args[0], "eval") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "complete") == LDG_TRUTH_TRUE)) || (*cunt == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "lua") == LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[1], "eval") == LDG_TRUTH_TRUE))
        {
            while (cursor < sizeof(ctx->req.text) && (ctx->req.text[cursor] == ' ' || ctx->req.text[cursor] == '\t')) { cursor++; }
            if (cursor < sizeof(ctx->req.text) && ctx->req.text[cursor] != LDG_STR_TERM) { args[(*cunt)++] = ctx->req.text + cursor; }

            return LDG_ERR_AOK;
        }
    }
    return res == LDG_ERR_EOF ? LDG_ERR_AOK : res;
}

static uint32_t cmd_backend_find(libresteam_ext_repl_ctx_t *ctx, const char *name, uint32_t *out)
{
    uint32_t idx = UINT32_MAX;

    *out = UINT32_MAX;
    for (idx = 0; idx < ctx->backend_cunt; idx++)
    {
        if (libresteam_ext_repl_cmd_eq_is(name, ctx->backend[idx].name) != LDG_TRUTH_TRUE) { continue; }

        *out = idx;
        return LDG_ERR_AOK;
    }
    return LDG_ERR_NOT_FOUND;
}

static uint8_t cmd_backend_owns_is(const libresteam_ext_repl_backend_t *backend, const char *name)
{
    uint32_t idx = UINT32_MAX;

    for (idx = 0; idx < LIBRESTEAM_EXT_REPL_BACKEND_CMD_CAP && backend->commands[idx]; idx++) { if (libresteam_ext_repl_cmd_eq_is(name, backend->commands[idx]) == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; } }
    return LDG_TRUTH_FALSE;
}

static uint32_t cmd_ext_emit_cb(void *sink, const char *text)
{
    if (LDG_UNLIKELY(!sink || !text)) { return LDG_ERR_FUNC_ARG_NULL; }

    return libresteam_ext_repl_emit((libresteam_ext_repl_ctx_t *)sink, "%s", text);
}

static uint32_t cmd_ext_find(libresteam_ext_repl_ctx_t *ctx, const char *name, libresteam_mod_cmd_t *out)
{
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!ctx->api->cmd_get) { return LDG_ERR_NOT_FOUND; }

    for (idx = 0; (res = ctx->api->cmd_get(ctx->mod, idx, out)) == LDG_ERR_AOK; idx++) { if (libresteam_ext_repl_cmd_eq_is(name, out->name) == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; } }
    return res;
}

static uint32_t cmd_backends(libresteam_ext_repl_ctx_t *ctx)
{
    libresteam_mod_cmd_t ext = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_emit(ctx, "selected backend: %s; execution thread: %u32", ctx->selected_backend == UINT32_MAX ? "auto" : ctx->backend[ctx->selected_backend].name, ctx->main_thread);
    for (idx = 0; res == LDG_ERR_AOK && idx < ctx->backend_cunt; idx++) { res = libresteam_ext_repl_emit(ctx, "backend: %s; tools: %s; tick res: %u32", ctx->backend[idx].name, ctx->backend[idx].desc, ctx->backend[idx].last_res); }
    for (idx = 0; res == LDG_ERR_AOK && ctx->api->cmd_get && ctx->api->cmd_get(ctx->mod, idx, &ext) == LDG_ERR_AOK; idx++) { res = libresteam_ext_repl_emit(ctx, "backend: %s; tools: %s; provided by an extension", ext.name, ext.desc); }
    return res;
}

static uint32_t cmd_backend_run(libresteam_ext_repl_ctx_t *ctx, uint32_t idx, uint32_t argc, char **args)
{
    uint32_t res = LDG_ERR_AOK;

    if (idx >= ctx->backend_cunt || !ctx->backend[idx].cmd || !argc) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = ctx->backend[idx].cmd(ctx, argc, args);
    if (ctx->searching)
    {
        if (res != LDG_ERR_AOK || !ctx->backend[idx].cont) { ctx->searching = LDG_TRUTH_FALSE; return res == LDG_ERR_AOK ? LDG_ERR_UNSUPPORTED : res; }

        ctx->pending_backend = idx;
    }

    return res;
}

uint32_t libresteam_ext_repl_complete_begin(libresteam_ext_repl_ctx_t *ctx, const char *prefix, uint8_t space)
{
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !prefix)) { return LDG_ERR_FUNC_ARG_NULL; }

    ctx->complete_cunt = 0;
    ctx->complete_more = 0;
    res = ldg_str_rbrcpy(ctx->complete_prefix, prefix, sizeof(ctx->complete_prefix));
    if (res == LDG_ERR_AOK) { res = ldg_str_len_get(prefix, sizeof(ctx->complete_prefix), &len); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "complete: %u64 %u32", len, (uint32_t)space) : res;
}

uint32_t libresteam_ext_repl_complete_add(libresteam_ext_repl_ctx_t *ctx, const char *value, const char *label)
{
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !value || !label)) { return LDG_ERR_FUNC_ARG_NULL; }

    while (ctx->complete_prefix[idx])
    {
        if (value[idx] != ctx->complete_prefix[idx]) { return LDG_ERR_AOK; }

        idx++;
    }
    for (idx = 0; value[idx]; idx++) { if (idx + LDG_STR_TERM_SIZE >= LIBRESTEAM_EXT_REPL_CMD_CAP || (uint8_t)value[idx] < LDG_ASCII_PRINTABLE_MIN || (uint8_t)value[idx] > LDG_ASCII_PRINTABLE_MAX) { return LDG_ERR_AOK; } }
    if (!idx) { return LDG_ERR_AOK; }

    if (ctx->complete_cunt == LIBRESTEAM_EXT_REPL_COMPLETE_CAP)
    {
        if (!ctx->complete_more) { res = libresteam_ext_repl_emit(ctx, "!"); ctx->complete_more = 1; }

        return res;
    }

    res = libresteam_ext_repl_name_get(label, ctx->detail, sizeof(ctx->detail));
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; ctx->detail[idx]; idx++) { if (ctx->detail[idx] == '\t') { ctx->detail[idx] = ' '; } }
    res = libresteam_ext_repl_emit(ctx, "%s\t%s", value, ctx->detail);
    if (res == LDG_ERR_AOK) { ctx->complete_cunt++; }

    return res;
}

static uint32_t cmd_complete_words(libresteam_ext_repl_ctx_t *ctx, const char *words)
{
    char word[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t pos = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    while (words[pos])
    {
        idx = 0;
        while (words[pos] && words[pos] != ' ')
        {
            if (idx + LDG_STR_TERM_SIZE >= sizeof(word)) { return LDG_ERR_FULL; }

            word[idx++] = words[pos++];
        }
        word[idx] = LDG_STR_TERM;
        if (words[pos]) { pos++; }

        res = libresteam_ext_repl_complete_add(ctx, word, "");
        if (res != LDG_ERR_AOK) { return res; }
    }
    return res;
}

static uint32_t cmd_complete_native(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    const char *types = "u8 u16 u32 u64 i8 i16 i32 i64 ptr f32 f64";
    const char *cmd = args[0];

    if (argc == 2U)
    {
        if (libresteam_ext_repl_cmd_eq_is(cmd, "trace") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, "help status threads read clear off capture exe"); }

        if (libresteam_ext_repl_cmd_eq_is(cmd, "scan") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, "help begin status where watch fit list best rebase stop clear"); }

        if (libresteam_ext_repl_cmd_eq_is(cmd, "buff") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, "status reset"); }

        if (libresteam_ext_repl_cmd_eq_is(cmd, "read") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(cmd, "write") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, types); }
    }

    if (argc == 3U && libresteam_ext_repl_cmd_eq_is(cmd, "invoke") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, "void u8 u16 u32 u64 i8 i16 i32 i64 ptr f32 f64"); }

    if (argc == 3U && libresteam_ext_repl_cmd_eq_is(cmd, "scan") == LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[1], "begin") == LDG_TRUTH_TRUE) { return cmd_complete_words(ctx, types); }

    return LDG_ERR_AOK;
}

static uint32_t cmd_complete(libresteam_ext_repl_ctx_t *ctx, char *text)
{
    char *args[LIBRESTEAM_EXT_REPL_ARG_CAP] = LDG_ARR_ZERO_INIT;
    libresteam_mod_cmd_t ext = LDG_STRUCT_ZERO_INIT;
    char *cursor = text;
    uint32_t argc = 0x0;
    uint32_t target = UINT32_MAX;
    uint32_t idx = 0x0;
    uint32_t cmd = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t explicit_backend = LDG_TRUTH_FALSE;

    for (;;)
    {
        while (*cursor == ' ' || *cursor == '\t') { cursor++; }
        if (argc == LIBRESTEAM_EXT_REPL_ARG_CAP) { return libresteam_ext_repl_complete_begin(ctx, "", LDG_TRUTH_FALSE); }

        args[argc++] = cursor;
        while (*cursor && *cursor != ' ' && *cursor != '\t') { cursor++; }
        if (!*cursor) { break; }

        *cursor++ = LDG_STR_TERM;
        if ((argc == 1U && libresteam_ext_repl_cmd_eq_is(args[0], "eval") == LDG_TRUTH_TRUE) || (argc == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "lua") == LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[1], "eval") == LDG_TRUTH_TRUE))
        {
            args[argc++] = cursor;
            break;
        }
    }
    if (argc == CMD_ARGC_PAIR && cmd_ext_find(ctx, args[0], &ext) == LDG_ERR_AOK)
    {
        res = libresteam_ext_repl_complete_begin(ctx, args[CMD_ARG_IDX], LDG_TRUTH_TRUE);
        return res == LDG_ERR_AOK ? cmd_complete_words(ctx, ext.words) : res;
    }

    if (argc > 1U && cmd_backend_find(ctx, args[0], &target) == LDG_ERR_AOK)
    {
        explicit_backend = LDG_TRUTH_TRUE;
        argc--;
        for (idx = 0; idx < argc; idx++) { args[idx] = args[idx + 1U]; }
    }

    if (argc > 1U && !explicit_backend) { for (idx = 0; idx < ctx->backend_cunt; idx++)
        {
            if (idx != LIBRESTEAM_EXT_REPL_NATIVE_IDX && ctx->selected_backend != UINT32_MAX && ctx->selected_backend != idx) { continue; }

            if (cmd_backend_owns_is(&ctx->backend[idx], args[0]) == LDG_TRUTH_TRUE) { target = idx; break; }
        }
    }

    if (argc > 1U && target < ctx->backend_cunt && ctx->backend[target].complete) { return ctx->backend[target].complete(ctx, argc, args); }

    res = libresteam_ext_repl_complete_begin(ctx, args[argc - 1U], LDG_TRUTH_TRUE);
    if (res != LDG_ERR_AOK) { return res; }

    if (argc == 1U)
    {
        res = cmd_complete_words(ctx, explicit_backend ? "help status" : "help status backends backend");
        for (idx = 0; res == LDG_ERR_AOK && !explicit_backend && ctx->api->cmd_get && ctx->api->cmd_get(ctx->mod, idx, &ext) == LDG_ERR_AOK; idx++) { res = libresteam_ext_repl_complete_add(ctx, ext.name, ext.desc); }
        for (idx = 0; res == LDG_ERR_AOK && idx < ctx->backend_cunt; idx++)
        {
            if (!explicit_backend) { res = libresteam_ext_repl_complete_add(ctx, ctx->backend[idx].name, ctx->backend[idx].desc); }

            if (res != LDG_ERR_AOK) { break; }

            if (explicit_backend && idx != target) { continue; }

            if (!explicit_backend && idx != LIBRESTEAM_EXT_REPL_NATIVE_IDX && ctx->selected_backend != UINT32_MAX && ctx->selected_backend != idx) { continue; }

            for (cmd = 0; res == LDG_ERR_AOK && cmd < LIBRESTEAM_EXT_REPL_BACKEND_CMD_CAP && ctx->backend[idx].commands[cmd]; cmd++) { res = libresteam_ext_repl_complete_add(ctx, ctx->backend[idx].commands[cmd], ctx->backend[idx].name); }
        }
        return res;
    }

    if (argc == 2U && (libresteam_ext_repl_cmd_eq_is(args[0], "backend") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE))
    {
        if (libresteam_ext_repl_cmd_eq_is(args[0], "backend") == LDG_TRUTH_TRUE) { res = libresteam_ext_repl_complete_add(ctx, "auto", ""); }

        for (idx = 0; res == LDG_ERR_AOK && idx < ctx->backend_cunt; idx++) { res = libresteam_ext_repl_complete_add(ctx, ctx->backend[idx].name, ctx->backend[idx].desc); }
        return res;
    }

    return target == LIBRESTEAM_EXT_REPL_NATIVE_IDX ? cmd_complete_native(ctx, argc, args) : LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_cmd_run(libresteam_ext_repl_ctx_t *ctx)
{
    char *args[LIBRESTEAM_EXT_REPL_ARG_CAP] = LDG_ARR_ZERO_INIT;
    libresteam_mod_cmd_t ext = LDG_STRUCT_ZERO_INIT;
    char status[] = "status";
    char help[] = "help";
    char *single[1] = LDG_ARR_ZERO_INIT;
    uint32_t argc = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t target = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = cmd_args_get(ctx, args, &argc);
    if (res != LDG_ERR_AOK || argc == 0) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "complete") == LDG_TRUTH_TRUE) { return cmd_complete(ctx, argc == 2U ? args[1] : ctx->req.text + sizeof("complete") - LDG_STR_TERM_SIZE); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "backends") == LDG_TRUTH_TRUE) { return argc == 1U ? cmd_backends(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "backend") == LDG_TRUTH_TRUE)
    {
        if (argc == 1U) { return cmd_backends(ctx); }

        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (libresteam_ext_repl_cmd_eq_is(args[1], "auto") != LDG_TRUTH_TRUE) { res = cmd_backend_find(ctx, args[1], &target); }

        if (res != LDG_ERR_AOK) { return res; }

        ctx->selected_backend = target;
        return cmd_backends(ctx);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE)
    {
        single[0] = help;
        if (argc == CMD_ARGC_PAIR && cmd_ext_find(ctx, args[CMD_ARG_IDX], &ext) == LDG_ERR_AOK) { return ext.cb(ext.user, CMD_ARGC_SINGLE, single, cmd_ext_emit_cb, ctx); }

        if (argc == 2U)
        {
            res = cmd_backend_find(ctx, args[1], &target);
            single[0] = help;
            return res == LDG_ERR_AOK ? cmd_backend_run(ctx, target, 1U, single) : res;
        }

        if (argc != 1U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_emit(ctx, "LibreSteam REPL: help [backend] | status | backends | backend <auto|name>");
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Explicit routing: native <command...> | il2cpp <command...> | lua <command...> | java <command...>. Existing unprefixed commands work in auto mode."); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Native tools are always routed independently of the selected engine. Aliases: name=command, then name. quit disconnects."); }

        if (res == LDG_ERR_AOK) { res = cmd_backends(ctx); }

        return res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "status") == LDG_TRUTH_TRUE)
    {
        if (argc != 1U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_emit(ctx, "dispatch thread: %u32", ctx->main_thread);
        if (res == LDG_ERR_AOK) { res = cmd_backends(ctx); }

        single[0] = status;
        for (idx = 0; res == LDG_ERR_AOK && idx < ctx->backend_cunt; idx++) { res = cmd_backend_run(ctx, idx, 1U, single); }
        return res;
    }

    res = cmd_backend_find(ctx, args[0], &target);
    if (res == LDG_ERR_AOK) { return argc > 1U ? cmd_backend_run(ctx, target, argc - 1U, args + 1) : LDG_ERR_FUNC_ARG_INVALID; }

    if (res != LDG_ERR_NOT_FOUND) { return res; }

    if (cmd_ext_find(ctx, args[0], &ext) == LDG_ERR_AOK) { return argc > CMD_ARGC_SINGLE ? ext.cb(ext.user, argc - CMD_ARGC_SINGLE, args + CMD_ARGC_SINGLE, cmd_ext_emit_cb, ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; idx < ctx->backend_cunt; idx++)
    {
        if (idx != LIBRESTEAM_EXT_REPL_NATIVE_IDX && ctx->selected_backend != UINT32_MAX && idx != ctx->selected_backend) { continue; }

        if (cmd_backend_owns_is(&ctx->backend[idx], args[0]) == LDG_TRUTH_TRUE) { return cmd_backend_run(ctx, idx, argc, args); }
    }
    res = libresteam_ext_repl_emit(ctx, "Unknown command for the selected backend; use help native, help il2cpp, help lua, or help java.");
    return res == LDG_ERR_AOK ? LDG_ERR_NOT_FOUND : res;
}
