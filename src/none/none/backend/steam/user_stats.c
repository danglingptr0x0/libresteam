#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/user_stats/ops.h>

#define STEAM_USER_STATS_SIGN_BIT 0x80000000U
#define STEAM_USER_STATS_SIGN_EXTEND_MASK 0xFFFFFFFF00000000ULL

static uint64_t steam_user_stats_sign_extend(uint32_t val)
{
    if ((val & STEAM_USER_STATS_SIGN_BIT) != 0) { return (uint64_t)val | STEAM_USER_STATS_SIGN_EXTEND_MASK; }

    return (uint64_t)val;
}

static uint32_t steam_user_stats_bool_call(libresteam_backend_steam_t *steam, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t false_err)
{
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t ok = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam->user_stats)) { return LDG_ERR_NOT_INIT; }

    res = ldg_cpp_slot_get(&steam->user_stats_desc, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_stats, slot, arg, arg_cunt, &ok);
    if (res != LDG_ERR_AOK) { return res; }

    if (ok != LDG_TRUTH_TRUE) { return false_err; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_stat_int_get(void *ctx, const char *name, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_INT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_stat_int_set(void *ctx, const char *name, uint32_t val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam_user_stats_sign_extend(val), .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_INT_SET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_stat_fp64_get(void *ctx, const char *name, double *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t stat = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &stat, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_FP32_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    *out = (double)stat;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_stat_fp64_set(void *ctx, const char *name, double val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = (libresteam_fp32_t)val, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STAT_FP32_SET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_avg_rate_stat_update(void *ctx, const char *name, double cunt_sesh, double sesh_len)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = (libresteam_fp32_t)cunt_sesh, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp64 = sesh_len, .kind = LDG_CPP_ARG_KIND_FP64 };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_AVG_RATE_STAT_UPDATE, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_get(void *ctx, const char *name, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_set(void *ctx, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_SET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_clear(void *ctx, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_CLEAR, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_unlock_time_get(void *ctx, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!achieved_out || !time_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *achieved_out = LDG_TRUTH_FALSE;
    *time_out = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = achieved_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = time_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_progress_indicate(void *ctx, const char *name, uint32_t curr, uint32_t max)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = curr, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_PROGRESS_INDICATE, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_achievement_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->user_stats)) { return LDG_ERR_NOT_INIT; }

    res = ldg_cpp_slot_get(&steam->user_stats_desc, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_CUNT_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->user_stats, slot, arg, cunt, out);
}

static uint32_t steam_user_stats_achievement_name_get(void *ctx, uint32_t idx, const char **out)
{
    libresteam_backend_steam_t *steam = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t name = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam = (libresteam_backend_steam_t *)ctx;
    if (LDG_UNLIKELY(!steam->user_stats)) { return LDG_ERR_NOT_INIT; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = ldg_cpp_slot_get(&steam->user_stats_desc, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_NAME_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_stats, slot, arg, cunt, &name);
    if (res != LDG_ERR_AOK) { return res; }

    if (name == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)name;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_stats_st(void *ctx)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STATS_ST, arg, cunt, LDG_ERR_NOT_INIT);
}

static uint32_t steam_user_stats_stats_rst(void *ctx, uint8_t achievements_too)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = achievements_too, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_bool_call((libresteam_backend_steam_t *)ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_STATS_RST, arg, cunt, LDG_ERR_NOT_INIT);
}

uint32_t libresteam_backend_steam_user_stats_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->user_stats_ops.stat_int_get = steam_user_stats_stat_int_get;
    steam->user_stats_ops.stat_int_set = steam_user_stats_stat_int_set;
    steam->user_stats_ops.stat_fp64_get = steam_user_stats_stat_fp64_get;
    steam->user_stats_ops.stat_fp64_set = steam_user_stats_stat_fp64_set;
    steam->user_stats_ops.avg_rate_stat_update = steam_user_stats_avg_rate_stat_update;
    steam->user_stats_ops.achievement_get = steam_user_stats_achievement_get;
    steam->user_stats_ops.achievement_set = steam_user_stats_achievement_set;
    steam->user_stats_ops.achievement_clear = steam_user_stats_achievement_clear;
    steam->user_stats_ops.achievement_unlock_time_get = steam_user_stats_achievement_unlock_time_get;
    steam->user_stats_ops.achievement_progress_indicate = steam_user_stats_achievement_progress_indicate;
    steam->user_stats_ops.achievement_cunt_get = steam_user_stats_achievement_cunt_get;
    steam->user_stats_ops.achievement_name_get = steam_user_stats_achievement_name_get;
    steam->user_stats_ops.stats_st = steam_user_stats_stats_st;
    steam->user_stats_ops.stats_rst = steam_user_stats_stats_rst;
    steam->ops.user_stats = &steam->user_stats_ops;

    return LDG_ERR_AOK;
}
