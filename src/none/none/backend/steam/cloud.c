#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/cloud/ops.h>

#define STEAM_CLOUD_LEN_MAX 0x7FFFFFFFU
#define STEAM_CLOUD_SIGN_BIT 0x80000000U

static uint32_t steam_cloud_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->cloud)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->cloud_desc, method_idx, out);
}

static uint32_t steam_cloud_bool_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = LDG_TRUTH_FALSE;

    res = steam_cloud_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint8_call(steam->cloud, slot, arg, arg_cunt, out);
}

static uint32_t steam_cloud_bool_ck(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t false_err)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t ok = LDG_TRUTH_FALSE;

    res = steam_cloud_bool_call(steam, method_idx, arg, arg_cunt, &ok);
    if (res != LDG_ERR_AOK) { return res; }

    if (ok != LDG_TRUTH_TRUE) { return false_err; }

    return LDG_ERR_AOK;
}

static uint32_t steam_cloud_cunt_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_cloud_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->cloud, slot, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_CLOUD_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_INIT;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_cloud_file_wr(void *ctx, const char *name, const void *data, uint32_t len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(len > STEAM_CLOUD_LEN_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = len, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_cloud_bool_ck((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_WR, arg, cunt, LDG_ERR_IO_WR);
}

static uint32_t steam_cloud_file_rd(void *ctx, const char *name, void *buff, uint32_t cap, uint32_t *out_len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t cap_clamped = cap;

    if (LDG_UNLIKELY(!out_len)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out_len = 0;

    if (LDG_UNLIKELY(!ctx || !name || !buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (cap_clamped > STEAM_CLOUD_LEN_MAX) { cap_clamped = STEAM_CLOUD_LEN_MAX; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap_clamped, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_cloud_cunt_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_RD, arg, cunt, out_len);
}

static uint32_t steam_cloud_file_forget(void *ctx, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_bool_ck((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_FORGET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_cloud_file_delete(void *ctx, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_bool_ck((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_DELETE, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_cloud_file_exists_get(void *ctx, const char *name, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_EXISTS, arg, cunt, out);
}

static uint32_t steam_cloud_file_persisted_get(void *ctx, const char *name, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_PERSISTED, arg, cunt, out);
}

static uint32_t steam_cloud_file_size_get(void *ctx, const char *name, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_cunt_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_SIZE_GET, arg, cunt, out);
}

static uint32_t steam_cloud_file_timestamp_get(void *ctx, const char *name, uint64_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_cloud_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_TIMESTAMP_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(steam->cloud, slot, arg, cunt, out);
}

static uint32_t steam_cloud_file_cunt_get(void *ctx, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_cloud_cunt_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_cloud_file_name_size_get(void *ctx, uint32_t idx, const char **name_out, uint32_t *size_out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t name = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name_out || !size_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *name_out = 0x0;
    *size_out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = size_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_cloud_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_CLOUD_FILE_NAME_SIZE_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->cloud, slot, arg, cunt, &name);
    if (res != LDG_ERR_AOK) { return res; }

    if (name == 0) { return LDG_ERR_NOT_FOUND; }

    *name_out = (const char *)(uintptr_t)name;

    return LDG_ERR_AOK;
}

static uint32_t steam_cloud_quota_get(void *ctx, uint64_t *total_out, uint64_t *avail_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!total_out || !avail_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *total_out = 0;
    *avail_out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = total_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = avail_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_cloud_bool_ck((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_QUOTA_GET, arg, cunt, LDG_ERR_NOT_INIT);
}

static uint32_t steam_cloud_account_en_get(void *ctx, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_cloud_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_ACCOUNT_EN, arg, cunt, out);
}

static uint32_t steam_cloud_app_en_get(void *ctx, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_cloud_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_CLOUD_APP_EN, arg, cunt, out);
}

static uint32_t steam_cloud_app_en_set(void *ctx, uint8_t en)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;

    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_cloud_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_CLOUD_APP_EN_SET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->cloud, slot, arg, cunt);
}

uint32_t libresteam_backend_steam_cloud_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->cloud_ops.file_wr = steam_cloud_file_wr;
    steam->cloud_ops.file_rd = steam_cloud_file_rd;
    steam->cloud_ops.file_forget = steam_cloud_file_forget;
    steam->cloud_ops.file_delete = steam_cloud_file_delete;
    steam->cloud_ops.file_exists_get = steam_cloud_file_exists_get;
    steam->cloud_ops.file_persisted_get = steam_cloud_file_persisted_get;
    steam->cloud_ops.file_size_get = steam_cloud_file_size_get;
    steam->cloud_ops.file_timestamp_get = steam_cloud_file_timestamp_get;
    steam->cloud_ops.file_cunt_get = steam_cloud_file_cunt_get;
    steam->cloud_ops.file_name_size_get = steam_cloud_file_name_size_get;
    steam->cloud_ops.quota_get = steam_cloud_quota_get;
    steam->cloud_ops.account_en_get = steam_cloud_account_en_get;
    steam->cloud_ops.app_en_get = steam_cloud_app_en_get;
    steam->cloud_ops.app_en_set = steam_cloud_app_en_set;
    steam->ops.cloud = &steam->cloud_ops;

    return LDG_ERR_AOK;
}
