#ifndef LIBRESTEAM_REMOTE_PLAY_OPS_H
#define LIBRESTEAM_REMOTE_PLAY_OPS_H

#include <stdint.h>

#include <libresteam/fp32/fp32.h>
#include <libresteam/steam_id/steam_id.h>

#define LIBRESTEAM_REMOTE_PLAY_INPUT_SIZE 64U
#define LIBRESTEAM_REMOTE_PLAY_INPUT_EV_SIZE 56U
#define LIBRESTEAM_REMOTE_PLAY_CB_BASE 5700U
#define LIBRESTEAM_REMOTE_PLAY_CB_SESH_CONNECTED (LIBRESTEAM_REMOTE_PLAY_CB_BASE + 1U)
#define LIBRESTEAM_REMOTE_PLAY_CB_SESH_DISCONNECTED (LIBRESTEAM_REMOTE_PLAY_CB_BASE + 2U)
#define LIBRESTEAM_REMOTE_PLAY_CB_GUEST_INVITE (LIBRESTEAM_REMOTE_PLAY_CB_BASE + 3U)
#define LIBRESTEAM_REMOTE_PLAY_CB_AVATAR_READY (LIBRESTEAM_REMOTE_PLAY_CB_BASE + 4U)
#define LIBRESTEAM_REMOTE_PLAY_CONNECT_URL_CAP 1024U

typedef enum libresteam_remote_play_form_factor
{
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_UNKNOWN = 0,
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_PHONE,
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_TABLET,
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_COMPUTER,
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_TV,
    LIBRESTEAM_REMOTE_PLAY_FORM_FACTOR_VR_HEADSET
} libresteam_remote_play_form_factor_t;

typedef enum libresteam_remote_play_avatar_size
{
    LIBRESTEAM_REMOTE_PLAY_AVATAR_SMALL = 0,
    LIBRESTEAM_REMOTE_PLAY_AVATAR_MEDIUM,
    LIBRESTEAM_REMOTE_PLAY_AVATAR_LARGE
} libresteam_remote_play_avatar_size_t;

typedef enum libresteam_remote_play_input_type
{
    LIBRESTEAM_REMOTE_PLAY_INPUT_UNKNOWN = 0,
    LIBRESTEAM_REMOTE_PLAY_INPUT_MOUSE_MOTION,
    LIBRESTEAM_REMOTE_PLAY_INPUT_MOUSE_BUTTON_DOWN,
    LIBRESTEAM_REMOTE_PLAY_INPUT_MOUSE_BUTTON_UP,
    LIBRESTEAM_REMOTE_PLAY_INPUT_MOUSE_WHEEL,
    LIBRESTEAM_REMOTE_PLAY_INPUT_KEY_DOWN,
    LIBRESTEAM_REMOTE_PLAY_INPUT_KEY_UP
} libresteam_remote_play_input_type_t;

typedef struct libresteam_remote_play_mouse_motion
{
    uint8_t absolute;
    uint8_t pudding[3];
    libresteam_fp32_t pos_x;
    libresteam_fp32_t pos_y;
    uint32_t delta_x;
    uint32_t delta_y;
} libresteam_remote_play_mouse_motion_t;

typedef struct libresteam_remote_play_mouse_wheel
{
    uint32_t dir;
    libresteam_fp32_t amount;
} libresteam_remote_play_mouse_wheel_t;

typedef struct libresteam_remote_play_key
{
    uint32_t scancode;
    uint32_t modifiers;
    uint32_t keycode;
} libresteam_remote_play_key_t;

typedef union libresteam_remote_play_input_ev
{
    libresteam_remote_play_mouse_motion_t motion;
    uint32_t mouse_button;
    libresteam_remote_play_mouse_wheel_t wheel;
    libresteam_remote_play_key_t key;
    uint8_t pudding[LIBRESTEAM_REMOTE_PLAY_INPUT_EV_SIZE];
} libresteam_remote_play_input_ev_t;

typedef struct libresteam_remote_play_input
{
    uint32_t sesh_id;
    uint32_t type;
    libresteam_remote_play_input_ev_t ev;
} libresteam_remote_play_input_t;

typedef struct libresteam_remote_play_sesh_cb
{
    uint32_t sesh_id;
} libresteam_remote_play_sesh_cb_t;

typedef struct libresteam_remote_play_guest_invite_cb
{
    char connect_url[LIBRESTEAM_REMOTE_PLAY_CONNECT_URL_CAP];
} libresteam_remote_play_guest_invite_cb_t;

typedef struct libresteam_remote_play_avatar_ready_cb
{
    uint32_t sesh_id;
    uint32_t img;
    uint32_t wide;
    uint32_t tall;
} libresteam_remote_play_avatar_ready_cb_t;

typedef struct libresteam_remote_play_ops
{
    uint32_t (*sesh_cunt_get)(void *ctx, uint32_t *out);
    uint32_t (*sesh_id_get)(void *ctx, uint32_t idx, uint32_t *out);
    uint32_t (*together_get)(void *ctx, uint32_t sesh_id, uint8_t *out);
    uint32_t (*steam_id_get)(void *ctx, uint32_t sesh_id, libresteam_steam_id_t *out);
    uint32_t (*guest_id_get)(void *ctx, uint32_t sesh_id, uint32_t *out);
    uint32_t (*avatar_get)(void *ctx, uint32_t sesh_id, libresteam_remote_play_avatar_size_t size, uint32_t *out);
    uint32_t (*client_name_get)(void *ctx, uint32_t sesh_id, const char **out);
    uint32_t (*form_factor_get)(void *ctx, uint32_t sesh_id, libresteam_remote_play_form_factor_t *out);
    uint32_t (*client_res_get)(void *ctx, uint32_t sesh_id, uint32_t *width_out, uint32_t *height_out);
    uint32_t (*ui_show)(void *ctx);
    uint32_t (*invite_send)(void *ctx, libresteam_steam_id_t friend_id);
    uint32_t (*direct_input_en)(void *ctx);
    uint32_t (*direct_input_disable)(void *ctx);
    uint32_t (*input_get)(void *ctx, libresteam_remote_play_input_t *input, uint32_t cap, uint32_t *out);
    uint32_t (*mouse_visible_set)(void *ctx, uint32_t sesh_id, uint8_t visible);
    uint32_t (*mouse_pos_set)(void *ctx, uint32_t sesh_id, libresteam_fp32_t pos_x, libresteam_fp32_t pos_y);
    uint32_t (*cursor_create)(void *ctx, uint32_t width, uint32_t height, uint32_t hot_x, uint32_t hot_y, const void *bgra, uint32_t pitch, uint32_t *out);
    uint32_t (*cursor_set)(void *ctx, uint32_t sesh_id, uint32_t cursor_id);
} libresteam_remote_play_ops_t;

#endif
