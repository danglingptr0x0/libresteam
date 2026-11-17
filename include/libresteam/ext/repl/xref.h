#ifndef LIBRESTEAM_EXT_REPL_XREF_H
#define LIBRESTEAM_EXT_REPL_XREF_H

#include <stdint.h>

#define LIBRESTEAM_EXT_REPL_XREF_INSN_CAP 15U

typedef struct libresteam_ext_repl_xref
{
    uint64_t tgt;
    const char *kind;
    uint32_t len;
    uint8_t pudding[4];
} libresteam_ext_repl_xref_t;

uint32_t libresteam_ext_repl_xref_decode(const uint8_t *bytes, uint32_t len, uint64_t addr, libresteam_ext_repl_xref_t *out);

#endif
