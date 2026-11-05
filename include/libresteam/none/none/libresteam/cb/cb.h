#ifndef LIBRESTEAM_CB_H
#define LIBRESTEAM_CB_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/cb/ops.h>

LDG_EXPORT uint32_t libresteam_cb_pump(const libresteam_backend_set_t *set, const libresteam_cb_sink_t *sink);
LDG_EXPORT uint32_t libresteam_cb_call_res_get(const libresteam_backend_set_t *set, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out);
LDG_EXPORT uint32_t libresteam_cb_next_get(const libresteam_backend_set_t *set, libresteam_cb_msg_t *out);
LDG_EXPORT uint32_t libresteam_cb_last_free(const libresteam_backend_set_t *set);

#endif
