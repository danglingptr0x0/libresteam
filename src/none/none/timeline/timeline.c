#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/timeline/ops.h>
#include <libresteam/timeline/timeline.h>

static uint32_t timeline_ops_get(const libresteam_backend_set_t *set, const libresteam_timeline_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->timeline) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->timeline;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_timeline_tooltip_set(const libresteam_backend_set_t *set, const char *desc, libresteam_fp32_t delta)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->tooltip_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->tooltip_set(ctx, desc, delta);
}

uint32_t libresteam_timeline_tooltip_clear(const libresteam_backend_set_t *set, libresteam_fp32_t delta)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->tooltip_clear) { return LDG_ERR_UNSUPPORTED; }

    return ops->tooltip_clear(ctx, delta);
}

uint32_t libresteam_timeline_mode_set(const libresteam_backend_set_t *set, libresteam_timeline_mode_t mode)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->mode_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->mode_set(ctx, mode);
}

uint32_t libresteam_timeline_event_instant_add(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_instant_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_instant_add(ctx, title, desc, icon, prio, offset, clip, out);
}

uint32_t libresteam_timeline_event_range_add(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_fp32_t duration, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_range_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_range_add(ctx, title, desc, icon, prio, offset, duration, clip, out);
}

uint32_t libresteam_timeline_event_range_start(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_range_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_range_start(ctx, title, desc, icon, prio, offset, clip, out);
}

uint32_t libresteam_timeline_event_range_update(const libresteam_backend_set_t *set, uint64_t event, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_timeline_clip_prio_t clip)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_range_update) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_range_update(ctx, event, title, desc, icon, prio, clip);
}

uint32_t libresteam_timeline_event_range_end(const libresteam_backend_set_t *set, uint64_t event, libresteam_fp32_t offset)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_range_end) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_range_end(ctx, event, offset);
}

uint32_t libresteam_timeline_event_remove(const libresteam_backend_set_t *set, uint64_t event)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_remove(ctx, event);
}

uint32_t libresteam_timeline_event_recording_req(const libresteam_backend_set_t *set, uint64_t event, uint64_t *call_out)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->event_recording_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->event_recording_req(ctx, event, call_out);
}

uint32_t libresteam_timeline_phase_start(const libresteam_backend_set_t *set)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_start(ctx);
}

uint32_t libresteam_timeline_phase_end(const libresteam_backend_set_t *set)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_end) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_end(ctx);
}

uint32_t libresteam_timeline_phase_id_set(const libresteam_backend_set_t *set, const char *id)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_id_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_id_set(ctx, id);
}

uint32_t libresteam_timeline_phase_recording_req(const libresteam_backend_set_t *set, const char *id, uint64_t *call_out)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_recording_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_recording_req(ctx, id, call_out);
}

uint32_t libresteam_timeline_phase_tag_add(const libresteam_backend_set_t *set, const char *name, const char *icon, const char *group, uint32_t prio)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_tag_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_tag_add(ctx, name, icon, group, prio);
}

uint32_t libresteam_timeline_phase_attr_set(const libresteam_backend_set_t *set, const char *group, const char *val, uint32_t prio)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->phase_attr_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->phase_attr_set(ctx, group, val, prio);
}

uint32_t libresteam_timeline_overlay_phase_open(const libresteam_backend_set_t *set, const char *id)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_phase_open) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_phase_open(ctx, id);
}

uint32_t libresteam_timeline_overlay_event_open(const libresteam_backend_set_t *set, uint64_t event)
{
    const libresteam_timeline_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = timeline_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->overlay_event_open) { return LDG_ERR_UNSUPPORTED; }

    return ops->overlay_event_open(ctx, event);
}
