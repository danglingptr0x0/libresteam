#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/repl.h>

#define VALUE_VOID 0x01U
#define VALUE_CHAR 0x03U
#define VALUE_I1 0x04U
#define VALUE_U1 0x05U
#define VALUE_I2 0x06U
#define VALUE_U2 0x07U
#define VALUE_U4 0x09U
#define VALUE_I8 0x0AU
#define VALUE_U8 0x0BU
#define VALUE_R8 0x0DU
#define VALUE_PTR 0x0FU
#define VALUE_ARRAY 0x14U
#define VALUE_GENERIC 0x15U
#define VALUE_NATIVE_I 0x18U
#define VALUE_NATIVE_U 0x19U
#define VALUE_OBJ 0x1CU
#define VALUE_SZARRAY 0x1DU

uint32_t libresteam_ext_repl_value_size_get(libresteam_ext_repl_ctx_t *ctx, void *type, uint32_t *out)
{
    void *klass = 0x0;
    void *field = 0x0;
    uint32_t kind = 0x0;

    if (LDG_UNLIKELY(!ctx || !type || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0;
    kind = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type);
    switch (kind)
    {
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_BOOL:
        case VALUE_I1:
        case VALUE_U1:
        { *out = sizeof(uint8_t);
          return LDG_ERR_AOK;
        }
        case VALUE_CHAR:
        case VALUE_I2:
        case VALUE_U2:
        { *out = sizeof(uint16_t);
          return LDG_ERR_AOK;
        }
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_I4:
        case VALUE_U4:
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_R4:
        { *out = sizeof(uint32_t);
          return LDG_ERR_AOK;
        }
        case VALUE_I8:
        case VALUE_U8:
        case VALUE_R8:
        case VALUE_PTR:
        case VALUE_NATIVE_I:
        case VALUE_NATIVE_U:
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_STR:
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_CLASS:
        case VALUE_ARRAY:
        case VALUE_OBJ:
        case VALUE_SZARRAY:
        { *out = sizeof(uint64_t);
          return LDG_ERR_AOK;
        }
        case VALUE_GENERIC:
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_VALUETYPE:
        {
            klass = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
            if (!klass) { return LDG_ERR_INVALID; }

            if (ctx->il2cpp.meta.class_value(klass) == LDG_TRUTH_FALSE) { *out = sizeof(void *); return LDG_ERR_AOK; }

            if (ctx->il2cpp.meta.class_enum(klass) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

            field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_field_from_name(klass, "value__");
            if (!field) { return LDG_ERR_INVALID; }

            type = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(field);
            if (!type || ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type) != LIBRESTEAM_BACKENDS_IL2CPP_TYPE_I4) { return LDG_ERR_UNSUPPORTED; }

            *out = sizeof(uint32_t);
            return LDG_ERR_AOK;
        }
        default: { return LDG_ERR_UNSUPPORTED;
        }
    }
}

uint32_t libresteam_ext_repl_value_print(libresteam_ext_repl_ctx_t *ctx, void *type, const void *data)
{
    void *obj = 0x0;
    void *klass = 0x0;
    const char *enum_name = 0x0;
    uint64_t bits = 0x0;
    double real = 0x0;
    libresteam_fp32_t small = 0x0;
    uint32_t kind = 0x0;
    uint32_t size = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !type)) { return LDG_ERR_FUNC_ARG_NULL; }

    kind = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type);
    if (kind == VALUE_VOID) { return libresteam_ext_repl_emit(ctx, "return: void"); }

    if (LDG_UNLIKELY(!data)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_ext_repl_value_size_get(ctx, type, &size);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = ldg_mem_secure_cpy(&bits, data, size);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_R4)
    {
        res = ldg_mem_secure_cpy(&small, data, sizeof(small));
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return libresteam_ext_repl_emit(ctx, "value: %f", (double)small);
    }

    if (kind == VALUE_R8)
    {
        res = ldg_mem_secure_cpy(&real, data, sizeof(real));
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return libresteam_ext_repl_emit(ctx, "value: %f", real);
    }

    if (kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_VALUETYPE || kind == VALUE_GENERIC)
    {
        klass = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
        if (klass && ctx->il2cpp.meta.class_enum(klass) == LDG_TRUTH_TRUE)
        {
            res = ctx->api->enum_name_get(ctx->api->il2cpp, klass, (uint32_t)bits, &enum_name);
            if (res != LDG_ERR_AOK) { enum_name = "-"; }

            res = libresteam_ext_repl_name_get(enum_name, ctx->name, sizeof(ctx->name));
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            return libresteam_ext_repl_emit(ctx, "enum: %s; value: %u32", ctx->name, (uint32_t)bits);
        }
    }

    if (kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_STR || kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_CLASS || kind == VALUE_ARRAY || kind == VALUE_GENERIC || kind == VALUE_OBJ || kind == VALUE_SZARRAY)
    {
        obj = (void *)(uintptr_t)bits;
        if (!obj) { return libresteam_ext_repl_emit(ctx, "value: null"); }

        res = libresteam_ext_repl_obj_hold(ctx, obj, &idx);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_STR)
        {
            res = ctx->api->str_ascii_get(ctx->api->il2cpp, obj, ctx->name, sizeof(ctx->name));
            if (res != LDG_ERR_AOK && res != LDG_ERR_STR_TRUNC) { return res; }

            res = libresteam_ext_repl_name_get(ctx->name, ctx->detail, sizeof(ctx->detail));
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            return libresteam_ext_repl_emit(ctx, "object: %u32; ptr: %p; string: %s", idx, obj, ctx->detail);
        }

        return libresteam_ext_repl_emit(ctx, "object: %u32; ptr: %p", idx, obj);
    }

    return libresteam_ext_repl_emit(ctx, "value unsigned: %u64; bits: 0x%x64; bytes: %u32; kind: 0x%x32", bits, bits, size, kind);
}
