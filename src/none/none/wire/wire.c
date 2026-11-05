#include <stdint.h>

#include <dangling/core/bits.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/proto/emiemi.h>
#include <dangling/str/str.h>

#include <libresteam/wire/wire.h>

#define WIRE_OFF_PROTO_REV 0U
#define WIRE_OFF_SERVICE 2U
#define WIRE_OFF_OP 3U
#define WIRE_OFF_FLAGS 4U
#define WIRE_OFF_SEQ 8U
#define WIRE_OFF_STATUS 12U
#define WIRE_OFF_BODY_LEN 16U
#define WIRE_OFF_BODY_HASH 20U
#define WIRE_OFF_CB_ID 24U
#define WIRE_OFF_RESERVED 28U
#define WIRE_RESERVED_VAL 0U

static void wire_uint_st(uint8_t *dst, uint64_t val, uint32_t size)
{
    uint32_t idx = 0;

    for (idx = 0; idx < size; idx++) { dst[idx] = (uint8_t)((val >> (idx * LDG_BYTE_BITS)) & LDG_BYTE_MASK); }
}

static uint64_t wire_uint_ld(const uint8_t *src, uint32_t size)
{
    uint64_t val = 0;
    uint32_t idx = 0;

    for (idx = 0; idx < size; idx++) { val |= (uint64_t)src[idx] << (idx * LDG_BYTE_BITS); }

    return val;
}

static uint32_t wire_uint_put(libresteam_wire_wr_t *wr, uint64_t val, uint32_t size)
{
    if (LDG_UNLIKELY(!wr || !wr->buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(wr->len > wr->cap || size > wr->cap - wr->len)) { return LDG_ERR_FULL; }

    wire_uint_st(&wr->buff[wr->len], val, size);
    wr->len += size;

    return LDG_ERR_AOK;
}

static uint32_t wire_uint_get(libresteam_wire_rd_t *rd, uint32_t size, uint64_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!rd || !rd->buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(rd->pos > rd->len || size > rd->len - rd->pos)) { return LDG_ERR_EOF; }

    *out = wire_uint_ld(&rd->buff[rd->pos], size);
    rd->pos += size;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_wr_init(libresteam_wire_wr_t *wr, uint8_t *buff, uint32_t cap)
{
    if (LDG_UNLIKELY(!wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    wr->buff = 0x0;
    wr->cap = 0;
    wr->len = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap < LIBRESTEAM_WIRE_HDR_SIZE)) { return LDG_ERR_FUNC_ARG_INVALID; }

    wr->buff = buff;
    wr->cap = cap;
    wr->len = LIBRESTEAM_WIRE_HDR_SIZE;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_u8_put(libresteam_wire_wr_t *wr, uint8_t val)
{
    if (LDG_UNLIKELY(!wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    return wire_uint_put(wr, val, (uint32_t)sizeof(uint8_t));
}

uint32_t libresteam_wire_u16_put(libresteam_wire_wr_t *wr, uint16_t val)
{
    if (LDG_UNLIKELY(!wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    return wire_uint_put(wr, val, (uint32_t)sizeof(uint16_t));
}

uint32_t libresteam_wire_u32_put(libresteam_wire_wr_t *wr, uint32_t val)
{
    if (LDG_UNLIKELY(!wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    return wire_uint_put(wr, val, (uint32_t)sizeof(uint32_t));
}

uint32_t libresteam_wire_u64_put(libresteam_wire_wr_t *wr, uint64_t val)
{
    if (LDG_UNLIKELY(!wr)) { return LDG_ERR_FUNC_ARG_NULL; }

    return wire_uint_put(wr, val, (uint32_t)sizeof(uint64_t));
}

uint32_t libresteam_wire_fp64_put(libresteam_wire_wr_t *wr, double val)
{
    uint64_t bits = 0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_mem_secure_cpy(&bits, &val, sizeof(double));
    if (res != LDG_ERR_AOK) { return res; }

    return wire_uint_put(wr, bits, (uint32_t)sizeof(uint64_t));
}

uint32_t libresteam_wire_blob_put(libresteam_wire_wr_t *wr, const void *data, uint32_t len)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!wr || !wr->buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!data && len != 0)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = wire_uint_put(wr, len, (uint32_t)sizeof(uint32_t));
    if (res != LDG_ERR_AOK) { return res; }

    if (len == 0) { return LDG_ERR_AOK; }

    if (LDG_UNLIKELY(len > wr->cap - wr->len)) { return LDG_ERR_FULL; }

    res = ldg_mem_secure_cpy(&wr->buff[wr->len], data, len);
    if (res != LDG_ERR_AOK) { return res; }

    wr->len += len;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_str_put(libresteam_wire_wr_t *wr, const char *str)
{
    uint64_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!wr || !str)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_str_len_get(str, LIBRESTEAM_WIRE_BODY_MAX, &len);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_wire_blob_put(wr, str, (uint32_t)len);
}

uint32_t libresteam_wire_hdr_finish(libresteam_wire_wr_t *wr, const libresteam_wire_hdr_t *hdr)
{
    uint32_t body_len = 0;
    uint32_t body_hash = 0;

    if (LDG_UNLIKELY(!wr || !wr->buff || !hdr)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(wr->len < LIBRESTEAM_WIRE_HDR_SIZE || wr->len > wr->cap)) { return LDG_ERR_INVALID; }

    if (LDG_UNLIKELY((hdr->flags & LIBRESTEAM_WIRE_FLAG_RESERVED_MASK) != 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    body_len = wr->len - LIBRESTEAM_WIRE_HDR_SIZE;
    body_hash = ldg_emiemi_fnv1a_compute(&wr->buff[LIBRESTEAM_WIRE_HDR_SIZE], body_len);

    wire_uint_st(&wr->buff[WIRE_OFF_PROTO_REV], LIBRESTEAM_WIRE_PROTO_REV, (uint32_t)sizeof(uint16_t));
    wire_uint_st(&wr->buff[WIRE_OFF_SERVICE], hdr->service, (uint32_t)sizeof(uint8_t));
    wire_uint_st(&wr->buff[WIRE_OFF_OP], hdr->op, (uint32_t)sizeof(uint8_t));
    wire_uint_st(&wr->buff[WIRE_OFF_FLAGS], hdr->flags, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_SEQ], hdr->seq, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_STATUS], hdr->status, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_BODY_LEN], body_len, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_BODY_HASH], body_hash, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_CB_ID], hdr->cb_id, (uint32_t)sizeof(uint32_t));
    wire_uint_st(&wr->buff[WIRE_OFF_RESERVED], WIRE_RESERVED_VAL, (uint32_t)sizeof(uint32_t));

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_rd_init(libresteam_wire_rd_t *rd, const uint8_t *payload, uint32_t len, libresteam_wire_hdr_t *hdr_out)
{
    uint32_t body_hash = 0;

    if (LDG_UNLIKELY(!rd || !hdr_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    rd->buff = 0x0;
    rd->len = 0;
    rd->pos = 0;

    if (LDG_UNLIKELY(!payload)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(len < LIBRESTEAM_WIRE_HDR_SIZE)) { return LDG_ERR_NET_PROTOCOL; }

    hdr_out->proto_rev = (uint16_t)wire_uint_ld(&payload[WIRE_OFF_PROTO_REV], (uint32_t)sizeof(uint16_t));
    hdr_out->service = (uint8_t)wire_uint_ld(&payload[WIRE_OFF_SERVICE], (uint32_t)sizeof(uint8_t));
    hdr_out->op = (uint8_t)wire_uint_ld(&payload[WIRE_OFF_OP], (uint32_t)sizeof(uint8_t));
    hdr_out->flags = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_FLAGS], (uint32_t)sizeof(uint32_t));
    hdr_out->seq = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_SEQ], (uint32_t)sizeof(uint32_t));
    hdr_out->status = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_STATUS], (uint32_t)sizeof(uint32_t));
    hdr_out->body_len = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_BODY_LEN], (uint32_t)sizeof(uint32_t));
    hdr_out->body_hash = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_BODY_HASH], (uint32_t)sizeof(uint32_t));
    hdr_out->cb_id = (uint32_t)wire_uint_ld(&payload[WIRE_OFF_CB_ID], (uint32_t)sizeof(uint32_t));

    if (hdr_out->proto_rev != LIBRESTEAM_WIRE_PROTO_REV) { return LDG_ERR_NET_PROTOCOL; }

    if ((hdr_out->flags & LIBRESTEAM_WIRE_FLAG_RESERVED_MASK) != 0) { return LDG_ERR_NET_PROTOCOL; }

    if (hdr_out->body_len != len - LIBRESTEAM_WIRE_HDR_SIZE) { return LDG_ERR_NET_PROTOCOL; }

    body_hash = ldg_emiemi_fnv1a_compute(&payload[LIBRESTEAM_WIRE_HDR_SIZE], hdr_out->body_len);
    if (body_hash != hdr_out->body_hash) { return LDG_ERR_PROTO_EMIEMI_BAD_CKSUM; }

    rd->buff = payload;
    rd->len = len;
    rd->pos = LIBRESTEAM_WIRE_HDR_SIZE;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_u8_get(libresteam_wire_rd_t *rd, uint8_t *out)
{
    uint64_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = wire_uint_get(rd, (uint32_t)sizeof(uint8_t), &val);
    *out = (uint8_t)val;

    return res;
}

uint32_t libresteam_wire_flag_get(libresteam_wire_rd_t *rd, uint8_t *out)
{
    uint64_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = wire_uint_get(rd, (uint32_t)sizeof(uint8_t), &val);
    if (res != LDG_ERR_AOK) { return res; }

    if (val == LDG_TRUTH_FALSE) { return LDG_ERR_AOK; }

    if (val != LDG_TRUTH_TRUE) { return LDG_ERR_NET_PROTOCOL; }

    *out = LDG_TRUTH_TRUE;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_u16_get(libresteam_wire_rd_t *rd, uint16_t *out)
{
    uint64_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = wire_uint_get(rd, (uint32_t)sizeof(uint16_t), &val);
    *out = (uint16_t)val;

    return res;
}

uint32_t libresteam_wire_u32_get(libresteam_wire_rd_t *rd, uint32_t *out)
{
    uint64_t val = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = wire_uint_get(rd, (uint32_t)sizeof(uint32_t), &val);
    *out = (uint32_t)val;

    return res;
}

uint32_t libresteam_wire_u64_get(libresteam_wire_rd_t *rd, uint64_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return wire_uint_get(rd, (uint32_t)sizeof(uint64_t), out);
}

uint32_t libresteam_wire_fp64_get(libresteam_wire_rd_t *rd, double *out)
{
    uint64_t bits = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wire_uint_get(rd, (uint32_t)sizeof(uint64_t), &bits);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_mem_secure_cpy(out, &bits, sizeof(double));
}

uint32_t libresteam_wire_blob_get(libresteam_wire_rd_t *rd, const uint8_t **data_out, uint32_t *len_out)
{
    uint64_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!data_out || !len_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *data_out = 0x0;
    *len_out = 0;

    res = wire_uint_get(rd, (uint32_t)sizeof(uint32_t), &len);
    if (res != LDG_ERR_AOK) { return res; }

    if (LDG_UNLIKELY(len > rd->len - rd->pos)) { return LDG_ERR_EOF; }

    *data_out = &rd->buff[rd->pos];
    *len_out = (uint32_t)len;
    rd->pos += (uint32_t)len;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wire_str_get(libresteam_wire_rd_t *rd, char *out, uint32_t cap)
{
    const uint8_t *data = 0x0;
    uint32_t len = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    out[0] = LDG_STR_TERM;

    res = libresteam_wire_blob_get(rd, &data, &len);
    if (res != LDG_ERR_AOK) { return res; }

    if (LDG_UNLIKELY(len >= cap)) { return LDG_ERR_STR_TRUNC; }

    if (len != 0)
    {
        res = ldg_mem_secure_cpy(out, data, len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    out[len] = LDG_STR_TERM;

    return LDG_ERR_AOK;
}
