#ifndef LIBRESTEAM_WSHOP_H
#define LIBRESTEAM_WSHOP_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/wshop/ops.h>
#include <libresteam/wshop/pform.h>
#include <libresteam/wshop/types.h>

LDG_EXPORT uint32_t libresteam_wshop_query_user_create(const libresteam_backend_set_t *set, uint32_t account, uint32_t list_type, uint32_t matching_type, uint32_t sort_order, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out);
LDG_EXPORT uint32_t libresteam_wshop_query_all_page_create(const libresteam_backend_set_t *set, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, uint32_t page, uint64_t *query_out);
LDG_EXPORT uint32_t libresteam_wshop_query_all_cursor_create(const libresteam_backend_set_t *set, uint32_t query_type, uint32_t matching_type, uint32_t creator_app, uint32_t consumer_app, const char *cursor, uint64_t *query_out);
LDG_EXPORT uint32_t libresteam_wshop_query_details_create(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *query_out);
LDG_EXPORT uint32_t libresteam_wshop_query_send(const libresteam_backend_set_t *set, uint64_t query, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_query_res_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, libresteam_wshop_details_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_tag_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_tag_display_name_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t tag_idx, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_preview_url_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_metadata_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, char *buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_children_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint64_t *file_id, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_statistic_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t stat_type, uint64_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_additional_preview_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_additional_preview_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t preview_idx, char *url_buff, uint32_t url_cap, char *orig_file_name_buff, uint32_t orig_file_name_cap, uint32_t *preview_type_out);
LDG_EXPORT uint32_t libresteam_wshop_query_kv_tag_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_kv_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t kv_idx, char *key_buff, uint32_t key_cap, char *val_buff, uint32_t val_cap);
LDG_EXPORT uint32_t libresteam_wshop_query_first_kv_tag_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, const char *key, char *val_buff, uint32_t val_cap);
LDG_EXPORT uint32_t libresteam_wshop_query_game_version_cunt_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_query_game_version_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t version_idx, char *branch_min_buff, char *branch_max_buff, uint32_t cap);
LDG_EXPORT uint32_t libresteam_wshop_query_content_desc_get(const libresteam_backend_set_t *set, uint64_t query, uint32_t idx, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_wshop_query_release(const libresteam_backend_set_t *set, uint64_t query);
LDG_EXPORT uint32_t libresteam_wshop_query_required_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *tag);
LDG_EXPORT uint32_t libresteam_wshop_query_required_tag_group_add(const libresteam_backend_set_t *set, uint64_t query, const libresteam_wshop_str_arr_t *group);
LDG_EXPORT uint32_t libresteam_wshop_query_excluded_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *tag);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_only_ids_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_kv_tags_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_long_desc_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_metadata_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_children_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_additional_previews_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_total_only_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_ret_playtime_stats_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t days);
LDG_EXPORT uint32_t libresteam_wshop_query_language_set(const libresteam_backend_set_t *set, uint64_t query, const char *language);
LDG_EXPORT uint32_t libresteam_wshop_query_cached_resp_max_age_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t max_age_sec);
LDG_EXPORT uint32_t libresteam_wshop_query_admin_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_cloud_file_name_filter_set(const libresteam_backend_set_t *set, uint64_t query, const char *file_name);
LDG_EXPORT uint32_t libresteam_wshop_query_match_any_tag_set(const libresteam_backend_set_t *set, uint64_t query, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_query_search_text_set(const libresteam_backend_set_t *set, uint64_t query, const char *text);
LDG_EXPORT uint32_t libresteam_wshop_query_ranked_by_trend_days_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t days);
LDG_EXPORT uint32_t libresteam_wshop_query_time_created_range_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t time_start, uint32_t time_end);
LDG_EXPORT uint32_t libresteam_wshop_query_time_updated_range_set(const libresteam_backend_set_t *set, uint64_t query, uint32_t time_start, uint32_t time_end);
LDG_EXPORT uint32_t libresteam_wshop_query_required_kv_tag_add(const libresteam_backend_set_t *set, uint64_t query, const char *key, const char *val);
LDG_EXPORT uint32_t libresteam_wshop_details_req(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t max_age_sec, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_create(const libresteam_backend_set_t *set, uint32_t consumer_app, uint32_t file_type, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_update_start(const libresteam_backend_set_t *set, uint32_t consumer_app, uint64_t file_id, uint64_t *update_out);
LDG_EXPORT uint32_t libresteam_wshop_item_title_set(const libresteam_backend_set_t *set, uint64_t update, const char *title);
LDG_EXPORT uint32_t libresteam_wshop_item_desc_set(const libresteam_backend_set_t *set, uint64_t update, const char *desc);
LDG_EXPORT uint32_t libresteam_wshop_item_update_language_set(const libresteam_backend_set_t *set, uint64_t update, const char *language);
LDG_EXPORT uint32_t libresteam_wshop_item_metadata_set(const libresteam_backend_set_t *set, uint64_t update, const char *metadata);
LDG_EXPORT uint32_t libresteam_wshop_item_visibility_set(const libresteam_backend_set_t *set, uint64_t update, uint32_t visibility);
LDG_EXPORT uint32_t libresteam_wshop_item_tags_set(const libresteam_backend_set_t *set, uint64_t update, const libresteam_wshop_str_arr_t *tags, uint8_t admin_tags_en);
LDG_EXPORT uint32_t libresteam_wshop_item_content_set(const libresteam_backend_set_t *set, uint64_t update, const char *folder);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_set(const libresteam_backend_set_t *set, uint64_t update, const char *file);
LDG_EXPORT uint32_t libresteam_wshop_item_legacy_upload_set(const libresteam_backend_set_t *set, uint64_t update, uint8_t en);
LDG_EXPORT uint32_t libresteam_wshop_item_kv_tags_all_remove(const libresteam_backend_set_t *set, uint64_t update);
LDG_EXPORT uint32_t libresteam_wshop_item_kv_tags_remove(const libresteam_backend_set_t *set, uint64_t update, const char *key);
LDG_EXPORT uint32_t libresteam_wshop_item_kv_tag_add(const libresteam_backend_set_t *set, uint64_t update, const char *key, const char *val);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_file_add(const libresteam_backend_set_t *set, uint64_t update, const char *file, uint32_t preview_type);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_video_add(const libresteam_backend_set_t *set, uint64_t update, const char *video_id);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_file_update(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx, const char *file);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_video_update(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx, const char *video_id);
LDG_EXPORT uint32_t libresteam_wshop_item_preview_remove(const libresteam_backend_set_t *set, uint64_t update, uint32_t idx);
LDG_EXPORT uint32_t libresteam_wshop_item_content_desc_add(const libresteam_backend_set_t *set, uint64_t update, uint32_t desc_id);
LDG_EXPORT uint32_t libresteam_wshop_item_content_desc_remove(const libresteam_backend_set_t *set, uint64_t update, uint32_t desc_id);
LDG_EXPORT uint32_t libresteam_wshop_item_required_game_versions_set(const libresteam_backend_set_t *set, uint64_t update, const char *branch_min, const char *branch_max);
LDG_EXPORT uint32_t libresteam_wshop_item_update_submit(const libresteam_backend_set_t *set, uint64_t update, const char *change_note, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_update_progress_get(const libresteam_backend_set_t *set, uint64_t update, uint64_t *done_out, uint64_t *total_out, uint32_t *status_out);
LDG_EXPORT uint32_t libresteam_wshop_item_vote_set(const libresteam_backend_set_t *set, uint64_t file_id, uint8_t vote_up, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_vote_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_favorite_add(const libresteam_backend_set_t *set, uint32_t app, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_favorite_remove(const libresteam_backend_set_t *set, uint32_t app, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_subscribe(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_unsubscribe(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_subscribed_item_cunt_get(const libresteam_backend_set_t *set, uint8_t disabled_locally_en, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_subscribed_items_get(const libresteam_backend_set_t *set, uint64_t *file_id, uint32_t cap, uint8_t disabled_locally_en, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_wshop_item_state_get(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_item_install_info_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *size_on_disk_out, char *folder_buff, uint32_t cap, uint32_t *timestamp_out);
LDG_EXPORT uint32_t libresteam_wshop_item_download_info_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *downloaded_out, uint64_t *total_out);
LDG_EXPORT uint32_t libresteam_wshop_item_download(const libresteam_backend_set_t *set, uint64_t file_id, uint8_t high_prio);
LDG_EXPORT uint32_t libresteam_wshop_game_srv_init(const libresteam_backend_set_t *set, uint32_t depot, const char *folder);
LDG_EXPORT uint32_t libresteam_wshop_downloads_suspend(const libresteam_backend_set_t *set, uint8_t suspend);
LDG_EXPORT uint32_t libresteam_wshop_playtime_tracking_start(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_playtime_tracking_stop(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_playtime_tracking_all_stop(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_dependency_add(const libresteam_backend_set_t *set, uint64_t parent, uint64_t child, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_dependency_remove(const libresteam_backend_set_t *set, uint64_t parent, uint64_t child, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_app_dependency_add(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t app, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_app_dependency_remove(const libresteam_backend_set_t *set, uint64_t file_id, uint32_t app, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_app_dependencies_get(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_item_delete(const libresteam_backend_set_t *set, uint64_t file_id, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_eula_show(const libresteam_backend_set_t *set);
LDG_EXPORT uint32_t libresteam_wshop_eula_status_get(const libresteam_backend_set_t *set, uint64_t *call_out);
LDG_EXPORT uint32_t libresteam_wshop_user_content_desc_preference_get(const libresteam_backend_set_t *set, uint32_t *desc_id, uint32_t cap, uint32_t *cunt_out);
LDG_EXPORT uint32_t libresteam_wshop_items_disabled_locally_set(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt, uint8_t disabled);
LDG_EXPORT uint32_t libresteam_wshop_subscribed_ld_order_set(const libresteam_backend_set_t *set, const uint64_t *file_id, uint32_t file_id_cunt);
LDG_EXPORT uint32_t libresteam_wshop_item_unused_mark(const libresteam_backend_set_t *set, uint64_t file_id);
LDG_EXPORT uint32_t libresteam_wshop_downloaded_item_cunt_get(const libresteam_backend_set_t *set, uint32_t *out);
LDG_EXPORT uint32_t libresteam_wshop_downloaded_items_get(const libresteam_backend_set_t *set, uint64_t *file_id, uint32_t cap, uint32_t *cunt_out);

#endif
