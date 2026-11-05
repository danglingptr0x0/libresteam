#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user_stats/ops.h>

#define STEAM_USER_STATS_EXT_ARG_NONE 0U
#define STEAM_USER_STATS_EXT_SIGN_BIT 0x80000000U
#define STEAM_USER_STATS_EXT_SIGN_EXTEND_MASK 0xFFFFFFFF00000000ULL
#define STEAM_USER_STATS_EXT_ID_SIZE ((uint32_t)sizeof(libresteam_steam_id_t))

static uint64_t steam_user_stats_ext_sign_extend(uint32_t val)
{
    if ((val & STEAM_USER_STATS_EXT_SIGN_BIT) != 0) { return (uint64_t)val | STEAM_USER_STATS_EXT_SIGN_EXTEND_MASK; }

    return (uint64_t)val;
}

static uint32_t steam_user_stats_ext_slot_get(void *ctx, uint32_t method_idx, libresteam_backend_steam_t **steam_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *steam_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->user_stats)) { return LDG_ERR_NOT_INIT; }

    *steam_out = steam;

    return ldg_cpp_slot_get(&steam->user_stats_desc, method_idx, slot_out);
}

static uint32_t steam_user_stats_ext_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_user_stats_ext_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->user_stats, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_stats_ext_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->user_stats, slot, arg, arg_cunt, out);
}

static uint32_t steam_user_stats_ext_cunt_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_user_stats_ext_u32_call(ctx, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_USER_STATS_EXT_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_call_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_user_stats_ext_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_stats, slot, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_str_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = steam_user_stats_ext_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_stats, slot, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_achievement_icon_get(void *ctx, const char *name, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_ICON_GET, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_achievement_display_attr_get(void *ctx, const char *name, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name || !key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET, arg, cunt, out);
}

static uint32_t steam_user_stats_ext_user_stats_req(void *ctx, libresteam_steam_id_t id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_USER_STATS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_STATS_REQ, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_user_stat_int_get(void *ctx, libresteam_steam_id_t id, const char *name, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_USER_STATS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_STAT_INT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_ext_user_stat_fp64_get(void *ctx, libresteam_steam_id_t id, const char *name, double *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t stat = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_USER_STATS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &stat, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_STAT_FP32_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    *out = (double)stat;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_user_achievement_get(void *ctx, libresteam_steam_id_t id, const char *name, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_USER_STATS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &achieved, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_ACHIEVEMENT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    if (achieved != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_user_achievement_unlock_time_get(void *ctx, libresteam_steam_id_t id, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!achieved_out || !time_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *achieved_out = LDG_TRUTH_FALSE;
    *time_out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &id, .size = STEAM_USER_STATS_EXT_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &achieved, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = time_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    if (achieved != LDG_TRUTH_FALSE) { *achieved_out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_lboard_find_or_create(void *ctx, const char *name, uint32_t sort, uint32_t display, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = sort, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = display, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_FIND_OR_CREATE, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_lboard_find(void *ctx, const char *name, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_FIND, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_lboard_name_get(void *ctx, uint64_t lboard, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_NAME_GET, arg, cunt, out);
}

static uint32_t steam_user_stats_ext_lboard_entry_cunt_get(void *ctx, uint64_t lboard, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_ENTRY_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_user_stats_ext_lboard_sort_get(void *ctx, uint64_t lboard, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_SORT_GET, arg, cunt, out);
}

static uint32_t steam_user_stats_ext_lboard_display_get(void *ctx, uint64_t lboard, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_DISPLAY_GET, arg, cunt, out);
}

static uint32_t steam_user_stats_ext_lboard_entries_download(void *ctx, uint64_t lboard, uint32_t req_type, uint32_t range_start, uint32_t range_end, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = req_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam_user_stats_ext_sign_extend(range_start), .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam_user_stats_ext_sign_extend(range_end), .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_lboard_entries_users_download(void *ctx, uint64_t lboard, libresteam_steam_id_t *users, uint32_t cunt_in, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!users)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = users, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cunt_in, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_lboard_downloaded_entry_get(void *ctx, uint64_t entries, uint32_t idx, libresteam_user_stats_lboard_entry_t *out, uint32_t *details, uint32_t details_max)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = (libresteam_user_stats_lboard_entry_t)LDG_STRUCT_ZERO_INIT;

    arg[cunt] = (ldg_cpp_arg_t) { .val = entries, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = details, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = details_max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_ext_lboard_score_upload(void *ctx, uint64_t lboard, uint32_t method, uint32_t score, const uint32_t *details, uint32_t detail_cunt, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = method, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = steam_user_stats_ext_sign_extend(score), .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = details, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = detail_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_SCORE_UPLOAD, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_lboard_ugc_attach(void *ctx, uint64_t lboard, uint64_t ugc, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = lboard, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ugc, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_LBOARD_UGC_ATTACH, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_current_players_req(void *ctx, uint64_t *call_out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = steam_user_stats_ext_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_CURRENT_PLAYERS_REQ, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_stats, slot, 0x0, STEAM_USER_STATS_EXT_ARG_NONE, call_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*call_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_glob_achievement_percent_req(void *ctx, uint64_t *call_out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = steam_user_stats_ext_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->user_stats, slot, 0x0, STEAM_USER_STATS_EXT_ARG_NONE, call_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*call_out == 0) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_achieved_finish(uint32_t res, uint32_t idx, libresteam_fp32_t percent, uint8_t achieved, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out)
{
    if (res != LDG_ERR_AOK) { return res; }

    if ((idx & STEAM_USER_STATS_EXT_SIGN_BIT) != 0) { return LDG_ERR_NOT_FOUND; }

    *idx_out = idx;
    *percent_out = (double)percent;
    if (achieved != LDG_TRUTH_FALSE) { *achieved_out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_most_achieved_get(void *ctx, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t percent = 0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!percent_out || !achieved_out || !idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *percent_out = 0;
    *achieved_out = LDG_TRUTH_FALSE;
    *idx_out = UINT32_MAX;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &percent, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &achieved, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_MOST_ACHIEVED_GET, arg, cunt, &idx);

    return steam_user_stats_ext_achieved_finish(res, idx, percent, achieved, percent_out, achieved_out, idx_out);
}

static uint32_t steam_user_stats_ext_next_most_achieved_get(void *ctx, uint32_t previous, char *name, uint32_t cap, double *percent_out, uint8_t *achieved_out, uint32_t *idx_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t percent = 0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!percent_out || !achieved_out || !idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *percent_out = 0;
    *achieved_out = LDG_TRUTH_FALSE;
    *idx_out = UINT32_MAX;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = steam_user_stats_ext_sign_extend(previous), .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &percent, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &achieved, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_NEXT_MOST_ACHIEVED_GET, arg, cunt, &idx);

    return steam_user_stats_ext_achieved_finish(res, idx, percent, achieved, percent_out, achieved_out, idx_out);
}

static uint32_t steam_user_stats_ext_achievement_percent_get(void *ctx, const char *name, double *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t percent = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &percent, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_ACHIEVEMENT_PERCENT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    *out = (double)percent;

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_glob_stats_req(void *ctx, uint32_t history_days, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = history_days, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_user_stats_ext_call_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STATS_REQ, arg, cunt, call_out);
}

static uint32_t steam_user_stats_ext_glob_stat_int64_get(void *ctx, const char *name, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_INT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_ext_glob_stat_fp64_get(void *ctx, const char *name, double *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_FP64_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_ext_history_call(void *ctx, uint32_t method_idx, const char *name, void *data, uint32_t byte_cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!name || !data)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = data, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = byte_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_user_stats_ext_cunt_call(ctx, method_idx, arg, cunt, cunt_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*cunt_out == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_user_stats_ext_glob_stat_history_int64_get(void *ctx, const char *name, uint64_t *data, uint32_t cap, uint32_t *cunt_out)
{
    if (LDG_UNLIKELY(cap > UINT32_MAX / (uint32_t)sizeof(uint64_t))) { return LDG_ERR_OVERFLOW; }

    return steam_user_stats_ext_history_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_HISTORY_INT_GET, name, data, cap * (uint32_t)sizeof(uint64_t), cunt_out);
}

static uint32_t steam_user_stats_ext_glob_stat_history_fp64_get(void *ctx, const char *name, double *data, uint32_t cap, uint32_t *cunt_out)
{
    if (LDG_UNLIKELY(cap > UINT32_MAX / (uint32_t)sizeof(double))) { return LDG_ERR_OVERFLOW; }

    return steam_user_stats_ext_history_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET, name, data, cap * (uint32_t)sizeof(double), cunt_out);
}

static uint32_t steam_user_stats_ext_progress_limit_int_get(void *ctx, const char *name, uint32_t *min_out, uint32_t *max_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!min_out || !max_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *min_out = 0;
    *max_out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = min_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = max_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_PROGRESS_LIMIT_INT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_user_stats_ext_progress_limit_fp64_get(void *ctx, const char *name, double *min_out, double *max_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_fp32_t min = 0;
    libresteam_fp32_t max = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!min_out || !max_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *min_out = 0;
    *max_out = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &min, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &max, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_user_stats_ext_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_USER_STATS_PROGRESS_LIMIT_FP32_GET, arg, cunt, LDG_ERR_NOT_FOUND);
    if (res != LDG_ERR_AOK) { return res; }

    *min_out = (double)min;
    *max_out = (double)max;

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_user_stats_ext_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_user_stats_ext_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->user_stats_ext_ops;
    ops->achievement_icon_get = steam_user_stats_ext_achievement_icon_get;
    ops->achievement_display_attr_get = steam_user_stats_ext_achievement_display_attr_get;
    ops->user_stats_req = steam_user_stats_ext_user_stats_req;
    ops->user_stat_int_get = steam_user_stats_ext_user_stat_int_get;
    ops->user_stat_fp64_get = steam_user_stats_ext_user_stat_fp64_get;
    ops->user_achievement_get = steam_user_stats_ext_user_achievement_get;
    ops->user_achievement_unlock_time_get = steam_user_stats_ext_user_achievement_unlock_time_get;
    ops->lboard_find_or_create = steam_user_stats_ext_lboard_find_or_create;
    ops->lboard_find = steam_user_stats_ext_lboard_find;
    ops->lboard_name_get = steam_user_stats_ext_lboard_name_get;
    ops->lboard_entry_cunt_get = steam_user_stats_ext_lboard_entry_cunt_get;
    ops->lboard_sort_get = steam_user_stats_ext_lboard_sort_get;
    ops->lboard_display_get = steam_user_stats_ext_lboard_display_get;
    ops->lboard_entries_download = steam_user_stats_ext_lboard_entries_download;
    ops->lboard_entries_users_download = steam_user_stats_ext_lboard_entries_users_download;
    ops->lboard_downloaded_entry_get = steam_user_stats_ext_lboard_downloaded_entry_get;
    ops->lboard_score_upload = steam_user_stats_ext_lboard_score_upload;
    ops->lboard_ugc_attach = steam_user_stats_ext_lboard_ugc_attach;
    ops->current_players_req = steam_user_stats_ext_current_players_req;
    ops->glob_achievement_percent_req = steam_user_stats_ext_glob_achievement_percent_req;
    ops->most_achieved_get = steam_user_stats_ext_most_achieved_get;
    ops->next_most_achieved_get = steam_user_stats_ext_next_most_achieved_get;
    ops->achievement_percent_get = steam_user_stats_ext_achievement_percent_get;
    ops->glob_stats_req = steam_user_stats_ext_glob_stats_req;
    ops->glob_stat_int64_get = steam_user_stats_ext_glob_stat_int64_get;
    ops->glob_stat_fp64_get = steam_user_stats_ext_glob_stat_fp64_get;
    ops->glob_stat_history_int64_get = steam_user_stats_ext_glob_stat_history_int64_get;
    ops->glob_stat_history_fp64_get = steam_user_stats_ext_glob_stat_history_fp64_get;
    ops->progress_limit_int_get = steam_user_stats_ext_progress_limit_int_get;
    ops->progress_limit_fp64_get = steam_user_stats_ext_progress_limit_fp64_get;
    steam->user_stats_ops.ext = ops;

    return LDG_ERR_AOK;
}
