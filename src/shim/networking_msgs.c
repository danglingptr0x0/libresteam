#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>

#include <libresteam/steam/steam.h>
#include <libresteam/net/types.h>
#include <libresteam/net/msgs/msgs.h>
#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_NETWORKING_MSGS_VERSION "SteamNetworkingMessages002"
#define LIBRESTEAM_SHIM_NETWORKING_MSGS_METHOD_CUNT 6U
#define LIBRESTEAM_SHIM_NETWORKING_MSGS_EFAIL 2U
#define LIBRESTEAM_SHIM_NETWORKING_MSGS_CUNT_INVALID 0xFFFFFFFFU
#define LIBRESTEAM_SHIM_NETWORKING_MSGS_STATE_NONE 0U
#define LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static uint8_t libresteam_shim_networking_msgs_flag(uint32_t res)
{
    if (res == LDG_ERR_AOK) { return LDG_TRUTH_TRUE; }

    return LDG_TRUTH_FALSE;
}

static uint32_t libresteam_shim_networking_msgs_msg_send(void *self, const libresteam_net_ident_t *remote, const void *data, uint32_t size, uint32_t flags, uint32_t remote_channel)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t eres = LIBRESTEAM_SHIM_NETWORKING_MSGS_EFAIL;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_MSG_SEND, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_EFAIL; }

    if (libresteam_net_msgs_msg_send(set, remote, data, size, flags, remote_channel, &eres) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_EFAIL; }

    return eres;
}

static uint32_t libresteam_shim_networking_msgs_channel_msgs_recv(void *self, uint32_t local_channel, libresteam_net_msg_t **msg_out, uint32_t cap)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t cunt = 0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_MSGS_RECV, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_CUNT_INVALID; }

    if (libresteam_net_msgs_channel_msgs_recv(set, local_channel, msg_out, cap, &cunt) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_CUNT_INVALID; }

    return cunt;
}

static uint8_t libresteam_shim_networking_msgs_sesh_accept(void *self, const libresteam_net_ident_t *remote)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_SESH_ACCEPT, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_msgs_flag(libresteam_net_msgs_sesh_accept(set, remote));
}

static uint8_t libresteam_shim_networking_msgs_sesh_close(void *self, const libresteam_net_ident_t *remote)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_SESH_CLOSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_msgs_flag(libresteam_net_msgs_sesh_close(set, remote));
}

static uint8_t libresteam_shim_networking_msgs_channel_close(void *self, const libresteam_net_ident_t *remote, uint32_t local_channel)
{
    const libresteam_backend_set_t *set = 0x0;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_CLOSE, &set) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return libresteam_shim_networking_msgs_flag(libresteam_net_msgs_channel_close(set, remote, local_channel));
}

static uint32_t libresteam_shim_networking_msgs_sesh_conn_info_get(void *self, const libresteam_net_ident_t *remote, libresteam_net_conn_info_t *info, libresteam_net_real_time_status_t *status)
{
    const libresteam_backend_set_t *set = 0x0;
    uint32_t state = LIBRESTEAM_SHIM_NETWORKING_MSGS_STATE_NONE;

    if (libresteam_shim_set_get(self, LIBRESTEAM_STEAM_NET_MSGS_SESH_CONN_INFO_GET, &set) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_STATE_NONE; }

    if (libresteam_net_msgs_sesh_conn_info_get(set, remote, info, status, &state) != LDG_ERR_AOK) { return LIBRESTEAM_SHIM_NETWORKING_MSGS_STATE_NONE; }

    return state;
}

static uint32_t libresteam_shim_networking_msgs_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_MSGS_MSG_SEND:
        { *out = "SendMessageToUser";
          break;
        }
        case LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_MSGS_RECV:
        { *out = "ReceiveMessagesOnChannel";
          break;
        }
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_ACCEPT:
        { *out = "AcceptSessionWithUser";
          break;
        }
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_CLOSE:
        { *out = "CloseSessionWithUser";
          break;
        }
        case LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_CLOSE:
        { *out = "CloseChannelWithUser";
          break;
        }
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_CONN_INFO_GET:
        { *out = "GetSessionConnectionInfo";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_msgs_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_MSGS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_msgs_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_STEAM_NET_MSGS_MSG_SEND:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_msg_send)
        case LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_MSGS_RECV:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_channel_msgs_recv)
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_ACCEPT:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_sesh_accept)
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_CLOSE:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_sesh_close)
        case LIBRESTEAM_STEAM_NET_MSGS_CHANNEL_CLOSE:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_channel_close)
        case LIBRESTEAM_STEAM_NET_MSGS_SESH_CONN_INFO_GET:
            LIBRESTEAM_SHIM_NETWORKING_MSGS_IMPL(libresteam_shim_networking_msgs_sesh_conn_info_get)
        default: { return LDG_ERR_RANGE;
        }
    }
}

uint32_t libresteam_shim_networking_msgs_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_MSGS_VERSION;
    out->name_get = libresteam_shim_networking_msgs_name_get;
    out->kind_get = libresteam_shim_networking_msgs_kind_get;
    out->impl_get = libresteam_shim_networking_msgs_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_MSGS_METHOD_CUNT;

    return LDG_ERR_AOK;
}
