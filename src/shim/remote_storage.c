#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_REMOTE_STORAGE_VERSION "STEAMREMOTESTORAGE_INTERFACE_VERSION014"
#define LIBRESTEAM_SHIM_REMOTE_STORAGE_METHOD_CUNT 61U
#define LIBRESTEAM_SHIM_REMOTE_STORAGE_FILE_NAME_SIZE_GET 19U

static uint32_t libresteam_shim_remote_storage_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_SLOT_FIRST:
        { *out = "FileWrite";
          break;
        }
        case LIBRESTEAM_SHIM_SLOT_SECOND:
        { *out = "FileRead";
          break;
        }
        case 2:
        { *out = "FileWriteAsync";
          break;
        }
        case 3:
        { *out = "FileReadAsync";
          break;
        }
        case 4:
        { *out = "FileReadAsyncComplete";
          break;
        }
        case 5:
        { *out = "FileForget";
          break;
        }
        case 6:
        { *out = "FileDelete";
          break;
        }
        case 7:
        { *out = "FileShare";
          break;
        }
        case 8:
        { *out = "SetSyncPlatforms";
          break;
        }
        case 9:
        { *out = "FileWriteStreamOpen";
          break;
        }
        case 10:
        { *out = "FileWriteStreamWriteChunk";
          break;
        }
        case 11:
        { *out = "FileWriteStreamClose";
          break;
        }
        case 12:
        { *out = "FileWriteStreamCancel";
          break;
        }
        case 13:
        { *out = "FileExists";
          break;
        }
        case 14:
        { *out = "FilePersisted";
          break;
        }
        case 15:
        { *out = "GetFileSize";
          break;
        }
        case 16:
        { *out = "GetFileTimestamp";
          break;
        }
        case 17:
        { *out = "GetSyncPlatforms";
          break;
        }
        case 18:
        { *out = "GetFileCount";
          break;
        }
        case 19:
        { *out = "GetFileNameAndSize";
          break;
        }
        case 20:
        { *out = "GetQuota";
          break;
        }
        case 21:
        { *out = "IsCloudEnabledForAccount";
          break;
        }
        case 22:
        { *out = "IsCloudEnabledForApp";
          break;
        }
        case 23:
        { *out = "SetCloudEnabledForApp";
          break;
        }
        case 24:
        { *out = "UGCDownload";
          break;
        }
        case 25:
        { *out = "GetUGCDownloadProgress";
          break;
        }
        case 26:
        { *out = "GetUGCDetails";
          break;
        }
        case 27:
        { *out = "UGCRead";
          break;
        }
        case 28:
        { *out = "GetCachedUGCCount";
          break;
        }
        case 29:
        { *out = "GetCachedUGCHandle";
          break;
        }
        case 30:
        { *out = "GetFileListFromServer";
          break;
        }
        case 31:
        { *out = "FileFetch";
          break;
        }
        case 32:
        { *out = "FilePersist";
          break;
        }
        case 33:
        { *out = "SynchronizeToClient";
          break;
        }
        case 34:
        { *out = "SynchronizeToServer";
          break;
        }
        case 35:
        { *out = "ResetFileRequestState";
          break;
        }
        case 36:
        { *out = "PublishWorkshopFile";
          break;
        }
        case 37:
        { *out = "CreatePublishedFileUpdateRequest";
          break;
        }
        case 38:
        { *out = "UpdatePublishedFileFile";
          break;
        }
        case 39:
        { *out = "UpdatePublishedFilePreviewFile";
          break;
        }
        case 40:
        { *out = "UpdatePublishedFileTitle";
          break;
        }
        case 41:
        { *out = "UpdatePublishedFileDescription";
          break;
        }
        case 42:
        { *out = "UpdatePublishedFileVisibility";
          break;
        }
        case 43:
        { *out = "UpdatePublishedFileTags";
          break;
        }
        case 44:
        { *out = "CommitPublishedFileUpdate";
          break;
        }
        case 45:
        { *out = "GetPublishedFileDetails";
          break;
        }
        case 46:
        { *out = "DeletePublishedFile";
          break;
        }
        case 47:
        { *out = "EnumerateUserPublishedFiles";
          break;
        }
        case 48:
        { *out = "SubscribePublishedFile";
          break;
        }
        case 49:
        { *out = "EnumerateUserSubscribedFiles";
          break;
        }
        case 50:
        { *out = "UnsubscribePublishedFile";
          break;
        }
        case 51:
        { *out = "UpdatePublishedFileSetChangeDescription";
          break;
        }
        case 52:
        { *out = "GetPublishedItemVoteDetails";
          break;
        }
        case 53:
        { *out = "UpdateUserPublishedItemVote";
          break;
        }
        case 54:
        { *out = "GetUserPublishedItemVoteDetails";
          break;
        }
        case 55:
        { *out = "EnumerateUserSharedWorkshopFiles";
          break;
        }
        case 56:
        { *out = "PublishVideo";
          break;
        }
        case 57:
        { *out = "SetUserPublishedFileAction";
          break;
        }
        case 58:
        { *out = "EnumeratePublishedFilesByUserAction";
          break;
        }
        case 59:
        { *out = "EnumeratePublishedWorkshopFiles";
          break;
        }
        case 60:
        { *out = "UGCDownloadToLocation";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_remote_storage_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_REMOTE_STORAGE_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    if (idx == LIBRESTEAM_SHIM_REMOTE_STORAGE_FILE_NAME_SIZE_GET) { *out = LIBRESTEAM_SHIM_RET_STR; }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_remote_storage_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_REMOTE_STORAGE_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_remote_storage_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_REMOTE_STORAGE_VERSION;
    out->name_get = libresteam_shim_remote_storage_name_get;
    out->kind_get = libresteam_shim_remote_storage_kind_get;
    out->impl_get = libresteam_shim_remote_storage_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_REMOTE_STORAGE_METHOD_CUNT;

    return LDG_ERR_AOK;
}
