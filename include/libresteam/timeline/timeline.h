#ifndef LIBRESTEAM_TIMELINE_H
#define LIBRESTEAM_TIMELINE_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/timeline/ops.h>

LDG_EXPORT uint32_t libresteam_timeline_tooltip_set(const libresteam_backend_set_t *set, const char *desc, libresteam_fp32_t delta);
LDG_EXPORT uint32_t libresteam_timeline_tooltip_clear(const libresteam_backend_set_t *set, libresteam_fp32_t delta);
LDG_EXPORT uint32_t libresteam_timeline_mode_set(const libresteam_backend_set_t *set, libresteam_timeline_mode_t mode);
LDG_EXPORT uint32_t libresteam_timeline_event_instant_add(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out);
LDG_EXPORT uint32_t libresteam_timeline_event_range_add(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_fp32_t duration, libresteam_timeline_clip_prio_t clip, uint64_t *out);
LDG_EXPORT uint32_t libresteam_timeline_event_range_start(const libresteam_backend_set_t *set, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_fp32_t offset, libresteam_timeline_clip_prio_t clip, uint64_t *out);
LDG_EXPORT uint32_t libresteam_timeline_event_range_update(const libresteam_backend_set_t *set, uint64_t event, const char *title, const char *desc, const char *icon, uint32_t prio, libresteam_timeline_clip_prio_t clip);
LDG_EXPORT uint32_t libresteam_timeline_event_range_end(const libresteam_backend_set_t *set, uint64_t event, libresteam_fp32_t offset);
LDG_EXPORT uint32_t libresteam_timeline_event_remove(const libresteam_backend_set_t *set, uint64_t event);
LDG_EXPORT uint32_t libresteam_timeline_event_recording_req(const libresteam_backend_set_t *set, uint64_t event, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_timeline_phase_start(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_timeline_phase_end(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_timeline_phase_id_set(const libresteam_backend_set_t *set, const char *id);
LDG_EXPORT uint32_t libresteam_timeline_phase_recording_req(const libresteam_backend_set_t *set, const char *id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_timeline_phase_tag_add(const libresteam_backend_set_t *set, const char *name, const char *icon, const char *group, uint32_t prio);
LDG_EXPORT uint32_t libresteam_timeline_phase_attr_set(const libresteam_backend_set_t *set, const char *group, const char *val, uint32_t prio);
LDG_EXPORT uint32_t libresteam_timeline_overlay_phase_open(const libresteam_backend_set_t *set, const char *id);
LDG_EXPORT uint32_t libresteam_timeline_overlay_event_open(const libresteam_backend_set_t *set, uint64_t event);

#endif
