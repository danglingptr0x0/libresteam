#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/timeline/ops.h>

#define STEAM_TIMELINE_ARG_NONE 0U

static uint32_t steam_timeline_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->timeline)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->timeline_desc, method_idx, out);
}

static uint32_t steam_timeline_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_timeline_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->timeline, slot, arg, arg_cunt);
}

static uint32_t steam_timeline_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_timeline_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(steam->timeline, slot, arg, arg_cunt, out);
}

static uint32_t steam_timeline_tooltip_set(void *ctx, const char *desc, libresteam_fp32_t delta)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!desc)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = delta, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_TOOLTIP_SET, arg, cunt);
}

static uint32_t steam_timeline_tooltip_clear(void *ctx, libresteam_fp32_t delta)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = delta, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_TOOLTIP_CLEAR, arg, cunt);
}

static uint32_t steam_timeline_mode_set(void *ctx, libresteam_timeline_mode_t mode)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)mode, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_MODE_SET, arg, cunt);
}

static uint32_t steam_timeline_event_instant_add(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!title || !desc || !icon)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = title, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = icon, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = offset, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)clip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_INSTANT_ADD, arg, cunt, out);
}

static uint32_t steam_timeline_event_range_add(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_fp32_t duration, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!title || !desc || !icon)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = title, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = icon, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = offset, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = duration, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)clip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_RANGE_ADD, arg, cunt, out);
}

static uint32_t steam_timeline_event_range_start(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!title || !desc || !icon)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = title, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = icon, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = offset, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)clip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_RANGE_START, arg, cunt, out);
}

static uint32_t steam_timeline_event_range_update(void *ctx, uint64_t event, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_timeline_clip_prio_t clip)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!title || !desc || !icon)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = title, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = icon, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (uint64_t)clip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_RANGE_UPDATE, arg, cunt);
}

static uint32_t steam_timeline_event_range_end(void *ctx, uint64_t event, libresteam_fp32_t offset)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = offset, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_RANGE_END, arg, cunt);
}

static uint32_t steam_timeline_event_remove(void *ctx, uint64_t event)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_REMOVE, arg, cunt);
}

static uint32_t steam_timeline_event_recording_req(void *ctx, uint64_t event, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_EVENT_RECORDING_REQ, arg, cunt, call_out);
}

static uint32_t steam_timeline_phase_start(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_timeline_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_START, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->timeline, slot, 0x0, STEAM_TIMELINE_ARG_NONE);
}

static uint32_t steam_timeline_phase_end(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_timeline_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_END, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->timeline, slot, 0x0, STEAM_TIMELINE_ARG_NONE);
}

static uint32_t steam_timeline_phase_id_set(void *ctx, const char *id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_ID_SET, arg, cunt);
}

static uint32_t steam_timeline_phase_recording_req(void *ctx, const char *id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_timeline_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_RECORDING_REQ, arg, cunt, call_out);
}

static uint32_t steam_timeline_phase_tag_add(void *ctx, const char *name, const char *icon, const char *group, uint32_t prio)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name || !icon || !group)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = icon, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = group, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_TAG_ADD, arg, cunt);
}

static uint32_t steam_timeline_phase_attr_set(void *ctx, const char *group, const char *val, uint32_t prio)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!group || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = group, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_PHASE_ATTR_SET, arg, cunt);
}

static uint32_t steam_timeline_overlay_phase_open(void *ctx, const char *id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_OVERLAY_PHASE_OPEN, arg, cunt);
}

static uint32_t steam_timeline_overlay_event_open(void *ctx, uint64_t event)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = event, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_timeline_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_TIMELINE_OVERLAY_EVENT_OPEN, arg, cunt);
}

uint32_t libresteam_backend_steam_timeline_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->timeline_ops.tooltip_set = steam_timeline_tooltip_set;
    steam->timeline_ops.tooltip_clear = steam_timeline_tooltip_clear;
    steam->timeline_ops.mode_set = steam_timeline_mode_set;
    steam->timeline_ops.event_instant_add = steam_timeline_event_instant_add;
    steam->timeline_ops.event_range_add = steam_timeline_event_range_add;
    steam->timeline_ops.event_range_start = steam_timeline_event_range_start;
    steam->timeline_ops.event_range_update = steam_timeline_event_range_update;
    steam->timeline_ops.event_range_end = steam_timeline_event_range_end;
    steam->timeline_ops.event_remove = steam_timeline_event_remove;
    steam->timeline_ops.event_recording_req = steam_timeline_event_recording_req;
    steam->timeline_ops.phase_start = steam_timeline_phase_start;
    steam->timeline_ops.phase_end = steam_timeline_phase_end;
    steam->timeline_ops.phase_id_set = steam_timeline_phase_id_set;
    steam->timeline_ops.phase_recording_req = steam_timeline_phase_recording_req;
    steam->timeline_ops.phase_tag_add = steam_timeline_phase_tag_add;
    steam->timeline_ops.phase_attr_set = steam_timeline_phase_attr_set;
    steam->timeline_ops.overlay_phase_open = steam_timeline_overlay_phase_open;
    steam->timeline_ops.overlay_event_open = steam_timeline_overlay_event_open;
    steam->ops.timeline = &steam->timeline_ops;

    return LDG_ERR_AOK;
}
