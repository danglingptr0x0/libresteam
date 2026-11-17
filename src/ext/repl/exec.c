#include <float.h>
#include <stdint.h>

#include <dangling/arch/amd64/call.h>
#include <dangling/core/err.h>
#include <dangling/core/bits.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/region.h>
#include <dangling/mem/secure.h>
#include <dangling/mem/vmem.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/exec.h>
#include <libresteam/ext/repl/expr.h>

#define EXEC_BUFF_CAP (64U * LDG_KIB)
#define EXEC_WR_CAP 128U
#define EXEC_TYPE_CAP 8U
#define EXEC_INVOKE_ARG_BASE 3U
#define EXEC_TYPE_SEP_SIZE 1U

typedef struct exec_type
{
    uint32_t bytes;
    uint8_t fp;
    uint8_t signed_val;
    uint8_t ptr;
    uint8_t pudding;
} exec_type_t;

typedef struct exec_ctx
{
    ldg_mem_pool_t *pool;
    libresteam_ext_repl_expr_t expr;
    ldg_call_frame_t frame;
    uint8_t bytes[EXEC_WR_CAP];
    uint8_t verify[EXEC_WR_CAP];
    uint8_t busy;
} exec_ctx_t;

uint32_t libresteam_ext_repl_exec_init(libresteam_ext_repl_ctx_t *ctx)
{
    exec_ctx_t *exec = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(ctx->exec)) { return LDG_ERR_EXISTS; }

    res = ldg_mem_alloc(sizeof(*exec), &ctx->exec);
    if (res != LDG_ERR_AOK) { return res; }

    exec = (exec_ctx_t *)ctx->exec;
    res = ldg_mem_secure_zero(exec, sizeof(*exec));
    if (res == LDG_ERR_AOK) { res = ldg_mem_pool_create(0, EXEC_BUFF_CAP, &exec->pool); }

    return res;
}

uint32_t libresteam_ext_repl_exec_shutdown(libresteam_ext_repl_ctx_t *ctx)
{
    exec_ctx_t *exec = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->exec) { return LDG_ERR_AOK; }

    exec = (exec_ctx_t *)ctx->exec;
    if (exec->pool) { res = ldg_mem_pool_destroy(&exec->pool); }

    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_dealloc(ctx->exec);
    if (res == LDG_ERR_AOK) { ctx->exec = 0x0; }

    return res;
}

static uint32_t exec_type_get(const char *text, uint8_t ret, exec_type_t *out)
{
    uint32_t res = ldg_mem_secure_zero(out, sizeof(*out));

    if (res != LDG_ERR_AOK) { return res; }

    if (ret && libresteam_ext_repl_cmd_eq_is(text, "void") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    if (libresteam_ext_repl_cmd_eq_is(text, "ptr") == LDG_TRUTH_TRUE)
    {
        out->bytes = sizeof(uintptr_t);
        out->ptr = LDG_TRUTH_TRUE;
        return LDG_ERR_AOK;
    }

    if (text[0] != 'u' && text[0] != 'i' && text[0] != 'f') { return LDG_ERR_FUNC_ARG_INVALID; }

    out->fp = text[0] == 'f';
    out->signed_val = text[0] == 'i';
    if (libresteam_ext_repl_cmd_eq_is(text + 1U, "8") == LDG_TRUTH_TRUE && !out->fp) { out->bytes = sizeof(uint8_t); }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1U, "16") == LDG_TRUTH_TRUE && !out->fp) { out->bytes = sizeof(uint16_t); }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1U, "32") == LDG_TRUTH_TRUE) { out->bytes = sizeof(uint32_t); }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1U, "64") == LDG_TRUTH_TRUE) { out->bytes = sizeof(uint64_t); }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t exec_val_get(libresteam_ext_repl_ctx_t *ctx, const exec_type_t *type, const char *text, uint64_t *out)
{
    exec_ctx_t *exec = (exec_ctx_t *)ctx->exec;
    libresteam_ext_repl_expr_env_t env = LDG_STRUCT_ZERO_INIT;
    libresteam_ext_repl_expr_val_t val = LDG_STRUCT_ZERO_INIT;
    libresteam_fp32_t narrow = 0x0;
    double real = 0x0;
    uint64_t mag = 0x0;
    uint64_t limit = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t idx = 0x0;
    uint8_t neg = text[0] == '-';

    *out = 0;
    if (type->ptr) { return libresteam_ext_repl_native_addr_get(ctx, text, out); }

    if (type->fp)
    {
        for (idx = 0; text[idx]; idx++) { if ((text[idx] < '0' || text[idx] > '9') && text[idx] != '.' && text[idx] != '-' && text[idx] != '+' && text[idx] != 'e' && text[idx] != 'E') { return LDG_ERR_FUNC_ARG_INVALID; } }
        res = libresteam_ext_repl_expr_compile(&exec->expr, text);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_eval(&exec->expr, &env, &val); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_real_get(&val, &real); }

        if (res != LDG_ERR_AOK) { return res; }

        if (type->bytes == sizeof(uint64_t)) { return ldg_mem_secure_cpy(out, &real, sizeof(real)); }

        if (real > FLT_MAX || real < -FLT_MAX) { return LDG_ERR_RANGE; }

        narrow = (libresteam_fp32_t)real;
        return ldg_mem_secure_cpy(out, &narrow, sizeof(narrow));
    }

    if (neg && !type->signed_val) { return LDG_ERR_RANGE; }

    res = libresteam_ext_repl_num_get(text + (neg || text[0] == '+' ? 1U : 0U), &mag);
    if (res != LDG_ERR_AOK) { return res; }

    limit = UINT64_MAX >> ((sizeof(uint64_t) - type->bytes) * LDG_BYTE_BITS);
    if (type->signed_val) { limit = (limit >> 1U) + neg; }

    if (mag > limit) { return LDG_ERR_RANGE; }

    *out = neg ? UINT64_C(0) - mag : mag;
    return LDG_ERR_AOK;
}

static uint32_t exec_mem_validate(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint64_t len, uint8_t code)
{
    ldg_mem_region_t region = LDG_STRUCT_ZERO_INIT;
    uint64_t end = 0x0;
    uint64_t next = 0x0;

    if (LDG_UNLIKELY(!addr || !len || addr > UINTPTR_MAX || len > UINTPTR_MAX - addr)) { return LDG_ERR_RANGE; }

    end = addr + len;
    while (addr < end)
    {
        if (ldg_mem_region_get(&ctx->proc, addr, &region) != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (!region.committed || region.guard || !(code ? region.executable : region.writable)) { return LDG_ERR_DENIED; }

        if (region.size > UINT64_MAX - region.base) { return LDG_ERR_OVERFLOW; }

        next = region.base + region.size;
        if (next <= addr) { return LDG_ERR_INVALID; }

        addr = next;
    }
    return LDG_ERR_AOK;
}

static uint32_t exec_write(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args, uint8_t raw)
{
    exec_ctx_t *exec = (exec_ctx_t *)ctx->exec;
    exec_type_t type = LDG_STRUCT_ZERO_INIT;
    uint64_t addr = 0x0;
    uint64_t val = 0x0;
    uint64_t written = 0x0;
    uint32_t len = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t emitted = LDG_ERR_AOK;
    char pair[3] = LDG_ARR_ZERO_INIT;

    if (argc != (raw ? 3U : 4U)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_native_addr_get(ctx, args[raw ? 1U : 2U], &addr);
    if (res != LDG_ERR_AOK) { return res; }

    if (raw)
    {
        while (args[2][idx])
        {
            if (len == EXEC_WR_CAP || !args[2][idx + 1U]) { return LDG_ERR_RANGE; }

            pair[0] = args[2][idx];
            pair[1] = args[2][idx + 1U];
            res = ldg_hex_to_bytes(pair, exec->bytes + len, 1U);
            if (res != LDG_ERR_AOK) { return res; }

            len++;
            idx += 2U;
        }
    }
    else
    {
        res = exec_type_get(args[1], LDG_TRUTH_FALSE, &type);
        if (res == LDG_ERR_AOK) { res = exec_val_get(ctx, &type, args[3], &val); }

        if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(exec->bytes, &val, type.bytes); }

        if (res != LDG_ERR_AOK) { return res; }

        len = type.bytes;
    }

    res = exec_mem_validate(ctx, addr, len, LDG_TRUTH_FALSE);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_mem_vmem_wr(&ctx->proc, addr, exec->bytes, len, &written) != LDG_ERR_AOK || written != len) { res = LDG_ERR_IO_WR; }

    if (written && ldg_mem_vmem_flush(&ctx->proc, addr, written) != LDG_ERR_AOK) { res = LDG_ERR_IO_FLUSH; }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_rd(ctx, addr, exec->verify, len); }

    if (res == LDG_ERR_AOK) { for (idx = 0; idx < len; idx++) { if (exec->bytes[idx] != exec->verify[idx]) { res = LDG_ERR_IO_WR; break; } } }

    emitted = libresteam_ext_repl_emit(ctx, "write; address: 0x%x64; bytes: %u32; written: %u64; res: %u32", addr, len, written, res);
    return res == LDG_ERR_AOK ? emitted : res;
}

static uint32_t exec_buff(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    exec_ctx_t *exec = (exec_ctx_t *)ctx->exec;
    void *ptr = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "status") == LDG_TRUTH_TRUE) { return libresteam_ext_repl_emit(ctx, "buff; used: %u64; remaining: %u64; capacity: %u32", ldg_mem_pool_used_get(exec->pool), ldg_mem_pool_remaining_get(exec->pool), EXEC_BUFF_CAP); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "reset") == LDG_TRUTH_TRUE)
    {
        res = ldg_mem_pool_rst(exec->pool);
        return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "buff reset; all previous buff addresses invalidated") : res;
    }

    res = libresteam_ext_repl_num_get(args[1], &len);
    if (res != LDG_ERR_AOK) { return res; }

    if (!len || len > EXEC_BUFF_CAP) { return LDG_ERR_RANGE; }

    res = ldg_mem_pool_alloc(exec->pool, len, &ptr);
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_zero(ptr, len); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "buff; address: %p; bytes: %u64; remaining: %u64", ptr, len, ldg_mem_pool_remaining_get(exec->pool)) : res;
}

static uint32_t exec_invoke(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    exec_ctx_t *exec = (exec_ctx_t *)ctx->exec;
    exec_type_t ret = LDG_STRUCT_ZERO_INIT;
    exec_type_t arg = LDG_STRUCT_ZERO_INIT;
    libresteam_ext_repl_expr_val_t decoded = LDG_STRUCT_ZERO_INIT;
    char name[EXEC_TYPE_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0x0;
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint64_t bits = 0x0;

    if (argc < 3U || argc > 3U + LIBRESTEAM_EXT_REPL_EXEC_ARG_CAP) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = ldg_mem_secure_zero(&exec->frame, sizeof(exec->frame));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_addr_get(ctx, args[1], &exec->frame.addr); }

    if (res == LDG_ERR_AOK) { res = exec_type_get(args[2], LDG_TRUTH_TRUE, &ret); }

    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 3U; idx < argc; idx++)
    {
        pos = 0;
        while (args[idx][pos] && args[idx][pos] != ':' && pos + LDG_STR_TERM_SIZE < sizeof(name)) { name[pos] = args[idx][pos]; pos++; }
        name[pos] = LDG_STR_TERM;
        if (args[idx][pos] != ':' || !args[idx][pos + 1U]) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = exec_type_get(name, LDG_TRUTH_FALSE, &arg);
        if (res == LDG_ERR_AOK) { res = exec_val_get(ctx, &arg, args[idx] + pos + EXEC_TYPE_SEP_SIZE, &exec->frame.arg[idx - EXEC_INVOKE_ARG_BASE]); }

        if (res != LDG_ERR_AOK) { return res; }

        if (arg.fp) { exec->frame.fp_mask |= (uint32_t)LDG_BIT(idx - EXEC_INVOKE_ARG_BASE); }
    }
    exec->frame.arg_cunt = argc - EXEC_INVOKE_ARG_BASE;
    res = exec_mem_validate(ctx, exec->frame.addr, sizeof(uint8_t), LDG_TRUTH_TRUE);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_call_frame_run(&exec->frame);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ret.bytes) { return libresteam_ext_repl_emit(ctx, "invoke; address: 0x%x64; thread: %u32; return: void", exec->frame.addr, ctx->main_thread); }

    bits = ret.fp ? exec->frame.fp_ret : exec->frame.ret;
    bits &= UINT64_MAX >> ((sizeof(uint64_t) - ret.bytes) * LDG_BYTE_BITS);
    res = libresteam_ext_repl_expr_decode(bits, ret.bytes, ret.fp, ret.signed_val, &decoded);
    if (res != LDG_ERR_AOK) { return libresteam_ext_repl_emit(ctx, "invoke; address: 0x%x64; type: %s; bits: 0x%x64; decode res: %u32", exec->frame.addr, args[2], bits, res); }

    if (ret.fp) { return libresteam_ext_repl_emit(ctx, "invoke; address: 0x%x64; type: %s; value: %f; bits: 0x%x64", exec->frame.addr, args[2], decoded.real, bits); }

    return libresteam_ext_repl_emit(ctx, "invoke; address: 0x%x64; type: %s; value: %s%u64; bits: 0x%x64", exec->frame.addr, args[2], decoded.neg ? "-" : "", decoded.mag, bits);
}

uint32_t libresteam_ext_repl_exec_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    exec_ctx_t *exec = 0x0;
    uint32_t thread = 0x0;
    uint32_t res = LDG_ERR_NOT_FOUND;

    if (LDG_UNLIKELY(!ctx || !ctx->exec || !args || !argc)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(libresteam_ext_repl_thread_get(&thread) != LDG_ERR_AOK || ctx->main_thread != thread)) { return LDG_ERR_DENIED; }

    exec = (exec_ctx_t *)ctx->exec;
    if (LDG_UNLIKELY(exec->busy)) { return LDG_ERR_BUSY; }

    exec->busy = LDG_TRUTH_TRUE;
    if (libresteam_ext_repl_cmd_eq_is(args[0], "write") == LDG_TRUTH_TRUE) { res = exec_write(ctx, argc, args, LDG_TRUTH_FALSE); }

    else if (libresteam_ext_repl_cmd_eq_is(args[0], "poke") == LDG_TRUTH_TRUE) { res = exec_write(ctx, argc, args, LDG_TRUTH_TRUE); }

    else if (libresteam_ext_repl_cmd_eq_is(args[0], "buff") == LDG_TRUTH_TRUE) { res = exec_buff(ctx, argc, args); }

    else if (libresteam_ext_repl_cmd_eq_is(args[0], "invoke") == LDG_TRUTH_TRUE) { res = exec_invoke(ctx, argc, args); }

    exec->busy = LDG_TRUTH_FALSE;
    return res;
}
