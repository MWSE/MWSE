-- First, look for objects in the core folder. DLL files may also exist in the root folder.
package.path = ".\\Data Files\\MWSE\\core\\?.lua;.\\Data Files\\MWSE\\core\\?\\init.lua;"
package.cpath = "?.dll;.\\Data Files\\MWSE\\core\\?.dll;"

-- Next, look in the library folders.
package.path = package.path .. ".\\Data Files\\MWSE\\core\\lib\\?.lua;.\\Data Files\\MWSE\\core\\lib\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\core\\lib\\?.dll;"
package.path = package.path .. ".\\Data Files\\MWSE\\lib\\?.lua;.\\Data Files\\MWSE\\lib\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\lib\\?.dll;"

-- Third, look in the mods folder.
package.path = package.path .. ".\\Data Files\\MWSE\\mods\\?.lua;.\\Data Files\\MWSE\\mods\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\mods\\?.dll;"

-- Provide backwards compatibility for old versions of MWSE 2.1. This will be removed before a stable release.
package.path = package.path .. ".\\Data Files\\MWSE\\lua\\?.lua;.\\Data Files\\MWSE\\lua\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\lua\\?.dll;"

--- Converts a given module name into a standard format, and ensures that it is lowercase.
--- @param s string
--- @return string
local function convertModuleName(s)
	local t = type(s)
	if t == "string" then
		return s:gsub("[/\\]", "."):lower()
	elseif t == "number" then
		return tostring(s):gsub("[/\\]", "."):lower()
	else
		error("bad argument #1 to 'require' (string expected, got "..t..")", 3)
	end
end

--- A tweaked version of pygy/require.lua (https://github.com/pygy/require.lua)
--- to make all module names lowercased.
--- @param name string
--- @return any
function require(name)
	name = convertModuleName(name)
	local module = package.loaded[name]
	if module then return module end

	local msg = {}
	local loader, param
	for _, searcher in ipairs(package.searchers) do
		loader, param = searcher(name)
		if type(loader) == "function" then break end
		if type(loader) == "string" then
			-- `loader` is actually an error message
			msg[#msg + 1] = loader
		end
		loader = nil
	end

	if loader == nil then
		error("module '" .. name .. "' not found: " .. table.concat(msg), 2)
		return
	end

	local res = loader(name, param)
	if res ~= nil then
		module = res
	elseif not package.loaded[name] then
		module = true
	else
		module = package.loaded[name]
	end

	package.loaded[name] = module
	return module
end

--- A dictionary keeping track of what files have already tried to be included.
--- @type table<string, boolean>
package.noinclude = {}

--- A tweaked version of pygy/require.lua (https://github.com/pygy/require.lua)
--- to make all module names lowercased. Instead of erroring when a module
--- isn't found, return nil.
--- @param name string
--- @return any
function include(name)
	name = convertModuleName(name)
	local module = package.loaded[name]
	if module then return module end
	if package.noinclude[name] then return end

	local msg = {}
	local loader, param
	for _, searcher in ipairs(package.searchers) do
		loader, param = searcher(name)
		if type(loader) == "function" then break end
		if type(loader) == "string" then
			-- `loader` is actually an error message
			msg[#msg + 1] = loader
		end
		loader = nil
	end

	if loader == nil then
		package.noinclude[name] = true
		return
	end

	local res = loader(name, param)
	if res ~= nil then
		module = res
	elseif not package.loaded[name] then
		module = true
	else
		module = package.loaded[name]
	end

	package.loaded[name] = module
	return module
end

-- Custom dofile that respects package pathing and supports lua's dot notation for paths.
local fileLocationCache = {}
local originalDoFile = dofile
function dofile(path)
	assert(path and type(path) == "string")

	-- Replace . and / with \, and remove .lua extension if it exists.
	local standardizedPath = path:gsub("[/.]", "\\"):lower()
	if (standardizedPath:endswith("\\lua")) then
		standardizedPath = standardizedPath:sub(0, -5)
	end

	-- Any results in cache?
	local cachedPath = fileLocationCache[standardizedPath]
	if (cachedPath) then
		return originalDoFile(cachedPath)
	end

	-- First pass: Direct load. Have to manually add the .lua extension.
	if (lfs.fileexists(tes3.installDirectory .. "\\" .. standardizedPath .. ".lua")) then
		fileLocationCache[standardizedPath] = standardizedPath .. ".lua"
		return originalDoFile(standardizedPath .. ".lua")
	end

	-- Check all package paths.
	for ppath in package.path:gmatch("[^;]+") do
		local adjustedPath = ppath:gsub("?", standardizedPath)
		if (lfs.fileexists(tes3.installDirectory .. "\\" .. adjustedPath)) then
			fileLocationCache[standardizedPath] = adjustedPath
			return originalDoFile(adjustedPath)
		end
	end

	-- No result? Error.
	error("dofile: Could not resolve path " .. path)
end

local function addUserFriendlyNameToSolType(friendlyName, metatable)
	metatable.__type.friendlyName = friendlyName
end

for friendlyName, maybeUserdataType in pairs(_G) do
	pcall(addUserFriendlyNameToSolType, friendlyName, maybeUserdataType)
end

local function getUserdataTypeName(variable)
    return variable.__type.friendlyName
end

local originalType = type
function type(variable)
	local baseType = originalType(variable)
	if (baseType == "userdata") then
		local success, typeName = pcall(getUserdataTypeName, variable)
		if (success) then
			return baseType, typeName
		end
	end
	return baseType
end


-------------------------------------------------
-- Global includes
-------------------------------------------------

_G.tes3 = require("tes3")
_G.mge = require("mge")
_G.ni = require("ni")
_G.event = require("event")
_G.json = require("dkjson")
_G.toml = require("toml")

-- Prevent requiring socket.core before socket from causing issues.
local socket = require("socket")
local socket_core = require("socket.core")


-------------------------------------------------
-- Translation helpers
-------------------------------------------------

local i18n = require("i18n")

-- TODO: Add these.
local pluralizationFunctions = {}

-- Metatable used to wrap around i18n so mods don't have to keep passing their mod name in translation calls/files.
local i18nWrapper = {}

function i18nWrapper:set(key, value)
	i18n.set(self.mod .. "." .. key, value)
end

function i18nWrapper:translate(key, data)
	return i18n.translate(self.mod .. "." .. key, data)
end

i18nWrapper.__call = i18nWrapper.translate

local function convertUTF8Table(t, language)
	for k, v in pairs(t) do
		local vType = type(v)
		if (vType == "string") then
			t[k] = mwse.iconv(language, v)
		elseif (vType == "table") then
			convertUTF8Table(v, language)
		end
	end
end

-- Helper around i18n.load with safety checks, package.path support, and loads the translation into its own namespace.
local function loadLocaleFile(mod, locale)
	local success, contents = pcall(dofile, string.format("%s.i18n.%s", mod, locale))
	if (success) then
		assert(type(contents) == "table", string.format("Translation file for mod %q does not have valid translation file for locale %q.", mod, locale))

		-- Convert encoding from UTF8 to the right type.
		convertUTF8Table(contents, tes3.getLanguageCode())

		-- Load the translation data.
		i18n.load({ [locale] = { [mod] = contents } })
	end
	return success
end

--- @param mod string
--- @return fun(key: string, data: any?): string i18n
function mwse.loadTranslations(mod)
	-- Lazy set language, since tes3.getLanguage() isn't available.
	local language = tes3.getLanguage() or "eng"
	i18n.setLocale(language, pluralizationFunctions[language])

	-- Load the language files.
	local loadedLanguage = false
	local loadedDefault = loadLocaleFile(mod, "eng")
	if (language ~= "eng") then
		loadedLanguage = loadLocaleFile(mod, language)
	end
	assert(loadedDefault or loadedLanguage, "Could not load any valid i18n files.")

	-- We create a wrapper around i18n prefixing with the mod key.
	return setmetatable({ mod = mod }, i18nWrapper)
end

-------------------------------------------------
-- Extend base API: table
-------------------------------------------------

-- Add LuaJIT extensions.
require("table.clear")
require("table.new")

-- The # operator only really makes sense for continuous arrays. Get the real value.
function table.size(t)
	local count = 0
	for _ in pairs(t) do
		count = count + 1
	end
	return count
end

function table.empty(t, deepCheck)
	if (deepCheck) then
		for _, v in pairs(t) do
			if (type(v) ~= "table" or not table.empty(v, true)) then
				return false
			end
		end
	else
		for _ in pairs(t) do
			return false
		end
	end
	return true
end

function table.choice(t)
	-- We can abort on empty tables.
	local size = table.size(t)
	if (size == 0) then
		return
	end

	-- Call next a random amount of times up to the size of the table to get a random key.
	local key = nil
	local nextCount = math.random(size)
	for _ = 1, nextCount do
		key = next(t, key)
	end

	return t[key], key
end

function table.find(t, value)
	for i, v in pairs(t) do
		if (v == value) then
			return i
		end
	end
end

function table.contains(t, value)
	return table.find(t, value) ~= nil
end


function table.equal(t1, t2)

	-- Try a quick basic equality check.
	if (t1 == t2) then
		return true
	end

	-- Make sure both inputs are tables.
	if (type(t1) ~= "table" or type(t2) ~= "table") then
		return false
	end

	-- Loop through pairs and see if all values match from t1 -> t2.
	local size1 = 0
	-- Store the function locally for faster function calls.
	local eq = table.equal
	for k, v1 in pairs(t1) do
		-- Note: If `v1 ~= v2`, then the recursive call to `table.equal` will
		-- result in a redundant comparison of `v1` and `v2`.
		-- But, testing shows that for highly similar tables, this approach is faster
		-- than only checking `not table.equal(v1, v2)`.
		-- This is likely due to the overhead from function calls.

		local v2 = t2[k]
		if (v1 ~= v2 and not eq(v1, v2)) then
			return false
		end

		size1 = size1 + 1
	end

	-- We can assume t1 == t2 if all values match for t1 -> t2 and both tables have the same size.
	return size1 == table.size(t2)
end


function table.removevalue(t, value)
	local i = table.find(t, value)
	if (i ~= nil) then
		table.remove(t, i)
		return true
	end
	return false
end

function table.copy(from, to)
	if (to == nil) then
		to = {}
	end

	for k, v in pairs(from) do
		to[k] = v
	end

	return to
end

function table.deepcopy(t)
	local copy = nil
	if type(t) == "table" then
		copy = {}
		for k, v in next, t, nil do
			copy[table.deepcopy(k)] = table.deepcopy(v)
		end
		setmetatable(copy, table.deepcopy(getmetatable(t)))
	else -- number, string, boolean, etc
		copy = t
	end
	return copy
end

function table.copymissing(to, from)
	for k, v in pairs(from) do
		if (type(to[k]) == "table" and type(v) == "table") then
			table.copymissing(to[k], v)
		else
			if (to[k] == nil) then
				to[k] = v
			end
		end
	end
end

function table.traverse(t, k)
	k = k or "children"
	local function iter(nodes)
		for i, node in ipairs(nodes or t) do
			if node then
				coroutine.yield(node)
				if node[k] then
					iter(node[k])
				end
			end
		end
	end
	return coroutine.wrap(iter)
end

function table.keys(t, sort)
	local keys = {}
	for k, _ in pairs(t) do
		table.insert(keys, k)
	end

	if (sort) then
		if (sort == true) then
			sort = nil
		end
		table.sort(keys, sort)
	end

	return keys
end

function table.values(t, sort)
	local values = {}
	for _, v in pairs(t) do
		table.insert(values, v)
	end

	if (sort) then
		if (sort == true) then
			sort = nil
		end
		table.sort(values, sort)
	end

	return values
end

function table.invert(t)
	local inverted = {}
	for k, v in pairs(t) do
		inverted[v] = k
	end
	return inverted
end

function table.swap(t, key, value)
	local old = t[key]
	t[key] = value
	return old
end

function table.get(t, key, default)
	local value = t[key]
	if (value == nil) then
		return default
	end
	return value
end

function table.getset(t, key, default)
	local value = t[key]
	if (value ~= nil) then
		return value
	end

	t[key] = default
	return default
end

function table.wrapindex(t, index)
	local size = #t
	local newIndex = index % size
	if (newIndex == 0) then
		newIndex = size
	end
	return newIndex
end

function table.shuffle(t, n)
	n = n or #t
	for i = n, 2, -1 do
		local j = math.random(i)
		t[i], t[j] = t[j], t[i]
	end
end


-------------------------------------------------
-- Extend base table: Add binary search/insert
-------------------------------------------------

--[[
	table.binsearch( table, value [, comp [, findAll] ] )

	finds a value by performing a binary search.
	If the `value` is found:
		if `findAll` evaluates to true, then the lowest matching index and the highest matching index will be returned.
		otherwise, the first index to match will be returned.
	If `comp` is given:
		comparisons will be performed as though the array was sorted via `table.sort(tbl, comp)`.
	If `findAll == true`:
		two indices will be returned, corresponding to the lowest and highest indices whose corresponding elements are equal to `value`.
	Return value:
		on success: two integers: `lowestMatch, highestMatch`
		on failure: nil
]]--
function table.binsearch(tbl, value, comp, findAll)
	-- initialize the index variables
	local first, last, midpt = 1, #tbl, 0
	local floor = math.floor
	comp = comp or function(a,b) return a < b end -- set to default if not given
	-- Binary Search
	while first <= last do
		-- calculate middle
		midpt = floor((first + last) / 2)

		if comp(value, tbl[midpt]) then -- `value < tbl[midpt]`

			last = midpt - 1 -- value is in the first half

		elseif comp(tbl[midpt], value) then -- `tbl[midpt] < value`

			first = midpt + 1 -- value is in the second half

		else -- `tbl[midpt] == value`

			-- only want the first match? bail
			if not findAll then return midpt, midpt end

			-- find all the remaining matches
			local lowestMatch, highestMatch = midpt, midpt
			while value == tbl[lowestMatch - 1] do
				lowestMatch = lowestMatch - 1
			end
			while value == tbl[highestMatch + 1] do
				highestMatch = highestMatch + 1
			end
			return lowestMatch, highestMatch
		end
	end
end

--[[
	table.bininsert( table, value [, comp] )

	Inserts a given value through BinaryInsert into the table sorted by [, comp].

	If 'comp' is given, then it must be a function that receives
	two table elements, and returns true when the first is less
	than the second, e.g. comp = function(a, b) return a > b end,
	will give a sorted table, with the biggest value on position 1.
	[, comp] behaves as in table.sort(table, value [, comp])
	returns the index where 'value' was inserted
]]--
local fcomp_default = function( a,b ) return a < b end
function table.bininsert(t, value, comp)
	-- Initialise compare function
	local comp = comp or fcomp_default
	--  Initialise numbers
	local iStart,iEnd,iMid,iState = 1,#t,1,0
	-- Get insert position
	while iStart <= iEnd do
		-- calculate middle
		iMid = math.floor( (iStart+iEnd)/2 )
		-- compare
		if comp( value,t[iMid] ) then
			iEnd,iState = iMid - 1,0
		else
			iStart,iState = iMid + 1,1
		end
	end
	table.insert( t,(iMid+iState),value )
	return (iMid+iState)
end


-- functional programming stuff

function table.map(t, f, ...)
	local tbl = {}
	for k, v in pairs(t) do
		tbl[k] = f(k, v, ...)
	end
	return tbl
end

function table.filter(t, f, ...)
	local tbl = {}
	for k, v in pairs(t) do
		if f(k, v, ...) then
			tbl[k] = v
		end
	end
	return tbl
end

function table.filterarray(t, f, ...)
	local tbl = {}
	for i, v in ipairs(t) do
		if f(i, v, ...) then
			table.insert(tbl, v)
		end
	end
	return tbl
end

-------------------------------------------------
-- Extend base API: string
-------------------------------------------------

function string.startswith(haystack, needle)
	return string.sub(haystack, 1, string.len(needle)) == needle
end
getmetatable("").startswith = string.startswith

function string.endswith(haystack, needle)
	return needle=='' or string.sub(haystack, -string.len(needle)) == needle
end
getmetatable("").endswith = string.endswith

function string.multifind(s, patterns, index, plain)
	for _, pattern in ipairs(patterns) do
		local r = { string.find(s, pattern, index, plain) }
		if (#r > 0) then
			return pattern, unpack(r)
		end
	end
end
getmetatable("").multifind = string.multifind

function string.insert(s1, s2, pos)
	return s1:sub(1, pos) .. s2 .. s1:sub(pos + 1)
end
getmetatable("").insert = string.insert

function string.split(str, sep)
	if sep == nil then
		sep = "%s"
	end
	local t = {}
	for str in string.gmatch(str, "([^" .. sep .. "]+)") do
		table.insert(t, str)
	end
	return t
end
getmetatable("").split = string.split

function string.trim(s)
	return string.match(s, '^()%s*$') and '' or string.match(s, '^%s*(.*%S)')
end
getmetatable("").trim = string.trim


-------------------------------------------------
-- Extend base API: debug
-------------------------------------------------

local function getNthLine(fileName, n)
	local f = io.open(fileName, "r")
	if (f == nil) then
		return
	end

	local i = 1
	for line in f:lines() do
		if i == n then
			f:close()
			return line
		end
		i = i + 1
	end
	f:close()
end

debug.logCache = {}

function debug.clearLogCacheForFile(file)
	if (file == nil) then
		local info = debug.getinfo(2, "Sl")

		if not info.source:find("^@") then
			error("'debug.log' called from invalid source")
		end

		-- strip the '@' tag
		file = info.source:sub(2):lower():gsub("data files\\mwse\\", "")
	else
		file = file:lower():lower():gsub("data files\\mwse\\", "")
	end

	local toRemove = {}
	for entry in pairs(debug.logCache) do
		local cachedFile, cachedLine = entry:match("^(.+):(%d+)$")
		if (cachedFile == file) then
			table.insert(toRemove, entry)
		end
	end

	for _, remove in ipairs(toRemove) do
		debug.logCache[remove] = nil
	end
end

function debug.log(value)
	local info = debug.getinfo(2, "Sl")

	if not info.source:find("^@") then
		error("'debug.log' called from invalid source")
		return value
	end

	-- strip the '@' tag
	local fileName = info.source:sub(2)

	-- include line info
	local location = fileName:lower():gsub("data files\\mwse\\", "") .. ":" .. info.currentline

	local text = debug.logCache[location]
	if text == nil then
		text = getNthLine(fileName, info.currentline)
		if text ~= nil then
			text = text:match("debug%.log%((.*)%)")
			debug.logCache[location] = text
		end
	end

	print(string.format("[%s] %s = %s", location, text, value))

	return value
end


-------------------------------------------------
-- Extend 3rd API: lfs
-------------------------------------------------

local lfs = require("lfs")

-- Cache the original lfs.rmdir and replace it with a version that supports recursion.
lfs.rmdir_old = lfs.rmdir
local function deleteDirectoryRecursive(dir, recursive)
	-- Default to not being recursive.
	local recursive = recursive or false
	if (recursive) then
		for file in lfs.dir(dir) do
			local path = dir .. "\\" .. file
			if (file ~= "." and file ~= "..") then
				if (lfs.attributes(path, "mode") == "file") then
					os.remove(path)
				elseif (lfs.attributes(path, "mode") == "directory") then
					deleteDirectoryRecursive(path, true)
				end
			end
		end
	end

	-- Call the original function at the end.
	return lfs.rmdir_old(dir)
end
lfs.rmdir = deleteDirectoryRecursive

-- Basic "file exists" check.
function lfs.fileexists(filepath)
	return lfs.attributes(filepath, "mode") == "file"
end

-- Basic "folder exists" check.
function lfs.directoryexists(filepath)
	return lfs.attributes(filepath, "mode") == "directory"
end

-- Visit all files in a directory tree (recursively).
function lfs.walkdir(root)
	local function iter(dir)
		dir = dir or root
		for name in lfs.dir(dir) do
			if not name:find("%.$") then
				local path = dir .. name
				local mode = lfs.attributes(path, "mode")
				if mode == "file" then
					coroutine.yield(path, dir, name)
				elseif mode == "directory" then
					iter(path .. "\\")
				end
			end
		end
	end
	return coroutine.wrap(iter)
end


-------------------------------------------------
-- Extend our base API: json
-------------------------------------------------

function json.loadfile(fileName)
	-- Allow optional suffix, for 'lfs.dir()' compatiblity.
	if not fileName:lower():find("%.json$") then
		fileName = fileName .. ".json"
	end

	-- Load the contents of the file.
	local f = io.open("Data Files\\MWSE\\" .. fileName, "r")
	if (f == nil) then
		return nil
	end
	local fileContents = f:read("*all")
	f:close()

	-- Return decoded json.
	return json.decode(fileContents)
end

function json.savefile(fileName, object, config)
	local f = assert(io.open("Data Files/MWSE/" .. fileName .. ".json", "w"))
	f:write(json.encode(object, config))
	f:close()
end

function json.traceexception(reason, value, state, defaultmessage)
	mwse.log("json.encode: Error when encoding value '%s'. Buffer at time of error:\n%s", value, table.concat(state.buffer))
end

local originalEncode = json.encode
function json.encode(object, state)
	state = state or {}

	-- Trace encoding errors to the log by default unless another exception is provided.
	if (state.exception == nil and state.trace) then
		state.exception = json.traceexception
	end

	return originalEncode(object, state)
end


-------------------------------------------------
-- Extend our base API: toml
-------------------------------------------------

function toml.loadFile(fileName)
	-- Load the contents of the file.
	local f = io.open(fileName, "r")
	if (f == nil) then
		return nil, { reason = "Could not open file." }
	end

	local fileContents = f:read("*all")
	f:close()

	-- Return decoded toml.
	local status, resultOrError = pcall(toml.decode, fileContents)
	if (status) then
		return resultOrError
	else
		return nil, resultOrError
	end
end

function toml.saveFile(fileName, object)
	local f = assert(io.open(fileName, "w"))
	f:write(toml.encode(object))
	f:close()
end

---@param key string
---@return MWSE.Metadata?
function toml.loadMetadata(key)
	return toml.loadFile(string.format("Data Files\\%s-metadata.toml", key))
end


-------------------------------------------------
-- Extend our base API: yaml
-------------------------------------------------

function yaml.loadFile(fileName)
	-- Load the contents of the file.
	local f = io.open(fileName, "r")
	if (f == nil) then
		return nil, { reason = "Could not open file." }
	end

	local fileContents = f:read("*all")
	f:close()

	-- Return decoded yaml.
	local status, resultOrError = pcall(yaml.decode, fileContents)
	if (status) then
		return resultOrError
	else
		return nil, resultOrError
	end
end


-------------------------------------------------
-- Extend our base API: mwse
-------------------------------------------------

function mwse.log(str, ...)
	print(tostring(str):format(...))
end

-- helper function for `mwse.loadConfig`.
-- this function is responsible for:
-- 1) restoring numeric keys (i.e. keys that should be numbers, but were turned into strings by `json.savefile`)
-- 2) adding missing values to `config` that are present in `defaultConfig`
--
-- both of these things need to be done recursively, so it's not possible to use `table.copymissing`.
-- (i.e., we may need to alternate between converting integer keys and adding missing values)
---@param config table
---@param defaultConfig table
local function fixLoadedResult(config, defaultConfig)
	local configValue
	for key, defaultValue in pairs(defaultConfig) do
		configValue = config[key]

		-- check if we need to convert a string key to a numeric key
		if configValue == nil and type(key) == "number" and config[tostring(key)] ~= nil then
			config[key] = config[tostring(key)]
			config[tostring(key)] = nil
			configValue = config[key]
		end

		-- recheck the config value because it may have changed in the last code block
		if configValue ~= nil then
			-- if the default value is a table, we need to fix values recursively
			if type(defaultValue) == "table" and type(configValue) == "table" then
				fixLoadedResult(configValue, defaultValue)
			else
				-- no change needed
			end
		else -- configValue == nil
			-- make sure the config gets a copy of any subtables
			if type(defaultValue) == "table" then
				config[key] = table.deepcopy(defaultValue)
			else
				config[key] = defaultValue
			end
		end
	end
end

function mwse.loadConfig(fileName, defaults)
	local result = json.loadfile(string.format("config\\%s", fileName))

	if not result and not defaults then return end

	result = result or {} -- make sure the user gets something

	-- the for loop in `fixLoadedResult` wont be iterated at all if `defaults` evaluates to false
	fixLoadedResult(result, defaults or {})

	return result
end

function mwse.saveConfig(fileName, object, config)
	if (fileName and object) then
		json.savefile(string.format("config\\%s", fileName), object, config or { indent = true })
	end
end

-- Exception handler called when an object can't be correctly validated in the save.
local function exceptionWhenSaving(reason, value, state, defaultmessage)
	-- Log the occurrence.
	mwse.log("WARNING: Could not encode value '%s' when attempting to save data. Buffer at time of error:\n%s", value, table.concat(state.buffer))

	-- Keep the value in the table, but null it out.
	return "null"
end

-- Custom function for safely saving an object.
function mwse.encodeForSave(object)
	return json.encode(object, { exception = exceptionWhenSaving })
end

mwse.Logger = require("logger")


-------------------------------------------------
-- Setup and load MWSE config.
-------------------------------------------------

-- Helper function: Sets a config value while obfuscating the userdata binding.
function mwse.setConfig(key, value)
	return pcall(function()
		mwseConfig[key] = value
	end)
end

-- Helper function: Gets a config value while obfuscating the userdata binding.
function mwse.getConfig(key)
	return mwseConfig[key]
end

local function getDefaultConfigForSerializing()
	local defaults = table.copy(mwseConfig.getDefaults())

	-- We don't want to override the build number with defaults if one isn't provided.
	defaults.BuildNumber = nil

	return defaults
end

-- Load user config values.
local defaultConfig = mwseConfig.getDefaults()
local userConfig = mwse.loadConfig("MWSE", getDefaultConfigForSerializing())
for k, v in pairs(userConfig) do
	if (not mwse.setConfig(k, v)) then
		mwse.log("WARNING: User config key '%s' could not be assigned to '%s'.", k, v)
	end
end

-- Update build number.
-- TODO: Add an actual migration map to parse and go through.
local lastBuildNumber = userConfig.BuildNumber or 0
if (mwse.buildNumber ~= lastBuildNumber) then
	-- Build 3260 sets ReplaceDialogueFiltering to be enabled by default.
	if (lastBuildNumber <= 3260) then
		mwse.setConfig("ReplaceDialogueFiltering", defaultConfig["ReplaceDialogueFiltering"])
	end

	if (lastBuildNumber > 0) then
		mwse.log("MWSE build updated from %d to %d.", lastBuildNumber, mwse.buildNumber)
	else
		mwse.log("MWSE build initialized to %d.", mwse.buildNumber)
	end

	userConfig.BuildNumber = mwse.buildNumber
end

-- Refresh the file so that it shows users what other values can be tweaked.
mwse.saveConfig("MWSE", userConfig)


-------------------------------------------------
-- Extend our base API: tes3
-------------------------------------------------

function tes3.claimSpellEffectId(name, id)
	-- Ignore duplicate claims.
	if (name and tes3.effect[name] == id) then
		return
	end

	assert(type(name) == "string", "Name must be a string.")
	assert(type(id) == "number", "ID must be a number.")
	assert(table.find(tes3.effect, id) == nil, "Effect ID is not unique.")
	assert(tes3.effect[name] == nil, "Effect name is not unique.")
	tes3.effect[name] = id
end

-- Store the root installation folder.
tes3.installDirectory = lfs.currentdir()

local safeObjectHandle = require("mwse_safeObjectHandle")
--- @return mwseSafeObjectHandle
function tes3.makeSafeObjectHandle(object)
	return safeObjectHandle.new(object)
end

-------------------------------------------------
-- Extend base API: math
-------------------------------------------------

dofile("math")


-------------------------------------------------
-- Setup debugger if necessary
-------------------------------------------------

local function onError(e)
	mwse.log('[MWSE-Lua] Error: %s', e)
end

local targetDebugger = os.getenv("MWSE_LUA_DEBUGGER")
if (targetDebugger == "vscode-debuggee") then
	-- Start up our debuggee.
	local debuggee = require('vscode-debuggee')

	-- Overwrite the mwse.log function to also print to the debug console.
	mwse.log = function(str, ...)
		local message = tostring(str):format(...)
		print(message)
		debuggee.print("log", message)
	end

	-- Poll every frame.
	event.register("enterFrame", debuggee.poll, { priority = 9001 })

	-- Start the debugger.
	local startResult, breakerType = debuggee.start(json, { onError = onError, luaStyleLog = true })
	mwse.log("[MWSE-Lua] vscode-debuggee start -> Result: %s, Type: %s", startResult, breakerType)
	if (startResult) then
		mwse.debuggee = debuggee
	end
end


-- Report that we're initialized.
mwse.log("MWSE Lua interface initialized.")
