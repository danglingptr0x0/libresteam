#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/remote_play/remote_play.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_VERSION "STEAMREMOTEPLAY_INTERFACE_VERSION001"
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_METHOD_CUNT 7U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_CUNT_GET 0U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_ID_GET 1U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_STEAM_ID_GET 2U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_NAME_GET 3U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_FORM_FACTOR_GET 4U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_RES_GET 5U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_001_INVITE_SEND 6U

#define LIBRESTEAM_SHIM_REMOTE_PLAY_004_VERSION "STEAMREMOTEPLAY_INTERFACE_VERSION004"
#define LIBRESTEAM_SHIM_REMOTE_PLAY_004_METHOD_CUNT 20U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET 0U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET 1U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_TOGETHER_GET 2U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET 3U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_GUEST_ID_GET 4U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_SMALL_GET 5U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_MEDIUM_GET 6U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_LARGE_GET 7U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET 8U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET 9U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET 10U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_UI_SHOW 11U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND 12U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_EN 13U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_DISABLE 14U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_INPUT_GET 15U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_VISIBLE_SET 16U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_POS_SET 17U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_CREATE 18U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_SET 19U

#define LIBRESTEAM_SHIM_REMOTE_PLAY_NONE 0U
#define LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_PENDING UINT32_MAX
#define LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint32_t libresteam_shim_remote_play_set_get(void *self, uint32_t idx, const libresteam_backend_set_t **set_out, uint32_t *idx_out)
{
    libresteam_shim_iface_t *iface = (libresteam_shim_iface_t *)self;
    uint32_t mapped = idx;

    *set_out = 0x0;
    *idx_out = idx;

    if (LDG_UNLIKELY(!iface)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (iface->id == LIBRESTEAM_SHIM_IFACE_REMOTE_PLAY_001)
    {
        switch (idx)
        {
            case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_CUNT_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_ID_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_STEAM_ID_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_NAME_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_FORM_FACTOR_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_RES_GET;
              break;
            }
            case LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND:
            { mapped = LIBRESTEAM_SHIM_REMOTE_PLAY_001_INVITE_SEND;
              break;
            }
            default: { return LDG_ERR_RANGE;
            }
        }
    }

    *idx_out = mapped;

    return libresteam_shim_set_get(self, mapped, set_out);
}

static uint32_t libresteam_shim_remote_play_sesh_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    res = libresteam_remote_play_sesh_cunt_get(set, &cunt);
    res = libresteam_shim_arg_log(self, idx, res, "cunt: %u32", cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return cunt;
}

static uint32_t libresteam_shim_remote_play_sesh_id_get(void *self, uint32_t sesh_idx)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t sesh_id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    res = libresteam_remote_play_sesh_id_get(set, sesh_idx, &sesh_id);
    res = libresteam_shim_arg_log(self, idx, res, "sesh idx: %u32; sesh: %u32", sesh_idx, sesh_id);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return sesh_id;
}

static uint8_t libresteam_shim_remote_play_together_get(void *self, uint32_t sesh_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_TOGETHER_GET, &set, &idx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_remote_play_together_get(set, sesh_id, &flag);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; together: %u8", sesh_id, flag);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (flag == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_remote_play_steam_id_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t), uint32_t sesh_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t id = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET, &set, &idx) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(id); }

    res = libresteam_remote_play_steam_id_get(set, sesh_id, &id);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; steam id: %u64", sesh_id, id.all);
    if (res != LDG_ERR_AOK) { id.all = LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    LDG_CPP_STRUCT_RET(id);
}

static uint32_t libresteam_shim_remote_play_guest_id_get(void *self, uint32_t sesh_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t guest_id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_GUEST_ID_GET, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    res = libresteam_remote_play_guest_id_get(set, sesh_id, &guest_id);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; guest: %u32", sesh_id, guest_id);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return guest_id;
}

static uint32_t libresteam_shim_remote_play_avatar_get(void *self, uint32_t method_idx, uint32_t sesh_id, libresteam_remote_play_avatar_size_t size)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t img = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, method_idx, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    res = libresteam_remote_play_avatar_get(set, sesh_id, size, &img);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; img: %u32", sesh_id, img);
    if (res == LDG_ERR_AGAIN) { return LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_PENDING; }

    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return img;
}

static uint32_t libresteam_shim_remote_play_avatar_small_get(void *self, uint32_t sesh_id)
{
    if (!self) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return libresteam_shim_remote_play_avatar_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_SMALL_GET, sesh_id, LIBRESTEAM_REMOTE_PLAY_AVATAR_SMALL);
}

static uint32_t libresteam_shim_remote_play_avatar_medium_get(void *self, uint32_t sesh_id)
{
    if (!self) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return libresteam_shim_remote_play_avatar_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_MEDIUM_GET, sesh_id, LIBRESTEAM_REMOTE_PLAY_AVATAR_MEDIUM);
}

static uint32_t libresteam_shim_remote_play_avatar_large_get(void *self, uint32_t sesh_id)
{
    if (!self) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return libresteam_shim_remote_play_avatar_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_LARGE_GET, sesh_id, LIBRESTEAM_REMOTE_PLAY_AVATAR_LARGE);
}

static const char* libresteam_shim_remote_play_client_name_get(void *self, uint32_t sesh_id)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *name = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET, &set, &idx) != LDG_ERR_AOK) { return 0x0; }

    res = libresteam_remote_play_client_name_get(set, sesh_id, &name);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32", sesh_id);
    if (res != LDG_ERR_AOK) { return 0x0; }

    return name;
}

static uint32_t libresteam_shim_remote_play_form_factor_get(void *self, uint32_t sesh_id)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_remote_play_form_factor_t factor = LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_UNKNOWN;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET, &set, &idx) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_UNKNOWN; }

    res = libresteam_remote_play_form_factor_get(set, sesh_id, &factor);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; form factor: %u32", sesh_id, (uint32_t)factor);
    if (res != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_UNKNOWN; }

    return (uint32_t)factor;
}

static uint8_t libresteam_shim_remote_play_client_res_get(void *self, uint32_t sesh_id, uint32_t *width_out, uint32_t *height_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t width = 0;
    uint32_t height = 0;
    uint32_t res = LDG_ERR_AOK;

    if (width_out) { *width_out = 0; }

    if (height_out) { *height_out = 0; }

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET, &set, &idx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_remote_play_client_res_get(set, sesh_id, &width, &height);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; width: %u32; height: %u32", sesh_id, width, height);
    if (width_out) { *width_out = width; }

    if (height_out) { *height_out = height; }

    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_remote_play_ui_show(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_UI_SHOW, &set, &idx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_remote_play_ui_show(set);
    res = libresteam_shim_arg_log(self, idx, res, "shown: %u32", res);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_remote_play_invite_send(void *self, uint64_t raw)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t friend_id = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND, &set, &idx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    friend_id.all = raw;
    res = libresteam_remote_play_invite_send(set, friend_id);
    res = libresteam_shim_arg_log(self, idx, res, "friend: %u64", raw);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_remote_play_direct_input_en(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_EN, &set, &idx) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    res = libresteam_remote_play_direct_input_en(set);
    res = libresteam_shim_arg_log(self, idx, res, "direct input: %u32", res);
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static void libresteam_shim_remote_play_direct_input_disable(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_DISABLE, &set, &idx) != LDG_ERR_AOK) { return; }

    res = libresteam_remote_play_direct_input_disable(set);
    res = libresteam_shim_arg_log(self, idx, res, "direct input: %u32", res);
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_remote_play_input_get(void *self, libresteam_remote_play_input_t *input, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_INPUT_GET, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    if (!input) { cap = 0; }

    res = libresteam_remote_play_input_get(set, input, cap, &cunt);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return cunt;
}

static void libresteam_shim_remote_play_mouse_visible_set(void *self, uint32_t sesh_id, uint8_t visible)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_VISIBLE_SET, &set, &idx) != LDG_ERR_AOK) { return; }

    if (visible) { flag = LDG_TRUTH_TRUE; }

    res = libresteam_remote_play_mouse_visible_set(set, sesh_id, flag);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; visible: %u8", sesh_id, flag);
    if (res != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_remote_play_mouse_pos_set(void *self, uint32_t sesh_id, libresteam_fp32_t pos_x, libresteam_fp32_t pos_y)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_POS_SET, &set, &idx) != LDG_ERR_AOK) { return; }

    res = libresteam_remote_play_mouse_pos_set(set, sesh_id, pos_x, pos_y);
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_remote_play_cursor_create(void *self, uint32_t width, uint32_t height, uint32_t hot_x, uint32_t hot_y, const void *bgra, uint32_t pitch)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t cursor_id = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_CREATE, &set, &idx) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    res = libresteam_remote_play_cursor_create(set, width, height, hot_x, hot_y, bgra, pitch, &cursor_id);
    res = libresteam_shim_arg_log(self, idx, res, "width: %u32; height: %u32; cursor: %u32", width, height, cursor_id);
    if (res != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_REMOTE_PLAY_NONE; }

    return cursor_id;
}

static void libresteam_shim_remote_play_cursor_set(void *self, uint32_t sesh_id, uint32_t cursor_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_remote_play_set_get(self, LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_SET, &set, &idx) != LDG_ERR_AOK) { return; }

    res = libresteam_remote_play_cursor_set(set, sesh_id, cursor_id);
    res = libresteam_shim_arg_log(self, idx, res, "sesh: %u32; cursor: %u32", sesh_id, cursor_id);
    if (res != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_remote_play_004_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET:
        { *out = "GetSessionCount";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET:
        { *out = "GetSessionID";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_TOGETHER_GET:
        { *out = "BSessionRemotePlayTogether";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET:
        { *out = "GetSessionSteamID";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_GUEST_ID_GET:
        { *out = "GetSessionGuestID";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_SMALL_GET:
        { *out = "GetSmallSessionAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_MEDIUM_GET:
        { *out = "GetMediumSessionAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_LARGE_GET:
        { *out = "GetLargeSessionAvatar";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET:
        { *out = "GetSessionClientName";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET:
        { *out = "GetSessionClientFormFactor";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET:
        { *out = "BGetSessionClientResolution";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_UI_SHOW:
        { *out = "ShowRemotePlayTogetherUI";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND:
        { *out = "BSendRemotePlayTogetherInvite";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_EN:
        { *out = "BEnableRemotePlayTogetherDirectInput";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_DISABLE:
        { *out = "DisableRemotePlayTogetherDirectInput";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_INPUT_GET:
        { *out = "GetInput";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_VISIBLE_SET:
        { *out = "SetMouseVisibility";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_POS_SET:
        { *out = "SetMousePosition";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_CREATE:
        { *out = "CreateMouseCursor";
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_SET:
        { *out = "SetMouseCursor";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_remote_play_004_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_REMOTE_PLAY_004_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET) { *out = LIBRESTEAM_SHIM_RET_STRUCT; }

    if (idx == LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET) { *out = LIBRESTEAM_SHIM_RET_STR; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_remote_play_004_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_sesh_cunt_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_sesh_id_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_TOGETHER_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_together_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_steam_id_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_GUEST_ID_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_guest_id_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_SMALL_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_avatar_small_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_MEDIUM_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_avatar_medium_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_AVATAR_LARGE_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_avatar_large_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_client_name_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_form_factor_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_client_res_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_UI_SHOW:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_ui_show)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_invite_send)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_EN:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_direct_input_en)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_DIRECT_INPUT_DISABLE:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_direct_input_disable)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_INPUT_GET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_input_get)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_VISIBLE_SET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_mouse_visible_set)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_MOUSE_POS_SET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_mouse_pos_set)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_CREATE:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_cursor_create)
        case LIBRESTEAM_SHIM_REMOTE_PLAY_CURSOR_SET:
            LIBRESTEAM_SHIM_REMOTE_PLAY_IMPL(libresteam_shim_remote_play_cursor_set)
        default: { return LDG_ERR_RANGE;
        }
    }
}

static uint32_t libresteam_shim_remote_play_001_idx_get(uint32_t idx, uint32_t *out)
{
    *out = 0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_CUNT_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_CUNT_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_SESH_ID_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_SESH_ID_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_STEAM_ID_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_STEAM_ID_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_NAME_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_NAME_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_FORM_FACTOR_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_FORM_FACTOR_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_CLIENT_RES_GET:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_CLIENT_RES_GET;
          break;
        }
        case LIBRESTEAM_SHIM_REMOTE_PLAY_001_INVITE_SEND:
        { *out = LIBRESTEAM_SHIM_REMOTE_PLAY_INVITE_SEND;
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_remote_play_001_name_get(uint32_t idx, const char **out)
{
    uint32_t full = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = libresteam_shim_remote_play_001_idx_get(idx, &full);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_shim_remote_play_004_name_get(full, out);
}

static uint32_t libresteam_shim_remote_play_001_kind_get(uint32_t idx, uint32_t *out)
{
    uint32_t full = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    res = libresteam_shim_remote_play_001_idx_get(idx, &full);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_shim_remote_play_004_kind_get(full, out);
}

static uint32_t libresteam_shim_remote_play_001_impl_get(uint32_t idx, void **out)
{
    uint32_t full = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = libresteam_shim_remote_play_001_idx_get(idx, &full);
    if (res != LDG_ERR_AOK) { return res; }

    return libresteam_shim_remote_play_004_impl_get(full, out);
}

uint32_t libresteam_shim_remote_play_001_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_REMOTE_PLAY_001_VERSION;
    out->name_get = libresteam_shim_remote_play_001_name_get;
    out->kind_get = libresteam_shim_remote_play_001_kind_get;
    out->impl_get = libresteam_shim_remote_play_001_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_REMOTE_PLAY_001_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_remote_play_004_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_REMOTE_PLAY_004_VERSION;
    out->name_get = libresteam_shim_remote_play_004_name_get;
    out->kind_get = libresteam_shim_remote_play_004_kind_get;
    out->impl_get = libresteam_shim_remote_play_004_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_REMOTE_PLAY_004_METHOD_CUNT;

    return LDG_ERR_AOK;
}
