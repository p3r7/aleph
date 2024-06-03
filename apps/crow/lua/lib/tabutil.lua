--- table utility
-- @module lib.tabutil
-- @alias tab

local tab = {}

--- print the contents of a table
-- @tparam table t table to print
tab.print = function(t)
  for k,v in pairs(t) do print(k .. '\t' .. tostring(v)) end
end

--- return a lexigraphically sorted array of keys for a table
-- @tparam table t table to sort
-- @treturn table sorted table
tab.sort = function(t)
  local keys = {}
  for k in pairs(t) do table.insert(keys, k) end
  table.sort(keys)
  return keys
end

--- count the number of entries in a table;
-- unlike table.getn() or #table, nil entries won't break the loop
-- @tparam table t table to count
-- @treturn number count
tab.count = function(t)
  local c = 0
  for _ in pairs(t) do c = c + 1 end
  return c
end

--- search table for element
-- @tparam table t table to check
-- @param e element to look for
-- @treturn boolean t/f is element is present
tab.contains = function(t,e)
  for index, value in ipairs(t) do
    if value == e then return true end
  end
  return false
end


--- given a simple table of primitives,
--- "invert" it so that values become keys and vice versa.
--- this allows more efficient checks on multiple values
-- @param t: a simple table
tab.invert = function(t)
  local inv = {}
  for k,v in pairs(t) do
    inv[v] = k
  end
  return inv
end


--- search table for element, return key
-- @tparam table t table to check
-- @param e element to look for
-- @return key, nil if not found
tab.key = function(t,e)
  for index, value in ipairs(t) do
    if value == e then return index end
  end
  return nil
end

--- split multi-line string into table of strings
-- @tparam string str string with line breaks
-- @treturn table table with entries for each line
tab.lines = function(str)
  local t = {}
  local function helper(line)
    table.insert(t, line)
    return ""
  end
  helper((str:gsub("(.-)\r?\n", helper)))
  return t
end


--- split string into table with delimiter
-- @tparam string inputstr : string to split
-- @tparam string sep : delimiter
tab.split = function(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end
	local t={}
	for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
		table.insert(t, str)
	end
	return t
end


--- Create a read-only proxy for a given table.
-- @param params params.table is the table to proxy, params.except a list of writable keys, params.expose limits which keys from params.table are exposed (optional)
-- @treturn table the proxied read-only table
function tab.readonly(params)
  local t = params.table
  local exceptions = params.except or {}
  local proxy = {}
  local mt = {
    __index = function(_, k)
      if params.expose == nil or tab.contains(params.expose, k) then
        return t[k]
      end
      return nil
    end,
    __newindex = function (_,k,v)
      if (tab.contains(exceptions, k)) then
        t[k] = v
      else
        error("'"..k.."', a read-only key, cannot be re-assigned.")
      end
    end,
    __pairs = function (_) return pairs(proxy) end,
    __ipairs = function (_) return ipairs(proxy) end,
  }
  setmetatable(proxy, mt)
  return proxy
end


--- return new table, gathering values:
--- - first from default_values,
--- - then from (i.e. overridden by) custom_values
--- nils in custom_values are ignored
-- @tparam table default_values base values (provides keys & fallback values)
-- @tparam table custom_values override values (take precedence)
-- @treturn table composite table
function tab.gather(default_values, custom_values)
  local result = {}
  for k,v in pairs(default_values) do
    result[k] = (custom_values[k] ~= nil) and custom_values[k] or v
  end
  return result
end

--- mutate first table, updating values from second table.
--- new keys from second table will be added to first.
--- nils in updated_values are ignored
-- @tparam table table_to_mutate table to mutate
-- @tparam table updated_values override values (take precedence)
-- @treturn table composite table
function tab.update(table_to_mutate, updated_values)
  for k,v in pairs(updated_values) do
    if updated_values[k] ~= nil then
      table_to_mutate[k] = updated_values[k]
    end
  end
  return table_to_mutate
end

--- Create a new table with all values that pass the test implemented by the provided function.
-- @tparam table t table to check
-- @param condition callback function that tests all values of provided table, passes value and key as arguments
-- @treturn table table with values that pass the test
tab.select_values = function(tbl, condition)
  local t = {}

  for k,v in pairs(tbl) do
    if condition(v,k) then t[k] = v end
  end

  return t
end

return tab
