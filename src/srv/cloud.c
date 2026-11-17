#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/dir.h>
#include <dangling/io/file.h>
#include <dangling/io/path.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/cloud/ops.h>
#include <libresteam/srv/srv.h>
#include <libresteam/wire/op.h>
#include <libresteam/wire/wire.h>

#define SRV_CLOUD_DEC_CAP 11U
#define SRV_CLOUD_DEC_BASE 10U
#define SRV_CLOUD_DIGIT_FIRST '0'
#define SRV_CLOUD_CH_DOT '.'
#define SRV_CLOUD_CH_SLASH '/'
#define SRV_CLOUD_CH_BSLASH '\\'
#define SRV_CLOUD_CH_COLON ':'
#define SRV_CLOUD_CH_UPPER_FIRST 'A'
#define SRV_CLOUD_CH_UPPER_LAST 'Z'
#define SRV_CLOUD_CASE_DELTA ('a' - 'A')
#define SRV_CLOUD_NAME_CAP (LIBRESTEAM_CLOUD_FILENAME_MAX + 1U)
#define SRV_CLOUD_LEAF_CAP (SRV_CLOUD_NAME_CAP + sizeof(LIBRESTEAM_SRV_PART_EXT))
#define SRV_CLOUD_REC_START 0U
#define SRV_CLOUD_KEY_NONE_LEN 0U
#define SRV_CLOUD_LIST_ALL UINT32_MAX
#define SRV_CLOUD_RD_HDR_SIZE (2U * sizeof(uint32_t))
#define SRV_CLOUD_RD_DATA_MAX ((LIBRESTEAM_WIRE_BODY_MAX - SRV_CLOUD_RD_HDR_SIZE) & ~LIBRESTEAM_SRV_DIO_MASK)

static uint32_t srv_cloud_dec_fmt(uint32_t val, char *out)
{
    uint32_t digit_cunt = 0;
    uint32_t idx = 0;
    uint32_t num = val;

    do
    {
        digit_cunt++;
        num /= SRV_CLOUD_DEC_BASE;
    } while (num != 0 && digit_cunt < SRV_CLOUD_DEC_CAP);

    num = val;
    idx = digit_cunt;

    while (idx > 0)
    {
        idx--;
        out[idx] = (char)(SRV_CLOUD_DIGIT_FIRST + (char)(num % SRV_CLOUD_DEC_BASE));
        num /= SRV_CLOUD_DEC_BASE;
    }

    out[digit_cunt] = LDG_STR_TERM;

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_name_get(libresteam_wire_rd_t *rd, char *name)
{
    uint64_t len = 0;
    uint64_t idx = 0;
    uint64_t ext_len = sizeof(LIBRESTEAM_SRV_PART_EXT) - LDG_STR_TERM_SIZE;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_str_get(rd, name, SRV_CLOUD_NAME_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_len_get(name, SRV_CLOUD_NAME_CAP, &len);
    if (res != LDG_ERR_AOK) { return res; }

    if (len == 0 || name[0] == SRV_CLOUD_CH_DOT) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; idx < len; idx++)
    {
        if (name[idx] == SRV_CLOUD_CH_SLASH || name[idx] == SRV_CLOUD_CH_BSLASH || name[idx] == SRV_CLOUD_CH_COLON) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (name[idx] >= SRV_CLOUD_CH_UPPER_FIRST && name[idx] <= SRV_CLOUD_CH_UPPER_LAST) { name[idx] = (char)(name[idx] + SRV_CLOUD_CASE_DELTA); }
    }

    if (len > ext_len && ldg_mem_secure_eq_is(&name[len - ext_len], LIBRESTEAM_SRV_PART_EXT, ext_len) == LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_dir_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, uint8_t create)
{
    char app_dec[SRV_CLOUD_DEC_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_io_path_join(srv->data_dir, client->name, srv->path_scan, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (create == LDG_TRUTH_TRUE && ldg_io_path_exists_is(srv->path_scan) != LDG_TRUTH_TRUE)
    {
        res = ldg_io_dir_create(srv->path_scan, LIBRESTEAM_SRV_DIR_ACCESS);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = srv_cloud_dec_fmt(client->app_id, app_dec);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_path_join(srv->path_scan, app_dec, srv->path_dir, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (create == LDG_TRUTH_TRUE && ldg_io_path_exists_is(srv->path_dir) != LDG_TRUTH_TRUE) { return ldg_io_dir_create(srv->path_dir, LIBRESTEAM_SRV_DIR_ACCESS); }

    return LDG_ERR_AOK;
}

static uint32_t srv_cloud_file_path_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, const char *name, uint8_t create)
{
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_dir_get(srv, client, create);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_io_path_join(srv->path_dir, name, srv->path, LDG_IO_PATH_MAX);
}

static uint32_t srv_cloud_dir_size_sum(libresteam_srv_t *srv, const char *dir_path, uint64_t *sum)
{
    ldg_io_dir_t *dir = 0x0;
    ldg_io_entry_t entry = LDG_STRUCT_ZERO_INIT;
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    const char *leaf = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_io_dir_open(dir_path, &dir);
    if (res != LDG_ERR_AOK) { return res; }

    while (ldg_io_dir_rd(dir, &entry) == LDG_ERR_AOK)
    {
        if (ldg_io_entry_dir_is(&entry) == LDG_TRUTH_TRUE) { continue; }

        if (ldg_io_entry_name_get(&entry, &leaf) != LDG_ERR_AOK) { continue; }

        if (ldg_io_path_join(dir_path, leaf, srv->path_alt, LDG_IO_PATH_MAX) != LDG_ERR_AOK) { continue; }

        if (ldg_io_path_info_get(srv->path_alt, &info) != LDG_ERR_AOK) { continue; }

        *sum += info.size;
    }

    return ldg_io_dir_close(dir);
}

static uint32_t srv_cloud_usage_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, uint64_t *out)
{
    char app_dir[LDG_IO_PATH_MAX] = LDG_ARR_ZERO_INIT;
    ldg_io_dir_t *dir = 0x0;
    ldg_io_entry_t entry = LDG_STRUCT_ZERO_INIT;
    const char *leaf = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = ldg_io_path_join(srv->data_dir, client->name, srv->path_scan, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path_scan) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    res = ldg_io_dir_open(srv->path_scan, &dir);
    if (res != LDG_ERR_AOK) { return res; }

    while (ldg_io_dir_rd(dir, &entry) == LDG_ERR_AOK)
    {
        if (ldg_io_entry_dir_is(&entry) != LDG_TRUTH_TRUE) { continue; }

        if (ldg_io_entry_name_get(&entry, &leaf) != LDG_ERR_AOK || leaf[0] == SRV_CLOUD_CH_DOT) { continue; }

        if (ldg_io_path_join(srv->path_scan, leaf, app_dir, LDG_IO_PATH_MAX) != LDG_ERR_AOK) { continue; }

        res = srv_cloud_dir_size_sum(srv, app_dir, out);
        if (res != LDG_ERR_AOK) { break; }
    }

    if (ldg_io_dir_close(dir) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    return res;
}

static uint32_t srv_cloud_entry_nth_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, uint32_t want, uint32_t *cunt_out, libresteam_wire_wr_t *wr)
{
    ldg_io_dir_t *dir = 0x0;
    ldg_io_entry_t entry = LDG_STRUCT_ZERO_INIT;
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    const char *leaf = 0x0;
    uint64_t leaf_len = 0;
    uint64_t ext_len = sizeof(LIBRESTEAM_SRV_PART_EXT) - LDG_STR_TERM_SIZE;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    *cunt_out = 0;

    res = srv_cloud_dir_get(srv, client, LDG_TRUTH_FALSE);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path_dir) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    res = ldg_io_dir_open(srv->path_dir, &dir);
    if (res != LDG_ERR_AOK) { return res; }

    while (ldg_io_dir_rd(dir, &entry) == LDG_ERR_AOK)
    {
        if (ldg_io_entry_dir_is(&entry) == LDG_TRUTH_TRUE) { continue; }

        if (ldg_io_entry_name_get(&entry, &leaf) != LDG_ERR_AOK || leaf[0] == SRV_CLOUD_CH_DOT) { continue; }

        if (ldg_str_len_get(leaf, LDG_IO_ENTRY_NAME_MAX, &leaf_len) != LDG_ERR_AOK) { continue; }

        if (leaf_len > ext_len && ldg_mem_secure_eq_is(&leaf[leaf_len - ext_len], LIBRESTEAM_SRV_PART_EXT, ext_len) == LDG_TRUTH_TRUE) { continue; }

        if (cunt == want && wr)
        {
            res = ldg_io_path_join(srv->path_dir, leaf, srv->path_alt, LDG_IO_PATH_MAX);
            if (res == LDG_ERR_AOK) { res = ldg_io_path_info_get(srv->path_alt, &info); }

            if (res == LDG_ERR_AOK) { res = libresteam_wire_str_put(wr, leaf); }

            if (res == LDG_ERR_AOK) { res = libresteam_wire_u32_put(wr, (uint32_t)info.size); }

            if (res != LDG_ERR_AOK) { break; }
        }

        cunt++;
    }

    if (ldg_io_dir_close(dir) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_IO_CLOSE; }

    *cunt_out = cunt;

    return res;
}

static uint32_t srv_cloud_file_wr(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    char name[SRV_CLOUD_NAME_CAP] = LDG_ARR_ZERO_INIT;
    char leaf[SRV_CLOUD_LEAF_CAP] = LDG_ARR_ZERO_INIT;
    const uint8_t *data = 0x0;
    uint64_t used = 0;
    uint64_t part_size = 0;
    uint32_t total = 0;
    uint32_t off = 0;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_name_get(rd, name);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &total);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &off);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_blob_get(rd, &data, &len);
    if (res != LDG_ERR_AOK) { return res; }

    if (off > total || len > total - off) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (srv->quota != LIBRESTEAM_SRV_QUOTA_NONE)
    {
        res = srv_cloud_usage_get(srv, client, &used);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = srv_cloud_file_path_get(srv, client, name, LDG_TRUTH_TRUE);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_rbrcpy(leaf, name, SRV_CLOUD_LEAF_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_str_concatenate(leaf, LIBRESTEAM_SRV_PART_EXT, SRV_CLOUD_LEAF_CAP);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_path_join(srv->path_dir, leaf, srv->path_alt, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_info_get(srv->path_alt, &info) == LDG_ERR_AOK) { part_size = info.size; }

    if (off != 0 && (off > part_size || part_size - off >= LIBRESTEAM_SRV_DIO_BLK)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (srv->quota != LIBRESTEAM_SRV_QUOTA_NONE)
    {
        if (ldg_io_path_info_get(srv->path, &info) == LDG_ERR_AOK && info.size <= used) { used -= info.size; }

        if (part_size <= used) { used -= part_size; }

        if (used + total > srv->quota) { return LDG_ERR_FULL; }
    }

    res = libresteam_srv_dio_patch(srv->path_alt, srv->blk, off, data, len, total);
    if (res != LDG_ERR_AOK) { return res; }

    if (off + len < total) { return LDG_ERR_AOK; }

    return ldg_io_path_rename(srv->path_alt, srv->path);
}

static uint32_t srv_cloud_file_rd(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    char name[SRV_CLOUD_NAME_CAP] = LDG_ARR_ZERO_INIT;
    const uint8_t *data = 0x0;
    uint32_t off = 0;
    uint32_t want = 0;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_name_get(rd, name);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &off);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_get(rd, &want);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_cloud_file_path_get(srv, client, name, LDG_TRUTH_FALSE);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_path_info_get(srv->path, &info);
    if (res != LDG_ERR_AOK) { return LDG_ERR_NOT_FOUND; }

    if (want > SRV_CLOUD_RD_DATA_MAX) { want = SRV_CLOUD_RD_DATA_MAX; }

    res = libresteam_srv_dio_range_rd(srv->path, srv->frame, LIBRESTEAM_SRV_FRAME_CAP, off, want, &data, &len);
    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_wire_u32_put(wr, (uint32_t)info.size);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_blob_put(wr, data, len);
}

static uint32_t srv_cloud_file_info_reply(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr, uint8_t op)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    char name[SRV_CLOUD_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;
    uint8_t exists = LDG_TRUTH_FALSE;

    res = srv_cloud_name_get(rd, name);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_cloud_file_path_get(srv, client, name, LDG_TRUTH_FALSE);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_info_get(srv->path, &info) == LDG_ERR_AOK && info.is_file == LDG_TRUTH_TRUE) { exists = LDG_TRUTH_TRUE; }
    else { info.size = 0; info.modified_ns = 0; }

    if (op == LIBRESTEAM_WIRE_OP_CLOUD_FILE_EXISTS_GET || op == LIBRESTEAM_WIRE_OP_CLOUD_FILE_PERSISTED_GET) { return libresteam_wire_u8_put(wr, exists); }

    if (op == LIBRESTEAM_WIRE_OP_CLOUD_FILE_SIZE_GET) { return libresteam_wire_u32_put(wr, (uint32_t)info.size); }

    return libresteam_wire_u64_put(wr, info.modified_ns / LDG_NS_PER_SEC);
}

static uint32_t srv_cloud_file_delete(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    char name[SRV_CLOUD_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_name_get(rd, name);
    if (res != LDG_ERR_AOK) { return res; }

    res = srv_cloud_file_path_get(srv, client, name, LDG_TRUTH_FALSE);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path) != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    return ldg_io_path_destroy(srv->path);
}

static uint32_t srv_cloud_quota_get(libresteam_srv_t *srv, const libresteam_srv_client_t *client, libresteam_wire_wr_t *wr)
{
    uint64_t used = 0;
    uint64_t avail = 0;
    uint32_t res = LDG_ERR_AOK;

    res = srv_cloud_usage_get(srv, client, &used);
    if (res != LDG_ERR_AOK) { return res; }

    if (used < srv->quota) { avail = srv->quota - used; }

    res = libresteam_wire_u64_put(wr, srv->quota);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_u64_put(wr, avail);
}

static uint32_t srv_cloud_app_en_get(const libresteam_srv_client_t *client, libresteam_wire_wr_t *wr)
{
    libresteam_srv_rec_t rec = LDG_STRUCT_ZERO_INIT;
    uint32_t pos = SRV_CLOUD_REC_START;
    uint8_t en = LDG_TRUTH_TRUE;

    while (libresteam_srv_account_rec_next(client, &pos, &rec) == LDG_ERR_AOK)
    {
        if (rec.app_id != client->app_id || rec.kind != LIBRESTEAM_SRV_REC_KIND_CLOUD_APP_EN || rec.val_len == 0) { continue; }

        en = rec.val[0];
    }

    return libresteam_wire_u8_put(wr, en);
}

static uint32_t srv_cloud_app_en_set(libresteam_srv_t *srv, libresteam_srv_client_t *client, libresteam_wire_rd_t *rd)
{
    uint8_t en = LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_wire_u8_get(rd, &en);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_srv_account_rec_append(srv, client, LIBRESTEAM_SRV_REC_KIND_CLOUD_APP_EN, 0x0, SRV_CLOUD_KEY_NONE_LEN, &en, (uint32_t)sizeof(uint8_t));
}

uint32_t libresteam_srv_cloud_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr)
{
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv || !client || !rd || !wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    switch (op)
    {
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_WR:
            return srv_cloud_file_wr(srv, client, rd);
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_RD:
            return srv_cloud_file_rd(srv, client, rd, wr);
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_FORGET:
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_DELETE:
            return srv_cloud_file_delete(srv, client, rd);
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_EXISTS_GET:
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_PERSISTED_GET:
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_SIZE_GET:
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_TIMESTAMP_GET:
            return srv_cloud_file_info_reply(srv, client, rd, wr, op);
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_CUNT_GET:
            res = srv_cloud_entry_nth_get(srv, client, SRV_CLOUD_LIST_ALL, &cunt, 0x0);
            if (res != LDG_ERR_AOK) { return res; }

            return libresteam_wire_u32_put(wr, cunt);
        case LIBRESTEAM_WIRE_OP_CLOUD_FILE_NAME_SIZE_GET:
            res = libresteam_wire_u32_get(rd, &idx);
            if (res != LDG_ERR_AOK) { return res; }

            res = srv_cloud_entry_nth_get(srv, client, idx, &cunt, wr);
            if (res != LDG_ERR_AOK) { return res; }

            if (idx >= cunt) { return LDG_ERR_NOT_FOUND; }

            return LDG_ERR_AOK;
        case LIBRESTEAM_WIRE_OP_CLOUD_QUOTA_GET:
            return srv_cloud_quota_get(srv, client, wr);
        case LIBRESTEAM_WIRE_OP_CLOUD_ACCOUNT_EN_GET:
            return libresteam_wire_u8_put(wr, LDG_TRUTH_TRUE);
        case LIBRESTEAM_WIRE_OP_CLOUD_APP_EN_GET:
            return srv_cloud_app_en_get(client, wr);
        case LIBRESTEAM_WIRE_OP_CLOUD_APP_EN_SET:
            return srv_cloud_app_en_set(srv, client, rd);
        default:
            return LDG_ERR_UNSUPPORTED;
    }
}
