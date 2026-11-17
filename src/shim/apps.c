#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/apps/apps.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_SHIM_APPS_VERSION "STEAMAPPS_INTERFACE_VERSION008"
#define LIBRESTEAM_SHIM_APPS_METHOD_CUNT 33U
#define LIBRESTEAM_SHIM_APPS_SUBSCRIBED_GET 0U
#define LIBRESTEAM_SHIM_APPS_LOW_VIOLENCE_GET 1U
#define LIBRESTEAM_SHIM_APPS_CYBERCAFE_GET 2U
#define LIBRESTEAM_SHIM_APPS_VAC_BANNED_GET 3U
#define LIBRESTEAM_SHIM_APPS_GAME_LANG_GET 4U
#define LIBRESTEAM_SHIM_APPS_AVAIL_LANG_GET 5U
#define LIBRESTEAM_SHIM_APPS_APP_SUBSCRIBED_GET 6U
#define LIBRESTEAM_SHIM_APPS_DLC_INSTALLED_GET 7U
#define LIBRESTEAM_SHIM_APPS_PURCHASE_TIME_GET 8U
#define LIBRESTEAM_SHIM_APPS_FREE_WEEKEND_GET 9U
#define LIBRESTEAM_SHIM_APPS_DLC_CUNT_GET 10U
#define LIBRESTEAM_SHIM_APPS_DLC_GET 11U
#define LIBRESTEAM_SHIM_APPS_DLC_INSTALL 12U
#define LIBRESTEAM_SHIM_APPS_DLC_UNINSTALL 13U
#define LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_REQ 14U
#define LIBRESTEAM_SHIM_APPS_BETA_NAME_GET 15U
#define LIBRESTEAM_SHIM_APPS_CONTENT_CORRUPT_MARK 16U
#define LIBRESTEAM_SHIM_APPS_DEPOTS_GET 17U
#define LIBRESTEAM_SHIM_APPS_INSTALL_DIR_GET 18U
#define LIBRESTEAM_SHIM_APPS_APP_INSTALLED_GET 19U
#define LIBRESTEAM_SHIM_APPS_OWNER_GET 20U
#define LIBRESTEAM_SHIM_APPS_LAUNCH_QUERY_PARAM_GET 21U
#define LIBRESTEAM_SHIM_APPS_DLC_PROGRESS_GET 22U
#define LIBRESTEAM_SHIM_APPS_BUILD_ID_GET 23U
#define LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_ALL_REQ 24U
#define LIBRESTEAM_SHIM_APPS_FILE_DETAILS_REQ 25U
#define LIBRESTEAM_SHIM_APPS_LAUNCH_CMD_LINE_GET 26U
#define LIBRESTEAM_SHIM_APPS_FAMILY_SHARING_GET 27U
#define LIBRESTEAM_SHIM_APPS_TIMED_TRIAL_GET 28U
#define LIBRESTEAM_SHIM_APPS_DLC_CTX_SET 29U
#define LIBRESTEAM_SHIM_APPS_BETA_CUNT_GET 30U
#define LIBRESTEAM_SHIM_APPS_BETA_INFO_GET 31U
#define LIBRESTEAM_SHIM_APPS_BETA_ACTIVE_SET 32U
#define LIBRESTEAM_SHIM_APPS_NONE 0U
#define LIBRESTEAM_SHIM_APPS_STR_EMPTY ""
#define LIBRESTEAM_SHIM_APPS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_apps_subscribed_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_SUBSCRIBED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_subscribed_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint8_t libresteam_shim_apps_low_violence_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_LOW_VIOLENCE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_low_violence_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint8_t libresteam_shim_apps_cybercafe_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_CYBERCAFE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_cybercafe_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint8_t libresteam_shim_apps_vac_banned_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_VAC_BANNED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_vac_banned_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static const char* libresteam_shim_apps_game_lang_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_GAME_LANG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    if (libresteam_apps_game_lang_get(set, &str) != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    return str;
}

static const char* libresteam_shim_apps_avail_lang_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_AVAIL_LANG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    if (libresteam_apps_avail_lang_get(set, &str) != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_apps_app_subscribed_get(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_APP_SUBSCRIBED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_app_subscribed_get(set, app_id, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint8_t libresteam_shim_apps_dlc_installed_get(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_INSTALLED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_dlc_installed_get(set, app_id, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint32_t libresteam_shim_apps_purchase_time_get(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t val = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_PURCHASE_TIME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_purchase_time_get(set, app_id, &val) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return val;
}

static uint8_t libresteam_shim_apps_free_weekend_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_FREE_WEEKEND_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_free_weekend_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint32_t libresteam_shim_apps_dlc_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_dlc_cunt_get(set, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return cunt;
}

static uint8_t libresteam_shim_apps_dlc_get(void *self, uint32_t idx, uint32_t *app_id_out, uint8_t *avail_out, char *name, uint32_t name_cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_dlc_get(set, idx, app_id_out, avail_out, name, name_cap) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static void libresteam_shim_apps_dlc_install(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_INSTALL, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_apps_dlc_install(set, app_id) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_apps_dlc_uninstall(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_UNINSTALL, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_apps_dlc_uninstall(set, app_id) != LDG_ERR_AOK) { return; }
}

static void libresteam_shim_apps_purchase_key_req(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_REQ, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_apps_purchase_key_req(set, app_id) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_apps_beta_name_get(void *self, char *name, uint32_t name_cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_BETA_NAME_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_beta_name_get(set, name, name_cap) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_apps_content_corrupt_mark(void *self, uint8_t missing_only)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_CONTENT_CORRUPT_MARK, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_content_corrupt_mark(set, missing_only) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_apps_depots_get(void *self, uint32_t app_id, uint32_t *depot, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DEPOTS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_depots_get(set, app_id, depot, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return cunt;
}

static uint32_t libresteam_shim_apps_install_dir_get(void *self, uint32_t app_id, char *dir, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_INSTALL_DIR_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_install_dir_get(set, app_id, dir, cap, &len) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return len;
}

static uint8_t libresteam_shim_apps_app_installed_get(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_APP_INSTALLED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_app_installed_get(set, app_id, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_steam_id_t) libresteam_shim_apps_owner_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_steam_id_t))
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t res = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_OWNER_GET, &set) != LDG_ERR_AOK) { LDG_CPP_STRUCT_RET(res); }

    if (libresteam_apps_owner_get(set, &res) != LDG_ERR_AOK) { res.all = LIBRESTEAM_SHIM_APPS_NONE; }

    LDG_CPP_STRUCT_RET(res);
}

static const char* libresteam_shim_apps_launch_query_param_get(void *self, const char *key)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_LAUNCH_QUERY_PARAM_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    if (libresteam_apps_launch_query_param_get(set, key, &str) != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_APPS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_apps_dlc_progress_get(void *self, uint32_t app_id, uint64_t *done_out, uint64_t *total_out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_PROGRESS_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_dlc_progress_get(set, app_id, done_out, total_out) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_apps_build_id_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t build_id = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_BUILD_ID_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_build_id_get(set, &build_id) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return build_id;
}

static void libresteam_shim_apps_purchase_key_all_req(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_ALL_REQ, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_apps_purchase_key_all_req(set) != LDG_ERR_AOK) { return; }
}

static uint64_t libresteam_shim_apps_file_details_req(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_FILE_DETAILS_REQ, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_file_details_req(set, name, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return call;
}

static uint32_t libresteam_shim_apps_launch_cmd_line_get(void *self, char *cmd_line, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_LAUNCH_CMD_LINE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_launch_cmd_line_get(set, cmd_line, cap, &len) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return len;
}

static uint8_t libresteam_shim_apps_family_sharing_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_FAMILY_SHARING_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_family_sharing_get(set, &flag) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return flag;
}

static uint8_t libresteam_shim_apps_timed_trial_get(void *self, uint32_t *allowed_out, uint32_t *played_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t trial = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_TIMED_TRIAL_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_timed_trial_get(set, &trial, allowed_out, played_out) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return trial;
}

static uint8_t libresteam_shim_apps_dlc_ctx_set(void *self, uint32_t app_id)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_DLC_CTX_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_dlc_ctx_set(set, app_id) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_apps_beta_cunt_get(void *self, uint32_t *avail_out, uint32_t *priv_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = LIBRESTEAM_SHIM_APPS_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_BETA_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    if (libresteam_apps_beta_cunt_get(set, &cunt, avail_out, priv_out) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_APPS_NONE; }

    return cunt;
}

static uint8_t libresteam_shim_apps_beta_info_get(void *self, uint32_t idx, uint32_t *flags_out, uint32_t *build_id_out, char *name, uint32_t name_cap, char *desc, uint32_t desc_cap, uint32_t *updated_out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_BETA_INFO_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_beta_info_get(set, idx, flags_out, build_id_out, name, name_cap, desc, desc_cap, updated_out) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_apps_beta_active_set(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_APPS_BETA_ACTIVE_SET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_apps_beta_active_set(set, name) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_TRUE;
}

static uint32_t libresteam_shim_apps_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_APPS_SUBSCRIBED_GET:
        { *out = "BIsSubscribed";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_LOW_VIOLENCE_GET:
        { *out = "BIsLowViolence";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_CYBERCAFE_GET:
        { *out = "BIsCybercafe";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_VAC_BANNED_GET:
        { *out = "BIsVACBanned";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_GAME_LANG_GET:
        { *out = "GetCurrentGameLanguage";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_AVAIL_LANG_GET:
        { *out = "GetAvailableGameLanguages";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_APP_SUBSCRIBED_GET:
        { *out = "BIsSubscribedApp";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_INSTALLED_GET:
        { *out = "BIsDlcInstalled";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_PURCHASE_TIME_GET:
        { *out = "GetEarliestPurchaseUnixTime";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_FREE_WEEKEND_GET:
        { *out = "BIsSubscribedFromFreeWeekend";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_CUNT_GET:
        { *out = "GetDLCCount";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_GET:
        { *out = "BGetDLCDataByIndex";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_INSTALL:
        { *out = "InstallDLC";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_UNINSTALL:
        { *out = "UninstallDLC";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_REQ:
        { *out = "RequestAppProofOfPurchaseKey";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_BETA_NAME_GET:
        { *out = "GetCurrentBetaName";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_CONTENT_CORRUPT_MARK:
        { *out = "MarkContentCorrupt";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DEPOTS_GET:
        { *out = "GetInstalledDepots";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_INSTALL_DIR_GET:
        { *out = "GetAppInstallDir";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_APP_INSTALLED_GET:
        { *out = "BIsAppInstalled";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_OWNER_GET:
        { *out = "GetAppOwner";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_LAUNCH_QUERY_PARAM_GET:
        { *out = "GetLaunchQueryParam";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_PROGRESS_GET:
        { *out = "GetDlcDownloadProgress";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_BUILD_ID_GET:
        { *out = "GetAppBuildId";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_ALL_REQ:
        { *out = "RequestAllProofOfPurchaseKeys";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_FILE_DETAILS_REQ:
        { *out = "GetFileDetails";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_LAUNCH_CMD_LINE_GET:
        { *out = "GetLaunchCommandLine";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_FAMILY_SHARING_GET:
        { *out = "BIsSubscribedFromFamilySharing";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_TIMED_TRIAL_GET:
        { *out = "BIsTimedTrial";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_DLC_CTX_SET:
        { *out = "SetDlcContext";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_BETA_CUNT_GET:
        { *out = "GetNumBetas";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_BETA_INFO_GET:
        { *out = "GetBetaInfo";
          break;
        }
        case LIBRESTEAM_SHIM_APPS_BETA_ACTIVE_SET:
        { *out = "SetActiveBeta";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_apps_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_APPS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_APPS_GAME_LANG_GET:
        case LIBRESTEAM_SHIM_APPS_AVAIL_LANG_GET:
        case LIBRESTEAM_SHIM_APPS_LAUNCH_QUERY_PARAM_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        case LIBRESTEAM_SHIM_APPS_OWNER_GET:
        { *out = LIBRESTEAM_SHIM_RET_STRUCT;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_apps_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_APPS_SUBSCRIBED_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_subscribed_get)
        case LIBRESTEAM_SHIM_APPS_LOW_VIOLENCE_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_low_violence_get)
        case LIBRESTEAM_SHIM_APPS_CYBERCAFE_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_cybercafe_get)
        case LIBRESTEAM_SHIM_APPS_VAC_BANNED_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_vac_banned_get)
        case LIBRESTEAM_SHIM_APPS_GAME_LANG_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_game_lang_get)
        case LIBRESTEAM_SHIM_APPS_AVAIL_LANG_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_avail_lang_get)
        case LIBRESTEAM_SHIM_APPS_APP_SUBSCRIBED_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_app_subscribed_get)
        case LIBRESTEAM_SHIM_APPS_DLC_INSTALLED_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_installed_get)
        case LIBRESTEAM_SHIM_APPS_PURCHASE_TIME_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_purchase_time_get)
        case LIBRESTEAM_SHIM_APPS_FREE_WEEKEND_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_free_weekend_get)
        case LIBRESTEAM_SHIM_APPS_DLC_CUNT_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_cunt_get)
        case LIBRESTEAM_SHIM_APPS_DLC_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_get)
        case LIBRESTEAM_SHIM_APPS_DLC_INSTALL:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_install)
        case LIBRESTEAM_SHIM_APPS_DLC_UNINSTALL:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_uninstall)
        case LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_REQ:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_purchase_key_req)
        case LIBRESTEAM_SHIM_APPS_BETA_NAME_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_beta_name_get)
        case LIBRESTEAM_SHIM_APPS_CONTENT_CORRUPT_MARK:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_content_corrupt_mark)
        case LIBRESTEAM_SHIM_APPS_DEPOTS_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_depots_get)
        case LIBRESTEAM_SHIM_APPS_INSTALL_DIR_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_install_dir_get)
        case LIBRESTEAM_SHIM_APPS_APP_INSTALLED_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_app_installed_get)
        case LIBRESTEAM_SHIM_APPS_OWNER_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_owner_get)
        case LIBRESTEAM_SHIM_APPS_LAUNCH_QUERY_PARAM_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_launch_query_param_get)
        case LIBRESTEAM_SHIM_APPS_DLC_PROGRESS_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_progress_get)
        case LIBRESTEAM_SHIM_APPS_BUILD_ID_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_build_id_get)
        case LIBRESTEAM_SHIM_APPS_PURCHASE_KEY_ALL_REQ:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_purchase_key_all_req)
        case LIBRESTEAM_SHIM_APPS_FILE_DETAILS_REQ:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_file_details_req)
        case LIBRESTEAM_SHIM_APPS_LAUNCH_CMD_LINE_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_launch_cmd_line_get)
        case LIBRESTEAM_SHIM_APPS_FAMILY_SHARING_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_family_sharing_get)
        case LIBRESTEAM_SHIM_APPS_TIMED_TRIAL_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_timed_trial_get)
        case LIBRESTEAM_SHIM_APPS_DLC_CTX_SET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_dlc_ctx_set)
        case LIBRESTEAM_SHIM_APPS_BETA_CUNT_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_beta_cunt_get)
        case LIBRESTEAM_SHIM_APPS_BETA_INFO_GET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_beta_info_get)
        case LIBRESTEAM_SHIM_APPS_BETA_ACTIVE_SET:
            LIBRESTEAM_SHIM_APPS_IMPL(libresteam_shim_apps_beta_active_set)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_apps_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_APPS_VERSION;
    out->name_get = libresteam_shim_apps_name_get;
    out->kind_get = libresteam_shim_apps_kind_get;
    out->impl_get = libresteam_shim_apps_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_APPS_METHOD_CUNT;

    return LDG_ERR_AOK;
}
