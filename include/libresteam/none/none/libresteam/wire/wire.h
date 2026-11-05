#ifndef LIBRESTEAM_WIRE_H
#define LIBRESTEAM_WIRE_H

#include <stdint.h>
#include <dangling/core/macros.h>
#include <dangling/proto/emiemi.h>

#define LIBRESTEAM_WIRE_PROTO_REV 1U
#define LIBRESTEAM_WIRE_HDR_SIZE 32U
#define LIBRESTEAM_WIRE_PAYLOAD_MAX LDG_EMIEMI_MAX_PAYLOAD
#define LIBRESTEAM_WIRE_BODY_MAX (LIBRESTEAM_WIRE_PAYLOAD_MAX - LIBRESTEAM_WIRE_HDR_SIZE)
#define LIBRESTEAM_WIRE_FLAG_REPLY 0x00000001U
#define LIBRESTEAM_WIRE_FLAG_PUSH 0x00000002U
#define LIBRESTEAM_WIRE_FLAG_RESERVED_MASK 0xFFFFFFFCU
#define LIBRESTEAM_WIRE_SEQ_PUSH 0U
#define LIBRESTEAM_WIRE_DEFAULT_PORT 27015U
#define LIBRESTEAM_WIRE_SESH_OP_HELLO 0U
#define LIBRESTEAM_WIRE_ACCOUNT_NAME_MAX 64U

typedef enum libresteam_wire_service
{
    LIBRESTEAM_WIRE_SERVICE_SESH = 0,
    LIBRESTEAM_WIRE_SERVICE_USER_STATS,
    LIBRESTEAM_WIRE_SERVICE_FRIENDS,
    LIBRESTEAM_WIRE_SERVICE_CLOUD,
    LIBRESTEAM_WIRE_SERVICE_LOBBY,
    LIBRESTEAM_WIRE_SERVICE_SRV_BROWSER,
    LIBRESTEAM_WIRE_SERVICE_MAX
} libresteam_wire_service_t;

typedef struct libresteam_wire_hdr
{
    uint32_t flags;
    uint32_t seq;
    uint32_t status;
    uint32_t body_len;
    uint32_t body_hash;
    uint32_t cb_id;
    uint16_t proto_rev;
    uint8_t service;
    uint8_t op;
} libresteam_wire_hdr_t;

typedef struct libresteam_wire_wr
{
    uint8_t *buff;
    uint32_t cap;
    uint32_t len;
} libresteam_wire_wr_t;

typedef struct libresteam_wire_rd
{
    const uint8_t *buff;
    uint32_t len;
    uint32_t pos;
} libresteam_wire_rd_t;

LDG_EXPORT uint32_t libresteam_wire_wr_init(libresteam_wire_wr_t *wr, uint8_t *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wire_u8_put(libresteam_wire_wr_t *wr, uint8_t val);
LDG_EXPORT uint32_t libresteam_wire_u16_put(libresteam_wire_wr_t *wr, uint16_t val);
LDG_EXPORT uint32_t libresteam_wire_u32_put(libresteam_wire_wr_t *wr, uint32_t val);
LDG_EXPORT uint32_t libresteam_wire_u64_put(libresteam_wire_wr_t *wr, uint64_t val);
LDG_EXPORT uint32_t libresteam_wire_fp64_put(libresteam_wire_wr_t *wr, double val);
LDG_EXPORT uint32_t libresteam_wire_blob_put(libresteam_wire_wr_t *wr, const void *data, uint32_t len);
LDG_EXPORT uint32_t libresteam_wire_str_put(libresteam_wire_wr_t *wr, const char *str);
LDG_EXPORT uint32_t libresteam_wire_hdr_finish(libresteam_wire_wr_t *wr, const libresteam_wire_hdr_t *hdr);

LDG_EXPORT uint32_t libresteam_wire_rd_init(libresteam_wire_rd_t *rd, const uint8_t *payload, uint32_t len, libresteam_wire_hdr_t *hdr_out);
LDG_EXPORT uint32_t libresteam_wire_u8_get(libresteam_wire_rd_t *rd, uint8_t *out);
LDG_EXPORT uint32_t libresteam_wire_flag_get(libresteam_wire_rd_t *rd, uint8_t *out);
LDG_EXPORT uint32_t libresteam_wire_u16_get(libresteam_wire_rd_t *rd, uint16_t *out);
LDG_EXPORT uint32_t libresteam_wire_u32_get(libresteam_wire_rd_t *rd, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wire_u64_get(libresteam_wire_rd_t *rd, uint64_t *out);
LDG_EXPORT uint32_t libresteam_wire_fp64_get(libresteam_wire_rd_t *rd, double *out);
LDG_EXPORT uint32_t libresteam_wire_blob_get(libresteam_wire_rd_t *rd, const uint8_t **data_out, uint32_t *len_out);
LDG_EXPORT uint32_t libresteam_wire_str_get(libresteam_wire_rd_t *rd, char *out, uint32_t cap);

#endif
