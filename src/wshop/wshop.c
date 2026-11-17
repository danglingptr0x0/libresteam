#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/wshop/cb.h>
#include <libresteam/wshop/ops.h>
#include <libresteam/wshop/pform.h>
#include <libresteam/wshop/types.h>
#include <libresteam/wshop/wshop.h>

#define WSHOP_LAYOUT_PASS 1
#define WSHOP_LAYOUT_FAIL 0

typedef struct { uint32_t ck : (sizeof(libresteam_wshop_details_t) == LIBRESTEAM_WSHOP_DETAILS_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_details_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_str_arr_t) == LIBRESTEAM_WSHOP_STR_ARR_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_str_arr_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_query_completed_t) == LIBRESTEAM_WSHOP_CB_QUERY_COMPLETED_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_query_completed_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_submit_res_t) == LIBRESTEAM_WSHOP_CB_SUBMIT_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_submit_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_favorite_list_changed_t) == LIBRESTEAM_WSHOP_CB_FAVORITE_LIST_CHANGED_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_favorite_list_changed_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_vote_set_res_t) == LIBRESTEAM_WSHOP_CB_VOTE_SET_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_vote_set_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_vote_get_res_t) == LIBRESTEAM_WSHOP_CB_VOTE_GET_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_vote_get_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_playtime_res_t) == LIBRESTEAM_WSHOP_CB_PLAYTIME_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_playtime_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_subscribed_list_changed_t) == LIBRESTEAM_WSHOP_CB_SUBSCRIBED_LIST_CHANGED_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_subscribed_list_changed_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_eula_status_t) == LIBRESTEAM_WSHOP_CB_EULA_STATUS_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_eula_status_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_details_res_t) == LIBRESTEAM_WSHOP_CB_DETAILS_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_details_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_create_res_t) == LIBRESTEAM_WSHOP_CB_CREATE_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_create_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_installed_t) == LIBRESTEAM_WSHOP_CB_INSTALLED_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_installed_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_download_res_t) == LIBRESTEAM_WSHOP_CB_DOWNLOAD_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_download_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_dependency_res_t) == LIBRESTEAM_WSHOP_CB_DEPENDENCY_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_dependency_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_app_dependency_res_t) == LIBRESTEAM_WSHOP_CB_APP_DEPENDENCY_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_app_dependency_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_app_dependencies_res_t) == LIBRESTEAM_WSHOP_CB_APP_DEPENDENCIES_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_app_dependencies_res_size_ck_t;
typedef struct { uint32_t ck : (sizeof(libresteam_wshop_cb_file_res_t) == LIBRESTEAM_WSHOP_CB_FILE_RES_SIZE) ? WSHOP_LAYOUT_PASS : WSHOP_LAYOUT_FAIL;
} wshop_cb_file_res_size_ck_t;

static uint32_t wshop_ops_get(const libresteam_backend_set_t *set, const libresteam_wshop_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->wshop) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->wshop;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_wshop_query_user_create(const libresteam_backend_set_t *set, uint32_t account, uint32_t list_type, uint32_t matching_type, uint32_t sort_order, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_user_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_user_create(ctx, account, list_type, matching_type, sort_order, creator_app, consumer_app, page, query_out);
}

uint32_t libresteam_wshop_query_all_page_create(const libresteam_backend_set_t *set, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_all_page_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_all_page_create(ctx, query_type, matching_type, creator_app, consumer_app, page, query_out);
}

uint32_t libresteam_wshop_query_all_cursor_create(const libresteam_backend_set_t *set, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, const char *cursor, uint64_t *query_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_all_cursor_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_all_cursor_create(ctx, query_type, matching_type, creator_app, consumer_app, cursor, query_out);
}

uint32_t libresteam_wshop_query_details_create(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *query_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_details_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_details_create(ctx, file_id, file_id_cunt, query_out);
}

uint32_t libresteam_wshop_query_send(const libresteam_backend_set_t *set, uint64_t query, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_send(ctx, query, call_out);
}

uint32_t libresteam_wshop_query_res_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, libresteam_wshop_details_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_res_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_res_get(ctx, query, idx, out);
}

uint32_t libresteam_wshop_query_tag_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_tag_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_tag_cunt_get(ctx, query, idx, out);
}

uint32_t libresteam_wshop_query_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_tag_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_tag_get(ctx, query, idx, tag_idx, buff, cap);
}

uint32_t libresteam_wshop_query_tag_display_name_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_tag_display_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_tag_display_name_get(ctx, query, idx, tag_idx, buff, cap);
}

uint32_t libresteam_wshop_query_preview_url_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, char *buff, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_preview_url_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_preview_url_get(ctx, query, idx, buff, cap);
}

uint32_t libresteam_wshop_query_metadata_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, char *buff, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_metadata_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_metadata_get(ctx, query, idx, buff, cap);
}

uint32_t libresteam_wshop_query_children_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint64_t *file_id, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_children_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_children_get(ctx, query, idx, file_id, cap);
}

uint32_t libresteam_wshop_query_statistic_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t stat_type, uint64_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_statistic_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_statistic_get(ctx, query, idx, stat_type, out);
}

uint32_t libresteam_wshop_query_additional_preview_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_additional_preview_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_additional_preview_cunt_get(ctx, query, idx, out);
}

uint32_t libresteam_wshop_query_additional_preview_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t preview_idx, char *url_buff, uint32_t url_cap, char *orig_file_name_buff, uint32_t orig_file_name_cap, uint32_t *preview_type_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!preview_type_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *preview_type_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_additional_preview_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_additional_preview_get(ctx, query, idx, preview_idx, url_buff, url_cap, orig_file_name_buff, orig_file_name_cap, preview_type_out);
}

uint32_t libresteam_wshop_query_kv_tag_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_kv_tag_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_kv_tag_cunt_get(ctx, query, idx, out);
}

uint32_t libresteam_wshop_query_kv_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t kv_idx, char *key_buff, uint32_t key_cap, char *val_buff, uint32_t val_cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_kv_tag_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_kv_tag_get(ctx, query, idx, kv_idx, key_buff, key_cap, val_buff, val_cap);
}

uint32_t libresteam_wshop_query_first_kv_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, const char *key, char *val_buff, uint32_t val_cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_first_kv_tag_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_first_kv_tag_get(ctx, query, idx, key, val_buff, val_cap);
}

uint32_t libresteam_wshop_query_game_version_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_game_version_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_game_version_cunt_get(ctx, query, idx, out);
}

uint32_t libresteam_wshop_query_game_version_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t version_idx, char *branch_min_buff, char *branch_max_buff, uint32_t cap)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_game_version_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_game_version_get(ctx, query, idx, version_idx, branch_min_buff, branch_max_buff, cap);
}

uint32_t libresteam_wshop_query_content_desc_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_content_desc_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_content_desc_get(ctx, query, idx, desc_id, cap, cunt_out);
}

uint32_t libresteam_wshop_query_release(const libresteam_backend_set_t *set, uint64_t query)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_release) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_release(ctx, query);
}

uint32_t libresteam_wshop_query_required_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *tag)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_required_tag_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_required_tag_add(ctx, query, tag);
}

uint32_t libresteam_wshop_query_required_tag_group_add(const libresteam_backend_set_t *set, uint64_t query, const libresteam_wshop_str_arr_t *group)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_required_tag_group_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_required_tag_group_add(ctx, query, group);
}

uint32_t libresteam_wshop_query_excluded_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *tag)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_excluded_tag_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_excluded_tag_add(ctx, query, tag);
}

uint32_t libresteam_wshop_query_ret_only_ids_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_only_ids_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_only_ids_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_kv_tags_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_kv_tags_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_kv_tags_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_long_desc_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_long_desc_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_long_desc_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_metadata_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_metadata_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_metadata_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_children_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_children_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_children_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_additional_previews_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_additional_previews_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_additional_previews_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_total_only_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_total_only_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_total_only_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_ret_playtime_stats_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t days)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ret_playtime_stats_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ret_playtime_stats_set(ctx, query, days);
}

uint32_t libresteam_wshop_query_language_set(const libresteam_backend_set_t *set, uint64_t query, const char *language)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_language_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_language_set(ctx, query, language);
}

uint32_t libresteam_wshop_query_cached_resp_max_age_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t max_age_sec)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_cached_resp_max_age_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_cached_resp_max_age_set(ctx, query, max_age_sec);
}

uint32_t libresteam_wshop_query_admin_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_admin_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_admin_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_cloud_file_name_filter_set(const libresteam_backend_set_t *set, uint64_t query, const char *file_name)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_cloud_file_name_filter_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_cloud_file_name_filter_set(ctx, query, file_name);
}

uint32_t libresteam_wshop_query_match_any_tag_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_match_any_tag_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_match_any_tag_set(ctx, query, en);
}

uint32_t libresteam_wshop_query_search_text_set(const libresteam_backend_set_t *set, uint64_t query, const char *text)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_search_text_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_search_text_set(ctx, query, text);
}

uint32_t libresteam_wshop_query_ranked_by_trend_days_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t days)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_ranked_by_trend_days_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_ranked_by_trend_days_set(ctx, query, days);
}

uint32_t libresteam_wshop_query_time_created_range_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t time_start, uint32_t time_end)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_time_created_range_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_time_created_range_set(ctx, query, time_start, time_end);
}

uint32_t libresteam_wshop_query_time_updated_range_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t time_start, uint32_t time_end)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_time_updated_range_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_time_updated_range_set(ctx, query, time_start, time_end);
}

uint32_t libresteam_wshop_query_required_kv_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *key, const char *val)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->query_required_kv_tag_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->query_required_kv_tag_add(ctx, query, key, val);
}

uint32_t libresteam_wshop_details_req(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t max_age_sec, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->details_req) { return LDG_ERR_UNSUPPORTED; }

    return ops->details_req(ctx, file_id, max_age_sec, call_out);
}

uint32_t libresteam_wshop_item_create(const libresteam_backend_set_t *set, uint32_t consumer_app, uint32_t file_type, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_create(ctx, consumer_app, file_type, call_out);
}

uint32_t libresteam_wshop_item_update_start(const libresteam_backend_set_t *set, uint32_t consumer_app, uint64_t file_id, uint64_t *update_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!update_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *update_out = LIBRESTEAM_WSHOP_HANDLE_INVALID;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_update_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_update_start(ctx, consumer_app, file_id, update_out);
}

uint32_t libresteam_wshop_item_title_set(const libresteam_backend_set_t *set, uint64_t update, const char *title)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_title_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_title_set(ctx, update, title);
}

uint32_t libresteam_wshop_item_desc_set(const libresteam_backend_set_t *set, uint64_t update, const char *desc)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_desc_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_desc_set(ctx, update, desc);
}

uint32_t libresteam_wshop_item_update_language_set(const libresteam_backend_set_t *set, uint64_t update, const char *language)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_update_language_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_update_language_set(ctx, update, language);
}

uint32_t libresteam_wshop_item_metadata_set(const libresteam_backend_set_t *set, uint64_t update, const char *metadata)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_metadata_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_metadata_set(ctx, update, metadata);
}

uint32_t libresteam_wshop_item_visibility_set(const libresteam_backend_set_t *set, uint64_t update, uint32_t visibility)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_visibility_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_visibility_set(ctx, update, visibility);
}

uint32_t libresteam_wshop_item_tags_set(const libresteam_backend_set_t *set, uint64_t update, const libresteam_wshop_str_arr_t *tags, uint8_t admin_tags_en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_tags_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_tags_set(ctx, update, tags, admin_tags_en);
}

uint32_t libresteam_wshop_item_content_set(const libresteam_backend_set_t *set, uint64_t update, const char *folder)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_content_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_content_set(ctx, update, folder);
}

uint32_t libresteam_wshop_item_preview_set(const libresteam_backend_set_t *set, uint64_t update, const char *file)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_set(ctx, update, file);
}

uint32_t libresteam_wshop_item_legacy_upload_set(const libresteam_backend_set_t *set, uint64_t update, uint8_t en)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_legacy_upload_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_legacy_upload_set(ctx, update, en);
}

uint32_t libresteam_wshop_item_kv_tags_all_remove(const libresteam_backend_set_t *set, uint64_t update)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_kv_tags_all_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_kv_tags_all_remove(ctx, update);
}

uint32_t libresteam_wshop_item_kv_tags_remove(const libresteam_backend_set_t *set, uint64_t update, const char *key)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_kv_tags_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_kv_tags_remove(ctx, update, key);
}

uint32_t libresteam_wshop_item_kv_tag_add(const libresteam_backend_set_t *set, uint64_t update, const char *key, const char *val)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_kv_tag_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_kv_tag_add(ctx, update, key, val);
}

uint32_t libresteam_wshop_item_preview_file_add(const libresteam_backend_set_t *set, uint64_t update, const char *file, uint32_t preview_type)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_file_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_file_add(ctx, update, file, preview_type);
}

uint32_t libresteam_wshop_item_preview_video_add(const libresteam_backend_set_t *set, uint64_t update, const char *video_id)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_video_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_video_add(ctx, update, video_id);
}

uint32_t libresteam_wshop_item_preview_file_update(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx, const char *file)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_file_update) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_file_update(ctx, update, idx, file);
}

uint32_t libresteam_wshop_item_preview_video_update(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx, const char *video_id)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_video_update) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_video_update(ctx, update, idx, video_id);
}

uint32_t libresteam_wshop_item_preview_remove(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_preview_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_preview_remove(ctx, update, idx);
}

uint32_t libresteam_wshop_item_content_desc_add(const libresteam_backend_set_t *set, uint64_t update, uint32_t desc_id)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_content_desc_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_content_desc_add(ctx, update, desc_id);
}

uint32_t libresteam_wshop_item_content_desc_remove(const libresteam_backend_set_t *set, uint64_t update, uint32_t desc_id)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_content_desc_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_content_desc_remove(ctx, update, desc_id);
}

uint32_t libresteam_wshop_item_required_game_versions_set(const libresteam_backend_set_t *set, uint64_t update, const char *branch_min, const char *branch_max)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_required_game_versions_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_required_game_versions_set(ctx, update, branch_min, branch_max);
}

uint32_t libresteam_wshop_item_update_submit(const libresteam_backend_set_t *set, uint64_t update, const char *change_note, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_update_submit) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_update_submit(ctx, update, change_note, call_out);
}

uint32_t libresteam_wshop_item_update_progress_get(const libresteam_backend_set_t *set, uint64_t update, uint64_t *done_out, uint64_t *total_out, uint32_t *status_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!done_out || !total_out || !status_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *done_out = 0;
    *total_out = 0;
    *status_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_update_progress_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_update_progress_get(ctx, update, done_out, total_out, status_out);
}

uint32_t libresteam_wshop_item_vote_set(const libresteam_backend_set_t *set, uint64_t file_id, uint8_t vote_up, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_vote_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_vote_set(ctx, file_id, vote_up, call_out);
}

uint32_t libresteam_wshop_item_vote_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_vote_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_vote_get(ctx, file_id, call_out);
}

uint32_t libresteam_wshop_favorite_add(const libresteam_backend_set_t *set, uint32_t app, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_add(ctx, app, file_id, call_out);
}

uint32_t libresteam_wshop_favorite_remove(const libresteam_backend_set_t *set, uint32_t app, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->favorite_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->favorite_remove(ctx, app, file_id, call_out);
}

uint32_t libresteam_wshop_item_subscribe(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_subscribe) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_subscribe(ctx, file_id, call_out);
}

uint32_t libresteam_wshop_item_unsubscribe(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_unsubscribe) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_unsubscribe(ctx, file_id, call_out);
}

uint32_t libresteam_wshop_subscribed_item_cunt_get(const libresteam_backend_set_t *set, uint8_t disabled_locally_en, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->subscribed_item_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->subscribed_item_cunt_get(ctx, disabled_locally_en, out);
}

uint32_t libresteam_wshop_subscribed_items_get(const libresteam_backend_set_t *set, uint64_t *file_id, uint32_t cap, uint8_t disabled_locally_en, uint32_t *cunt_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->subscribed_items_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->subscribed_items_get(ctx, file_id, cap, disabled_locally_en, cunt_out);
}

uint32_t libresteam_wshop_item_state_get(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_WSHOP_ITEM_STATE_NONE;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_state_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_state_get(ctx, file_id, out);
}

uint32_t libresteam_wshop_item_install_info_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *size_on_disk_out, char *folder_buff, uint32_t cap, uint32_t *timestamp_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!size_on_disk_out || !timestamp_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *size_on_disk_out = 0;
    *timestamp_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_install_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_install_info_get(ctx, file_id, size_on_disk_out, folder_buff, cap, timestamp_out);
}

uint32_t libresteam_wshop_item_download_info_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *downloaded_out, uint64_t *total_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!downloaded_out || !total_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *downloaded_out = 0;
    *total_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_download_info_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_download_info_get(ctx, file_id, downloaded_out, total_out);
}

uint32_t libresteam_wshop_item_download(const libresteam_backend_set_t *set, uint64_t file_id, uint8_t high_prio)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_download) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_download(ctx, file_id, high_prio);
}

uint32_t libresteam_wshop_game_srv_init(const libresteam_backend_set_t *set, uint32_t depot, const char *folder)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->game_srv_init) { return LDG_ERR_UNSUPPORTED; }

    return ops->game_srv_init(ctx, depot, folder);
}

uint32_t libresteam_wshop_downloads_suspend(const libresteam_backend_set_t *set, uint8_t suspend)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->downloads_suspend) { return LDG_ERR_UNSUPPORTED; }

    return ops->downloads_suspend(ctx, suspend);
}

uint32_t libresteam_wshop_playtime_tracking_start(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->playtime_tracking_start) { return LDG_ERR_UNSUPPORTED; }

    return ops->playtime_tracking_start(ctx, file_id, file_id_cunt, call_out);
}

uint32_t libresteam_wshop_playtime_tracking_stop(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->playtime_tracking_stop) { return LDG_ERR_UNSUPPORTED; }

    return ops->playtime_tracking_stop(ctx, file_id, file_id_cunt, call_out);
}

uint32_t libresteam_wshop_playtime_tracking_all_stop(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->playtime_tracking_all_stop) { return LDG_ERR_UNSUPPORTED; }

    return ops->playtime_tracking_all_stop(ctx, call_out);
}

uint32_t libresteam_wshop_dependency_add(const libresteam_backend_set_t *set, uint64_t parent, uint64_t child, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dependency_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->dependency_add(ctx, parent, child, call_out);
}

uint32_t libresteam_wshop_dependency_remove(const libresteam_backend_set_t *set, uint64_t parent, uint64_t child, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->dependency_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->dependency_remove(ctx, parent, child, call_out);
}

uint32_t libresteam_wshop_app_dependency_add(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t app, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_dependency_add) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_dependency_add(ctx, file_id, app, call_out);
}

uint32_t libresteam_wshop_app_dependency_remove(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t app, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_dependency_remove) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_dependency_remove(ctx, file_id, app, call_out);
}

uint32_t libresteam_wshop_app_dependencies_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->app_dependencies_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->app_dependencies_get(ctx, file_id, call_out);
}

uint32_t libresteam_wshop_item_delete(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_delete) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_delete(ctx, file_id, call_out);
}

uint32_t libresteam_wshop_eula_show(const libresteam_backend_set_t *set)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->eula_show) { return LDG_ERR_UNSUPPORTED; }

    return ops->eula_show(ctx);
}

uint32_t libresteam_wshop_eula_status_get(const libresteam_backend_set_t *set, uint64_t *call_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!call_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *call_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->eula_status_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->eula_status_get(ctx, call_out);
}

uint32_t libresteam_wshop_user_content_desc_preference_get(const libresteam_backend_set_t *set, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->user_content_desc_preference_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->user_content_desc_preference_get(ctx, desc_id, cap, cunt_out);
}

uint32_t libresteam_wshop_items_disabled_locally_set(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint8_t disabled)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->items_disabled_locally_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->items_disabled_locally_set(ctx, file_id, file_id_cunt, disabled);
}

uint32_t libresteam_wshop_subscribed_ld_order_set(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->subscribed_ld_order_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->subscribed_ld_order_set(ctx, file_id, file_id_cunt);
}

uint32_t libresteam_wshop_item_unused_mark(const libresteam_backend_set_t *set, uint64_t file_id)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->item_unused_mark) { return LDG_ERR_UNSUPPORTED; }

    return ops->item_unused_mark(ctx, file_id);
}

uint32_t libresteam_wshop_downloaded_item_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->downloaded_item_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->downloaded_item_cunt_get(ctx, out);
}

uint32_t libresteam_wshop_downloaded_items_get(const libresteam_backend_set_t *set, uint64_t *file_id, uint32_t cap, uint32_t *cunt_out)
{
    const libresteam_wshop_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!cunt_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *cunt_out = 0;

    res = wshop_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->downloaded_items_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->downloaded_items_get(ctx, file_id, cap, cunt_out);
}
