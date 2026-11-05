#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/remote_play/ops.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_REMOTE_PLAY_ARG_NONE 0U
#define STEAM_REMOTE_PLAY_ID_SIZE ((uint32_t)sizeof(libresteam_steam_id_t))
#define STEAM_REMOTE_PLAY_AVATAR_PENDING UINT32_MAX
#define STEAM_REMOTE_PLAY_NONE 0U

static uint32_t steam_remote_play_slot_get(void *ctx, uint32_t method_idx, libresteam_backend_steam_t **steam_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *steam_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->remote_play)) { return LDG_ERR_NOT_INIT; }

    *steam_out = steam;

    return ldg_cpp_slot_get(&steam->remote_play_desc, method_idx, slot_out);
}

static uint32_t steam_remote_play_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_remote_play_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->remote_play, slot, arg, arg_cunt, out);
}

static uint32_t steam_remote_play_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    res = steam_remote_play_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->remote_play, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_remote_play_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_remote_play_slot_get(ctx, method_idx, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->remote_play, slot, arg, arg_cunt);
}

static uint32_t steam_remote_play_sesh_u32_call(void *ctx, uint32_t method_idx, uint32_t sesh_id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_remote_play_u32_call(ctx, method_idx, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == STEAM_REMOTE_PLAY_NONE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_sesh_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_SESH_CUNT_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->remote_play, slot, 0x0, STEAM_REMOTE_PLAY_ARG_NONE, out);
}

static uint32_t steam_remote_play_sesh_id_get(void *ctx, uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    return steam_remote_play_sesh_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_SESH_ID_GET, idx, out);
}

static uint32_t steam_remote_play_together_get(void *ctx, uint32_t sesh_id, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_remote_play_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_TOGETHER_GET, arg, cunt, out);
}

static uint32_t steam_remote_play_steam_id_get(void *ctx, uint32_t sesh_id, libresteam_steam_id_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_STEAM_ID_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = ldg_cpp_struct_call(steam->remote_play, slot, arg, cunt, out, STEAM_REMOTE_PLAY_ID_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    if (out->all == 0) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_guest_id_get(void *ctx, uint32_t sesh_id, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    return steam_remote_play_sesh_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_GUEST_ID_GET, sesh_id, out);
}

static uint32_t steam_remote_play_avatar_get(void *ctx, uint32_t sesh_id, libresteam_remote_play_avatar_size_t size, uint32_t *out)
{
    uint32_t method_idx = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    switch ((uint32_t)size)
    {
        case LIBRESTEAM_REMOTE_PLAY_AVATAR_SMALL:
        { method_idx = LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_AVATAR_SMALL_GET;
          break;
        }
        case LIBRESTEAM_REMOTE_PLAY_AVATAR_MEDIUM:
        { method_idx = LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_AVATAR_MEDIUM_GET;
          break;
        }
        case LIBRESTEAM_REMOTE_PLAY_AVATAR_LARGE:
        { method_idx = LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_AVATAR_LARGE_GET;
          break;
        }
        default: { return LDG_ERR_FUNC_ARG_INVALID;
        }
    }

    res = steam_remote_play_sesh_u32_call(ctx, method_idx, sesh_id, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == STEAM_REMOTE_PLAY_AVATAR_PENDING)
    {
        *out = 0;
        return LDG_ERR_AGAIN;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_client_name_get(void *ctx, uint32_t sesh_id, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    libresteam_backend_steam_t *steam = 0x0;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_CLIENT_NAME_GET, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = ldg_cpp_uint64_call(steam->remote_play, slot, arg, cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_form_factor_get(void *ctx, uint32_t sesh_id, libresteam_remote_play_form_factor_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t factor = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_UNKNOWN;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_remote_play_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_FORM_FACTOR_GET, arg, cunt, &factor);
    if (res != LDG_ERR_AOK) { return res; }

    if (factor > (uint32_t)LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_VR_HEADSET) { return LDG_ERR_RANGE; }

    *out = (libresteam_remote_play_form_factor_t)factor;

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_client_res_get(void *ctx, uint32_t sesh_id, uint32_t *width_out, uint32_t *height_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!width_out || !height_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *width_out = 0;
    *height_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = width_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = height_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_remote_play_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_CLIENT_RES_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_remote_play_ui_show(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_UI_SHOW, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->remote_play, slot, 0x0, STEAM_REMOTE_PLAY_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_invite_send(void *ctx, libresteam_steam_id_t friend_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &friend_id, .size = STEAM_REMOTE_PLAY_ID_SIZE, .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;

    return steam_remote_play_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_INVITE_SEND, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_remote_play_direct_input_en(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_DIRECT_INPUT_EN, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->remote_play, slot, 0x0, STEAM_REMOTE_PLAY_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_direct_input_disable(void *ctx)
{
    libresteam_backend_steam_t *steam = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_remote_play_slot_get(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_DIRECT_INPUT_DISABLE, &steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->remote_play, slot, 0x0, STEAM_REMOTE_PLAY_ARG_NONE);
}

static uint32_t steam_remote_play_input_get(void *ctx, libresteam_remote_play_input_t *input, uint32_t cap, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!input && cap != 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = input, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_remote_play_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_INPUT_GET, arg, cunt, out);
}

static uint32_t steam_remote_play_mouse_visible_set(void *ctx, uint32_t sesh_id, uint8_t visible)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = (visible == LDG_TRUTH_TRUE) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_remote_play_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_MOUSE_VISIBLE_SET, arg, cunt);
}

static uint32_t steam_remote_play_mouse_pos_set(void *ctx, uint32_t sesh_id, libresteam_fp32_t pos_x, libresteam_fp32_t pos_y)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = pos_x, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .fp32 = pos_y, .kind = LDG_CPP_ARG_KIND_FP32 };
    cunt++;

    return steam_remote_play_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_MOUSE_POS_SET, arg, cunt);
}

static uint32_t steam_remote_play_cursor_create(void *ctx, uint32_t width, uint32_t height, uint32_t hot_x, uint32_t hot_y, const void *bgra, uint32_t pitch, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!bgra)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = width, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = height, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = hot_x, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = hot_y, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = bgra, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = pitch, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_remote_play_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_CURSOR_CREATE, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*out == STEAM_REMOTE_PLAY_NONE) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_remote_play_cursor_set(void *ctx, uint32_t sesh_id, uint32_t cursor_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = sesh_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cursor_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_remote_play_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_REMOTE_PLAY_CURSOR_SET, arg, cunt);
}

uint32_t libresteam_backend_steam_remote_play_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->remote_play_ops.sesh_cunt_get = steam_remote_play_sesh_cunt_get;
    steam->remote_play_ops.sesh_id_get = steam_remote_play_sesh_id_get;
    steam->remote_play_ops.together_get = steam_remote_play_together_get;
    steam->remote_play_ops.steam_id_get = steam_remote_play_steam_id_get;
    steam->remote_play_ops.guest_id_get = steam_remote_play_guest_id_get;
    steam->remote_play_ops.avatar_get = steam_remote_play_avatar_get;
    steam->remote_play_ops.client_name_get = steam_remote_play_client_name_get;
    steam->remote_play_ops.form_factor_get = steam_remote_play_form_factor_get;
    steam->remote_play_ops.client_res_get = steam_remote_play_client_res_get;
    steam->remote_play_ops.ui_show = steam_remote_play_ui_show;
    steam->remote_play_ops.invite_send = steam_remote_play_invite_send;
    steam->remote_play_ops.direct_input_en = steam_remote_play_direct_input_en;
    steam->remote_play_ops.direct_input_disable = steam_remote_play_direct_input_disable;
    steam->remote_play_ops.input_get = steam_remote_play_input_get;
    steam->remote_play_ops.mouse_visible_set = steam_remote_play_mouse_visible_set;
    steam->remote_play_ops.mouse_pos_set = steam_remote_play_mouse_pos_set;
    steam->remote_play_ops.cursor_create = steam_remote_play_cursor_create;
    steam->remote_play_ops.cursor_set = steam_remote_play_cursor_set;
    steam->ops.remote_play = &steam->remote_play_ops;

    return LDG_ERR_AOK;
}
