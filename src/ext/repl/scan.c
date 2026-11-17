#include <math.h>
#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/region.h>
#include <dangling/mem/secure.h>
#include <dangling/mem/vmem.h>
#include <dangling/time/time.h>

#include <libresteam/ext/repl/expr.h>
#include <libresteam/ext/repl/scan.h>

#define SCAN_CANDIDATE_CAP 32768U
#define SCAN_STEP_CAP 256U
#define SCAN_TIME_BUDGET_MS 2U
#define SCAN_PAGE_BYTES (4U * LDG_KIB)
#define SCAN_RANGE_CAP (256U * LDG_MIB)
#define SCAN_SAMPLE_CAP 4096U
#define SCAN_INTERVAL_MIN_MS 16U
#define SCAN_INTERVAL_MAX_MS 60000U
#define SCAN_BEST_CAP 64U
#define SCAN_LAST_EXPR_ARG 2U

typedef enum scan_mode
{
    SCAN_IDLE,
    SCAN_BASE,
    SCAN_WHERE,
    SCAN_WATCH,
    SCAN_FIT,
    SCAN_REBASE
} scan_mode_t;

typedef struct scan_item
{
    uint64_t addr;
    uint64_t alloc;
    uint64_t base;
    uint64_t prev;
    uint64_t base_ms;
    uint64_t prev_ms;
    double score;
    uint32_t samples;
    uint32_t kind;
    uint8_t alive;
    uint8_t pudding[7];
} scan_item_t;

typedef struct scan_ctx
{
    scan_item_t items[SCAN_CANDIDATE_CAP];
    libresteam_ext_repl_expr_t expr;
    libresteam_ext_repl_expr_t pending;
    libresteam_ext_repl_expr_env_t env;
    uint8_t page[SCAN_PAGE_BYTES];
    char expr_text[LIBRESTEAM_EXT_REPL_EXPR_TEXT_CAP];
    uint32_t best[SCAN_BEST_CAP];
    uint64_t excluded_addr;
    uint64_t excluded_len;
    uint64_t cursor;
    uint64_t end;
    uint64_t skipped;
    uint64_t page_addr;
    uint64_t page_end;
    uint64_t page_alloc;
    uint64_t page_ms;
    uint64_t start_ms;
    uint64_t next_ms;
    uint64_t pass_ms;
    uint64_t pass_span_ms;
    uint64_t max_dt_ms;
    uint32_t page_kind;
    uint32_t mode;
    uint32_t bytes;
    uint32_t stride;
    uint32_t cunt;
    uint32_t alive;
    uint32_t idx;
    uint32_t pass;
    uint32_t passes;
    uint32_t interval_ms;
    uint32_t errors;
    uint32_t lost;
    uint32_t res;
    uint32_t best_cunt;
    uint8_t fp;
    uint8_t signed_val;
    uint8_t ready;
    uint8_t ranking;
    uint8_t pudding[4];
} scan_ctx_t;

static uint64_t scan_ms_get(void)
{
    uint64_t ns = 0x0;

    if (ldg_time_monotonic_ns_get(&ns) != LDG_ERR_AOK) { return UINT64_MAX; }

    return ns / LDG_NS_PER_MS;
}

uint32_t libresteam_ext_repl_scan_init(libresteam_ext_repl_ctx_t *ctx, uint64_t excluded_addr, uint64_t excluded_len)
{
    scan_ctx_t *scan = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_alloc(sizeof(*scan), &ctx->scan);
    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_zero(ctx->scan, sizeof(*scan)); }

    if (res != LDG_ERR_AOK) { return res; }

    scan = (scan_ctx_t *)ctx->scan;
    scan->excluded_addr = excluded_addr;
    scan->excluded_len = excluded_len;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_scan_shutdown(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->scan) { return LDG_ERR_AOK; }

    res = ldg_mem_dealloc(ctx->scan);
    if (res == LDG_ERR_AOK) { ctx->scan = 0x0; }

    return res;
}

static uint8_t scan_overlap_is(uint64_t addr, uint64_t len, uint64_t other, uint64_t size)
{
    return addr < other + size && other < addr + len ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

uint8_t libresteam_ext_repl_scan_owned_is(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint64_t len)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (len > UINT64_MAX - addr) { return LDG_ERR_RANGE; }

    return ctx->scan ? scan_overlap_is(addr, len, (uintptr_t)ctx->scan, sizeof(scan_ctx_t)) : LDG_TRUTH_FALSE;
}

static uint32_t scan_bits_rd(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint64_t addr, uint64_t *bits, uint64_t *next)
{
    ldg_mem_region_t region = LDG_STRUCT_ZERO_INIT;
    uint64_t end = 0x0;
    uint64_t begin = 0x0;
    uint64_t got = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *bits = 0;
    *next = addr + scan->stride;
    if (addr < scan->page_addr || addr + scan->bytes > scan->page_end)
    {
        scan->page_end = 0;
        if (ldg_mem_region_get(&ctx->proc, addr, &region) != LDG_ERR_AOK) { *next = scan->end; return LDG_ERR_IO_RD; }

        if (region.size > UINT64_MAX - region.base) { return LDG_ERR_OVERFLOW; }

        end = region.base + region.size;
        if (end <= addr) { return LDG_ERR_INVALID; }

        *next = end;
        if (!region.committed || region.guard || !region.readable) { return LDG_ERR_DENIED; }

        begin = addr;
        if (end - begin > SCAN_PAGE_BYTES) { end = begin + SCAN_PAGE_BYTES; }

        if (end > scan->end) { end = scan->end; }

        *next = end;
        if (end - begin < scan->bytes) { return LDG_ERR_IO_RD; }

        if (scan_overlap_is(begin, end - begin, (uintptr_t)scan, sizeof(*scan)) == LDG_TRUTH_TRUE || scan_overlap_is(begin, end - begin, (uintptr_t)ctx, sizeof(*ctx)) == LDG_TRUTH_TRUE || scan_overlap_is(begin, end - begin, scan->excluded_addr, scan->excluded_len) == LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

        if (ldg_mem_vmem_rd(&ctx->proc, begin, scan->page, end - begin, &got) != LDG_ERR_AOK || got != end - begin) { return LDG_ERR_IO_RD; }

        scan->page_addr = begin;
        scan->page_end = end;
        scan->page_alloc = region.alloc_base;
        scan->page_kind = region.kind;
        scan->page_ms = scan_ms_get();
    }

    res = ldg_mem_secure_cpy(bits, scan->page + (addr - scan->page_addr), scan->bytes);
    return res;
}

static uint32_t scan_env_set(scan_ctx_t *scan, const scan_item_t *item, uint64_t bits)
{
    uint32_t res = LDG_ERR_AOK;
    uint64_t origin = scan->mode == SCAN_WATCH || scan->mode == SCAN_FIT ? scan->start_ms : item->base_ms;

    res = ldg_mem_secure_zero(&scan->env, sizeof(scan->env));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_decode(bits, scan->bytes, scan->fp, scan->signed_val, &scan->env.vars[0]); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_decode(item->prev, scan->bytes, scan->fp, scan->signed_val, &scan->env.vars[1]); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_decode(item->base, scan->bytes, scan->fp, scan->signed_val, &scan->env.vars[2]); }

    scan->env.vars[4].fp = LDG_TRUTH_TRUE;
    scan->env.vars[4].real = (double)(scan->page_ms - origin) / LDG_MS_PER_SEC;
    scan->env.vars[5].fp = LDG_TRUTH_TRUE;
    scan->env.vars[5].real = (double)(scan->page_ms - item->prev_ms) / LDG_MS_PER_SEC;
    scan->env.vars[6].mag = scan->pass;
    return res;
}

static void scan_best_add(scan_ctx_t *scan, uint32_t idx)
{
    uint32_t pos = 0x0;
    uint32_t move = 0x0;

    for (pos = 0; pos < scan->best_cunt; pos++) { if (scan->items[idx].score < scan->items[scan->best[pos]].score) { break; } }
    if (pos == SCAN_BEST_CAP) { return; }

    if (scan->best_cunt < SCAN_BEST_CAP) { scan->best_cunt++; }

    for (move = scan->best_cunt - 1U; move > pos; move--) { scan->best[move] = scan->best[move - 1U]; }
    scan->best[pos] = idx;
}

static uint32_t scan_base_step(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan)
{
    scan_item_t item = LDG_STRUCT_ZERO_INIT;
    libresteam_ext_repl_expr_val_t val = LDG_STRUCT_ZERO_INIT;
    uint64_t bits = 0x0;
    uint64_t next = 0x0;
    uint64_t steps = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (scan->end - scan->cursor < scan->bytes) { scan->cursor = scan->end; return LDG_ERR_AOK; }

    res = scan_bits_rd(ctx, scan, scan->cursor, &bits, &next);
    if (res != LDG_ERR_AOK)
    {
        if (next <= scan->cursor) { return LDG_ERR_INVALID; }

        if (next > scan->end) { next = scan->end; }

        scan->skipped += next - scan->cursor;
        steps = (next - scan->cursor + scan->stride - 1U) / scan->stride;
        scan->cursor = steps > (scan->end - scan->cursor) / scan->stride ? scan->end : scan->cursor + steps * scan->stride;
        return LDG_ERR_AOK;
    }

    item.addr = scan->cursor;
    item.alloc = scan->page_alloc;
    item.kind = scan->page_kind;
    item.base = bits;
    item.prev = bits;
    item.base_ms = scan->page_ms;
    item.prev_ms = scan->page_ms;
    item.alive = LDG_TRUTH_TRUE;
    res = scan_env_set(scan, &item, bits);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_eval(&scan->expr, &scan->env, &val); }

    if (res != LDG_ERR_AOK) { scan->errors++; }
    else if (libresteam_ext_repl_expr_truth_is(&val) == LDG_TRUTH_TRUE)
    {
        if (scan->cunt == SCAN_CANDIDATE_CAP) { return LDG_ERR_FULL; }

        scan->items[scan->cunt++] = item;
        scan->alive++;
    }

    scan->cursor = scan->stride > scan->end - scan->cursor ? scan->end : scan->cursor + scan->stride;
    return LDG_ERR_AOK;
}

static uint32_t scan_sample_step(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan)
{
    scan_item_t *item = &scan->items[scan->idx];
    libresteam_ext_repl_expr_val_t val = LDG_STRUCT_ZERO_INIT;
    uint64_t bits = 0x0;
    uint64_t next = 0x0;
    double score = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!item->alive) { scan->idx++; return LDG_ERR_AOK; }

    res = scan_bits_rd(ctx, scan, item->addr, &bits, &next);
    if (res != LDG_ERR_AOK || item->alloc != scan->page_alloc || item->kind != scan->page_kind)
    {
        item->alive = LDG_TRUTH_FALSE;
        scan->alive--;
        scan->lost++;
        scan->idx++;
        return LDG_ERR_AOK;
    }

    res = scan_env_set(scan, item, bits);
    if (res == LDG_ERR_AOK && scan->mode != SCAN_REBASE) { res = libresteam_ext_repl_expr_eval(&scan->expr, &scan->env, &val); }

    if (res == LDG_ERR_AOK && scan->mode == SCAN_FIT)
    {
        res = libresteam_ext_repl_expr_real_get(&val, &score);
        if (res == LDG_ERR_AOK)
        {
            item->samples++;
            score = fabs(score);
            item->score += (score - item->score) / item->samples;
            if (!isfinite(item->score)) { res = LDG_ERR_RANGE; }
        }
    }

    if (res != LDG_ERR_AOK)
    {
        scan->errors++;
        item->alive = LDG_TRUTH_FALSE;
    }
    else if (scan->mode != SCAN_FIT && scan->mode != SCAN_REBASE && libresteam_ext_repl_expr_truth_is(&val) != LDG_TRUTH_TRUE) { item->alive = LDG_TRUTH_FALSE; }

    if (!item->alive) { scan->alive--; }

    if (scan->page_ms - item->prev_ms > scan->max_dt_ms) { scan->max_dt_ms = scan->page_ms - item->prev_ms; }

    item->prev = bits;
    item->prev_ms = scan->page_ms;
    if (scan->mode == SCAN_REBASE)
    {
        item->base = bits;
        item->base_ms = scan->page_ms;
        item->score = 0;
        item->samples = 0;
    }

    if (scan->mode == SCAN_FIT && scan->pass == scan->passes && item->alive) { scan_best_add(scan, scan->idx); }

    scan->idx++;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_scan_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    scan_ctx_t *scan = 0x0;
    uint64_t now = scan_ms_get();
    uint32_t step = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->scan)) { return LDG_ERR_NOT_INIT; }

    scan = (scan_ctx_t *)ctx->scan;
    if (scan->mode == SCAN_IDLE || now < scan->next_ms) { return scan->res; }

    if (!scan->idx && scan->mode != SCAN_BASE) { scan->pass_ms = now; }

    scan->page_end = 0;
    for (step = 0; step < SCAN_STEP_CAP; step++)
    {
        res = scan->mode == SCAN_BASE ? scan_base_step(ctx, scan) : scan_sample_step(ctx, scan);
        if (res != LDG_ERR_AOK || (scan->mode == SCAN_BASE ? scan->cursor == scan->end : scan->idx == scan->cunt))
        {
            if (scan->mode == SCAN_BASE) { scan->ready = LDG_TRUTH_TRUE; }

            scan->pass_span_ms = scan_ms_get() - scan->pass_ms;
            if (res != LDG_ERR_AOK || scan->mode == SCAN_BASE || scan->pass >= scan->passes || !scan->alive)
            {
                scan->res = res;
                scan->ranking = scan->mode == SCAN_FIT && res == LDG_ERR_AOK ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
                scan->mode = SCAN_IDLE;
                return res;
            }

            scan->idx = 0;
            scan->pass++;
            scan->next_ms = scan->pass_ms + scan->interval_ms;
            return LDG_ERR_AOK;
        }

        if (scan_ms_get() - now >= SCAN_TIME_BUDGET_MS) { break; }
    }
    return LDG_ERR_AOK;
}

static uint32_t scan_status(libresteam_ext_repl_ctx_t *ctx, const scan_ctx_t *scan)
{
    return libresteam_ext_repl_emit(ctx, "scan; active: %u32; ready: %u32; mode: %u32; candidates: %u32; retained: %u32; sample: %u32/%u32; cursor: 0x%x64; remaining bytes: %u64; skipped bytes: %u64; expression errors: %u32; unreadable/remapped: %u32; pass span ms: %u64; max dt ms: %u64; ranked: %u32; res: %u32", (uint32_t)(scan->mode != SCAN_IDLE), (uint32_t)scan->ready, scan->mode, scan->cunt, scan->alive, scan->pass, scan->passes, scan->cursor, scan->end - scan->cursor, scan->skipped, scan->errors, scan->lost, scan->pass_span_ms, scan->max_dt_ms, (uint32_t)scan->ranking, scan->res);
}

static uint32_t scan_expr_compile(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint32_t argc, char **args, uint32_t start)
{
    uint32_t idx = 0x0;
    uint32_t pos = 0x0;
    uint32_t ch = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (idx = start; idx < argc; idx++)
    {
        if (idx != start)
        {
            if (pos + LDG_STR_TERM_SIZE >= sizeof(scan->expr_text)) { return LDG_ERR_FULL; }

            scan->expr_text[pos++] = ' ';
        }

        for (ch = 0; args[idx][ch]; ch++)
        {
            if (pos + LDG_STR_TERM_SIZE >= sizeof(scan->expr_text)) { return LDG_ERR_FULL; }

            scan->expr_text[pos++] = args[idx][ch];
        }
    }
    if (start == argc) { scan->expr_text[pos++] = '1'; }

    scan->expr_text[pos] = LDG_STR_TERM;
    res = libresteam_ext_repl_expr_compile(&scan->pending, scan->expr_text);
    if (res != LDG_ERR_AOK)
    {
        if (libresteam_ext_repl_emit(ctx, "expression rejected; offset: %u32; res: %u32", scan->pending.err_pos, res) != LDG_ERR_AOK) { return LDG_ERR_FULL; }

        return res;
    }

    return LDG_ERR_AOK;
}

static uint32_t scan_type_get(const char *text, uint32_t *bytes, uint8_t *fp, uint8_t *signed_val)
{
    *fp = text[0] == 'f' ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    *signed_val = text[0] == 'i' ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    if (!*fp && !*signed_val && text[0] != 'u') { return LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(text + 1, "8") == LDG_TRUTH_TRUE && !*fp) { *bytes = 1U; }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1, "16") == LDG_TRUTH_TRUE && !*fp) { *bytes = 2U; }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1, "32") == LDG_TRUTH_TRUE) { *bytes = 4U; }
    else if (libresteam_ext_repl_cmd_eq_is(text + 1, "64") == LDG_TRUTH_TRUE) { *bytes = 8U; }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t scan_begin(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint32_t argc, char **args)
{
    uint64_t addr = 0x0;
    uint64_t len = 0x0;
    uint64_t stride = 0x0;
    uint32_t bytes = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t fp = LDG_TRUTH_FALSE;
    uint8_t signed_val = LDG_TRUTH_FALSE;

    if (argc < 6U) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = scan_type_get(args[2], &bytes, &fp, &signed_val);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_addr_get(ctx, args[3], &addr); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[4], &len); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[5], &stride); }

    if (res != LDG_ERR_AOK) { return res; }

    if (len < bytes || len > SCAN_RANGE_CAP || len > UINTPTR_MAX - addr || !stride || stride > SCAN_PAGE_BYTES) { return LDG_ERR_RANGE; }

    res = scan_expr_compile(ctx, scan, argc, args, 6U);
    if (res != LDG_ERR_AOK) { return res; }

    scan->expr = scan->pending;
    scan->cursor = addr;
    scan->end = addr + len;
    scan->stride = (uint32_t)stride;
    scan->bytes = bytes;
    scan->fp = fp;
    scan->signed_val = signed_val;
    scan->cunt = 0;
    scan->alive = 0;
    scan->idx = 0;
    scan->pass = 0;
    scan->passes = 0;
    scan->errors = 0;
    scan->lost = 0;
    scan->skipped = 0;
    scan->max_dt_ms = 0;
    scan->pass_span_ms = 0;
    scan->res = LDG_ERR_AOK;
    scan->ready = LDG_TRUTH_FALSE;
    scan->ranking = LDG_TRUTH_FALSE;
    scan->best_cunt = 0;
    scan->start_ms = scan_ms_get();
    scan->next_ms = scan->start_ms;
    scan->pass_ms = scan->start_ms;
    scan->mode = SCAN_BASE;
    return scan_status(ctx, scan);
}

static uint32_t scan_run(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint32_t argc, char **args, uint32_t mode)
{
    uint64_t interval = 0x0;
    uint64_t passes = 1U;
    uint32_t start = SCAN_LAST_EXPR_ARG;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!scan->ready || !scan->alive) { return LDG_ERR_EMPTY; }

    if (mode == SCAN_REBASE && argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (mode == SCAN_WATCH || mode == SCAN_FIT)
    {
        if (argc < 5U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_num_get(args[2], &interval);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[3], &passes); }

        if (res != LDG_ERR_AOK) { return res; }

        if (interval < SCAN_INTERVAL_MIN_MS || interval > SCAN_INTERVAL_MAX_MS || !passes || passes > SCAN_SAMPLE_CAP) { return LDG_ERR_RANGE; }

        start = 4U;
    }

    if (mode != SCAN_REBASE && argc == start) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = scan_expr_compile(ctx, scan, argc, args, start);
    if (res != LDG_ERR_AOK) { return res; }

    scan->expr = scan->pending;
    scan->idx = 0;
    scan->pass = 1U;
    scan->passes = (uint32_t)passes;
    scan->interval_ms = (uint32_t)interval;
    scan->errors = 0;
    scan->lost = 0;
    scan->max_dt_ms = 0;
    scan->pass_span_ms = 0;
    scan->res = LDG_ERR_AOK;
    scan->ranking = LDG_TRUTH_FALSE;
    scan->best_cunt = 0;
    for (idx = 0; idx < scan->cunt; idx++) { scan->items[idx].score = 0; scan->items[idx].samples = 0; }
    scan->start_ms = scan_ms_get();
    scan->next_ms = scan->start_ms + interval;
    scan->mode = mode;
    return scan_status(ctx, scan);
}

static uint32_t scan_row_emit(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint32_t idx, uint32_t row)
{
    scan_item_t *item = &scan->items[idx];
    libresteam_ext_repl_expr_val_t val = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_expr_decode(item->prev, scan->bytes, scan->fp, scan->signed_val, &val);
    if (res != LDG_ERR_AOK) { return res; }

    if (val.fp) { return libresteam_ext_repl_emit(ctx, "candidate: %u32; slot: %u32; address: 0x%x64; value: %f; bits: 0x%x64; baseline bits: 0x%x64; mean absolute error: %f; samples: %u32", row, idx, item->addr, val.real, item->prev, item->base, item->score, item->samples); }

    return libresteam_ext_repl_emit(ctx, "candidate: %u32; slot: %u32; address: 0x%x64; value: %s%u64; bits: 0x%x64; baseline bits: 0x%x64; mean absolute error: %f; samples: %u32", row, idx, item->addr, val.neg ? "-" : "", val.mag, item->prev, item->base, item->score, item->samples);
}

static uint32_t scan_list(libresteam_ext_repl_ctx_t *ctx, scan_ctx_t *scan, uint32_t argc, char **args, uint8_t best)
{
    uint64_t start = 0x0;
    uint32_t idx = 0x0;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (argc > 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &start); }

    if (res != LDG_ERR_AOK) { return res; }

    if (start > (best ? scan->best_cunt : scan->cunt)) { return LDG_ERR_RANGE; }

    if (best && !scan->ranking) { return LDG_ERR_NOT_INIT; }

    idx = (uint32_t)start;
    while (idx < (best ? scan->best_cunt : scan->cunt) && rows < LIBRESTEAM_EXT_REPL_ROW_CAP)
    {
        if (best || scan->items[idx].alive)
        {
            res = scan_row_emit(ctx, scan, best ? scan->best[idx] : idx, idx);
            if (res != LDG_ERR_AOK) { return res; }

            rows++;
        }

        idx++;
    }
    return libresteam_ext_repl_emit(ctx, "rows: %u32; next start: %u32; retained: %u32; cached samples only", rows, idx, scan->alive);
}

static uint32_t scan_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res |= libresteam_ext_repl_emit(ctx, "scan begin <u8/u16/u32/u64/i8/i16/i32/i64/f32/f64> <address> <bytes> <stride> [predicate]");
    res |= libresteam_ext_repl_emit(ctx, "scan where <predicate> | scan watch <interval-ms> <samples> <predicate> | scan fit <interval-ms> <samples> <residual>");
    res |= libresteam_ext_repl_emit(ctx, "scan status | scan list [start-slot] | scan best [start-rank] | scan rebase | scan stop | scan clear");
    res |= libresteam_ext_repl_emit(ctx, "v=current; p=previous retained sample; b=baseline; d=v-p; t=seconds since baseline (watch/fit: job start); dt=seconds since previous sample; n=sample number, baseline=0");
    res |= libresteam_ext_repl_emit(ctx, "Expressions: decimal/hex/scientific literals, parentheses, + - * / %% ^, < <= > >= == !=, && || !, abs/sin/cos/sqrt/floor/min/max. ^ means power; / yields real. No quotes needed; omit spaces for long expressions.");
    res |= libresteam_ext_repl_emit(ctx, "Examples: scan where d==2 | scan where v<p | scan where abs(d)<0.01 | scan watch 50 40 v>=b | scan fit 50 40 (v-b)-4*t*(2-t)");
    res |= libresteam_ext_repl_emit(ctx, "Predicates retain nonzero results; watch requires every sample to pass. Fit ranks lowest mean absolute residual; use scan best. No automatic scale/time fitting.");
    res |= libresteam_ext_repl_emit(ctx, "Async jobs: status reports completion/errors; stop preserves partial changes. Capacity 32768 candidates; full retains partial baseline and reports res. Narrow range or initial predicate. Max range 256 MiB; stride starts at supplied address.");
    res |= libresteam_ext_repl_emit(ctx, "At most 256 slots/tick, 2 ms cooperative budget. Samples are not simultaneous; actual t/dt and pass span matter. 16..60000 ms requested interval, 1..4096 samples. No catch-up samples.");
    res |= libresteam_ext_repl_emit(ctx, "64-bit integer +,-,*,%% and comparisons are exact with overflow rejection. Mixed real arithmetic rejects integer magnitudes above 2^53. NaN/Inf, invalid arithmetic and unreadable/remapped candidates are discarded and counted.");
    return res;
}

uint32_t libresteam_ext_repl_scan_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    scan_ctx_t *scan = 0x0;

    if (LDG_UNLIKELY(!ctx || !ctx->scan || !args || !argc)) { return LDG_ERR_FUNC_ARG_NULL; }

    scan = (scan_ctx_t *)ctx->scan;
    if (argc == 1U) { return scan_help(ctx); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "help") == LDG_TRUTH_TRUE) { return argc == 2U ? scan_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "status") == LDG_TRUTH_TRUE) { return argc == 2U ? scan_status(ctx, scan) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "stop") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (scan->mode == SCAN_BASE) { scan->ready = LDG_TRUTH_TRUE; }

        if (scan->mode != SCAN_IDLE) { scan->res = LDG_ERR_CANCELED; }

        scan->mode = SCAN_IDLE;
        return scan_status(ctx, scan);
    }

    if (scan->mode != SCAN_IDLE) { return LDG_ERR_BUSY; }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "begin") == LDG_TRUTH_TRUE) { return scan_begin(ctx, scan, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "where") == LDG_TRUTH_TRUE) { return scan_run(ctx, scan, argc, args, SCAN_WHERE); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "watch") == LDG_TRUTH_TRUE) { return scan_run(ctx, scan, argc, args, SCAN_WATCH); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "fit") == LDG_TRUTH_TRUE) { return scan_run(ctx, scan, argc, args, SCAN_FIT); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "rebase") == LDG_TRUTH_TRUE) { return scan_run(ctx, scan, argc, args, SCAN_REBASE); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "list") == LDG_TRUTH_TRUE) { return scan_list(ctx, scan, argc, args, LDG_TRUTH_FALSE); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "best") == LDG_TRUTH_TRUE) { return scan_list(ctx, scan, argc, args, LDG_TRUTH_TRUE); }

    if (libresteam_ext_repl_cmd_eq_is(args[1], "clear") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        scan->cunt = 0;
        scan->alive = 0;
        scan->ready = LDG_TRUTH_FALSE;
        scan->ranking = LDG_TRUTH_FALSE;
        scan->best_cunt = 0;
        scan->cursor = 0;
        scan->end = 0;
        scan->idx = 0;
        scan->pass = 0;
        scan->passes = 0;
        scan->skipped = 0;
        scan->errors = 0;
        scan->lost = 0;
        scan->pass_span_ms = 0;
        scan->max_dt_ms = 0;
        scan->res = LDG_ERR_AOK;
        return scan_status(ctx, scan);
    }

    return LDG_ERR_FUNC_ARG_INVALID;
}
