tab = require 'tabutil'
util = require 'util'

require 'math'
inf = math.huge

_old_print = print
print = debug_usart


function include(file)
  local dirs = {_path.code}
  for _, dir in ipairs(dirs) do
    local p = dir..file..'.lua'
    if util.file_exists(p) then
      print("including "..p)
      return dofile(p)
    end
  end

  -- didn't find anything
  print("### MISSING INCLUDE: "..file)
  error("MISSING INCLUDE: "..file,2)
end
