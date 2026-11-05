#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/cpp.h>
#include <dangling/mem/secure.h>

#include <libresteam/backend/steam/steam.h>
#include <libresteam/fp32/fp32.h>
#include <libresteam/srv_browser/ops.h>

#define STEAM_SRV_BROWSER_SIGN_BIT 0x80000000U
#define STEAM_SRV_BROWSER_ARG_NONE 0U
#define STEAM_SRV_BROWSER_IFACE_SRVS 0U
#define STEAM_SRV_BROWSER_IFACE_MATCHMAKING 1U
#define STEAM_SRV_BROWSER_LIST_VFUNC_RESPONDED 0U
#define STEAM_SRV_BROWSER_LIST_VFUNC_FAILED 1U
#define STEAM_SRV_BROWSER_LIST_VFUNC_COMPLETE 2U
#define STEAM_SRV_BROWSER_PING_VFUNC_RESPONDED 0U
#define STEAM_SRV_BROWSER_PING_VFUNC_FAILED 1U
#define STEAM_SRV_BROWSER_PLAYER_VFUNC_ADD 0U
#define STEAM_SRV_BROWSER_PLAYER_VFUNC_FAILED 1U
#define STEAM_SRV_BROWSER_PLAYER_VFUNC_COMPLETE 2U
#define STEAM_SRV_BROWSER_RULE_VFUNC_RESPONDED 0U
#define STEAM_SRV_BROWSER_RULE_VFUNC_FAILED 1U
#define STEAM_SRV_BROWSER_RULE_VFUNC_COMPLETE 2U
#define STEAM_SRV_BROWSER_VFUNC_SIZE ((uint64_t)sizeof(libresteam_backend_steam_vfunc_t))

typedef void (*steam_srv_browser_list_cb_t)(void *self, void *req, uint32_t val);
typedef void (*steam_srv_browser_ping_responded_cb_t)(void *self, const libresteam_srv_browser_item_t *item);
typedef void (*steam_srv_browser_self_cb_t)(void *self);
typedef void (*steam_srv_browser_player_add_cb_t)(void *self, const char *name, uint32_t score, libresteam_fp32_t time_played);
typedef void (*steam_srv_browser_rule_responded_cb_t)(void *self, const char *rule, const char *val);

static void steam_srv_browser_list_responded_cb(void *self, void *req, uint32_t srv_idx)
{
    libresteam_backend_steam_srv_browser_list_slot_t *slot = (libresteam_backend_steam_srv_browser_list_slot_t *)self;

    if (LDG_UNLIKELY(!slot || !slot->resp.ops || !slot->resp.ops->srv_responded_cb)) { return; }

    slot->resp.ops->srv_responded_cb(slot->resp.content, req, srv_idx);
}

static void steam_srv_browser_list_failed_cb(void *self, void *req, uint32_t srv_idx)
{
    libresteam_backend_steam_srv_browser_list_slot_t *slot = (libresteam_backend_steam_srv_browser_list_slot_t *)self;

    if (LDG_UNLIKELY(!slot || !slot->resp.ops || !slot->resp.ops->srv_failed_cb)) { return; }

    slot->resp.ops->srv_failed_cb(slot->resp.content, req, srv_idx);
}

static void steam_srv_browser_list_complete_cb(void *self, void *req, uint32_t resp)
{
    libresteam_backend_steam_srv_browser_list_slot_t *slot = (libresteam_backend_steam_srv_browser_list_slot_t *)self;
    libresteam_srv_browser_resp_t kind = LIBRESTEAM_SRV_BROWSER_RESP_FAILED_TO_RESPOND;

    if (LDG_UNLIKELY(!slot || !slot->resp.ops || !slot->resp.ops->refresh_complete_cb)) { return; }

    if (resp < (uint32_t)LIBRESTEAM_SRV_BROWSER_RESP_MAX) { kind = (libresteam_srv_browser_resp_t)resp; }

    slot->resp.ops->refresh_complete_cb(slot->resp.content, req, kind);
}

static void steam_srv_browser_ping_responded_cb(void *self, const libresteam_srv_browser_item_t *item)
{
    libresteam_backend_steam_srv_browser_ping_slot_t *slot = (libresteam_backend_steam_srv_browser_ping_slot_t *)self;
    libresteam_srv_browser_ping_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->srv_responded_cb)) { return; }

    resp.ops->srv_responded_cb(resp.content, item);
}

static void steam_srv_browser_ping_failed_cb(void *self)
{
    libresteam_backend_steam_srv_browser_ping_slot_t *slot = (libresteam_backend_steam_srv_browser_ping_slot_t *)self;
    libresteam_srv_browser_ping_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->srv_failed_cb)) { return; }

    resp.ops->srv_failed_cb(resp.content);
}

static void steam_srv_browser_player_add_cb(void *self, const char *name, uint32_t score, libresteam_fp32_t time_played)
{
    libresteam_backend_steam_srv_browser_player_slot_t *slot = (libresteam_backend_steam_srv_browser_player_slot_t *)self;

    if (LDG_UNLIKELY(!slot || !slot->resp.ops || !slot->resp.ops->player_add_cb)) { return; }

    slot->resp.ops->player_add_cb(slot->resp.content, name, score, (double)time_played);
}

static void steam_srv_browser_player_failed_cb(void *self)
{
    libresteam_backend_steam_srv_browser_player_slot_t *slot = (libresteam_backend_steam_srv_browser_player_slot_t *)self;
    libresteam_srv_browser_player_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->player_failed_cb)) { return; }

    resp.ops->player_failed_cb(resp.content);
}

static void steam_srv_browser_player_complete_cb(void *self)
{
    libresteam_backend_steam_srv_browser_player_slot_t *slot = (libresteam_backend_steam_srv_browser_player_slot_t *)self;
    libresteam_srv_browser_player_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->player_refresh_complete_cb)) { return; }

    resp.ops->player_refresh_complete_cb(resp.content);
}

static void steam_srv_browser_rule_responded_cb(void *self, const char *rule, const char *val)
{
    libresteam_backend_steam_srv_browser_rule_slot_t *slot = (libresteam_backend_steam_srv_browser_rule_slot_t *)self;

    if (LDG_UNLIKELY(!slot || !slot->resp.ops || !slot->resp.ops->rule_responded_cb)) { return; }

    slot->resp.ops->rule_responded_cb(slot->resp.content, rule, val);
}

static void steam_srv_browser_rule_failed_cb(void *self)
{
    libresteam_backend_steam_srv_browser_rule_slot_t *slot = (libresteam_backend_steam_srv_browser_rule_slot_t *)self;
    libresteam_srv_browser_rule_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->rule_failed_cb)) { return; }

    resp.ops->rule_failed_cb(resp.content);
}

static void steam_srv_browser_rule_complete_cb(void *self)
{
    libresteam_backend_steam_srv_browser_rule_slot_t *slot = (libresteam_backend_steam_srv_browser_rule_slot_t *)self;
    libresteam_srv_browser_rule_resp_t resp = LDG_STRUCT_ZERO_INIT;

    if (LDG_UNLIKELY(!slot)) { return; }

    resp = slot->resp;
    slot->in_use = LDG_TRUTH_FALSE;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!resp.ops || !resp.ops->rule_refresh_complete_cb)) { return; }

    resp.ops->rule_refresh_complete_cb(resp.content);
}

static uint32_t steam_srv_browser_tgt_get(void *ctx, uint32_t iface, uint32_t method_idx, void **obj_out, uint32_t *slot_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;

    *obj_out = 0x0;
    *slot_out = 0;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (iface == STEAM_SRV_BROWSER_IFACE_MATCHMAKING)
    {
        if (LDG_UNLIKELY(!steam->matchmaking)) { return LDG_ERR_NOT_INIT; }

        *obj_out = steam->matchmaking;
        return ldg_cpp_slot_get(&steam->matchmaking_desc, method_idx, slot_out);
    }

    if (LDG_UNLIKELY(!steam->matchmaking_srvs)) { return LDG_ERR_NOT_INIT; }

    *obj_out = steam->matchmaking_srvs;

    return ldg_cpp_slot_get(&steam->matchmaking_srvs_desc, method_idx, slot_out);
}

static uint32_t steam_srv_browser_void_call(void *ctx, uint32_t iface, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    res = steam_srv_browser_tgt_get(ctx, iface, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_void_call(obj, slot, arg, arg_cunt);
}

static uint32_t steam_srv_browser_flag_call(void *ctx, uint32_t iface, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint8_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t flag = LDG_TRUTH_FALSE;

    *out = LDG_TRUTH_FALSE;

    res = steam_srv_browser_tgt_get(ctx, iface, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_cpp_uint8_call(obj, slot, arg, arg_cunt, &flag);
    if (res != LDG_ERR_AOK) { return res; }

    if (flag != LDG_TRUTH_FALSE) { *out = LDG_TRUTH_TRUE; }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_u32_call(void *ctx, uint32_t iface, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint32_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_srv_browser_tgt_get(ctx, iface, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint32_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_srv_browser_u64_call(void *ctx, uint32_t iface, uint32_t method_idx, const ldg_cpp_arg_t *arg, uint32_t arg_cunt, uint64_t *out)
{
    void *obj = 0x0;
    uint32_t slot = 0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0;

    res = steam_srv_browser_tgt_get(ctx, iface, method_idx, &obj, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_cpp_uint64_call(obj, slot, arg, arg_cunt, out);
}

static uint32_t steam_srv_browser_list_slot_alloc(libresteam_backend_steam_t *steam, libresteam_backend_steam_srv_browser_list_slot_t **out)
{
    uint32_t idx = 0;

    *out = 0x0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_LIST_MAX; idx++) { if (steam->srv_browser_list_slot[idx].in_use == LDG_TRUTH_FALSE)
        {
            *out = &steam->srv_browser_list_slot[idx];
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_FULL;
}

static uint32_t steam_srv_browser_ping_slot_alloc(libresteam_backend_steam_t *steam, libresteam_backend_steam_srv_browser_ping_slot_t **out)
{
    uint32_t idx = 0;

    *out = 0x0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_QUERY_MAX; idx++) { if (steam->srv_browser_ping_slot[idx].in_use == LDG_TRUTH_FALSE)
        {
            *out = &steam->srv_browser_ping_slot[idx];
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_FULL;
}

static uint32_t steam_srv_browser_player_slot_alloc(libresteam_backend_steam_t *steam, libresteam_backend_steam_srv_browser_player_slot_t **out)
{
    uint32_t idx = 0;

    *out = 0x0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_QUERY_MAX; idx++) { if (steam->srv_browser_player_slot[idx].in_use == LDG_TRUTH_FALSE)
        {
            *out = &steam->srv_browser_player_slot[idx];
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_FULL;
}

static uint32_t steam_srv_browser_rule_slot_alloc(libresteam_backend_steam_t *steam, libresteam_backend_steam_srv_browser_rule_slot_t **out)
{
    uint32_t idx = 0;

    *out = 0x0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_QUERY_MAX; idx++) { if (steam->srv_browser_rule_slot[idx].in_use == LDG_TRUTH_FALSE)
        {
            *out = &steam->srv_browser_rule_slot[idx];
            return LDG_ERR_AOK;
        }
    }

    return LDG_ERR_FULL;
}

static void steam_srv_browser_query_slot_free(libresteam_backend_steam_t *steam, uint32_t query)
{
    uint32_t idx = 0;

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_QUERY_MAX; idx++)
    {
        if (steam->srv_browser_ping_slot[idx].in_use == LDG_TRUTH_TRUE && steam->srv_browser_ping_slot[idx].query == query)
        {
            steam->srv_browser_ping_slot[idx].in_use = LDG_TRUTH_FALSE;
            steam->srv_browser_ping_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        }

        if (steam->srv_browser_player_slot[idx].in_use == LDG_TRUTH_TRUE && steam->srv_browser_player_slot[idx].query == query)
        {
            steam->srv_browser_player_slot[idx].in_use = LDG_TRUTH_FALSE;
            steam->srv_browser_player_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        }

        if (steam->srv_browser_rule_slot[idx].in_use == LDG_TRUTH_TRUE && steam->srv_browser_rule_slot[idx].query == query)
        {
            steam->srv_browser_rule_slot[idx].in_use = LDG_TRUTH_FALSE;
            steam->srv_browser_rule_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        }
    }
}

static uint32_t steam_srv_browser_list_req(void *ctx, libresteam_srv_browser_list_type_t type, uint32_t app, const libresteam_srv_browser_kv_t *const *filter, uint32_t filter_cunt, const libresteam_srv_browser_list_resp_t *resp, void **req_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    libresteam_backend_steam_srv_browser_list_slot_t *slot = 0x0;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t req = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!req_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *req_out = 0x0;

    if (LDG_UNLIKELY(!steam || !resp)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(type >= LIBRESTEAM_SRV_BROWSER_LIST_TYPE_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (LDG_UNLIKELY(!steam->matchmaking_srvs)) { return LDG_ERR_NOT_INIT; }

    res = steam_srv_browser_list_slot_alloc(steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    slot->resp = *resp;
    slot->req = 0x0;
    slot->in_use = LDG_TRUTH_TRUE;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    if (type != LIBRESTEAM_SRV_BROWSER_LIST_TYPE_LAN)
    {
        arg[cunt] = (ldg_cpp_arg_t) { .ptr = filter, .kind = LDG_CPP_ARG_KIND_PTR };
        cunt++;
        arg[cunt] = (ldg_cpp_arg_t) { .val = filter_cunt, .kind = LDG_CPP_ARG_KIND_INT };
        cunt++;
    }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = slot, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_srv_browser_u64_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_INTERNET_REQ + (uint32_t)type, arg, cunt, &req);
    if (res != LDG_ERR_AOK || req == 0)
    {
        slot->in_use = LDG_TRUTH_FALSE;
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_INVALID;
    }

    slot->req = (void *)(uintptr_t)req;
    *req_out = slot->req;

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_req_release(void *ctx, void *req)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t idx = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam || !req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_srv_browser_void_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_REQ_RELEASE, arg, cunt);
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_LIST_MAX; idx++) { if (steam->srv_browser_list_slot[idx].in_use == LDG_TRUTH_TRUE && steam->srv_browser_list_slot[idx].req == req)
        {
            steam->srv_browser_list_slot[idx].in_use = LDG_TRUTH_FALSE;
            steam->srv_browser_list_slot[idx].req = 0x0;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_srv_details_get(void *ctx, void *req, uint32_t srv_idx, const libresteam_srv_browser_item_t **out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint64_t item = 0;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = srv_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_srv_browser_u64_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_SRV_DETAILS_GET, arg, cunt, &item);
    if (res != LDG_ERR_AOK) { return res; }

    if (item == 0) { return LDG_ERR_NOT_FOUND; }

    *out = (const libresteam_srv_browser_item_t *)(uintptr_t)item;

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_query_cancel(void *ctx, void *req)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_srv_browser_void_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_QUERY_CANCEL, arg, cunt);
}

static uint32_t steam_srv_browser_query_refresh(void *ctx, void *req)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_srv_browser_void_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_QUERY_REFRESH, arg, cunt);
}

static uint32_t steam_srv_browser_refreshing_get(void *ctx, void *req, uint8_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    return steam_srv_browser_flag_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_REFRESHING_GET, arg, cunt, out);
}

static uint32_t steam_srv_browser_srv_cunt_get(void *ctx, void *req, uint32_t *out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_srv_browser_u32_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_SRV_CUNT_GET, arg, cunt, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_SRV_BROWSER_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_srv_refresh(void *ctx, void *req, uint32_t srv_idx)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!req)) { return LDG_ERR_FUNC_ARG_NULL; }

    arg[cunt] = (ldg_cpp_arg_t) { .ptr = req, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = srv_idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_srv_browser_void_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_SRV_REFRESH, arg, cunt);
}

static uint32_t steam_srv_browser_srv_query_start(void *ctx, uint32_t method_idx, uint32_t ip, uint16_t port, void *adapter, uint32_t *query_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = adapter, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_srv_browser_u32_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, method_idx, arg, cunt, query_out);
    if (res != LDG_ERR_AOK)
    {
        *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        return res;
    }

    if (*query_out == LIBRESTEAM_SRV_BROWSER_QUERY_INVALID) { return LDG_ERR_INVALID; }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_ping_req(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_ping_resp_t *resp, uint32_t *query_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    libresteam_backend_steam_srv_browser_ping_slot_t *slot = 0x0;
    uint32_t query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!steam || !resp)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->matchmaking_srvs)) { return LDG_ERR_NOT_INIT; }

    res = steam_srv_browser_ping_slot_alloc(steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    slot->resp = *resp;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    slot->in_use = LDG_TRUTH_TRUE;

    res = steam_srv_browser_srv_query_start(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_PING_REQ, ip, port, slot, &query);
    if (res != LDG_ERR_AOK)
    {
        slot->in_use = LDG_TRUTH_FALSE;
        return res;
    }

    if (slot->in_use == LDG_TRUTH_TRUE) { slot->query = query; }

    *query_out = query;

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_player_details_req(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_player_resp_t *resp, uint32_t *query_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    libresteam_backend_steam_srv_browser_player_slot_t *slot = 0x0;
    uint32_t query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!steam || !resp)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->matchmaking_srvs)) { return LDG_ERR_NOT_INIT; }

    res = steam_srv_browser_player_slot_alloc(steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    slot->resp = *resp;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    slot->in_use = LDG_TRUTH_TRUE;

    res = steam_srv_browser_srv_query_start(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_PLAYER_DETAILS_REQ, ip, port, slot, &query);
    if (res != LDG_ERR_AOK)
    {
        slot->in_use = LDG_TRUTH_FALSE;
        return res;
    }

    if (slot->in_use == LDG_TRUTH_TRUE) { slot->query = query; }

    *query_out = query;

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_rule_req(void *ctx, uint32_t ip, uint16_t port, const libresteam_srv_browser_rule_resp_t *resp, uint32_t *query_out)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    libresteam_backend_steam_srv_browser_rule_slot_t *slot = 0x0;
    uint32_t query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!query_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *query_out = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;

    if (LDG_UNLIKELY(!steam || !resp)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!steam->matchmaking_srvs)) { return LDG_ERR_NOT_INIT; }

    res = steam_srv_browser_rule_slot_alloc(steam, &slot);
    if (res != LDG_ERR_AOK) { return res; }

    slot->resp = *resp;
    slot->query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
    slot->in_use = LDG_TRUTH_TRUE;

    res = steam_srv_browser_srv_query_start(ctx, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_RULE_REQ, ip, port, slot, &query);
    if (res != LDG_ERR_AOK)
    {
        slot->in_use = LDG_TRUTH_FALSE;
        return res;
    }

    if (slot->in_use == LDG_TRUTH_TRUE) { slot->query = query; }

    *query_out = query;

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_srv_query_cancel(void *ctx, uint32_t query)
{
    libresteam_backend_steam_t *steam = (libresteam_backend_steam_t *)ctx;
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(query == LIBRESTEAM_SRV_BROWSER_QUERY_INVALID)) { return LDG_ERR_FUNC_ARG_INVALID; }

    arg[cunt] = (ldg_cpp_arg_t) { .val = query, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_srv_browser_void_call(ctx, STEAM_SRV_BROWSER_IFACE_SRVS, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_SRVS_SRV_QUERY_CANCEL, arg, cunt);
    if (res != LDG_ERR_AOK) { return res; }

    steam_srv_browser_query_slot_free(steam, query);

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_favorite_cunt_get(void *ctx, uint32_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;

    res = steam_srv_browser_u32_call(ctx, STEAM_SRV_BROWSER_IFACE_MATCHMAKING, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_CUNT_GET, 0x0, STEAM_SRV_BROWSER_ARG_NONE, out);
    if (res != LDG_ERR_AOK) { return res; }

    if ((*out & STEAM_SRV_BROWSER_SIGN_BIT) != 0)
    {
        *out = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_favorite_get(void *ctx, uint32_t idx, uint32_t *app_out, uint32_t *ip_out, uint16_t *conn_port_out, uint16_t *query_port_out, uint32_t *flags_out, uint32_t *time_last_played_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t found = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!app_out || !ip_out || !conn_port_out || !query_port_out || !flags_out || !time_last_played_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *app_out = 0;
    *ip_out = 0;
    *conn_port_out = 0;
    *query_port_out = 0;
    *flags_out = 0;
    *time_last_played_out = 0;

    arg[cunt] = (ldg_cpp_arg_t) { .val = idx, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = app_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = ip_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = conn_port_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = query_port_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = flags_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .ptr = time_last_played_out, .kind = LDG_CPP_ARG_KIND_PTR };
    cunt++;

    res = steam_srv_browser_flag_call(ctx, STEAM_SRV_BROWSER_IFACE_MATCHMAKING, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_GET, arg, cunt, &found);
    if (res != LDG_ERR_AOK) { return res; }

    if (found != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_favorite_add(void *ctx, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint32_t time_last_played, uint32_t *idx_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!idx_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *idx_out = UINT32_MAX;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = conn_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = query_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = time_last_played, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    res = steam_srv_browser_u32_call(ctx, STEAM_SRV_BROWSER_IFACE_MATCHMAKING, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_ADD, arg, cunt, idx_out);
    if (res != LDG_ERR_AOK)
    {
        *idx_out = UINT32_MAX;
        return res;
    }

    if ((*idx_out & STEAM_SRV_BROWSER_SIGN_BIT) != 0)
    {
        *idx_out = UINT32_MAX;
        return LDG_ERR_INVALID;
    }

    return LDG_ERR_AOK;
}

static uint32_t steam_srv_browser_favorite_remove(void *ctx, uint32_t app, uint32_t ip, uint16_t conn_port, uint16_t query_port, uint32_t flags, uint8_t *removed_out)
{
    ldg_cpp_arg_t arg[LDG_CPP_ARG_MAX] = LDG_ARR_ZERO_INIT;
    uint32_t cunt = 0;

    if (LDG_UNLIKELY(!removed_out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *removed_out = LDG_TRUTH_FALSE;

    arg[cunt] = (ldg_cpp_arg_t) { .val = app, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = ip, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = conn_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = query_port, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;
    arg[cunt] = (ldg_cpp_arg_t) { .val = flags, .kind = LDG_CPP_ARG_KIND_INT };
    cunt++;

    return steam_srv_browser_flag_call(ctx, STEAM_SRV_BROWSER_IFACE_MATCHMAKING, LIBRESTEAM_BACKEND_STEAM_MATCHMAKING_FAVORITE_REMOVE, arg, cunt, removed_out);
}

static uint32_t steam_srv_browser_vtbl_fill(libresteam_backend_steam_t *steam)
{
    steam_srv_browser_list_cb_t list_cb = 0x0;
    steam_srv_browser_ping_responded_cb_t ping_responded_cb = 0x0;
    steam_srv_browser_self_cb_t self_cb = 0x0;
    steam_srv_browser_player_add_cb_t player_add_cb = 0x0;
    steam_srv_browser_rule_responded_cb_t rule_responded_cb = 0x0;
    uint32_t idx = 0;
    uint32_t res = LDG_ERR_AOK;

    list_cb = steam_srv_browser_list_responded_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_list_vtbl[STEAM_SRV_BROWSER_LIST_VFUNC_RESPONDED], &list_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    list_cb = steam_srv_browser_list_failed_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_list_vtbl[STEAM_SRV_BROWSER_LIST_VFUNC_FAILED], &list_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    list_cb = steam_srv_browser_list_complete_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_list_vtbl[STEAM_SRV_BROWSER_LIST_VFUNC_COMPLETE], &list_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    ping_responded_cb = steam_srv_browser_ping_responded_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_ping_vtbl[STEAM_SRV_BROWSER_PING_VFUNC_RESPONDED], &ping_responded_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_srv_browser_ping_failed_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_ping_vtbl[STEAM_SRV_BROWSER_PING_VFUNC_FAILED], &self_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    player_add_cb = steam_srv_browser_player_add_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_player_vtbl[STEAM_SRV_BROWSER_PLAYER_VFUNC_ADD], &player_add_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_srv_browser_player_failed_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_player_vtbl[STEAM_SRV_BROWSER_PLAYER_VFUNC_FAILED], &self_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_srv_browser_player_complete_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_player_vtbl[STEAM_SRV_BROWSER_PLAYER_VFUNC_COMPLETE], &self_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    rule_responded_cb = steam_srv_browser_rule_responded_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_rule_vtbl[STEAM_SRV_BROWSER_RULE_VFUNC_RESPONDED], &rule_responded_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_srv_browser_rule_failed_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_rule_vtbl[STEAM_SRV_BROWSER_RULE_VFUNC_FAILED], &self_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    self_cb = steam_srv_browser_rule_complete_cb;
    res = ldg_mem_secure_cpy(&steam->srv_browser_rule_vtbl[STEAM_SRV_BROWSER_RULE_VFUNC_COMPLETE], &self_cb, STEAM_SRV_BROWSER_VFUNC_SIZE);
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_LIST_MAX; idx++)
    {
        steam->srv_browser_list_slot[idx].vtbl = steam->srv_browser_list_vtbl;
        steam->srv_browser_list_slot[idx].req = 0x0;
        steam->srv_browser_list_slot[idx].in_use = LDG_TRUTH_FALSE;
    }

    for (idx = 0; idx < LIBRESTEAM_BACKEND_STEAM_SRV_BROWSER_QUERY_MAX; idx++)
    {
        steam->srv_browser_ping_slot[idx].vtbl = steam->srv_browser_ping_vtbl;
        steam->srv_browser_ping_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        steam->srv_browser_ping_slot[idx].in_use = LDG_TRUTH_FALSE;
        steam->srv_browser_player_slot[idx].vtbl = steam->srv_browser_player_vtbl;
        steam->srv_browser_player_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        steam->srv_browser_player_slot[idx].in_use = LDG_TRUTH_FALSE;
        steam->srv_browser_rule_slot[idx].vtbl = steam->srv_browser_rule_vtbl;
        steam->srv_browser_rule_slot[idx].query = LIBRESTEAM_SRV_BROWSER_QUERY_INVALID;
        steam->srv_browser_rule_slot[idx].in_use = LDG_TRUTH_FALSE;
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_backend_steam_srv_browser_ops_fill(libresteam_backend_steam_t *steam)
{
    libresteam_srv_browser_ops_t *ops = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!steam)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = steam_srv_browser_vtbl_fill(steam);
    if (res != LDG_ERR_AOK) { return res; }

    ops = &steam->srv_browser_ops;
    ops->list_req = steam_srv_browser_list_req;
    ops->req_release = steam_srv_browser_req_release;
    ops->srv_details_get = steam_srv_browser_srv_details_get;
    ops->query_cancel = steam_srv_browser_query_cancel;
    ops->query_refresh = steam_srv_browser_query_refresh;
    ops->refreshing_get = steam_srv_browser_refreshing_get;
    ops->srv_cunt_get = steam_srv_browser_srv_cunt_get;
    ops->srv_refresh = steam_srv_browser_srv_refresh;
    ops->ping_req = steam_srv_browser_ping_req;
    ops->player_details_req = steam_srv_browser_player_details_req;
    ops->rule_req = steam_srv_browser_rule_req;
    ops->srv_query_cancel = steam_srv_browser_srv_query_cancel;
    ops->favorite_cunt_get = steam_srv_browser_favorite_cunt_get;
    ops->favorite_get = steam_srv_browser_favorite_get;
    ops->favorite_add = steam_srv_browser_favorite_add;
    ops->favorite_remove = steam_srv_browser_favorite_remove;
    steam->ops.srv_browser = &steam->srv_browser_ops;

    return LDG_ERR_AOK;
}
