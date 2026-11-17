#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>

#include <libresteam/ext/repl/repl.h>
#include <libresteam/ext/repl/scan.h>

uint32_t libresteam_ext_repl_backend_init(libresteam_ext_repl_ctx_t *ctx)
{
    libresteam_ext_repl_backend_t *backend = 0x0;
    uint32_t cmd = 0x0;

    if (LDG_UNLIKELY(!ctx)) { return LDG_ERR_FUNC_ARG_NULL; }

    ctx->selected_backend = UINT32_MAX;
    ctx->pending_backend = UINT32_MAX;
    backend = &ctx->backend[LIBRESTEAM_EXT_REPL_NATIVE_IDX];
    backend->name = "native";
    backend->desc = "modules, exports by name, addresses, memory regions, typed reads/writes, buffers, native invocation, byte/string/pointer search, code-reference candidates, execution tracing, expression/delta/curve scans";
    backend->cmd = libresteam_ext_repl_native_cmd;
    backend->tick = libresteam_ext_repl_native_tick;
    backend->cont = libresteam_ext_repl_native_search_tick;
    backend->shutdown = libresteam_ext_repl_native_shutdown;
    backend->commands[0] = "modules";
    backend->commands[1] = "symbol";
    backend->commands[2] = "peek";
    backend->commands[3] = "address";
    backend->commands[4] = "regions";
    backend->commands[5] = "read";
    backend->commands[6] = "find";
    backend->commands[7] = "findstr";
    backend->commands[8] = "findptr";
    backend->commands[9] = "trace";
    backend->commands[10] = "xrefs";
    backend->commands[11] = "scan";
    backend->commands[12] = "terminate";
    backend->commands[13] = "write";
    backend->commands[14] = "poke";
    backend->commands[15] = "buff";
    backend->commands[16] = "invoke";
    backend = &ctx->backend[LIBRESTEAM_EXT_REPL_IL2CPP_IDX];
    backend->name = "il2cpp";
    backend->desc = "managed types, fields, methods, objects, enums, invocation";
    backend->cmd = libresteam_ext_repl_il2cpp_cmd;
    backend->complete = libresteam_ext_repl_il2cpp_complete;
    backend->tick = libresteam_ext_repl_meta_tick;
    backend->cont = libresteam_ext_repl_search_tick;
    backend->shutdown = libresteam_ext_repl_il2cpp_shutdown;
    backend->commands[0] = "types";
    backend->commands[1] = "type";
    backend->commands[2] = "fields";
    backend->commands[3] = "methods";
    backend->commands[4] = "method";
    backend->commands[5] = "objects";
    backend->commands[6] = "get";
    backend->commands[7] = "fieldtype";
    backend->commands[8] = "objtype";
    backend->commands[9] = "enum";
    backend->commands[10] = "set";
    backend->commands[11] = "call";
    backend->commands[12] = "drop";
    backend = &ctx->backend[LIBRESTEAM_EXT_REPL_LUA_IDX];
    backend->name = "lua";
    backend->desc = "existing Lua 5.1 VM attachment, API binding, stack inspection, protected evaluation";
    backend->cmd = libresteam_ext_repl_lua_cmd;
    backend->complete = libresteam_ext_repl_lua_complete;
    backend->cont = libresteam_ext_repl_lua_auto_tick;
    backend->commands[0] = "eval";
    backend->commands[1] = "evalmem";
    backend->commands[2] = "attach";
    backend->commands[3] = "detach";
    backend->commands[4] = "stack";
    backend->commands[5] = "bind";
    backend->commands[6] = "api";
    backend->commands[7] = "auto";
    backend = &ctx->backend[LIBRESTEAM_EXT_REPL_JAVA_IDX];
    backend->name = "java";
    backend->desc = "existing HotSpot VM: loaded classes; declared fields and methods; live objects; field reads and writes; method invocation";
    backend->cmd = libresteam_ext_repl_java_cmd;
    backend->shutdown = libresteam_ext_repl_java_shutdown;
    backend->commands[cmd++] = "types";
    backend->commands[cmd++] = "fields";
    backend->commands[cmd++] = "methods";
    backend->commands[cmd++] = "objects";
    backend->commands[cmd++] = "get";
    backend->commands[cmd++] = "drop";
    backend->commands[cmd++] = "set";
    backend->commands[cmd++] = "call";
    ctx->backend_cunt = LIBRESTEAM_EXT_REPL_BACKEND_CUNT;
    return LDG_ERR_AOK;
}
