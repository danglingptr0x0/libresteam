#ifndef LIBRESTEAM_EXT_REPL_SCAN_H
#define LIBRESTEAM_EXT_REPL_SCAN_H

#include <stdint.h>

#include <libresteam/ext/repl/repl.h>

uint32_t libresteam_ext_repl_scan_init(libresteam_ext_repl_ctx_t *ctx, uint64_t excluded_addr, uint64_t excluded_len);
uint32_t libresteam_ext_repl_scan_shutdown(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_scan_tick(void *user);
uint32_t libresteam_ext_repl_scan_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args);
uint8_t libresteam_ext_repl_scan_owned_is(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint64_t len);

#endif
