#include <stdint.h>

#include <dangling/core/arith.h>
#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/secure.h>
#include <dangling/str/fmt.h>
#include <dangling/str/str.h>

#include <libresteam/ext/repl/repl.h>

#define CMD_FIELD_LITERAL 0x40U

static uint32_t cmd_idx_get(const char *text, uint32_t *out)
{
    uint64_t val = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_num_get(text, &val);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (LDG_UNLIKELY(val > UINT32_MAX)) { return LDG_ERR_RANGE; }

    *out = (uint32_t)val;
    return LDG_ERR_AOK;
}

static uint32_t cmd_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = LDG_ERR_AOK;

    res |= libresteam_ext_repl_emit(ctx, "help | status | types [find] <substring|*> [start]");
    res |= libresteam_ext_repl_emit(ctx, "type <type-id> | fields <type-id> [start] | methods <type-id> [start]");
    res |= libresteam_ext_repl_emit(ctx, "method <type-id> <method-id> | objects <type-id> [start]");
    res |= libresteam_ext_repl_emit(ctx, "get <type-id> <field-id> [object-id]");
    res |= libresteam_ext_repl_emit(ctx, "fieldtype <type-id> <field-id> | objtype <object-id> | enum <type-id> [start]");
    res |= libresteam_ext_repl_emit(ctx, "set <type-id> <field-id> <enum-name|value> [object-id]   (32-bit declared enum members)");
    res |= libresteam_ext_repl_emit(ctx, "call <type-id> <method-id> <object-id|-> [arguments...]   (executes game code)");
    res |= libresteam_ext_repl_emit(ctx, "Call args: enum names/values, nonnegative integers, true/false, @object-id, null; max 8. No ref/out, structs, floats, or generic methods.");
    res |= libresteam_ext_repl_emit(ctx, "drop <object-id|all>");
    res |= libresteam_ext_repl_emit(ctx, "IDs are zero-based and local to this process, not metadata file indices. Fields/methods are declared members.");
    res |= libresteam_ext_repl_emit(ctx, "Addresses accept 0x hex. Names use ASCII byte escapes. Object handles retain managed wrappers; destroyed Unity objects are rejected.");
    res |= libresteam_ext_repl_emit(ctx, "Lists show at most 64 rows. Object search uses Unity FindObjectsOfType and can take a frame. quit disconnects the client.");
    return res;
}

static uint32_t cmd_type(libresteam_ext_repl_ctx_t *ctx, uint32_t idx)
{
    void *klass = ctx->il2cpp.classes[idx];
    void *img = ctx->il2cpp.meta.class_img(klass);
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_name_get(ctx->il2cpp.meta.class_ns(klass), ctx->name, sizeof(ctx->name));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_name_get(klass), ctx->detail, sizeof(ctx->detail)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "type: %u32; namespace: %s; name: %s; class: %p", idx, ctx->name, ctx->detail, klass); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(img ? ctx->il2cpp.meta.img_name(img) : 0x0, ctx->name, sizeof(ctx->name)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "image: %s; value type: %u32; enum: %u32", ctx->name, (uint32_t)ctx->il2cpp.meta.class_value(klass), (uint32_t)ctx->il2cpp.meta.class_enum(klass)); }

    return res;
}

static uint32_t cmd_class_type(libresteam_ext_repl_ctx_t *ctx, void *klass)
{
    uint32_t idx = UINT32_MAX;

    if (!klass) { return LDG_ERR_NOT_FOUND; }

    for (idx = 0; idx < ctx->il2cpp.class_cunt; idx++) { if (ctx->il2cpp.classes[idx] == klass) { return cmd_type(ctx, idx); } }
    return LDG_ERR_NOT_FOUND;
}

static uint32_t cmd_enum(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t start)
{
    void *iter = 0x0;
    void *field = 0x0;
    uint32_t idx = 0x0;
    uint32_t rows = 0x0;
    uint32_t val = 0x0;
    uint32_t size = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->il2cpp.meta.class_enum(klass) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_value_size_get(ctx, ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_type_get(klass), &size);
    if (res != LDG_ERR_AOK || size != sizeof(val)) { return LDG_ERR_UNSUPPORTED; }

    for (field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter); field; field = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter))
    {
        if (idx++ < start) { continue; }

        if (rows == LIBRESTEAM_EXT_REPL_ROW_CAP) { idx--; break; }

        if ((((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(field) & CMD_FIELD_LITERAL) == 0) { continue; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_static_val_get(field, &val);
        res = libresteam_ext_repl_name_get(((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_name_get(field), ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "field: %u32; name: %s; value: %u32", idx - 1U, ctx->name, val); }

        if (res != LDG_ERR_AOK) { return res; }

        rows++;
    }
    return libresteam_ext_repl_emit(ctx, "rows: %u32; next start: %u32", rows, idx);
}

uint32_t libresteam_ext_repl_search_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    void *klass = 0x0;
    const char *name = 0x0;
    const char *ns = 0x0;
    uint64_t hit = UINT64_MAX;
    uint32_t idx = UINT32_MAX;
    uint32_t budget = LIBRESTEAM_EXT_REPL_BATCH;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    while (ctx->il2cpp.search_idx < ctx->il2cpp.class_cunt && budget-- != 0 && ctx->il2cpp.search_rows < LIBRESTEAM_EXT_REPL_ROW_CAP)
    {
        idx = ctx->il2cpp.search_idx++;
        klass = ctx->il2cpp.classes[idx];
        name = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_name_get(klass);
        ns = ctx->il2cpp.meta.class_ns(klass);
        if (libresteam_ext_repl_cmd_eq_is(ctx->il2cpp.filter, "*") != LDG_TRUTH_TRUE)
        {
            res = name ? ldg_str_substring_find(name, ctx->il2cpp.filter, LIBRESTEAM_EXT_REPL_NAME_CAP, &hit) : LDG_ERR_NOT_FOUND;
            if (res == LDG_ERR_NOT_FOUND) { res = ns ? ldg_str_substring_find(ns, ctx->il2cpp.filter, LIBRESTEAM_EXT_REPL_NAME_CAP, &hit) : LDG_ERR_NOT_FOUND; }

            if (res == LDG_ERR_NOT_FOUND) { continue; }

            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
        }

        res = libresteam_ext_repl_name_get(ns, ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(name, ctx->detail, sizeof(ctx->detail)); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "type: %u32; namespace: %s; name: %s", idx, ctx->name, ctx->detail); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        ctx->il2cpp.search_rows++;
    }
    if (ctx->il2cpp.search_idx == ctx->il2cpp.class_cunt || ctx->il2cpp.search_rows == LIBRESTEAM_EXT_REPL_ROW_CAP)
    {
        ctx->searching = LDG_TRUTH_FALSE;
        return libresteam_ext_repl_emit(ctx, "rows: %u32; next start: %u32; total types: %u32", ctx->il2cpp.search_rows, ctx->il2cpp.search_idx, ctx->il2cpp.class_cunt);
    }

    return LDG_ERR_AOK;
}

static uint32_t cmd_members(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t start, uint8_t methods)
{
    void *iter = 0x0;
    void *member = 0x0;
    void *type = 0x0;
    void *type_class = 0x0;
    uint32_t idx = 0x0;
    uint32_t rows = 0x0;
    uint32_t flags = 0x0;
    uint32_t impl = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (;;)
    {
        member = methods ? ctx->il2cpp.meta.methods(klass, &iter) : ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter);
        if (!member) { break; }

        if (idx++ < start) { continue; }

        if (rows == LIBRESTEAM_EXT_REPL_ROW_CAP) { idx--; break; }

        type = methods ? ctx->il2cpp.meta.ret_type(member) : ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(member);
        if (LDG_UNLIKELY(!type)) { return LDG_ERR_INVALID; }

        type_class = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
        res = libresteam_ext_repl_name_get(methods ? ctx->il2cpp.meta.method_name(member) : ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_name_get(member), ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(type_class ? ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_name_get(type_class) : 0x0, ctx->detail, sizeof(ctx->detail)); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (methods)
        {
            flags = ctx->il2cpp.meta.method_flags(member, &impl);
            res = libresteam_ext_repl_emit(ctx, "method: %u32; name: %s; argc: %u32; return: %s; flags: 0x%x32", idx - 1U, ctx->name, ctx->il2cpp.meta.param_cunt(member), ctx->detail, flags);
        }
        else
        {
            flags = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(member);
            res = libresteam_ext_repl_emit(ctx, "field: %u32; name: %s; type: %s; kind: 0x%x32; flags: 0x%x32; off: %u64", idx - 1U, ctx->name, ctx->detail, ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type), flags, ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_off_get(member));
        }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        rows++;
    }
    return libresteam_ext_repl_emit(ctx, "rows: %u32; next start: %u32", rows, idx);
}

static uint32_t cmd_method(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t idx)
{
    void *method = 0x0;
    void *type = 0x0;
    void *type_class = 0x0;
    uintptr_t entry = 0x0;
    uint32_t argc = 0x0;
    uint32_t pos = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_method_get(ctx, klass, idx, &method);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = libresteam_ext_repl_name_get(ctx->il2cpp.meta.method_name(method), ctx->name, sizeof(ctx->name));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "method: %u32; name: %s; metadata: %p", idx, ctx->name, method); }

    /* The current IL2CPP ABI places methodPointer first; report only an executable candidate. */
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_rd(ctx, (uintptr_t)method, &entry, sizeof(entry)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_entry(ctx, entry); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    argc = ctx->il2cpp.meta.param_cunt(method);
    if (argc > LIBRESTEAM_EXT_REPL_ROW_CAP) { return LDG_ERR_FULL; }

    for (pos = 0; pos <= argc; pos++)
    {
        type = pos == argc ? ctx->il2cpp.meta.ret_type(method) : ctx->il2cpp.meta.param_type(method, pos);
        if (LDG_UNLIKELY(!type)) { return LDG_ERR_INVALID; }

        type_class = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type);
        res = libresteam_ext_repl_name_get(type_class ? ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_name_get(type_class) : 0x0, ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "%s: %u32; type: %s; kind: 0x%x32", pos == argc ? "return" : "parameter", pos == argc ? 0U : pos, ctx->name, ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->type_kind_get(type)); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    return LDG_ERR_AOK;
}

static uint32_t cmd_objects(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t start)
{
    void **elems = 0x0;
    uint32_t arr = 0x0;
    uint32_t cunt = 0x0;
    uint32_t idx = UINT32_MAX;
    uint32_t obj_idx = UINT32_MAX;
    uint32_t rows = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (ctx->il2cpp.meta.assignable(ctx->il2cpp.unity_class, klass) != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

    res = ctx->api->objs_hold(ctx->api->il2cpp, klass, &arr, &elems, &cunt);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (start > cunt) { res = LDG_ERR_RANGE; }

    for (idx = start; res == LDG_ERR_AOK && idx < cunt && rows < LIBRESTEAM_EXT_REPL_ROW_CAP; idx++)
    {
        if (!elems[idx] || ctx->api->obj_alive_is(ctx->api->il2cpp, elems[idx]) != LDG_TRUTH_TRUE) { continue; }

        res = libresteam_ext_repl_obj_hold(ctx, elems[idx], &obj_idx);
        if (res != LDG_ERR_AOK) { break; }

        res = ctx->api->obj_name_get(ctx->api->il2cpp, elems[idx], ctx->name, sizeof(ctx->name));
        if (res != LDG_ERR_AOK && res != LDG_ERR_STR_TRUNC) { res = ldg_str_rbrcpy(ctx->name, "-", sizeof(ctx->name)); }

        if (res == LDG_ERR_STR_TRUNC) { res = LDG_ERR_AOK; }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(ctx->name, ctx->detail, sizeof(ctx->detail)); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "object: %u32; ptr: %p; name: %s", obj_idx, elems[idx], ctx->detail); }

        rows++;
    }
    if (arr) { ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free(arr); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    return libresteam_ext_repl_emit(ctx, "found: %u32; rows: %u32; next start: %u32", cunt, rows, idx);
}

static uint32_t cmd_get(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t field_idx, uint32_t obj_idx)
{
    void *field = 0x0;
    void *obj = 0x0;
    void *type = 0x0;
    uint64_t data = 0x0;
    uint32_t size = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = libresteam_ext_repl_field_get(ctx, klass, field_idx, &field);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    type = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(field);
    res = libresteam_ext_repl_value_size_get(ctx, type, &size);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK || size > sizeof(data))) { return res != LDG_ERR_AOK ? res : LDG_ERR_BOUNDS; }

    if ((((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(field) & LIBRESTEAM_BACKENDS_IL2CPP_FIELD_STATIC) != 0)
    {
        if (obj_idx != UINT32_MAX) { return LDG_ERR_FUNC_ARG_INVALID; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_static_val_get(field, &data);
    }
    else
    {
        res = libresteam_ext_repl_obj_get(ctx, obj_idx, &obj);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (ctx->il2cpp.meta.assignable(klass, ctx->il2cpp.meta.obj_class(obj)) != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

        ctx->il2cpp.meta.field_rd(obj, field, &data);
    }

    return libresteam_ext_repl_value_print(ctx, type, &data);
}

uint32_t libresteam_ext_repl_il2cpp_complete(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    void *klass = 0x0;
    void *iter = 0x0;
    void *member = 0x0;
    const char *name = 0x0;
    char value[32] = LDG_ARR_ZERO_INIT;
    uint64_t len = 0x0;
    uint32_t idx = 0x0;
    uint32_t type_idx = UINT32_MAX;
    uint32_t field_idx = UINT32_MAX;
    uint32_t prefix_idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t methods = LDG_TRUTH_FALSE;
    uint8_t objects = LDG_TRUTH_FALSE;
    uint8_t enums = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!ctx || !args || argc < 2U)) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = libresteam_ext_repl_complete_begin(ctx, args[argc - 1U], LDG_TRUTH_TRUE);
    if (res != LDG_ERR_AOK) { return res; }

    if (!ctx->api || !ctx->api->il2cpp || !((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->ready || !ctx->il2cpp.meta_tried || ctx->il2cpp.meta_res != LDG_ERR_AOK) { return LDG_ERR_AOK; }

    objects = (argc == 2U && (libresteam_ext_repl_cmd_eq_is(args[0], "objtype") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "drop") == LDG_TRUTH_TRUE)) || (argc == 4U && (libresteam_ext_repl_cmd_eq_is(args[0], "call") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "get") == LDG_TRUTH_TRUE)) || (argc == 5U && libresteam_ext_repl_cmd_eq_is(args[0], "set") == LDG_TRUTH_TRUE);
    if (objects)
    {
        if (libresteam_ext_repl_cmd_eq_is(args[0], "drop") == LDG_TRUTH_TRUE) { res = libresteam_ext_repl_complete_add(ctx, "all", "release all retained objects"); }

        if (libresteam_ext_repl_cmd_eq_is(args[0], "call") == LDG_TRUTH_TRUE) { res = libresteam_ext_repl_complete_add(ctx, "-", "static call"); }

        for (idx = 0; res == LDG_ERR_AOK && idx < ctx->il2cpp.obj_next && !ctx->complete_more; idx++)
        {
            if (!ctx->il2cpp.objs[idx].handle) { continue; }

            res = ldg_str_fmt(value, sizeof(value), &len, "%u32", idx);
            if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_complete_add(ctx, value, "retained object"); }
        }
        return res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "types") == LDG_TRUTH_TRUE)
    {
        if (argc == 2U) { res = libresteam_ext_repl_complete_add(ctx, "find", ""); }

        if (res == LDG_ERR_AOK && argc <= 3U) { res = libresteam_ext_repl_complete_add(ctx, "*", "all indexed types"); }

        return res;
    }

    if (argc == 2U)
    {
        if (libresteam_ext_repl_cmd_eq_is(args[0], "type") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "fields") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "methods") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "method") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "objects") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "get") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "fieldtype") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "enum") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "set") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "call") != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        for (idx = 0; res == LDG_ERR_AOK && idx < ctx->il2cpp.class_cunt && !ctx->complete_more; idx++)
        {
            res = ldg_str_fmt(value, sizeof(value), &len, "%u32", idx);
            if (res != LDG_ERR_AOK) { break; }

            for (prefix_idx = 0; ctx->complete_prefix[prefix_idx] && ctx->complete_prefix[prefix_idx] == value[prefix_idx]; prefix_idx++) {}
            if (ctx->complete_prefix[prefix_idx]) { continue; }

            name = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_name_get(ctx->il2cpp.classes[idx]);
            res = libresteam_ext_repl_complete_add(ctx, value, name ? name : "");
        }
        return res;
    }

    res = cmd_idx_get(args[1], &type_idx);
    if (res != LDG_ERR_AOK || type_idx >= ctx->il2cpp.class_cunt) { return LDG_ERR_AOK; }

    klass = ctx->il2cpp.classes[type_idx];
    methods = libresteam_ext_repl_cmd_eq_is(args[0], "call") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "method") == LDG_TRUTH_TRUE;
    enums = argc == 4U && libresteam_ext_repl_cmd_eq_is(args[0], "set") == LDG_TRUTH_TRUE;
    if (enums)
    {
        res = cmd_idx_get(args[2], &field_idx);
        if (res != LDG_ERR_AOK || field_idx >= LIBRESTEAM_EXT_REPL_COMPLETE_VISIT_CAP) { return LDG_ERR_AOK; }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_field_get(ctx, klass, field_idx, &member); }

        if (res != LDG_ERR_AOK) { return LDG_ERR_AOK; }

        klass = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(member));
        if (!klass || ctx->il2cpp.meta.class_enum(klass) != LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }
    }
    else if (argc != 3U || (!methods && libresteam_ext_repl_cmd_eq_is(args[0], "get") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "set") != LDG_TRUTH_TRUE && libresteam_ext_repl_cmd_eq_is(args[0], "fieldtype") != LDG_TRUTH_TRUE)) { return LDG_ERR_AOK; }

    for (idx = 0; idx < LIBRESTEAM_EXT_REPL_COMPLETE_VISIT_CAP && !ctx->complete_more; idx++)
    {
        member = methods ? ctx->il2cpp.meta.methods(klass, &iter) : ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_fields_get(klass, &iter);
        if (!member) { break; }

        name = methods ? ctx->il2cpp.meta.method_name(member) : ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_name_get(member);
        if (enums)
        {
            if ((((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_flags_get(member) & CMD_FIELD_LITERAL) == 0 || !name) { continue; }

            res = libresteam_ext_repl_complete_add(ctx, name, "enum member");
        }
        else
        {
            res = ldg_str_fmt(value, sizeof(value), &len, "%u32", idx);
            if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_complete_add(ctx, value, name ? name : ""); }
        }

        if (res != LDG_ERR_AOK) { return res; }
    }
    if (idx == LIBRESTEAM_EXT_REPL_COMPLETE_VISIT_CAP && !ctx->complete_more) { ctx->complete_more = 1; return libresteam_ext_repl_emit(ctx, "!"); }

    return res;
}

uint32_t libresteam_ext_repl_il2cpp_cmd(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = (libresteam_ext_repl_ctx_t *)user;
    void *obj = 0x0;
    void *field = 0x0;
    void *type = 0x0;
    uint32_t type_idx = UINT32_MAX;
    uint32_t member_idx = 0x0;
    uint32_t obj_idx = UINT32_MAX;
    uint32_t idx = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !args || !argc)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE) { return argc == 1U ? cmd_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "status") == LDG_TRUTH_TRUE)
    {
        if (argc != 1U) { return LDG_ERR_FUNC_ARG_INVALID; }

        return libresteam_ext_repl_emit(ctx, "il2cpp ready: %u32; failed: %u32; metadata res: %u32; indexed: %u32; index done: %u32; object IDs used: %u32; execution thread: %u32", (uint32_t)(ctx->api->il2cpp && ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->ready), (uint32_t)(!ctx->api->il2cpp || ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->failed), ctx->il2cpp.meta_res, ctx->il2cpp.class_cunt, (uint32_t)ctx->il2cpp.index_done, ctx->il2cpp.obj_next, ctx->main_thread);
    }

    if (!ctx->api->il2cpp || !((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->ready || !ctx->il2cpp.meta_tried)
    {
        res = libresteam_ext_repl_emit(ctx, "IL2CPP backend unavailable or not ready; native commands remain available. Use backends or help native.");
        return res == LDG_ERR_AOK ? LDG_ERR_NOT_INIT : res;
    }

    if (ctx->il2cpp.meta_res != LDG_ERR_AOK) { return ctx->il2cpp.meta_res; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "drop") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (libresteam_ext_repl_cmd_eq_is(args[1], "all") == LDG_TRUTH_TRUE)
        {
            for (idx = 0; idx < ctx->il2cpp.obj_next; idx++) { if (ctx->il2cpp.objs[idx].handle) { ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free(ctx->il2cpp.objs[idx].handle); ctx->il2cpp.objs[idx].handle = 0; } }
            return libresteam_ext_repl_emit(ctx, "released all retained objects; IDs are not reused");
        }

        res = cmd_idx_get(args[1], &obj_idx);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (obj_idx >= ctx->il2cpp.obj_next || !ctx->il2cpp.objs[obj_idx].handle) { return LDG_ERR_NOT_FOUND; }

        ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->gchandle_free(ctx->il2cpp.objs[obj_idx].handle);
        ctx->il2cpp.objs[obj_idx].handle = 0;
        return libresteam_ext_repl_emit(ctx, "released object: %u32", obj_idx);
    }

    if (!ctx->il2cpp.index_done) { return LDG_ERR_AGAIN; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "objtype") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = cmd_idx_get(args[1], &obj_idx);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_obj_get(ctx, obj_idx, &obj); }

        return res == LDG_ERR_AOK ? cmd_class_type(ctx, ctx->il2cpp.meta.obj_class(obj)) : res;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "types") == LDG_TRUTH_TRUE)
    {
        idx = argc > 1U && libresteam_ext_repl_cmd_eq_is(args[1], "find") == LDG_TRUTH_TRUE ? 2U : 1U;
        if (argc != idx + 1U && argc != idx + 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = ldg_str_rbrcpy(ctx->il2cpp.filter, args[idx], sizeof(ctx->il2cpp.filter));
        if (res == LDG_ERR_AOK && argc == idx + 2U) { res = cmd_idx_get(args[idx + 1U], &member_idx); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (member_idx > ctx->il2cpp.class_cunt) { return LDG_ERR_RANGE; }

        ctx->il2cpp.search_idx = member_idx;
        ctx->il2cpp.search_rows = 0;
        ctx->searching = LDG_TRUTH_TRUE;
        return LDG_ERR_AOK;
    }

    if (argc < 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = cmd_idx_get(args[1], &type_idx);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (type_idx >= ctx->il2cpp.class_cunt) { return LDG_ERR_RANGE; }

    if (argc >= 3U) { res = cmd_idx_get(args[2], &member_idx); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "set") == LDG_TRUTH_TRUE)
    {
        if (argc != 4U && argc != 5U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (argc == 5U) { res = cmd_idx_get(args[4], &obj_idx); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return libresteam_ext_repl_enum_set(ctx, ctx->il2cpp.classes[type_idx], member_idx, args[3], obj_idx);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "call") == LDG_TRUTH_TRUE)
    {
        if (argc < 4U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (libresteam_ext_repl_cmd_eq_is(args[3], "-") != LDG_TRUTH_TRUE) { res = cmd_idx_get(args[3], &obj_idx); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        return libresteam_ext_repl_call(ctx, ctx->il2cpp.classes[type_idx], member_idx, obj_idx, argc - 4U, args + 4);
    }

    if (argc > 4U) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (res == LDG_ERR_AOK && argc == 4U) { res = cmd_idx_get(args[3], &obj_idx); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "type") == LDG_TRUTH_TRUE && argc == 2U) { return cmd_type(ctx, type_idx); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "enum") == LDG_TRUTH_TRUE && argc <= 3U) { return cmd_enum(ctx, ctx->il2cpp.classes[type_idx], member_idx); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "fieldtype") == LDG_TRUTH_TRUE && argc == 3U)
    {
        res = libresteam_ext_repl_field_get(ctx, ctx->il2cpp.classes[type_idx], member_idx, &field);
        if (res != LDG_ERR_AOK) { return res; }

        type = ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->field_type_get(field);
        return type ? cmd_class_type(ctx, ((const libresteam_backends_il2cpp_t *)ctx->api->il2cpp)->class_from_type(type)) : LDG_ERR_INVALID;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "fields") == LDG_TRUTH_TRUE && argc <= 3U) { return cmd_members(ctx, ctx->il2cpp.classes[type_idx], member_idx, LDG_TRUTH_FALSE); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "methods") == LDG_TRUTH_TRUE && argc <= 3U) { return cmd_members(ctx, ctx->il2cpp.classes[type_idx], member_idx, LDG_TRUTH_TRUE); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "method") == LDG_TRUTH_TRUE && argc == 3U) { return cmd_method(ctx, ctx->il2cpp.classes[type_idx], member_idx); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "objects") == LDG_TRUTH_TRUE && argc <= 3U) { return cmd_objects(ctx, ctx->il2cpp.classes[type_idx], member_idx); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "get") == LDG_TRUTH_TRUE && argc >= 3U) { return cmd_get(ctx, ctx->il2cpp.classes[type_idx], member_idx, obj_idx); }

    return LDG_ERR_FUNC_ARG_INVALID;
}
