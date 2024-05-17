#include "lualink.h"
#include "print_funcs.h"

#include <stdio.h>
#include <string.h> // strcmp(), strlen()
#include <stdlib.h>

// Lua itself
//#include "../../../lua/src/lua.h" // in header
#include "../../../lua/src/lauxlib.h"
#include "../../../lua/src/lualib.h"

// Lua lib C implementations
// as much low-level functionality is in here as possible
// thus keeping the lua VM as free as possible
// #include "l_bootstrap.h"

// Private prototypes
static void Lua_linkctolua( lua_State* L );
static float Lua_check_memory( void );

lua_State* L; // global access for 'reset-environment'

lua_State* Lua_Init(void)
{
    L = luaL_newstate();
    luaL_openlibs(L);
    Lua_linkctolua(L);
    // l_bootstrap_init(L); // redefine dofile(), print(), load crowlib
    return L;
}

lua_State* Lua_ReInit_Environment(lua_State* L){
    // clear user-created globals
    luaL_dostring(L, "for k,_ in pairs(_user) do\n"
                  "_G[k] = nil\n"
                  "end\n"
                  "_G._user = {}\n");
    // init() has to be manually reset to the void function
    // l_crowlib_emptyinit(L);
    // cleanup memory to get back to a blank slate
    lua_gc(L, LUA_GCCOLLECT, 1);
    lua_gc(L, LUA_GCCOLLECT, 1);
    return L;
}

lua_State* Lua_Reset( void )
{
    print_dbg("\r\n Lua_Reset");

    /* // cleanup any C-based event generators
     * Metro_stop_all();
     * for( int i=0; i<2; i++ ){
     *     Detect_none( Detect_ix_to_p(i) );
     * }
     * for( int i=0; i<4; i++ ){
     *     S_toward( i, 0.0, 0.0, SHAPE_Linear, NULL );
     * }
     * events_clear();
     * clock_cancel_coro_all();
     *
     * // set all crowlib modules to default states / callbacks
     * l_crowlib_crow_reset(L); */

    // delete all user globals, but keep overall Lua env
    // we do this to avoid memory fragmentation when resetting env
    return Lua_ReInit_Environment(L);
}

void Lua_DeInit(void)
{
    lua_close(L);
}

// C-fns accessible to lua

// NB these static functions are prefixed  with '_'
// to avoid shadowing similar-named extern functions in other modules
// and also to distinguish from extern 'L_' functions.

static int _debug( lua_State *L )
{
    const char* msg = luaL_checkstring(L, 1);
    lua_pop( L, 1 );
    char str[80];
    sprintf(str, "%s\n", (char*)msg);
    print_dbg(str);
    lua_settop(L, 0);
    return 0;
}

// array of all the available functions
static const struct luaL_Reg libCrow[]=
    // bootstrap
    {
        // { "c_dofile"         , l_bootstrap_dofile },
        { "debug_usart"      , _debug            },

        { NULL               , NULL              }
    };
// make functions available to lua
static void Lua_linkctolua( lua_State *L )
{
    // Make C fns available to Lua
    uint8_t fn = 0;
    while( libCrow[fn].func != NULL ){
        lua_register( L, libCrow[fn].name, libCrow[fn].func );
        fn++;
    }
}

static float Lua_check_memory( void )
{
    lua_getglobal(L,"collectgarbage");
    lua_pushstring(L, "count");
    lua_pcall(L,1,1,0); // NOT PROTECTED (called from watchdog)
    float mem = luaL_checknumber(L, 1);
    lua_pop(L,1);
    return mem;
}
