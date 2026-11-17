#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/io/path.h>

#include <libresteam/steam/steam.h>

#define STEAM_PATH_SDK_REL ".steam/sdk64/steamclient.so"
#define STEAM_PATH_INSTALL_REL ".local/share/Steam/linux64/steamclient.so"
#define STEAM_PATH_PROG_X86 "C:/Program Files (x86)/Steam/steamclient64.dll"
#define STEAM_PATH_PROG "C:/Program Files/Steam/steamclient64.dll"
#define STEAM_PATH_IDX_SDK 0U
#define STEAM_PATH_IDX_INSTALL 1U
#define STEAM_PATH_IDX_PROG_X86 2U
#define STEAM_PATH_IDX_PROG 3U

uint32_t libresteam_steam_path_default_get(uint32_t idx, char *buff, uint64_t cap)
{
    char user_dir[LDG_IO_PATH_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    if (idx == STEAM_PATH_IDX_PROG_X86) { return ldg_io_path_normalize(STEAM_PATH_PROG_X86, buff, cap); }

    if (idx == STEAM_PATH_IDX_PROG) { return ldg_io_path_normalize(STEAM_PATH_PROG, buff, cap); }

    if (idx != STEAM_PATH_IDX_SDK && idx != STEAM_PATH_IDX_INSTALL) { return LDG_ERR_NOT_FOUND; }

    res = ldg_io_path_user_get(user_dir, LDG_IO_PATH_MAX);
    if (res != LDG_ERR_AOK) { return res; }

    if (idx == STEAM_PATH_IDX_SDK) { return ldg_io_path_join(user_dir, STEAM_PATH_SDK_REL, buff, cap); }

    return ldg_io_path_join(user_dir, STEAM_PATH_INSTALL_REL, buff, cap);
}
