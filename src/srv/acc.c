#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/dir.h>
#include <dangling/io/path.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/srv/srv.h>
#include <libresteam/wire/wire.h>

#define SRV_ACCOUNT_CHAR_DIGIT_FIRST '0'
#define SRV_ACCOUNT_CHAR_DIGIT_LAST '9'
#define SRV_ACCOUNT_CHAR_UPPER_FIRST 'A'
#define SRV_ACCOUNT_CHAR_UPPER_LAST 'Z'
#define SRV_ACCOUNT_CHAR_LOWER_FIRST 'a'
#define SRV_ACCOUNT_CHAR_LOWER_LAST 'z'
#define SRV_ACCOUNT_CHAR_UNDERSCORE '_'
#define SRV_ACCOUNT_CHAR_DASH '-'
#define SRV_ACCOUNT_REC_FIXED 13U
#define SRV_ACCOUNT_HDR_PROBE 64U
#define SRV_ACCOUNT_RST_ACHIEVEMENTS_IDX 0U
#define SRV_ACCOUNT_FILE_START 0U

uint8_t libresteam_srv_account_name_valid_is(const char *name)
{
    uint64_t len = 0;
    uint64_t idx = 0;
    char ch = LDG_STR_TERM;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (ldg_str_len_get(name, LIBRESTEAM_SRV_NAME_CAP, &len) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (len == 0 || len > LIBRESTEAM_WIRE_ACCOUNT_NAME_MAX) { return LDG_TRUTH_FALSE; }

    for (idx = 0; idx < len; idx++)
    {
        ch = name[idx];
        if (ch >= SRV_ACCOUNT_CHAR_DIGIT_FIRST && ch <= SRV_ACCOUNT_CHAR_DIGIT_LAST) { continue; }

        if (ch >= SRV_ACCOUNT_CHAR_UPPER_FIRST && ch <= SRV_ACCOUNT_CHAR_UPPER_LAST) { continue; }

        if (ch >= SRV_ACCOUNT_CHAR_LOWER_FIRST && ch <= SRV_ACCOUNT_CHAR_LOWER_LAST) { continue; }

        if (ch == SRV_ACCOUNT_CHAR_UNDERSCORE || ch == SRV_ACCOUNT_CHAR_DASH) { continue; }

        return LDG_TRUTH_FALSE;
    }

    return LDG_TRUTH_TRUE;
}

uint32_t libresteam_srv_account_path_get(const libresteam_srv_t *srv, const char *name, char *out)
{
    char leaf[LIBRESTEAM_SRV_NAME_CAP + sizeof(LIBRESTEAM_SRV_ACCOUNT_EXT)] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !name || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_str_rbrcpy(leaf, name, sizeof(leaf));
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_concatenate(leaf, LIBRESTEAM_SRV_ACCOUNT_EXT, sizeof(leaf));
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_io_path_join(srv->data_dir, leaf, out, LDG_IO_PATH_MAX);
}

static uint32_t srv_account_hdr_parse(const uint8_t *img, uint32_t img_len, uint32_t *account_out, const uint8_t **name_out, uint32_t *name_len_out, uint32_t *hdr_end_out)
{
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;
    uint32_t magic = 0;
    uint32_t rev = 0;

    rd.buff = img;
    rd.len = img_len;
    rd.pos = 0;

    if (libresteam_wire_u32_get(&rd, &magic) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_u32_get(&rd, &rev) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (magic != LIBRESTEAM_SRV_ACCOUNT_MAGIC || rev != LIBRESTEAM_SRV_ACCOUNT_REV) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_u32_get(&rd, account_out) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_blob_get(&rd, name_out, name_len_out) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    *hdr_end_out = rd.pos;

    return LDG_ERR_AOK;
}

static uint32_t srv_account_rec_parse(const uint8_t *img, uint32_t img_len, uint32_t *pos, libresteam_srv_rec_t *out)
{
    libresteam_wire_rd_t rd = LDG_STRUCT_ZERO_INIT;

    if (*pos >= img_len) { return LDG_ERR_EOF; }

    rd.buff = img;
    rd.len = img_len;
    rd.pos = *pos;

    if (libresteam_wire_u32_get(&rd, &out->app_id) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_u8_get(&rd, &out->kind) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_blob_get(&rd, &out->key, &out->key_len) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    if (libresteam_wire_blob_get(&rd, &out->val, &out->val_len) != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    *pos = rd.pos;

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_account_rec_next(const libresteam_srv_client_t *client, uint32_t *pos, libresteam_srv_rec_t *out)
{
    const uint8_t *name = 0x0;
    uint32_t name_len = 0;
    uint32_t account = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!client || !pos || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (*pos == 0)
    {
        res = srv_account_hdr_parse(client->img, client->img_len, &account, &name, &name_len, pos);
        if (res != LDG_ERR_AOK) { return res; }
    }

    return srv_account_rec_parse(client->img, client->img_len, pos, out);
}

static uint8_t srv_account_rec_same_is(const libresteam_srv_rec_t *lhs, const libresteam_srv_rec_t *rhs)
{
    if (lhs->app_id != rhs->app_id || lhs->kind != rhs->kind || lhs->key_len != rhs->key_len) { return LDG_TRUTH_FALSE; }

    if (lhs->key_len == 0) { return LDG_TRUTH_TRUE; }

    return ldg_mem_secure_eq_is(lhs->key, rhs->key, lhs->key_len);
}

static uint8_t srv_account_rec_superseded_is(const libresteam_srv_client_t *client, const libresteam_srv_rec_t *rec, uint32_t after)
{
    libresteam_srv_rec_t later = LDG_STRUCT_ZERO_INIT;
    uint32_t pos = after;
    uint8_t stat_kind = LDG_TRUTH_FALSE;

    if (rec->kind == LIBRESTEAM_SRV_REC_KIND_STATS_RST) { return LDG_TRUTH_TRUE; }

    if (rec->kind == LIBRESTEAM_SRV_REC_KIND_STAT_INT || rec->kind == LIBRESTEAM_SRV_REC_KIND_STAT_FP64 || rec->kind == LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE) { stat_kind = LDG_TRUTH_TRUE; }

    while (srv_account_rec_parse(client->img, client->img_len, &pos, &later) == LDG_ERR_AOK)
    {
        if (srv_account_rec_same_is(rec, &later) == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

        if (later.kind != LIBRESTEAM_SRV_REC_KIND_STATS_RST || later.app_id != rec->app_id) { continue; }

        if (stat_kind == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

        if (rec->kind == LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT && later.val_len != 0 && later.val[SRV_ACCOUNT_RST_ACHIEVEMENTS_IDX] == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }
    }

    return LDG_TRUTH_FALSE;
}

static uint32_t srv_account_rec_encode(uint8_t *buff, uint32_t cap, uint32_t *len, uint32_t app_id, uint8_t kind, const void *key, uint32_t key_len, const void *val, uint32_t val_len)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    wr.buff = buff;
    wr.cap = cap;
    wr.len = *len;

    res = libresteam_wire_u32_put(&wr, app_id);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u8_put(&wr, kind);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_blob_put(&wr, key, key_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_blob_put(&wr, val, val_len);
    if (res != LDG_ERR_AOK) { return res; }

    *len = wr.len;

    return LDG_ERR_AOK;
}

static uint32_t srv_account_compact(libresteam_srv_t *srv, libresteam_srv_client_t *client)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    const uint8_t *name = 0x0;
    uint32_t name_len = 0;
    uint32_t account = 0;
    uint32_t pos = 0;
    uint32_t hdr_end = 0;
    uint32_t new_len = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_account_hdr_parse(client->img, client->img_len, &account, &name, &name_len, &hdr_end);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_zero(srv->compact, LIBRESTEAM_SRV_IMG_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_cpy(srv->compact, client->img, hdr_end);
    if (res != LDG_ERR_AOK) { return res; }

    new_len = hdr_end;
    pos = hdr_end;

    while (srv_account_rec_parse(client->img, client->img_len, &pos, &rec) == LDG_ERR_AOK)
    {
        if (srv_account_rec_superseded_is(client, &rec, pos) == LDG_TRUTH_TRUE) { continue; }

        res = srv_account_rec_encode(srv->compact, LIBRESTEAM_SRV_IMG_CAP, &new_len, rec.app_id, rec.kind, rec.key, rec.key_len, rec.val, rec.val_len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = ldg_mem_secure_cpy(client->img, srv->compact, LIBRESTEAM_SRV_IMG_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    client->img_len = new_len;

    res = libresteam_srv_account_path_get(srv, client->name, srv->path);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_srv_dio_wr(srv->path, client->img, SRV_ACCOUNT_FILE_START, client->img_len, LDG_TRUTH_TRUE);
}

uint32_t libresteam_srv_account_rec_append(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t kind, const void *key, uint32_t key_len, const void *val, uint32_t val_len)
{
    uint64_t need = 0;
    uint32_t old_len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !client)) { return LDG_ERR_FUNC_ARG_NULL; }

    need = (uint64_t)SRV_ACCOUNT_REC_FIXED + key_len + val_len;

    if (client->img_len + need > LIBRESTEAM_SRV_IMG_CAP)
    {
        res = srv_account_compact(srv, client);
        if (res != LDG_ERR_AOK) { return res; }

        if (client->img_len + need > LIBRESTEAM_SRV_IMG_CAP) { return LDG_ERR_FULL; }
    }

    old_len = client->img_len;

    res = srv_account_rec_encode(client->img, LIBRESTEAM_SRV_IMG_CAP, &client->img_len, client->app_id, kind, key, key_len, val, val_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_srv_account_path_get(srv, client->name, srv->path);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_srv_dio_wr(srv->path, client->img, old_len, client->img_len, LDG_TRUTH_FALSE);
}

static uint32_t srv_account_create(libresteam_srv_t *srv, libresteam_srv_client_t *client)
{
    libresteam_wire_wr_t wr = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_secure_zero(client->img, LIBRESTEAM_SRV_IMG_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    client->account = srv->account_next;
    srv->account_next++;

    wr.buff = client->img;
    wr.cap = LIBRESTEAM_SRV_IMG_CAP;
    wr.len = 0;

    res = libresteam_wire_u32_put(&wr, LIBRESTEAM_SRV_ACCOUNT_MAGIC);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, LIBRESTEAM_SRV_ACCOUNT_REV);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(&wr, client->account);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_str_put(&wr, client->name);
    if (res != LDG_ERR_AOK) { return res; }

    client->img_len = wr.len;

    return libresteam_srv_dio_wr(srv->path, client->img, SRV_ACCOUNT_FILE_START, client->img_len, LDG_TRUTH_TRUE);
}

uint32_t libresteam_srv_account_open(libresteam_srv_t *srv, libresteam_srv_client_t *client)
{
    const uint8_t *name = 0x0;
    uint64_t want_len = 0;
    uint32_t name_len = 0;
    uint32_t hdr_end = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !client)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_srv_account_path_get(srv, client->name, srv->path);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path) != LDG_TRUTH_TRUE) { return srv_account_create(srv, client); }

    res = ldg_mem_secure_zero(client->img, LIBRESTEAM_SRV_IMG_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_srv_dio_rd(srv->path, client->img, LIBRESTEAM_SRV_IMG_CAP, &client->img_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_account_hdr_parse(client->img, client->img_len, &client->account, &name, &name_len, &hdr_end);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_len_get(client->name, LIBRESTEAM_SRV_NAME_CAP, &want_len);
    if (res != LDG_ERR_AOK) { return res; }

    if (want_len != name_len || ldg_mem_secure_eq_is(name, client->name, name_len) != LDG_TRUTH_TRUE) { return LDG_ERR_IO_FMT; }

    return LDG_ERR_AOK;
}

static uint32_t srv_account_entry_ck(libresteam_srv_t *srv, const char *leaf)
{
    const uint8_t *data = 0x0;
    const uint8_t *name = 0x0;
    uint64_t leaf_len = 0;
    uint32_t data_len = 0;
    uint32_t name_len = 0;
    uint32_t account = 0;
    uint32_t hdr_end = 0;
    uint32_t ext_len = (uint32_t)(sizeof(LIBRESTEAM_SRV_ACCOUNT_EXT) - LDG_STR_TERM_SIZE);
    uint32_t res = LDG_ERR_AOK;

    res = ldg_str_len_get(leaf, LDG_IO_ENTRY_NAME_MAX, &leaf_len);
    if (res != LDG_ERR_AOK) { return res; }

    if (leaf_len <= ext_len) { return LDG_ERR_AOK; }

    if (ldg_mem_secure_eq_is(&leaf[leaf_len - ext_len], LIBRESTEAM_SRV_ACCOUNT_EXT, ext_len) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    res = ldg_io_path_join(srv->data_dir, leaf, srv->path_alt, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_srv_dio_range_rd(srv->path_alt, srv->blk, LIBRESTEAM_SRV_DIO_BLK, SRV_ACCOUNT_FILE_START, SRV_ACCOUNT_HDR_PROBE, &data, &data_len);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_account_hdr_parse(data, data_len, &account, &name, &name_len, &hdr_end);
    if (res != LDG_ERR_AOK) { return LDG_ERR_AOK; }

    if (account >= srv->account_next) { srv->account_next = account + LIBRESTEAM_SRV_ACCOUNT_FIRST; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_account_next_scan(libresteam_srv_t *srv)
{
    ldg_io_dir_t *dir = 0x0;
    ldg_io_entry_t entry = LDG_STRUCT_ZERO_INIT;
    const char *leaf = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->account_next = LIBRESTEAM_SRV_ACCOUNT_FIRST;

    res = ldg_io_dir_open(srv->data_dir, &dir);
    if (res != LDG_ERR_AOK) { return res; }

    while (ldg_io_dir_rd(dir, &entry) == LDG_ERR_AOK)
    {
        if (ldg_io_entry_dir_is(&entry) == LDG_TRUTH_TRUE) { continue; }

        if (ldg_io_entry_name_get(&entry, &leaf) != LDG_ERR_AOK) { continue; }

        res = srv_account_entry_ck(srv, leaf);
        if (res != LDG_ERR_AOK) { break; }
    }

    close_res = ldg_io_dir_close(dir);
    if (res != LDG_ERR_AOK) { return res; }

    return close_res;
}
