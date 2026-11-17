#ifndef LIBRESTEAM_MOD_H
#define LIBRESTEAM_MOD_H

#include <stdint.h>

#include <dangling/io/path.h>
#include <dangling/thread/sync.h>

#include <libresteam/fp32/fp32.h>

#define LIBRESTEAM_MOD_API_VERSION 1U
#define LIBRESTEAM_MOD_EXT ".dangext"
#define LIBRESTEAM_MOD_DIR "extensions"
#define LIBRESTEAM_MOD_INIT_SYM "dangmod_init"
#define LIBRESTEAM_MOD_SHUTDOWN_SYM "dangmod_shutdown"
#define LIBRESTEAM_MOD_MAX 1024U
#define LIBRESTEAM_MOD_TICK_MAX 1024U
#define LIBRESTEAM_MOD_NAME_MAX 512U
#define LIBRESTEAM_MOD_CMD_MAX 16U

typedef void (*libresteam_mod_tick_cb_t)(void *user);
typedef uint32_t (*libresteam_mod_log_cb_t)(void *mod, const char *fmt, ...);
typedef uint32_t (*libresteam_mod_tick_reg_cb_t)(void *mod, libresteam_mod_tick_cb_t cb, void *user);
typedef uint32_t (*libresteam_mod_class_find_cb_t)(const void *il2cpp, const char *ns, const char *name, void **out);
typedef uint32_t (*libresteam_mod_method_find_cb_t)(const void *il2cpp, void *klass, const char *name, uint32_t argc, void **out);
typedef uint32_t (*libresteam_mod_invoke_cb_t)(const void *il2cpp, void *method, void *obj, void **params, void **out);
typedef uint32_t (*libresteam_mod_field_class_find_cb_t)(const void *il2cpp, void *klass, void *field_class, uint64_t *out);
typedef uint32_t (*libresteam_mod_field_kind_find_cb_t)(const void *il2cpp, void *klass, uint32_t kind, uint32_t nth, uint64_t *out);
typedef uint32_t (*libresteam_mod_objs_find_cb_t)(const void *il2cpp, void *klass, void ***elems_out, uint32_t *cunt_out);
typedef uint32_t (*libresteam_mod_str_ascii_get_cb_t)(const void *il2cpp, void *str, char *buff, uint32_t cap);
typedef uint32_t (*libresteam_mod_obj_name_get_cb_t)(const void *il2cpp, void *obj, char *buff, uint32_t cap);
typedef uint32_t (*libresteam_mod_pos_get_cb_t)(const void *il2cpp, void *comp, libresteam_fp32_t *out);
typedef uint32_t (*libresteam_mod_enum_name_get_cb_t)(const void *il2cpp, void *enum_class, uint32_t val, const char **out);
typedef uint32_t (*libresteam_mod_objs_hold_cb_t)(const void *il2cpp, void *klass, uint32_t *handle_out, void ***elems_out, uint32_t *cunt_out);
typedef uint8_t (*libresteam_mod_obj_alive_is_cb_t)(const void *il2cpp, void *obj);
typedef void (*libresteam_mod_cleanup_cb_t)(void *user);
typedef uint32_t (*libresteam_mod_cleanup_reg_cb_t)(void *mod, libresteam_mod_cleanup_cb_t cb, void *user);
typedef uint32_t (*libresteam_mod_cmd_emit_cb_t)(void *sink, const char *text);
typedef uint32_t (*libresteam_mod_cmd_cb_t)(void *user, uint32_t argc, char **args, libresteam_mod_cmd_emit_cb_t emit, void *sink);

typedef struct libresteam_mod_cmd
{
    const char *name;
    const char *desc;
    const char *words;
    libresteam_mod_cmd_cb_t cb;
    void *user;
    void *mod;
} libresteam_mod_cmd_t;

typedef uint32_t (*libresteam_mod_cmd_reg_cb_t)(void *mod, const libresteam_mod_cmd_t *cmd);
typedef uint32_t (*libresteam_mod_cmd_get_cb_t)(void *mod, uint32_t idx, libresteam_mod_cmd_t *out);

typedef struct libresteam_mod_api
{
    uint32_t version;
    uint32_t size;
    libresteam_mod_log_cb_t log;
    libresteam_mod_tick_reg_cb_t tick_reg;
    const void *il2cpp;
    libresteam_mod_class_find_cb_t class_find;
    libresteam_mod_method_find_cb_t method_find;
    libresteam_mod_invoke_cb_t invoke;
    libresteam_mod_field_class_find_cb_t field_class_find;
    libresteam_mod_field_kind_find_cb_t field_kind_find;
    libresteam_mod_objs_find_cb_t objs_find;
    libresteam_mod_str_ascii_get_cb_t str_ascii_get;
    libresteam_mod_obj_name_get_cb_t obj_name_get;
    libresteam_mod_pos_get_cb_t pos_get;
    libresteam_mod_enum_name_get_cb_t enum_name_get;
    libresteam_mod_objs_hold_cb_t objs_hold;
    libresteam_mod_obj_alive_is_cb_t obj_alive_is;
    libresteam_mod_cleanup_reg_cb_t cleanup_reg;
    const void *lua;
    uint64_t lua_api_size;
    libresteam_mod_cmd_reg_cb_t cmd_reg;
    libresteam_mod_cmd_get_cb_t cmd_get;
    const void *java;
    uint64_t java_api_size;
} libresteam_mod_api_t;

typedef uint32_t (*libresteam_mod_init_cb_t)(const libresteam_mod_api_t *api, void *mod);
typedef void (*libresteam_mod_shutdown_cb_t)(void);

typedef struct libresteam_mod
{
    void *lib;
    void *owner;
    libresteam_mod_shutdown_cb_t shutdown;
    libresteam_mod_cleanup_cb_t cleanup;
    void *cleanup_user;
    char name[LIBRESTEAM_MOD_NAME_MAX];
    uint8_t accepting;
    uint8_t stopped;
    uint8_t pudding[6];
} libresteam_mod_t;

typedef struct libresteam_mod_tick
{
    libresteam_mod_tick_cb_t cb;
    void *user;
    void *mod;
} libresteam_mod_tick_t;

typedef struct libresteam_mod_host
{
    libresteam_mod_api_t api;
    libresteam_mod_t mod[LIBRESTEAM_MOD_MAX];
    libresteam_mod_tick_t tick[LIBRESTEAM_MOD_TICK_MAX];
    libresteam_mod_cmd_t cmd[LIBRESTEAM_MOD_CMD_MAX];
    ldg_mut_t mut;
    char root_path[LDG_IO_PATH_MAX];
    char dir_path[LDG_IO_PATH_MAX];
    char file_path[LDG_IO_PATH_MAX];
    char dir_name[LIBRESTEAM_MOD_NAME_MAX];
    char file_name[LIBRESTEAM_MOD_NAME_MAX];
    char mod_name[LIBRESTEAM_MOD_NAME_MAX];
    char log_path[LDG_IO_PATH_MAX];
    uint32_t mod_cunt;
    uint32_t tick_cunt;
    uint32_t cmd_cunt;
    uint8_t ld_done;
    uint8_t active;
    uint8_t dispatching;
    uint8_t loading;
    uint8_t stopping;
    uint8_t runtime;
    uint8_t pudding[6];
} libresteam_mod_host_t;

uint32_t libresteam_mod_ld(libresteam_mod_host_t *mods, const libresteam_mod_api_t *seed);
uint32_t libresteam_mod_dir_ld(libresteam_mod_host_t *mods, const libresteam_mod_api_t *seed, const char *root);
uint32_t libresteam_mod_tick(libresteam_mod_host_t *mods);
uint32_t libresteam_mod_unld(libresteam_mod_host_t *mods);

#endif
