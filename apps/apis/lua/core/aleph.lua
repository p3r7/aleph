-- aleph.lua
-- entrypoint script, called by C layer
-- defines top-level global tables and functions needed by other modules
-- external c functions are in the _aleph table
-- external callbacks in the aleph table, which also includes management

--- System utilities
-- @module aleph
aleph = {}

aleph.version = {}
aleph.version.update = "0.0.1"

aleph.platform = 2

-- ------------------------------------------------------------------------
-- paths

local lua_root = "/lua"
local lua_script_root = "/code"
local core = lua_root .. "/core/?.lua;"
local lib = lua_root .. "/lib/?.lua;"
local code = lua_script_root .. "/?.lua;"

package.path = core .. lib .. package.path

local tab = require 'tabutil'

-- local p = _path
-- p.this = tab.readonly{
--   table = norns.state,
--   expose = {'data', 'path', 'lib'}
-- }
-- paths = tab.readonly{ table = p }

_path = {}
_path.code = code

-- ------------------------------------------------------------------------

-- startup function will be run after I/O subsystems are initialized,
-- but before I/O event loop starts ticking (see readme-script.md)
_startup = function()
  print("lua startup...")
  require('startup')
end

_startup()
