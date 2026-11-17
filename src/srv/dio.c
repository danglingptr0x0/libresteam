#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/file.h>
#include <dangling/mem/secure.h>

#include <libresteam/srv/srv.h>

#define SRV_DIO_ACCESS_NONE 0U

static uint32_t srv_dio_blk_rd(ldg_io_file_t *file, uint32_t off, uint8_t *blk, uint64_t *len_out)
{
    uint32_t res = LDG_ERR_AOK;

    *len_out = 0;

    res = ldg_io_file_seek(file, off, LDG_IO_SEEK_START);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_io_file_rd(file, blk, LIBRESTEAM_SRV_DIO_BLK, len_out);
}

uint32_t libresteam_srv_dio_rd(const char *path, uint8_t *dst, uint32_t cap, uint32_t *len_out)
{
    ldg_io_file_t *file = 0x0;
    uint64_t bytes_rd = 0;
    uint32_t total = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *len_out = 0;

    if (LDG_UNLIKELY(!path || !dst)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(((uintptr_t)dst & LIBRESTEAM_SRV_DIO_MASK) != 0 || (cap & LIBRESTEAM_SRV_DIO_MASK) != 0)) { return LDG_ERR_MEM_ALIGNMENT; }

    res = ldg_io_file_open(path, LDG_IO_RD_ONLY | LDG_IO_DIRECT, SRV_DIO_ACCESS_NONE, &file);
    if (res != LDG_ERR_AOK) { return res; }

    while (res == LDG_ERR_AOK && total < cap)
    {
        bytes_rd = 0;
        res = ldg_io_file_rd(file, &dst[total], LIBRESTEAM_SRV_DIO_BLK, &bytes_rd);
        if (res != LDG_ERR_AOK || bytes_rd == 0) { break; }

        total += (uint32_t)bytes_rd;
        if (bytes_rd < LIBRESTEAM_SRV_DIO_BLK) { break; }
    }

    if (res == LDG_ERR_AOK && total >= cap)
    {
        bytes_rd = 0;
        res = ldg_io_file_rd(file, dst, LIBRESTEAM_SRV_DIO_BLK, &bytes_rd);
        if (res == LDG_ERR_AOK && bytes_rd != 0) { res = LDG_ERR_FULL; }
    }

    close_res = ldg_io_file_close(file);
    if (res != LDG_ERR_AOK) { return res; }

    if (close_res != LDG_ERR_AOK) { return close_res; }

    *len_out = total;

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_dio_wr(const char *path, const uint8_t *src, uint32_t off, uint32_t end, uint8_t create)
{
    ldg_io_file_t *file = 0x0;
    uint64_t bytes_wr = 0;
    uint32_t blk_off = 0;
    uint32_t blk_end = 0;
    uint32_t flags = LDG_IO_WR_ONLY | LDG_IO_DIRECT;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!path || !src)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(((uintptr_t)src & LIBRESTEAM_SRV_DIO_MASK) != 0)) { return LDG_ERR_MEM_ALIGNMENT; }

    if (LDG_UNLIKELY(off > end)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (create == LDG_TRUTH_TRUE) { flags |= LDG_IO_CREATE | LDG_IO_TRUNCATE; }

    blk_off = off & ~LIBRESTEAM_SRV_DIO_MASK;
    blk_end = (end + LIBRESTEAM_SRV_DIO_MASK) & ~LIBRESTEAM_SRV_DIO_MASK;

    res = ldg_io_file_open(path, flags, LIBRESTEAM_SRV_FILE_ACCESS, &file);
    if (res != LDG_ERR_AOK) { return res; }

    if (blk_end > blk_off)
    {
        res = ldg_io_file_seek(file, blk_off, LDG_IO_SEEK_START);
        if (res == LDG_ERR_AOK) { res = ldg_io_file_wr(file, &src[blk_off], blk_end - blk_off, &bytes_wr); }

        if (res == LDG_ERR_AOK && bytes_wr != blk_end - blk_off) { res = LDG_ERR_IO_WR; }
    }

    if (res == LDG_ERR_AOK) { res = ldg_io_file_truncate(file, end); }

    close_res = ldg_io_file_close(file);
    if (res != LDG_ERR_AOK) { return res; }

    return close_res;
}

uint32_t libresteam_srv_dio_range_rd(const char *path, uint8_t *blk_buff, uint32_t blk_cap, uint32_t off, uint32_t len, const uint8_t **data_out, uint32_t *len_out)
{
    ldg_io_file_t *file = 0x0;
    ldg_io_info_t info = LDG_STRUCT_ZERO_INIT;
    uint64_t bytes_rd = 0;
    uint64_t blk_off = 0;
    uint64_t blk_end = 0;
    uint64_t end = 0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!data_out || !len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *data_out = 0x0;
    *len_out = 0;

    if (LDG_UNLIKELY(!path || !blk_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(((uintptr_t)blk_buff & LIBRESTEAM_SRV_DIO_MASK) != 0 || (blk_cap & LIBRESTEAM_SRV_DIO_MASK) != 0)) { return LDG_ERR_MEM_ALIGNMENT; }

    if (LDG_UNLIKELY(blk_cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = ldg_io_path_info_get(path, &info);
    if (res != LDG_ERR_AOK) { return res; }

    if (off >= info.size) { return LDG_ERR_AOK; }

    end = (uint64_t)off + len;
    if (end > info.size) { end = info.size; }

    blk_off = (uint64_t)off & ~(uint64_t)LIBRESTEAM_SRV_DIO_MASK;
    blk_end = (end + LIBRESTEAM_SRV_DIO_MASK) & ~(uint64_t)LIBRESTEAM_SRV_DIO_MASK;
    if (blk_end - blk_off > blk_cap)
    {
        blk_end = blk_off + blk_cap;
        end = blk_end;
    }

    res = ldg_io_file_open(path, LDG_IO_RD_ONLY | LDG_IO_DIRECT, SRV_DIO_ACCESS_NONE, &file);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_io_file_seek(file, (uint32_t)blk_off, LDG_IO_SEEK_START);
    if (res == LDG_ERR_AOK) { res = ldg_io_file_rd(file, blk_buff, blk_end - blk_off, &bytes_rd); }

    close_res = ldg_io_file_close(file);
    if (res != LDG_ERR_AOK) { return res; }

    if (close_res != LDG_ERR_AOK) { return close_res; }

    if (bytes_rd > blk_end - blk_off) { return LDG_ERR_OVERFLOW; }

    if (bytes_rd < end - blk_off) { end = blk_off + bytes_rd; }

    if (end <= off) { return LDG_ERR_AOK; }

    *data_out = &blk_buff[off - blk_off];
    *len_out = (uint32_t)(end - off);

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_dio_patch(const char *path, uint8_t *blk, uint32_t off, const uint8_t *data, uint32_t len, uint32_t total)
{
    ldg_io_file_t *file = 0x0;
    uint64_t bytes_rd = 0;
    uint64_t bytes_wr = 0;
    uint32_t blk_off = 0;
    uint32_t in_blk = 0;
    uint32_t chunk = 0;
    uint32_t done = 0;
    uint32_t flags = LDG_IO_RD_WR | LDG_IO_DIRECT | LDG_IO_CREATE;
    uint32_t res = LDG_ERR_AOK;
    uint32_t close_res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!path || !blk || (!data && len != 0))) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(((uintptr_t)blk & LIBRESTEAM_SRV_DIO_MASK) != 0)) { return LDG_ERR_MEM_ALIGNMENT; }

    if (off == 0) { flags |= LDG_IO_TRUNCATE; }

    res = ldg_io_file_open(path, flags, LIBRESTEAM_SRV_FILE_ACCESS, &file);
    if (res != LDG_ERR_AOK) { return res; }

    while (res == LDG_ERR_AOK && done < len)
    {
        blk_off = (off + done) & ~LIBRESTEAM_SRV_DIO_MASK;
        in_blk = (off + done) - blk_off;
        chunk = LIBRESTEAM_SRV_DIO_BLK - in_blk;
        if (chunk > len - done) { chunk = len - done; }

        res = ldg_mem_secure_zero(blk, LIBRESTEAM_SRV_DIO_BLK);
        if (res == LDG_ERR_AOK) { res = srv_dio_blk_rd(file, blk_off, blk, &bytes_rd); }

        if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&blk[in_blk], &data[done], chunk); }

        if (res == LDG_ERR_AOK) { res = ldg_io_file_seek(file, blk_off, LDG_IO_SEEK_START); }

        if (res == LDG_ERR_AOK) { res = ldg_io_file_wr(file, blk, LIBRESTEAM_SRV_DIO_BLK, &bytes_wr); }

        if (res == LDG_ERR_AOK && bytes_wr != LIBRESTEAM_SRV_DIO_BLK) { res = LDG_ERR_IO_WR; }

        done += chunk;
    }

    if (res == LDG_ERR_AOK && off + len >= total) { res = ldg_io_file_truncate(file, total); }

    close_res = ldg_io_file_close(file);
    if (res != LDG_ERR_AOK) { return res; }

    return close_res;
}
