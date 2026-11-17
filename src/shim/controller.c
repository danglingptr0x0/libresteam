#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/cpp/callee.h>
#include <dangling/mem/secure.h>

#include <libresteam/shim/shim.h>

#define LIBRESTEAM_SHIM_CONTROLLER_007_VERSION "SteamController007"
#define LIBRESTEAM_SHIM_CONTROLLER_007_METHOD_CUNT 34U
#define LIBRESTEAM_SHIM_CONTROLLER_INIT 0U
#define LIBRESTEAM_SHIM_CONTROLLER_SHUTDOWN 1U
#define LIBRESTEAM_SHIM_CONTROLLER_DIGITAL_DATA_GET 12U
#define LIBRESTEAM_SHIM_CONTROLLER_ANALOG_DATA_GET 15U
#define LIBRESTEAM_SHIM_CONTROLLER_GLYPH_GET 17U
#define LIBRESTEAM_SHIM_CONTROLLER_STR_GET 18U
#define LIBRESTEAM_SHIM_CONTROLLER_MOTION_DATA_GET 20U
#define LIBRESTEAM_SHIM_CONTROLLER_XBOX_STR_GET 29U
#define LIBRESTEAM_SHIM_CONTROLLER_XBOX_GLYPH_GET 30U
#define LIBRESTEAM_SHIM_CONTROLLER_STUB_NONE 0U
#define LIBRESTEAM_SHIM_CONTROLLER_IMPL(fn) { __typeof__(&fn) cb = 0x0; cb = fn; return ldg_mem_secure_cpy(out, &cb, (uint64_t)sizeof(cb)); }

static LDG_CPP_STRUCT_RET_TYPE(libresteam_shim_flat_input_digital_action_data_t) libresteam_shim_controller_digital_data_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_shim_flat_input_digital_action_data_t))
{
    libresteam_shim_flat_input_digital_action_data_t res = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_stub_hit(self, LIBRESTEAM_SHIM_CONTROLLER_DIGITAL_DATA_GET) != LIBRESTEAM_SHIM_CONTROLLER_STUB_NONE) { LDG_CPP_STRUCT_RET(res); }

    LDG_CPP_STRUCT_RET(res);
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_shim_flat_input_analog_action_data_t) libresteam_shim_controller_analog_data_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_shim_flat_input_analog_action_data_t))
{
    libresteam_shim_flat_input_analog_action_data_t res = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_stub_hit(self, LIBRESTEAM_SHIM_CONTROLLER_ANALOG_DATA_GET) != LIBRESTEAM_SHIM_CONTROLLER_STUB_NONE) { LDG_CPP_STRUCT_RET(res); }

    LDG_CPP_STRUCT_RET(res);
}

static LDG_CPP_STRUCT_RET_TYPE(libresteam_shim_flat_input_motion_data_t) libresteam_shim_controller_motion_data_get(LDG_CPP_STRUCT_SELF_PARAMS(libresteam_shim_flat_input_motion_data_t))
{
    libresteam_shim_flat_input_motion_data_t res = LDG_STRUCT_ZERO_INIT;

    if (libresteam_shim_stub_hit(self, LIBRESTEAM_SHIM_CONTROLLER_MOTION_DATA_GET) != LIBRESTEAM_SHIM_CONTROLLER_STUB_NONE) { LDG_CPP_STRUCT_RET(res); }

    LDG_CPP_STRUCT_RET(res);
}

static uint32_t libresteam_shim_controller_007_name_get(uint32_t idx, const char **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    switch (idx)
    {
        case LIBRESTEAM_SHIM_CONTROLLER_INIT:
        { *out = "Init";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_SHUTDOWN:
        { *out = "Shutdown";
          break;
        }
        case 2:
        { *out = "RunFrame";
          break;
        }
        case 3:
        { *out = "GetConnectedControllers";
          break;
        }
        case 4:
        { *out = "GetActionSetHandle";
          break;
        }
        case 5:
        { *out = "ActivateActionSet";
          break;
        }
        case 6:
        { *out = "GetCurrentActionSet";
          break;
        }
        case 7:
        { *out = "ActivateActionSetLayer";
          break;
        }
        case 8:
        { *out = "DeactivateActionSetLayer";
          break;
        }
        case 9:
        { *out = "DeactivateAllActionSetLayers";
          break;
        }
        case 10:
        { *out = "GetActiveActionSetLayers";
          break;
        }
        case 11:
        { *out = "GetDigitalActionHandle";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_DIGITAL_DATA_GET:
        { *out = "GetDigitalActionData";
          break;
        }
        case 13:
        { *out = "GetDigitalActionOrigins";
          break;
        }
        case 14:
        { *out = "GetAnalogActionHandle";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_ANALOG_DATA_GET:
        { *out = "GetAnalogActionData";
          break;
        }
        case 16:
        { *out = "GetAnalogActionOrigins";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_GLYPH_GET:
        { *out = "GetGlyphForActionOrigin";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_STR_GET:
        { *out = "GetStringForActionOrigin";
          break;
        }
        case 19:
        { *out = "StopAnalogActionMomentum";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_MOTION_DATA_GET:
        { *out = "GetMotionData";
          break;
        }
        case 21:
        { *out = "TriggerHapticPulse";
          break;
        }
        case 22:
        { *out = "TriggerRepeatedHapticPulse";
          break;
        }
        case 23:
        { *out = "TriggerVibration";
          break;
        }
        case 24:
        { *out = "SetLEDColor";
          break;
        }
        case 25:
        { *out = "ShowBindingPanel";
          break;
        }
        case 26:
        { *out = "GetInputTypeForHandle";
          break;
        }
        case 27:
        { *out = "GetControllerForGamepadIndex";
          break;
        }
        case 28:
        { *out = "GetGamepadIndexForController";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_XBOX_STR_GET:
        { *out = "GetStringForXboxOrigin";
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_XBOX_GLYPH_GET:
        { *out = "GetGlyphForXboxOrigin";
          break;
        }
        case 31:
        { *out = "GetActionOriginFromXboxOrigin";
          break;
        }
        case 32:
        { *out = "TranslateActionOrigin";
          break;
        }
        case 33:
        { *out = "GetControllerBindingRevision";
          break;
        }
        default: { return LDG_ERR_RANGE;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_controller_007_kind_get(uint32_t idx, uint32_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = LIBRESTEAM_SHIM_RET_INT;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_CONTROLLER_007_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_CONTROLLER_GLYPH_GET:
        case LIBRESTEAM_SHIM_CONTROLLER_STR_GET:
        case LIBRESTEAM_SHIM_CONTROLLER_XBOX_STR_GET:
        case LIBRESTEAM_SHIM_CONTROLLER_XBOX_GLYPH_GET:
        { *out = LIBRESTEAM_SHIM_RET_STR;
          break;
        }
        case LIBRESTEAM_SHIM_CONTROLLER_DIGITAL_DATA_GET:
        case LIBRESTEAM_SHIM_CONTROLLER_ANALOG_DATA_GET:
        case LIBRESTEAM_SHIM_CONTROLLER_MOTION_DATA_GET:
        { *out = LIBRESTEAM_SHIM_RET_STRUCT;
          break;
        }
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

static uint32_t libresteam_shim_controller_007_impl_get(uint32_t idx, void **out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;

    if (LDG_UNLIKELY(idx >= LIBRESTEAM_SHIM_CONTROLLER_007_METHOD_CUNT)) { return LDG_ERR_RANGE; }

    switch (idx)
    {
        case LIBRESTEAM_SHIM_CONTROLLER_DIGITAL_DATA_GET:
            LIBRESTEAM_SHIM_CONTROLLER_IMPL(libresteam_shim_controller_digital_data_get)
        case LIBRESTEAM_SHIM_CONTROLLER_ANALOG_DATA_GET:
            LIBRESTEAM_SHIM_CONTROLLER_IMPL(libresteam_shim_controller_analog_data_get)
        case LIBRESTEAM_SHIM_CONTROLLER_MOTION_DATA_GET:
            LIBRESTEAM_SHIM_CONTROLLER_IMPL(libresteam_shim_controller_motion_data_get)
        default: { break;
        }
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_shim_controller_007_def_get(libresteam_shim_iface_def_t *out)
{
    if (LDG_UNLIKELY(!out)) { return LDG_ERR_FUNC_ARG_NULL; }

    out->version = LIBRESTEAM_SHIM_CONTROLLER_007_VERSION;
    out->name_get = libresteam_shim_controller_007_name_get;
    out->kind_get = libresteam_shim_controller_007_kind_get;
    out->impl_get = libresteam_shim_controller_007_impl_get;
    out->method_cunt = LIBRESTEAM_SHIM_CONTROLLER_007_METHOD_CUNT;

    return LDG_ERR_AOK;
}
