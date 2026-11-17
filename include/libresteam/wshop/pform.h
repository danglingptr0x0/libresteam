#ifndef LIBRESTEAM_WSHOP_PFORM_H
#define LIBRESTEAM_WSHOP_PFORM_H

#include <stdint.h>
#include <dangling/core/macros.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/wshop/types.h>

#define LIBRESTEAM_WSHOP_DETAILS_SIZE 9784U
#define LIBRESTEAM_WSHOP_STR_ARR_SIZE 16U

typedef struct libresteam_wshop_details
{
    uint64_t file_id;
    uint32_t eres;
    uint32_t file_type;
    uint32_t creator_app;
    uint32_t consumer_app;
    char title[LIBRESTEAM_WSHOP_TITLE_MAX];
    char desc[LIBRESTEAM_WSHOP_DESC_MAX];
    uint8_t pudding0[7];
    uint64_t owner;
    uint32_t time_created;
    uint32_t time_updated;
    uint32_t time_added_to_user_list;
    uint32_t visibility;
    uint8_t banned;
    uint8_t accepted_for_use;
    uint8_t tags_truncated;
    char tags[LIBRESTEAM_WSHOP_TAG_LIST_MAX];
    uint8_t pudding1[4];
    uint64_t file;
    uint64_t preview_file;
    char file_name[LIBRESTEAM_WSHOP_FILE_NAME_MAX];
    uint32_t file_size;
    uint32_t preview_file_size;
    char url[LIBRESTEAM_WSHOP_URL_MAX];
    uint32_t votes_up;
    uint32_t votes_down;
    libresteam_fp32_t score;
    uint32_t child_cunt;
    uint8_t pudding2[4];
    uint64_t total_files_size;
} LDG_PACKED libresteam_wshop_details_t;

typedef struct libresteam_wshop_str_arr
{
    const char **str;
    uint32_t cunt;
    uint8_t pudding[4];
} LDG_PACKED libresteam_wshop_str_arr_t;

#endif
