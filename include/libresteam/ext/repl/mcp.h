#ifndef LIBRESTEAM_EXT_REPL_MCP_H
#define LIBRESTEAM_EXT_REPL_MCP_H

#include <stdint.h>

#define LIBRESTEAM_EXT_REPL_MCP_PORT 7797U
#define LIBRESTEAM_EXT_REPL_MCP_POLL_MS 50U

uint32_t libresteam_ext_repl_mcp_init(void **out, uint16_t port);
uint32_t libresteam_ext_repl_mcp_stop(void *opaque);
uint32_t libresteam_ext_repl_mcp_poll(void *opaque, char *cmd, uint32_t cap, uint8_t *ready);
uint32_t libresteam_ext_repl_mcp_wr(void *opaque, const char *text, uint32_t len);
uint32_t libresteam_ext_repl_mcp_finish(void *opaque, uint32_t res);

#endif
