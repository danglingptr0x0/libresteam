#ifndef LIBRESTEAM_SRV_H
#define LIBRESTEAM_SRV_H

#include <stdint.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>
#include <dangling/mem/alloc.h>
#include <dangling/net/sock.h>
#include <dangling/proto/emiemi.h>

#include <libresteam/friends/ops.h>
#include <libresteam/wire/wire.h>

#define LIBRESTEAM_SRV_LISTENER_CUNT 2U
#define LIBRESTEAM_SRV_CLIENT_MAX (LDG_NET_SOCK_WAIT_MAX - LIBRESTEAM_SRV_LISTENER_CUNT)
#define LIBRESTEAM_SRV_IMG_CAP (LDG_KIB * LDG_KIB)
#define LIBRESTEAM_SRV_DIO_BLK (4U * LDG_KIB)
#define LIBRESTEAM_SRV_DIO_MASK (LIBRESTEAM_SRV_DIO_BLK - 1U)
#define LIBRESTEAM_SRV_ACCOUNT_MAGIC 0x4153524CU
#define LIBRESTEAM_SRV_ACCOUNT_REV 1U
#define LIBRESTEAM_SRV_ACCOUNT_FIRST 1U
#define LIBRESTEAM_SRV_DIR_NAME "libresteam"
#define LIBRESTEAM_SRV_CONF_NAME "libresteam-srv.conf"
#define LIBRESTEAM_SRV_ACCOUNT_EXT ".bin"
#define LIBRESTEAM_SRV_PART_EXT ".part"
#define LIBRESTEAM_SRV_QUOTA_NONE UINT64_MAX
#define LIBRESTEAM_SRV_QUOTA_DEFAULT ((uint64_t)LDG_KIB * LDG_KIB * LDG_KIB)
#define LIBRESTEAM_SRV_FRAME_CAP ((LIBRESTEAM_WIRE_PAYLOAD_MAX + LIBRESTEAM_SRV_DIO_MASK) & ~LIBRESTEAM_SRV_DIO_MASK)
#define LIBRESTEAM_SRV_FILE_ACCESS 0600U
#define LIBRESTEAM_SRV_DIR_ACCESS 0700U
#define LIBRESTEAM_SRV_TIMEOUT LDG_NET_SOCK_TIMEOUT_DEFAULT
#define LIBRESTEAM_SRV_NAME_CAP (LIBRESTEAM_WIRE_ACCOUNT_NAME_MAX + 1U)
#define LIBRESTEAM_SRV_PRESENCE_KEY_CAP (LIBRESTEAM_FRIENDS_RICH_PRESENCE_KEY_LEN_MAX + 1U)
#define LIBRESTEAM_SRV_PRESENCE_VAL_CAP (LIBRESTEAM_FRIENDS_RICH_PRESENCE_VAL_LEN_MAX + 1U)
#define LIBRESTEAM_SRV_ACCOUNT_NONE 0U

typedef enum libresteam_srv_rec_kind
{
    LIBRESTEAM_SRV_REC_KIND_NONE = 0,
    LIBRESTEAM_SRV_REC_KIND_STAT_INT,
    LIBRESTEAM_SRV_REC_KIND_STAT_FP64,
    LIBRESTEAM_SRV_REC_KIND_STAT_AVG_RATE,
    LIBRESTEAM_SRV_REC_KIND_ACHIEVEMENT,
    LIBRESTEAM_SRV_REC_KIND_PLAYED_WITH,
    LIBRESTEAM_SRV_REC_KIND_CLOUD_APP_EN,
    LIBRESTEAM_SRV_REC_KIND_STATS_RST,
    LIBRESTEAM_SRV_REC_KIND_MAX
} libresteam_srv_rec_kind_t;

typedef struct libresteam_srv_rec
{
    const uint8_t *key;
    const uint8_t *val;
    uint32_t app_id;
    uint32_t key_len;
    uint32_t val_len;
    uint8_t kind;
    uint8_t pudding[3];
} libresteam_srv_rec_t;

typedef struct libresteam_srv_presence
{
    char key[LIBRESTEAM_SRV_PRESENCE_KEY_CAP];
    char val[LIBRESTEAM_SRV_PRESENCE_VAL_CAP];
} libresteam_srv_presence_t;

typedef struct libresteam_srv_client
{
    ldg_net_sock_t sock;
    ldg_emiemi_io_ctx_t io;
    uint8_t *img;
    uint32_t img_len;
    uint32_t account;
    uint32_t app_id;
    uint32_t presence_cunt;
    uint8_t live;
    uint8_t hello_done;
    char name[LIBRESTEAM_SRV_NAME_CAP];
    libresteam_srv_presence_t presence[LIBRESTEAM_FRIENDS_RICH_PRESENCE_KEY_CUNT_MAX];
    uint8_t pudding[1];
} libresteam_srv_client_t;

typedef struct libresteam_srv
{
    ldg_net_sock_t listener[LIBRESTEAM_SRV_LISTENER_CUNT];
    libresteam_srv_client_t client[LIBRESTEAM_SRV_CLIENT_MAX];
    ldg_net_sock_t wait_sock[LDG_NET_SOCK_WAIT_MAX];
    ldg_mem_pool_t *pool;
    uint8_t *frame;
    uint8_t *reply;
    uint8_t *blk;
    uint8_t *compact;
    uint64_t quota;
    uint32_t account_next;
    uint32_t listener_cunt;
    uint32_t wait_client[LDG_NET_SOCK_WAIT_MAX];
    uint8_t wait_ready[LDG_NET_SOCK_WAIT_MAX];
    char data_dir[LDG_IO_PATH_MAX];
    char path[LDG_IO_PATH_MAX];
    char path_alt[LDG_IO_PATH_MAX];
    char path_dir[LDG_IO_PATH_MAX];
    char path_scan[LDG_IO_PATH_MAX];
    uint16_t port;
    uint8_t pudding[6];
} libresteam_srv_t;

uint32_t libresteam_srv_init(libresteam_srv_t *srv);
uint32_t libresteam_srv_shutdown(libresteam_srv_t *srv);
uint32_t libresteam_srv_run(libresteam_srv_t *srv);

uint32_t libresteam_srv_conf_rd(libresteam_srv_t *srv);

uint32_t libresteam_srv_dio_rd(const char *path, uint8_t *dst, uint32_t cap, uint32_t *len_out);
uint32_t libresteam_srv_dio_wr(const char *path, const uint8_t *src, uint32_t off, uint32_t end, uint8_t create);
uint32_t libresteam_srv_dio_range_rd(const char *path, uint8_t *blk_buff, uint32_t blk_cap, uint32_t off, uint32_t len, const uint8_t **data_out, uint32_t *len_out);
uint32_t libresteam_srv_dio_patch(const char *path, uint8_t *blk, uint32_t off, const uint8_t *data, uint32_t len, uint32_t total);

uint8_t libresteam_srv_account_name_valid_is(const char *name);
uint32_t libresteam_srv_account_path_get(const libresteam_srv_t *srv, const char *name, char *out);
uint32_t libresteam_srv_account_open(libresteam_srv_t *srv, libresteam_srv_client_t *client);
uint32_t libresteam_srv_account_rec_append(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t kind, const void *key, uint32_t key_len, const void *val, uint32_t val_len);
uint32_t libresteam_srv_account_rec_next(const libresteam_srv_client_t *client, uint32_t *pos, libresteam_srv_rec_t *out);
uint32_t libresteam_srv_account_next_scan(libresteam_srv_t *srv);

uint32_t libresteam_srv_friends_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr);
uint32_t libresteam_srv_cloud_handle(libresteam_srv_t *srv, libresteam_srv_client_t *client, uint8_t op, libresteam_wire_rd_t *rd, libresteam_wire_wr_t *wr);

#endif
