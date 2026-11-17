#ifndef LIBRESTEAM_EXT_REPL_EXEC_H
#define LIBRESTEAM_EXT_REPL_EXEC_H

#include <stdint.h>
#include <libresteam/ext/repl/repl.h>

#define LIBRESTEAM_EXT_REPL_EXEC_ARG_CAP 8U

uint32_t libresteam_ext_repl_exec_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_exec_shutdown(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_exec_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args);

#endif
