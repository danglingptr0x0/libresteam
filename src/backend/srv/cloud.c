#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/backend/srv/srv.h>
#include <libresteam/cloud/ops.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

#define SRV_CLOUD_FIELD_SIZE 4U
#define SRV_CLOUD_WR_FIXED (SRV_CLOUD_FIELD_SIZE * 4U)
#define SRV_CLOUD_RD_FIXED (SRV_CLOUD_FIELD_SIZE * 2U)
#define SRV_CLOUD_RD_CHUNK (LIBRESTEAM_WIRE_BODY_MAX - SRV_CLOUD_RD_FIXED)

static uint32_t srv_cloud_name_req(libresteam_backend_srv_t *srv, const char *name, libresteam_wire_wr_t *wr)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_backend_srv_req_begin(srv, wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_str_put(wr, name);
}

static uint32_t srv_cloud_name_u8_get(void *ctx, uint8_t op, const char *name, uint8_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = srv_cloud_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, op, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_flag_get(&rd, out);
}

static uint32_t srv_cloud_name_only(void *ctx, uint8_t op, const char *name)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, op, &wr, &rd);
}

static uint32_t srv_cloud_flag_get(void *ctx, uint8_t op, uint8_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, op, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_flag_get(&rd, out);
}

static uint32_t srv_cloud_file_wr(void *ctx, const char *name, const void *data, uint32_t len)
{
    libresteam_backend_srv_t *srv = 0x0;
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    const uint8_t *src = 0x0;
    uint64_t name_len = 0;
    uint32_t chunk_max = 0;
    uint32_t chunk = 0;
    uint32_t off = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t first = LDG_TRUTH_TRUE;

    if (LDG_UNLIKELY(!ctx || !name || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;
    src = (const uint8_t *)data;

    res = ldg_str_len_get(name, LIBRESTEAM_CLOUD_FILENAME_MAX, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    chunk_max = LIBRESTEAM_WIRE_BODY_MAX - SRV_CLOUD_WR_FIXED - (uint32_t)name_len;

    while (off < len || first == LDG_TRUTH_TRUE)
    {
        first = LDG_TRUTH_FALSE;
        chunk = len - off;
        if (chunk > chunk_max) { chunk = chunk_max; }

        res = srv_cloud_name_req(srv, name, &wr);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_put(&wr, len);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_put(&wr, off);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_blob_put(&wr, &src[off], chunk);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_backend_srv_call(srv, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_WR, &wr, &rd);
        if (res != LDG_ERR_AOK) { return res; }

        off += chunk;
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_file_rd(void *ctx, const char *name, void *buff, uint32_t cap, uint32_t *out_len)
{
    libresteam_backend_srv_t *srv = 0x0;
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    const uint8_t *chunk = 0x0;
    uint8_t *dst = 0x0;
    uint32_t chunk_len = 0;
    uint32_t want = 0;
    uint32_t total = 0;
    uint32_t off = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out_len)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out_len = 0;

    if (LDG_UNLIKELY(!ctx || !name || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;
    dst = (uint8_t *)buff;
    total = cap;

    while (off < cap && off < total)
    {
        want = cap - off;
        if (want > SRV_CLOUD_RD_CHUNK) { want = SRV_CLOUD_RD_CHUNK; }

        res = srv_cloud_name_req(srv, name, &wr);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_put(&wr, off);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_put(&wr, want);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_backend_srv_call(srv, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_RD, &wr, &rd);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_get(&rd, &total);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_blob_get(&rd, &chunk, &chunk_len);
        if (res != LDG_ERR_AOK) { return res; }

        if (chunk_len > want) { return LDG_ERR_NET_PROTOCOL; }

        if (chunk_len == 0) { break; }

        res = ldg_mem_secure_cpy(&dst[off], chunk, chunk_len);
        if (res != LDG_ERR_AOK) { return res; }

        off += chunk_len;
    }

    *out_len = off;

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_file_forget(void *ctx, const char *name)
{
    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_name_only(ctx, LIBRESTEAM_WIRE_OP_CLOUD_FILE_FORGET, name);
}

static uint32_t srv_cloud_file_delete(void *ctx, const char *name)
{
    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_name_only(ctx, LIBRESTEAM_WIRE_OP_CLOUD_FILE_DELETE, name);
}

static uint32_t srv_cloud_file_exists_get(void *ctx, const char *name, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx || !name || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_name_u8_get(ctx, LIBRESTEAM_WIRE_OP_CLOUD_FILE_EXISTS_GET, name, out);
}

static uint32_t srv_cloud_file_persisted_get(void *ctx, const char *name, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx || !name || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_name_u8_get(ctx, LIBRESTEAM_WIRE_OP_CLOUD_FILE_PERSISTED_GET, name, out);
}

static uint32_t srv_cloud_file_size_get(void *ctx, const char *name, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = srv_cloud_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_SIZE_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_cloud_file_timestamp_get(void *ctx, const char *name, uint64_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = srv_cloud_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_TIMESTAMP_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u64_get(&rd, out);
}

static uint32_t srv_cloud_file_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_CUNT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_cloud_file_name_size_get(void *ctx, uint32_t idx, const char **name_out, uint32_t *size_out)
{
    libresteam_backend_srv_t *srv = 0x0;
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name_out || !size_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *name_out = 0x0;
    *size_out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;

    res = libresteam_backend_srv_req_begin(srv, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, idx);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call(srv, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_FILE_NAME_SIZE_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_get(&rd, srv->str, LIBRESTEAM_BACKEND_SRV_STR_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(&rd, size_out);
    if (res != LDG_ERR_AOK) { return res; }

    *name_out = srv->str;

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_quota_get(void *ctx, uint64_t *total_out, uint64_t *avail_out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!total_out || !avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *total_out = 0;
    *avail_out = 0;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_QUOTA_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u64_get(&rd, total_out);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u64_get(&rd, avail_out);
}

static uint32_t srv_cloud_account_en_get(void *ctx, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_flag_get(ctx, LIBRESTEAM_WIRE_OP_CLOUD_ACCOUNT_EN_GET, out);
}

static uint32_t srv_cloud_app_en_get(void *ctx, uint8_t *out)
{
    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return srv_cloud_flag_get(ctx, LIBRESTEAM_WIRE_OP_CLOUD_APP_EN_GET, out);
}

static uint32_t srv_cloud_app_en_set(void *ctx, uint8_t en)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u8_put(&wr, en);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_CLOUD, LIBRESTEAM_WIRE_OP_CLOUD_APP_EN_SET, &wr, &rd);
}

uint32_t libresteam_backend_srv_cloud_ops_fill(libresteam_backend_srv_t *srv)
{
    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->cloud_ops.file_wr = srv_cloud_file_wr;
    srv->cloud_ops.file_rd = srv_cloud_file_rd;
    srv->cloud_ops.file_forget = srv_cloud_file_forget;
    srv->cloud_ops.file_delete = srv_cloud_file_delete;
    srv->cloud_ops.file_exists_get = srv_cloud_file_exists_get;
    srv->cloud_ops.file_persisted_get = srv_cloud_file_persisted_get;
    srv->cloud_ops.file_size_get = srv_cloud_file_size_get;
    srv->cloud_ops.file_timestamp_get = srv_cloud_file_timestamp_get;
    srv->cloud_ops.file_cunt_get = srv_cloud_file_cunt_get;
    srv->cloud_ops.file_name_size_get = srv_cloud_file_name_size_get;
    srv->cloud_ops.quota_get = srv_cloud_quota_get;
    srv->cloud_ops.account_en_get = srv_cloud_account_en_get;
    srv->cloud_ops.app_en_get = srv_cloud_app_en_get;
    srv->cloud_ops.app_en_set = srv_cloud_app_en_set;
    srv->ops.cloud = &srv->cloud_ops;

    return LDG_ERR_AOK;
}
