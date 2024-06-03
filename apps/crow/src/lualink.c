#include "lualink.h"
#include "print_funcs.h"
#include "render.h"

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


#define WATCHDOG_FREQ      0x100000 // ~1s how often we run the watchdog
#define WATCHDOG_COUNT     2        // how many watchdogs before 'frozen'


// Private prototypes
static void Lua_linkctolua( lua_State* L );
static float Lua_check_memory( void );
static int Lua_call_usercode( lua_State* L, int nargs, int nresults );
static int Lua_handle_error( lua_State* L );
static void timeouthook( lua_State* L, lua_Debug* ar );
void print_ser(const char* msg);

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

#define STRING_NUM(n) #n
#define LUA_ARG_ERROR(n) "error: requires " STRING_NUM(n) " arguments"
#define lua_check_num_args(n)                   \
    if (lua_gettop(L) != n) {                   \
        return luaL_error(L, LUA_ARG_ERROR(n)); \
    }

// C-fns accessible to lua

// NB these static functions are prefixed  with '_'
// to avoid shadowing similar-named extern functions in other modules
// and also to distinguish from extern 'L_' functions.

static int _debug( lua_State *L )
{
    const char* msg = luaL_checkstring(L, 1);
    lua_pop( L, 1 );
    /* char str[80];
     * sprintf(str, "%s\n", (char*)msg);
     * print_dbg(str); */
    print_dbg("\r\n ");
    print_dbg(msg);
    lua_settop(L, 0);
    return 0;
}

static int _screen_text( lua_State *L ) {
    lua_check_num_args(1);
    clearln();
    appendln((const char*)luaL_checkstring(L, 1));
    endln();
    lua_settop(L, 0);

    font_string_region_clip(lineRegion, lineBuf, 0, 0, 0xa, 0);

    return 0;
}

// array of all the available functions
static const struct luaL_Reg libCrow[]=
    // bootstrap
    {
        // { "c_dofile"         , l_bootstrap_dofile },
        { "debug_usart"      , _debug            },
        { "prn"              , _debug            },
        // screen
        { "screen_text"      , _screen_text      },

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

void print_ser(const char* msg) {
    print_dbg("\r\n ");
    print_dbg(msg);
}

void print_ser_err(const char* msg) {
    print_dbg("\r\n ");
    print_dbg("error: ");
    print_dbg(msg);
}

uint8_t Lua_eval( lua_State*     L
                  , const char*    script
                  , size_t         script_len
                  , const char*    chunkname
                  ){
    int error = luaL_loadbuffer( L, script, script_len, chunkname );
    if( error != LUA_OK ){
        print_ser_err((char*)lua_tostring( L, -1 ));
        lua_pop( L, 1 );
        return 1;
    }

    if( (error |= Lua_call_usercode( L, 0, 0 )) != LUA_OK ){
        lua_pop( L, 1 );
        switch( error ){
        case LUA_ERRSYNTAX: print_ser_err("syntax error."); break;
        case LUA_ERRMEM:    print_ser_err("not enough memory."); break;
        case LUA_ERRRUN:    print_ser_err("runtime error."); break;
        case LUA_ERRERR:    print_ser_err("error in error handler."); break;
        default: break;
        }
        return 1;
    }
    return 0;
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

// Watchdog timer for infinite looped Lua scripts
volatile int watchdog = WATCHDOG_COUNT;
static void timeouthook( lua_State* L, lua_Debug* ar )
{
    if( --watchdog <= 0 ){
        print_ser("CPU timed out.");
        lua_sethook(L, timeouthook, LUA_MASKLINE, 0); // error until top
        luaL_error(L, "user code timeout exceeded");
    }
}

static int Lua_handle_error( lua_State *L )
{
    const char *msg = lua_tostring( L, 1 );
    if( msg == NULL ){
        if( luaL_callmeta( L, 1, "__tostring" )
            && lua_type ( L, -1 ) == LUA_TSTRING ) {
            return 1;
        } else {
            msg = lua_pushfstring( L
                                   , "(error object is a %s value)"
                                   , luaL_typename( L, 1 ) );
        }
    }
    luaL_traceback( L, L, msg, 1 );
    char* traceback = (char*)lua_tostring( L, -1 );
    print_ser(traceback);
    return 1;
}

static int Lua_call_usercode( lua_State* L, int nargs, int nresults )
{
    lua_sethook(L, timeouthook, LUA_MASKCOUNT, WATCHDOG_FREQ); // reset timeout hook
    watchdog = WATCHDOG_COUNT; // reset timeout hook counter

    int errFunc = lua_gettop(L) - nargs;
    lua_pushcfunction( L, Lua_handle_error );
    lua_insert( L, errFunc );
    int status = lua_pcall(L, nargs, nresults, errFunc);
    lua_remove( L, errFunc );

    lua_sethook(L, timeouthook, 0, 0);

    return status;
}
