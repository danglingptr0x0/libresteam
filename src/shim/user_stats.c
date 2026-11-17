#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/user/stats/ops.h>
#include <libresteam/user/stats/stats.h>

#define LIBRESTEAM_SHIM_USER_STATS_VERSION "STEAMUSERSTATS_INTERFACE_VERSION012"
#define LIBRESTEAM_SHIM_USER_STATS_METHOD_CUNT 45U
#define LIBRESTEAM_SHIM_USER_STATS_011_VERSION "STEAMUSERSTATS_INTERFACE_VERSION011"
#define LIBRESTEAM_SHIM_USER_STATS_011_METHOD_CUNT 43U
#define LIBRESTEAM_SHIM_USER_STATS_CURR_REQ 0U
#define LIBRESTEAM_SHIM_USER_STATS_STAT_INT_GET 1U
#define LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_GET 2U
#define LIBRESTEAM_SHIM_USER_STATS_STAT_INT_SET 3U
#define LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_SET 4U
#define LIBRESTEAM_SHIM_USER_STATS_AVG_RATE_UPDATE 5U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_GET 6U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_SET 7U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CLEAR 8U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET 9U
#define LIBRESTEAM_SHIM_USER_STATS_STATS_ST 10U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_ICON_GET 11U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET 12U
#define LIBRESTEAM_SHIM_USER_STATS_PROGRESS_INDICATE 13U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CUNT_GET 14U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET 15U
#define LIBRESTEAM_SHIM_USER_STATS_USER_STATS_REQ 16U
#define LIBRESTEAM_SHIM_USER_STATS_USER_STAT_INT_GET 17U
#define LIBRESTEAM_SHIM_USER_STATS_USER_STAT_FP32_GET 18U
#define LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_GET 19U
#define LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET 20U
#define LIBRESTEAM_SHIM_USER_STATS_STATS_RST 21U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND_OR_CREATE 22U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND 23U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET 24U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRY_CUNT_GET 25U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_SORT_GET 26U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_DISPLAY_GET 27U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD 28U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD 29U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET 30U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_SCORE_UPLOAD 31U
#define LIBRESTEAM_SHIM_USER_STATS_LBOARD_UGC_ATTACH 32U
#define LIBRESTEAM_SHIM_USER_STATS_CURRENT_PLAYERS_REQ 33U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ 34U
#define LIBRESTEAM_SHIM_USER_STATS_MOST_ACHIEVED_GET 35U
#define LIBRESTEAM_SHIM_USER_STATS_NEXT_MOST_ACHIEVED_GET 36U
#define LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_PERCENT_GET 37U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_STATS_REQ 38U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_INT64_GET 39U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_FP64_GET 40U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_INT64_GET 41U
#define LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET 42U
#define LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_INT_GET 43U
#define LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_FP32_GET 44U
#define LIBRESTEAM_SHIM_USER_STATS_NONE 0U
#define LIBRESTEAM_SHIM_USER_STATS_CALL_NONE 0U
#define LIBRESTEAM_SHIM_USER_STATS_IDX_NONE 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY ""
#define LIBRESTEAM_SHIM_USER_STATS_STR(str) ((str) ? (str) : LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY)
#define LIBRESTEAM_SHIM_USER_STATS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_user_stats_aok_is(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t libresteam_shim_user_stats_curr_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_CURR_REQ, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (!set) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_stat_int_get(void *self, const char *name, uint32_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STAT_INT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_stat_int_get(set, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STAT_INT_GET, res, "name: %s; val: %i32", LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = val; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_stat_fp32_get(void *self, const char *name, libresteam_fp32_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    double val = 0.0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_stat_fp64_get(set, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_GET, res, "name: %s; val: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = (libresteam_fp32_t)val; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_stat_int_set(void *self, const char *name, uint32_t val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STAT_INT_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_stat_int_set(set, name, val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STAT_INT_SET, res, "name: %s; val: %i32", LIBRESTEAM_SHIM_USER_STATS_STR(name), val);

    return libresteam_shim_user_stats_aok_is(res);
}

static uint8_t libresteam_shim_user_stats_stat_fp32_set(void *self, const char *name, libresteam_fp32_t val)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_stat_fp64_set(set, name, (double)val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_SET, res, "name: %s; val: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), (double)val);

    return libresteam_shim_user_stats_aok_is(res);
}

static uint8_t libresteam_shim_user_stats_avg_rate_update(void *self, const char *name, libresteam_fp32_t cunt_sesh, double sesh_len)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_AVG_RATE_UPDATE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_avg_rate_stat_update(set, name, (double)cunt_sesh, sesh_len);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_AVG_RATE_UPDATE, res, "name: %s; cunt: %f; len: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), (double)cunt_sesh, sesh_len);

    return libresteam_shim_user_stats_aok_is(res);
}

static uint8_t libresteam_shim_user_stats_achievement_get(void *self, const char *name, uint8_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_get(set, name, &achieved);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_GET, res, "name: %s; achieved: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), (uint32_t)achieved);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = achieved; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_achievement_set(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_set(set, name);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_SET, res, "name: %s", LIBRESTEAM_SHIM_USER_STATS_STR(name));

    return libresteam_shim_user_stats_aok_is(res);
}

static uint8_t libresteam_shim_user_stats_achievement_clear(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CLEAR, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_clear(set, name);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CLEAR, res, "name: %s", LIBRESTEAM_SHIM_USER_STATS_STR(name));

    return libresteam_shim_user_stats_aok_is(res);
}

static uint8_t libresteam_shim_user_stats_achievement_unlock_time_get(void *self, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t time = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_unlock_time_get(set, name, &achieved, &time);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET, res, "name: %s; achieved: %u32; time: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), (uint32_t)achieved, time);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (achieved_out) { *achieved_out = achieved; }

    if (time_out) { *time_out = time; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_stats_st(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STATS_ST, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_stats_st(set);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STATS_ST, res, "store");

    return libresteam_shim_user_stats_aok_is(res);
}

static uint32_t libresteam_shim_user_stats_achievement_icon_get(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t icon = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_ICON_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_achievement_icon_get(set, name, &icon);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_ICON_GET, res, "name: %s; icon: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), icon);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return icon;
}

static const char* libresteam_shim_user_stats_achievement_display_attr_get(void *self, const char *name, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    res = libresteam_user_stats_achievement_display_attr_get(set, name, key, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET, res, "name: %s; key: %s; val: %s", LIBRESTEAM_SHIM_USER_STATS_STR(name), LIBRESTEAM_SHIM_USER_STATS_STR(key), LIBRESTEAM_SHIM_USER_STATS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_user_stats_progress_indicate(void *self, const char *name, uint32_t curr, uint32_t max)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_INDICATE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_progress_indicate(set, name, curr, max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_INDICATE, res, "name: %s; curr: %u32; max: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), curr, max);

    return libresteam_shim_user_stats_aok_is(res);
}

static uint32_t libresteam_shim_user_stats_achievement_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_achievement_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CUNT_GET, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return cunt;
}

static const char* libresteam_shim_user_stats_achievement_name_get(void *self, uint32_t idx)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *name = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    res = libresteam_user_stats_achievement_name_get(set, idx, &name);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET, res, "idx: %u32; name: %s", idx, LIBRESTEAM_SHIM_USER_STATS_STR(name));
    if (res != LDG_ERR_AOK || !name) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    return name;
}

static uint64_t libresteam_shim_user_stats_user_stats_req(void *self, uint64_t user_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_USER_STATS_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    user.all = user_id;
    res = libresteam_user_stats_user_stats_req(set, user, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_USER_STATS_REQ, res, "user: %u64; call: %u64", user_id, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_stats_user_stat_int_get(void *self, uint64_t user_id, const char *name, uint32_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint32_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_USER_STAT_INT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = user_id;
    res = libresteam_user_stats_user_stat_int_get(set, user, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_USER_STAT_INT_GET, res, "user: %u64; name: %s; val: %i32", user_id, LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = val; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_user_stat_fp32_get(void *self, uint64_t user_id, const char *name, libresteam_fp32_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    double val = 0.0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_USER_STAT_FP32_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = user_id;
    res = libresteam_user_stats_user_stat_fp64_get(set, user, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_USER_STAT_FP32_GET, res, "user: %u64; name: %s; val: %f", user_id, LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = (libresteam_fp32_t)val; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_user_achievement_get(void *self, uint64_t user_id, const char *name, uint8_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = user_id;
    res = libresteam_user_stats_user_achievement_get(set, user, name, &achieved);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_GET, res, "user: %u64; name: %s; achieved: %u32", user_id, LIBRESTEAM_SHIM_USER_STATS_STR(name), (uint32_t)achieved);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = achieved; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_user_achievement_unlock_time_get(void *self, uint64_t user_id, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t user = LDG_STRUCT_ZERO_INIT;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t time = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    user.all = user_id;
    res = libresteam_user_stats_user_achievement_unlock_time_get(set, user, name, &achieved, &time);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET, res, "user: %u64; name: %s; achieved: %u32; time: %u32", user_id, LIBRESTEAM_SHIM_USER_STATS_STR(name), (uint32_t)achieved, time);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (achieved_out) { *achieved_out = achieved; }

    if (time_out) { *time_out = time; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_stats_rst(void *self, uint8_t achievements_too)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_STATS_RST, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (achievements_too) { flag = LDG_TRUTH_TRUE; }

    res = libresteam_user_stats_stats_rst(set, flag);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_STATS_RST, res, "achievements_too: %u32", (uint32_t)flag);

    return libresteam_shim_user_stats_aok_is(res);
}

static uint64_t libresteam_shim_user_stats_lboard_find_or_create(void *self, const char *name, uint32_t sort, uint32_t display)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND_OR_CREATE, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_lboard_find_or_create(set, name, sort, display, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND_OR_CREATE, res, "name: %s; sort: %u32; display: %u32; call: %u64", LIBRESTEAM_SHIM_USER_STATS_STR(name), sort, display, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_stats_lboard_find(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_lboard_find(set, name, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND, res, "name: %s; call: %u64", LIBRESTEAM_SHIM_USER_STATS_STR(name), call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static const char* libresteam_shim_user_stats_lboard_name_get(void *self, uint64_t lboard)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    res = libresteam_user_stats_lboard_name_get(set, lboard, &str);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET, res, "lboard: %u64; name: %s", lboard, LIBRESTEAM_SHIM_USER_STATS_STR(str));
    if (res != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY; }

    return str;
}

static uint32_t libresteam_shim_user_stats_lboard_entry_cunt_get(void *self, uint64_t lboard)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRY_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_lboard_entry_cunt_get(set, lboard, &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRY_CUNT_GET, res, "lboard: %u64; cunt: %u32", lboard, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return cunt;
}

static uint32_t libresteam_shim_user_stats_lboard_sort_get(void *self, uint64_t lboard)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t sort = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_SORT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_lboard_sort_get(set, lboard, &sort);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_SORT_GET, res, "lboard: %u64; sort: %u32", lboard, sort);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return sort;
}

static uint32_t libresteam_shim_user_stats_lboard_display_get(void *self, uint64_t lboard)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t display = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_DISPLAY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_lboard_display_get(set, lboard, &display);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_DISPLAY_GET, res, "lboard: %u64; display: %u32", lboard, display);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return display;
}

static uint64_t libresteam_shim_user_stats_lboard_entries_download(void *self, uint64_t lboard, uint32_t req_type, uint32_t range_start, uint32_t range_end)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_lboard_entries_download(set, lboard, req_type, range_start, range_end, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD, res, "lboard: %u64; req_type: %u32; start: %i32; end: %i32; call: %u64", lboard, req_type, range_start, range_end, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_stats_lboard_entries_users_download(void *self, uint64_t lboard, libresteam_steam_id_t *users, uint32_t cunt)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint64_t first = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    if (users && cunt != 0) { first = users->all; }

    res = libresteam_user_stats_lboard_entries_users_download(set, lboard, users, cunt, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD, res, "lboard: %u64; cunt: %u32; first: %u64; call: %u64", lboard, cunt, first, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_stats_lboard_downloaded_entry_get(void *self, uint64_t entries, uint32_t idx, libresteam_user_stats_lboard_entry_t *out, uint32_t *details, uint32_t details_max)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_user_stats_lboard_entry_t entry = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_lboard_downloaded_entry_get(set, entries, idx, &entry, details, details_max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET, res, "entries: %u64; idx: %u32; user: %u64; rank: %u32; score: %i32; details: %u32", entries, idx, entry.user.all, entry.global_rank, entry.score, entry.detail_cunt);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out && ldg_mem_secure_cpy(out, &entry, (uint64_t)sizeof(entry)) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint64_t libresteam_shim_user_stats_lboard_score_upload(void *self, uint64_t lboard, uint32_t method, uint32_t score, const uint32_t *details, uint32_t detail_cunt)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_SCORE_UPLOAD, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_lboard_score_upload(set, lboard, method, score, details, detail_cunt, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_SCORE_UPLOAD, res, "lboard: %u64; method: %u32; score: %i32; details: %u32; call: %u64", lboard, method, score, detail_cunt, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_stats_lboard_ugc_attach(void *self, uint64_t lboard, uint64_t ugc)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_UGC_ATTACH, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_lboard_ugc_attach(set, lboard, ugc, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_LBOARD_UGC_ATTACH, res, "lboard: %u64; ugc: %u64; call: %u64", lboard, ugc, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_stats_current_players_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_CURRENT_PLAYERS_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_current_players_req(set, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_CURRENT_PLAYERS_REQ, res, "call: %u64", call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint64_t libresteam_shim_user_stats_glob_achievement_percent_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_glob_achievement_percent_req(set, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ, res, "call: %u64", call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint32_t libresteam_shim_user_stats_achieved_out(uint32_t res, uint32_t idx, double percent, uint8_t achieved, libresteam_fp32_t *percent_out, uint8_t *achieved_out)
{
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_IDX_NONE; }

    if (percent_out) { *percent_out = (libresteam_fp32_t)percent; }

    if (achieved_out) { *achieved_out = achieved; }

    return idx;
}

static uint32_t libresteam_shim_user_stats_most_achieved_get(void *self, char *name, uint32_t cap, libresteam_fp32_t *percent_out, uint8_t *achieved_out)
{
    const libresteam_backend_set_t *set = 0x0;
    double percent = 0.0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t idx = LIBRESTEAM_SHIM_USER_STATS_IDX_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_MOST_ACHIEVED_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_IDX_NONE; }

    res = libresteam_user_stats_most_achieved_get(set, name, cap, &percent, &achieved, &idx);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_MOST_ACHIEVED_GET, res, "idx: %u32; name: %s; percent: %f; achieved: %u32", idx, (res == LDG_ERR_AOK) ? LIBRESTEAM_SHIM_USER_STATS_STR(name) : LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY, percent, (uint32_t)achieved);

    return libresteam_shim_user_stats_achieved_out(res, idx, percent, achieved, percent_out, achieved_out);
}

static uint32_t libresteam_shim_user_stats_next_most_achieved_get(void *self, uint32_t previous, char *name, uint32_t cap, libresteam_fp32_t *percent_out, uint8_t *achieved_out)
{
    const libresteam_backend_set_t *set = 0x0;
    double percent = 0.0;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint32_t idx = LIBRESTEAM_SHIM_USER_STATS_IDX_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_NEXT_MOST_ACHIEVED_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_IDX_NONE; }

    res = libresteam_user_stats_next_most_achieved_get(set, previous, name, cap, &percent, &achieved, &idx);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_NEXT_MOST_ACHIEVED_GET, res, "previous: %u32; idx: %u32; name: %s; percent: %f; achieved: %u32", previous, idx, (res == LDG_ERR_AOK) ? LIBRESTEAM_SHIM_USER_STATS_STR(name) : LIBRESTEAM_SHIM_USER_STATS_STR_EMPTY, percent, (uint32_t)achieved);

    return libresteam_shim_user_stats_achieved_out(res, idx, percent, achieved, percent_out, achieved_out);
}

static uint8_t libresteam_shim_user_stats_achievement_percent_get(void *self, const char *name, libresteam_fp32_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    double percent = 0.0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_PERCENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_achievement_percent_get(set, name, &percent);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_PERCENT_GET, res, "name: %s; percent: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), percent);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = (libresteam_fp32_t)percent; }

    return LDG_TRUTH_TRUE;
}

static uint64_t libresteam_shim_user_stats_glob_stats_req(void *self, uint32_t history_days)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_USER_STATS_CALL_NONE;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STATS_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    res = libresteam_user_stats_glob_stats_req(set, history_days, &call);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STATS_REQ, res, "days: %u32; call: %u64", history_days, call);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_user_stats_glob_stat_int64_get(void *self, const char *name, uint64_t *out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_INT64_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_glob_stat_int64_get(set, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_INT64_GET, res, "name: %s; val: %i64", LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = val; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_glob_stat_fp64_get(void *self, const char *name, double *out)
{
    const libresteam_backend_set_t *set = 0x0;
    double val = 0.0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_FP64_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_glob_stat_fp64_get(set, name, &val);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_FP64_GET, res, "name: %s; val: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), val);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (out) { *out = val; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_user_stats_glob_stat_history_int64_get(void *self, const char *name, uint64_t *data, uint32_t byte_cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_INT64_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_glob_stat_history_int64_get(set, name, data, byte_cap / (uint32_t)sizeof(uint64_t), &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_INT64_GET, res, "name: %s; cap: %u32; cunt: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), byte_cap, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return cunt;
}

static uint32_t libresteam_shim_user_stats_glob_stat_history_fp64_get(void *self, const char *name, double *data, uint32_t byte_cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    res = libresteam_user_stats_glob_stat_history_fp64_get(set, name, data, byte_cap / (uint32_t)sizeof(double), &cunt);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET, res, "name: %s; cap: %u32; cunt: %u32", LIBRESTEAM_SHIM_USER_STATS_STR(name), byte_cap, cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_USER_STATS_NONE; }

    return cunt;
}

static uint8_t libresteam_shim_user_stats_progress_limit_int_get(void *self, const char *name, uint32_t *min_out, uint32_t *max_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t min = 0;
    uint32_t max = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_INT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_progress_limit_int_get(set, name, &min, &max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_INT_GET, res, "name: %s; min: %i32; max: %i32", LIBRESTEAM_SHIM_USER_STATS_STR(name), min, max);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (min_out) { *min_out = min; }

    if (max_out) { *max_out = max; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_user_stats_progress_limit_fp32_get(void *self, const char *name, libresteam_fp32_t *min_out, libresteam_fp32_t *max_out)
{
    const libresteam_backend_set_t *set = 0x0;
    double min = 0.0;
    double max = 0.0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_FP32_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_user_stats_progress_limit_fp64_get(set, name, &min, &max);
    res = libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_FP32_GET, res, "name: %s; min: %f; max: %f", LIBRESTEAM_SHIM_USER_STATS_STR(name), min, max);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (min_out) { *min_out = (libresteam_fp32_t)min; }

    if (max_out) { *max_out = (libresteam_fp32_t)max; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_user_stats_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_USER_STATS_CURR_REQ:
        { *out = "RequestCurrentStats";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_STAT_INT_GET:
        case LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_GET:
        { *out = "GetStat";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_STAT_INT_SET:
        case LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_SET:
        { *out = "SetStat";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_AVG_RATE_UPDATE:
        { *out = "UpdateAvgRateStat";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_GET:
        { *out = "GetAchievement";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_SET:
        { *out = "SetAchievement";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CLEAR:
        { *out = "ClearAchievement";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET:
        { *out = "GetAchievementAndUnlockTime";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_STATS_ST:
        { *out = "StoreStats";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_ICON_GET:
        { *out = "GetAchievementIcon";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET:
        { *out = "GetAchievementDisplayAttribute";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_INDICATE:
        { *out = "IndicateAchievementProgress";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CUNT_GET:
        { *out = "GetNumAchievements";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET:
        { *out = "GetAchievementName";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_USER_STATS_REQ:
        { *out = "RequestUserStats";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_USER_STAT_INT_GET:
        case LIBRESTEAM_SHIM_USER_STATS_USER_STAT_FP32_GET:
        { *out = "GetUserStat";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_GET:
        { *out = "GetUserAchievement";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET:
        { *out = "GetUserAchievementAndUnlockTime";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_STATS_RST:
        { *out = "ResetAllStats";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND_OR_CREATE:
        { *out = "FindOrCreateLeaderboard";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND:
        { *out = "FindLeaderboard";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET:
        { *out = "GetLeaderboardName";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRY_CUNT_GET:
        { *out = "GetLeaderboardEntryCount";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_SORT_GET:
        { *out = "GetLeaderboardSortMethod";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_DISPLAY_GET:
        { *out = "GetLeaderboardDisplayType";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD:
        { *out = "DownloadLeaderboardEntries";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD:
        { *out = "DownloadLeaderboardEntriesForUsers";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET:
        { *out = "GetDownloadedLeaderboardEntry";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_SCORE_UPLOAD:
        { *out = "UploadLeaderboardScore";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_UGC_ATTACH:
        { *out = "AttachLeaderboardUGC";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_CURRENT_PLAYERS_REQ:
        { *out = "GetNumberOfCurrentPlayers";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ:
        { *out = "RequestGlobalAchievementPercentages";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_MOST_ACHIEVED_GET:
        { *out = "GetMostAchievedAchievementInfo";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_NEXT_MOST_ACHIEVED_GET:
        { *out = "GetNextMostAchievedAchievementInfo";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_PERCENT_GET:
        { *out = "GetAchievementAchievedPercent";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STATS_REQ:
        { *out = "RequestGlobalStats";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_INT64_GET:
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_FP64_GET:
        { *out = "GetGlobalStat";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_INT64_GET:
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET:
        { *out = "GetGlobalStatHistory";
          break;
        }
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_INT_GET:
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_FP32_GET:
        { *out = "GetAchievementProgressLimits";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_user_stats_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_USER_STATS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET:
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET:
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_user_stats_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_USER_STATS_CURR_REQ:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_curr_req)
        case LIBRESTEAM_SHIM_USER_STATS_STAT_INT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stat_int_get)
        case LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stat_fp32_get)
        case LIBRESTEAM_SHIM_USER_STATS_STAT_INT_SET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stat_int_set)
        case LIBRESTEAM_SHIM_USER_STATS_STAT_FP32_SET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stat_fp32_set)
        case LIBRESTEAM_SHIM_USER_STATS_AVG_RATE_UPDATE:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_avg_rate_update)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_get)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_SET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_set)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CLEAR:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_clear)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_unlock_time_get)
        case LIBRESTEAM_SHIM_USER_STATS_STATS_ST:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stats_st)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_ICON_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_icon_get)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_DISPLAY_ATTR_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_display_attr_get)
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_INDICATE:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_progress_indicate)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_CUNT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_cunt_get)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_NAME_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_name_get)
        case LIBRESTEAM_SHIM_USER_STATS_USER_STATS_REQ:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_user_stats_req)
        case LIBRESTEAM_SHIM_USER_STATS_USER_STAT_INT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_user_stat_int_get)
        case LIBRESTEAM_SHIM_USER_STATS_USER_STAT_FP32_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_user_stat_fp32_get)
        case LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_user_achievement_get)
        case LIBRESTEAM_SHIM_USER_STATS_USER_ACHIEVEMENT_UNLOCK_TIME_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_user_achievement_unlock_time_get)
        case LIBRESTEAM_SHIM_USER_STATS_STATS_RST:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_stats_rst)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND_OR_CREATE:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_find_or_create)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_FIND:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_find)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_NAME_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_name_get)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRY_CUNT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_entry_cunt_get)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_SORT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_sort_get)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_DISPLAY_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_display_get)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_DOWNLOAD:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_entries_download)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_ENTRIES_USERS_DOWNLOAD:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_entries_users_download)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_DOWNLOADED_ENTRY_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_downloaded_entry_get)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_SCORE_UPLOAD:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_score_upload)
        case LIBRESTEAM_SHIM_USER_STATS_LBOARD_UGC_ATTACH:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_lboard_ugc_attach)
        case LIBRESTEAM_SHIM_USER_STATS_CURRENT_PLAYERS_REQ:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_current_players_req)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_ACHIEVEMENT_PERCENT_REQ:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_achievement_percent_req)
        case LIBRESTEAM_SHIM_USER_STATS_MOST_ACHIEVED_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_most_achieved_get)
        case LIBRESTEAM_SHIM_USER_STATS_NEXT_MOST_ACHIEVED_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_next_most_achieved_get)
        case LIBRESTEAM_SHIM_USER_STATS_ACHIEVEMENT_PERCENT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_achievement_percent_get)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STATS_REQ:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_stats_req)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_INT64_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_stat_int64_get)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_FP64_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_stat_fp64_get)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_INT64_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_stat_history_int64_get)
        case LIBRESTEAM_SHIM_USER_STATS_GLOB_STAT_HISTORY_FP64_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_glob_stat_history_fp64_get)
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_INT_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_progress_limit_int_get)
        case LIBRESTEAM_SHIM_USER_STATS_PROGRESS_LIMIT_FP32_GET:
            LIBRESTEAM_SHIM_USER_STATS_IMPL(libresteam_shim_user_stats_progress_limit_fp32_get)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_user_stats_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_USER_STATS_VERSION;
    out->name_get = libresteam_shim_user_stats_name_get;
    out->kind_get = libresteam_shim_user_stats_kind_get;
    out->impl_get = libresteam_shim_user_stats_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_USER_STATS_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_user_stats_011_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_USER_STATS_011_VERSION;
    out->name_get = libresteam_shim_user_stats_name_get;
    out->kind_get = libresteam_shim_user_stats_kind_get;
    out->impl_get = libresteam_shim_user_stats_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_USER_STATS_011_METHOD_CUNT;

    return LDG_ERR_AOK;
}
