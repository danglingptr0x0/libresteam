#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/bits.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/region.h>
#include <dangling/mem/secure.h>
#include <dangling/mem/vmem.h>
#include <dangling/str/str.h>
#include <dangling/sys/info.h>
#include <dangling/sys/mod.h>
#include <dangling/sys/proc.h>
#include <dangling/thread/ref.h>

#include <libresteam/ext/repl/repl.h>
#include <libresteam/ext/repl/exec.h>
#include <libresteam/ext/repl/scan.h>
#include <libresteam/ext/repl/xref.h>

#define NATIVE_PEEK_CAP 256U
#define NATIVE_HEX_ROW 16U
#define NATIVE_DEFAULT_PEEK 64U
#define NATIVE_THREAD_PAGE_CAP 32U
#define NATIVE_PATTERN_CAP 128U
#define NATIVE_SCAN_CHUNK (4U * LDG_KIB)
#define NATIVE_SCAN_STEPS 4U
#define NATIVE_SCAN_MAX (256U * LDG_MIB)
#define NATIVE_HEX_PAIR 2U
#define NATIVE_XREF_CONTEXT_BYTES 8U
#define NATIVE_XREF_CONTEXT_CAP (LIBRESTEAM_EXT_REPL_XREF_INSN_CAP + 2U * NATIVE_XREF_CONTEXT_BYTES)
#define NATIVE_MOD_CAP 1024U
#define NATIVE_THREAD_CAP 4096U
#define NATIVE_DETOUR_JMP_OPCODE 0xe9U
#define NATIVE_DETOUR_JMP_BYTES 5U
#define NATIVE_DETOUR_RELAY_BYTES 14U
#define NATIVE_DETOUR_RELAY_PTR_OFF 6U

typedef struct native_ctx
{
    ldg_sys_mod_t mods[NATIVE_MOD_CAP];
    ldg_sys_mod_t mod;
    ldg_mem_region_t region;
    uint64_t threads[NATIVE_THREAD_CAP];
    uint8_t bytes[NATIVE_PEEK_CAP];
    char hex[NATIVE_HEX_ROW * 3U + LDG_STR_TERM_SIZE];
    char path[512];
    uint8_t pattern[NATIVE_PATTERN_CAP];
    uint8_t mask[NATIVE_PATTERN_CAP];
    uint8_t scan[NATIVE_SCAN_CHUNK + NATIVE_PATTERN_CAP];
    uint8_t tail[NATIVE_PATTERN_CAP];
    uint8_t pudding[7];
    uint64_t cursor;
    uint64_t end;
    uint64_t examined;
    uint64_t skipped;
    uint32_t pattern_len;
    uint32_t carry;
    uint32_t hits;
    uint8_t xrefs;
    uint8_t pudding_tail[3];
    uint64_t xref_tgt;
    char xref_hex[NATIVE_XREF_CONTEXT_CAP * 3U + LDG_STR_TERM_SIZE];
} native_ctx_t;

uint32_t libresteam_ext_repl_native_init(libresteam_ext_repl_ctx_t *ctx)
{
    uint64_t pid = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_sys_pid_get(&pid);
    if (res == LDG_ERR_AOK && pid > UINT32_MAX) { res = LDG_ERR_RANGE; }

    if (res == LDG_ERR_AOK) { res = ldg_sys_proc_open((uint32_t)pid, &ctx->proc); }

    if (res != LDG_ERR_AOK) { return res; }

    ctx->proc_up = LDG_TRUTH_TRUE;
    res = ldg_mem_alloc(sizeof(native_ctx_t), &ctx->native);
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_zero(ctx->native, sizeof(native_ctx_t)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_scan_init(ctx, (uintptr_t)ctx->native, sizeof(native_ctx_t)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_exec_init(ctx); }

    return res;
}

uint32_t libresteam_ext_repl_native_shutdown(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_ext_repl_exec_shutdown(ctx);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_ext_repl_scan_shutdown(ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (ctx->native)
    {
        res = ldg_mem_dealloc(ctx->native);
        if (res != LDG_ERR_AOK) { return res; }

        ctx->native = 0x0;
    }

    if (!ctx->proc_up) { return LDG_ERR_AOK; }

    res = ldg_sys_proc_close(&ctx->proc);
    if (res == LDG_ERR_AOK) { ctx->proc_up = LDG_TRUTH_FALSE; }

    return res;
}

uint32_t libresteam_ext_repl_thread_get(uint32_t *out)
{
    uint64_t id = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;
    res = ldg_thread_id_self_get(&id);
    if (res != LDG_ERR_AOK) { return res; }

    if (id > UINT32_MAX) { return LDG_ERR_RANGE; }

    *out = (uint32_t)id;
    return LDG_ERR_AOK;
}

static uint32_t native_img_get(libresteam_ext_repl_ctx_t *ctx, ldg_sys_mod_t *out)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t cunt = 0x0;
    uint64_t idx = 0x0;
    uint32_t cmp = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_sys_proc_img_path_get(&ctx->proc, native->path, sizeof(native->path));
    if (res == LDG_ERR_AOK) { res = ldg_sys_mod_list(&ctx->proc, native->mods, NATIVE_MOD_CAP, &cunt); }

    if (res != LDG_ERR_AOK) { return res; }

    if (cunt > NATIVE_MOD_CAP) { cunt = NATIVE_MOD_CAP; }

    for (idx = 0; idx < cunt; idx++)
    {
        res = ldg_str_cmp(native->path, native->mods[idx].path, sizeof(native->path), &cmp);
        if (res != LDG_ERR_AOK) { return res; }

        if (cmp != LDG_ARITH_ORD_EQ) { continue; }

        *out = native->mods[idx];
        return LDG_ERR_AOK;
    }
    return LDG_ERR_NOT_FOUND;
}

static uint32_t native_modules(libresteam_ext_repl_ctx_t *ctx, uint32_t start)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t cunt = 0x0;
    uint64_t idx = 0x0;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_sys_mod_list(&ctx->proc, native->mods, NATIVE_MOD_CAP, &cunt);
    if (res != LDG_ERR_AOK) { return res; }

    if (cunt > NATIVE_MOD_CAP) { cunt = NATIVE_MOD_CAP; }

    for (idx = start; idx < cunt && rows < LIBRESTEAM_EXT_REPL_ROW_CAP; idx++)
    {
        res = libresteam_ext_repl_name_get(native->mods[idx].name, ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "module: %s; base: 0x%x64; bytes: %u64", ctx->name, native->mods[idx].base, native->mods[idx].size); }

        if (res != LDG_ERR_AOK) { return res; }

        rows++;
    }
    return libresteam_ext_repl_emit(ctx, "rows: %u32; next start: %u64; more: %u32", rows, idx, (uint32_t)(idx < cunt));
}

static uint32_t native_symbol(libresteam_ext_repl_ctx_t *ctx, const char *module, const char *name)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t addr = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_sys_mod_find(&ctx->proc, module, &native->mod);
    if (res == LDG_ERR_AOK) { res = ldg_sys_mod_sym_get(&ctx->proc, &native->mod, name, &addr); }

    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_ext_repl_emit(ctx, "symbol: %s!%s; address: 0x%x64; module base: 0x%x64", module, name, addr, native->mod.base);
}

static uint8_t native_readable_is(const ldg_mem_region_t *region)
{
    return region->committed && region->readable && !region->guard ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

uint32_t libresteam_ext_repl_native_rd(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, void *out, uint64_t len)
{
    native_ctx_t *native = 0x0;
    uint64_t cursor = addr;
    uint64_t next = 0x0;
    uint64_t end = 0x0;
    uint64_t got = 0x0;

    if (LDG_UNLIKELY(!ctx || !ctx->native || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    native = (native_ctx_t *)ctx->native;
    if (!len || addr > UINTPTR_MAX || len > UINTPTR_MAX - addr) { return LDG_ERR_RANGE; }

    end = addr + len;
    while (cursor < end)
    {
        if (ldg_mem_region_get(&ctx->proc, cursor, &native->region) != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (native_readable_is(&native->region) != LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

        if (native->region.size > UINT64_MAX - native->region.base) { return LDG_ERR_OVERFLOW; }

        next = native->region.base + native->region.size;
        if (next <= cursor) { return LDG_ERR_INVALID; }

        cursor = next;
    }
    if (ldg_mem_vmem_rd(&ctx->proc, addr, out, len, &got) != LDG_ERR_AOK || got != len) { return LDG_ERR_IO_RD; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_native_addr_get(libresteam_ext_repl_ctx_t *ctx, const char *text, uint64_t *out)
{
    native_ctx_t *native = 0x0;
    uint64_t off = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->native || !text || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    native = (native_ctx_t *)ctx->native;
    *out = 0;
    res = libresteam_ext_repl_num_get(text, out);
    if (res == LDG_ERR_AOK) { return *out <= UINTPTR_MAX ? LDG_ERR_AOK : LDG_ERR_RANGE; }

    while (text[idx] && text[idx] != '+')
    {
        if (idx + LDG_STR_TERM_SIZE >= sizeof(native->path)) { return LDG_ERR_STR_TRUNC; }

        native->path[idx] = text[idx];
        idx++;
    }
    native->path[idx] = LDG_STR_TERM;
    if (!idx) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (text[idx] == '+')
    {
        res = libresteam_ext_repl_num_get(text + idx + 1U, &off);
        if (res != LDG_ERR_AOK) { return res; }
    }

    if (libresteam_ext_repl_cmd_eq_is(native->path, "exe") == LDG_TRUTH_TRUE) { res = native_img_get(ctx, &native->mod); }
    else { res = ldg_sys_mod_find(&ctx->proc, native->path, &native->mod); }

    if (res != LDG_ERR_AOK) { return res; }

    if (off > UINT64_MAX - native->mod.base) { return LDG_ERR_OVERFLOW; }

    *out = native->mod.base + off;
    return LDG_ERR_AOK;
}

static uint32_t native_regions(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint8_t single)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    const char *kind = 0x0;
    uint64_t next = addr;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t done = LDG_TRUTH_FALSE;

    while (rows < LIBRESTEAM_EXT_REPL_ROW_CAP)
    {
        res = ldg_mem_region_get(&ctx->proc, next, &native->region);
        if (res == LDG_ERR_EOF)
        {
            done = LDG_TRUTH_TRUE;
            break;
        }

        if (res != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

        if (native->region.size > UINT64_MAX - native->region.base) { return LDG_ERR_OVERFLOW; }

        addr = native->region.base + native->region.size;
        if (addr <= next) { return LDG_ERR_INVALID; }

        kind = native->region.kind == LDG_MEM_REGION_KIND_PRIVATE ? "private" : native->region.kind == LDG_MEM_REGION_KIND_MAPPED ? "mapped" : native->region.kind == LDG_MEM_REGION_KIND_IMG ? "image" : "none";
        res = libresteam_ext_repl_emit(ctx, "region: 0x%x64; bytes: %u64; allocation: 0x%x64; committed: %u32; readable: %u32; writable: %u32; executable: %u32; guard: %u32; kind: %s", native->region.base, native->region.size, native->region.alloc_base, (uint32_t)native->region.committed, (uint32_t)native->region.readable, (uint32_t)native->region.writable, (uint32_t)native->region.executable, (uint32_t)native->region.guard, kind);
        if (res != LDG_ERR_AOK) { return res; }

        next = addr;
        rows++;
        if (single) { break; }
    }
    return libresteam_ext_repl_emit(ctx, "rows: %u32; next address: 0x%x64; end: %u32", rows, next, (uint32_t)done);
}

static uint32_t native_value(libresteam_ext_repl_ctx_t *ctx, const char *kind, uint64_t addr)
{
    uint64_t val = 0x0;
    double real = 0;
    libresteam_fp32_t narrow = 0;
    uint32_t size = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_ext_repl_cmd_eq_is(kind, "u8") == LDG_TRUTH_TRUE) { size = sizeof(uint8_t); }
    else if (libresteam_ext_repl_cmd_eq_is(kind, "u16") == LDG_TRUTH_TRUE) { size = sizeof(uint16_t); }
    else if (libresteam_ext_repl_cmd_eq_is(kind, "u32") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(kind, "f32") == LDG_TRUTH_TRUE) { size = sizeof(uint32_t); }
    else if (libresteam_ext_repl_cmd_eq_is(kind, "u64") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(kind, "f64") == LDG_TRUTH_TRUE) { size = sizeof(uint64_t); }
    else if (libresteam_ext_repl_cmd_eq_is(kind, "ptr") == LDG_TRUTH_TRUE) { size = sizeof(uintptr_t); }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_native_rd(ctx, addr, &val, size);
    if (res != LDG_ERR_AOK) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(kind, "f32") == LDG_TRUTH_TRUE)
    {
        res = ldg_mem_secure_cpy(&narrow, &val, sizeof(narrow));
        real = (double)narrow;
    }
    else if (libresteam_ext_repl_cmd_eq_is(kind, "f64") == LDG_TRUTH_TRUE) { res = ldg_mem_secure_cpy(&real, &val, sizeof(real)); }
    else { return libresteam_ext_repl_emit(ctx, "address: 0x%x64; type: %s; value: %u64; hex: 0x%x64", addr, kind, val, val); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "address: 0x%x64; type: %s; value: %f; bits: 0x%x64", addr, kind, real, val) : res;
}

static uint32_t native_peek(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint64_t len)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint32_t idx = 0x0;
    uint32_t row = 0x0;
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (len == 0 || len > sizeof(native->bytes) || addr > UINT64_MAX - len) { return LDG_ERR_RANGE; }

    res = libresteam_ext_repl_native_rd(ctx, addr, native->bytes, len);
    if (res != LDG_ERR_AOK) { return res; }

    while (idx < len)
    {
        row = idx;
        pos = 0;
        while (idx < len && idx - row < NATIVE_HEX_ROW)
        {
            res = ldg_byte_to_hex(native->bytes[idx++], native->hex + pos);
            if (res != LDG_ERR_AOK) { return res; }

            pos += 2U;
            native->hex[pos++] = ' ';
        }
        native->hex[pos] = LDG_STR_TERM;
        res = libresteam_ext_repl_emit(ctx, "0x%x64: %s", addr + row, native->hex);
        if (res != LDG_ERR_AOK) { return res; }
    }
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_native_detour_validate(libresteam_ext_repl_ctx_t *ctx, uint64_t addr)
{
    uint8_t code[NATIVE_DETOUR_JMP_BYTES] = LDG_ARR_ZERO_INIT;
    ldg_mem_region_t region = LDG_STRUCT_ZERO_INIT;
    uint64_t candidates[2] = LDG_ARR_ZERO_INIT;
    uint8_t relay[NATIVE_DETOUR_RELAY_BYTES] = LDG_ARR_ZERO_INIT;
    uint64_t disp = 0x0;
    uint64_t target = 0x0;
    uint64_t entry = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_native_rd(ctx, addr, code, sizeof(code));
    if (res != LDG_ERR_AOK) { return res; }

    if (code[0] != NATIVE_DETOUR_JMP_OPCODE || addr > UINT64_MAX - NATIVE_DETOUR_JMP_BYTES) { return LDG_ERR_UNSUPPORTED; }

    target = addr + NATIVE_DETOUR_JMP_BYTES;
    for (idx = 0; idx < sizeof(uint32_t); idx++) { disp |= (uint64_t)code[idx + 1U] << (idx * LDG_BYTE_BITS); }
    if (disp > INT32_MAX)
    {
        disp = (uint64_t)UINT32_MAX - disp + 1U;
        if (target < disp) { return LDG_ERR_BOUNDS; }

        target -= disp;
    }
    else
    {
        if (target > UINT64_MAX - disp) { return LDG_ERR_BOUNDS; }

        target += disp;
    }

    res = libresteam_ext_repl_native_rd(ctx, target, relay, sizeof(relay));
    if (res != LDG_ERR_AOK) { return res; }

    if (relay[0] != 0xffU || relay[1] != 0x25U || relay[2] || relay[3] || relay[4] || relay[5]) { return LDG_ERR_UNSUPPORTED; }

    for (idx = 0; idx < sizeof(entry); idx++) { entry |= (uint64_t)relay[NATIVE_DETOUR_RELAY_PTR_OFF + idx] << (idx * LDG_BYTE_BITS); }
    candidates[0] = target;
    candidates[1] = entry;
    for (idx = 0; idx < sizeof(candidates) / sizeof(candidates[0]); idx++)
    {
        if (!candidates[idx] || candidates[idx] > UINTPTR_MAX || ldg_mem_region_get(&ctx->proc, candidates[idx], &region) != LDG_ERR_AOK) { return LDG_ERR_MEM_BAD; }

        if (!region.committed || region.guard || !region.executable) { return LDG_ERR_DENIED; }
    }
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_native_entry(libresteam_ext_repl_ctx_t *ctx, uint64_t addr)
{
    native_ctx_t *native = 0x0;
    uint64_t base = 0x0;

    if (LDG_UNLIKELY(!ctx || !ctx->native)) { return LDG_ERR_FUNC_ARG_NULL; }

    native = (native_ctx_t *)ctx->native;
    if (addr > UINTPTR_MAX) { return LDG_ERR_RANGE; }

    if (!addr || ldg_mem_region_get(&ctx->proc, addr, &native->region) != LDG_ERR_AOK) { return libresteam_ext_repl_emit(ctx, "native entry: unavailable"); }

    if (!native->region.committed || native->region.guard || !native->region.executable) { return libresteam_ext_repl_emit(ctx, "native entry: unavailable"); }

    base = native->region.alloc_base;
    native->mod.path[0] = LDG_STR_TERM;
    if (ldg_sys_mod_addr_find(&ctx->proc, addr, &native->mod) == LDG_ERR_AOK) { base = native->mod.base; }
    else { native->mod.path[0] = LDG_STR_TERM; }

    if (base > addr) { return LDG_ERR_INVALID; }

    if (libresteam_ext_repl_name_get(native->mod.path, ctx->name, sizeof(ctx->name)) != LDG_ERR_AOK) { return LDG_ERR_STR_FMT; }

    return libresteam_ext_repl_emit(ctx, "native entry candidate: 0x%x64; base: 0x%x64; rva: 0x%x64; module: %s", addr, base, addr - base, ctx->name);
}

static uint32_t native_search_begin(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t addr = 0x0;
    uint64_t len = 0x0;
    uint64_t value = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    char byte[NATIVE_HEX_PAIR + LDG_STR_TERM_SIZE] = LDG_ARR_ZERO_INIT;

    if (argc != 4U) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[2], &len); }

    if (res != LDG_ERR_AOK) { return res; }

    if (!len || len > NATIVE_SCAN_MAX || len > UINTPTR_MAX - addr) { return LDG_ERR_RANGE; }

    native->pattern_len = 0;
    if (libresteam_ext_repl_cmd_eq_is(args[0], "findptr") == LDG_TRUTH_TRUE)
    {
        res = libresteam_ext_repl_native_addr_get(ctx, args[3], &value);
        if (res != LDG_ERR_AOK) { return res; }

        native->pattern_len = sizeof(uintptr_t);
        for (idx = 0; idx < native->pattern_len; idx++)
        {
            native->pattern[idx] = (uint8_t)(value >> (idx * LDG_BYTE_BITS));
            native->mask[idx] = LDG_BYTE_MASK;
        }
    }
    else if (libresteam_ext_repl_cmd_eq_is(args[0], "findstr") == LDG_TRUTH_TRUE)
    {
        for (idx = 0; args[3][idx]; idx++)
        {
            if (idx == NATIVE_PATTERN_CAP) { return LDG_ERR_FULL; }

            native->pattern[idx] = (uint8_t)args[3][idx];
            native->mask[idx] = LDG_BYTE_MASK;
        }
        native->pattern_len = idx;
    }
    else
    {
        for (idx = 0; args[3][idx]; idx += NATIVE_HEX_PAIR)
        {
            if (!args[3][idx + 1U]) { return LDG_ERR_STR_INVALID; }

            if (native->pattern_len == NATIVE_PATTERN_CAP) { return LDG_ERR_FULL; }

            if (args[3][idx] == '?' && args[3][idx + 1U] == '?')
            {
                native->pattern[native->pattern_len] = 0;
                native->mask[native->pattern_len++] = 0;
                continue;
            }

            byte[0] = args[3][idx];
            byte[1] = args[3][idx + 1U];
            if (ldg_char_hex_is((uint8_t)byte[0]) != LDG_TRUTH_TRUE || ldg_char_hex_is((uint8_t)byte[1]) != LDG_TRUTH_TRUE) { return LDG_ERR_STR_INVALID; }

            res = ldg_str_uint_parse(byte, LDG_BASE_HEX, NATIVE_HEX_PAIR, &value);
            if (res != LDG_ERR_AOK) { return res; }

            native->pattern[native->pattern_len] = (uint8_t)value;
            native->mask[native->pattern_len++] = LDG_BYTE_MASK;
        }
    }

    if (!native->pattern_len || native->pattern_len > len) { return LDG_ERR_RANGE; }

    native->cursor = addr;
    native->end = addr + len;
    native->examined = 0;
    native->skipped = 0;
    native->carry = 0;
    native->hits = 0;
    native->xrefs = LDG_TRUTH_FALSE;
    ctx->searching = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t native_xrefs_begin(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t tgt = 0x0;
    uint64_t addr = 0x0;
    uint64_t len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(argc != 2U && argc != 4U)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (sizeof(uintptr_t) != sizeof(uint64_t)) { return LDG_ERR_UNSUPPORTED; }

    res = libresteam_ext_repl_native_addr_get(ctx, args[1], &tgt);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (argc == 4U)
    {
        res = libresteam_ext_repl_native_addr_get(ctx, args[2], &addr);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[3], &len); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    else
    {
        res = native_img_get(ctx, &native->mod);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        addr = native->mod.base;
        len = native->mod.size;
    }

    if (LDG_UNLIKELY(!len || len > NATIVE_SCAN_MAX || len > UINTPTR_MAX - addr)) { return LDG_ERR_RANGE; }

    res = libresteam_ext_repl_emit(ctx, "xref candidates; target: 0x%x64; start: 0x%x64; bytes: %u64; executable pages only; instruction boundaries unverified", tgt, addr, len);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    native->cursor = addr;
    native->end = addr + len;
    native->examined = 0;
    native->skipped = 0;
    native->carry = 0;
    native->hits = 0;
    native->xref_tgt = tgt;
    native->xrefs = LDG_TRUTH_TRUE;
    ctx->searching = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t native_xref_tail_rd(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint32_t offset, uint32_t *out)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    ldg_mem_region_t region = LDG_STRUCT_ZERO_INIT;
    uint64_t end = 0x0;
    uint64_t len = 0x0;
    uint64_t got = 0x0;
    uint32_t remaining = LIBRESTEAM_EXT_REPL_XREF_INSN_CAP - LDG_STR_TERM_SIZE;

    *out = 0;
    while (remaining && addr < native->end)
    {
        if (ldg_mem_region_get(&ctx->proc, addr, &region) != LDG_ERR_AOK) { break; }

        if (native_readable_is(&region) != LDG_TRUTH_TRUE || !region.executable) { break; }

        if (region.size > UINT64_MAX - region.base) { return LDG_ERR_OVERFLOW; }

        end = region.base + region.size;
        if (LDG_UNLIKELY(end <= addr)) { return LDG_ERR_INVALID; }

        if (end > native->end) { end = native->end; }

        len = end - addr;
        if (len > remaining) { len = remaining; }

        if (ldg_mem_vmem_rd(&ctx->proc, addr, native->scan + offset + *out, len, &got) != LDG_ERR_AOK || got != len) { break; }

        *out += (uint32_t)len;
        remaining -= (uint32_t)len;
        addr += len;
    }
    return LDG_ERR_AOK;
}

static uint32_t native_xref_emit(libresteam_ext_repl_ctx_t *ctx, uint32_t idx, uint32_t total, const libresteam_ext_repl_xref_t *xref)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint32_t begin = idx > NATIVE_XREF_CONTEXT_BYTES ? idx - NATIVE_XREF_CONTEXT_BYTES : 0;
    uint32_t end = idx + xref->len + NATIVE_XREF_CONTEXT_BYTES;
    uint32_t pos = 0x0;
    uint32_t byte = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (end > total) { end = total; }

    for (byte = begin; byte < end; byte++)
    {
        res = ldg_byte_to_hex(native->scan[byte], native->xref_hex + pos);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        pos += NATIVE_HEX_PAIR;
        native->xref_hex[pos++] = ' ';
    }
    native->xref_hex[pos] = LDG_STR_TERM;
    return libresteam_ext_repl_emit(ctx, "xref candidate: 0x%x64; kind: %s; target: 0x%x64; length: %u32; context: 0x%x64; instruction offset: %u32; bytes: %s", native->cursor + idx, xref->kind, xref->tgt, xref->len, native->cursor + begin, idx - begin, native->xref_hex);
}

static uint32_t native_xref_chunk(libresteam_ext_repl_ctx_t *ctx, uint64_t end, uint32_t len)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    libresteam_ext_repl_xref_t xref = LDG_STRUCT_ZERO_INIT;
    uint32_t extra = 0x0;
    uint32_t total = 0x0;
    uint32_t idx = 0x0;
    uint32_t avail = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = native_xref_tail_rd(ctx, end, len, &extra);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    total = len + extra;
    for (idx = 0; idx < len; idx++)
    {
        avail = total - idx;
        if (avail > LIBRESTEAM_EXT_REPL_XREF_INSN_CAP) { avail = LIBRESTEAM_EXT_REPL_XREF_INSN_CAP; }

        res = libresteam_ext_repl_xref_decode(native->scan + idx, avail, native->cursor + idx, &xref);
        if (res == LDG_ERR_NOT_FOUND || res == LDG_ERR_OVERFLOW) { continue; }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (xref.tgt != native->xref_tgt) { continue; }

        res = native_xref_emit(ctx, idx, total, &xref);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (++native->hits == LIBRESTEAM_EXT_REPL_ROW_CAP)
        {
            native->cursor += idx + 1U;
            return LDG_ERR_AOK;
        }
    }
    native->cursor = end;
    return LDG_ERR_AOK;
}

static uint32_t native_search_chunk(libresteam_ext_repl_ctx_t *ctx)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t end = 0x0;
    uint64_t len = 0x0;
    uint64_t got = 0x0;
    uint64_t base = 0x0;
    uint32_t total = 0x0;
    uint32_t idx = 0x0;
    uint32_t pat = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ldg_mem_region_get(&ctx->proc, native->cursor, &native->region) != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

    if (native->region.size > UINT64_MAX - native->region.base) { return LDG_ERR_OVERFLOW; }

    end = native->region.base + native->region.size;
    if (end <= native->cursor) { return LDG_ERR_INVALID; }

    if (end > native->end) { end = native->end; }

    if (native_readable_is(&native->region) != LDG_TRUTH_TRUE || (native->xrefs && !native->region.executable))
    {
        native->skipped += end - native->cursor;
        native->cursor = end;
        native->carry = 0;
        return LDG_ERR_AOK;
    }

    len = end - native->cursor;
    if (len > NATIVE_SCAN_CHUNK) { len = NATIVE_SCAN_CHUNK; }

    end = native->cursor + len;
    if ((native->cursor < (uintptr_t)ctx + sizeof(*ctx) && end > (uintptr_t)ctx) || (native->cursor < (uintptr_t)native + sizeof(*native) && end > (uintptr_t)native) || libresteam_ext_repl_scan_owned_is(ctx, native->cursor, len) == LDG_TRUTH_TRUE)
    {
        native->skipped += len;
        native->cursor = end;
        native->carry = 0;
        return LDG_ERR_AOK;
    }

    if (native->carry)
    {
        res = ldg_mem_secure_cpy(native->scan, native->tail, native->carry);
        if (res != LDG_ERR_AOK) { return res; }
    }

    if (ldg_mem_vmem_rd(&ctx->proc, native->cursor, native->scan + native->carry, len, &got) != LDG_ERR_AOK || got != len)
    {
        native->skipped += len;
        native->cursor = end;
        native->carry = 0;
        return LDG_ERR_AOK;
    }

    base = native->cursor - native->carry;
    total = (uint32_t)len + native->carry;
    native->examined += len;
    if (native->xrefs) { return native_xref_chunk(ctx, end, (uint32_t)len); }

    for (idx = 0; idx + native->pattern_len <= total; idx++)
    {
        for (pat = 0; pat < native->pattern_len; pat++) { if ((native->scan[idx + pat] & native->mask[pat]) != native->pattern[pat]) { break; } }
        if (pat != native->pattern_len) { continue; }

        res = libresteam_ext_repl_emit(ctx, "match: 0x%x64", base + idx);
        if (res != LDG_ERR_AOK) { return res; }

        if (++native->hits == LIBRESTEAM_EXT_REPL_ROW_CAP)
        {
            native->cursor = base + idx + 1U;
            return LDG_ERR_AOK;
        }
    }
    native->carry = native->pattern_len - 1U;
    if (native->carry > total) { native->carry = total; }

    if (native->carry)
    {
        res = ldg_mem_secure_cpy(native->tail, native->scan + total - native->carry, native->carry);
        if (res != LDG_ERR_AOK) { return res; }
    }

    native->cursor = end;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_native_search_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    native_ctx_t *native = 0x0;
    uint32_t step = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->native)) { return LDG_ERR_FUNC_ARG_NULL; }

    native = (native_ctx_t *)ctx->native;
    for (step = 0; step < NATIVE_SCAN_STEPS && native->cursor < native->end && native->hits < LIBRESTEAM_EXT_REPL_ROW_CAP; step++)
    {
        res = native_search_chunk(ctx);
        if (res != LDG_ERR_AOK) { ctx->searching = LDG_TRUTH_FALSE; return res; }
    }
    if (native->cursor < native->end && native->hits < LIBRESTEAM_EXT_REPL_ROW_CAP) { return LDG_ERR_AOK; }

    ctx->searching = LDG_TRUTH_FALSE;
    return libresteam_ext_repl_emit(ctx, "matches: %u32; examined: %u64; skipped: %u64; next address: 0x%x64; remaining bytes: %u64; complete: %u32", native->hits, native->examined, native->skipped, native->cursor, native->end - native->cursor, (uint32_t)(native->cursor == native->end));
}

uint32_t libresteam_ext_repl_native_tick(void *user)
{
    uint32_t trace_res = libresteam_ext_repl_trace_tick(user);
    uint32_t scan_res = libresteam_ext_repl_scan_tick(user);

    return trace_res != LDG_ERR_AOK ? trace_res : scan_res;
}

static uint32_t native_trace_threads(libresteam_ext_repl_ctx_t *ctx, uint32_t start)
{
    native_ctx_t *native = (native_ctx_t *)ctx->native;
    uint64_t cunt = 0x0;
    uint64_t idx = 0x0;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_thread_id_list(&ctx->proc, native->threads, NATIVE_THREAD_CAP, &cunt);
    if (res != LDG_ERR_AOK) { return res; }

    if (cunt > NATIVE_THREAD_CAP) { cunt = NATIVE_THREAD_CAP; }

    for (idx = start; idx < cunt && rows < NATIVE_THREAD_PAGE_CAP; idx++)
    {
        res = libresteam_ext_repl_emit(ctx, "thread: %u64; callback: %u32", native->threads[idx], (uint32_t)(native->threads[idx] == ctx->main_thread));
        if (res != LDG_ERR_AOK) { return res; }

        rows++;
    }
    return libresteam_ext_repl_emit(ctx, "threads: %u32; next start: %u64; more: %u32", rows, idx, (uint32_t)(idx < cunt));
}

static uint32_t native_trace_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_emit(ctx, "trace <address> [thread-id] | trace threads [start] | trace status | trace off | trace clear | trace read [hits]; default: all queued hits");
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Default: automatic rotation, 250 ms per thread; refresh after each pass. An explicit thread ID pins tracing to that process thread. Status/read show current thread and rotation progress; hits retain originating thread IDs. Disarm before changing address/thread; old hits remain until read/clear."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Rotation runs on REPL ticks, one thread at a time; calls outside each thread's window can be missed. Worker context changes briefly suspend that thread. BUSY or pending cleanup delays rotation; trace off cancels rotation and retries cleanup."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "trace capture | trace capture off | trace capture <slot: 0..3> off | trace capture <slot> <register> <bytes: 1..128> <offset> [offset...]"); }

    return res;
}

static uint32_t native_trace_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    uint64_t addr = 0x0;
    uint64_t val = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (argc == 1U) { return libresteam_ext_repl_trace_status(ctx); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "capture") == LDG_TRUTH_TRUE) { return libresteam_ext_repl_trace_capture_cmd(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "clear") == LDG_TRUTH_TRUE) { return argc == 2U ? libresteam_ext_repl_trace_clear(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (argc > 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "help") == LDG_TRUTH_TRUE) { return argc == 2U ? native_trace_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "threads") == LDG_TRUTH_TRUE)
    {
        if (argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &val); }

        if (res != LDG_ERR_AOK || val > UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        return native_trace_threads(ctx, (uint32_t)val);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "status") == LDG_TRUTH_TRUE) { return argc == 2U ? libresteam_ext_repl_trace_status(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "off") == LDG_TRUTH_TRUE) { return argc == 2U ? libresteam_ext_repl_trace_off(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "read") == LDG_TRUTH_TRUE)
    {
        val = UINT32_MAX;
        if (argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &val); }

        if (res != LDG_ERR_AOK || val > UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        return libresteam_ext_repl_trace_rd(ctx, (uint32_t)val);
    }

    val = ctx->main_thread;
    if (argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &val); }

    if (res != LDG_ERR_AOK || !val || val > UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
    if (res != LDG_ERR_AOK) { return res; }

    return argc == 2U ? libresteam_ext_repl_trace_auto(ctx, addr) : libresteam_ext_repl_trace_set(ctx, addr, (uint32_t)val);
}

static uint32_t native_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res |= libresteam_ext_repl_emit(ctx, "modules [start] | symbol <module> <export> | address <address>");
    res |= libresteam_ext_repl_emit(ctx, "regions [address] | peek <address> [bytes: 1..256]");
    res |= libresteam_ext_repl_emit(ctx, "read <u8|u16|u32|u64|ptr|f32|f64> <address>");
    res |= libresteam_ext_repl_emit(ctx, "write <type> <address> <value> | poke <address> <hex-bytes> | buff <bytes> | buff status | buff reset");
    res |= libresteam_ext_repl_emit(ctx, "invoke <address> <return-type> [type:value ...]: Windows x64 ABI, up to 8 scalar arguments; explicit this/hidden return pointer when required.");
    res |= libresteam_ext_repl_emit(ctx, "Types: u8/u16/u32/u64/i8/i16/i32/i64/ptr/f32/f64; void return only. Calls run synchronously on the callback thread; invalid calls may crash or block the host.");
    res |= libresteam_ext_repl_emit(ctx, "Writes require writable committed pages, verify bytes and flush instruction cache; no page-protection changes. Buff: zeroed pool allocations, 64 KiB total, valid until reset/unload; do not reset while the game retains pointers.");
    res |= libresteam_ext_repl_emit(ctx, "terminate: force-stop the host process immediately; no save/shutdown callbacks; the client disconnects.");
    res |= libresteam_ext_repl_emit(ctx, "scan: expression/delta/curve scanner; scan help for baseline, filters, timed sampling and ranking");
    res |= libresteam_ext_repl_emit(ctx, "trace <address> [thread-id] | trace threads [start] | trace help | trace status | trace read [hits] | trace off");
    res |= libresteam_ext_repl_emit(ctx, "trace clear | trace capture | trace capture off | trace capture <slot: 0..3> off");
    res |= libresteam_ext_repl_emit(ctx, "trace capture <slot> <register> <bytes: 1..128> <offset> [offset...]: 1..8 signed offsets; dereference each except last, then snapshot. Configure/clear only while disarmed.");
    res |= libresteam_ext_repl_emit(ctx, "Trace: automatic 250 ms thread rotation by default; explicit thread ID pins one thread. Status/read show the current thread; each hit identifies its thread. Queue: 256 hits. Disarm before changing address/thread.");
    res |= libresteam_ext_repl_emit(ctx, "find <address> <bytes> <hex-pattern>   e.g. 488B??FF");
    res |= libresteam_ext_repl_emit(ctx, "findstr <address> <bytes> <ASCII-token> | findptr <address> <bytes> <target-address>");
    res |= libresteam_ext_repl_emit(ctx, "xrefs <target> [start bytes]: x64 code-reference candidates; defaults to the main executable, scans readable executable pages only.");
    res |= libresteam_ext_repl_emit(ctx, "Xrefs: RIP-relative address/memory operands, direct relative branches, MOV imm64 and absolute-memory offsets. Memory targets are operand addresses, not dereferenced values.");
    res |= libresteam_ext_repl_emit(ctx, "Xrefs tests every byte; overlapping decodes/data can produce false positives. Legacy x64 decoder: no VEX/EVEX, address-size override, or FS/GS addressing. Results include context bytes.");
    res |= libresteam_ext_repl_emit(ctx, "Addresses: decimal, 0xhex, module.exe+0xRVA, or exe+0xRVA for the main executable. No spaces in address expressions.");
    res |= libresteam_ext_repl_emit(ctx, "Search: max 256 MiB/request, 128 pattern bytes, 64 matches; use next address and remaining bytes to continue.");
    res |= libresteam_ext_repl_emit(ctx, "Searches read up to 16 KiB/tick, return on completion or 64 matches, and can take minutes for large ranges. Start with a small range.");
    res |= libresteam_ext_repl_emit(ctx, "Searches are not atomic snapshots; unreadable/guard pages and the search workspace are skipped. findstr takes one ASCII token without quotes.");
    res |= libresteam_ext_repl_emit(ctx, "Region rows: committed, readable, writable, executable and guard are flags; kind is none, private, mapped or image.");
    return res;
}

uint32_t libresteam_ext_repl_native_cmd(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    uint64_t addr = 0x0;
    uint64_t len = NATIVE_DEFAULT_PEEK;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->native || !args || !argc)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "write") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "poke") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "buff") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "invoke") == LDG_TRUTH_TRUE) { return libresteam_ext_repl_exec_cmd(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "terminate") == LDG_TRUTH_TRUE)
    {
        if (LDG_UNLIKELY(argc != 1U)) { return LDG_ERR_FUNC_ARG_INVALID; }

        return ldg_sys_proc_term(&ctx->proc, LDG_ERR_AOK);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "scan") == LDG_TRUTH_TRUE) { return libresteam_ext_repl_scan_cmd(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "trace") == LDG_TRUTH_TRUE) { return native_trace_cmd(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "xrefs") == LDG_TRUTH_TRUE) { return native_xrefs_begin(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE) { return argc == 1U ? native_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "status") == LDG_TRUTH_TRUE) { return argc == 1U ? libresteam_ext_repl_emit(ctx, "native ready: 1; pointer bytes: %u32; search bytes per tick: %u32; engine required: 0", (uint32_t)sizeof(uintptr_t), (uint32_t)(NATIVE_SCAN_CHUNK * NATIVE_SCAN_STEPS)) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "modules") == LDG_TRUTH_TRUE)
    {
        if (argc > 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (argc == 2U) { res = libresteam_ext_repl_num_get(args[1], &addr); }

        if (res != LDG_ERR_AOK || addr > UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        return native_modules(ctx, (uint32_t)addr);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "symbol") == LDG_TRUTH_TRUE) { return argc == 3U ? native_symbol(ctx, args[1], args[2]) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "regions") == LDG_TRUTH_TRUE)
    {
        if (argc > 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (argc == 2U) { res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr); }

        return res == LDG_ERR_AOK ? native_regions(ctx, addr, LDG_TRUTH_FALSE) : res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "read") == LDG_TRUTH_TRUE)
    {
        if (argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_native_addr_get(ctx, args[2], &addr);
        return res == LDG_ERR_AOK ? native_value(ctx, args[1], addr) : res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "find") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "findstr") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "findptr") == LDG_TRUTH_TRUE) { return native_search_begin(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "address") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "address: 0x%x64", addr); }

        return res == LDG_ERR_AOK ? native_regions(ctx, addr, LDG_TRUTH_TRUE) : res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "peek") != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    if (argc != 2U && argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
    if (res == LDG_ERR_AOK && argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &len); }

    return res == LDG_ERR_AOK ? native_peek(ctx, addr, len) : res;
}
