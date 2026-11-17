#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>
#include <dangling/sys/lib.h>

#include <libresteam/ext/repl/repl.h>

static uint32_t meta_sym_get(void *lib, const char *name, void *out)
{
    ldg_sys_lib_func_t sym = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = ldg_sys_lib_sym_get(lib, name, &sym);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    return ldg_mem_secure_cpy(out, &sym, sizeof(sym));
}

static uint32_t meta_init(libresteam_ext_repl_ctx_t *ctx)
{
    void *lib = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->lib;
    uint32_t res = LDG_ERR_AOK;

    res |= meta_sym_get(lib, "il2cpp_image_get_class_count", &ctx->il2cpp.meta.img_cunt);
    res |= meta_sym_get(lib, "il2cpp_image_get_class", &ctx->il2cpp.meta.img_class);
    res |= meta_sym_get(lib, "il2cpp_class_get_namespace", &ctx->il2cpp.meta.class_ns);
    res |= meta_sym_get(lib, "il2cpp_class_get_image", &ctx->il2cpp.meta.class_img);
    res |= meta_sym_get(lib, "il2cpp_image_get_name", &ctx->il2cpp.meta.img_name);
    res |= meta_sym_get(lib, "il2cpp_class_get_methods", &ctx->il2cpp.meta.methods);
    res |= meta_sym_get(lib, "il2cpp_method_get_name", &ctx->il2cpp.meta.method_name);
    res |= meta_sym_get(lib, "il2cpp_method_get_param_count", &ctx->il2cpp.meta.param_cunt);
    res |= meta_sym_get(lib, "il2cpp_method_get_return_type", &ctx->il2cpp.meta.ret_type);
    res |= meta_sym_get(lib, "il2cpp_method_get_param", &ctx->il2cpp.meta.param_type);
    res |= meta_sym_get(lib, "il2cpp_method_get_flags", &ctx->il2cpp.meta.method_flags);
    res |= meta_sym_get(lib, "il2cpp_field_get_value", &ctx->il2cpp.meta.field_rd);
    res |= meta_sym_get(lib, "il2cpp_field_set_value", &ctx->il2cpp.meta.field_wr);
    res |= meta_sym_get(lib, "il2cpp_field_static_set_value", &ctx->il2cpp.meta.static_wr);
    res |= meta_sym_get(lib, "il2cpp_object_get_class", &ctx->il2cpp.meta.obj_class);
    res |= meta_sym_get(lib, "il2cpp_class_is_enum", &ctx->il2cpp.meta.class_enum);
    res |= meta_sym_get(lib, "il2cpp_class_is_valuetype", &ctx->il2cpp.meta.class_value);
    res |= meta_sym_get(lib, "il2cpp_method_is_generic", &ctx->il2cpp.meta.method_generic);
    res |= meta_sym_get(lib, "il2cpp_class_is_generic", &ctx->il2cpp.meta.class_generic);
    res |= meta_sym_get(lib, "il2cpp_type_is_byref", &ctx->il2cpp.meta.type_byref);
    res |= meta_sym_get(lib, "il2cpp_class_is_assignable_from", &ctx->il2cpp.meta.assignable);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return LDG_ERR_NOT_FOUND; }

    return ctx->api->class_find(ctx->api->il2cpp, "UnityEngine", "Object", &ctx->il2cpp.unity_class);
}

uint32_t libresteam_ext_repl_meta_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    void **assemblies = 0x0;
    void *img = 0x0;
    void *klass = 0x0;
    uint64_t cunt = 0x0;
    uint64_t classes = 0x0;
    uint32_t budget = LIBRESTEAM_EXT_REPL_BATCH;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->api->il2cpp || !((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->ready || !ctx->api->class_find || !ctx->api->objs_hold || !ctx->api->obj_alive_is || !ctx->api->obj_name_get || !ctx->api->invoke) { return LDG_ERR_AOK; }

    if (!ctx->il2cpp.meta_tried)
    {
        ctx->il2cpp.meta_res = meta_init(ctx);
        ctx->il2cpp.meta_tried = LDG_TRUTH_TRUE;
        res = ctx->api->log(ctx->mod, "repl metadata init; res: %u32", ctx->il2cpp.meta_res);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }

    if (ctx->il2cpp.meta_res != LDG_ERR_AOK || ctx->il2cpp.index_done) { return LDG_ERR_AOK; }

    assemblies = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->domain_assemblies_get(((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->domain, &cunt);
    if (LDG_UNLIKELY(!assemblies && cunt != 0)) { ctx->il2cpp.meta_res = LDG_ERR_INVALID; return ctx->il2cpp.meta_res; }

    while (ctx->il2cpp.assembly_idx < cunt && budget != 0)
    {
        budget--;
        img = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->assembly_img_get(assemblies[ctx->il2cpp.assembly_idx]);
        if (LDG_UNLIKELY(!img)) { ctx->il2cpp.meta_res = LDG_ERR_INVALID; return ctx->il2cpp.meta_res; }

        classes = ctx->il2cpp.meta.img_cunt(img);
        if (ctx->il2cpp.class_idx >= classes)
        {
            ctx->il2cpp.class_idx = 0;
            ctx->il2cpp.assembly_idx++;
            continue;
        }

        if (ctx->il2cpp.class_cunt == LIBRESTEAM_EXT_REPL_CLASS_CAP)
        {
            ctx->il2cpp.index_done = LDG_TRUTH_TRUE;
            return ctx->api->log(ctx->mod, "repl type index truncated; limit: %u32", (uint32_t)LIBRESTEAM_EXT_REPL_CLASS_CAP);
        }

        klass = ctx->il2cpp.meta.img_class(img, ctx->il2cpp.class_idx++);
        if (klass) { ctx->il2cpp.classes[ctx->il2cpp.class_cunt++] = klass; }
    }
    if (ctx->il2cpp.assembly_idx == cunt)
    {
        ctx->il2cpp.index_done = LDG_TRUTH_TRUE;
        return ctx->api->log(ctx->mod, "repl type index ready; types: %u32", ctx->il2cpp.class_cunt);
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_obj_hold(libresteam_ext_repl_ctx_t *ctx, void *obj, uint32_t *out)
{
    void *klass = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t handle = 0x0;

    if (LDG_UNLIKELY(!ctx || !obj || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = UINT32_MAX;
    for (idx = 0; idx < ctx->il2cpp.obj_next; idx++) { if (ctx->il2cpp.objs[idx].handle && ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_target_get(ctx->il2cpp.objs[idx].handle) == obj) { *out = idx; return LDG_ERR_AOK; } }
    if (LDG_UNLIKELY(ctx->il2cpp.obj_next == LIBRESTEAM_EXT_REPL_OBJ_CAP)) { return LDG_ERR_FULL; }

    handle = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_new(obj, LDG_TRUTH_TRUE);
    if (LDG_UNLIKELY(handle == 0)) { return LDG_ERR_ALLOC_NULL; }

    klass = ctx->il2cpp.meta.obj_class(obj);
    idx = ctx->il2cpp.obj_next++;
    ctx->il2cpp.objs[idx].handle = handle;
    ctx->il2cpp.objs[idx].unity = ctx->il2cpp.meta.assignable(ctx->il2cpp.unity_class, klass);
    *out = idx;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_obj_get(libresteam_ext_repl_ctx_t *ctx, uint32_t idx, void **out)
{
    uint8_t alive = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;
    if (LDG_UNLIKELY(idx >= ctx->il2cpp.obj_next || !ctx->il2cpp.objs[idx].handle)) { return LDG_ERR_NOT_FOUND; }

    *out = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_target_get(ctx->il2cpp.objs[idx].handle);
    if (*out && ctx->il2cpp.objs[idx].unity)
    {
        alive = ctx->api->obj_alive_is(ctx->api->il2cpp, *out);
        if (alive != LDG_TRUTH_TRUE) { *out = 0x0; }
    }

    if (!*out)
    {
        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free(ctx->il2cpp.objs[idx].handle);
        ctx->il2cpp.objs[idx].handle = 0;
        return LDG_ERR_NOT_FOUND;
    }

    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_field_get(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t idx, void **out)
{
    void *iter = 0x0;
    void *field = 0x0;
    uint32_t pos = 0x0;

    if (LDG_UNLIKELY(!ctx || !klass || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;
    for (field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter); field; field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter)) { if (pos++ == idx) { *out = field; return LDG_ERR_AOK; } }
    return LDG_ERR_NOT_FOUND;
}

uint32_t libresteam_ext_repl_method_get(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t idx, void **out)
{
    void *iter = 0x0;
    void *method = 0x0;
    uint32_t pos = 0x0;

    if (LDG_UNLIKELY(!ctx || !klass || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    *out = 0x0;
    for (method = ctx->il2cpp.meta.methods(klass, &iter); method; method = ctx->il2cpp.meta.methods(klass, &iter)) { if (pos++ == idx) { *out = method; return LDG_ERR_AOK; } }
    return LDG_ERR_NOT_FOUND;
}

uint32_t libresteam_ext_repl_il2cpp_shutdown(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    uint32_t idx = UINT32_MAX;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (idx = 0; idx < ctx->il2cpp.obj_next; idx++)
    {
        if (!ctx->il2cpp.objs[idx].handle) { continue; }

        if (!ctx->api->il2cpp || !((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free) { return LDG_ERR_NOT_INIT; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free(ctx->il2cpp.objs[idx].handle);
        ctx->il2cpp.objs[idx].handle = 0;
    }
    return LDG_ERR_AOK;
}
