#ifndef LIBRESTEAM_EXT_RUNTIME_H
#define LIBRESTEAM_EXT_RUNTIME_H

#include <stdint.h>

#include <dangling/core/macros.h>

#define LIBRESTEAM_EXT_RUNTIME_VERSION 1U
#define LIBRESTEAM_EXT_RUNTIME_NAME_CAP 128U
#define LIBRESTEAM_EXT_RUNTIME_WAIT_MS 15000U
#define LIBRESTEAM_EXT_RUNTIME_START_SYM "libresteam_ext_runtime_init"
#define LIBRESTEAM_EXT_RUNTIME_STOP_FMT "Local\\LibreSteam-runtime-stop-%u32"
#define LIBRESTEAM_EXT_RUNTIME_READY_FMT "Local\\LibreSteam-runtime-ready-%u32-%u32"

typedef struct libresteam_ext_runtime_req
{
    uint32_t size;
    uint32_t version;
    uint64_t window;
    uint32_t res;
    uint32_t thread_id;
    uint32_t win_err;
    uint32_t reserved;
    char ready[LIBRESTEAM_EXT_RUNTIME_NAME_CAP];
} libresteam_ext_runtime_req_t;

LDG_EXPORT uint32_t libresteam_ext_runtime_init(void *arg);

#endif
