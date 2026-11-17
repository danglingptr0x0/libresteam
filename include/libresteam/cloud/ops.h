#ifndef LIBRESTEAM_CLOUD_OPS_H
#define LIBRESTEAM_CLOUD_OPS_H

#include <stdint.h>

#define LIBRESTEAM_CLOUD_FILENAME_MAX 260U

typedef struct libresteam_cloud_ops
{
    uint32_t (*file_wr)(void *ctx, const char *name, const void *data, uint32_t len);
    uint32_t (*file_rd)(void *ctx, const char *name, void *buff, uint32_t cap, uint32_t *out_len);
    uint32_t (*file_forget)(void *ctx, const char *name);
    uint32_t (*file_delete)(void *ctx, const char *name);
    uint32_t (*file_exists_get)(void *ctx, const char *name, uint8_t *out);
    uint32_t (*file_persisted_get)(void *ctx, const char *name, uint8_t *out);
    uint32_t (*file_size_get)(void *ctx, const char *name, uint32_t *out);
    uint32_t (*file_timestamp_get)(void *ctx, const char *name, uint64_t *out);
    uint32_t (*file_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*file_name_size_get)(void *ctx, uint32_t idx, const char **name_out, uint32_t *size_out);
    uint32_t (*quota_get)(void *ctx, uint64_t *total_out, uint64_t *avail_out);
    uint32_t (*account_en_get)(void *ctx, uint8_t *out);
    uint32_t (*app_en_get)(void *ctx, uint8_t *out);
    uint32_t (*app_en_set)(void *ctx, uint8_t en);
} libresteam_cloud_ops_t;

#endif
