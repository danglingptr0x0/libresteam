#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/srv/srv.h>
#include <libresteam/wire/wire.h>

#define SRV_CONF_KEY_QUOTA "quota"
#define SRV_CONF_KEY_PORT "port"
#define SRV_CONF_CH_SPACE ' '
#define SRV_CONF_CH_TAB '\t'
#define SRV_CONF_CH_CR '\r'
#define SRV_CONF_CH_LF '\n'
#define SRV_CONF_DIGIT_FIRST '0'
#define SRV_CONF_DIGIT_LAST '9'
#define SRV_CONF_NUM_CAP 21U
#define SRV_CONF_PORT_MAX 0xFFFFU

static uint8_t srv_conf_blank_is(char ch)
{
    if (ch == SRV_CONF_CH_SPACE || ch == SRV_CONF_CH_TAB || ch == SRV_CONF_CH_CR) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t srv_conf_digit_is(char ch)
{
    if (ch >= SRV_CONF_DIGIT_FIRST && ch <= SRV_CONF_DIGIT_LAST) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t srv_conf_key_eq_is(const char *key, uint32_t key_len, const char *want, uint32_t want_len)
{
    if (key_len != want_len) { return LDG_TRUTH_FALSE; }

    return ldg_mem_secure_eq_is(key, want, key_len);
}

static uint32_t srv_conf_num_parse(const char *text, uint32_t len, uint64_t max, uint64_t *out)
{
    char num[SRV_CONF_NUM_CAP] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    if (len == 0 || len >= SRV_CONF_NUM_CAP) { return LDG_ERR_IO_FMT; }

    res = ldg_mem_secure_cpy(num, text, len);
    if (res != LDG_ERR_AOK) { return res; }

    num[len] = LDG_STR_TERM;

    res = ldg_str_uint_parse(num, LDG_BASE_DEC, max, out);
    if (res != LDG_ERR_AOK) { return LDG_ERR_IO_FMT; }

    return LDG_ERR_AOK;
}

static uint32_t srv_conf_line_apply(libresteam_srv_t *srv, const char *key, uint32_t key_len, const char *val, uint32_t val_len)
{
    uint64_t num = 0;
    uint32_t res = LDG_ERR_AOK;

    if (srv_conf_key_eq_is(key, key_len, SRV_CONF_KEY_QUOTA, (uint32_t)(sizeof(SRV_CONF_KEY_QUOTA) - LDG_STR_TERM_SIZE)) == LDG_TRUTH_TRUE)
    {
        res = srv_conf_num_parse(val, val_len, UINT64_MAX, &num);
        if (res != LDG_ERR_AOK) { return res; }

        srv->quota = num;
        return LDG_ERR_AOK;
    }

    if (srv_conf_key_eq_is(key, key_len, SRV_CONF_KEY_PORT, (uint32_t)(sizeof(SRV_CONF_KEY_PORT) - LDG_STR_TERM_SIZE)) == LDG_TRUTH_TRUE)
    {
        res = srv_conf_num_parse(val, val_len, SRV_CONF_PORT_MAX, &num);
        if (res != LDG_ERR_AOK) { return res; }

        if (num == 0) { return LDG_ERR_IO_FMT; }

        srv->port = (uint16_t)num;
        return LDG_ERR_AOK;
    }

    return LDG_ERR_IO_FMT;
}

static uint32_t srv_conf_parse(libresteam_srv_t *srv, const char *text, uint32_t len)
{
    const char *key = 0x0;
    const char *val = 0x0;
    uint32_t key_len = 0;
    uint32_t val_len = 0;
    uint32_t pos = 0;
    uint32_t res = LDG_ERR_AOK;

    while (pos < len)
    {
        while (pos < len && (srv_conf_blank_is(text[pos]) == LDG_TRUTH_TRUE || text[pos] == SRV_CONF_CH_LF)) { pos++; }
        if (pos >= len) { break; }

        key = &text[pos];
        key_len = 0;
        while (pos < len && srv_conf_blank_is(text[pos]) != LDG_TRUTH_TRUE && text[pos] != SRV_CONF_CH_LF) { pos++; key_len++; }

        while (pos < len && srv_conf_blank_is(text[pos]) == LDG_TRUTH_TRUE) { pos++; }

        val = &text[pos];
        val_len = 0;
        while (pos < len && srv_conf_digit_is(text[pos]) == LDG_TRUTH_TRUE) { pos++; val_len++; }

        while (pos < len && srv_conf_blank_is(text[pos]) == LDG_TRUTH_TRUE) { pos++; }
        if (pos < len && text[pos] != SRV_CONF_CH_LF) { return LDG_ERR_IO_FMT; }

        res = srv_conf_line_apply(srv, key, key_len, val, val_len);
        if (res != LDG_ERR_AOK) { return res; }
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_srv_conf_rd(libresteam_srv_t *srv)
{
    uint32_t len = 0;
    uint32_t cap = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!srv)) { return LDG_ERR_FUNC_ARG_NULL; }

    srv->port = LIBRESTEAM_WIRE_DEFAULT_PORT;
    srv->quota = LIBRESTEAM_SRV_QUOTA_DEFAULT;

    res = ldg_io_path_join(srv->data_dir, LIBRESTEAM_SRV_CONF_NAME, srv->path, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (ldg_io_path_exists_is(srv->path) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

    cap = LIBRESTEAM_SRV_FRAME_CAP;

    res = libresteam_srv_dio_rd(srv->path, srv->frame, cap, &len);
    if (res != LDG_ERR_AOK) { return res; }

    return srv_conf_parse(srv, (const char *)srv->frame, len);
}
