#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/wshop/ops.h>
#include <libresteam/wshop/pform.h>
#include <libresteam/wshop/types.h>

#define STEAM_WSHOP_ARG_NONE 0U
#define STEAM_WSHOP_CALL_INVALID 0U

static uint32_t steam_wshop_slot_get(void *ctx, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->wshop)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->wshop;

    return ldg_cpp_slot_get(&steam->wshop_desc, method_idx, slot_out);
}

static uint32_t steam_wshop_void_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_wshop_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(obj, slot, arg, arg_cunt);
}

static uint32_t steam_wshop_bool_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t fail_res)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    res = steam_wshop_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag == LDG_TRUTH_FALSE) { return fail_res; }

    return LDG_ERR_AOK;
}

static uint32_t steam_wshop_u32_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_wshop_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_wshop_u64_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_wshop_slot_get(ctx, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_wshop_handle_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    res = steam_wshop_u64_call(ctx, method_idx, arg, arg_cunt, out);
    if (res != LDG_ERR_AOK)
    {
        *out = LIBRESTEAM_WSHOP_HANDLE_INVALID;
        return res;
    }

    if (*out == LIBRESTEAM_WSHOP_HANDLE_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_wshop_async_call(void *ctx, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *call_out)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_wshop_u64_call(ctx, method_idx, arg, arg_cunt, call_out);
    if (res != LDG_ERR_AOK) { return res; }

    if (*call_out == STEAM_WSHOP_CALL_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_wshop_query_user_create(void *ctx, uint32_t account, uint32_t list_type, uint32_t matching_type, uint32_t sort_order, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = account, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = list_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = matching_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = sort_order, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = creator_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = consumer_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = page, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_USER_CREATE, arg, cunt, query_out);
}

static uint32_t steam_wshop_query_all_page_create(void *ctx, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = matching_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = creator_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = consumer_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = page, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ALL_PAGE_CREATE, arg, cunt, query_out);
}

static uint32_t steam_wshop_query_all_cursor_create(void *ctx, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, const char *cursor, uint64_t *query_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = matching_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = creator_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = consumer_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = cursor, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ALL_CURSOR_CREATE, arg, cunt, query_out);
}

static uint32_t steam_wshop_query_details_create(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *query_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_DETAILS_CREATE, arg, cunt, query_out);
}

static uint32_t steam_wshop_query_send(void *ctx, uint64_t query, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_SEND, arg, cunt, call_out);
}

static uint32_t steam_wshop_query_res_get(void *ctx, uint64_t query, uint32_t idx, libresteam_wshop_details_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RES_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_tag_cunt_get(void *ctx, uint64_t query, uint32_t idx, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_TAG_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_wshop_query_tag_get(void *ctx, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = tag_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_TAG_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_tag_display_name_get(void *ctx, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = tag_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_TAG_DISPLAY_NAME_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_preview_url_get(void *ctx, uint64_t query, uint32_t idx, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_PREVIEW_URL_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_metadata_get(void *ctx, uint64_t query, uint32_t idx, char *buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_METADATA_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_children_get(void *ctx, uint64_t query, uint32_t idx, uint64_t *file_id, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_CHILDREN_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_statistic_get(void *ctx, uint64_t query, uint32_t idx, uint32_t stat_type, uint64_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = stat_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_STATISTIC_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_additional_preview_cunt_get(void *ctx, uint64_t query, uint32_t idx, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ADDITIONAL_PREVIEW_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_wshop_query_additional_preview_get(void *ctx, uint64_t query, uint32_t idx, uint32_t preview_idx, char *url_buff, uint32_t url_cap, char *orig_file_name_buff, uint32_t orig_file_name_cap, uint32_t *preview_type_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!preview_type_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *preview_type_out = 0;

    if (LDG_UNLIKELY(!url_buff || !orig_file_name_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(url_cap == 0 || orig_file_name_cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    url_buff[0] = LDG_STR_TERM;
    orig_file_name_buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = preview_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = url_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = url_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = orig_file_name_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = orig_file_name_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = preview_type_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ADDITIONAL_PREVIEW_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_kv_tag_cunt_get(void *ctx, uint64_t query, uint32_t idx, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_KV_TAG_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_wshop_query_kv_tag_get(void *ctx, uint64_t query, uint32_t idx, uint32_t kv_idx, char *key_buff, uint32_t key_cap, char *val_buff, uint32_t val_cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key_buff || !val_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(key_cap == 0 || val_cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    key_buff[0] = LDG_STR_TERM;
    val_buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = kv_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = key_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_KV_TAG_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_first_kv_tag_get(void *ctx, uint64_t query, uint32_t idx, const char *key, char *val_buff, uint32_t val_cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(val_cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    val_buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = val_cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_FIRST_KV_TAG_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_game_version_cunt_get(void *ctx, uint64_t query, uint32_t idx, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_GAME_VERSION_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_wshop_query_game_version_get(void *ctx, uint64_t query, uint32_t idx, uint32_t version_idx, char *branch_min_buff, char *branch_max_buff, uint32_t cap)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!branch_min_buff || !branch_max_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    branch_min_buff[0] = LDG_STR_TERM;
    branch_max_buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = version_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = branch_min_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = branch_max_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_GAME_VERSION_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_query_content_desc_get(void *ctx, uint64_t query, uint32_t idx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!desc_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_CONTENT_DESC_GET, arg, cunt, cunt_out);
}

static uint32_t steam_wshop_query_release(void *ctx, uint64_t query)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RELEASE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_required_tag_add(void *ctx, uint64_t query, const char *tag)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!tag)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tag, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_REQUIRED_TAG_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_required_tag_group_add(void *ctx, uint64_t query, const libresteam_wshop_str_arr_t *group)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!group)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = group, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_REQUIRED_TAG_GROUP_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_excluded_tag_add(void *ctx, uint64_t query, const char *tag)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!tag)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tag, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_EXCLUDED_TAG_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_only_ids_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_ONLY_IDS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_kv_tags_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_KV_TAGS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_long_desc_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_LONG_DESC_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_metadata_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_METADATA_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_children_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_CHILDREN_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_additional_previews_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_ADDITIONAL_PREVIEWS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_total_only_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_TOTAL_ONLY_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ret_playtime_stats_set(void *ctx, uint64_t query, uint32_t days)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = days, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RET_PLAYTIME_STATS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_language_set(void *ctx, uint64_t query, const char *language)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!language)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = language, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_LANGUAGE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_cached_resp_max_age_set(void *ctx, uint64_t query, uint32_t max_age_sec)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = max_age_sec, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_CACHED_RESP_MAX_AGE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_admin_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_ADMIN_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_cloud_file_name_filter_set(void *ctx, uint64_t query, const char *file_name)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_name)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_name, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_CLOUD_FILE_NAME_FILTER_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_match_any_tag_set(void *ctx, uint64_t query, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_MATCH_ANY_TAG_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_search_text_set(void *ctx, uint64_t query, const char *text)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!text)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = text, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_SEARCH_TEXT_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_ranked_by_trend_days_set(void *ctx, uint64_t query, uint32_t days)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = days, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_RANKED_BY_TREND_DAYS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_time_created_range_set(void *ctx, uint64_t query, uint32_t time_start, uint32_t time_end)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = time_start, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = time_end, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_TIME_CREATED_RANGE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_time_updated_range_set(void *ctx, uint64_t query, uint32_t time_start, uint32_t time_end)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = time_start, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = time_end, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_TIME_UPDATED_RANGE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_query_required_kv_tag_add(void *ctx, uint64_t query, const char *key, const char *val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_QUERY_REQUIRED_KV_TAG_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_details_req(void *ctx, uint64_t file_id, uint32_t max_age_sec, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = max_age_sec, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DETAILS_REQ, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_create(void *ctx, uint32_t consumer_app, uint32_t file_type, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = consumer_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_CREATE, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_update_start(void *ctx, uint32_t consumer_app, uint64_t file_id, uint64_t *update_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!update_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = consumer_app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_handle_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UPDATE_START, arg, cunt, update_out);
}

static uint32_t steam_wshop_item_title_set(void *ctx, uint64_t update, const char *title)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!title)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = title, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_TITLE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_desc_set(void *ctx, uint64_t update, const char *desc)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!desc)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_DESC_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_update_language_set(void *ctx, uint64_t update, const char *language)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!language)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = language, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UPDATE_LANGUAGE_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_metadata_set(void *ctx, uint64_t update, const char *metadata)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!metadata)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = metadata, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_METADATA_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_visibility_set(void *ctx, uint64_t update, uint32_t visibility)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = visibility, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_VISIBILITY_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_tags_set(void *ctx, uint64_t update, const libresteam_wshop_str_arr_t *tags, uint8_t admin_tags_en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!tags)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = tags, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = admin_tags_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_TAGS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_content_set(void *ctx, uint64_t update, const char *folder)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!folder)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = folder, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_CONTENT_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_set(void *ctx, uint64_t update, const char *file)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_legacy_upload_set(void *ctx, uint64_t update, uint8_t en)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_LEGACY_UPLOAD_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_kv_tags_all_remove(void *ctx, uint64_t update)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_KV_TAGS_ALL_REMOVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_kv_tags_remove(void *ctx, uint64_t update, const char *key)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_KV_TAGS_REMOVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_kv_tag_add(void *ctx, uint64_t update, const char *key, const char *val)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!key || !val)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = key, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = val, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_KV_TAG_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_file_add(void *ctx, uint64_t update, const char *file, uint32_t preview_type)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = preview_type, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_FILE_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_video_add(void *ctx, uint64_t update, const char *video_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!video_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = video_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_VIDEO_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_file_update(void *ctx, uint64_t update, uint32_t idx, const char *file)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_FILE_UPDATE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_video_update(void *ctx, uint64_t update, uint32_t idx, const char *video_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!video_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = video_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_VIDEO_UPDATE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_preview_remove(void *ctx, uint64_t update, uint32_t idx)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_PREVIEW_REMOVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_content_desc_add(void *ctx, uint64_t update, uint32_t desc_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = desc_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_CONTENT_DESC_ADD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_content_desc_remove(void *ctx, uint64_t update, uint32_t desc_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = desc_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_CONTENT_DESC_REMOVE, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_required_game_versions_set(void *ctx, uint64_t update, const char *branch_min, const char *branch_max)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!branch_min || !branch_max)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = branch_min, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = branch_max, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_REQUIRED_GAME_VERSIONS_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_update_submit(void *ctx, uint64_t update, const char *change_note, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = change_note, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UPDATE_SUBMIT, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_update_progress_get(void *ctx, uint64_t update, uint64_t *done_out, uint64_t *total_out, uint32_t *status_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!done_out || !total_out || !status_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *done_out = 0;
    *total_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = update, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = done_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = total_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UPDATE_PROGRESS_GET, arg, cunt, status_out);
}

static uint32_t steam_wshop_item_vote_set(void *ctx, uint64_t file_id, uint8_t vote_up, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = vote_up, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_VOTE_SET, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_vote_get(void *ctx, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_VOTE_GET, arg, cunt, call_out);
}

static uint32_t steam_wshop_favorite_add(void *ctx, uint32_t app, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_FAVORITE_ADD, arg, cunt, call_out);
}

static uint32_t steam_wshop_favorite_remove(void *ctx, uint32_t app, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_FAVORITE_REMOVE, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_subscribe(void *ctx, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_SUBSCRIBE, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_unsubscribe(void *ctx, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UNSUBSCRIBE, arg, cunt, call_out);
}

static uint32_t steam_wshop_subscribed_item_cunt_get(void *ctx, uint8_t disabled_locally_en, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = disabled_locally_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_SUBSCRIBED_ITEM_CUNT_GET, arg, cunt, out);
}

static uint32_t steam_wshop_subscribed_items_get(void *ctx, uint64_t *file_id, uint32_t cap, uint8_t disabled_locally_en, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = disabled_locally_en, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_SUBSCRIBED_ITEMS_GET, arg, cunt, cunt_out);
}

static uint32_t steam_wshop_item_state_get(void *ctx, uint64_t file_id, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_STATE_GET, arg, cunt, out);
}

static uint32_t steam_wshop_item_install_info_get(void *ctx, uint64_t file_id, uint64_t *size_on_disk_out, char *folder_buff, uint32_t cap, uint32_t *timestamp_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!size_on_disk_out || !timestamp_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *size_on_disk_out = 0;
    *timestamp_out = 0;

    if (LDG_UNLIKELY(!folder_buff)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(cap == 0)) { return LDG_ERR_FUNC_ARG_INVALID; }

    folder_buff[0] = LDG_STR_TERM;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = size_on_disk_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = folder_buff, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = timestamp_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_INSTALL_INFO_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_item_download_info_get(void *ctx, uint64_t file_id, uint64_t *downloaded_out, uint64_t *total_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!downloaded_out || !total_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *downloaded_out = 0;
    *total_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = downloaded_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = total_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_DOWNLOAD_INFO_GET, arg, cunt, LDG_ERR_NOT_FOUND);
}

static uint32_t steam_wshop_item_download(void *ctx, uint64_t file_id, uint8_t high_prio)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = high_prio, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_DOWNLOAD, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_game_srv_init(void *ctx, uint32_t depot, const char *folder)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!folder)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = depot, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = folder, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_GAME_SRV_INIT, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_downloads_suspend(void *ctx, uint8_t suspend)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = suspend, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_void_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DOWNLOADS_SUSPEND, arg, cunt);
}

static uint32_t steam_wshop_playtime_tracking_start(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_PLAYTIME_TRACKING_START, arg, cunt, call_out);
}

static uint32_t steam_wshop_playtime_tracking_stop(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_PLAYTIME_TRACKING_STOP, arg, cunt, call_out);
}

static uint32_t steam_wshop_playtime_tracking_all_stop(void *ctx, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_PLAYTIME_TRACKING_ALL_STOP, 0x0, STEAM_WSHOP_ARG_NONE, call_out);
}

static uint32_t steam_wshop_dependency_add(void *ctx, uint64_t parent, uint64_t child, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = parent, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = child, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DEPENDENCY_ADD, arg, cunt, call_out);
}

static uint32_t steam_wshop_dependency_remove(void *ctx, uint64_t parent, uint64_t child, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = parent, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = child, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DEPENDENCY_REMOVE, arg, cunt, call_out);
}

static uint32_t steam_wshop_app_dependency_add(void *ctx, uint64_t file_id, uint32_t app, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_APP_DEPENDENCY_ADD, arg, cunt, call_out);
}

static uint32_t steam_wshop_app_dependency_remove(void *ctx, uint64_t file_id, uint32_t app, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_APP_DEPENDENCY_REMOVE, arg, cunt, call_out);
}

static uint32_t steam_wshop_app_dependencies_get(void *ctx, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_APP_DEPENDENCIES_GET, arg, cunt, call_out);
}

static uint32_t steam_wshop_item_delete(void *ctx, uint64_t file_id, uint64_t *call_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_DELETE, arg, cunt, call_out);
}

static uint32_t steam_wshop_eula_show(void *ctx)
{
    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_EULA_SHOW, 0x0, STEAM_WSHOP_ARG_NONE, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_eula_status_get(void *ctx, uint64_t *call_out)
{
    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_wshop_async_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_EULA_STATUS_GET, 0x0, STEAM_WSHOP_ARG_NONE, call_out);
}

static uint32_t steam_wshop_user_content_desc_preference_get(void *ctx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!desc_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = desc_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_USER_CONTENT_DESC_PREFERENCE_GET, arg, cunt, cunt_out);
}

static uint32_t steam_wshop_items_disabled_locally_set(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint8_t disabled)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = disabled, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEMS_DISABLED_LOCALLY_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_subscribed_ld_order_set(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id_cunt, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_SUBSCRIBED_LD_ORDER_SET, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_item_unused_mark(void *ctx, uint64_t file_id)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = file_id, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_bool_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_ITEM_UNUSED_MARK, arg, cunt, LDG_ERR_INVALID);
}

static uint32_t steam_wshop_downloaded_item_cunt_get(void *ctx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DOWNLOADED_ITEM_CUNT_GET, 0x0, STEAM_WSHOP_ARG_NONE, out);
}

static uint32_t steam_wshop_downloaded_items_get(void *ctx, uint64_t *file_id, uint32_t cap, uint32_t *cunt_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    if (LDG_UNLIKELY(!file_id)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = file_id, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = cap, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_wshop_u32_call(ctx, LIBRESTEAM_BACKEND_STEAM_WSHOP_DOWNLOADED_ITEMS_GET, arg, cunt, cunt_out);
}

uint32_t libresteam_backend_steam_wshop_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_wshop_ops_t *ops = 0x0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    ops = &steam->wshop_ops;
    ops->query_user_create = steam_wshop_query_user_create;
    ops->query_all_page_create = steam_wshop_query_all_page_create;
    ops->query_all_cursor_create = steam_wshop_query_all_cursor_create;
    ops->query_details_create = steam_wshop_query_details_create;
    ops->query_send = steam_wshop_query_send;
    ops->query_res_get = steam_wshop_query_res_get;
    ops->query_tag_cunt_get = steam_wshop_query_tag_cunt_get;
    ops->query_tag_get = steam_wshop_query_tag_get;
    ops->query_tag_display_name_get = steam_wshop_query_tag_display_name_get;
    ops->query_preview_url_get = steam_wshop_query_preview_url_get;
    ops->query_metadata_get = steam_wshop_query_metadata_get;
    ops->query_children_get = steam_wshop_query_children_get;
    ops->query_statistic_get = steam_wshop_query_statistic_get;
    ops->query_additional_preview_cunt_get = steam_wshop_query_additional_preview_cunt_get;
    ops->query_additional_preview_get = steam_wshop_query_additional_preview_get;
    ops->query_kv_tag_cunt_get = steam_wshop_query_kv_tag_cunt_get;
    ops->query_kv_tag_get = steam_wshop_query_kv_tag_get;
    ops->query_first_kv_tag_get = steam_wshop_query_first_kv_tag_get;
    ops->query_game_version_cunt_get = steam_wshop_query_game_version_cunt_get;
    ops->query_game_version_get = steam_wshop_query_game_version_get;
    ops->query_content_desc_get = steam_wshop_query_content_desc_get;
    ops->query_release = steam_wshop_query_release;
    ops->query_required_tag_add = steam_wshop_query_required_tag_add;
    ops->query_required_tag_group_add = steam_wshop_query_required_tag_group_add;
    ops->query_excluded_tag_add = steam_wshop_query_excluded_tag_add;
    ops->query_ret_only_ids_set = steam_wshop_query_ret_only_ids_set;
    ops->query_ret_kv_tags_set = steam_wshop_query_ret_kv_tags_set;
    ops->query_ret_long_desc_set = steam_wshop_query_ret_long_desc_set;
    ops->query_ret_metadata_set = steam_wshop_query_ret_metadata_set;
    ops->query_ret_children_set = steam_wshop_query_ret_children_set;
    ops->query_ret_additional_previews_set = steam_wshop_query_ret_additional_previews_set;
    ops->query_ret_total_only_set = steam_wshop_query_ret_total_only_set;
    ops->query_ret_playtime_stats_set = steam_wshop_query_ret_playtime_stats_set;
    ops->query_language_set = steam_wshop_query_language_set;
    ops->query_cached_resp_max_age_set = steam_wshop_query_cached_resp_max_age_set;
    ops->query_admin_set = steam_wshop_query_admin_set;
    ops->query_cloud_file_name_filter_set = steam_wshop_query_cloud_file_name_filter_set;
    ops->query_match_any_tag_set = steam_wshop_query_match_any_tag_set;
    ops->query_search_text_set = steam_wshop_query_search_text_set;
    ops->query_ranked_by_trend_days_set = steam_wshop_query_ranked_by_trend_days_set;
    ops->query_time_created_range_set = steam_wshop_query_time_created_range_set;
    ops->query_time_updated_range_set = steam_wshop_query_time_updated_range_set;
    ops->query_required_kv_tag_add = steam_wshop_query_required_kv_tag_add;
    ops->details_req = steam_wshop_details_req;
    ops->item_create = steam_wshop_item_create;
    ops->item_update_start = steam_wshop_item_update_start;
    ops->item_title_set = steam_wshop_item_title_set;
    ops->item_desc_set = steam_wshop_item_desc_set;
    ops->item_update_language_set = steam_wshop_item_update_language_set;
    ops->item_metadata_set = steam_wshop_item_metadata_set;
    ops->item_visibility_set = steam_wshop_item_visibility_set;
    ops->item_tags_set = steam_wshop_item_tags_set;
    ops->item_content_set = steam_wshop_item_content_set;
    ops->item_preview_set = steam_wshop_item_preview_set;
    ops->item_legacy_upload_set = steam_wshop_item_legacy_upload_set;
    ops->item_kv_tags_all_remove = steam_wshop_item_kv_tags_all_remove;
    ops->item_kv_tags_remove = steam_wshop_item_kv_tags_remove;
    ops->item_kv_tag_add = steam_wshop_item_kv_tag_add;
    ops->item_preview_file_add = steam_wshop_item_preview_file_add;
    ops->item_preview_video_add = steam_wshop_item_preview_video_add;
    ops->item_preview_file_update = steam_wshop_item_preview_file_update;
    ops->item_preview_video_update = steam_wshop_item_preview_video_update;
    ops->item_preview_remove = steam_wshop_item_preview_remove;
    ops->item_content_desc_add = steam_wshop_item_content_desc_add;
    ops->item_content_desc_remove = steam_wshop_item_content_desc_remove;
    ops->item_required_game_versions_set = steam_wshop_item_required_game_versions_set;
    ops->item_update_submit = steam_wshop_item_update_submit;
    ops->item_update_progress_get = steam_wshop_item_update_progress_get;
    ops->item_vote_set = steam_wshop_item_vote_set;
    ops->item_vote_get = steam_wshop_item_vote_get;
    ops->favorite_add = steam_wshop_favorite_add;
    ops->favorite_remove = steam_wshop_favorite_remove;
    ops->item_subscribe = steam_wshop_item_subscribe;
    ops->item_unsubscribe = steam_wshop_item_unsubscribe;
    ops->subscribed_item_cunt_get = steam_wshop_subscribed_item_cunt_get;
    ops->subscribed_items_get = steam_wshop_subscribed_items_get;
    ops->item_state_get = steam_wshop_item_state_get;
    ops->item_install_info_get = steam_wshop_item_install_info_get;
    ops->item_download_info_get = steam_wshop_item_download_info_get;
    ops->item_download = steam_wshop_item_download;
    ops->game_srv_init = steam_wshop_game_srv_init;
    ops->downloads_suspend = steam_wshop_downloads_suspend;
    ops->playtime_tracking_start = steam_wshop_playtime_tracking_start;
    ops->playtime_tracking_stop = steam_wshop_playtime_tracking_stop;
    ops->playtime_tracking_all_stop = steam_wshop_playtime_tracking_all_stop;
    ops->dependency_add = steam_wshop_dependency_add;
    ops->dependency_remove = steam_wshop_dependency_remove;
    ops->app_dependency_add = steam_wshop_app_dependency_add;
    ops->app_dependency_remove = steam_wshop_app_dependency_remove;
    ops->app_dependencies_get = steam_wshop_app_dependencies_get;
    ops->item_delete = steam_wshop_item_delete;
    ops->eula_show = steam_wshop_eula_show;
    ops->eula_status_get = steam_wshop_eula_status_get;
    ops->user_content_desc_preference_get = steam_wshop_user_content_desc_preference_get;
    ops->items_disabled_locally_set = steam_wshop_items_disabled_locally_set;
    ops->subscribed_ld_order_set = steam_wshop_subscribed_ld_order_set;
    ops->item_unused_mark = steam_wshop_item_unused_mark;
    ops->downloaded_item_cunt_get = steam_wshop_downloaded_item_cunt_get;
    ops->downloaded_items_get = steam_wshop_downloaded_items_get;
    steam->ops.wshop = &steam->wshop_ops;

    return LDG_ERR_AOK;
}
