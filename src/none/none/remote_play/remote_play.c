#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/backend/backend.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/remote_play/ops.h>
#include <libresteam/remote_play/remote_play.h>
#include <libresteam/steam_id/steam_id.h>

static uint32_t remote_play_ops_get(const libresteam_backend_set_t *set, const libresteam_remote_play_ops_t **ops_out, void **ctx_out)
{
    const libresteam_backend_t *primary = 0x0;

    *ops_out = 0x0;
    *ctx_out = 0x0;

    if (LDG_UNLIKELY(!set)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!set->backend || set->primary_idx >= set->backend_cunt)) { return LDG_ERR_NOT_INIT; }

    primary = &set->backend[set->primary_idx];
    if (!primary->ops->remote_play) { return LDG_ERR_UNSUPPORTED; }

    *ops_out = primary->ops->remote_play;
    *ctx_out = primary->ctx;

    return LDG_ERR_AOK;
}

uint32_t libresteam_remote_play_sesh_cunt_get(const libresteam_backend_set_t *set, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sesh_cunt_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->sesh_cunt_get(ctx, out);
}

uint32_t libresteam_remote_play_sesh_id_get(const libresteam_backend_set_t *set, uint32_t idx, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->sesh_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->sesh_id_get(ctx, idx, out);
}

uint32_t libresteam_remote_play_together_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint8_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->together_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->together_get(ctx, sesh_id, out);
}

uint32_t libresteam_remote_play_steam_id_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_steam_id_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->steam_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->steam_id_get(ctx, sesh_id, out);
}

uint32_t libresteam_remote_play_guest_id_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->guest_id_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->guest_id_get(ctx, sesh_id, out);
}

uint32_t libresteam_remote_play_avatar_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_remote_play_avatar_size_t size, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->avatar_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->avatar_get(ctx, sesh_id, size, out);
}

uint32_t libresteam_remote_play_client_name_get(const libresteam_backend_set_t *set, uint32_t sesh_id, const char **out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->client_name_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->client_name_get(ctx, sesh_id, out);
}

uint32_t libresteam_remote_play_form_factor_get(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_remote_play_form_factor_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->form_factor_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->form_factor_get(ctx, sesh_id, out);
}

uint32_t libresteam_remote_play_client_res_get(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t *width_out, uint32_t *height_out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->client_res_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->client_res_get(ctx, sesh_id, width_out, height_out);
}

uint32_t libresteam_remote_play_ui_show(const libresteam_backend_set_t *set)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->ui_show) { return LDG_ERR_UNSUPPORTED; }

    return ops->ui_show(ctx);
}

uint32_t libresteam_remote_play_invite_send(const libresteam_backend_set_t *set, libresteam_steam_id_t friend_id)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->invite_send) { return LDG_ERR_UNSUPPORTED; }

    return ops->invite_send(ctx, friend_id);
}

uint32_t libresteam_remote_play_direct_input_en(const libresteam_backend_set_t *set)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->direct_input_en) { return LDG_ERR_UNSUPPORTED; }

    return ops->direct_input_en(ctx);
}

uint32_t libresteam_remote_play_direct_input_disable(const libresteam_backend_set_t *set)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->direct_input_disable) { return LDG_ERR_UNSUPPORTED; }

    return ops->direct_input_disable(ctx);
}

uint32_t libresteam_remote_play_input_get(const libresteam_backend_set_t *set, libresteam_remote_play_input_t *input, uint32_t cap, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->input_get) { return LDG_ERR_UNSUPPORTED; }

    return ops->input_get(ctx, input, cap, out);
}

uint32_t libresteam_remote_play_mouse_visible_set(const libresteam_backend_set_t *set, uint32_t sesh_id, uint8_t visible)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->mouse_visible_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->mouse_visible_set(ctx, sesh_id, visible);
}

uint32_t libresteam_remote_play_mouse_pos_set(const libresteam_backend_set_t *set, uint32_t sesh_id, libresteam_fp32_t pos_x, libresteam_fp32_t pos_y)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->mouse_pos_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->mouse_pos_set(ctx, sesh_id, pos_x, pos_y);
}

uint32_t libresteam_remote_play_cursor_create(const libresteam_backend_set_t *set, uint32_t width, uint32_t height, uint32_t hot_x, uint32_t hot_y, const void *bgra, uint32_t pitch, uint32_t *out)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->cursor_create) { return LDG_ERR_UNSUPPORTED; }

    return ops->cursor_create(ctx, width, height, hot_x, hot_y, bgra, pitch, out);
}

uint32_t libresteam_remote_play_cursor_set(const libresteam_backend_set_t *set, uint32_t sesh_id, uint32_t cursor_id)
{
    const libresteam_remote_play_ops_t *ops = 0x0;
    void *ctx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = remote_play_ops_get(set, &ops, &ctx);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ops->cursor_set) { return LDG_ERR_UNSUPPORTED; }

    return ops->cursor_set(ctx, sesh_id, cursor_id);
}
