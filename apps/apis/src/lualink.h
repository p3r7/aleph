#pragma once

#include <stdint.h>
#include <stdbool.h>

// Lua itself
#include "../../../lua/src/lua.h" // lua_State*

lua_State* Lua_Init(void);
lua_State* Lua_Reset(void);
void Lua_DeInit(void);
void Lua_load_entrypoint( void );

lua_State* Lua_ReInit_Environment(lua_State*);

uint8_t Lua_eval( lua_State*     L
                  , const char*    script
                  , size_t         script_len
                  , const char*    chunkname
                  );
