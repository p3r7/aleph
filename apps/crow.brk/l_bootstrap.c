#include "l_bootstrap.h"


void l_bootstrap_init(lua_State* L){
    // collectgarbage('setpause', 55)
    lua_gc(L, LUA_GCSETPAUSE, 55);
    lua_gc(L, LUA_GCSETSTEPMUL, 260);

    // dofile just calls c_dofile
    lua_getglobal(L, "c_dofile");
    lua_setglobal(L, "dofile");

    // crowlib.lua now only contains our print() definition
    // _c = dofile('lua/crowlib.lua')
    lua_pushliteral(L, "lua/crowlib.lua");
    l_bootstrap_dofile(L); // hotrod without l_call
    lua_settop(L, 0);

    // _c = {}
    lua_newtable(L);
    lua_setglobal(L, "_c");

    // crow = _c
    lua_getglobal(L, "_c");
    lua_setglobal(L, "crow");

    // crowlib C extensions
    l_crowlib_init(L);

    // track all user-created globals
    luaL_dostring(L,
                  "_user={}\n"
                  "local function trace(t,k,v)\n"
                  "_user[k]=true\n"
                  "rawset(t,k,v)\n"
                  "end\n"
                  "setmetatable(_G,{ __newindex = trace })\n"
                  );

    // perform two full garbage collection cycles for full cleanup
    lua_full_gc(L);
}
