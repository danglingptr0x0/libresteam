#include <float.h>
#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/mem/alloc.h>
#include <dangling/mem/secure.h>
#include <dangling/str/str.h>
#include <dangling/sys/mod.h>

#include <libresteam/ext/repl/expr.h>
#include <libresteam/ext/repl/repl.h>
#include <libresteam/fp32/fp32.h>

#define JAVA_MOD_NAME_PE "jvm.dll"
#define JAVA_MOD_NAME_ELF "libjvm.so"
#define JAVA_VM_LIST_SYM "JNI_GetCreatedJavaVMs"
#define JAVA_VM_CAP 1U
#define JAVA_JNI_VERSION 0x00010006U
#define JAVA_TI_VERSION 0x30010200U
#define JAVA_JNI_DETACHED 0xFFFFFFFEU
#define JAVA_VM_SLOT_ENV_GET 6U
#define JAVA_VM_SLOT_DAEMON_ATTACH 7U
#define JAVA_JNI_SLOT_LOCAL_REF_DROP 23U
#define JAVA_TI_SLOT_DEALLOC 46U
#define JAVA_TI_SLOT_CLASS_SIG 47U
#define JAVA_TI_SLOT_CLASS_METHODS 51U
#define JAVA_TI_SLOT_CLASS_FIELDS 52U
#define JAVA_TI_SLOT_FIELD_NAME 59U
#define JAVA_TI_SLOT_FIELD_MODS 61U
#define JAVA_TI_SLOT_METHOD_NAME 63U
#define JAVA_TI_SLOT_METHOD_MODS 65U
#define JAVA_TI_SLOT_CLASSES_GET 77U
#define JAVA_ACC_STATIC 0x0008U
#define JAVA_SIG_CLASS 'L'
#define JAVA_SIG_END ';'
#define JAVA_SIG_SEP '/'
#define JAVA_NAME_SEP '.'
#define JAVA_ARGC_CMD 1U
#define JAVA_ARGC_CMD_ARG 2U
#define JAVA_ARG_IDX 1U
#define JAVA_ARGC_CMD_ARG_PAIR 3U
#define JAVA_ARG_NEXT_IDX 2U
#define JAVA_OBJ_CAP LIBRESTEAM_EXT_REPL_OBJ_CAP
#define JAVA_JNI_SLOT_SUPER_GET 10U
#define JAVA_JNI_SLOT_GLOBAL_REF_NEW 21U
#define JAVA_JNI_SLOT_GLOBAL_REF_DROP 22U
#define JAVA_JNI_SLOT_CLASS_GET 31U
#define JAVA_JNI_SLOT_FIELD_GET 95U
#define JAVA_JNI_SLOT_STATIC_FIELD_GET 145U
#define JAVA_JNI_SLOT_TEXT_GET 169U
#define JAVA_JNI_SLOT_TEXT_DROP 170U
#define JAVA_TI_SLOT_TAG_SET 106U
#define JAVA_TI_SLOT_TAGGED_GET 113U
#define JAVA_TI_SLOT_HEAP_WALK 115U
#define JAVA_TI_SLOT_CAPS_ADD 141U
#define JAVA_KIND_OBJ 0U
#define JAVA_KIND_BOOL 1U
#define JAVA_KIND_BYTE 2U
#define JAVA_KIND_CHAR 3U
#define JAVA_KIND_SHORT 4U
#define JAVA_KIND_INT 5U
#define JAVA_KIND_WIDE 6U
#define JAVA_KIND_FP32 7U
#define JAVA_KIND_FP64 8U
#define JAVA_BITS_BYTE 8U
#define JAVA_BITS_SHORT 16U
#define JAVA_BITS_INT 32U
#define JAVA_BITS_WIDE 64U
#define JAVA_SIGN_BITS 1U
#define JAVA_COMPLEMENT_STEP 1U
#define JAVA_TAG 0x4C5352504CU
#define JAVA_TAG_NONE 0U
#define JAVA_TAG_CUNT 1U
#define JAVA_CAPS_BYTES 16U
#define JAVA_CAP_TAG_OBJS 0x01U
#define JAVA_HEAP_CB_CUNT 16U
#define JAVA_HEAP_FILTER_NONE 0U
#define JAVA_VISIT_ABORT 0x8000U
#define JAVA_TEXT_CLASS "java.lang.String"
#define JAVA_TEXT_SIG "Ljava/lang/String;"
#define JAVA_TEXT_METHOD "toString"
#define JAVA_TEXT_METHOD_SIG "()Ljava/lang/String;"
#define JAVA_JNI_SLOT_EXC_GET 15U
#define JAVA_JNI_SLOT_EXC_CLEAR 17U
#define JAVA_JNI_SLOT_METHOD_ID_GET 33U
#define JAVA_JNI_SLOT_CALL 36U
#define JAVA_JNI_SLOT_FIELD_SET 104U
#define JAVA_JNI_SLOT_STATIC_CALL 116U
#define JAVA_JNI_SLOT_STATIC_FIELD_SET 154U
#define JAVA_JNI_SLOT_TEXT_NEW 167U
#define JAVA_JNI_SLOT_EXC_CK 228U
#define JAVA_CALL_STRIDE 3U
#define JAVA_KIND_VOID 9U
#define JAVA_PARAM_CAP LIBRESTEAM_EXT_REPL_PARAM_CAP
#define JAVA_SIG_PARAMS_OFF 1U
#define JAVA_SIG_CLOSE ')'
#define JAVA_SIG_VOID 'V'
#define JAVA_MATCH_SINGLE 1U
#define JAVA_ARGC_SET 4U
#define JAVA_ARG_LAST_IDX 3U

typedef uint32_t (*java_vm_list_func_t)(void **vms, uint32_t cap, uint32_t *cunt);
typedef uint32_t (*java_env_get_func_t)(void *vm, void **env, uint32_t version);
typedef uint32_t (*java_thread_attach_func_t)(void *vm, void **env, void *args);
typedef void (*java_ref_drop_func_t)(void *env, void *ref);
typedef uint32_t (*java_dealloc_func_t)(void *ti, void *mem);
typedef uint32_t (*java_classes_get_func_t)(void *ti, uint32_t *cunt, void ***classes);
typedef uint32_t (*java_sig_get_func_t)(void *ti, void *klass, char **sig, char **generic);
typedef uint32_t (*java_members_get_func_t)(void *ti, void *klass, uint32_t *cunt, void ***ids);
typedef uint32_t (*java_field_name_func_t)(void *ti, void *klass, void *field, char **name, char **sig, char **generic);
typedef uint32_t (*java_field_mods_func_t)(void *ti, void *klass, void *field, uint32_t *mods);
typedef uint32_t (*java_method_name_func_t)(void *ti, void *method, char **name, char **sig, char **generic);
typedef uint32_t (*java_method_mods_func_t)(void *ti, void *method, uint32_t *mods);
typedef void *(*java_ref_get_func_t)(void *env, void *obj);
typedef void *(*java_obj_get_func_t)(void *env, void *tgt, void *field);
typedef uint64_t (*java_word_get_func_t)(void *env, void *tgt, void *field);
typedef libresteam_fp32_t (*java_fp32_get_func_t)(void *env, void *tgt, void *field);
typedef double (*java_fp64_get_func_t)(void *env, void *tgt, void *field);
typedef const char *(*java_text_get_func_t)(void *env, void *text, uint8_t *copied);
typedef void (*java_text_drop_func_t)(void *env, void *text, const char *chars);
typedef uint32_t (*java_tag_set_func_t)(void *ti, void *obj, uint64_t tag);
typedef uint32_t (*java_tagged_get_func_t)(void *ti, uint32_t tag_cunt, const uint64_t *tags, uint32_t *cunt, void ***objs, uint64_t **obj_tags);
typedef uint32_t (*java_heap_walk_func_t)(void *ti, uint32_t filter, void *klass, const void *cbs, const void *user);
typedef uint32_t (*java_caps_add_func_t)(void *ti, const uint8_t *caps);
typedef uint32_t (*java_heap_cb_t)(uint64_t class_tag, uint64_t size, uint64_t *tag, uint32_t len, void *user);
typedef void (*java_word_set_func_t)(void *env, void *tgt, void *field, uint64_t word);
typedef void (*java_fp32_set_func_t)(void *env, void *tgt, void *field, libresteam_fp32_t real);
typedef void (*java_fp64_set_func_t)(void *env, void *tgt, void *field, double real);
typedef uint64_t (*java_word_call_func_t)(void *env, void *tgt, void *method, const uint64_t *vals);
typedef libresteam_fp32_t (*java_fp32_call_func_t)(void *env, void *tgt, void *method, const uint64_t *vals);
typedef double (*java_fp64_call_func_t)(void *env, void *tgt, void *method, const uint64_t *vals);
typedef void *(*java_obj_call_func_t)(void *env, void *tgt, void *method, const uint64_t *vals);
typedef uint8_t (*java_exc_ck_func_t)(void *env);
typedef void *(*java_exc_get_func_t)(void *env);
typedef void (*java_exc_clear_func_t)(void *env);
typedef void *(*java_method_id_func_t)(void *env, void *klass, const char *name, const char *sig);
typedef void *(*java_text_new_func_t)(void *env, const char *chars);

typedef struct java_walk
{
    uint64_t bytes;
    uint64_t class_tag;
    uint32_t cunt;
    uint32_t max;
    uint32_t len;
    uint8_t pudding[4];
} java_walk_t;

typedef struct java_ctx
{
    ldg_sys_mod_t mod;
    libresteam_ext_repl_expr_t expr;
    void *vm;
    void *ti;
    void *objs[JAVA_OBJ_CAP];
    uint32_t bind_res;
    uint8_t bound;
    uint8_t attached;
    uint8_t caps_up;
    uint8_t pudding;
} java_ctx_t;

static uint32_t java_slot_get(void *obj, uint32_t idx, void *out)
{
    void ***iface = (void ***)obj;

    if (LDG_UNLIKELY(!obj || !out)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (LDG_UNLIKELY(!*iface)) { return LDG_ERR_NOT_INIT; }

    return ldg_mem_secure_cpy(out, &(*iface)[idx], sizeof(void *));
}

static uint32_t java_ti_res_get(libresteam_ext_repl_ctx_t *ctx, uint32_t code)
{
    uint32_t res = LDG_ERR_AOK;

    if (!code) { return LDG_ERR_AOK; }

    res = libresteam_ext_repl_emit(ctx, "jvmti error: %u32", code);
    return res == LDG_ERR_AOK ? LDG_ERR_INVALID : res;
}

static uint32_t java_mod_bind(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, const char *name, uint64_t *out)
{
    uint32_t res = LDG_ERR_AOK;

    *out = 0;
    res = ldg_sys_mod_find(&ctx->proc, name, &java->mod);
    if (res != LDG_ERR_AOK) { return res; }

    return ldg_sys_mod_sym_get(&ctx->proc, &java->mod, JAVA_VM_LIST_SYM, out);
}

static uint32_t java_bind(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java)
{
    java_vm_list_func_t list = 0x0;
    java_env_get_func_t env_get = 0x0;
    void *vm = 0x0;
    void *ti = 0x0;
    uint64_t addr = 0x0;
    uint32_t cunt = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (java->bound) { return LDG_ERR_AOK; }

    if (!ctx->proc_up) { return LDG_ERR_NOT_INIT; }

    res = java_mod_bind(ctx, java, JAVA_MOD_NAME_PE, &addr);
    if (res != LDG_ERR_AOK) { res = java_mod_bind(ctx, java, JAVA_MOD_NAME_ELF, &addr); }

    if (res != LDG_ERR_AOK) { return res; }

    if (!addr) { return LDG_ERR_NOT_FOUND; }

    res = ldg_mem_secure_cpy(&list, &addr, sizeof(list));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (list(&vm, JAVA_VM_CAP, &cunt) != 0 || !cunt || !vm) { return LDG_ERR_NOT_INIT; }

    res = java_slot_get(vm, JAVA_VM_SLOT_ENV_GET, &env_get);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (env_get(vm, &ti, JAVA_TI_VERSION) != 0 || !ti) { return LDG_ERR_UNSUPPORTED; }

    java->vm = vm;
    java->ti = ti;
    java->bound = LDG_TRUTH_TRUE;
    return LDG_ERR_AOK;
}

static uint32_t java_env_get(java_ctx_t *java, void **out)
{
    java_env_get_func_t env_get = 0x0;
    java_thread_attach_func_t attach = 0x0;
    uint32_t code = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = 0x0;
    res = java_slot_get(java->vm, JAVA_VM_SLOT_ENV_GET, &env_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->vm, JAVA_VM_SLOT_DAEMON_ATTACH, &attach); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    code = env_get(java->vm, out, JAVA_JNI_VERSION);
    if (code == JAVA_JNI_DETACHED)
    {
        code = attach(java->vm, out, 0x0);
        if (!code) { java->attached = LDG_TRUTH_TRUE; }
    }

    return !code && *out ? LDG_ERR_AOK : LDG_ERR_DENIED;
}

static uint32_t java_class_name_get(const char *sig, char *out, uint32_t cap)
{
    uint32_t src = 0x0;
    uint32_t dst = 0x0;
    uint8_t klass = LDG_TRUTH_FALSE;

    if (LDG_UNLIKELY(!sig || !out || !cap)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (sig[0] == JAVA_SIG_CLASS)
    {
        klass = LDG_TRUTH_TRUE;
        src++;
    }

    while (sig[src] != LDG_STR_TERM && dst + LDG_STR_TERM_SIZE < cap)
    {
        if (klass && sig[src] == JAVA_SIG_END && sig[src + LDG_STR_TERM_SIZE] == LDG_STR_TERM) { break; }

        out[dst++] = sig[src] == JAVA_SIG_SEP ? JAVA_NAME_SEP : sig[src];
        src++;
    }
    out[dst] = LDG_STR_TERM;
    return LDG_ERR_AOK;
}

static uint8_t java_text_has_is(const char *text, const char *part)
{
    uint32_t start = 0x0;
    uint32_t idx = 0x0;

    if (LDG_UNLIKELY(!text || !part)) { return LDG_ERR_FUNC_ARG_NULL; }

    for (start = 0;; start++)
    {
        idx = 0;
        while (part[idx] != LDG_STR_TERM && text[start + idx] == part[idx]) { idx++; }
        if (part[idx] == LDG_STR_TERM) { return LDG_TRUTH_TRUE; }

        if (text[start + idx] == LDG_STR_TERM) { return LDG_TRUTH_FALSE; }
    }
}

static uint32_t java_classes_drop(java_ctx_t *java, void *env, void **classes, uint32_t cunt, void *keep)
{
    java_ref_drop_func_t ref_drop = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!classes) { return LDG_ERR_AOK; }

    res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    for (idx = 0; idx < cunt; idx++) { if (classes[idx] && classes[idx] != keep) { ref_drop(env, classes[idx]); } }
    return dealloc(java->ti, classes) ? LDG_ERR_INVALID : LDG_ERR_AOK;
}

static uint32_t java_class_raw_get(java_ctx_t *java, void *klass, char *out, uint32_t cap)
{
    java_sig_get_func_t sig_get = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    char *sig = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = LDG_STR_TERM;
    res = java_slot_get(java->ti, JAVA_TI_SLOT_CLASS_SIG, &sig_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (sig_get(java->ti, klass, &sig, 0x0) != 0 || !sig) { return LDG_ERR_NOT_FOUND; }

    res = java_class_name_get(sig, out, cap);
    if (dealloc(java->ti, sig) != 0 && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    return res;
}

static uint32_t java_types(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *filter)
{
    char raw[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_classes_get_func_t classes_get = 0x0;
    void **classes = 0x0;
    uint32_t cunt = 0x0;
    uint32_t matched = 0x0;
    uint32_t shown = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t drop_res = LDG_ERR_AOK;

    res = java_slot_get(java->ti, JAVA_TI_SLOT_CLASSES_GET, &classes_get);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, classes_get(java->ti, &cunt, &classes));
    for (idx = 0; res == LDG_ERR_AOK && idx < cunt; idx++)
    {
        if (java_class_raw_get(java, classes[idx], raw, sizeof(raw)) != LDG_ERR_AOK) { continue; }

        if (filter && java_text_has_is(raw, filter) != LDG_TRUTH_TRUE) { continue; }

        matched++;
        if (shown == LIBRESTEAM_EXT_REPL_ROW_CAP) { continue; }

        res = libresteam_ext_repl_name_get(raw, ctx->name, sizeof(ctx->name));
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "class: %s", ctx->name); }

        shown++;
    }
    drop_res = java_classes_drop(java, env, classes, cunt, 0x0);
    if (res == LDG_ERR_AOK) { res = drop_res; }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "java types; classes: %u32; matched: %u32; shown: %u32", cunt, matched, shown) : res;
}

static uint32_t java_class_find(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, void **out)
{
    char raw[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_classes_get_func_t classes_get = 0x0;
    void **classes = 0x0;
    uint32_t cunt = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t drop_res = LDG_ERR_AOK;

    *out = 0x0;
    res = java_slot_get(java->ti, JAVA_TI_SLOT_CLASSES_GET, &classes_get);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, classes_get(java->ti, &cunt, &classes));
    for (idx = 0; res == LDG_ERR_AOK && idx < cunt && !*out; idx++)
    {
        if (java_class_raw_get(java, classes[idx], raw, sizeof(raw)) != LDG_ERR_AOK) { continue; }

        if (libresteam_ext_repl_cmd_eq_is(raw, name) == LDG_TRUTH_TRUE) { *out = classes[idx]; }
    }
    drop_res = java_classes_drop(java, env, classes, cunt, *out);
    if (res == LDG_ERR_AOK) { res = drop_res; }

    if (res != LDG_ERR_AOK) { return res; }

    return *out ? LDG_ERR_AOK : LDG_ERR_NOT_FOUND;
}

static uint32_t java_member_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, uint32_t idx, uint32_t mods, char *name, char *sig, uint8_t methods)
{
    java_dealloc_func_t dealloc = 0x0;
    uint32_t fixed = (mods & JAVA_ACC_STATIC) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    uint32_t res = LDG_ERR_AOK;

    res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = libresteam_ext_repl_name_get(name, ctx->name, sizeof(ctx->name));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(sig, ctx->detail, sizeof(ctx->detail)); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, methods ? "method: %u32; static: %u32; name: %s; sig: %s" : "field: %u32; static: %u32; name: %s; type: %s", idx, fixed, ctx->name, ctx->detail); }

    if (name && dealloc(java->ti, name) != 0 && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    if (sig && dealloc(java->ti, sig) != 0 && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    return res;
}

static uint32_t java_members_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *klass, uint8_t methods)
{
    java_members_get_func_t members_get = 0x0;
    java_field_name_func_t field_name = 0x0;
    java_field_mods_func_t field_mods = 0x0;
    java_method_name_func_t method_name = 0x0;
    java_method_mods_func_t method_mods = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    void **ids = 0x0;
    char *name = 0x0;
    char *sig = 0x0;
    uint32_t cunt = 0x0;
    uint32_t mods = 0x0;
    uint32_t code = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = java_slot_get(java->ti, methods ? JAVA_TI_SLOT_CLASS_METHODS : JAVA_TI_SLOT_CLASS_FIELDS, &members_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_FIELD_NAME, &field_name); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_FIELD_MODS, &field_mods); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_METHOD_NAME, &method_name); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_METHOD_MODS, &method_mods); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, members_get(java->ti, klass, &cunt, &ids));
    for (idx = 0; res == LDG_ERR_AOK && idx < cunt; idx++)
    {
        name = 0x0;
        sig = 0x0;
        mods = 0;
        code = methods ? method_name(java->ti, ids[idx], &name, &sig, 0x0) : field_name(java->ti, klass, ids[idx], &name, &sig, 0x0);
        if (!code) { code = methods ? method_mods(java->ti, ids[idx], &mods) : field_mods(java->ti, klass, ids[idx], &mods); }

        res = java_ti_res_get(ctx, code);
        if (res == LDG_ERR_AOK) { res = java_member_emit(ctx, java, idx, mods, name, sig, methods); }
    }
    if (ids && dealloc(java->ti, ids) != 0 && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, methods ? "java methods; declared: %u32" : "java fields; declared: %u32", cunt) : res;
}

static uint32_t java_members(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, uint8_t methods)
{
    java_ref_drop_func_t ref_drop = 0x0;
    void *klass = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop);
    if (res == LDG_ERR_AOK) { res = java_class_find(ctx, java, env, name, &klass); }

    if (res != LDG_ERR_AOK) { return res; }

    res = java_members_emit(ctx, java, klass, methods);
    ref_drop(env, klass);
    return res;
}

static uint32_t java_heap_cb(uint64_t class_tag, uint64_t size, uint64_t *tag, uint32_t len, void *user)
{
    java_walk_t *walk = (java_walk_t *)user;

    if (LDG_UNLIKELY(!walk || !tag)) { return JAVA_VISIT_ABORT; }

    if (walk->cunt == walk->max) { return JAVA_VISIT_ABORT; }

    *tag = JAVA_TAG;
    walk->bytes += size;
    walk->class_tag = class_tag;
    walk->len = len;
    walk->cunt++;
    return LDG_ERR_AOK;
}

static uint32_t java_obj_hold(java_ctx_t *java, void *env, void *obj, uint32_t *out)
{
    java_ref_get_func_t global_new = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *out = UINT32_MAX;
    res = java_slot_get(env, JAVA_JNI_SLOT_GLOBAL_REF_NEW, &global_new);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    while (idx < JAVA_OBJ_CAP && java->objs[idx]) { idx++; }
    if (idx == JAVA_OBJ_CAP) { return LDG_ERR_FULL; }

    java->objs[idx] = global_new(env, obj);
    if (!java->objs[idx]) { return LDG_ERR_ALLOC_NULL; }

    *out = idx;
    return LDG_ERR_AOK;
}

static uint32_t java_drop(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *text)
{
    java_ref_drop_func_t global_drop = 0x0;
    uint64_t num = 0x0;
    uint32_t idx = 0x0;
    uint32_t end = JAVA_OBJ_CAP;
    uint32_t dropped = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = java_slot_get(env, JAVA_JNI_SLOT_GLOBAL_REF_DROP, &global_drop);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(text, "all") != LDG_TRUTH_TRUE)
    {
        res = libresteam_ext_repl_num_get(text, &num);
        if (res != LDG_ERR_AOK) { return res; }

        if (num >= JAVA_OBJ_CAP || !java->objs[num]) { return LDG_ERR_NOT_FOUND; }

        idx = (uint32_t)num;
        end = idx + JAVA_TAG_CUNT;
    }

    for (; idx < end; idx++)
    {
        if (!java->objs[idx]) { continue; }

        global_drop(env, java->objs[idx]);
        java->objs[idx] = 0x0;
        dropped++;
    }
    return libresteam_ext_repl_emit(ctx, "java drop; released: %u32", dropped);
}

static uint32_t java_caps_get(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java)
{
    uint8_t caps[JAVA_CAPS_BYTES] = LDG_ARR_ZERO_INIT;
    java_caps_add_func_t caps_add = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (java->caps_up) { return LDG_ERR_AOK; }

    res = java_slot_get(java->ti, JAVA_TI_SLOT_CAPS_ADD, &caps_add);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    caps[0] = JAVA_CAP_TAG_OBJS;
    res = java_ti_res_get(ctx, caps_add(java->ti, caps));
    if (res == LDG_ERR_AOK) { java->caps_up = LDG_TRUTH_TRUE; }

    return res;
}

static uint32_t java_tagged_hold(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, uint32_t *held)
{
    java_tagged_get_func_t tagged_get = 0x0;
    java_tag_set_func_t tag_set = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    void **objs = 0x0;
    uint64_t tag = JAVA_TAG;
    uint32_t cunt = 0x0;
    uint32_t handle = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t hold_res = LDG_ERR_AOK;

    *held = 0;
    res = java_slot_get(java->ti, JAVA_TI_SLOT_TAGGED_GET, &tagged_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_TAG_SET, &tag_set); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, tagged_get(java->ti, JAVA_TAG_CUNT, &tag, &cunt, &objs, 0x0));
    if (res != LDG_ERR_AOK) { return res; }

    for (idx = 0; idx < cunt; idx++)
    {
        if (tag_set(java->ti, objs[idx], JAVA_TAG_NONE) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

        if (hold_res == LDG_ERR_AOK) { hold_res = java_obj_hold(java, env, objs[idx], &handle); }

        if (hold_res == LDG_ERR_AOK && res == LDG_ERR_AOK)
        {
            res = libresteam_ext_repl_emit(ctx, "object: %u32", handle);
            (*held)++;
        }

        ref_drop(env, objs[idx]);
    }
    if (objs && dealloc(java->ti, objs) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    if (res == LDG_ERR_AOK && hold_res == LDG_ERR_FULL) { res = libresteam_ext_repl_emit(ctx, "handle table full; release some with java drop"); }

    return res;
}

static uint32_t java_objects(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, uint32_t max)
{
    void *cbs[JAVA_HEAP_CB_CUNT] = LDG_ARR_ZERO_INIT;
    java_walk_t walk = LDG_STRUCT_ZERO_INIT;
    java_heap_walk_func_t heap_walk = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    java_heap_cb_t cb = java_heap_cb;
    void *klass = 0x0;
    uint32_t held = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = java_caps_get(ctx, java);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_HEAP_WALK, &heap_walk); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&cbs[0], &cb, sizeof(cb)); }

    if (res == LDG_ERR_AOK) { res = java_class_find(ctx, java, env, name, &klass); }

    if (res != LDG_ERR_AOK) { return res; }

    walk.max = max;
    res = java_ti_res_get(ctx, heap_walk(java->ti, JAVA_HEAP_FILTER_NONE, klass, cbs, &walk));
    ref_drop(env, klass);
    if (res == LDG_ERR_AOK) { res = java_tagged_hold(ctx, java, env, &held); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "java objects; found: %u32; held: %u32; bytes: %u64; limit reached: %u32", walk.cunt, held, walk.bytes, (uint32_t)(walk.cunt == max ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE)) : res;
}

static uint32_t java_field_find(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *klass, const char *name, void **field, uint32_t *mods, char *sig, uint32_t cap)
{
    java_members_get_func_t fields_get = 0x0;
    java_field_name_func_t field_name = 0x0;
    java_field_mods_func_t field_mods = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    void **ids = 0x0;
    char *found_name = 0x0;
    char *found_sig = 0x0;
    uint32_t cunt = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *field = 0x0;
    *mods = 0;
    res = java_slot_get(java->ti, JAVA_TI_SLOT_CLASS_FIELDS, &fields_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_FIELD_NAME, &field_name); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_FIELD_MODS, &field_mods); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, fields_get(java->ti, klass, &cunt, &ids));
    for (idx = 0; res == LDG_ERR_AOK && idx < cunt && !*field; idx++)
    {
        found_name = 0x0;
        found_sig = 0x0;
        if (field_name(java->ti, klass, ids[idx], &found_name, &found_sig, 0x0) != LDG_ERR_AOK) { continue; }

        if (found_name && found_sig && libresteam_ext_repl_cmd_eq_is(found_name, name) == LDG_TRUTH_TRUE)
        {
            res = ldg_str_rbrcpy(sig, found_sig, cap);
            if (res == LDG_ERR_AOK) { res = java_ti_res_get(ctx, field_mods(java->ti, klass, ids[idx], mods)); }

            if (res == LDG_ERR_AOK) { *field = ids[idx]; }
        }

        if (found_name && dealloc(java->ti, found_name) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

        if (found_sig && dealloc(java->ti, found_sig) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }
    }
    if (ids && dealloc(java->ti, ids) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    if (res != LDG_ERR_AOK) { return res; }

    return *field ? LDG_ERR_AOK : LDG_ERR_NOT_FOUND;
}

static uint32_t java_kind_get(char sig, uint32_t *kind, uint32_t *bits, uint8_t *sign)
{
    *kind = JAVA_KIND_OBJ;
    *bits = JAVA_BITS_WIDE;
    *sign = LDG_TRUTH_TRUE;
    if (sig == 'L' || sig == '[') { return LDG_ERR_AOK; }

    if (sig == 'Z') { *kind = JAVA_KIND_BOOL; *bits = JAVA_BITS_BYTE; *sign = LDG_TRUTH_FALSE; }
    else if (sig == 'B') { *kind = JAVA_KIND_BYTE; *bits = JAVA_BITS_BYTE; }
    else if (sig == 'C') { *kind = JAVA_KIND_CHAR; *bits = JAVA_BITS_SHORT; *sign = LDG_TRUTH_FALSE; }
    else if (sig == 'S') { *kind = JAVA_KIND_SHORT; *bits = JAVA_BITS_SHORT; }
    else if (sig == 'I') { *kind = JAVA_KIND_INT; *bits = JAVA_BITS_INT; }
    else if (sig == 'J') { *kind = JAVA_KIND_WIDE; }
    else if (sig == 'F') { *kind = JAVA_KIND_FP32; }
    else if (sig == 'D') { *kind = JAVA_KIND_FP64; }
    else { return LDG_ERR_UNSUPPORTED; }

    return LDG_ERR_AOK;
}

static uint32_t java_ref_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, void *ref)
{
    char raw[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_ref_get_func_t class_get = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    java_text_get_func_t text_get = 0x0;
    java_text_drop_func_t text_drop = 0x0;
    const char *chars = 0x0;
    void *klass = 0x0;
    uint32_t handle = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (!ref) { return libresteam_ext_repl_emit(ctx, "name: %s; null; type: %s", name, ctx->name); }

    res = java_slot_get(env, JAVA_JNI_SLOT_CLASS_GET, &class_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_TEXT_GET, &text_get); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_TEXT_DROP, &text_drop); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    klass = class_get(env, ref);
    if (!klass)
    {
        ref_drop(env, ref);
        return LDG_ERR_INVALID;
    }

    res = java_class_raw_get(java, klass, raw, sizeof(raw));
    ref_drop(env, klass);
    if (res == LDG_ERR_AOK && libresteam_ext_repl_cmd_eq_is(raw, JAVA_TEXT_CLASS) == LDG_TRUTH_TRUE)
    {
        chars = text_get(env, ref, 0x0);
        res = libresteam_ext_repl_name_get(chars, ctx->detail, sizeof(ctx->detail));
        if (chars) { text_drop(env, ref, chars); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "name: %s; text: %s; type: %s", name, ctx->detail, ctx->name); }
    }
    else if (res == LDG_ERR_AOK)
    {
        res = libresteam_ext_repl_name_get(raw, ctx->detail, sizeof(ctx->detail));
        if (res == LDG_ERR_AOK) { res = java_obj_hold(java, env, ref, &handle); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "name: %s; object: %u32; class: %s; type: %s", name, handle, ctx->detail, ctx->name); }
    }

    ref_drop(env, ref);
    return res;
}

static uint32_t java_word_emit(libresteam_ext_repl_ctx_t *ctx, const char *name, uint64_t word, uint32_t kind, uint32_t bits, uint8_t sign)
{
    uint64_t mask = UINT64_MAX >> (JAVA_BITS_WIDE - bits);
    uint64_t val = word & mask;
    uint64_t mag = val;
    uint8_t neg = LDG_TRUTH_FALSE;

    if (kind == JAVA_KIND_BOOL) { return libresteam_ext_repl_emit(ctx, "name: %s; flag: %s; type: %s", name, val ? "true" : "false", ctx->name); }

    if (sign && val > (mask >> JAVA_SIGN_BITS))
    {
        neg = LDG_TRUTH_TRUE;
        mag = (mask - val) + JAVA_COMPLEMENT_STEP;
    }

    return libresteam_ext_repl_emit(ctx, "name: %s; number: %s%u64; hex: 0x%x64; type: %s", name, neg ? "-" : "", mag, val, ctx->name);
}

static uint32_t java_val_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, const char *sig, void *tgt, void *field, uint8_t fixed)
{
    java_obj_get_func_t obj_get = 0x0;
    java_word_get_func_t word_get = 0x0;
    java_fp32_get_func_t fp32_get = 0x0;
    java_fp64_get_func_t fp64_get = 0x0;
    double real = 0;
    uint32_t slot = fixed ? JAVA_JNI_SLOT_STATIC_FIELD_GET : JAVA_JNI_SLOT_FIELD_GET;
    uint32_t kind = 0x0;
    uint32_t bits = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t sign = LDG_TRUTH_FALSE;

    res = java_kind_get(sig[0], &kind, &bits, &sign);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(sig, ctx->name, sizeof(ctx->name)); }

    if (res != LDG_ERR_AOK) { return res; }

    slot += kind;
    if (kind == JAVA_KIND_OBJ)
    {
        res = java_slot_get(env, slot, &obj_get);
        return res == LDG_ERR_AOK ? java_ref_emit(ctx, java, env, name, obj_get(env, tgt, field)) : res;
    }

    if (kind == JAVA_KIND_FP32)
    {
        res = java_slot_get(env, slot, &fp32_get);
        if (res == LDG_ERR_AOK) { real = (double)fp32_get(env, tgt, field); }
    }
    else if (kind == JAVA_KIND_FP64)
    {
        res = java_slot_get(env, slot, &fp64_get);
        if (res == LDG_ERR_AOK) { real = fp64_get(env, tgt, field); }
    }
    else
    {
        res = java_slot_get(env, slot, &word_get);
        return res == LDG_ERR_AOK ? java_word_emit(ctx, name, word_get(env, tgt, field), kind, bits, sign) : res;
    }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "name: %s; real: %f; type: %s", name, real, ctx->name) : res;
}

static uint32_t java_get(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *tgt, const char *name)
{
    char sig[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_ref_get_func_t class_get = 0x0;
    java_ref_get_func_t super_get = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    void *obj = 0x0;
    void *cur = 0x0;
    void *next = 0x0;
    void *field = 0x0;
    uint64_t num = 0x0;
    uint32_t mods = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t fixed = LDG_TRUTH_FALSE;

    res = java_slot_get(env, JAVA_JNI_SLOT_CLASS_GET, &class_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_SUPER_GET, &super_get); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (libresteam_ext_repl_num_get(tgt, &num) == LDG_ERR_AOK)
    {
        if (num >= JAVA_OBJ_CAP || !java->objs[num]) { return LDG_ERR_NOT_FOUND; }

        obj = java->objs[num];
        cur = class_get(env, obj);
    }
    else
    {
        res = java_class_find(ctx, java, env, tgt, &cur);
        if (res != LDG_ERR_AOK) { return res; }
    }

    res = LDG_ERR_NOT_FOUND;
    while (cur)
    {
        res = java_field_find(ctx, java, cur, name, &field, &mods, sig, sizeof(sig));
        if (res != LDG_ERR_NOT_FOUND) { break; }

        next = super_get(env, cur);
        ref_drop(env, cur);
        cur = next;
    }
    if (!cur) { return res; }

    fixed = (mods & JAVA_ACC_STATIC) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    if (res == LDG_ERR_AOK && !fixed && !obj)
    {
        res = libresteam_ext_repl_emit(ctx, "instance field; pass an object handle from java objects, not a class name");
        if (res == LDG_ERR_AOK) { res = LDG_ERR_FUNC_ARG_INVALID; }
    }
    else if (res == LDG_ERR_AOK) { res = java_val_emit(ctx, java, env, name, sig, fixed ? cur : obj, field, fixed); }

    ref_drop(env, cur);
    return res;
}

static uint32_t java_tgt_get(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *tgt, void **obj, void **klass)
{
    java_ref_get_func_t class_get = 0x0;
    uint64_t num = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *obj = 0x0;
    *klass = 0x0;
    if (libresteam_ext_repl_num_get(tgt, &num) != LDG_ERR_AOK) { return java_class_find(ctx, java, env, tgt, klass); }

    if (num >= JAVA_OBJ_CAP || !java->objs[num]) { return LDG_ERR_NOT_FOUND; }

    res = java_slot_get(env, JAVA_JNI_SLOT_CLASS_GET, &class_get);
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    *obj = java->objs[num];
    *klass = class_get(env, *obj);
    return *klass ? LDG_ERR_AOK : LDG_ERR_INVALID;
}

static uint8_t java_prefix_is(const char *text, const char *prefix)
{
    uint32_t idx = 0x0;

    if (LDG_UNLIKELY(!text || !prefix)) { return LDG_ERR_FUNC_ARG_NULL; }

    while (prefix[idx] != LDG_STR_TERM && text[idx] == prefix[idx]) { idx++; }
    return prefix[idx] == LDG_STR_TERM ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
}

static uint32_t java_param_next(const char *sig, uint32_t *pos, uint32_t *kind, uint32_t *bits, uint8_t *sign, uint8_t *text)
{
    uint32_t idx = *pos;

    *text = LDG_TRUTH_FALSE;
    if (sig[idx] == JAVA_SIG_CLOSE || sig[idx] == LDG_STR_TERM) { return LDG_ERR_EOF; }

    if (sig[idx] != 'L' && sig[idx] != '[')
    {
        *pos = idx + LDG_STR_TERM_SIZE;
        return java_kind_get(sig[idx], kind, bits, sign);
    }

    *kind = JAVA_KIND_OBJ;
    *bits = JAVA_BITS_WIDE;
    *sign = LDG_TRUTH_FALSE;
    if (java_prefix_is(sig + idx, JAVA_TEXT_SIG) == LDG_TRUTH_TRUE) { *text = LDG_TRUTH_TRUE; }

    while (sig[idx] == '[') { idx++; }
    if (sig[idx] == 'L') { while (sig[idx] != LDG_STR_TERM && sig[idx] != JAVA_SIG_END) { idx++; } }

    if (sig[idx] == LDG_STR_TERM) { return LDG_ERR_STR_INVALID; }

    *pos = idx + LDG_STR_TERM_SIZE;
    return LDG_ERR_AOK;
}

static uint32_t java_param_cunt_get(const char *sig, uint32_t *out)
{
    uint32_t pos = JAVA_SIG_PARAMS_OFF;
    uint32_t kind = 0x0;
    uint32_t bits = 0x0;
    uint32_t cunt = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t sign = LDG_TRUTH_FALSE;
    uint8_t text = LDG_TRUTH_FALSE;

    *out = UINT32_MAX;
    if (sig[0] != '(') { return LDG_ERR_STR_INVALID; }

    while ((res = java_param_next(sig, &pos, &kind, &bits, &sign, &text)) == LDG_ERR_AOK) { cunt++; }
    if (res != LDG_ERR_EOF) { return res; }

    *out = cunt;
    return LDG_ERR_AOK;
}

static uint32_t java_word_parse(const char *text, uint32_t bits, uint8_t sign, uint64_t *out)
{
    uint64_t mask = UINT64_MAX >> (JAVA_BITS_WIDE - bits);
    uint64_t limit = sign ? mask >> JAVA_SIGN_BITS : mask;
    uint64_t mag = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t neg = LDG_TRUTH_FALSE;

    *out = UINT64_MAX;
    if (text[0] == '-')
    {
        if (!sign) { return LDG_ERR_RANGE; }

        neg = LDG_TRUTH_TRUE;
        limit += JAVA_COMPLEMENT_STEP;
        text++;
    }

    res = libresteam_ext_repl_num_get(text, &mag);
    if (res != LDG_ERR_AOK) { return res; }

    if (mag > limit) { return LDG_ERR_RANGE; }

    *out = neg ? ((mask - mag) + JAVA_COMPLEMENT_STEP) & mask : mag;
    return LDG_ERR_AOK;
}

static uint32_t java_real_parse(java_ctx_t *java, const char *text, double *out)
{
    libresteam_ext_repl_expr_env_t expr_env = LDG_STRUCT_ZERO_INIT;
    libresteam_ext_repl_expr_val_t val = LDG_STRUCT_ZERO_INIT;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    for (idx = 0; text[idx] != LDG_STR_TERM; idx++) { if (ldg_char_digit_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE && text[idx] != '.' && text[idx] != '-' && text[idx] != '+' && text[idx] != 'e' && text[idx] != 'E') { return LDG_ERR_FUNC_ARG_INVALID; } }
    res = libresteam_ext_repl_expr_compile(&java->expr, text);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_expr_eval(&java->expr, &expr_env, &val); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_expr_real_get(&val, out) : res;
}

static uint32_t java_arg_get(java_ctx_t *java, void *env, uint32_t kind, uint32_t bits, uint8_t sign, uint8_t is_text, const char *text, uint64_t *word, void **local)
{
    java_text_new_func_t text_new = 0x0;
    libresteam_fp32_t narrow = 0;
    void *ref = 0x0;
    double real = 0;
    uint64_t num = 0x0;
    uint32_t res = LDG_ERR_AOK;

    *word = JAVA_TAG_NONE;
    *local = 0x0;
    if (kind == JAVA_KIND_OBJ)
    {
        if (libresteam_ext_repl_cmd_eq_is(text, "null") == LDG_TRUTH_TRUE) { return LDG_ERR_AOK; }

        if (is_text)
        {
            res = java_slot_get(env, JAVA_JNI_SLOT_TEXT_NEW, &text_new);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            *local = text_new(env, text);
            if (!*local) { return LDG_ERR_ALLOC_NULL; }

            ref = *local;
        }
        else
        {
            res = libresteam_ext_repl_num_get(text, &num);
            if (res != LDG_ERR_AOK) { return res; }

            if (num >= JAVA_OBJ_CAP || !java->objs[num]) { return LDG_ERR_NOT_FOUND; }

            ref = java->objs[num];
        }

        return ldg_mem_secure_cpy(word, &ref, sizeof(ref));
    }

    if (kind == JAVA_KIND_BOOL)
    {
        if (libresteam_ext_repl_cmd_eq_is(text, "true") == LDG_TRUTH_TRUE) { *word = LDG_TRUTH_TRUE; }
        else if (libresteam_ext_repl_cmd_eq_is(text, "false") != LDG_TRUTH_TRUE) { return LDG_ERR_FUNC_ARG_INVALID; }

        return LDG_ERR_AOK;
    }

    if (kind != JAVA_KIND_FP32 && kind != JAVA_KIND_FP64) { return java_word_parse(text, bits, sign, word); }

    res = java_real_parse(java, text, &real);
    if (res != LDG_ERR_AOK) { return res; }

    if (kind == JAVA_KIND_FP64) { return ldg_mem_secure_cpy(word, &real, sizeof(real)); }

    if (real > FLT_MAX || real < -FLT_MAX) { return LDG_ERR_RANGE; }

    narrow = (libresteam_fp32_t)real;
    return ldg_mem_secure_cpy(word, &narrow, sizeof(narrow));
}

static uint32_t java_exc_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env)
{
    char raw[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_exc_ck_func_t exc_ck = 0x0;
    java_exc_get_func_t exc_get = 0x0;
    java_exc_clear_func_t exc_clear = 0x0;
    java_ref_get_func_t class_get = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    java_method_id_func_t method_id = 0x0;
    java_obj_call_func_t obj_call = 0x0;
    java_text_get_func_t text_get = 0x0;
    java_text_drop_func_t text_drop = 0x0;
    const char *chars = 0x0;
    void *exc = 0x0;
    void *klass = 0x0;
    void *method = 0x0;
    void *text = 0x0;
    uint32_t res = LDG_ERR_AOK;

    res = java_slot_get(env, JAVA_JNI_SLOT_EXC_CK, &exc_ck);
    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_EXC_GET, &exc_get); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_EXC_CLEAR, &exc_clear); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_CLASS_GET, &class_get); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_METHOD_ID_GET, &method_id); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_CALL, &obj_call); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_TEXT_GET, &text_get); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_TEXT_DROP, &text_drop); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!exc_ck(env)) { return LDG_ERR_AOK; }

    exc = exc_get(env);
    exc_clear(env);
    if (!exc) { return LDG_ERR_INVALID; }

    klass = class_get(env, exc);
    if (klass)
    {
        res = java_class_raw_get(java, klass, raw, sizeof(raw));
        method = method_id(env, klass, JAVA_TEXT_METHOD, JAVA_TEXT_METHOD_SIG);
        if (method) { text = obj_call(env, exc, method, 0x0); }

        if (exc_ck(env))
        {
            exc_clear(env);
            text = 0x0;
        }

        ref_drop(env, klass);
    }

    if (text) { chars = text_get(env, text, 0x0); }

    res = libresteam_ext_repl_name_get(raw, ctx->name, sizeof(ctx->name));
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(chars, ctx->detail, sizeof(ctx->detail)); }

    if (chars) { text_drop(env, text, chars); }

    if (text) { ref_drop(env, text); }

    ref_drop(env, exc);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "java exception: %s; text: %s", ctx->name, ctx->detail); }

    return res == LDG_ERR_AOK ? LDG_ERR_INVALID : res;
}

static uint32_t java_set(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *tgt, const char *name, const char *text)
{
    char sig[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    java_ref_get_func_t super_get = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    java_word_set_func_t word_set = 0x0;
    java_fp32_set_func_t fp32_set = 0x0;
    java_fp64_set_func_t fp64_set = 0x0;
    libresteam_fp32_t narrow = 0;
    void *obj = 0x0;
    void *cur = 0x0;
    void *next = 0x0;
    void *field = 0x0;
    void *local = 0x0;
    void *tgt_ref = 0x0;
    double real = 0;
    uint64_t word = 0x0;
    uint32_t mods = 0x0;
    uint32_t kind = 0x0;
    uint32_t bits = 0x0;
    uint32_t slot = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t sign = LDG_TRUTH_FALSE;
    uint8_t fixed = LDG_TRUTH_FALSE;

    res = java_slot_get(env, JAVA_JNI_SLOT_SUPER_GET, &super_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (res == LDG_ERR_AOK) { res = java_tgt_get(ctx, java, env, tgt, &obj, &cur); }

    if (res != LDG_ERR_AOK) { return res; }

    res = LDG_ERR_NOT_FOUND;
    while (cur)
    {
        res = java_field_find(ctx, java, cur, name, &field, &mods, sig, sizeof(sig));
        if (res != LDG_ERR_NOT_FOUND) { break; }

        next = super_get(env, cur);
        ref_drop(env, cur);
        cur = next;
    }
    if (!cur) { return res; }

    fixed = (mods & JAVA_ACC_STATIC) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    tgt_ref = fixed ? cur : obj;
    if (res == LDG_ERR_AOK && !tgt_ref) { res = LDG_ERR_FUNC_ARG_INVALID; }

    if (res == LDG_ERR_AOK) { res = java_kind_get(sig[0], &kind, &bits, &sign); }

    if (res == LDG_ERR_AOK) { res = java_arg_get(java, env, kind, bits, sign, libresteam_ext_repl_cmd_eq_is(sig, JAVA_TEXT_SIG), text, &word, &local); }

    slot = (fixed ? JAVA_JNI_SLOT_STATIC_FIELD_SET : JAVA_JNI_SLOT_FIELD_SET) + kind;
    if (res == LDG_ERR_AOK && kind == JAVA_KIND_FP32)
    {
        res = java_slot_get(env, slot, &fp32_set);
        if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&narrow, &word, sizeof(narrow)); }

        if (res == LDG_ERR_AOK) { fp32_set(env, tgt_ref, field, narrow); }
    }
    else if (res == LDG_ERR_AOK && kind == JAVA_KIND_FP64)
    {
        res = java_slot_get(env, slot, &fp64_set);
        if (res == LDG_ERR_AOK) { res = ldg_mem_secure_cpy(&real, &word, sizeof(real)); }

        if (res == LDG_ERR_AOK) { fp64_set(env, tgt_ref, field, real); }
    }
    else if (res == LDG_ERR_AOK)
    {
        res = java_slot_get(env, slot, &word_set);
        if (res == LDG_ERR_AOK) { word_set(env, tgt_ref, field, word); }
    }

    if (local) { ref_drop(env, local); }

    ref_drop(env, cur);
    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_name_get(sig, ctx->name, sizeof(ctx->name)); }

    return res == LDG_ERR_AOK ? libresteam_ext_repl_emit(ctx, "name: %s; written; type: %s", name, ctx->name) : res;
}

static uint32_t java_method_find(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *klass, const char *tok, uint32_t argc, void **method, uint32_t *mods, char *sig, uint32_t cap)
{
    java_members_get_func_t methods_get = 0x0;
    java_method_name_func_t method_name = 0x0;
    java_method_mods_func_t method_mods = 0x0;
    java_dealloc_func_t dealloc = 0x0;
    void **ids = 0x0;
    char *found_name = 0x0;
    char *found_sig = 0x0;
    uint64_t name_len = 0x0;
    uint32_t cunt = 0x0;
    uint32_t params = 0x0;
    uint32_t matches = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t exact = LDG_TRUTH_FALSE;
    uint8_t take = LDG_TRUTH_FALSE;

    *method = 0x0;
    *mods = 0;
    res = java_slot_get(java->ti, JAVA_TI_SLOT_CLASS_METHODS, &methods_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_METHOD_NAME, &method_name); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_METHOD_MODS, &method_mods); }

    if (res == LDG_ERR_AOK) { res = java_slot_get(java->ti, JAVA_TI_SLOT_DEALLOC, &dealloc); }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = java_ti_res_get(ctx, methods_get(java->ti, klass, &cunt, &ids));
    for (idx = 0; res == LDG_ERR_AOK && idx < cunt && !exact; idx++)
    {
        found_name = 0x0;
        found_sig = 0x0;
        take = LDG_TRUTH_FALSE;
        if (method_name(java->ti, ids[idx], &found_name, &found_sig, 0x0) != LDG_ERR_AOK) { continue; }

        if (found_name && found_sig && java_prefix_is(tok, found_name) == LDG_TRUTH_TRUE)
        {
            res = ldg_str_len_get(found_name, cap, &name_len);
            if (res == LDG_ERR_AOK && libresteam_ext_repl_cmd_eq_is(tok + name_len, found_sig) == LDG_TRUTH_TRUE)
            {
                exact = LDG_TRUTH_TRUE;
                take = LDG_TRUTH_TRUE;
            }
            else if (res == LDG_ERR_AOK && tok[name_len] == LDG_STR_TERM && java_param_cunt_get(found_sig, &params) == LDG_ERR_AOK && params == argc)
            {
                matches++;
                take = matches == JAVA_MATCH_SINGLE ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
            }
        }

        if (res == LDG_ERR_AOK && take)
        {
            res = ldg_str_rbrcpy(sig, found_sig, cap);
            if (res == LDG_ERR_AOK) { res = java_ti_res_get(ctx, method_mods(java->ti, ids[idx], mods)); }

            if (res == LDG_ERR_AOK) { *method = ids[idx]; }
        }

        if (found_name && dealloc(java->ti, found_name) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

        if (found_sig && dealloc(java->ti, found_sig) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }
    }
    if (ids && dealloc(java->ti, ids) != LDG_ERR_AOK && res == LDG_ERR_AOK) { res = LDG_ERR_INVALID; }

    if (res != LDG_ERR_AOK) { return res; }

    if (!exact && matches > JAVA_MATCH_SINGLE)
    {
        res = libresteam_ext_repl_emit(ctx, "ambiguous method; append its signature exactly as java methods lists it, with no space");
        return res == LDG_ERR_AOK ? LDG_ERR_BUSY : res;
    }

    return *method ? LDG_ERR_AOK : LDG_ERR_NOT_FOUND;
}

static uint32_t java_ret_emit(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *name, const char *ret_sig, void *tgt_ref, void *method, const uint64_t *vals, uint8_t fixed)
{
    java_word_call_func_t word_call = 0x0;
    java_fp32_call_func_t fp32_call = 0x0;
    java_fp64_call_func_t fp64_call = 0x0;
    java_obj_call_func_t obj_call = 0x0;
    void *ref = 0x0;
    double real = 0;
    uint64_t word = 0x0;
    uint32_t kind = JAVA_KIND_VOID;
    uint32_t bits = 0x0;
    uint32_t slot = fixed ? JAVA_JNI_SLOT_STATIC_CALL : JAVA_JNI_SLOT_CALL;
    uint32_t res = LDG_ERR_AOK;
    uint8_t sign = LDG_TRUTH_FALSE;

    if (ret_sig[0] != JAVA_SIG_VOID) { res = java_kind_get(ret_sig[0], &kind, &bits, &sign); }

    if (res != LDG_ERR_AOK) { return res; }

    slot += kind * JAVA_CALL_STRIDE;
    if (kind == JAVA_KIND_OBJ)
    {
        res = java_slot_get(env, slot, &obj_call);
        if (res == LDG_ERR_AOK) { ref = obj_call(env, tgt_ref, method, vals); }
    }
    else if (kind == JAVA_KIND_FP32)
    {
        res = java_slot_get(env, slot, &fp32_call);
        if (res == LDG_ERR_AOK) { real = (double)fp32_call(env, tgt_ref, method, vals); }
    }
    else if (kind == JAVA_KIND_FP64)
    {
        res = java_slot_get(env, slot, &fp64_call);
        if (res == LDG_ERR_AOK) { real = fp64_call(env, tgt_ref, method, vals); }
    }
    else
    {
        res = java_slot_get(env, slot, &word_call);
        if (res == LDG_ERR_AOK) { word = word_call(env, tgt_ref, method, vals); }
    }

    if (res == LDG_ERR_AOK) { res = java_exc_emit(ctx, java, env); }

    if (res != LDG_ERR_AOK) { return res; }

    res = libresteam_ext_repl_name_get(ret_sig, ctx->name, sizeof(ctx->name));
    if (res != LDG_ERR_AOK) { return res; }

    if (kind == JAVA_KIND_VOID) { return libresteam_ext_repl_emit(ctx, "name: %s; done; type: %s", name, ctx->name); }

    if (kind == JAVA_KIND_OBJ) { return java_ref_emit(ctx, java, env, name, ref); }

    if (kind == JAVA_KIND_FP32 || kind == JAVA_KIND_FP64) { return libresteam_ext_repl_emit(ctx, "name: %s; real: %f; type: %s", name, real, ctx->name); }

    return java_word_emit(ctx, name, word, kind, bits, sign);
}

static uint32_t java_call(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java, void *env, const char *tgt, const char *name, uint32_t argc, char **args)
{
    char sig[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t vals[JAVA_PARAM_CAP] = LDG_ARR_ZERO_INIT;
    void *locals[JAVA_PARAM_CAP] = LDG_ARR_ZERO_INIT;
    java_ref_get_func_t super_get = 0x0;
    java_ref_drop_func_t ref_drop = 0x0;
    void *obj = 0x0;
    void *cur = 0x0;
    void *next = 0x0;
    void *method = 0x0;
    void *tgt_ref = 0x0;
    uint32_t mods = 0x0;
    uint32_t pos = JAVA_SIG_PARAMS_OFF;
    uint32_t kind = 0x0;
    uint32_t bits = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t sign = LDG_TRUTH_FALSE;
    uint8_t is_text = LDG_TRUTH_FALSE;
    uint8_t fixed = LDG_TRUTH_FALSE;

    if (name[0] == '<' || argc > JAVA_PARAM_CAP) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = java_slot_get(env, JAVA_JNI_SLOT_SUPER_GET, &super_get);
    if (res == LDG_ERR_AOK) { res = java_slot_get(env, JAVA_JNI_SLOT_LOCAL_REF_DROP, &ref_drop); }

    if (res == LDG_ERR_AOK) { res = java_tgt_get(ctx, java, env, tgt, &obj, &cur); }

    if (res != LDG_ERR_AOK) { return res; }

    res = LDG_ERR_NOT_FOUND;
    while (cur)
    {
        res = java_method_find(ctx, java, cur, name, argc, &method, &mods, sig, sizeof(sig));
        if (res != LDG_ERR_NOT_FOUND) { break; }

        next = super_get(env, cur);
        ref_drop(env, cur);
        cur = next;
    }
    if (!cur) { return res; }

    fixed = (mods & JAVA_ACC_STATIC) ? LDG_TRUTH_TRUE : LDG_TRUTH_FALSE;
    tgt_ref = fixed ? cur : obj;
    if (res == LDG_ERR_AOK && !tgt_ref)
    {
        res = libresteam_ext_repl_emit(ctx, "instance method; pass an object handle from java objects, not a class name");
        if (res == LDG_ERR_AOK) { res = LDG_ERR_FUNC_ARG_INVALID; }
    }

    for (idx = 0; res == LDG_ERR_AOK && idx < argc; idx++)
    {
        res = java_param_next(sig, &pos, &kind, &bits, &sign, &is_text);
        if (res == LDG_ERR_EOF) { res = LDG_ERR_FUNC_ARG_INVALID; }

        if (res == LDG_ERR_AOK) { res = java_arg_get(java, env, kind, bits, sign, is_text, args[idx], &vals[idx], &locals[idx]); }
    }
    if (res == LDG_ERR_AOK && sig[pos] != JAVA_SIG_CLOSE) { res = LDG_ERR_FUNC_ARG_INVALID; }

    if (res == LDG_ERR_AOK) { res = java_ret_emit(ctx, java, env, name, sig + pos + LDG_STR_TERM_SIZE, tgt_ref, method, vals, fixed); }

    for (idx = 0; idx < JAVA_PARAM_CAP; idx++) { if (locals[idx]) { ref_drop(env, locals[idx]); } }
    ref_drop(env, cur);
    return res;
}

static uint32_t java_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = libresteam_ext_repl_emit(ctx, "Java: existing HotSpot VM in this process, reached through JNI and JVMTI; binds on first use");

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "java status | java types [substring] | java fields <class> | java methods <class>"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "java objects <class> [limit] | java get <handle|class> <field> | java drop <handle|all>"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "objects holds live instances of exactly that class (not subclasses) as numbered handles; get reads a field by name, searching superclasses; a class name reads static fields only; an object-typed field becomes a new handle."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "java set <handle|class> <field> <value> | java call <handle|class> <method> [args...]"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Values: decimal or hex integers, true/false, decimal reals, null, a handle number for an object, one unspaced word for a String. call picks the method by name and argument total; when several match, append the signature as java methods lists it. Constructors are not callable. A thrown exception is cleared and printed."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "set and call change the running game from the REPL callback thread, not from a defined point in the game tick."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Class names are dotted runtime names (obfuscated where the game is); fields and methods list declared members only, not inherited ones."); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Commands run on the REPL callback thread; a thread unknown to the VM is attached as a daemon thread."); }

    return res;
}

static uint32_t java_status(libresteam_ext_repl_ctx_t *ctx, java_ctx_t *java)
{
    return libresteam_ext_repl_emit(ctx, "java; bound: %u32; bind res: %u32; module: %s; VM: 0x%x64; thread attached by REPL: %u32", (uint32_t)java->bound, java->bind_res, java->bound ? java->mod.name : "-", (uint64_t)(uintptr_t)java->vm, (uint32_t)java->attached);
}

uint32_t libresteam_ext_repl_java_init(libresteam_ext_repl_ctx_t *ctx)
{
    void *mem = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = ldg_mem_alloc(sizeof(java_ctx_t), &mem);
    if (res != LDG_ERR_AOK) { return res; }

    res = ldg_mem_secure_zero(mem, sizeof(java_ctx_t));
    if (res != LDG_ERR_AOK)
    {
        res = ldg_mem_dealloc(mem);
        if (res != LDG_ERR_AOK) { return res; }

        return LDG_ERR_NOT_INIT;
    }

    ctx->java = mem;
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_java_shutdown(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (!ctx->java) { return LDG_ERR_AOK; }

    res = ldg_mem_dealloc(ctx->java);
    ctx->java = 0x0;
    return res;
}

uint32_t libresteam_ext_repl_java_cmd(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    java_ctx_t *java = 0x0;
    void *env = 0x0;
    uint64_t max = LIBRESTEAM_EXT_REPL_ROW_CAP;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !args || !argc)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE) { return argc == JAVA_ARGC_CMD ? java_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (!ctx->java) { return LDG_ERR_NOT_INIT; }

    java = (java_ctx_t *)ctx->java;
    java->bind_res = java_bind(ctx, java);
    if (libresteam_ext_repl_cmd_eq_is(args[0], "status") == LDG_TRUTH_TRUE) { return argc == JAVA_ARGC_CMD ? java_status(ctx, java) : LDG_ERR_FUNC_ARG_INVALID; }

    if (java->bind_res != LDG_ERR_AOK) { return java->bind_res; }

    res = java_env_get(java, &env);
    if (res != LDG_ERR_AOK) { return res; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "types") == LDG_TRUTH_TRUE)
    {
        if (argc > JAVA_ARGC_CMD_ARG) { return LDG_ERR_FUNC_ARG_INVALID; }

        return java_types(ctx, java, env, argc == JAVA_ARGC_CMD_ARG ? args[JAVA_ARG_IDX] : 0x0);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "objects") == LDG_TRUTH_TRUE)
    {
        if (argc != JAVA_ARGC_CMD_ARG && argc != JAVA_ARGC_CMD_ARG_PAIR) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (argc == JAVA_ARGC_CMD_ARG_PAIR) { res = libresteam_ext_repl_num_get(args[JAVA_ARG_NEXT_IDX], &max); }

        if (res != LDG_ERR_AOK || !max || max > JAVA_OBJ_CAP) { return LDG_ERR_FUNC_ARG_INVALID; }

        return java_objects(ctx, java, env, args[JAVA_ARG_IDX], (uint32_t)max);
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "set") == LDG_TRUTH_TRUE) { return argc == JAVA_ARGC_SET ? java_set(ctx, java, env, args[JAVA_ARG_IDX], args[JAVA_ARG_NEXT_IDX], args[JAVA_ARG_LAST_IDX]) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "call") == LDG_TRUTH_TRUE) { return argc >= JAVA_ARGC_CMD_ARG_PAIR ? java_call(ctx, java, env, args[JAVA_ARG_IDX], args[JAVA_ARG_NEXT_IDX], argc - JAVA_ARGC_CMD_ARG_PAIR, args + JAVA_ARGC_CMD_ARG_PAIR) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "get") == LDG_TRUTH_TRUE) { return argc == JAVA_ARGC_CMD_ARG_PAIR ? java_get(ctx, java, env, args[JAVA_ARG_IDX], args[JAVA_ARG_NEXT_IDX]) : LDG_ERR_FUNC_ARG_INVALID; }

    if (argc != JAVA_ARGC_CMD_ARG) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "drop") == LDG_TRUTH_TRUE) { return java_drop(ctx, java, env, args[JAVA_ARG_IDX]); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "fields") == LDG_TRUTH_TRUE) { return java_members(ctx, java, env, args[JAVA_ARG_IDX], LDG_TRUTH_FALSE); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "methods") == LDG_TRUTH_TRUE) { return java_members(ctx, java, env, args[JAVA_ARG_IDX], LDG_TRUTH_TRUE); }

    return LDG_ERR_NOT_FOUND;
}
