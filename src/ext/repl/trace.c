#include <stdint.h>

#include <dangling/arch/amd64/atomic.h>
#include <dangling/core/bits.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/region.h>
#include <dangling/mem/secure.h>
#include <dangling/mem/vmem.h>
#include <dangling/str/str.h>
#include <dangling/sys/mod.h>
#include <dangling/thread/ref.h>
#include <dangling/thread/trap.h>
#include <dangling/time/time.h>

#include <libresteam/ext/repl/repl.h>

#define TRACE_QUEUE_CAP 256U
#define TRACE_DWELL_MS 250U
#define TRACE_RETRY_MS 50U
#define TRACE_STACK_WORDS 16U
#define TRACE_THREAD_CAP 4096U
#define TRACE_CAPTURE_CAP 4U
#define TRACE_CAPTURE_BYTES 128U
#define TRACE_CAPTURE_STEPS 8U
#define TRACE_CAPTURE_ROW 16U
#define TRACE_HEX_COLS 3U
#define TRACE_HEX_DIGITS 2U
#define TRACE_PROBE_TIMEOUT_MS 5000U
#define TRACE_CB_HANDLED LDG_ERR_AOK
#define TRACE_CB_PASS LDG_ERR_NOT_FOUND
#define TRACE_HIT_STEP 1U
#define TRACE_ID_NONE 0U
#define TRACE_PLAN_FIXED_ARGS 2U
#define TRACE_LAST_STEP_OFF 1U
#define TRACE_ARGC_LIST 2U
#define TRACE_ARGC_ALL_OFF 3U
#define TRACE_ARGC_SLOT_MIN 4U
#define TRACE_CAPTURE_ARG_BASE 3U

typedef enum trace_reg
{
    TRACE_REG_RAX,
    TRACE_REG_RBX,
    TRACE_REG_RCX,
    TRACE_REG_RDX,
    TRACE_REG_RBP,
    TRACE_REG_RSP,
    TRACE_REG_RSI,
    TRACE_REG_RDI,
    TRACE_REG_R8,
    TRACE_REG_R9,
    TRACE_REG_R10,
    TRACE_REG_R11,
    TRACE_REG_R12,
    TRACE_REG_R13,
    TRACE_REG_R14,
    TRACE_REG_R15,
    TRACE_REG_RIP,
    TRACE_REG_CUNT
} trace_reg_t;

typedef struct trace_capture_plan
{
    uint64_t off[TRACE_CAPTURE_STEPS];
    uint8_t neg[TRACE_CAPTURE_STEPS];
    uint32_t reg;
    uint32_t len;
    uint32_t steps;
    uint8_t en;
    uint8_t pudding[3];
} trace_capture_plan_t;

typedef struct trace_capture
{
    uint64_t root;
    uint64_t addr;
    uint64_t hop[TRACE_CAPTURE_STEPS];
    uint8_t data[TRACE_CAPTURE_BYTES];
    uint32_t reg;
    uint32_t len;
    uint32_t res;
    uint32_t step;
    uint32_t hops;
    uint8_t en;
    uint8_t pudding[3];
} trace_capture_t;

typedef struct trace_event
{
    uint64_t seq;
    ldg_thread_regs_t regs;
    uint64_t stack[TRACE_STACK_WORDS];
    trace_capture_t capture[TRACE_CAPTURE_CAP];
    uint32_t thread_id;
    uint32_t stack_bytes;
} trace_event_t;

typedef struct trace_ctx
{
    ldg_spsc_queue_t events;
    trace_event_t wr_event;
    trace_event_t rd_event;
    trace_capture_plan_t capture[TRACE_CAPTURE_CAP];
    const ldg_sys_proc_t *proc;
    ldg_thread_ref_t thread;
    ldg_sys_mod_t self;
    uint64_t threads[TRACE_THREAD_CAP];
    uint64_t thread_cunt;
    uint64_t thread_next;
    uint64_t auto_addr;
    uint64_t req_addr;
    uint64_t next_ms;
    uint64_t probe_deadline;
    uint64_t probe_rcx;
    uint64_t pass;
    uint64_t attempts;
    uint64_t skipped;
    uint64_t armed;
    uint64_t busy;
    uint64_t slot;
    uint64_t thread_id;
    uint64_t hit_thread_id;
    uint64_t addr;
    uint64_t hits;
    uint64_t dropped;
    uint32_t probe_res;
    uint32_t pass_threads;
    uint32_t attempted_thread;
    uint32_t auto_res;
    uint32_t thread_err;
    uint32_t init_res;
    uint8_t probe;
    uint8_t cycling;
    uint8_t listing;
    uint8_t suspended;
    uint8_t own;
    uint8_t thread_up;
    uint8_t self_up;
    uint8_t trapped;
    uint8_t running;
    uint8_t queue_up;
    uint8_t pudding[6];
} trace_ctx_t;

static uint64_t trace_ms_get(void)
{
    uint64_t ns = 0x0;

    if (ldg_time_monotonic_ns_get(&ns) != LDG_ERR_AOK) { return UINT64_MAX; }

    return ns / LDG_NS_PER_MS;
}

static uint8_t trace_main_is(const libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t thread = 0x0;

    if (libresteam_ext_repl_thread_get(&thread) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return thread == ctx->main_thread ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t trace_capture_rd(const trace_ctx_t *trace, uint64_t addr, void *out, uint32_t len)
{
    uint64_t got = 0x0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!addr || !len || addr > UINT64_MAX - len)) { return LDG_ERR_RANGE; }

    if (ldg_mem_vmem_rd(trace->proc, addr, out, len, &got) != LDG_ERR_AOK || got != len) { return LDG_ERR_IO_RD; }

    return LDG_ERR_AOK;
}

static uint32_t trace_capture_reg_get(const ldg_thread_regs_t *regs, uint32_t reg, uint64_t *out)
{
    *out = 0;
    switch (reg)
    {
        case TRACE_REG_RAX:
            *out = regs->gpr[LDG_THREAD_REG_RAX];
            break;
        case TRACE_REG_RBX:
            *out = regs->gpr[LDG_THREAD_REG_RBX];
            break;
        case TRACE_REG_RCX:
            *out = regs->gpr[LDG_THREAD_REG_RCX];
            break;
        case TRACE_REG_RDX:
            *out = regs->gpr[LDG_THREAD_REG_RDX];
            break;
        case TRACE_REG_RBP:
            *out = regs->gpr[LDG_THREAD_REG_RBP];
            break;
        case TRACE_REG_RSP:
            *out = regs->gpr[LDG_THREAD_REG_RSP];
            break;
        case TRACE_REG_RSI:
            *out = regs->gpr[LDG_THREAD_REG_RSI];
            break;
        case TRACE_REG_RDI:
            *out = regs->gpr[LDG_THREAD_REG_RDI];
            break;
        case TRACE_REG_R8:
            *out = regs->gpr[LDG_THREAD_REG_R8];
            break;
        case TRACE_REG_R9:
            *out = regs->gpr[LDG_THREAD_REG_R9];
            break;
        case TRACE_REG_R10:
            *out = regs->gpr[LDG_THREAD_REG_R10];
            break;
        case TRACE_REG_R11:
            *out = regs->gpr[LDG_THREAD_REG_R11];
            break;
        case TRACE_REG_R12:
            *out = regs->gpr[LDG_THREAD_REG_R12];
            break;
        case TRACE_REG_R13:
            *out = regs->gpr[LDG_THREAD_REG_R13];
            break;
        case TRACE_REG_R14:
            *out = regs->gpr[LDG_THREAD_REG_R14];
            break;
        case TRACE_REG_R15:
            *out = regs->gpr[LDG_THREAD_REG_R15];
            break;
        case TRACE_REG_RIP:
            *out = regs->rip;
            break;
        default:
            return LDG_ERR_FUNC_ARG_INVALID;
    }
    return LDG_ERR_AOK;
}

static uint32_t trace_reg_find(const char *name, uint32_t *out)
{
    *out = TRACE_REG_CUNT;
    if (libresteam_ext_repl_cmd_eq_is(name, "rax") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RAX; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rbx") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RBX; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rcx") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RCX; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rdx") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RDX; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rbp") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RBP; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rsp") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RSP; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rsi") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RSI; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rdi") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RDI; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r8") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R8; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r9") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R9; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r10") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R10; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r11") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R11; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r12") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R12; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r13") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R13; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r14") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R14; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "r15") == LDG_TRUTH_TRUE) { *out = TRACE_REG_R15; }
    else if (libresteam_ext_repl_cmd_eq_is(name, "rip") == LDG_TRUTH_TRUE) { *out = TRACE_REG_RIP; }
    else { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t trace_capture_parse(uint32_t argc, char **args, trace_capture_plan_t *out)
{
    const char *text = 0x0;
    uint64_t len = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!args || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_secure_zero(out, sizeof(*out));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (LDG_UNLIKELY(argc <= TRACE_PLAN_FIXED_ARGS || argc > TRACE_CAPTURE_STEPS + TRACE_PLAN_FIXED_ARGS)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = trace_reg_find(args[0], &out->reg);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[1], &len); }

    if (res != LDG_ERR_AOK) { return res; }

    if (!len || len > TRACE_CAPTURE_BYTES) { return LDG_ERR_RANGE; }

    out->len = (uint32_t)len;
    out->steps = argc - TRACE_PLAN_FIXED_ARGS;
    for (idx = 0; idx < out->steps; idx++)
    {
        text = args[idx + TRACE_PLAN_FIXED_ARGS];
        if (!text) { return LDG_ERR_FUNC_ARG_NULL; }

        out->neg[idx] = *text == '-' ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
        if (*text == '-' || *text == '+') { text++; }

        res = libresteam_ext_repl_num_get(text, &out->off[idx]);
        if (res != LDG_ERR_AOK) { return res; }
    }
    out->en = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t trace_capture_take(const trace_ctx_t *trace, const trace_capture_plan_t *plan, const ldg_thread_regs_t *regs, trace_capture_t *out)
{
    uint64_t val = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    out->en = plan->en;
    if (!plan->en) { return LDG_ERR_AOK; }

    out->reg = plan->reg;
    res = trace_capture_reg_get(regs, plan->reg, &val);
    if (res != LDG_ERR_AOK) { return res; }

    out->root = val;
    for (idx = 0; idx < plan->steps; idx++)
    {
        out->step = idx;
        out->addr = val;
        if (!val) { return LDG_ERR_NOT_FOUND; }

        if (plan->neg[idx])
        {
            if (val < plan->off[idx]) { return LDG_ERR_OVERFLOW; }

            val -= plan->off[idx];
        }
        else
        {
            if (val > UINT64_MAX - plan->off[idx]) { return LDG_ERR_OVERFLOW; }

            val += plan->off[idx];
        }

        out->addr = val;
        if (idx + TRACE_LAST_STEP_OFF == plan->steps)
        {
            res = trace_capture_rd(trace, val, out->data, plan->len);
            if (res == LDG_ERR_AOK) { out->len = plan->len; }

            return res;
        }

        res = trace_capture_rd(trace, val, &val, sizeof(val));
        if (res != LDG_ERR_AOK) { return res; }

        out->hop[out->hops++] = val;
    }
    return LDG_ERR_FUNC_ARG_INVALID;
}

static uint32_t trace_capture_emit(libresteam_ext_repl_ctx_t *ctx, uint32_t slot, const trace_capture_t *capture)
{
    char hex[TRACE_CAPTURE_ROW * TRACE_HEX_COLS + LDG_STR_TERM_SIZE] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0x0;
    uint32_t row = 0x0;
    uint32_t pos = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!capture->en) { return LDG_ERR_AOK; }

    res = libresteam_ext_repl_emit(ctx, "capture: %u32; root: 0x%x64; address: 0x%x64; bytes: %u32; step: %u32; res: %u32", slot, capture->root, capture->addr, capture->len, capture->step, capture->res);
    for (idx = 0; res == LDG_ERR_AOK && idx < capture->hops; idx++) { res = libresteam_ext_repl_emit(ctx, "capture: %u32; hop: %u32; pointer: 0x%x64", slot, idx, capture->hop[idx]); }
    if (res != LDG_ERR_AOK) { return res; }

    idx = 0;
    while (idx < capture->len)
    {
        row = idx;
        pos = 0;
        while (idx < capture->len && idx - row < TRACE_CAPTURE_ROW)
        {
            res = ldg_byte_to_hex(capture->data[idx++], hex + pos);
            if (res != LDG_ERR_AOK) { return res; }

            pos += TRACE_HEX_DIGITS;
            hex[pos++] = ' ';
        }
        hex[pos] = LDG_STR_TERM;
        res = libresteam_ext_repl_emit(ctx, "0x%x64: %s", capture->addr + row, hex);
        if (res != LDG_ERR_AOK) { return res; }
    }
    return LDG_ERR_AOK;
}

static uint32_t trace_trap_cb(ldg_thread_trap_kind_t kind, uint64_t thread_id, uint32_t slot, ldg_thread_regs_t *regs, void *user)
{
    trace_ctx_t *trace = (trace_ctx_t *)user;
    trace_event_t *event = 0x0;
    uint64_t seq = 0x0;
    uint64_t got = 0x0;
    uint64_t idle = LDG_TRUTH_FALSE;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (!trace || !regs || kind != LDG_THREAD_TRAP_BREAK) { return TRACE_CB_PASS; }

    if (thread_id != LDG_LD_ACQUIRE(trace->thread_id) || LDG_LD_ACQUIRE(trace->armed) == LDG_TRUTH_FALSE) { return TRACE_CB_PASS; }

    if (slot != LDG_LD_ACQUIRE(trace->slot) || regs->rip != LDG_LD_ACQUIRE(trace->addr)) { return TRACE_CB_PASS; }

    regs->rflags |= LDG_THREAD_TRAP_RFLAGS_RESUME;
    if (trace->probe)
    {
        if (trace->probe_res == LDG_ERR_AGAIN)
        {
            trace->probe_rcx = regs->gpr[LDG_THREAD_REG_RCX];
            trace->probe_res = LDG_ERR_AOK;
        }

        return TRACE_CB_HANDLED;
    }

    LDG_ST_RELEASE(trace->hit_thread_id, thread_id);
    seq = LDG_ADD_FETCH(trace->hits, TRACE_HIT_STEP);
    if (!LDG_CAS(&trace->busy, &idle, LDG_TRUTH_TRUE))
    {
        seq = LDG_ADD_FETCH(trace->dropped, TRACE_HIT_STEP);
        return TRACE_CB_HANDLED;
    }

    event = &trace->wr_event;
    res = ldg_spsc_full_is(&trace->events);
    if (res == LDG_TRUTH_FALSE) { res = ldg_mem_secure_zero(event, sizeof(*event)); }
    else if (res == LDG_TRUTH_TRUE) { res = LDG_ERR_FULL; }

    if (res == LDG_ERR_AOK)
    {
        event->seq = seq;
        event->thread_id = (uint32_t)thread_id;
        event->regs = *regs;
        res = ldg_mem_vmem_rd(trace->proc, regs->gpr[LDG_THREAD_REG_RSP], event->stack, sizeof(event->stack), &got);
        if (got <= sizeof(event->stack)) { event->stack_bytes = (uint32_t)got; }

        for (idx = 0; idx < TRACE_CAPTURE_CAP; idx++) { event->capture[idx].res = trace_capture_take(trace, &trace->capture[idx], regs, &event->capture[idx]); }
        res = ldg_spsc_push(&trace->events, event);
    }

    if (res != LDG_ERR_AOK) { seq = LDG_ADD_FETCH(trace->dropped, TRACE_HIT_STEP); }

    LDG_ST_RELEASE(trace->busy, LDG_TRUTH_FALSE);
    return TRACE_CB_HANDLED;
}

static uint32_t trace_thread_release(trace_ctx_t *trace)
{
    uint32_t res = LDG_ERR_AOK;

    if (!trace->thread_up) { return LDG_ERR_AOK; }

    if (trace->suspended)
    {
        res = ldg_thread_ref_resume(&trace->thread);
        if (res != LDG_ERR_AOK)
        {
            trace->thread_err = res;
            return LDG_ERR_DENIED;
        }

        trace->suspended = LDG_TRUTH_FALSE;
    }

    if (LDG_LD_ACQUIRE(trace->armed) == LDG_TRUTH_FALSE)
    {
        res = ldg_thread_ref_close(&trace->thread);
        if (res != LDG_ERR_AOK)
        {
            trace->thread_err = res;
            return LDG_ERR_IO_CLOSE;
        }

        trace->thread_up = LDG_TRUTH_FALSE;
    }

    return LDG_ERR_AOK;
}

static uint32_t trace_thread_req(trace_ctx_t *trace, uint8_t arm)
{
    uint32_t slot = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t cleanup = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!trace->thread_up)) { return LDG_ERR_NOT_INIT; }

    trace->thread_err = LDG_ERR_AOK;
    if (ldg_thread_ref_exited_is(&trace->thread) == LDG_TRUTH_TRUE)
    {
        trace->suspended = LDG_TRUTH_FALSE;
        LDG_ST_RELEASE(trace->armed, LDG_TRUTH_FALSE);
        LDG_ST_RELEASE(trace->busy, LDG_TRUTH_FALSE);
        res = trace_thread_release(trace);
        return res == LDG_ERR_AOK && arm ? LDG_ERR_CLOSED : res;
    }

    if (!arm && LDG_LD_ACQUIRE(trace->armed) == LDG_TRUTH_FALSE) { return trace_thread_release(trace); }

    if (!trace->own && !trace->suspended)
    {
        res = ldg_thread_ref_suspend(&trace->thread);
        if (res != LDG_ERR_AOK)
        {
            trace->thread_err = res;
            cleanup = trace_thread_release(trace);
            return cleanup == LDG_ERR_AOK ? LDG_ERR_DENIED : cleanup;
        }

        trace->suspended = LDG_TRUTH_TRUE;
    }

    if (LDG_LD_ACQUIRE(trace->busy) != LDG_TRUTH_FALSE) { res = LDG_ERR_BUSY; }
    else if (arm)
    {
        LDG_ST_RELEASE(trace->addr, trace->req_addr);
        res = LDG_ERR_FULL;
        for (slot = 0; slot < LDG_THREAD_TRAP_SLOT_MAX; slot++)
        {
            LDG_ST_RELEASE(trace->slot, slot);
            LDG_ST_RELEASE(trace->armed, LDG_TRUTH_TRUE);
            res = ldg_thread_trap_break_arm(&trace->thread, slot, trace->req_addr);
            if (res == LDG_ERR_AOK) { break; }

            LDG_ST_RELEASE(trace->armed, LDG_TRUTH_FALSE);
        }
        if (res != LDG_ERR_AOK)
        {
            LDG_ST_RELEASE(trace->slot, LDG_THREAD_TRAP_SLOT_NONE);
            trace->thread_err = res;
        }
    }
    else
    {
        res = ldg_thread_trap_break_disarm(&trace->thread, (uint32_t)LDG_LD_ACQUIRE(trace->slot));
        if (res == LDG_ERR_AOK) { LDG_ST_RELEASE(trace->armed, LDG_TRUTH_FALSE); }
        else { trace->thread_err = res; }
    }

    cleanup = trace_thread_release(trace);
    return cleanup == LDG_ERR_AOK ? res : cleanup;
}

static uint32_t trace_stop(trace_ctx_t *trace)
{
    return trace->thread_up ? trace_thread_req(trace, LDG_TRUTH_FALSE) : LDG_ERR_AOK;
}

static uint32_t trace_cancel(trace_ctx_t *trace)
{
    uint32_t res = LDG_ERR_AOK;

    trace->cycling = LDG_TRUTH_FALSE;
    trace->listing = LDG_TRUTH_FALSE;
    res = trace_stop(trace);
    if (res == LDG_ERR_AOK) { trace->probe = LDG_TRUTH_FALSE; }

    return res;
}

static uint32_t trace_trap_init(libresteam_ext_repl_ctx_t *ctx, trace_ctx_t *trace)
{
    ldg_thread_trap_cb_t cb = trace_trap_cb;
    uintptr_t func_addr = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_secure_cpy(&func_addr, &cb, sizeof(func_addr));
    if (res == LDG_ERR_AOK) { res = ldg_sys_mod_addr_find(&ctx->proc, func_addr, &trace->self); }

    if (res == LDG_ERR_AOK) { res = ldg_sys_mod_pin(&ctx->proc, &trace->self); }

    if (res != LDG_ERR_AOK) { return res; }

    trace->self_up = LDG_TRUTH_TRUE;
    res = ldg_thread_trap_init(trace_trap_cb, trace);
    if (res == LDG_ERR_AOK) { trace->trapped = LDG_TRUTH_TRUE; }

    return res;
}

uint32_t libresteam_ext_repl_trace_init(libresteam_ext_repl_ctx_t *ctx)
{
    trace_ctx_t *trace = 0x0;
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_alloc(sizeof(*trace), &mem);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_mem_secure_zero(mem, sizeof(*trace));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK))
    {
        res = ldg_mem_dealloc(mem);
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    trace = (trace_ctx_t *)mem;
    ctx->trace = trace;
    trace->proc = &ctx->proc;
    trace->slot = LDG_THREAD_TRAP_SLOT_NONE;
    res = ldg_spsc_init(&trace->events, sizeof(trace_event_t), TRACE_QUEUE_CAP);
    if (res == LDG_ERR_AOK)
    {
        trace->queue_up = LDG_TRUTH_TRUE;
        res = trace_trap_init(ctx, trace);
    }

    if (res == LDG_ERR_AOK) { trace->running = LDG_TRUTH_TRUE; }

    trace->init_res = res;
    return res;
}

uint32_t libresteam_ext_repl_trace_shutdown(libresteam_ext_repl_ctx_t *ctx)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace) { return LDG_ERR_AOK; }

    res = trace_cancel(trace);
    if (res != LDG_ERR_AOK) { return res; }

    if (trace->running)
    {
        res = ldg_thread_trap_shutdown();
        if (res != LDG_ERR_AOK) { return res; }

        trace->running = LDG_TRUTH_FALSE;
    }

    if (!trace->trapped)
    {
        if (trace->queue_up) { res = ldg_spsc_shutdown(&trace->events); }

        if (res == LDG_ERR_AOK) { trace->queue_up = LDG_TRUTH_FALSE; }

        if (res == LDG_ERR_AOK) { res = ldg_mem_dealloc(trace); }

        if (res != LDG_ERR_AOK) { return res; }
    }

    ctx->trace = 0x0;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_trace_status(libresteam_ext_repl_ctx_t *ctx)
{
    trace_ctx_t *trace = 0x0;
    uint64_t armed = 0x0;
    uint64_t cur = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace) { return libresteam_ext_repl_emit(ctx, "trace available: %u32", (uint32_t)LDG_TRUTH_FALSE); }

    armed = LDG_LD_ACQUIRE(trace->armed);
    if (armed != LDG_TRUTH_FALSE) { cur = LDG_LD_ACQUIRE(trace->thread_id); }

    res = libresteam_ext_repl_emit(ctx, "trace available: %u32; init res: %u32; armed: %u32; thread: %u32; slot: %u32; address: 0x%x64; hits: %u64; dropped: %u64; control pending: %u32; resume pending: %u32; thread error: %u32", (uint32_t)trace->running, trace->init_res, (uint32_t)armed, (uint32_t)LDG_LD_ACQUIRE(trace->thread_id), (uint32_t)LDG_LD_ACQUIRE(trace->slot), LDG_LD_ACQUIRE(trace->addr), LDG_LD_ACQUIRE(trace->hits), LDG_LD_ACQUIRE(trace->dropped), (uint32_t)LDG_TRUTH_FALSE, (uint32_t)trace->suspended, trace->thread_err);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_ext_repl_emit(ctx, "trace rotation; active: %u32; current thread: %u32; dwell ms: %u32; pass: %u64; threads visited: %u32; attempts: %u64; skipped: %u64; last attempt thread: %u32; last hit thread: %u32; res: %u32", (uint32_t)trace->cycling, (uint32_t)cur, TRACE_DWELL_MS, trace->pass, trace->pass_threads, trace->attempts, trace->skipped, trace->attempted_thread, (uint32_t)LDG_LD_ACQUIRE(trace->hit_thread_id), trace->auto_res);
}

static uint32_t trace_addr_validate(const trace_ctx_t *trace, uint64_t addr)
{
    ldg_mem_region_t region = LDG_STRUCT_ZERO_INIT;

    if (!addr || addr > UINTPTR_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (ldg_mem_region_get(trace->proc, addr, &region) != LDG_ERR_AOK) { return LDG_ERR_IO_RD; }

    if (!region.committed || region.guard || !region.executable) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (trace->self_up && addr >= trace->self.base && addr - trace->self.base < trace->self.size) { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t trace_arm(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint32_t thread_id)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace->running) { return trace->init_res ? trace->init_res : LDG_ERR_NOT_INIT; }

    if (!addr || addr > UINTPTR_MAX || !thread_id) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_LD_ACQUIRE(trace->armed) != LDG_TRUTH_FALSE || trace->thread_up || LDG_LD_ACQUIRE(trace->busy) != LDG_TRUTH_FALSE) { return LDG_ERR_BUSY; }

    res = trace_addr_validate(trace, addr);
    if (res != LDG_ERR_AOK) { return res; }

    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    trace->thread_err = LDG_ERR_AOK;
    res = ldg_thread_ref_open(&ctx->proc, thread_id, &trace->thread);
    if (res != LDG_ERR_AOK)
    {
        trace->thread_err = res;
        return LDG_ERR_DENIED;
    }

    trace->thread_up = LDG_TRUTH_TRUE;
    trace->own = thread_id == ctx->main_thread ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    LDG_ST_RELEASE(trace->thread_id, thread_id);
    trace->req_addr = addr;
    return trace_thread_req(trace, LDG_TRUTH_TRUE);
}

uint32_t libresteam_ext_repl_trace_probe_start(libresteam_ext_repl_ctx_t *ctx, uint64_t addr)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

    trace = ctx->trace;
    if (trace->probe || trace->cycling || trace->listing || LDG_LD_ACQUIRE(trace->armed) != LDG_TRUTH_FALSE || trace->thread_up || LDG_LD_ACQUIRE(trace->busy) != LDG_TRUTH_FALSE) { return LDG_ERR_BUSY; }

    trace->probe_rcx = 0;
    trace->probe_res = LDG_ERR_AGAIN;
    trace->probe_deadline = trace_ms_get() + TRACE_PROBE_TIMEOUT_MS;
    trace->probe = LDG_TRUTH_TRUE;
    res = trace_arm(ctx, addr, ctx->main_thread);
    if (res != LDG_ERR_AOK) { trace->probe = LDG_TRUTH_FALSE; }

    return res;
}

uint32_t libresteam_ext_repl_trace_probe_get(libresteam_ext_repl_ctx_t *ctx, uint64_t *out)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;
    if (!ctx->trace) { return LDG_ERR_NOT_INIT; }

    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_DENIED; }

    trace = ctx->trace;
    if (!trace->probe) { return LDG_ERR_NOT_INIT; }

    if (trace->probe_res == LDG_ERR_AGAIN && trace_ms_get() < trace->probe_deadline) { return LDG_ERR_AGAIN; }

    res = trace_stop(trace);
    if (res != LDG_ERR_AOK) { return res; }

    trace->probe = LDG_TRUTH_FALSE;
    if (trace->probe_res == LDG_ERR_AGAIN) { return LDG_ERR_TIMEOUT; }

    if (trace->probe_res != LDG_ERR_AOK) { return trace->probe_res; }

    if (!trace->probe_rcx) { return LDG_ERR_INVALID; }

    *out = trace->probe_rcx;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_trace_set(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint32_t thread_id)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t emit_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (trace->probe || trace->cycling || trace->listing) { return LDG_ERR_BUSY; }

    res = trace_arm(ctx, addr, thread_id);
    if (res != LDG_ERR_AOK)
    {
        emit_res = libresteam_ext_repl_emit(ctx, "trace arm failed; res: %u32; control pending: %u32; resume pending: %u32; thread error: %u32", res, (uint32_t)LDG_TRUTH_FALSE, (uint32_t)trace->suspended, trace->thread_err);
        return emit_res == LDG_ERR_AOK ? res : emit_res;
    }

    return libresteam_ext_repl_trace_status(ctx);
}

static uint32_t trace_thread_next(trace_ctx_t *trace, uint32_t *out)
{
    uint64_t id = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    if (!trace->listing)
    {
        res = ldg_thread_id_list(trace->proc, trace->threads, TRACE_THREAD_CAP, &trace->thread_cunt);
        if (res != LDG_ERR_AOK)
        {
            trace->thread_err = res;
            return LDG_ERR_DENIED;
        }

        if (trace->thread_cunt > TRACE_THREAD_CAP) { trace->thread_cunt = TRACE_THREAD_CAP; }

        trace->thread_next = 0;
        trace->listing = LDG_TRUTH_TRUE;
        trace->pass++;
        trace->pass_threads = 0;
    }

    while (trace->thread_next < trace->thread_cunt)
    {
        id = trace->threads[trace->thread_next++];
        if (!id || id > UINT32_MAX) { continue; }

        trace->pass_threads++;
        *out = (uint32_t)id;
        return LDG_ERR_AOK;
    }
    trace->listing = LDG_TRUTH_FALSE;
    return LDG_ERR_EMPTY;
}

uint32_t libresteam_ext_repl_trace_auto(libresteam_ext_repl_ctx_t *ctx, uint64_t addr)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace->running) { return trace->init_res ? trace->init_res : LDG_ERR_NOT_INIT; }

    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    if (trace->probe || trace->cycling || trace->listing || trace->thread_up || LDG_LD_ACQUIRE(trace->armed) != LDG_TRUTH_FALSE || LDG_LD_ACQUIRE(trace->busy) != LDG_TRUTH_FALSE) { return LDG_ERR_BUSY; }

    res = trace_addr_validate(trace, addr);
    if (res != LDG_ERR_AOK) { return res; }

    trace->auto_addr = addr;
    LDG_ST_RELEASE(trace->addr, addr);
    LDG_ST_RELEASE(trace->thread_id, TRACE_ID_NONE);
    trace->next_ms = 0;
    trace->pass = 0;
    trace->attempts = 0;
    trace->skipped = 0;
    trace->pass_threads = 0;
    trace->attempted_thread = 0;
    trace->auto_res = LDG_ERR_AOK;
    trace->thread_err = LDG_ERR_AOK;
    trace->cycling = LDG_TRUTH_TRUE;
    return libresteam_ext_repl_trace_status(ctx);
}

uint32_t libresteam_ext_repl_trace_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    trace_ctx_t *trace = 0x0;
    uint64_t now = trace_ms_get();
    uint32_t thread_id = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace->cycling || now < trace->next_ms) { return LDG_ERR_AOK; }

    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    res = trace_stop(trace);
    if (res != LDG_ERR_AOK)
    {
        trace->auto_res = res;
        trace->next_ms = now + TRACE_RETRY_MS;
        return res;
    }

    if (LDG_LD_ACQUIRE(trace->busy) != LDG_TRUTH_FALSE)
    {
        trace->auto_res = LDG_ERR_BUSY;
        trace->next_ms = now + TRACE_RETRY_MS;
        return LDG_ERR_BUSY;
    }

    res = trace_thread_next(trace, &thread_id);
    if (res == LDG_ERR_EMPTY)
    {
        trace->auto_res = LDG_ERR_AOK;
        return LDG_ERR_AOK;
    }

    if (res != LDG_ERR_AOK)
    {
        trace->auto_res = res;
        trace->next_ms = now + TRACE_RETRY_MS;
        return res;
    }

    trace->attempted_thread = thread_id;
    trace->attempts++;
    res = trace_arm(ctx, trace->auto_addr, thread_id);
    trace->auto_res = res;
    if (res != LDG_ERR_AOK) { trace->skipped++; }

    trace->next_ms = trace_ms_get() + (res == LDG_ERR_AOK ? TRACE_DWELL_MS : TRACE_RETRY_MS);
    return res;
}

uint32_t libresteam_ext_repl_trace_off(libresteam_ext_repl_ctx_t *ctx)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t emit_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (trace_main_is(ctx) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    res = trace_cancel(trace);
    if (res != LDG_ERR_AOK)
    {
        emit_res = libresteam_ext_repl_emit(ctx, "trace disarm failed; res: %u32; control pending: %u32; resume pending: %u32; thread error: %u32", res, (uint32_t)LDG_TRUTH_FALSE, (uint32_t)trace->suspended, trace->thread_err);
        return emit_res == LDG_ERR_AOK ? res : emit_res;
    }

    return libresteam_ext_repl_trace_status(ctx);
}

static uint8_t trace_idle_is(const libresteam_ext_repl_ctx_t *ctx, const trace_ctx_t *trace)
{
    if (trace_main_is(ctx) != LDG_TRUTH_TRUE || trace->cycling || trace->listing || trace->thread_up) { return LDG_TRUTH_FALSE; }

    return LDG_LD_ACQUIRE(trace->armed) == LDG_TRUTH_FALSE && LDG_LD_ACQUIRE(trace->busy) == LDG_TRUTH_FALSE ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

uint32_t libresteam_ext_repl_trace_clear(libresteam_ext_repl_ctx_t *ctx)
{
    trace_ctx_t *trace = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t idx = UINT32_MAX;
    uint64_t old = 0x0;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace->queue_up) { return LDG_ERR_NOT_INIT; }

    if (trace_idle_is(ctx, trace) != LDG_TRUTH_TRUE) { return LDG_ERR_BUSY; }

    for (idx = 0; idx < TRACE_QUEUE_CAP; idx++)
    {
        res = ldg_spsc_pop(&trace->events, &trace->rd_event);
        if (res == LDG_ERR_EMPTY) { break; }

        if (res != LDG_ERR_AOK) { return res; }
    }
    old = LDG_LD_ACQUIRE(trace->hits) | LDG_LD_ACQUIRE(trace->dropped) | LDG_LD_ACQUIRE(trace->hit_thread_id);
    LDG_ST_RELEASE(trace->hits, TRACE_ID_NONE);
    LDG_ST_RELEASE(trace->dropped, TRACE_ID_NONE);
    LDG_ST_RELEASE(trace->hit_thread_id, TRACE_ID_NONE);
    return libresteam_ext_repl_emit(ctx, "trace cleared; queued: %u32; totals reset: %u32", idx, (uint32_t)(old != 0));
}

uint32_t libresteam_ext_repl_trace_capture_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    trace_ctx_t *trace = 0x0;
    trace_capture_plan_t plan = LDG_STRUCT_ZERO_INIT;
    uint64_t slot = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t step = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace || !args)) { return LDG_ERR_FUNC_ARG_NULL; }

    trace = (trace_ctx_t *)ctx->trace;
    if (argc == TRACE_ARGC_LIST)
    {
        for (idx = 0; idx < TRACE_CAPTURE_CAP && res == LDG_ERR_AOK; idx++)
        {
            res = libresteam_ext_repl_emit(ctx, "capture: %u32; enabled: %u32; register: %u32; bytes: %u32; steps: %u32", idx, (uint32_t)trace->capture[idx].en, trace->capture[idx].reg, trace->capture[idx].len, trace->capture[idx].steps);
            for (step = 0; step < trace->capture[idx].steps && res == LDG_ERR_AOK; step++) { res = libresteam_ext_repl_emit(ctx, "capture: %u32; step: %u32; offset: %s0x%x64", idx, step, trace->capture[idx].neg[step] ? "-" : "+", trace->capture[idx].off[step]); }
        }
        return res;
    }

    if (trace_idle_is(ctx, trace) != LDG_TRUTH_TRUE) { return LDG_ERR_BUSY; }

    if (argc == TRACE_ARGC_ALL_OFF && libresteam_ext_repl_cmd_eq_is(args[2], "off") == LDG_TRUTH_TRUE)
    {
        res = ldg_mem_secure_zero(trace->capture, sizeof(trace->capture));
        return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "trace captures disabled") : res;
    }

    if (argc < TRACE_ARGC_SLOT_MIN) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_num_get(args[2], &slot);
    if (res != LDG_ERR_AOK) { return res; }

    if (slot >= TRACE_CAPTURE_CAP) { return LDG_ERR_RANGE; }

    if (argc == TRACE_ARGC_SLOT_MIN && libresteam_ext_repl_cmd_eq_is(args[3], "off") == LDG_TRUTH_TRUE)
    {
        res = ldg_mem_secure_zero(&trace->capture[slot], sizeof(trace->capture[slot]));
        return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "capture: %u32; enabled: %u32", (uint32_t)slot, (uint32_t)LDG_TRUTH_FALSE) : res;
    }

    res = trace_capture_parse(argc - TRACE_CAPTURE_ARG_BASE, args + TRACE_CAPTURE_ARG_BASE, &plan);
    if (res != LDG_ERR_AOK) { return res; }

    trace->capture[slot] = plan;
    return libresteam_ext_repl_emit(ctx, "capture: %u32; register: %s; bytes: %u32; steps: %u32", (uint32_t)slot, args[3], plan.len, plan.steps);
}

static uint32_t trace_event_emit(libresteam_ext_repl_ctx_t *ctx, const trace_event_t *event)
{
    const uint64_t *gpr = event->regs.gpr;
    uint64_t ret = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (event->stack_bytes >= sizeof(uint64_t)) { ret = event->stack[0]; }

    res = libresteam_ext_repl_emit(ctx, "trace hit: %u64; thread: %u32; rip: 0x%x64; rsp: 0x%x64; return: 0x%x64; stack bytes: %u32", event->seq, event->thread_id, event->regs.rip, gpr[LDG_THREAD_REG_RSP], ret, event->stack_bytes);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "rax: 0x%x64; rbx: 0x%x64; rcx: 0x%x64; rdx: 0x%x64; rbp: 0x%x64; rsi: 0x%x64; rdi: 0x%x64", gpr[LDG_THREAD_REG_RAX], gpr[LDG_THREAD_REG_RBX], gpr[LDG_THREAD_REG_RCX], gpr[LDG_THREAD_REG_RDX], gpr[LDG_THREAD_REG_RBP], gpr[LDG_THREAD_REG_RSI], gpr[LDG_THREAD_REG_RDI]); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "r8: 0x%x64; r9: 0x%x64; r10: 0x%x64; r11: 0x%x64; r12: 0x%x64; r13: 0x%x64; r14: 0x%x64; r15: 0x%x64", gpr[LDG_THREAD_REG_R8], gpr[LDG_THREAD_REG_R9], gpr[LDG_THREAD_REG_R10], gpr[LDG_THREAD_REG_R11], gpr[LDG_THREAD_REG_R12], gpr[LDG_THREAD_REG_R13], gpr[LDG_THREAD_REG_R14], gpr[LDG_THREAD_REG_R15]); }

    for (idx = 0; res == LDG_ERR_AOK && idx < event->stack_bytes / sizeof(uint64_t); idx++) { res = libresteam_ext_repl_emit(ctx, "stack[%u32]: 0x%x64", idx, event->stack[idx]); }
    for (idx = 0; res == LDG_ERR_AOK && idx < TRACE_CAPTURE_CAP; idx++) { res = trace_capture_emit(ctx, idx, &event->capture[idx]); }
    return res;
}

uint32_t libresteam_ext_repl_trace_rd(libresteam_ext_repl_ctx_t *ctx, uint32_t max)
{
    trace_ctx_t *trace = 0x0;
    uint64_t queued = 0x0;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->trace)) { return LDG_ERR_NOT_INIT; }

    if (!max) { return LDG_ERR_RANGE; }

    trace = (trace_ctx_t *)ctx->trace;
    if (!trace->queue_up) { return LDG_ERR_NOT_INIT; }

    queued = ldg_spsc_cunt_get(&trace->events);
    if (LDG_UNLIKELY(queued > TRACE_QUEUE_CAP)) { return LDG_ERR_INVALID; }

    if (max > queued) { max = (uint32_t)queued; }

    for (rows = 0; rows < max; rows++)
    {
        res = ldg_spsc_pop(&trace->events, &trace->rd_event);
        if (res == LDG_ERR_EMPTY) { break; }

        if (res == LDG_ERR_AOK) { res = trace_event_emit(ctx, &trace->rd_event); }

        if (res != LDG_ERR_AOK) { return res; }
    }
    res = libresteam_ext_repl_emit(ctx, "trace rows: %u32; hits: %u64; dropped: %u64", rows, LDG_LD_ACQUIRE(trace->hits), LDG_LD_ACQUIRE(trace->dropped));
    return res == LDG_ERR_AOK ? libresteam_ext_repl_trace_status(ctx) : res;
}
