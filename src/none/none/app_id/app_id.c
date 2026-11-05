#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/file.h>
#include <dangling/io/path.h>
#include <dangling/mem/alloc.h>
#include <dangling/str/str.h>

#include <libresteam/app_id/app_id.h>

#define APP_ID_DIO_ALIGN (4U * LDG_KIB)
#define APP_ID_DIO_BLK APP_ID_DIO_ALIGN
#define APP_ID_POOL_VAR 0U
#define APP_ID_DIO_MASK (APP_ID_DIO_ALIGN - 1U)
#define APP_ID_POOL_CAP (16U * LDG_KIB)
#define APP_ID_RD_ALLOC (2U * APP_ID_DIO_ALIGN)
#define APP_ID_ACCESS_NONE 0U
#define APP_ID_DIGIT_MAX 10U
#define APP_ID_DIGIT_FIRST '0'
#define APP_ID_DIGIT_LAST '9'

static uint32_t app_id_parse(const char *text, uint64_t len, uint32_t *out)
{
    char digit[APP_ID_DIGIT_MAX + LDG_STR_TERM_SIZE] = LDG_ARR_ZERO_INIT;
    uint64_t val = 0;
    uint64_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = LIBRESTEAM_APP_ID_NONE;

    while (idx < len && idx < APP_ID_DIGIT_MAX && text[idx] >= APP_ID_DIGIT_FIRST && text[idx] <= APP_ID_DIGIT_LAST)
    {
        digit[idx] = text[idx];
        idx++;
    }

    if (idx == 0) { return LDG_ERR_IO_FMT; }

    digit[idx] = LDG_STR_TERM;

    res = ldg_str_uint_parse(digit, LDG_BASE_DEC, UINT32_MAX, &val);
    if (res != LDG_ERR_AOK) { return res; }

    if (val == LIBRESTEAM_APP_ID_NONE) { return LDG_ERR_IO_FMT; }

    *out = (uint32_t)val;

    return LDG_ERR_AOK;
}

static uint32_t app_id_file_path_get(char *dir, char *path)
{
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_io_path_prog_get(path, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_path_info_get(path, &info);
    if (res != LDG_ERR_AOK) { return res; }

    if (info.is_dir == LDG_TRUTH_TRUE) { res = ldg_io_path_normalize(path, dir, LDG_IO_PATH_MAX); }
    else { res = ldg_io_path_parent_get(path, dir, LDG_IO_PATH_MAX); }

    if (res != LDG_ERR_AOK) { return res; }

    return ldg_io_path_join(dir, LIBRESTEAM_APP_ID_FILE_NAME, path, LDG_IO_PATH_MAX);
}

static uint32_t app_id_file_rd(const char *path, uint8_t *blk, uint64_t *len_out)
{
    ldg_io_file_t *file = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    *len_out = 0;

    res = ldg_io_file_open(path, LDG_IO_RD_ONLY | LDG_IO_DIRECT, APP_ID_ACCESS_NONE, &file);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_file_rd(file, blk, APP_ID_DIO_BLK, len_out);
    close_res = ldg_io_file_close(file);

    if (res != LDG_ERR_AOK) { return res; }

    return close_res;
}

static uint32_t app_id_file_resolve(ldg_mem_pool_t *pool, uint32_t *out)
{
    void *rd_alloc = 0x0;
    void *dir_alloc = 0x0;
    void *path_alloc = 0x0;
    uint8_t *blk = 0x0;
    uint64_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_pool_alloc(pool, APP_ID_RD_ALLOC, &rd_alloc);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_pool_alloc(pool, LDG_IO_PATH_MAX, &dir_alloc);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_pool_alloc(pool, LDG_IO_PATH_MAX, &path_alloc);
    if (res != LDG_ERR_AOK) { return res; }

    blk = (uint8_t *)(((uintptr_t)rd_alloc + APP_ID_DIO_MASK) & ~(uintptr_t)APP_ID_DIO_MASK);

    res = app_id_file_path_get((char *)dir_alloc, (char *)path_alloc);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is((const char *)path_alloc) != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    res = app_id_file_rd((const char *)path_alloc, blk, &len);
    if (res != LDG_ERR_AOK) { return res; }

    return app_id_parse((const char *)blk, len, out);
}

uint32_t libresteam_app_id_resolve(uint32_t conf_app_id, uint32_t *out)
{
    ldg_mem_pool_t *pool = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t destroy_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_APP_ID_NONE;

    if (conf_app_id != LIBRESTEAM_APP_ID_NONE)
    {
        *out = conf_app_id;
        return LDG_ERR_AOK;
    }

    res = ldg_mem_pool_create(APP_ID_POOL_VAR, APP_ID_POOL_CAP, &pool);
    if (res != LDG_ERR_AOK) { return res; }

    res = app_id_file_resolve(pool, out);
    destroy_res = ldg_mem_pool_destroy(&pool);

    if (res != LDG_ERR_AOK) { return res; }

    return destroy_res;
}
