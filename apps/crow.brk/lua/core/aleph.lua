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

-- startup function will be run after I/O subsystems are initialized,
-- but before I/O event loop starts ticking (see readme-script.md)
_startup = function()
  require('core/startup')
end

_startup()
