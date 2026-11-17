#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_MATCHMAKING_SRVS_VERSION "SteamMatchMakingServers002"
#define LIBRESTEAM_SHIM_MATCHMAKING_SRVS_METHOD_CUNT 17U

static uint32_t libresteam_shim_matchmaking_srvs_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_SLOT_FIRST:
        { *out = "RequestInternetServerList";
          break;
        }
        case LIBRESTEAM_SHIM_SLOT_SECOND:
        { *out = "RequestLANServerList";
          break;
        }
        case 2:
        { *out = "RequestFriendsServerList";
          break;
        }
        case 3:
        { *out = "RequestFavoritesServerList";
          break;
        }
        case 4:
        { *out = "RequestHistoryServerList";
          break;
        }
        case 5:
        { *out = "RequestSpectatorServerList";
          break;
        }
        case 6:
        { *out = "ReleaseRequest";
          break;
        }
        case 7:
        { *out = "GetServerDetails";
          break;
        }
        case 8:
        { *out = "CancelQuery";
          break;
        }
        case 9:
        { *out = "RefreshQuery";
          break;
        }
        case 10:
        { *out = "IsRefreshing";
          break;
        }
        case 11:
        { *out = "GetServerCount";
          break;
        }
        case 12:
        { *out = "RefreshServer";
          break;
        }
        case 13:
        { *out = "PingServer";
          break;
        }
        case 14:
        { *out = "PlayerDetails";
          break;
        }
        case 15:
        { *out = "ServerRules";
          break;
        }
        case 16:
        { *out = "CancelServerQuery";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_matchmaking_srvs_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_MATCHMAKING_SRVS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_matchmaking_srvs_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_MATCHMAKING_SRVS_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_matchmaking_srvs_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_MATCHMAKING_SRVS_VERSION;
    out->name_get = libresteam_shim_matchmaking_srvs_name_get;
    out->kind_get = libresteam_shim_matchmaking_srvs_kind_get;
    out->impl_get = libresteam_shim_matchmaking_srvs_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_MATCHMAKING_SRVS_METHOD_CUNT;

    return LDG_ERR_AOK;
}
