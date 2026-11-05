#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/apps/ops.h>
#include <libresteam/backend/steam/steam.h>
#include <libresteam/steam_id/steam_id.h>

#define STEAM_APPS_SIGN_BIT 0x80000000U
#define STEAM_APPS_ARG_NONE 0U

static uint32_t steam_apps_slot_get(const libresteam_backend_steam_t *steam, uint32_t method_idx, uint32_t *out)
{
    *out = 0;

    if (LDG_UNLIKELY(!steam->apps)) { return LDG_ERR_NOT_INIT; }

    return ldg_cpp_slot_get(&steam->apps_desc, method_idx, out);
}

static uint32_t steam_apps_flag_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_apps_flag_call(ctx, method_idx, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_TRUE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->apps, slot, arg, arg_cunt, out);
}

static uint32_t steam_apps_cunt_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_apps_u32_call(ctx, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_APPS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_INIT;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_str_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, const char **out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->apps, slot, arg, arg_cunt, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, method_idx, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->apps, slot, arg, arg_cunt);
}

static uint32_t steam_apps_subscribed_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_SUBSCRIBED_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_low_violence_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_LOW_VIOLENCE_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_cybercafe_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_CYBERCAFE_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_vac_banned_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_VAC_BANNED_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_game_lang_get(void *ctx, const char **out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_GAME_LANG_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_avail_lang_get(void *ctx, const char **out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint64_t str = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_AVAIL_LANG_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint64_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &str);
    if (res != LDG_ERR_AOK) { return res; }

    if (str == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const char *)(uintptr_t)str;

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_app_subscribed_get(void *ctx, uint32_t app_id, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_APP_SUBSCRIBED_GET, arg, cunt, out);
}

static uint32_t steam_apps_dlc_installed_get(void *ctx, uint32_t app_id, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_INSTALLED_GET, arg, cunt, out);
}

static uint32_t steam_apps_purchase_time_get(void *ctx, uint32_t app_id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_PURCHASE_TIME_GET, arg, cunt, out);
}

static uint32_t steam_apps_free_weekend_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_FREE_WEEKEND_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_dlc_cunt_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_CUNT_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint32_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_APPS_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_INIT;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_dlc_get(void *ctx, uint32_t idx, uint32_t *app_id_out, uint8_t *avail_out, char *name, uint32_t name_cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!app_id_out || !avail_out || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    *app_id_out = 0;
    *avail_out = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = app_id_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = avail_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = name_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_apps_dlc_install(void *ctx, uint32_t app_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_INSTALL, arg, cunt);
}

static uint32_t steam_apps_dlc_uninstall(void *ctx, uint32_t app_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_UNINSTALL, arg, cunt);
}

static uint32_t steam_apps_purchase_key_req(void *ctx, uint32_t app_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_PURCHASE_KEY_REQ, arg, cunt);
}

static uint32_t steam_apps_beta_name_get(void *ctx, char *name, uint32_t name_cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = name_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_BETA_NAME_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_apps_content_corrupt_mark(void *ctx, uint8_t missing_only)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = missing_only, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_CONTENT_CORRUPT_MARK, arg, cunt, LDG_ERR_DENIED);
}

static uint32_t steam_apps_depots_get(void *ctx, uint32_t app_id, uint32_t *depot, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!depot)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = depot, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DEPOTS_GET, arg, cunt, cunt_out);
}

static uint32_t steam_apps_install_dir_get(void *ctx, uint32_t app_id, char *dir, uint32_t cap, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!dir)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = dir, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_INSTALL_DIR_GET, arg, cunt, len_out);
}

static uint32_t steam_apps_app_installed_get(void *ctx, uint32_t app_id, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_APP_INSTALLED_GET, arg, cunt, out);
}

static uint32_t steam_apps_owner_get(void *ctx, libresteam_steam_id_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->all = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_OWNER_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_struct_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, out, (uint32_t)sizeof(libresteam_steam_id_t));
}

static uint32_t steam_apps_launch_query_param_get(void *ctx, const char *key, const char **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_str_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_LAUNCH_QUERY_PARAM_GET, arg, cunt, out);
}

static uint32_t steam_apps_dlc_progress_get(void *ctx, uint32_t app_id, uint64_t *done_out, uint64_t *total_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!done_out || !total_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *done_out = 0;
    *total_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = done_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = total_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_PROGRESS_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_apps_build_id_get(void *ctx, uint32_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_BUILD_ID_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, out);
}

static uint32_t steam_apps_purchase_key_all_req(void *ctx)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_PURCHASE_KEY_ALL_REQ, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE);
}

static uint32_t steam_apps_file_details_req(void *ctx, const char *name, uint64_t *call_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    if (LDG_UNLIKELY(!steam || !name)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_FILE_DETAILS_REQ, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return ldg_cpp_uint64_call(steam->apps, slot, arg, cunt, call_out);
}

static uint32_t steam_apps_launch_cmd_line_get(void *ctx, char *cmd_line, uint32_t cap, uint32_t *len_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cmd_line)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = cmd_line, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_LAUNCH_CMD_LINE_GET, arg, cunt, len_out);
}

static uint32_t steam_apps_family_sharing_get(void *ctx, uint8_t *out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_apps_slot_get(steam, LIBRESTEAM_BACKEND_STEAM_APPS_FAMILY_SHARING_GET, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(steam->apps, slot, 0x0, STEAM_APPS_ARG_NONE, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_apps_timed_trial_get(void *ctx, uint8_t *trial_out, uint32_t *allowed_out, uint32_t *played_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!allowed_out || !played_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *allowed_out = 0;
    *played_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = allowed_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = played_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_flag_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_TIMED_TRIAL_GET, arg, cunt, trial_out);
}

static uint32_t steam_apps_dlc_ctx_set(void *ctx, uint32_t app_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_DLC_CTX_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_apps_beta_cunt_get(void *ctx, uint32_t *cunt_out, uint32_t *avail_out, uint32_t *priv_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!avail_out || !priv_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *avail_out = 0;
    *priv_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = avail_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = priv_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_cunt_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_BETA_CUNT_GET, arg, cunt, cunt_out);
}

static uint32_t steam_apps_beta_info_get(void *ctx, uint32_t idx, uint32_t *flags_out, uint32_t *build_id_out, char *name, uint32_t name_cap, char *desc, uint32_t desc_cap, uint32_t *updated_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!flags_out || !build_id_out || !name || !desc || !updated_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *flags_out = 0;
    *build_id_out = 0;
    *updated_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = flags_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = build_id_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = name_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = desc_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = updated_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_BETA_INFO_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_apps_beta_active_set(void *ctx, const char *name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_apps_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_APPS_BETA_ACTIVE_SET, arg, cunt, LDG_ERR_DENIED);
}

uint32_t libresteam_backend_steam_apps_ops_fill(libresteam_backend_steam_t *steam)
{
    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    steam->apps_ops.subscribed_get = steam_apps_subscribed_get;
    steam->apps_ops.low_violence_get = steam_apps_low_violence_get;
    steam->apps_ops.cybercafe_get = steam_apps_cybercafe_get;
    steam->apps_ops.vac_banned_get = steam_apps_vac_banned_get;
    steam->apps_ops.game_lang_get = steam_apps_game_lang_get;
    steam->apps_ops.avail_lang_get = steam_apps_avail_lang_get;
    steam->apps_ops.app_subscribed_get = steam_apps_app_subscribed_get;
    steam->apps_ops.dlc_installed_get = steam_apps_dlc_installed_get;
    steam->apps_ops.purchase_time_get = steam_apps_purchase_time_get;
    steam->apps_ops.free_weekend_get = steam_apps_free_weekend_get;
    steam->apps_ops.dlc_cunt_get = steam_apps_dlc_cunt_get;
    steam->apps_ops.dlc_get = steam_apps_dlc_get;
    steam->apps_ops.dlc_install = steam_apps_dlc_install;
    steam->apps_ops.dlc_uninstall = steam_apps_dlc_uninstall;
    steam->apps_ops.purchase_key_req = steam_apps_purchase_key_req;
    steam->apps_ops.beta_name_get = steam_apps_beta_name_get;
    steam->apps_ops.content_corrupt_mark = steam_apps_content_corrupt_mark;
    steam->apps_ops.depots_get = steam_apps_depots_get;
    steam->apps_ops.install_dir_get = steam_apps_install_dir_get;
    steam->apps_ops.app_installed_get = steam_apps_app_installed_get;
    steam->apps_ops.owner_get = steam_apps_owner_get;
    steam->apps_ops.launch_query_param_get = steam_apps_launch_query_param_get;
    steam->apps_ops.dlc_progress_get = steam_apps_dlc_progress_get;
    steam->apps_ops.build_id_get = steam_apps_build_id_get;
    steam->apps_ops.purchase_key_all_req = steam_apps_purchase_key_all_req;
    steam->apps_ops.file_details_req = steam_apps_file_details_req;
    steam->apps_ops.launch_cmd_line_get = steam_apps_launch_cmd_line_get;
    steam->apps_ops.family_sharing_get = steam_apps_family_sharing_get;
    steam->apps_ops.timed_trial_get = steam_apps_timed_trial_get;
    steam->apps_ops.dlc_ctx_set = steam_apps_dlc_ctx_set;
    steam->apps_ops.beta_cunt_get = steam_apps_beta_cunt_get;
    steam->apps_ops.beta_info_get = steam_apps_beta_info_get;
    steam->apps_ops.beta_active_set = steam_apps_beta_active_set;
    steam->ops.apps = &steam->apps_ops;

    return LDG_ERR_AOK;
}
