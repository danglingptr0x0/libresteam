#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>
#include <dangling/time/time.h>

#include <libresteam/srv/srv.h>
#include <libresteam/srv/user/user.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

#define SRV_US_KEY_CAP 256U
#define SRV_US_VAL_CAP 16U
#define SRV_US_RST_ACHIEVEMENTS_IDX 0U
#define SRV_US_REC_START 0U
#define SRV_US_KEY_NONE_LEN 0U

static uint8_t srv_us_stat_kind_is(uint8_t kind)
{
    if (kind == LIBRESTEAM_SRV_REC_KIND_STAT_INT || kind == LIBRESTEAM_SRV_REC_KIND_STAT_FP64 || kind == LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t srv_us_find(const libresteam_srv_client_t *client, uint8_t kind_a, uint8_t kind_b, const char *key, uint32_t key_len, libresteam_srv_rec_t *out, uint8_t *found)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    uint32_t pos = SRV_US_REC_START;
    uint8_t want_stat = srv_us_stat_kind_is(kind_a);

    *found = LDG_TRUTH_FALSE;

    while (libresteam_srv_account_rec_next(client, &pos, &rec) == LDG_ERR_AOK)
    {
        if (rec.app_id != client->app_id) { continue; }

        if (rec.kind == LIBRESTEAM_SRV_REC_KIND_STATS_RST)
        {
            if (want_stat == LDG_TRUTH_TRUE) { *found = LDG_TRUTH_FALSE; }
            else if (rec.val_len != 0 && rec.val[SRV_US_RST_ACHIEVEMENTS_IDX] == LDG_TRUTH_TRUE) { *found = LDG_TRUTH_FALSE; }

            continue;
        }

        if (rec.kind != kind_a && rec.kind != kind_b) { continue; }

        if (rec.key_len != key_len || ldg_mem_secure_eq_is(rec.key, key, key_len) != LDG_TRUTH_TRUE) { continue; }

        *out = rec;
        *found = LDG_TRUTH_TRUE;
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_us_val_u32_get(const libresteam_srv_rec_t *rec, uint32_t *out)
{
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;

    rd.buff = rec->val;
    rd.len = rec->val_len;

    return libresteam_wire_u32_get(&rd, out);
}

static uint32_t srv_us_name_get(libresteam_wire_rd_t *rd, char *name, uint32_t *len_out)
{
    uint64_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_str_get(rd, name, SRV_US_KEY_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_len_get(name, SRV_US_KEY_CAP, &len);
    if (res != LDG_ERR_AOK) { return res; }

    *len_out = (uint32_t)len;

    return LDG_ERR_AOK;
}

static uint32_t srv_us_achievement_last_is(const libresteam_srv_client_t *client, const libresteam_srv_rec_t *rec, uint32_t after, uint8_t *out)
{
    libresteam_srv_rec_t later = LDG_STRUCT_ZERO_INIT;
    uint32_t pos = after;

    *out = LDG_TRUTH_TRUE;

    while (libresteam_srv_account_rec_next(client, &pos, &later) == LDG_ERR_AOK)
    {
        if (later.app_id != rec->app_id || later.kind != LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT || later.key_len != rec->key_len) { continue; }

        if (ldg_mem_secure_eq_is(later.key, rec->key, rec->key_len) == LDG_TRUTH_TRUE)
        {
            *out = LDG_TRUTH_FALSE;
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t srv_us_achievement_nth_get(const libresteam_srv_client_t *client, uint32_t want_idx, uint32_t *cunt_out, libresteam_srv_rec_t *nth_out)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    uint32_t pos = SRV_US_REC_START;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t last = LDG_TRUTH_FALSE;

    *cunt_out = 0;

    while (libresteam_srv_account_rec_next(client, &pos, &rec) == LDG_ERR_AOK)
    {
        if (rec.app_id != client->app_id || rec.kind != LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT) { continue; }

        res = srv_us_achievement_last_is(client, &rec, pos, &last);
        if (res != LDG_ERR_AOK) { return res; }

        if (last != LDG_TRUTH_TRUE) { continue; }

        if (cunt == want_idx && nth_out) { *nth_out = rec; }

        cunt++;
    }

    *cunt_out = cunt;

    return LDG_ERR_AOK;
}

static uint32_t srv_us_stat_get(const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr, uint8_t want_fp64)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t val_rd = LDG_STRUCT_ZERO_INIT;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    double num = 0;
    double sum_cunt = 0;
    double sum_len = 0;
    uint32_t name_len = 0;
    uint32_t stat = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t found = LDG_TRUTH_FALSE;

    res = srv_us_name_get(rd, name, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    if (want_fp64 != LDG_TRUTH_TRUE)
    {
        res = srv_us_find(client, LIBRESTEAM_SRV_REC_KIND_STAT_INT, LIBRESTEAM_SRV_REC_KIND_STAT_INT, name, name_len, &rec, &found);
        if (res != LDG_ERR_AOK) { return res; }

        if (found == LDG_TRUTH_TRUE)
        {
            res = srv_us_val_u32_get(&rec, &stat);
            if (res != LDG_ERR_AOK) { return res; }
        }

        return libresteam_wire_u32_put(wr, stat);
    }

    res = srv_us_find(client, LIBRESTEAM_SRV_REC_KIND_STAT_FP64, LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE, name, name_len, &rec, &found);
    if (res != LDG_ERR_AOK) { return res; }

    if (found == LDG_TRUTH_TRUE)
    {
        val_rd.buff = rec.val;
        val_rd.len = rec.val_len;

        if (rec.kind == LIBRESTEAM_SRV_REC_KIND_STAT_FP64)
        {
            res = libresteam_wire_fp64_get(&val_rd, &num);
            if (res != LDG_ERR_AOK) { return res; }
        }
        else
        {
            res = libresteam_wire_fp64_get(&val_rd, &sum_cunt);
            if (res != LDG_ERR_AOK) { return res; }

            res = libresteam_wire_fp64_get(&val_rd, &sum_len);
            if (res != LDG_ERR_AOK) { return res; }

            if (sum_len > 0) { num = sum_cunt / sum_len; }
        }
    }

    return libresteam_wire_fp64_put(wr, num);
}

static uint32_t srv_us_val_append(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t kind, const char *name, uint32_t name_len, const libresteam_wire_wr_t *val)
{
    if (LDG_UNLIKELY(!val || !val->buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    return libresteam_srv_account_rec_append(srv, client, kind, name, name_len, val->buff, val->len);
}

static uint32_t srv_us_stat_set(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, uint8_t want_fp64)
{
    libresteam_wire_wr_t val = LDG_STRUCT_ZERO_INIT;
    uint8_t val_buff[SRV_US_VAL_CAP] = LDG_ARR_ZERO_INIT;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    double num = 0;
    uint32_t name_len = 0;
    uint32_t stat = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t kind = LIBRESTEAM_SRV_REC_KIND_STAT_INT;

    res = srv_us_name_get(rd, name, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    val.buff = val_buff;
    val.cap = SRV_US_VAL_CAP;

    if (want_fp64 == LDG_TRUTH_TRUE)
    {
        kind = LIBRESTEAM_SRV_REC_KIND_STAT_FP64;
        res = libresteam_wire_fp64_get(rd, &num);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_fp64_put(&val, num);
    }
    else
    {
        res = libresteam_wire_u32_get(rd, &stat);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_put(&val, stat);
    }

    if (res != LDG_ERR_AOK) { return res; }

    return srv_us_val_append(srv, client, kind, name, name_len, &val);
}

static uint32_t srv_us_avg_rate_update(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t val_rd = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_wr_t val = LDG_STRUCT_ZERO_INIT;
    uint8_t val_buff[SRV_US_VAL_CAP] = LDG_ARR_ZERO_INIT;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    double add_cunt = 0;
    double add_len = 0;
    double sum_cunt = 0;
    double sum_len = 0;
    uint32_t name_len = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t found = LDG_TRUTH_FALSE;

    res = srv_us_name_get(rd, name, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_get(rd, &add_cunt);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_get(rd, &add_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_us_find(client, LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE, LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE, name, name_len, &rec, &found);
    if (res != LDG_ERR_AOK) { return res; }

    if (found == LDG_TRUTH_TRUE)
    {
        val_rd.buff = rec.val;
        val_rd.len = rec.val_len;
        res = libresteam_wire_fp64_get(&val_rd, &sum_cunt);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_fp64_get(&val_rd, &sum_len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    val.buff = val_buff;
    val.cap = SRV_US_VAL_CAP;

    res = libresteam_wire_fp64_put(&val, sum_cunt + add_cunt);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_fp64_put(&val, sum_len + add_len);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_us_val_append(srv, client, LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE, name, name_len, &val);
}

static uint32_t srv_us_achievement_get(const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr, uint8_t with_time)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    libresteam_wire_rd_t val_rd = LDG_STRUCT_ZERO_INIT;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t name_len = 0;
    uint32_t unlock_time = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t achieved = LDG_TRUTH_FALSE;
    uint8_t found = LDG_TRUTH_FALSE;

    res = srv_us_name_get(rd, name, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_us_find(client, LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT, LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT, name, name_len, &rec, &found);
    if (res != LDG_ERR_AOK) { return res; }

    if (found == LDG_TRUTH_TRUE)
    {
        val_rd.buff = rec.val;
        val_rd.len = rec.val_len;
        res = libresteam_wire_u8_get(&val_rd, &achieved);
        if (res != LDG_ERR_AOK) { return res; }

        res = libresteam_wire_u32_get(&val_rd, &unlock_time);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = libresteam_wire_u8_put(wr, achieved);
    if (res != LDG_ERR_AOK) { return res; }

    if (with_time != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    return libresteam_wire_u32_put(wr, unlock_time);
}

static uint32_t srv_us_achievement_set(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, uint8_t achieved)
{
    libresteam_wire_wr_t val = LDG_STRUCT_ZERO_INIT;
    uint8_t val_buff[SRV_US_VAL_CAP] = LDG_ARR_ZERO_INIT;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t name_len = 0;
    uint32_t unlock_time = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_us_name_get(rd, name, &name_len);
    if (res != LDG_ERR_AOK) { return res; }

    if (achieved == LDG_TRUTH_TRUE) { unlock_time = (uint32_t)(ldg_time_epoch_ms_get() / LDG_MS_PER_SEC); }

    val.buff = val_buff;
    val.cap = SRV_US_VAL_CAP;

    res = libresteam_wire_u8_put(&val, achieved);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&val, unlock_time);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_us_val_append(srv, client, LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT, name, name_len, &val);
}

static uint32_t srv_us_achievement_name_get(const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    libresteam_srv_rec_t nth = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u32_get(rd, &idx);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_us_achievement_nth_get(client, idx, &cunt, &nth);
    if (res != LDG_ERR_AOK) { return res; }

    if (idx >= cunt) { return LDG_ERR_NOT_FOUND; }

    return libresteam_wire_blob_put(wr, nth.key, nth.key_len);
}

static uint32_t srv_us_stats_rst(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    uint8_t achievements_too = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u8_get(rd, &achievements_too);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_srv_account_rec_append(srv, client, LIBRESTEAM_SRV_REC_KIND_STATS_RST, 0x0, SRV_US_KEY_NONE_LEN, &achievements_too, (uint32_t)sizeof(uint8_t));
}

uint32_t libresteam_srv_user_stats_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    uint32_t cunt = 0;
    uint32_t curr = 0;
    uint32_t max = 0;
    uint32_t res = LDG_ERR_AOK;
    char name[SRV_US_KEY_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t name_len = 0;

    if (LDG_UNLIKELY(!srv || !client || !rd || !wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    switch (op)
    {
        case LIBRESTEAM_WIRE_OP_USER_STATS_STAT_INT_GET:
            return srv_us_stat_get(client, rd, wr, LDG_TRUTH_FALSE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_STAT_INT_SET:
            return srv_us_stat_set(srv, client, rd, LDG_TRUTH_FALSE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_STAT_FP64_GET:
            return srv_us_stat_get(client, rd, wr, LDG_TRUTH_TRUE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_STAT_FP64_SET:
            return srv_us_stat_set(srv, client, rd, LDG_TRUTH_TRUE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_AVG_RATE_STAT_UPDATE:
            return srv_us_avg_rate_update(srv, client, rd);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_GET:
            return srv_us_achievement_get(client, rd, wr, LDG_TRUTH_FALSE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_SET:
            return srv_us_achievement_set(srv, client, rd, LDG_TRUTH_TRUE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_CLEAR:
            return srv_us_achievement_set(srv, client, rd, LDG_TRUTH_FALSE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_UNLOCK_TIME_GET:
            return srv_us_achievement_get(client, rd, wr, LDG_TRUTH_TRUE);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_PROGRESS_INDICATE:
            res = srv_us_name_get(rd, name, &name_len);
            if (res != LDG_ERR_AOK) { return res; }

            res = libresteam_wire_u32_get(rd, &curr);
            if (res != LDG_ERR_AOK) { return res; }

            return libresteam_wire_u32_get(rd, &max);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_CUNT_GET:
            res = srv_us_achievement_nth_get(client, UINT32_MAX, &cunt, 0x0);
            if (res != LDG_ERR_AOK) { return res; }

            return libresteam_wire_u32_put(wr, cunt);
        case LIBRESTEAM_WIRE_OP_USER_STATS_ACHIEVEMENT_NAME_GET:
            return srv_us_achievement_name_get(client, rd, wr);
        case LIBRESTEAM_WIRE_OP_USER_STATS_STATS_ST:
            return LDG_ERR_AOK;
        case LIBRESTEAM_WIRE_OP_USER_STATS_STATS_RST:
            return srv_us_stats_rst(srv, client, rd);
        default:
            return LDG_ERR_UNSUPPORTED;
    }
}
