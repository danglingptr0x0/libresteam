#ifndef LIBRESTEAM_CB_OPS_H
#define LIBRESTEAM_CB_OPS_H

#include <stdint.h>

#include <libresteam/cb/msg.h>

typedef struct libresteam_cb_sink_ops
{
    void (*msg_cb)(void *content, uint32_t user, uint32_t cb_id, const uint8_t *param, uint32_t param_size);
} libresteam_cb_sink_ops_t;

typedef struct libresteam_cb_sink
{
    const libresteam_cb_sink_ops_t *ops;
    void *content;
} libresteam_cb_sink_t;

typedef struct libresteam_cb_ops
{
    uint32_t (*pump)(void *ctx, const libresteam_cb_sink_t *sink);
    uint32_t (*call_res_get)(void *ctx, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out);
    uint32_t (*next_get)(void *ctx, libresteam_cb_msg_t *out);
    uint32_t (*last_free)(void *ctx);
} libresteam_cb_ops_t;

#endif
