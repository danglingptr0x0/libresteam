#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/app_id/app_id.h>
#include <libresteam/cb/cb.h>
#include <libresteam/shim/shim.h>
#include <libresteam/steam_id/steam_id.h>
#include <libresteam/utils/utils.h>

#define LIBRESTEAM_SHIM_UTILS_VERSION "SteamUtils010"
#define LIBRESTEAM_SHIM_UTILS_METHOD_CUNT 38U
#define LIBRESTEAM_SHIM_UTILS_009_VERSION "SteamUtils009"
#define LIBRESTEAM_SHIM_UTILS_009_METHOD_CUNT 33U
#define LIBRESTEAM_SHIM_UTILS_009_FILTER_OPTS_DEFAULT 0U
#define LIBRESTEAM_SHIM_UTILS_009_FILTER_CTX_UNKNOWN 0U
#define LIBRESTEAM_SHIM_UTILS_APP_ACTIVE_SECS_GET 0U
#define LIBRESTEAM_SHIM_UTILS_COMPUTER_ACTIVE_SECS_GET 1U
#define LIBRESTEAM_SHIM_UTILS_UNIVERSE_GET 2U
#define LIBRESTEAM_SHIM_UTILS_SRV_TIME_GET 3U
#define LIBRESTEAM_SHIM_UTILS_IP_COUNTRY_GET 4U
#define LIBRESTEAM_SHIM_UTILS_IMG_SIZE_GET 5U
#define LIBRESTEAM_SHIM_UTILS_IMG_RGBA_GET 6U
#define LIBRESTEAM_SHIM_UTILS_CSER_ADDR_GET 7U
#define LIBRESTEAM_SHIM_UTILS_BATTERY_GET 8U
#define LIBRESTEAM_SHIM_UTILS_APP_ID_GET 9U
#define LIBRESTEAM_SHIM_UTILS_NOTIFICATION_POS_SET 10U
#define LIBRESTEAM_SHIM_UTILS_CALL_COMPLETED_GET 11U
#define LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_GET 12U
#define LIBRESTEAM_SHIM_UTILS_CALL_RES_GET 13U
#define LIBRESTEAM_SHIM_UTILS_FRAME_RUN 14U
#define LIBRESTEAM_SHIM_UTILS_IPC_CALL_CUNT_GET 15U
#define LIBRESTEAM_SHIM_UTILS_WARNING_HOOK_SET 16U
#define LIBRESTEAM_SHIM_UTILS_OVERLAY_EN_GET 17U
#define LIBRESTEAM_SHIM_UTILS_OVERLAY_PRESENT_GET 18U
#define LIBRESTEAM_SHIM_UTILS_FILE_SIG_CK 19U
#define LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_SHOW 20U
#define LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_LEN_GET 21U
#define LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_GET 22U
#define LIBRESTEAM_SHIM_UTILS_UI_LANG_GET 23U
#define LIBRESTEAM_SHIM_UTILS_VR_GET 24U
#define LIBRESTEAM_SHIM_UTILS_NOTIFICATION_INSET_SET 25U
#define LIBRESTEAM_SHIM_UTILS_BIG_PICTURE_GET 26U
#define LIBRESTEAM_SHIM_UTILS_VR_DASHBOARD_START 27U
#define LIBRESTEAM_SHIM_UTILS_VR_STREAMING_GET 28U
#define LIBRESTEAM_SHIM_UTILS_VR_STREAMING_SET 29U
#define LIBRESTEAM_SHIM_UTILS_CHINA_LAUNCHER_GET 30U
#define LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT 31U
#define LIBRESTEAM_SHIM_UTILS_TEXT_FILTER 32U
#define LIBRESTEAM_SHIM_UTILS_IPV6_STATE_GET 33U
#define LIBRESTEAM_SHIM_UTILS_STEAM_DECK_GET 34U
#define LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_SHOW 35U
#define LIBRESTEAM_SHIM_UTILS_LAUNCHER_MODE_SET 36U
#define LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_DISMISS 37U
#define LIBRESTEAM_SHIM_UTILS_NONE 0U
#define LIBRESTEAM_SHIM_UTILS_CALL_NONE 0U
#define LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_NONE 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_UTILS_BATTERY_AC 255U
#define LIBRESTEAM_SHIM_UTILS_IPV6_STATE_UNKNOWN 0U
#define LIBRESTEAM_SHIM_UTILS_LANG "english"
#define LIBRESTEAM_SHIM_UTILS_STR_EMPTY ""
#define LIBRESTEAM_SHIM_UTILS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_utils_aok_is(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t libresteam_shim_utils_flag_get(uint32_t res, uint8_t flag)
{
    if (res != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (flag == LDG_TRUTH_TRUE) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_utils_app_active_secs_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t secs = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_APP_ACTIVE_SECS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_utils_app_active_secs_get(set, &secs) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return secs;
}

static uint32_t libresteam_shim_utils_computer_active_secs_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t secs = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_COMPUTER_ACTIVE_SECS_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_utils_computer_active_secs_get(set, &secs) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return secs;
}

static uint32_t libresteam_shim_utils_universe_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t universe = (uint32_t)LIBRESTEAM_STEAM_ID_UNIVERSE_PUBLIC;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_UNIVERSE_GET, &set) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_STEAM_ID_UNIVERSE_PUBLIC; }

    if (libresteam_utils_universe_get(set, &universe) != LDG_ERR_AOK) { return (uint32_t)LIBRESTEAM_STEAM_ID_UNIVERSE_PUBLIC; }

    return universe;
}

static uint32_t libresteam_shim_utils_srv_time_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t time = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_SRV_TIME_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_utils_srv_time_get(set, &time) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return time;
}

static const char* libresteam_shim_utils_ip_country_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_IP_COUNTRY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_STR_EMPTY; }

    if (libresteam_utils_ip_country_get(set, &str) != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_UTILS_STR_EMPTY; }

    return str;
}

static uint8_t libresteam_shim_utils_img_size_get(void *self, uint32_t img, uint32_t *width_out, uint32_t *height_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t width = 0;
    uint32_t height = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_IMG_SIZE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_utils_img_size_get(set, img, &width, &height) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (width_out) { *width_out = width; }

    if (height_out) { *height_out = height; }

    return LDG_TRUTH_TRUE;
}

static uint8_t libresteam_shim_utils_img_rgba_get(void *self, uint32_t img, uint8_t *dst, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_IMG_RGBA_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_img_rgba_get(set, img, dst, cap));
}

static uint8_t libresteam_shim_utils_cser_addr_get(void *self, uint32_t *ip_out, uint16_t *port_out)
{
    const libresteam_backend_set_t *set = 0x0;

    if (ip_out) { *ip_out = LIBRESTEAM_SHIM_UTILS_NONE; }

    if (port_out) { *port_out = LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_CSER_ADDR_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return LDG_TRUTH_FALSE;
}

static uint8_t libresteam_shim_utils_battery_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t battery = LIBRESTEAM_SHIM_UTILS_BATTERY_AC;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_BATTERY_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_BATTERY_AC; }

    if (libresteam_utils_battery_get(set, &battery) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_BATTERY_AC; }

    return battery;
}

static uint32_t libresteam_shim_utils_app_id_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t app_id = LIBRESTEAM_APP_ID_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_APP_ID_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_APP_ID_NONE; }

    if (libresteam_utils_app_id_get(set, &app_id) == LDG_ERR_AOK && app_id != LIBRESTEAM_APP_ID_NONE) { return app_id; }

    if (libresteam_app_id_resolve(LIBRESTEAM_APP_ID_NONE, &app_id) != LDG_ERR_AOK) { return LIBRESTEAM_APP_ID_NONE; }

    return app_id;
}

static void libresteam_shim_utils_notification_pos_set(void *self, uint32_t pos)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_NOTIFICATION_POS_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_utils_notification_pos_set(set, pos) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_call_completed_get(void *self, uint64_t call, uint8_t *failed_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t done = LDG_TRUTH_FALSE;
    uint8_t failed = LDG_TRUTH_FALSE;

    if (failed_out) { *failed_out = LDG_TRUTH_FALSE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_CALL_COMPLETED_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_utils_call_completed_get(set, call, &done, &failed) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (failed_out && failed == LDG_TRUTH_TRUE) { *failed_out = LDG_TRUTH_TRUE; }

    return libresteam_shim_utils_flag_get(LDG_ERR_AOK, done);
}

static uint32_t libresteam_shim_utils_call_failure_get(void *self, uint64_t call)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t reason = LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_NONE; }

    if (libresteam_utils_call_failure_get(set, call, &reason) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_NONE; }

    return reason;
}

static uint8_t libresteam_shim_utils_call_res_get(void *self, uint64_t call, void *buff, uint32_t cap, uint32_t cb_id_expected, uint8_t *failed_out)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t failed = LDG_TRUTH_FALSE;

    if (failed_out) { *failed_out = LDG_TRUTH_FALSE; }

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_CALL_RES_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (libresteam_cb_call_res_get(set, call, buff, cap, cb_id_expected, &failed) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    if (failed_out && failed == LDG_TRUTH_TRUE) { *failed_out = LDG_TRUTH_TRUE; }

    return LDG_TRUTH_TRUE;
}

static void libresteam_shim_utils_frame_run(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_FRAME_RUN, &set) != LDG_ERR_AOK) { return; }
}

static uint32_t libresteam_shim_utils_ipc_call_cunt_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_IPC_CALL_CUNT_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_utils_ipc_call_cunt_get(set, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return cunt;
}

static void libresteam_shim_utils_warning_hook_set(void *self, void *hook)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_WARNING_HOOK_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_utils_warning_hook_set(set, hook) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_overlay_en_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_OVERLAY_EN_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_overlay_en_get(set, &flag), flag);
}

static uint8_t libresteam_shim_utils_overlay_present_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_OVERLAY_PRESENT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_overlay_present_get(set, &flag), flag);
}

static uint64_t libresteam_shim_utils_file_sig_ck(void *self, const char *name)
{
    const libresteam_backend_set_t *set = 0x0;
    uint64_t call = LIBRESTEAM_SHIM_UTILS_CALL_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_FILE_SIG_CK, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_CALL_NONE; }

    if (libresteam_utils_file_sig_ck(set, name, &call) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_CALL_NONE; }

    return call;
}

static uint8_t libresteam_shim_utils_gamepad_text_show(void *self, uint32_t mode, uint32_t line_mode, const char *desc, uint32_t char_max, const char *existing)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_SHOW, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_gamepad_text_show(set, mode, line_mode, desc, char_max, existing));
}

static uint32_t libresteam_shim_utils_gamepad_text_len_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t len = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_LEN_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    if (libresteam_utils_gamepad_text_len_get(set, &len) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return len;
}

static uint8_t libresteam_shim_utils_gamepad_text_get(void *self, char *text, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_gamepad_text_get(set, text, cap));
}

static const char* libresteam_shim_utils_ui_lang_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    const char *str = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_UI_LANG_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_LANG; }

    if (libresteam_utils_ui_lang_get(set, &str) != LDG_ERR_AOK || !str) { return LIBRESTEAM_SHIM_UTILS_LANG; }

    return str;
}

static uint8_t libresteam_shim_utils_vr_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_VR_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_vr_get(set, &flag), flag);
}

static void libresteam_shim_utils_notification_inset_set(void *self, uint32_t horizontal, uint32_t vertical)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_NOTIFICATION_INSET_SET, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_utils_notification_inset_set(set, horizontal, vertical) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_big_picture_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_BIG_PICTURE_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_big_picture_get(set, &flag), flag);
}

static void libresteam_shim_utils_vr_dashboard_start(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_VR_DASHBOARD_START, &set) != LDG_ERR_AOK) { return; }

    if (libresteam_utils_vr_dashboard_start(set) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_vr_streaming_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_VR_STREAMING_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_vr_streaming_get(set, &flag), flag);
}

static void libresteam_shim_utils_vr_streaming_set(void *self, uint8_t en)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_VR_STREAMING_SET, &set) != LDG_ERR_AOK) { return; }

    if (en) { flag = LDG_TRUTH_TRUE; }

    if (libresteam_utils_vr_streaming_set(set, flag) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_china_launcher_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_CHINA_LAUNCHER_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_china_launcher_get(set, &flag), flag);
}

static uint8_t libresteam_shim_utils_text_filter_init(void *self, uint32_t opts)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_text_filter_init(set, opts));
}

static uint32_t libresteam_shim_utils_text_filter(void *self, uint32_t filter_ctx, uint64_t src_id, const char *text, char *dst, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t src = LDG_STRUCT_ZERO_INIT;
    uint32_t filtered = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_TEXT_FILTER, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    src.all = src_id;
    if (libresteam_utils_text_filter(set, filter_ctx, src, text, dst, cap, &filtered) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return filtered;
}

static uint32_t libresteam_shim_utils_ipv6_state_get(void *self, uint32_t proto)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t state = LIBRESTEAM_SHIM_UTILS_IPV6_STATE_UNKNOWN;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_IPV6_STATE_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_IPV6_STATE_UNKNOWN; }

    if (libresteam_utils_ipv6_state_get(set, proto, &state) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_IPV6_STATE_UNKNOWN; }

    return state;
}

static uint8_t libresteam_shim_utils_steam_deck_get(void *self)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_STEAM_DECK_GET, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_flag_get(libresteam_utils_steam_deck_get(set, &flag), flag);
}

static uint8_t libresteam_shim_utils_gamepad_text_float_show(void *self, uint32_t mode, uint32_t pos_x, uint32_t pos_y, uint32_t width, uint32_t height)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_SHOW, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_gamepad_text_float_show(set, mode, pos_x, pos_y, width, height));
}

static void libresteam_shim_utils_launcher_mode_set(void *self, uint8_t en)
{
    const libresteam_backend_set_t *set = 0x0;
    uint8_t flag = LDG_TRUTH_FALSE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_LAUNCHER_MODE_SET, &set) != LDG_ERR_AOK) { return; }

    if (en) { flag = LDG_TRUTH_TRUE; }

    if (libresteam_utils_launcher_mode_set(set, flag) != LDG_ERR_AOK) { return; }
}

static uint8_t libresteam_shim_utils_gamepad_text_float_dismiss(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_DISMISS, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_gamepad_text_float_dismiss(set));
}

static uint32_t libresteam_shim_utils_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_UTILS_APP_ACTIVE_SECS_GET:
        { *out = "GetSecondsSinceAppActive";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_COMPUTER_ACTIVE_SECS_GET:
        { *out = "GetSecondsSinceComputerActive";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_UNIVERSE_GET:
        { *out = "GetConnectedUniverse";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_SRV_TIME_GET:
        { *out = "GetServerRealTime";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_IP_COUNTRY_GET:
        { *out = "GetIPCountry";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_IMG_SIZE_GET:
        { *out = "GetImageSize";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_IMG_RGBA_GET:
        { *out = "GetImageRGBA";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_CSER_ADDR_GET:
        { *out = "GetCSERIPPort";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_BATTERY_GET:
        { *out = "GetCurrentBatteryPower";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_APP_ID_GET:
        { *out = "GetAppID";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_NOTIFICATION_POS_SET:
        { *out = "SetOverlayNotificationPosition";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_CALL_COMPLETED_GET:
        { *out = "IsAPICallCompleted";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_GET:
        { *out = "GetAPICallFailureReason";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_CALL_RES_GET:
        { *out = "GetAPICallResult";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_FRAME_RUN:
        { *out = "RunFrame";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_IPC_CALL_CUNT_GET:
        { *out = "GetIPCCallCount";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_WARNING_HOOK_SET:
        { *out = "SetWarningMessageHook";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_OVERLAY_EN_GET:
        { *out = "IsOverlayEnabled";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_OVERLAY_PRESENT_GET:
        { *out = "BOverlayNeedsPresent";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_FILE_SIG_CK:
        { *out = "CheckFileSignature";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_SHOW:
        { *out = "ShowGamepadTextInput";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_LEN_GET:
        { *out = "GetEnteredGamepadTextLength";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_GET:
        { *out = "GetEnteredGamepadTextInput";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_UI_LANG_GET:
        { *out = "GetSteamUILanguage";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_VR_GET:
        { *out = "IsSteamRunningInVR";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_NOTIFICATION_INSET_SET:
        { *out = "SetOverlayNotificationInset";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_BIG_PICTURE_GET:
        { *out = "IsSteamInBigPictureMode";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_VR_DASHBOARD_START:
        { *out = "StartVRDashboard";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_VR_STREAMING_GET:
        { *out = "IsVRHeadsetStreamingEnabled";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_VR_STREAMING_SET:
        { *out = "SetVRHeadsetStreamingEnabled";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_CHINA_LAUNCHER_GET:
        { *out = "IsSteamChinaLauncher";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT:
        { *out = "InitFilterText";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER:
        { *out = "FilterText";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_IPV6_STATE_GET:
        { *out = "GetIPv6ConnectivityState";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_STEAM_DECK_GET:
        { *out = "IsSteamRunningOnSteamDeck";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_SHOW:
        { *out = "ShowFloatingGamepadTextInput";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_LAUNCHER_MODE_SET:
        { *out = "SetGameLauncherMode";
          break;
        }
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_DISMISS:
        { *out = "DismissFloatingGamepadTextInput";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_utils_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_UTILS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_UTILS_IP_COUNTRY_GET:
        case LIBRESTEAM_SHIM_UTILS_UI_LANG_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_utils_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_UTILS_APP_ACTIVE_SECS_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_app_active_secs_get)
        case LIBRESTEAM_SHIM_UTILS_COMPUTER_ACTIVE_SECS_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_computer_active_secs_get)
        case LIBRESTEAM_SHIM_UTILS_UNIVERSE_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_universe_get)
        case LIBRESTEAM_SHIM_UTILS_SRV_TIME_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_srv_time_get)
        case LIBRESTEAM_SHIM_UTILS_IP_COUNTRY_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_ip_country_get)
        case LIBRESTEAM_SHIM_UTILS_IMG_SIZE_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_img_size_get)
        case LIBRESTEAM_SHIM_UTILS_IMG_RGBA_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_img_rgba_get)
        case LIBRESTEAM_SHIM_UTILS_CSER_ADDR_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_cser_addr_get)
        case LIBRESTEAM_SHIM_UTILS_BATTERY_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_battery_get)
        case LIBRESTEAM_SHIM_UTILS_APP_ID_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_app_id_get)
        case LIBRESTEAM_SHIM_UTILS_NOTIFICATION_POS_SET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_notification_pos_set)
        case LIBRESTEAM_SHIM_UTILS_CALL_COMPLETED_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_call_completed_get)
        case LIBRESTEAM_SHIM_UTILS_CALL_FAILURE_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_call_failure_get)
        case LIBRESTEAM_SHIM_UTILS_CALL_RES_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_call_res_get)
        case LIBRESTEAM_SHIM_UTILS_FRAME_RUN:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_frame_run)
        case LIBRESTEAM_SHIM_UTILS_IPC_CALL_CUNT_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_ipc_call_cunt_get)
        case LIBRESTEAM_SHIM_UTILS_WARNING_HOOK_SET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_warning_hook_set)
        case LIBRESTEAM_SHIM_UTILS_OVERLAY_EN_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_overlay_en_get)
        case LIBRESTEAM_SHIM_UTILS_OVERLAY_PRESENT_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_overlay_present_get)
        case LIBRESTEAM_SHIM_UTILS_FILE_SIG_CK:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_file_sig_ck)
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_SHOW:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_gamepad_text_show)
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_LEN_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_gamepad_text_len_get)
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_gamepad_text_get)
        case LIBRESTEAM_SHIM_UTILS_UI_LANG_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_ui_lang_get)
        case LIBRESTEAM_SHIM_UTILS_VR_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_vr_get)
        case LIBRESTEAM_SHIM_UTILS_NOTIFICATION_INSET_SET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_notification_inset_set)
        case LIBRESTEAM_SHIM_UTILS_BIG_PICTURE_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_big_picture_get)
        case LIBRESTEAM_SHIM_UTILS_VR_DASHBOARD_START:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_vr_dashboard_start)
        case LIBRESTEAM_SHIM_UTILS_VR_STREAMING_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_vr_streaming_get)
        case LIBRESTEAM_SHIM_UTILS_VR_STREAMING_SET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_vr_streaming_set)
        case LIBRESTEAM_SHIM_UTILS_CHINA_LAUNCHER_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_china_launcher_get)
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_text_filter_init)
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_text_filter)
        case LIBRESTEAM_SHIM_UTILS_IPV6_STATE_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_ipv6_state_get)
        case LIBRESTEAM_SHIM_UTILS_STEAM_DECK_GET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_steam_deck_get)
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_SHOW:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_gamepad_text_float_show)
        case LIBRESTEAM_SHIM_UTILS_LAUNCHER_MODE_SET:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_launcher_mode_set)
        case LIBRESTEAM_SHIM_UTILS_GAMEPAD_TEXT_FLOAT_DISMISS:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_gamepad_text_float_dismiss)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_utils_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_UTILS_VERSION;
    out->name_get = libresteam_shim_utils_name_get;
    out->kind_get = libresteam_shim_utils_kind_get;
    out->impl_get = libresteam_shim_utils_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_UTILS_METHOD_CUNT;

    return LDG_ERR_AOK;
}

static uint8_t libresteam_shim_utils_009_text_filter_init(void *self)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_utils_aok_is(libresteam_utils_text_filter_init(set, LIBRESTEAM_SHIM_UTILS_009_FILTER_OPTS_DEFAULT));
}

static uint32_t libresteam_shim_utils_009_text_filter(void *self, char *dst, uint32_t cap, const char *text, uint8_t legal_only)
{
    const libresteam_backend_set_t *set = 0x0;
    libresteam_steam_id_t src = LDG_STRUCT_ZERO_INIT;
    uint32_t filtered = 0;
    uint32_t res = LDG_ERR_AOK;

    if (libresteam_shim_set_get(self, LIBRESTEAM_SHIM_UTILS_TEXT_FILTER, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    res = libresteam_utils_text_filter(set, LIBRESTEAM_SHIM_UTILS_009_FILTER_CTX_UNKNOWN, src, text, dst, cap, &filtered);
    if (libresteam_shim_arg_log(self, LIBRESTEAM_SHIM_UTILS_TEXT_FILTER, res, "cap: %u32; legal only: %u32", cap, (uint32_t)legal_only) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_UTILS_NONE; }

    return filtered;
}

static uint32_t libresteam_shim_utils_009_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER_INIT:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_009_text_filter_init)
        case LIBRESTEAM_SHIM_UTILS_TEXT_FILTER:
            LIBRESTEAM_SHIM_UTILS_IMPL(libresteam_shim_utils_009_text_filter)
        default: { break;
        }
    }

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_UTILS_009_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return libresteam_shim_utils_impl_get(idx, out);
}

uint32_t libresteam_shim_utils_009_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_UTILS_009_VERSION;
    out->name_get = libresteam_shim_utils_name_get;
    out->kind_get = libresteam_shim_utils_kind_get;
    out->impl_get = libresteam_shim_utils_009_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_UTILS_009_METHOD_CUNT;

    return LDG_ERR_AOK;
}
