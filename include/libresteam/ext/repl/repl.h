#ifndef LIBRESTEAM_EXT_REPL_H
#define LIBRESTEAM_EXT_REPL_H

#include <stdint.h>

#include <dangling/sys/proc.h>
#include <dangling/thread/pool.h>
#include <dangling/thread/spsc.h>

#include <libresteam/backends/il2cpp/il2cpp.h>
#include <libresteam/backends/java/java.h>
#include <libresteam/backends/lua/lua.h>
#include <libresteam/mod/mod.h>

#define LIBRESTEAM_EXT_REPL_CMD_CAP 256U
#define LIBRESTEAM_EXT_REPL_RESP_CAP 32768U
#define LIBRESTEAM_EXT_REPL_RESP_HDR_SIZE (sizeof(uint64_t) + 2U * sizeof(uint32_t))
#define LIBRESTEAM_EXT_REPL_CLASS_CAP 65536U
#define LIBRESTEAM_EXT_REPL_OBJ_CAP 256U
#define LIBRESTEAM_EXT_REPL_ROW_CAP 64U
#define LIBRESTEAM_EXT_REPL_BATCH 256U
#define LIBRESTEAM_EXT_REPL_QUEUE_CAP 4U
#define LIBRESTEAM_EXT_REPL_PARAM_CAP 8U
#define LIBRESTEAM_EXT_REPL_NAME_CAP 192U
#define LIBRESTEAM_EXT_REPL_COMPLETE_CAP 64U
#define LIBRESTEAM_EXT_REPL_COMPLETE_VISIT_CAP 4096U
#define LIBRESTEAM_EXT_REPL_PIPE_PREFIX "libresteam-repl-"


#define LIBRESTEAM_EXT_REPL_BACKEND_CAP 8U
#define LIBRESTEAM_EXT_REPL_BACKEND_CMD_CAP 24U
#define LIBRESTEAM_EXT_REPL_NATIVE_IDX 0U
#define LIBRESTEAM_EXT_REPL_IL2CPP_IDX 1U
#define LIBRESTEAM_EXT_REPL_LUA_IDX 2U
#define LIBRESTEAM_EXT_REPL_JAVA_IDX 3U
#define LIBRESTEAM_EXT_REPL_BACKEND_CUNT (LIBRESTEAM_EXT_REPL_JAVA_IDX + 1U)
#define LIBRESTEAM_EXT_REPL_ARG_CAP (5U + LIBRESTEAM_EXT_REPL_PARAM_CAP)

typedef uint32_t (*libresteam_ext_repl_backend_cmd_cb_t)(void *user, uint32_t argc, char **args);
typedef uint32_t (*libresteam_ext_repl_backend_tick_cb_t)(void *user);

typedef struct libresteam_ext_repl_backend
{
    const char *name;
    const char *desc;
    const char *commands[LIBRESTEAM_EXT_REPL_BACKEND_CMD_CAP];
    libresteam_ext_repl_backend_cmd_cb_t cmd;
    libresteam_ext_repl_backend_cmd_cb_t complete;
    libresteam_ext_repl_backend_tick_cb_t tick;
    libresteam_ext_repl_backend_tick_cb_t cont;
    libresteam_ext_repl_backend_tick_cb_t shutdown;
    uint32_t last_res;
    uint8_t pudding[4];
} libresteam_ext_repl_backend_t;

typedef uint64_t (*libresteam_ext_repl_img_cunt_cb_t)(void *img);
typedef void *(*libresteam_ext_repl_img_class_cb_t)(void *img, uint64_t idx);
typedef const char *(*libresteam_ext_repl_name_cb_t)(void *obj);
typedef void *(*libresteam_ext_repl_ptr_cb_t)(void *obj);
typedef void *(*libresteam_ext_repl_iter_cb_t)(void *klass, void **iter);
typedef uint32_t (*libresteam_ext_repl_cunt_cb_t)(void *method);
typedef uint32_t (*libresteam_ext_repl_flags_cb_t)(void *method, uint32_t *impl);
typedef void *(*libresteam_ext_repl_param_cb_t)(void *method, uint32_t idx);
typedef void (*libresteam_ext_repl_field_rd_cb_t)(void *obj, void *field, void *out);
typedef void (*libresteam_ext_repl_static_wr_cb_t)(void *field, void *val);
typedef uint8_t (*libresteam_ext_repl_pred_cb_t)(void *obj);
typedef uint8_t (*libresteam_ext_repl_assign_cb_t)(void *klass, void *other);

typedef struct libresteam_ext_repl_meta
{
    libresteam_ext_repl_img_cunt_cb_t img_cunt;
    libresteam_ext_repl_img_class_cb_t img_class;
    libresteam_ext_repl_name_cb_t class_ns;
    libresteam_ext_repl_ptr_cb_t class_img;
    libresteam_ext_repl_name_cb_t img_name;
    libresteam_ext_repl_iter_cb_t methods;
    libresteam_ext_repl_name_cb_t method_name;
    libresteam_ext_repl_cunt_cb_t param_cunt;
    libresteam_ext_repl_ptr_cb_t ret_type;
    libresteam_ext_repl_param_cb_t param_type;
    libresteam_ext_repl_flags_cb_t method_flags;
    libresteam_ext_repl_field_rd_cb_t field_rd;
    libresteam_ext_repl_field_rd_cb_t field_wr;
    libresteam_ext_repl_static_wr_cb_t static_wr;
    libresteam_ext_repl_ptr_cb_t obj_class;
    libresteam_ext_repl_pred_cb_t class_enum;
    libresteam_ext_repl_pred_cb_t class_value;
    libresteam_ext_repl_pred_cb_t method_generic;
    libresteam_ext_repl_pred_cb_t class_generic;
    libresteam_ext_repl_pred_cb_t type_byref;
    libresteam_ext_repl_assign_cb_t assignable;
} libresteam_ext_repl_meta_t;

#define LIBRESTEAM_EXT_REPL_RESP_MORE UINT32_MAX
#define LIBRESTEAM_EXT_REPL_PROTO_VERSION 1U
#define LIBRESTEAM_EXT_REPL_HELLO_MAGIC "LSREPL\0"
#define LIBRESTEAM_EXT_REPL_HELLO_MAGIC_SIZE 8U

typedef struct libresteam_ext_repl_hello
{
    char magic[LIBRESTEAM_EXT_REPL_HELLO_MAGIC_SIZE];
    uint32_t version;
    uint32_t size;
    uint64_t pid;
    uint64_t thread;
    uint32_t req_cap;
    uint32_t resp_cap;
} libresteam_ext_repl_hello_t;

typedef struct libresteam_ext_repl_req
{
    uint64_t id;
    char text[LIBRESTEAM_EXT_REPL_CMD_CAP];
} libresteam_ext_repl_req_t;

typedef struct libresteam_ext_repl_resp
{
    uint64_t id;
    uint32_t res;
    uint32_t len;
    char text[LIBRESTEAM_EXT_REPL_RESP_CAP];
} libresteam_ext_repl_resp_t;

typedef struct libresteam_ext_repl_obj
{
    uint32_t handle;
    uint8_t unity;
    uint8_t pudding[3];
} libresteam_ext_repl_obj_t;

typedef struct libresteam_ext_repl_il2cpp
{
    void *unity_class;
    libresteam_ext_repl_meta_t meta;
    void *classes[LIBRESTEAM_EXT_REPL_CLASS_CAP];
    libresteam_ext_repl_obj_t objs[LIBRESTEAM_EXT_REPL_OBJ_CAP];
    char filter[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint64_t assembly_idx;
    uint64_t class_idx;
    uint32_t class_cunt;
    uint32_t obj_next;
    uint32_t meta_res;
    uint32_t search_idx;
    uint32_t search_rows;
    uint8_t meta_tried;
    uint8_t index_done;
    uint8_t pudding[2];
} libresteam_ext_repl_il2cpp_t;

typedef struct libresteam_ext_repl_ctx
{
    ldg_spsc_queue_t req_queue;
    ldg_spsc_queue_t resp_queue;
    ldg_thread_pool_t pool;
    const libresteam_mod_api_t *api;
    void *mod;
    void *io;
    uint32_t (*flush)(void *user);
    void *native;
    void *exec;
    void *scan;
    void *trace;
    void *java;
    ldg_sys_proc_t proc;
    libresteam_ext_repl_il2cpp_t il2cpp;
    libresteam_backends_lua_res_t lua_res;
    char lua_src[LIBRESTEAM_BACKENDS_LUA_SRC_CAP];
    libresteam_ext_repl_backend_t backend[LIBRESTEAM_EXT_REPL_BACKEND_CAP];
    libresteam_ext_repl_req_t req;
    libresteam_ext_repl_resp_t resp;
    char line[1024];
    char name[LIBRESTEAM_EXT_REPL_NAME_CAP];
    char detail[LIBRESTEAM_EXT_REPL_NAME_CAP];
    char pipe_name[LIBRESTEAM_EXT_REPL_NAME_CAP];
    char complete_prefix[LIBRESTEAM_EXT_REPL_CMD_CAP];
    uint32_t complete_cunt;
    uint32_t complete_more;
    uint32_t backend_cunt;
    uint32_t selected_backend;
    uint32_t pending_backend;
    uint32_t main_thread;
    uint8_t searching;
    uint8_t response_ready;
    uint8_t req_up;
    uint8_t resp_up;
    uint8_t pool_up;
    uint8_t pool_running;
    uint8_t proc_up;
    uint8_t pudding[25];
} libresteam_ext_repl_ctx_t;

uint32_t libresteam_ext_repl_output_wr(libresteam_ext_repl_ctx_t *ctx, const char *text, uint64_t len);

uint32_t libresteam_ext_repl_emit(libresteam_ext_repl_ctx_t *ctx, const char *fmt, ...);
uint32_t libresteam_ext_repl_name_get(const char *src, char *out, uint32_t cap);
uint32_t libresteam_ext_repl_meta_tick(void *user);
uint8_t libresteam_ext_repl_cmd_eq_is(const char *lhs, const char *rhs);
uint32_t libresteam_ext_repl_complete_begin(libresteam_ext_repl_ctx_t *ctx, const char *prefix, uint8_t space);
uint32_t libresteam_ext_repl_complete_add(libresteam_ext_repl_ctx_t *ctx, const char *value, const char *label);
uint32_t libresteam_ext_repl_lua_complete(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_il2cpp_complete(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_backend_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_il2cpp_cmd(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_lua_cmd(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_il2cpp_shutdown(void *user);
uint32_t libresteam_ext_repl_java_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_java_cmd(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_java_shutdown(void *user);
uint32_t libresteam_ext_repl_native_search_tick(void *user);
uint32_t libresteam_ext_repl_native_tick(void *user);
uint32_t libresteam_ext_repl_cmd_run(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_search_tick(void *user);
uint32_t libresteam_ext_repl_obj_hold(libresteam_ext_repl_ctx_t *ctx, void *obj, uint32_t *out);
uint32_t libresteam_ext_repl_obj_get(libresteam_ext_repl_ctx_t *ctx, uint32_t idx, void **out);
uint32_t libresteam_ext_repl_field_get(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t idx, void **out);
uint32_t libresteam_ext_repl_method_get(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t idx, void **out);
uint32_t libresteam_ext_repl_value_print(libresteam_ext_repl_ctx_t *ctx, void *type, const void *data);
uint32_t libresteam_ext_repl_value_size_get(libresteam_ext_repl_ctx_t *ctx, void *type, uint32_t *out);
uint32_t libresteam_ext_repl_num_get(const char *text, uint64_t *out);
uint32_t libresteam_ext_repl_enum_set(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t field_idx, const char *text, uint32_t obj_idx);
uint32_t libresteam_ext_repl_call(libresteam_ext_repl_ctx_t *ctx, void *klass, uint32_t method_idx, uint32_t obj_idx, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_native_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_native_shutdown(void *user);
uint32_t libresteam_ext_repl_io_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_io_stop(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_io_shutdown(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_io_worker_cb(uint32_t worker_id, void *arg);
uint32_t libresteam_ext_repl_native_cmd(void *user, uint32_t argc, char **args);
uint32_t libresteam_ext_repl_native_rd(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, void *out, uint64_t len);
uint32_t libresteam_ext_repl_native_addr_get(libresteam_ext_repl_ctx_t *ctx, const char *text, uint64_t *out);
uint32_t libresteam_ext_repl_native_entry(libresteam_ext_repl_ctx_t *ctx, uint64_t addr);
uint32_t libresteam_ext_repl_native_detour_validate(libresteam_ext_repl_ctx_t *ctx, uint64_t addr);
uint32_t libresteam_ext_repl_thread_get(uint32_t *out);
uint32_t libresteam_ext_repl_trace_init(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_trace_shutdown(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_trace_set(libresteam_ext_repl_ctx_t *ctx, uint64_t addr, uint32_t thread_id);
uint32_t libresteam_ext_repl_trace_probe_start(libresteam_ext_repl_ctx_t *ctx, uint64_t addr);
uint32_t libresteam_ext_repl_trace_probe_get(libresteam_ext_repl_ctx_t *ctx, uint64_t *out);
uint32_t libresteam_ext_repl_lua_auto_tick(void *user);
uint32_t libresteam_ext_repl_trace_auto(libresteam_ext_repl_ctx_t *ctx, uint64_t addr);
uint32_t libresteam_ext_repl_trace_tick(void *user);
uint32_t libresteam_ext_repl_trace_off(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_trace_status(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_trace_rd(libresteam_ext_repl_ctx_t *ctx, uint32_t max);
uint32_t libresteam_ext_repl_trace_clear(libresteam_ext_repl_ctx_t *ctx);
uint32_t libresteam_ext_repl_trace_capture_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args);
LDG_EXPORT uint32_t libresteam_ext_repl_init(const libresteam_mod_api_t *api, void *mod);

#endif
