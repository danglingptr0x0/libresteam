#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/srv/srv.h>
#include <libresteam/user_stats/ops.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

static uint32_t srv_user_stats_name_req(libresteam_backend_srv_t *srv, const char *name, libresteam_wire_wr_t *wr)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_backend_srv_req_begin(srv, wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_str_put(wr, name);
}

static uint32_t srv_user_stats_stat_int_get(void *ctx, const char *name, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STAT_INT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_user_stats_stat_int_set(void *ctx, const char *name, uint32_t val)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, val);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STAT_INT_SET, &wr, &rd);
}

static uint32_t srv_user_stats_stat_fp64_get(void *ctx, const char *name, double *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STAT_FP64_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_fp64_get(&rd, out);
}

static uint32_t srv_user_stats_stat_fp64_set(void *ctx, const char *name, double val)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_put(&wr, val);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STAT_FP64_SET, &wr, &rd);
}

static uint32_t srv_user_stats_avg_rate_stat_update(void *ctx, const char *name, double cunt_sesh, double sesh_len)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_put(&wr, cunt_sesh);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_put(&wr, sesh_len);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_AVG_RATE_STAT_UPDATE, &wr, &rd);
}

static uint32_t srv_user_stats_achievement_get(void *ctx, const char *name, uint8_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_flag_get(&rd, out);
}

static uint32_t srv_user_stats_achievement_set(void *ctx, const char *name)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_SET, &wr, &rd);
}

static uint32_t srv_user_stats_achievement_clear(void *ctx, const char *name)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_CLEAR, &wr, &rd);
}

static uint32_t srv_user_stats_achievement_unlock_time_get(void *ctx, const char *name, uint8_t *achieved_out, uint32_t *time_out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;
    uint32_t unlock_time = 0;
    uint8_t achieved = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!achieved_out || !time_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *achieved_out = LDG_TRUTH_FALSE;
    *time_out = 0;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_flag_get(&rd, &achieved);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(&rd, &unlock_time);
    if (res != LDG_ERR_AOK) { return res; }

    *achieved_out = achieved;
    *time_out = unlock_time;

    return LDG_ERR_AOK;
}

static uint32_t srv_user_stats_achievement_progress_indicate(void *ctx, const char *name, uint32_t curr, uint32_t max)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_user_stats_name_req((libresteam_backend_srv_t *)ctx, name, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, curr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, max);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_PROGRESS_INDICATE, &wr, &rd);
}

static uint32_t srv_user_stats_achievement_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_CUNT_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_user_stats_achievement_name_get(void *ctx, uint32_t idx, const char **out)
{
    libresteam_backend_srv_t *srv = 0x0;
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv = (libresteam_backend_srv_t *)ctx;

    res = libresteam_backend_srv_req_begin(srv, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, idx);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_backend_srv_call(srv, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_NAME_GET, &wr, &rd);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_get(&rd, srv->str, LIBRESTEAM_BACKEND_SRV_STR_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    *out = srv->str;

    return LDG_ERR_AOK;
}

static uint32_t srv_user_stats_stats_st(void *ctx)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STATS_ST, &wr, &rd);
}

static uint32_t srv_user_stats_stats_rst(void *ctx, uint8_t achievements_too)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_backend_srv_req_begin((libresteam_backend_srv_t *)ctx, &wr);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u8_put(&wr, achievements_too);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_backend_srv_call((libresteam_backend_srv_t *)ctx, LIBRESTEAM_WIRE_SERVICE_USER_STATS, LIBRESTEAM_WIRE_OP_USER_STATS_STATS_RST, &wr, &rd);
}

uint32_t libresteam_backend_srv_user_stats_ops_fill(libresteam_backend_srv_t *srv)
{
    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->user_stats_ops.stat_int_get = srv_user_stats_stat_int_get;
    srv->user_stats_ops.stat_int_set = srv_user_stats_stat_int_set;
    srv->user_stats_ops.stat_fp64_get = srv_user_stats_stat_fp64_get;
    srv->user_stats_ops.stat_fp64_set = srv_user_stats_stat_fp64_set;
    srv->user_stats_ops.avg_rate_stat_update = srv_user_stats_avg_rate_stat_update;
    srv->user_stats_ops.achievement_get = srv_user_stats_achievement_get;
    srv->user_stats_ops.achievement_set = srv_user_stats_achievement_set;
    srv->user_stats_ops.achievement_clear = srv_user_stats_achievement_clear;
    srv->user_stats_ops.achievement_unlock_time_get = srv_user_stats_achievement_unlock_time_get;
    srv->user_stats_ops.achievement_progress_indicate = srv_user_stats_achievement_progress_indicate;
    srv->user_stats_ops.achievement_cunt_get = srv_user_stats_achievement_cunt_get;
    srv->user_stats_ops.achievement_name_get = srv_user_stats_achievement_name_get;
    srv->user_stats_ops.stats_st = srv_user_stats_stats_st;
    srv->user_stats_ops.stats_rst = srv_user_stats_stats_rst;
    srv->ops.user_stats = &srv->user_stats_ops;

    return LDG_ERR_AOK;
}
