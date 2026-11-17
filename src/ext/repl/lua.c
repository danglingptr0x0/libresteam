#include <stdint.h>

#include <dangling/core/err.h>
#include <dangling/core/macros.h>
#include <dangling/str/str.h>
#include <dangling/str/fmt.h>

#include <libresteam/ext/repl/repl.h>

#define LUA_ESCAPE_WIDTH 4U
#define LUA_AUTO_ARG_CUNT 2U
#define LUA_AUTO_SIG_BYTES 13U
#define LUA_AUTO_PROBE_CUNT 1U
#define LUA_AUTO_STATE_TOP_OFF 0x10U
#define LUA_AUTO_SLOT_BYTES 16U
#define LUA_AUTO_STATE_PTR_CUNT 2U
#define LUA_AUTO_PCALL_IDX 8U
#define LUA_AUTO_JMP_OPCODE 0xe9U
#define LUA_AUTO_JMP_BYTES 5U
#define LUA_AUTO_PROFILE_UNRAVEL 0U
#define LUA_AUTO_PROFILE_UNRAVEL_TWO 1U
#define LUA_UNRAVEL_TWO_NAME "unravel2"
#define LUA_UNRAVEL_GETTOP_ADDR "exe+0x1936650"
#define LUA_UNRAVEL_SETTOP_ADDR "exe+0x1937480"
#define LUA_UNRAVEL_TYPE_ADDR "exe+0x19379b0"
#define LUA_UNRAVEL_TOLSTRING_ADDR "exe+0x19376b0"
#define LUA_UNRAVEL_TONUMBER_ADDR "exe+0x1937760"
#define LUA_UNRAVEL_TOBOOLEAN_ADDR "exe+0x19375f0"
#define LUA_UNRAVEL_TOPOINTER_ADDR "exe+0x19377b0"
#define LUA_UNRAVEL_LOADBUFFER_ADDR "exe+0x1938a40"
#define LUA_UNRAVEL_PCALL_ADDR "exe+0x1936ab0"
#define LUA_UNRAVEL_PUSHNUMBER_ADDR "exe+0x1936d90"
#define LUA_UNRAVEL_GETTOP_SIG "\x48\x8B\x41\x10\x48\x2B\x41\x18\x48\xC1\xF8\x04\xC3"
#define LUA_UNRAVEL_SETTOP_SIG "\x85\xD2\x78\x3E\x4C\x63\xC2\x48\x8B\x51\x18\x49\xC1"
#define LUA_UNRAVEL_TYPE_SIG "\x48\x83\xEC\x28\xE8\x77\xE4\xFF\xFF\x48\x8D\x0D\x10"
#define LUA_UNRAVEL_TOLSTRING_SIG "\x48\x89\x5C\x24\x08\x48\x89\x74\x24\x10\x57\x48\x83"
#define LUA_UNRAVEL_TONUMBER_SIG "\x48\x83\xEC\x38\xE8\xC7\xE6\xFF\xFF\x83\x78\x08\x03"
#define LUA_UNRAVEL_TOBOOLEAN_SIG "\x48\x83\xEC\x28\xE8\x37\xE8\xFF\xFF\x8B\x48\x08\x85"
#define LUA_UNRAVEL_TOPOINTER_SIG "\x48\x83\xEC\x28\x41\x89\xD2\x49\x89\xCB\xE8\x71\xE6"
#define LUA_UNRAVEL_LOADBUFFER_SIG "\x48\x83\xEC\x38\x48\x89\x54\x24\x20\x4C\x89\x44\x24"
#define LUA_UNRAVEL_PCALL_SIG "\x48\x89\x5C\x24\x08\x57\x48\x83\xEC\x40\x44\x89\xC7"
#define LUA_UNRAVEL_PUSHNUMBER_SIG "\x48\x8B\x41\x10\xF2\x0F\x11\x08\xC7\x40\x08\x03\x00"
#define LUA_UNRAVEL_TWO_GETTOP_ADDR "exe+0x87473e0"
#define LUA_UNRAVEL_TWO_SETTOP_ADDR "exe+0x8753b80"
#define LUA_UNRAVEL_TWO_TYPE_ADDR "exe+0x8756170"
#define LUA_UNRAVEL_TWO_TOLSTRING_ADDR "exe+0x8754da0"
#define LUA_UNRAVEL_TWO_TONUMBER_ADDR "exe+0x8755110"
#define LUA_UNRAVEL_TWO_TOBOOLEAN_ADDR "exe+0x87542c0"
#define LUA_UNRAVEL_TWO_TOPOINTER_ADDR "exe+0x8755620"
#define LUA_UNRAVEL_TWO_LOADBUFFER_ADDR "exe+0x875bd60"
#define LUA_UNRAVEL_TWO_PCALL_ADDR "exe+0x874a740"
#define LUA_UNRAVEL_TWO_PROBE_ADDR "exe+0x869f50"
#define LUA_UNRAVEL_TWO_GETTOP_SIG LUA_UNRAVEL_GETTOP_SIG
#define LUA_UNRAVEL_TWO_SETTOP_SIG "\x85\xD2\x78\x42\x4C\x63\xC2\x48\x8B\x51\x18\x49\xC1"
#define LUA_UNRAVEL_TWO_TYPE_SIG "\x48\x83\xEC\x28\xE8\xD7\x32\x11\xF8\x48\x8D\x0D\x60"
#define LUA_UNRAVEL_TWO_TOLSTRING_SIG LUA_UNRAVEL_TOLSTRING_SIG
#define LUA_UNRAVEL_TWO_TONUMBER_SIG "\x48\x83\xEC\x38\xE8\x37\x43\x11\xF8\x83\x78\x08\x03"
#define LUA_UNRAVEL_TWO_TOBOOLEAN_SIG "\x48\x83\xEC\x28\xE8\x87\x51\x11\xF8\x8B\x48\x08\x85"
#define LUA_UNRAVEL_TWO_TOPOINTER_SIG "\x48\x83\xEC\x28\x41\x89\xD2\x49\x89\xCB\xE8\x21\x3E"
#define LUA_UNRAVEL_TWO_LOADBUFFER_SIG "\x48\x83\xEC\x38\x48\x89\x54\x24\x20\x48\x8D\x15\xA0"
#define LUA_UNRAVEL_TWO_PCALL_SIG LUA_UNRAVEL_PCALL_SIG
#define LUA_UNRAVEL_TWO_PROBE_SIG "\xE9\xEB\x07\xEE\x07\xCC\xCC\xCC\xCC\xCC\xCC\xCC\xCC"

#define LUA_COMPLETE_SCRIPT "local path='%s'\n" \
        "local function parent(t)\n" \
        "    if type(t)~='table' then return nil end\n" \
        "    local m=getmetatable(t)\n" \
        "    if type(m)~='table' then return nil end\n" \
        "    local v=rawget(m,'__index')\n" \
        "    if type(v)=='table' then return v end\n" \
        "end\n" \
        "local function lookup(t,k)\n" \
        "    local seen={}\n" \
        "    for i=1,8 do\n" \
        "        if type(t)~='table' or seen[t] then return nil end\n" \
        "        seen[t]=true\n" \
        "        local v=rawget(t,k)\n" \
        "        if v~=nil then return v end\n" \
        "        t=parent(t)\n" \
        "    end\n" \
        "end\n" \
        "local t=_G\n" \
        "local stem,prefix=path:match('^(.*[.:])([%%w_]*)$')\n" \
        "if not stem then stem='';prefix=path end\n" \
        "for k in stem:gmatch('[%%w_]+') do t=lookup(t,k) end\n" \
        "if type(t)~='table' then return '' end\n" \
        "local out,keys,tables={},{},{}\n" \
        "local visits=0\n" \
        "local limited=false\n" \
        "for depth=1,8 do\n" \
        "    if type(t)~='table' or tables[t] then break end\n" \
        "    tables[t]=true\n" \
        "    local k=nil\n" \
        "    while true do\n" \
        "        local v\n" \
        "        k,v=next(t,k)\n" \
        "        if k==nil then break end\n" \
        "        visits=visits+1\n" \
        "        if visits>4096 then limited=true;break end\n" \
        "        if type(k)=='string' and #k<192 and k:match('^[%%a_][%%w_]*$') and k:sub(1,#prefix)==prefix and not keys[k] then\n" \
        "            keys[k]=true\n" \
        "            if #out==64 then limited=true;break end\n" \
        "            out[#out+1]=k..'\\t'..type(v)\n" \
        "        end\n" \
        "    end\n" \
        "    if limited then break end\n" \
        "    t=parent(t)\n" \
        "    if depth==8 and type(t)=='table' then limited=true end\n" \
        "end\n" \
        "table.sort(out)\n" \
        "if limited then out[#out+1]='!' end\n" \
        "return table.concat(out,'\\n')\n"

static uint32_t lua_help(libresteam_ext_repl_ctx_t *ctx)
{
    uint32_t res = libresteam_ext_repl_emit(ctx, "Lua 5.1, 64-bit, double lua_Number; existing game VM only");

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "lua bind 5.1 <loaded-module> | lua api <symbol> <address> | lua status"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "lua auto unravel | lua auto " LUA_UNRAVEL_TWO_NAME "; verify the selected executable profile, bind APIs, and attach from a live call on the dispatch thread"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "API symbols: lua_gettop lua_settop lua_type lua_tolstring lua_tonumber lua_toboolean lua_topointer luaL_loadbuffer lua_pcall"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "lua attach <state-address> <owning-thread-id> | lua detach | lua stack [zero-based-start [rows]]; default: all rows"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "lua eval <Lua source to end of line> | lua evalmem <address> <bytes>; use return to display values"); }

    if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_emit(ctx, "Commands run on the REPL callback thread, which must own this VM. Detach before state destruction. Returned object addresses are not rooted handles."); }

    return res;
}

static uint32_t lua_status(libresteam_ext_repl_ctx_t *ctx)
{
    libresteam_backends_lua_status_t status = LDG_STRUCT_ZERO_INIT;
    uint32_t res = ((const libresteam_backends_lua_api_t *)ctx->api->lua)->status_get(((const libresteam_backends_lua_api_t *)ctx->api->lua)->ctx, &status);

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    return libresteam_ext_repl_emit(ctx, "lua; ABI: %u32; bound: %u32/9; attached: %u32; state: 0x%x64; owning thread: %u32; dispatch thread: %u32", status.abi, status.bound, (uint32_t)status.attached, status.state, status.thread_id, ctx->main_thread);
}

static uint32_t lua_auto_start(libresteam_ext_repl_ctx_t *ctx, uint32_t profile)
{
    const char *name[] = { "lua_gettop", "lua_settop", "lua_type", "lua_tolstring", "lua_tonumber", "lua_toboolean", "lua_topointer", "luaL_loadbuffer", "lua_pcall", "state probe" };
    const char *addr[][LIBRESTEAM_BACKENDS_LUA_FUNC_CAP + LUA_AUTO_PROBE_CUNT] =
    {
        { LUA_UNRAVEL_GETTOP_ADDR, LUA_UNRAVEL_SETTOP_ADDR, LUA_UNRAVEL_TYPE_ADDR, LUA_UNRAVEL_TOLSTRING_ADDR, LUA_UNRAVEL_TONUMBER_ADDR, LUA_UNRAVEL_TOBOOLEAN_ADDR, LUA_UNRAVEL_TOPOINTER_ADDR, LUA_UNRAVEL_LOADBUFFER_ADDR, LUA_UNRAVEL_PCALL_ADDR, LUA_UNRAVEL_PUSHNUMBER_ADDR },
        { LUA_UNRAVEL_TWO_GETTOP_ADDR, LUA_UNRAVEL_TWO_SETTOP_ADDR, LUA_UNRAVEL_TWO_TYPE_ADDR, LUA_UNRAVEL_TWO_TOLSTRING_ADDR, LUA_UNRAVEL_TWO_TONUMBER_ADDR, LUA_UNRAVEL_TWO_TOBOOLEAN_ADDR, LUA_UNRAVEL_TWO_TOPOINTER_ADDR, LUA_UNRAVEL_TWO_LOADBUFFER_ADDR, LUA_UNRAVEL_TWO_PCALL_ADDR, LUA_UNRAVEL_TWO_PROBE_ADDR }
    };
    const char *sig[][LIBRESTEAM_BACKENDS_LUA_FUNC_CAP + LUA_AUTO_PROBE_CUNT] =
    {
        { LUA_UNRAVEL_GETTOP_SIG, LUA_UNRAVEL_SETTOP_SIG, LUA_UNRAVEL_TYPE_SIG, LUA_UNRAVEL_TOLSTRING_SIG, LUA_UNRAVEL_TONUMBER_SIG, LUA_UNRAVEL_TOBOOLEAN_SIG, LUA_UNRAVEL_TOPOINTER_SIG, LUA_UNRAVEL_LOADBUFFER_SIG, LUA_UNRAVEL_PCALL_SIG, LUA_UNRAVEL_PUSHNUMBER_SIG },
        { LUA_UNRAVEL_TWO_GETTOP_SIG, LUA_UNRAVEL_TWO_SETTOP_SIG, LUA_UNRAVEL_TWO_TYPE_SIG, LUA_UNRAVEL_TWO_TOLSTRING_SIG, LUA_UNRAVEL_TWO_TONUMBER_SIG, LUA_UNRAVEL_TWO_TOBOOLEAN_SIG, LUA_UNRAVEL_TWO_TOPOINTER_SIG, LUA_UNRAVEL_TWO_LOADBUFFER_SIG, LUA_UNRAVEL_TWO_PCALL_SIG, LUA_UNRAVEL_TWO_PROBE_SIG }
    };
    uint64_t resolved[LIBRESTEAM_BACKENDS_LUA_FUNC_CAP + LUA_AUTO_PROBE_CUNT] = LDG_ARR_ZERO_INIT;
    uint8_t code[LUA_AUTO_SIG_BYTES] = LDG_ARR_ZERO_INIT;
    libresteam_backends_lua_status_t status = LDG_STRUCT_ZERO_INIT;
    const libresteam_backends_lua_api_t *lua = ctx->api->lua;
    uint32_t idx = 0;
    uint32_t byte = 0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(profile >= sizeof(addr) / sizeof(addr[0]))) { return LDG_ERR_FUNC_ARG_INVALID; }

    res = lua->status_get(lua->ctx, &status);

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (status.attached) { return lua_status(ctx); }

    for (idx = 0; idx < sizeof(resolved) / sizeof(resolved[0]); idx++)
    {
        res = libresteam_ext_repl_native_addr_get(ctx, addr[profile][idx], &resolved[idx]);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_native_rd(ctx, resolved[idx], code, sizeof(code)); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        byte = 0;
        if (profile == LUA_AUTO_PROFILE_UNRAVEL && idx == LUA_AUTO_PCALL_IDX && code[0] == LUA_AUTO_JMP_OPCODE)
        {
            res = libresteam_ext_repl_native_detour_validate(ctx, resolved[idx]);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            byte = LUA_AUTO_JMP_BYTES;
        }

        for (; byte < sizeof(code); byte++)
        {
            if (code[byte] == (uint8_t)sig[profile][idx][byte]) { continue; }

            res = libresteam_ext_repl_emit(ctx, "lua auto; profile mismatch: %s; address: 0x%x64", name[idx], resolved[idx]);
            return res == LDG_ERR_AOK ? LDG_ERR_UNSUPPORTED : res;
        }
    }
    for (idx = 0; idx < LIBRESTEAM_BACKENDS_LUA_FUNC_CAP; idx++)
    {
        res = lua->func_bind(lua->ctx, name[idx], resolved[idx], LIBRESTEAM_BACKENDS_LUA_ABI_51);
        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }
    }
    res = libresteam_ext_repl_trace_probe_start(ctx, resolved[LIBRESTEAM_BACKENDS_LUA_FUNC_CAP]);
    if (res == LDG_ERR_AOK) { ctx->searching = LDG_TRUTH_TRUE; }

    return res;
}

uint32_t libresteam_ext_repl_lua_auto_tick(void *user)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    uint64_t state = 0;
    uint64_t stack[LUA_AUTO_STATE_PTR_CUNT] = LDG_ARR_ZERO_INIT;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !ctx->api || !ctx->api->lua)) { return LDG_ERR_FUNC_ARG_NULL; }

    res = libresteam_ext_repl_trace_probe_get(ctx, &state);
    if (res == LDG_ERR_AGAIN) { return LDG_ERR_AOK; }

    ctx->searching = LDG_TRUTH_FALSE;
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (state > UINT64_MAX - LUA_AUTO_STATE_TOP_OFF || state % sizeof(uint64_t)) { return LDG_ERR_INVALID; }

    res = libresteam_ext_repl_native_rd(ctx, state + LUA_AUTO_STATE_TOP_OFF, stack, sizeof(stack));
    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    if (!stack[1] || stack[0] < stack[1] || (stack[0] - stack[1]) % LUA_AUTO_SLOT_BYTES) { return LDG_ERR_INVALID; }

    res = ((const libresteam_backends_lua_api_t *)ctx->api->lua)->attach(((const libresteam_backends_lua_api_t *)ctx->api->lua)->ctx, state, ctx->main_thread);
    return res == LDG_ERR_AOK ? lua_status(ctx) : res;
}

static uint32_t lua_str_emit(libresteam_ext_repl_ctx_t *ctx, const char *src, uint64_t len)
{
    char hex[3] = LDG_ARR_ZERO_INIT;
    char text[LIBRESTEAM_BACKENDS_LUA_TEXT_CAP * LUA_ESCAPE_WIDTH] = LDG_ARR_ZERO_INIT;
    uint64_t idx = 0x0;
    uint32_t dst = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint8_t ch = 0x0;

    for (idx = 0; idx < len; idx++)
    {
        if (dst + LUA_ESCAPE_WIDTH > sizeof(text))
        {
            res = libresteam_ext_repl_output_wr(ctx, text, dst);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            dst = 0;
        }

        ch = (uint8_t)src[idx];
        if (ch >= LDG_ASCII_PRINTABLE_MIN && ch <= LDG_ASCII_PRINTABLE_MAX && ch != '\\' && ch != '"') { text[dst++] = (char)ch; }
        else
        {
            res = ldg_byte_to_hex(ch, hex);
            if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

            text[dst++] = '\\';
            text[dst++] = 'x';
            text[dst++] = hex[0];
            text[dst++] = hex[1];
        }
    }
    return libresteam_ext_repl_output_wr(ctx, text, dst);
}

static uint32_t lua_val_emit(void *user, uint32_t idx, const libresteam_backends_lua_val_t *val, const char *text, uint64_t len)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    const char *types[] = { "nil", "boolean", "lightuserdata", "number", "string", "table", "function", "userdata", "thread" };
    uint64_t hdr_len = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(val->type > LIBRESTEAM_BACKENDS_LUA_TYPE_THREAD)) { return LDG_ERR_INVALID; }

    if (val->type == LIBRESTEAM_BACKENDS_LUA_TYPE_STR)
    {
        res = ldg_str_fmt(ctx->line, sizeof(ctx->line), &hdr_len, "row: %u32; type: string; bytes: %u64; value: \"", idx, len);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_output_wr(ctx, ctx->line, hdr_len); }

        if (res == LDG_ERR_AOK) { res = lua_str_emit(ctx, text, len); }

        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_output_wr(ctx, "\"\n", 2U); }
    }
    else if (val->type == LIBRESTEAM_BACKENDS_LUA_TYPE_NUMBER) { res = libresteam_ext_repl_emit(ctx, "row: %u32; type: number; value: %f", idx, val->number); }
    else if (val->type == LIBRESTEAM_BACKENDS_LUA_TYPE_BOOL) { res = libresteam_ext_repl_emit(ctx, "row: %u32; type: boolean; value: %s", idx, val->number != 0.0 ? "true" : "false"); }
    else { res = libresteam_ext_repl_emit(ctx, "row: %u32; type: %s; address: 0x%x64", idx, types[val->type], val->addr); }

    return res;
}

static uint32_t lua_summary(libresteam_ext_repl_ctx_t *ctx)
{
    return libresteam_ext_repl_emit(ctx, "lua rows: %u32; total: %u32; original stack top: %u32; lua status: %u32", ctx->lua_res.cunt, ctx->lua_res.total, ctx->lua_res.stack_top, ctx->lua_res.lua_status);
}

static uint32_t lua_eval_cmd(libresteam_ext_repl_ctx_t *ctx, uint32_t argc, char **args)
{
    const libresteam_backends_lua_api_t *lua = ctx->api->lua;
    const char *src = 0x0;
    uint64_t len = 0x0;
    uint64_t addr = 0x0;
    uint32_t res = LDG_ERR_AOK;
    uint32_t print_res = LDG_ERR_AOK;

    if (libresteam_ext_repl_cmd_eq_is(args[0], "eval") == LDG_TRUTH_TRUE)
    {
        if (argc != 2U) { return LDG_ERR_FUNC_ARG_INVALID; }

        src = args[1];
        res = ldg_str_len_get(src, LIBRESTEAM_EXT_REPL_CMD_CAP, &len);
    }
    else
    {
        if (argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[2], &len); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (LDG_UNLIKELY(!len || len > sizeof(ctx->lua_src))) { return LDG_ERR_BOUNDS; }

        res = libresteam_ext_repl_native_rd(ctx, addr, ctx->lua_src, len);
        src = ctx->lua_src;
    }

    if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

    res = lua->eval_stream(lua->ctx, src, len, lua_val_emit, ctx, &ctx->lua_res);
    if (res != LDG_ERR_AOK && !ctx->lua_res.lua_status) { return res; }

    print_res = lua_summary(ctx);
    return print_res == LDG_ERR_AOK ? res : print_res;
}

static uint32_t lua_complete_emit(void *user, uint32_t idx, const libresteam_backends_lua_val_t *val, const char *text, uint64_t len)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    char value[LIBRESTEAM_EXT_REPL_CMD_CAP] = LDG_ARR_ZERO_INIT;
    char label[LIBRESTEAM_EXT_REPL_NAME_CAP] = LDG_ARR_ZERO_INIT;
    uint64_t pos = 0x0;
    uint32_t dst = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (idx || val->type != LIBRESTEAM_BACKENDS_LUA_TYPE_STR) { return LDG_ERR_INVALID; }

    while (pos < len)
    {
        dst = 0;
        while (pos < len && text[pos] != '\t' && text[pos] != '\n')
        {
            if (dst + LDG_STR_TERM_SIZE >= sizeof(value)) { return LDG_ERR_BOUNDS; }

            value[dst++] = text[pos++];
        }
        value[dst] = LDG_STR_TERM;
        dst = 0;
        if (pos < len && text[pos] == '\t')
        {
            pos++;
            while (pos < len && text[pos] != '\n')
            {
                if (dst + LDG_STR_TERM_SIZE >= sizeof(label)) { return LDG_ERR_BOUNDS; }

                label[dst++] = text[pos++];
            }
        }

        label[dst] = LDG_STR_TERM;
        if (pos < len) { pos++; }

        if (value[0] == '!' && !value[1]) { ctx->complete_more = 1; res = libresteam_ext_repl_emit(ctx, "!"); }
        else { res = libresteam_ext_repl_complete_add(ctx, value, label); }

        if (res != LDG_ERR_AOK) { return res; }
    }
    return res;
}

static uint32_t lua_complete_path(const char *text, uint32_t *start, uint32_t *leaf)
{
    uint32_t idx = 0x0;
    uint32_t end = 0x0;
    uint32_t probe = 0x0;
    uint32_t level = 0x0;
    uint32_t long_level = UINT32_MAX;
    uint8_t quote = 0x0;

    *start = 0;
    *leaf = 0;
    while (text[end]) { end++; }
    for (idx = 0; idx < end; idx++)
    {
        if (long_level != UINT32_MAX)
        {
            if (text[idx] != ']') { continue; }

            probe = idx + 1U;
            while (probe < end && text[probe] == '=') { probe++; }
            if (probe < end && text[probe] == ']' && probe - idx - 1U == long_level) { long_level = UINT32_MAX; idx = probe; }

            continue;
        }

        if (quote)
        {
            if (text[idx] == '\\' && idx + 1U < end) { idx++; }
            else if ((uint8_t)text[idx] == quote) { quote = 0; }

            continue;
        }

        if (text[idx] == '\'' || text[idx] == '"') { quote = (uint8_t)text[idx]; continue; }

        if (text[idx] == '-' && idx + 1U < end && text[idx + 1U] == '-')
        {
            idx += 2U;
            if (text[idx] != '[') { return LDG_ERR_NOT_FOUND; }

            probe = idx + 1U;
            while (probe < end && text[probe] == '=') { probe++; }
            if (probe == end || text[probe] != '[') { return LDG_ERR_NOT_FOUND; }
        }

        if (text[idx] == '[')
        {
            probe = idx + 1U;
            while (probe < end && text[probe] == '=') { probe++; }
            if (probe < end && text[probe] == '[') { long_level = probe - idx - 1U; idx = probe; }
        }
    }
    if (quote || long_level != UINT32_MAX) { return LDG_ERR_NOT_FOUND; }

    idx = end;
    while (idx && (text[idx - 1U] == '_' || text[idx - 1U] == '.' || text[idx - 1U] == ':' || ldg_char_alnum_is((uint8_t)text[idx - 1U]) == LDG_TRUTH_TRUE)) { idx--; }
    *start = idx;
    *leaf = idx;
    for (; idx < end; idx++)
    {
        if (idx == *leaf && text[idx] != '_' && ldg_char_alpha_is((uint8_t)text[idx]) != LDG_TRUTH_TRUE) { return LDG_ERR_NOT_FOUND; }

        if (text[idx] == '.' || text[idx] == ':')
        {
            if (++level > 16U) { return LDG_ERR_BOUNDS; }

            *leaf = idx + 1U;
        }
    }
    return LDG_ERR_AOK;
}

uint32_t libresteam_ext_repl_lua_complete(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    const char *apis[] = { "lua_gettop", "lua_settop", "lua_type", "lua_tolstring", "lua_tonumber", "lua_toboolean", "lua_topointer", "luaL_loadbuffer", "lua_pcall" };
    uint64_t len = 0x0;
    uint32_t start = 0x0;
    uint32_t leaf = 0x0;
    uint32_t idx = 0x0;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !args || argc < 2U)) { return LDG_ERR_FUNC_ARG_INVALID; }

    if (argc == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "eval") == LDG_TRUTH_TRUE)
    {
        res = lua_complete_path(args[1], &start, &leaf);
        if (res != LDG_ERR_AOK) { return libresteam_ext_repl_complete_begin(ctx, "", LDG_TRUTH_FALSE); }

        res = libresteam_ext_repl_complete_begin(ctx, args[1] + leaf, LDG_TRUTH_FALSE);
        if (res != LDG_ERR_AOK) { return res; }

        if (!ctx->api || ctx->api->size < sizeof(*ctx->api) || !ctx->api->lua || ctx->api->lua_api_size < sizeof(libresteam_backends_lua_api_t) || !((const libresteam_backends_lua_api_t *)ctx->api->lua)->eval_stream) { return LDG_ERR_NOT_INIT; }

        res = ldg_str_fmt(ctx->lua_src, sizeof(ctx->lua_src), &len, LUA_COMPLETE_SCRIPT, args[1] + start);
        return res == LDG_ERR_AOK ? ((const libresteam_backends_lua_api_t *)ctx->api->lua)->eval_stream(((const libresteam_backends_lua_api_t *)ctx->api->lua)->ctx, ctx->lua_src, len, lua_complete_emit, ctx, &ctx->lua_res) : res;
    }

    res = libresteam_ext_repl_complete_begin(ctx, args[argc - 1U], LDG_TRUTH_TRUE);
    if (res != LDG_ERR_AOK) { return res; }

    if (argc == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "auto") == LDG_TRUTH_TRUE)
    {
        res = libresteam_ext_repl_complete_add(ctx, "unravel", "");
        return res == LDG_ERR_AOK ? libresteam_ext_repl_complete_add(ctx, LUA_UNRAVEL_TWO_NAME, "") : res;
    }

    if (argc == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "bind") == LDG_TRUTH_TRUE) { return libresteam_ext_repl_complete_add(ctx, "5.1", ""); }

    if (argc == 2U && libresteam_ext_repl_cmd_eq_is(args[0], "api") == LDG_TRUTH_TRUE) { for (idx = 0; res == LDG_ERR_AOK && idx < sizeof(apis) / sizeof(apis[0]); idx++) { res = libresteam_ext_repl_complete_add(ctx, apis[idx], ""); } }

    return res;
}

uint32_t libresteam_ext_repl_lua_cmd(void *user, uint32_t argc, char **args)
{
    libresteam_ext_repl_ctx_t *ctx = user;
    const libresteam_backends_lua_api_t *lua = 0x0;
    uint64_t addr = 0x0;
    uint64_t number = 0x0;
    uint64_t limit = UINT32_MAX;
    uint32_t res = LDG_ERR_AOK;

    if (LDG_UNLIKELY(!ctx || !args || !argc || !ctx->api)) { return LDG_ERR_FUNC_ARG_NULL; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "help") == LDG_TRUTH_TRUE) { return argc == 1U ? lua_help(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (ctx->api->size < sizeof(*ctx->api) || !ctx->api->lua) { return LDG_ERR_UNSUPPORTED; }

    if (ctx->api->lua_api_size < sizeof(libresteam_backends_lua_api_t)) { return LDG_ERR_UNSUPPORTED; }

    lua = ctx->api->lua;
    if (libresteam_ext_repl_cmd_eq_is(args[0], "auto") == LDG_TRUTH_TRUE)
    {
        if (argc != LUA_AUTO_ARG_CUNT) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (libresteam_ext_repl_cmd_eq_is(args[1], "unravel") == LDG_TRUTH_TRUE) { return lua_auto_start(ctx, LUA_AUTO_PROFILE_UNRAVEL); }

        if (libresteam_ext_repl_cmd_eq_is(args[1], LUA_UNRAVEL_TWO_NAME) == LDG_TRUTH_TRUE) { return lua_auto_start(ctx, LUA_AUTO_PROFILE_UNRAVEL_TWO); }

        return LDG_ERR_FUNC_ARG_INVALID;
    }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "status") == LDG_TRUTH_TRUE) { return argc == 1U ? lua_status(ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "detach") == LDG_TRUTH_TRUE) { return argc == 1U ? lua->detach(lua->ctx) : LDG_ERR_FUNC_ARG_INVALID; }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "eval") == LDG_TRUTH_TRUE || libresteam_ext_repl_cmd_eq_is(args[0], "evalmem") == LDG_TRUTH_TRUE) { return lua_eval_cmd(ctx, argc, args); }

    if (libresteam_ext_repl_cmd_eq_is(args[0], "bind") == LDG_TRUTH_TRUE)
    {
        if (argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (libresteam_ext_repl_cmd_eq_is(args[1], "5.1") != LDG_TRUTH_TRUE) { return LDG_ERR_UNSUPPORTED; }

        res = lua->module_bind(lua->ctx, args[2], LIBRESTEAM_BACKENDS_LUA_ABI_51);
    }
    else if (libresteam_ext_repl_cmd_eq_is(args[0], "api") == LDG_TRUTH_TRUE)
    {
        if (argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_native_addr_get(ctx, args[2], &addr);
        if (res == LDG_ERR_AOK) { res = lua->func_bind(lua->ctx, args[1], addr, LIBRESTEAM_BACKENDS_LUA_ABI_51); }
    }
    else if (libresteam_ext_repl_cmd_eq_is(args[0], "attach") == LDG_TRUTH_TRUE)
    {
        if (argc != 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        res = libresteam_ext_repl_native_addr_get(ctx, args[1], &addr);
        if (res == LDG_ERR_AOK) { res = libresteam_ext_repl_num_get(args[2], &number); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (number > UINT32_MAX) { return LDG_ERR_RANGE; }

        res = lua->attach(lua->ctx, addr, (uint32_t)number);
    }
    else if (libresteam_ext_repl_cmd_eq_is(args[0], "stack") == LDG_TRUTH_TRUE)
    {
        if (argc > 3U) { return LDG_ERR_FUNC_ARG_INVALID; }

        if (argc >= 2U) { res = libresteam_ext_repl_num_get(args[1], &number); }

        if (res == LDG_ERR_AOK && argc == 3U) { res = libresteam_ext_repl_num_get(args[2], &limit); }

        if (LDG_UNLIKELY(res != LDG_ERR_AOK)) { return res; }

        if (number > UINT32_MAX || limit > UINT32_MAX) { return LDG_ERR_RANGE; }

        res = lua->stack_stream(lua->ctx, (uint32_t)number, (uint32_t)limit, lua_val_emit, ctx, &ctx->lua_res);
        return res == LDG_ERR_AOK ? lua_summary(ctx) : res;
    }
    else { return LDG_ERR_NOT_FOUND; }

    return res == LDG_ERR_AOK ? lua_status(ctx) : res;
}
