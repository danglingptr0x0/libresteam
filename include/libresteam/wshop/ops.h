#ifndef LIBRESTEAM_WSHOP_OPS_H
#define LIBRESTEAM_WSHOP_OPS_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/wshop/pform.h>
#include <libresteam/wshop/types.h>

typedef struct libresteam_wshop_ops
{
    uint32_t (*query_user_create)(void *ctx, uint32_t account, uint32_t list_type, uint32_t matching_type, uint32_t sort_order, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out);
    uint32_t (*query_all_page_create)(void *ctx, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out);
    uint32_t (*query_all_cursor_create)(void *ctx, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, const char *cursor, uint64_t *query_out);
    uint32_t (*query_details_create)(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *query_out);
    uint32_t (*query_send)(void *ctx, uint64_t query, uint64_t *call_out);
    uint32_t (*query_res_get)(void *ctx, uint64_t query, uint32_t idx, libresteam_wshop_details_t *out);
    uint32_t (*query_tag_cunt_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t *out);
    uint32_t (*query_tag_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap);
    uint32_t (*query_tag_display_name_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap);
    uint32_t (*query_preview_url_get)(void *ctx, uint64_t query, uint32_t idx, char *buff, uint32_t cap);
    uint32_t (*query_metadata_get)(void *ctx, uint64_t query, uint32_t idx, char *buff, uint32_t cap);
    uint32_t (*query_children_get)(void *ctx, uint64_t query, uint32_t idx, uint64_t *file_id, uint32_t cap);
    uint32_t (*query_statistic_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t stat_type, uint64_t *out);
    uint32_t (*query_additional_preview_cunt_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t *out);
    uint32_t (*query_additional_preview_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t preview_idx, char *url_buff, uint32_t url_cap, char *orig_file_name_buff, uint32_t orig_file_name_cap, uint32_t *preview_type_out);
    uint32_t (*query_kv_tag_cunt_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t *out);
    uint32_t (*query_kv_tag_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t kv_idx, char *key_buff, uint32_t key_cap, char *val_buff, uint32_t val_cap);
    uint32_t (*query_first_kv_tag_get)(void *ctx, uint64_t query, uint32_t idx, const char *key, char *val_buff, uint32_t val_cap);
    uint32_t (*query_game_version_cunt_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t *out);
    uint32_t (*query_game_version_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t version_idx, char *branch_min_buff, char *branch_max_buff, uint32_t cap);
    uint32_t (*query_content_desc_get)(void *ctx, uint64_t query, uint32_t idx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*query_release)(void *ctx, uint64_t query);
    uint32_t (*query_required_tag_add)(void *ctx, uint64_t query, const char *tag);
    uint32_t (*query_required_tag_group_add)(void *ctx, uint64_t query, const libresteam_wshop_str_arr_t *group);
    uint32_t (*query_excluded_tag_add)(void *ctx, uint64_t query, const char *tag);
    uint32_t (*query_ret_only_ids_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_kv_tags_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_long_desc_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_metadata_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_children_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_additional_previews_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_total_only_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_ret_playtime_stats_set)(void *ctx, uint64_t query, uint32_t days);
    uint32_t (*query_language_set)(void *ctx, uint64_t query, const char *language);
    uint32_t (*query_cached_resp_max_age_set)(void *ctx, uint64_t query, uint32_t max_age_sec);
    uint32_t (*query_admin_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_cloud_file_name_filter_set)(void *ctx, uint64_t query, const char *file_name);
    uint32_t (*query_match_any_tag_set)(void *ctx, uint64_t query, uint8_t en);
    uint32_t (*query_search_text_set)(void *ctx, uint64_t query, const char *text);
    uint32_t (*query_ranked_by_trend_days_set)(void *ctx, uint64_t query, uint32_t days);
    uint32_t (*query_time_created_range_set)(void *ctx, uint64_t query, uint32_t time_start, uint32_t time_end);
    uint32_t (*query_time_updated_range_set)(void *ctx, uint64_t query, uint32_t time_start, uint32_t time_end);
    uint32_t (*query_required_kv_tag_add)(void *ctx, uint64_t query, const char *key, const char *val);
    uint32_t (*details_req)(void *ctx, uint64_t file_id, uint32_t max_age_sec, uint64_t *call_out);
    uint32_t (*item_create)(void *ctx, uint32_t consumer_app, uint32_t file_type, uint64_t *call_out);
    uint32_t (*item_update_start)(void *ctx, uint32_t consumer_app, uint64_t file_id, uint64_t *update_out);
    uint32_t (*item_title_set)(void *ctx, uint64_t update, const char *title);
    uint32_t (*item_desc_set)(void *ctx, uint64_t update, const char *desc);
    uint32_t (*item_update_language_set)(void *ctx, uint64_t update, const char *language);
    uint32_t (*item_metadata_set)(void *ctx, uint64_t update, const char *metadata);
    uint32_t (*item_visibility_set)(void *ctx, uint64_t update, uint32_t visibility);
    uint32_t (*item_tags_set)(void *ctx, uint64_t update, const libresteam_wshop_str_arr_t *tags, uint8_t admin_tags_en);
    uint32_t (*item_content_set)(void *ctx, uint64_t update, const char *folder);
    uint32_t (*item_preview_set)(void *ctx, uint64_t update, const char *file);
    uint32_t (*item_legacy_upload_set)(void *ctx, uint64_t update, uint8_t en);
    uint32_t (*item_kv_tags_all_remove)(void *ctx, uint64_t update);
    uint32_t (*item_kv_tags_remove)(void *ctx, uint64_t update, const char *key);
    uint32_t (*item_kv_tag_add)(void *ctx, uint64_t update, const char *key, const char *val);
    uint32_t (*item_preview_file_add)(void *ctx, uint64_t update, const char *file, uint32_t preview_type);
    uint32_t (*item_preview_video_add)(void *ctx, uint64_t update, const char *video_id);
    uint32_t (*item_preview_file_update)(void *ctx, uint64_t update, uint32_t idx, const char *file);
    uint32_t (*item_preview_video_update)(void *ctx, uint64_t update, uint32_t idx, const char *video_id);
    uint32_t (*item_preview_remove)(void *ctx, uint64_t update, uint32_t idx);
    uint32_t (*item_content_desc_add)(void *ctx, uint64_t update, uint32_t desc_id);
    uint32_t (*item_content_desc_remove)(void *ctx, uint64_t update, uint32_t desc_id);
    uint32_t (*item_required_game_versions_set)(void *ctx, uint64_t update, const char *branch_min, const char *branch_max);
    uint32_t (*item_update_submit)(void *ctx, uint64_t update, const char *change_note, uint64_t *call_out);
    uint32_t (*item_update_progress_get)(void *ctx, uint64_t update, uint64_t *done_out, uint64_t *total_out, uint32_t *status_out);
    uint32_t (*item_vote_set)(void *ctx, uint64_t file_id, uint8_t vote_up, uint64_t *call_out);
    uint32_t (*item_vote_get)(void *ctx, uint64_t file_id, uint64_t *call_out);
    uint32_t (*favorite_add)(void *ctx, uint32_t app, uint64_t file_id, uint64_t *call_out);
    uint32_t (*favorite_remove)(void *ctx, uint32_t app, uint64_t file_id, uint64_t *call_out);
    uint32_t (*item_subscribe)(void *ctx, uint64_t file_id, uint64_t *call_out);
    uint32_t (*item_unsubscribe)(void *ctx, uint64_t file_id, uint64_t *call_out);
    uint32_t (*subscribed_item_cunt_get)(void *ctx, uint8_t disabled_locally_en, uint32_t *out);
    uint32_t (*subscribed_items_get)(void *ctx, uint64_t *file_id, uint32_t cap, uint8_t disabled_locally_en, uint32_t *cunt_out);
    uint32_t (*item_state_get)(void *ctx, uint64_t file_id, uint32_t *out);
    uint32_t (*item_install_info_get)(void *ctx, uint64_t file_id, uint64_t *size_on_disk_out, char *folder_buff, uint32_t cap, uint32_t *timestamp_out);
    uint32_t (*item_download_info_get)(void *ctx, uint64_t file_id, uint64_t *downloaded_out, uint64_t *total_out);
    uint32_t (*item_download)(void *ctx, uint64_t file_id, uint8_t high_prio);
    uint32_t (*game_srv_init)(void *ctx, uint32_t depot, const char *folder);
    uint32_t (*downloads_suspend)(void *ctx, uint8_t suspend);
    uint32_t (*playtime_tracking_start)(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out);
    uint32_t (*playtime_tracking_stop)(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out);
    uint32_t (*playtime_tracking_all_stop)(void *ctx, uint64_t *call_out);
    uint32_t (*dependency_add)(void *ctx, uint64_t parent, uint64_t child, uint64_t *call_out);
    uint32_t (*dependency_remove)(void *ctx, uint64_t parent, uint64_t child, uint64_t *call_out);
    uint32_t (*app_dependency_add)(void *ctx, uint64_t file_id, uint32_t app, uint64_t *call_out);
    uint32_t (*app_dependency_remove)(void *ctx, uint64_t file_id, uint32_t app, uint64_t *call_out);
    uint32_t (*app_dependencies_get)(void *ctx, uint64_t file_id, uint64_t *call_out);
    uint32_t (*item_delete)(void *ctx, uint64_t file_id, uint64_t *call_out);
    uint32_t (*eula_show)(void *ctx);
    uint32_t (*eula_status_get)(void *ctx, uint64_t *call_out);
    uint32_t (*user_content_desc_preference_get)(void *ctx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out);
    uint32_t (*items_disabled_locally_set)(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt, uint8_t disabled);
    uint32_t (*subscribed_ld_order_set)(void *ctx, const uint64_t *file_id, uint32_t file_id_cunt);
    uint32_t (*item_unused_mark)(void *ctx, uint64_t file_id);
    uint32_t (*downloaded_item_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*downloaded_items_get)(void *ctx, uint64_t *file_id, uint32_t cap, uint32_t *cunt_out);
} libresteam_wshop_ops_t;

#endif
