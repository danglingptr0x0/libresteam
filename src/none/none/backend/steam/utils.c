#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/utils/ops.h>

#define STEAM_UTILS_ARG_NONE 0U

static uint32_t steam_utils_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->utils)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->utils_desc, method_idx, out);
}

static uint32_t steam_utils_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_utils_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, arg, arg_cunt, out);
}

static uint32_t steam_utils_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(steam->utils, slot, arg, arg_cunt, out);
}

static uint32_t steam_utils_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->utils, slot, arg, arg_cunt);
}

static uint32_t steam_utils_app_active_secs_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_APP_ACTIVE_SECS_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_computer_active_secs_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_COMPUTER_ACTIVE_SECS_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_universe_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_UNIVERSE_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_srv_time_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_SRV_TIME_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_ip_country_get(void *ctx, const char **out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_IP_COUNTRY_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_img_size_get(void *ctx, uint32_t img, uint32_t *width_out, uint32_t *height_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!width_out || !height_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *width_out = 0;
    *height_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = img, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = width_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = height_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_IMG_SIZE_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_utils_img_rgba_get(void *ctx, uint32_t img, uint8_t *dst, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!dst)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = img, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = dst, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_IMG_RGBA_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_utils_battery_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_BATTERY_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_app_id_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_APP_ID_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_notification_pos_set(void *ctx, uint32_t pos)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = pos, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_NOTIFICATION_POS_SET, arg, cunt);
}

static uint32_t steam_utils_call_completed_get(void *ctx, uint64_t call, uint8_t *done_out, uint8_t *failed_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!failed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *failed_out = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .val = call, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = failed_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_utils_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_CALL_COMPLETED_GET, arg, cunt, done_out);
}

static uint32_t steam_utils_call_failure_get(void *ctx, uint64_t call, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = call, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_CALL_FAILURE_GET, arg, cunt, out);
}

static uint32_t steam_utils_ipc_call_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_IPC_CALL_CUNT_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_warning_hook_set(void *ctx, void *hook)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = hook, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_WARNING_HOOK_SET, arg, cunt);
}

static uint32_t steam_utils_overlay_en_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_OVERLAY_EN_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_overlay_present_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_OVERLAY_PRESENT_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_file_sig_ck(void *ctx, const char *name, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_utils_u64_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_FILE_SIG_CK, arg, cunt, call_out);
}

static uint32_t steam_utils_gamepad_text_show(void *ctx, uint32_t mode, uint32_t line_mode, const char *desc, uint32_t char_max, const char *existing)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!desc || !existing)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = mode, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = line_mode, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = char_max, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = existing, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_SHOW, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_utils_gamepad_text_len_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_LEN_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, out);
}

static uint32_t steam_utils_gamepad_text_get(void *ctx, char *text, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!text)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = text, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_utils_ui_lang_get(void *ctx, const char **out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_UI_LANG_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_vr_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_VR_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_notification_inset_set(void *ctx, uint32_t horizontal, uint32_t vertical)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = horizontal, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = vertical, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_NOTIFICATION_INSET_SET, arg, cunt);
}

static uint32_t steam_utils_big_picture_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_BIG_PICTURE_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_vr_dashboard_start(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_VR_DASHBOARD_START, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE);
}

static uint32_t steam_utils_vr_streaming_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_VR_STREAMING_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_vr_streaming_set(void *ctx, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_VR_STREAMING_SET, arg, cunt);
}

static uint32_t steam_utils_china_launcher_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_CHINA_LAUNCHER_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_text_filter_init(void *ctx, uint32_t opts)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = opts, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_TEXT_FILTER_INIT, arg, cunt, LDG_ERR_UNSUPPORTED);
}

static uint32_t steam_utils_text_filter(void *ctx, uint32_t filter_ctx, libresteam_steam_id_t src, const char *text, char *out, uint32_t cap, uint32_t *filtered_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!text || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = filter_ctx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = &src, .size = (uint32_t)sizeof(libresteam_steam_id_t), .kind = LDG_CPP_ARG_KIND_STRUCT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = text, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_TEXT_FILTER, arg, cunt, filtered_out);
}

static uint32_t steam_utils_ipv6_state_get(void *ctx, uint32_t proto, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = proto, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_IPV6_STATE_GET, arg, cunt, out);
}

static uint32_t steam_utils_steam_deck_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_STEAM_DECK_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_gamepad_text_float_show(void *ctx, uint32_t mode, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = mode, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = pos_x, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = pos_y, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = width, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = height, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_FLOAT_SHOW, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_utils_launcher_mode_set(void *ctx, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_utils_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_UTILS_LAUNCHER_MODE_SET, arg, cunt);
}

static uint32_t steam_utils_gamepad_text_float_dismiss(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_FLOAT_DISMISS, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_DENIED; }

    return LDG_ERR_AOK;
}

static uint32_t steam_utils_gamepad_text_dismiss(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_utils_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_UTILS_GAMEPAD_TEXT_DISMISS, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->utils, slot, 0x0, STEAM_UTILS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return LDG_ERR_DENIED; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_utils_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->utils_ops.app_active_secs_get = steam_utils_app_active_secs_get;
    steam->utils_ops.computer_active_secs_get = steam_utils_computer_active_secs_get;
    steam->utils_ops.universe_get = steam_utils_universe_get;
    steam->utils_ops.srv_time_get = steam_utils_srv_time_get;
    steam->utils_ops.ip_country_get = steam_utils_ip_country_get;
    steam->utils_ops.img_size_get = steam_utils_img_size_get;
    steam->utils_ops.img_rgba_get = steam_utils_img_rgba_get;
    steam->utils_ops.battery_get = steam_utils_battery_get;
    steam->utils_ops.app_id_get = steam_utils_app_id_get;
    steam->utils_ops.notification_pos_set = steam_utils_notification_pos_set;
    steam->utils_ops.call_completed_get = steam_utils_call_completed_get;
    steam->utils_ops.call_failure_get = steam_utils_call_failure_get;
    steam->utils_ops.ipc_call_cunt_get = steam_utils_ipc_call_cunt_get;
    steam->utils_ops.warning_hook_set = steam_utils_warning_hook_set;
    steam->utils_ops.overlay_en_get = steam_utils_overlay_en_get;
    steam->utils_ops.overlay_present_get = steam_utils_overlay_present_get;
    steam->utils_ops.file_sig_ck = steam_utils_file_sig_ck;
    steam->utils_ops.gamepad_text_show = steam_utils_gamepad_text_show;
    steam->utils_ops.gamepad_text_len_get = steam_utils_gamepad_text_len_get;
    steam->utils_ops.gamepad_text_get = steam_utils_gamepad_text_get;
    steam->utils_ops.ui_lang_get = steam_utils_ui_lang_get;
    steam->utils_ops.vr_get = steam_utils_vr_get;
    steam->utils_ops.notification_inset_set = steam_utils_notification_inset_set;
    steam->utils_ops.big_picture_get = steam_utils_big_picture_get;
    steam->utils_ops.vr_dashboard_start = steam_utils_vr_dashboard_start;
    steam->utils_ops.vr_streaming_get = steam_utils_vr_streaming_get;
    steam->utils_ops.vr_streaming_set = steam_utils_vr_streaming_set;
    steam->utils_ops.china_launcher_get = steam_utils_china_launcher_get;
    steam->utils_ops.text_filter_init = steam_utils_text_filter_init;
    steam->utils_ops.text_filter = steam_utils_text_filter;
    steam->utils_ops.ipv6_state_get = steam_utils_ipv6_state_get;
    steam->utils_ops.steam_deck_get = steam_utils_steam_deck_get;
    steam->utils_ops.gamepad_text_float_show = steam_utils_gamepad_text_float_show;
    steam->utils_ops.launcher_mode_set = steam_utils_launcher_mode_set;
    steam->utils_ops.gamepad_text_float_dismiss = steam_utils_gamepad_text_float_dismiss;
    steam->utils_ops.gamepad_text_dismiss = steam_utils_gamepad_text_dismiss;
    steam->ops.utils = &steam->utils_ops;

    return LDG_ERR_AOK;
}
