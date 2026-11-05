#ifndef LIBRESTEAM_CLOUD_H
#define LIBRESTEAM_CLOUD_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/cloud/ops.h>

LDG_EXPORT uint32_t libresteam_cloud_file_wr(const libresteam_backend_set_t *set, const char *name, const void *data, uint32_t len);
LDG_EXPORT uint32_t libresteam_cloud_file_rd(const libresteam_backend_set_t *set, const char *name, void *buff, uint32_t cap, uint32_t *out_len);
LDG_EXPORT uint32_t libresteam_cloud_file_forget(const libresteam_backend_set_t *set, const char *name);
LDG_EXPORT uint32_t libresteam_cloud_file_delete(const libresteam_backend_set_t *set, const char *name);
LDG_EXPORT uint32_t libresteam_cloud_file_exists_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out);
LDG_EXPORT uint32_t libresteam_cloud_file_persisted_get(const libresteam_backend_set_t *set, const char *name, uint8_t *out);
LDG_EXPORT uint32_t libresteam_cloud_file_size_get(const libresteam_backend_set_t *set, const char *name, uint32_t *out);
LDG_EXPORT uint32_t libresteam_cloud_file_timestamp_get(const libresteam_backend_set_t *set, const char *name, uint64_t *out);
LDG_EXPORT uint32_t libresteam_cloud_file_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_cloud_file_name_size_get(const libresteam_backend_set_t *set, uint32_t idx, const char **name_out, uint32_t *size_out);
LDG_EXPORT uint32_t libresteam_cloud_quota_get(const libresteam_backend_set_t *set, uint64_t *total_out, uint64_t *avail_out);
LDG_EXPORT uint32_t libresteam_cloud_account_en_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_cloud_app_en_get(const libresteam_backend_set_t *set, uint8_t *out);
LDG_EXPORT uint32_t libresteam_cloud_app_en_set(const libresteam_backend_set_t *set, uint8_t en);

#endif
