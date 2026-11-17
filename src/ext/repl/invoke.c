#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/repl.h>

#define INVOKE_STATIC 0x10U
#define INVOKE_READONLY 0x20U
#define INVOKE_LITERAL 0x40U
#define INVOKE_ABSTRACT 0x0400U
#define INVOKE_VOID 0x01U
#define INVOKE_I1 0x04U
#define INVOKE_U1 0x05U
#define INVOKE_I2 0x06U
#define INVOKE_U2 0x07U
#define INVOKE_U4 0x09U
#define INVOKE_I8 0x0AU
#define INVOKE_U8 0x0BU
#define INVOKE_OBJ 0x1CU
#define INVOKE_ARRAY 0x14U
#define INVOKE_SZARRAY 0x1DU
#define INVOKE_GENERIC 0x15U

static uint8_t invoke_eq_is(const char *lhs, const char *rhs)
{
    uint32_t cmp = 0x0;

    if (ldg_str_cmp(lhs, rhs, LIBRESTEAM_EXT_REPL_CMD_CAP, &cmp) != LDG_ERR_AOK) { return LDG_TRUTH_FALSE; }

    return cmp == LDG_ARITH_ORD_EQ ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t invoke_enum_get(libresteam_ext_repl_ctx_t *ctx, void *type, const char *text, uint32_t *out)
{
    void *klass = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
    void *iter = 0x0;
    void *field = 0x0;
    uint64_t number = 0x0;
    uint32_t val = 0x0;
    uint32_t size = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t numeric = LDG_TRUTH_FALSE;

    *out = 0;
    if (!klass || ctx->il2cpp.meta.class_enum(klass) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_value_size_get(ctx, type, &size);
    if (res != LDG_ERR_AOK || size != sizeof(val)) { return LDG_ERR_UNSUPPORTED; }

    res = libresteam_ext_repl_num_get(text, &number);
    numeric = res == LDG_ERR_AOK ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    if (numeric && number > UINT32_MAX) { return LDG_ERR_RANGE; }

    for (field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter); field; field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter))
    {
        if ((((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(field) & INVOKE_LITERAL) == 0) { continue; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_static_val_get(field, &val);
        if ((numeric && val == number) || invoke_eq_is(((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_name_get(field), text) == LDG_TRUTH_TRUE)
        {
            *out = val;
            return LDG_ERR_AOK;
        }
    }
    return LDG_ERR_NOT_FOUND;
}

uint32_t libresteam_ext_repl_enum_set(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t field_idx, const char *text, uint32_t obj_idx)
{
    void *field = 0x0;
    void *type = 0x0;
    void *obj = 0x0;
    uint32_t flags = 0x0;
    uint32_t old = 0x0;
    uint32_t val = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !klass || !text)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (ctx->il2cpp.meta.class_value(klass) == LDG_TRUTH_TRUE || ctx->il2cpp.meta.class_generic(klass) == LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    res = libresteam_ext_repl_field_get(ctx, klass, field_idx, &field);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    flags = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(field);
    if ((flags & (INVOKE_READONLY | INVOKE_LITERAL)) != 0) { return LDG_ERR_DENIED; }

    type = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(field);
    res = invoke_enum_get(ctx, type, text, &val);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if ((flags & INVOKE_STATIC) != 0)
    {
        if (obj_idx != UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_static_val_get(field, &old);
    }
    else
    {
        res = libresteam_ext_repl_obj_get(ctx, obj_idx, &obj);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (ctx->il2cpp.meta.assignable(klass, ctx->il2cpp.meta.obj_class(obj)) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

        ctx->il2cpp.meta.field_rd(obj, field, &old);
    }

    res = ctx->api->log(ctx->mod, "repl enum set; field: %p; object: %p; old: %u32; new: %u32", field, obj, old, val);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if ((flags & INVOKE_STATIC) != 0) { ctx->il2cpp.meta.static_wr(field, &val); }
    else { ctx->il2cpp.meta.field_wr(obj, field, &val); }

    res = libresteam_ext_repl_emit(ctx, "assigned; previous: %u32", old);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_value_print(ctx, type, &val); }

    return res;
}

static uint32_t invoke_arg_get(libresteam_ext_repl_ctx_t *ctx, void *type, const char *text, uint64_t *storage, void **arg)
{
    void *klass = 0x0;
    void *obj = 0x0;
    uint64_t val = 0x0;
    uint64_t limit = 0x0;
    uint32_t enum_val = 0x0;
    uint32_t kind = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!type || ctx->il2cpp.meta.type_byref(type) == LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    *storage = 0;
    *arg = storage;
    kind = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type);
    klass = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
    if (klass && ctx->il2cpp.meta.class_enum(klass) == LDG_TRUTH_TRUE)
    {
        res = invoke_enum_get(ctx, type, text, &enum_val);
        *storage = enum_val;
        return res;
    }

    if (kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_CLASS || kind == LIBRESTEAM_BACKENDS_IL2CPP_TYPE_STR || kind == INVOKE_OBJ || kind == INVOKE_ARRAY || kind == INVOKE_SZARRAY || (kind == INVOKE_GENERIC && klass && ctx->il2cpp.meta.class_value(klass) == LDG_TRUTH_FALSE))
    {
        *arg = 0x0;
        if (invoke_eq_is(text, "null") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        if (text[0] != '@') { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_num_get(text + 1, &val);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK || val > UINT32_MAX)) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_obj_get(ctx, (uint32_t)val, &obj);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (!klass || ctx->il2cpp.meta.assignable(klass, ctx->il2cpp.meta.obj_class(obj)) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

        *arg = obj;
        return LDG_ERR_AOK;
    }

    switch (kind)
    {
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_BOOL:
        {
            if (invoke_eq_is(text, "true") == LDG_TRUTH_TRUE) { *storage = LDG_TRUTH_TRUE; return LDG_ERR_AOK; }

            if (invoke_eq_is(text, "false") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

            limit = LDG_TRUTH_TRUE;
            break;
        }
        case INVOKE_I1:
        { limit = INT8_MAX;
          break;
        }
        case INVOKE_U1:
        { limit = UINT8_MAX;
          break;
        }
        case INVOKE_I2:
        { limit = INT16_MAX;
          break;
        }
        case INVOKE_U2:
        { limit = UINT16_MAX;
          break;
        }
        case LIBRESTEAM_BACKENDS_IL2CPP_TYPE_I4:
        { limit = INT32_MAX;
          break;
        }
        case INVOKE_U4:
        { limit = UINT32_MAX;
          break;
        }
        case INVOKE_I8:
        { limit = INT64_MAX;
          break;
        }
        case INVOKE_U8:
        { limit = UINT64_MAX;
          break;
        }
        default: { return LDG_ERR_UNSUPPORTED;
        }
    }
    res = libresteam_ext_repl_num_get(text, &val);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (val > limit) { return LDG_ERR_RANGE; }

    *storage = val;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_call(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t method_idx, uint32_t obj_idx, uint32_t argc, char **args)
{
    uint64_t storage[LIBRESTEAM_EXT_REPL_PARAM_CAP] = LDG_ARR_ZERO_INIT;
    void *params[LIBRESTEAM_EXT_REPL_PARAM_CAP] = LDG_ARR_ZERO_INIT;
    void *method = 0x0;
    void *obj = 0x0;
    void *ret = 0x0;
    void *type = 0x0;
    void *data = 0x0;
    uint32_t impl = 0x0;
    uint32_t flags = 0x0;
    uint32_t size = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !klass || (argc && !args))) { return LDG_ERR_FUNC_ARG_NULL; }

    if (argc > LIBRESTEAM_EXT_REPL_PARAM_CAP || ctx->il2cpp.meta.class_value(klass) == LDG_TRUTH_TRUE || ctx->il2cpp.meta.class_generic(klass) == LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    res = libresteam_ext_repl_method_get(ctx, klass, method_idx, &method);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (ctx->il2cpp.meta.method_generic(method) == LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    if (ctx->il2cpp.meta.param_cunt(method) != argc) { return LDG_ERR_FUNC_ARG_INVALID; }

    type = ctx->il2cpp.meta.ret_type(method);
    if (!type || ctx->il2cpp.meta.type_byref(type) == LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    if (((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type) != INVOKE_VOID)
    {
        res = libresteam_ext_repl_value_size_get(ctx, type, &size);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }

    flags = ctx->il2cpp.meta.method_flags(method, &impl);
    if ((flags & INVOKE_ABSTRACT) != 0) { return LDG_ERR_UNSUPPORTED; }

    if ((flags & INVOKE_STATIC) == 0)
    {
        res = libresteam_ext_repl_obj_get(ctx, obj_idx, &obj);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (ctx->il2cpp.meta.assignable(klass, ctx->il2cpp.meta.obj_class(obj)) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }
    }
    else if (obj_idx != UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

    for (idx = 0; idx < argc; idx++)
    {
        res = invoke_arg_get(ctx, ctx->il2cpp.meta.param_type(method, idx), args[idx], &storage[idx], &params[idx]);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    res = ctx->api->log(ctx->mod, "repl call; method: %p; object: %p; argc: %u32", method, obj, argc);
    if (res == LDG_ERR_AOK) { res = ctx->api->invoke(ctx->api->il2cpp, method, obj, argc ? params : 0x0, &ret); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type) == INVOKE_VOID) { return libresteam_ext_repl_value_print(ctx, type, 0x0); }

    if (!ret) { return libresteam_ext_repl_emit(ctx, "return: null"); }

    if (ctx->il2cpp.meta.class_value(ctx->il2cpp.meta.obj_class(ret)) == LDG_TRUTH_TRUE)
    {
        data = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->obj_unbox(ret);
        if (!data) { return LDG_ERR_INVALID; }

        return libresteam_ext_repl_value_print(ctx, type, data);
    }

    return libresteam_ext_repl_value_print(ctx, type, &ret);
}
