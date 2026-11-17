#ifndef LIBRESTEAM_TIMELINE_OPS_H
#define LIBRESTEAM_TIMELINE_OPS_H

#include <stdint.h>

#include <libresteam/fp32/fp32.h>

typedef enum libresteam_timeline_mode
{
    LIBRESTEAM_TIMELINE_MODE_INVALID = 0,
    LIBRESTEAM_TIMELINE_MODE_PLAYING,
    LIBRESTEAM_TIMELINE_MODE_STAGING,
    LIBRESTEAM_TIMELINE_MODE_MENUS,
    LIBRESTEAM_TIMELINE_MODE_LD_SCREEN,
    LIBRESTEAM_TIMELINE_MODE_MAX
} libresteam_timeline_mode_t;

typedef enum libresteam_timeline_clip_prio
{
    LIBRESTEAM_TIMELINE_CLIP_PRIO_INVALID = 0,
    LIBRESTEAM_TIMELINE_CLIP_PRIO_NONE,
    LIBRESTEAM_TIMELINE_CLIP_PRIO_STANDARD,
    LIBRESTEAM_TIMELINE_CLIP_PRIO_FEATURED
} libresteam_timeline_clip_prio_t;

typedef struct libresteam_timeline_ops
{
    uint32_t (*tooltip_set)(void *ctx, const char *desc, libresteam_fp32_t delta);
    uint32_t (*tooltip_clear)(void *ctx, libresteam_fp32_t delta);
    uint32_t (*mode_set)(void *ctx, libresteam_timeline_mode_t mode);
    uint32_t (*event_instant_add)(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out);
    uint32_t (*event_range_add)(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_fp32_t duration, libresteam_timeline_clip_prio_t clip, uint64_t *out);
    uint32_t (*event_range_start)(void *ctx, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out);
    uint32_t (*event_range_update)(void *ctx, uint64_t event, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_timeline_clip_prio_t clip);
    uint32_t (*event_range_end)(void *ctx, uint64_t event, libresteam_fp32_t offset);
    uint32_t (*event_remove)(void *ctx, uint64_t event);
    uint32_t (*event_recording_req)(void *ctx, uint64_t event, uint64_t *call_out);
    uint32_t (*phase_start)(void *ctx);
    uint32_t (*phase_end)(void *ctx);
    uint32_t (*phase_id_set)(void *ctx, const char *id);
    uint32_t (*phase_recording_req)(void *ctx, const char *id, uint64_t *call_out);
    uint32_t (*phase_tag_add)(void *ctx, const char *name, const char *icon, const char *group, uint32_t prio);
    uint32_t (*phase_attr_set)(void *ctx, const char *group, const char *val, uint32_t prio);
    uint32_t (*overlay_phase_open)(void *ctx, const char *id);
    uint32_t (*overlay_event_open)(void *ctx, uint64_t event);
} libresteam_timeline_ops_t;

#endif
