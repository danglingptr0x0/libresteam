#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>

#include <libresteam/backend/steam/steam.h>

#define STEAM_PATH_PROG_X86 "C:/Program Files (x86)/Steam/steamclient64.dll"
#define STEAM_PATH_PROG "C:/Program Files/Steam/steamclient64.dll"
#define STEAM_PATH_IDX_PROG_X86 0U
#define STEAM_PATH_IDX_PROG 1U

uint32_t libresteam_backend_steam_path_default_get(uint32_t idx, char *buff, uint64_t cap)
{
    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    if (idx == STEAM_PATH_IDX_PROG_X86) { return ldg_io_path_normalize(STEAM_PATH_PROG_X86, buff, cap); }

    if (idx == STEAM_PATH_IDX_PROG) { return ldg_io_path_normalize(STEAM_PATH_PROG, buff, cap); }

    return LDG_ERR_NOT_FOUND;
}
