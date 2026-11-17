#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_NETWORKING_VERSION "SteamNetworking006"
#define LIBRESTEAM_SHIM_NETWORKING_METHOD_CUNT 22U
#define LIBRESTEAM_SHIM_NETWORKING_005_VERSION "SteamNetworking005"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_SEND "SendP2PPacket"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_AVAIL "IsP2PPacketAvailable"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_RD "ReadP2PPacket"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_ACCEPT "AcceptP2PSessionWithUser"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_SESH_CLOSE "CloseP2PSessionWithUser"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_CHANNEL_CLOSE "CloseP2PChannelWithUser"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_SESH_STATE "GetP2PSessionState"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_RELAY "AllowP2PPacketRelay"
#define LIBRESTEAM_SHIM_NETWORKING_NAME_CONN_CREATE "CreateP2PConnectionSo" "cket"

static uint32_t libresteam_shim_networking_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_SLOT_FIRST:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_SEND;
          break;
        }
        case LIBRESTEAM_SHIM_SLOT_SECOND:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_AVAIL;
          break;
        }
        case 2:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_RD;
          break;
        }
        case 3:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_ACCEPT;
          break;
        }
        case 4:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_SESH_CLOSE;
          break;
        }
        case 5:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_CHANNEL_CLOSE;
          break;
        }
        case 6:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_SESH_STATE;
          break;
        }
        case 7:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_RELAY;
          break;
        }
        case 8:
        { *out = "CreateListenSocket";
          break;
        }
        case 9:
        { *out = LIBRESTEAM_SHIM_NETWORKING_NAME_CONN_CREATE;
          break;
        }
        case 10:
        { *out = "CreateConnectionSocket";
          break;
        }
        case 11:
        { *out = "DestroySocket";
          break;
        }
        case 12:
        { *out = "DestroyListenSocket";
          break;
        }
        case 13:
        { *out = "SendDataOnSocket";
          break;
        }
        case 14:
        { *out = "IsDataAvailableOnSocket";
          break;
        }
        case 15:
        { *out = "RetrieveDataFromSocket";
          break;
        }
        case 16:
        { *out = "IsDataAvailable";
          break;
        }
        case 17:
        { *out = "RetrieveData";
          break;
        }
        case 18:
        { *out = "GetSocketInfo";
          break;
        }
        case 19:
        { *out = "GetListenSocketInfo";
          break;
        }
        case 20:
        { *out = "GetSocketConnectionType";
          break;
        }
        case 21:
        { *out = "GetMaxPacketSize";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_networking_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_NETWORKING_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_networking_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_VERSION;
    out->name_get = libresteam_shim_networking_name_get;
    out->kind_get = libresteam_shim_networking_kind_get;
    out->impl_get = libresteam_shim_networking_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_METHOD_CUNT;

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_networking_005_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_NETWORKING_005_VERSION;
    out->name_get = libresteam_shim_networking_name_get;
    out->kind_get = libresteam_shim_networking_kind_get;
    out->impl_get = libresteam_shim_networking_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_NETWORKING_METHOD_CUNT;

    return LDG_ERR_AOK;
}
