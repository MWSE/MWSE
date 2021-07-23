-- First, look for objects in the core folder. DLL files may also exist in the root folder.
package.path = ".\\Data Files\\MWSE\\core\\?.lua;.\\Data Files\\MWSE\\core\\?\\init.lua;"
package.cpath = "?.dll;.\\Data Files\\MWSE\\core\\?.dll;"

-- Next, look in the library folder.
package.path = package.path .. ".\\Data Files\\MWSE\\lib\\?.lua;.\\Data Files\\MWSE\\lib\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\lib\\?.dll;"

-- Third, look in the mods folder.
package.path = package.path .. ".\\Data Files\\MWSE\\mods\\?.lua;.\\Data Files\\MWSE\\mods\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\mods\\?.dll;"

-- Provide backwards compatibility for old versions of MWSE 2.1. This will be removed before a stable release.
package.path = package.path .. ".\\Data Files\\MWSE\\lua\\?.lua;.\\Data Files\\MWSE\\lua\\?\\init.lua;"
package.cpath = package.cpath .. ".\\Data Files\\MWSE\\lua\\?.dll;"

local originalRequire = require

-- Allow users to try to include files that may not exist.
function include(moduleName)
	-- First try to load the lowercase module.
	local status, result = pcall(originalRequire, moduleName:gsub("[/\\]", "."):lower())
	if (status) then
		return result
	end

	-- Then try to load the original path.
	local status, result = pcall(originalRequire, moduleName)
	if (status) then
		return result
	end
end

-- Try to return a lowercased module first, fall back to regular require.
function require(moduleName)
	-- First try to load the lowercase module.
	local status, result = pcall(originalRequire, moduleName:gsub("[/\\]", "."):lower())
	if (status) then
		return result
	end

	-- Then load the original path.
	return originalRequire(moduleName)
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

-------------------------------------------------
-- Global includes
-------------------------------------------------

_G.tes3 = require("tes3.init")
_G.event = require("event")
_G.json = require("dkjson")


-------------------------------------------------
-- Extend base API: math
-------------------------------------------------

-- Seed random number generator.
-- There are reports that the first few results aren't random enough. Try and likely fail to make people happy.
math.randomseed(os.time())
for i = 1, 10 do
	math.random()
end

function math.lerp(v0, v1, t)
	return (1 - t) * v0 + t * v1;
end

function math.clamp(value, low, high)
	if (low > high) then
		low, high = high, low
	end
	return math.max(low, math.min(high, value))
end

function math.remap(value, lowIn, highIn, lowOut, highOut)
	return lowOut + (value - lowIn) * (highOut - lowOut) / (highIn - lowIn)
end

function math.round(value, digits)
	local mult = 10 ^ (digits or 0)
	return math.floor(value * mult + 0.5) / mult
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
	-- We need to get a list of all of our values first.
	local keys = table.keys(t)

	-- Now we want to get a random key, and return the value for that key.
	local key = keys[math.random(#keys)]
	return t[key], key
end

function table.find(t, value)
	for i, v in pairs(t) do
		if (v == value) then
			return i
		end
	end
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
	elseif (type(from) ~= "table" or type(to) ~= "table") then
		error("Arguments for table.copy must be tables.")
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
	if (type(to) ~= "table" or type(from) ~= "table") then
		error("Arguments for table.copymissing must be tables.")
	end

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


-------------------------------------------------
-- Extend base table: Add binary search/insert
-------------------------------------------------

--[[
	table.binsearch( table, value [, compval [, reversed] ] )

	Searches the table through BinarySearch for the given value.
	If the  value is found:
		it returns a table holding all the mathing indices (e.g. { startindice,endindice } )
		endindice may be the same as startindice if only one matching indice was found
	If compval is given:
		then it must be a function that takes one value and returns a second value2,
		to be compared with the input value, e.g.:
		compvalue = function( value ) return value[1] end
	If reversed is set to true:
		then the search assumes that the table is sorted in reverse order (largest value at position 1)
		note when reversed is given compval must be given as well, it can be nil/_ in this case
	Return value:
		on success: a table holding matching indices (e.g. { startindice,endindice } )
		on failure: nil
]]--
local function default_fcompval( value ) return value end
local function fcompf( a,b ) return a < b end
local function fcompr( a,b ) return a > b end
function table.binsearch( t,value,compval,reversed )
	-- Initialise functions
	local compval = compval or default_fcompval
	local fcomp = reversed and fcompr or fcompf
	--  Initialise numbers
	local iStart,iEnd,iMid = 1,#t,0
	-- Binary Search
	while iStart <= iEnd do
		-- calculate middle
		iMid = math.floor( (iStart+iEnd)/2 )
		-- get compare value
		local value2 = compval( t[iMid] )
		-- get all values that match
		if value == value2 then
			local tfound,num = { iMid,iMid },iMid - 1
			while value == compval( t[num] ) do
				tfound[1],num = num,num - 1
			end
			num = iMid + 1
			while value == compval( t[num] ) do
				tfound[2],num = num,num + 1
			end
			return tfound
		-- keep searching
		elseif fcomp( value,value2 ) then
			iEnd = iMid - 1
		else
			iStart = iMid + 1
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


-------------------------------------------------
-- Extend base API: debug
-------------------------------------------------

local function getNthLine(fileName, n)
	local f = io.open(fileName, "r")
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
-- Extend our base API: mge
-------------------------------------------------

function mge.getUIScale()
	-- MGE XE uses uniform scaling, so we only need check the width.
	return mge.getScreenWidth() / tes3.worldController.viewWidth
end


-------------------------------------------------
-- Extend our base API: mwse
-------------------------------------------------

function mwse.log(str, ...)
	print(tostring(str):format(...))
end

function mwse.loadConfig(fileName, defaults)
	local result = json.loadfile(string.format("config\\%s", fileName))

	if (result) then
		if (type(defaults) == "table") then
			table.copymissing(result, defaults)
		end
	else
		result = defaults
	end

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

-- Load user config values.
local userConfig = mwse.loadConfig("MWSE", mwseConfig.getDefaults())
for k, v in pairs(userConfig) do
	if (not mwse.setConfig(k, v)) then
		mwse.log("WARNING: User config key '%s' could not be assigned to '%s'.", k, v)
	end
end

-- Refresh the file so that it shows users what other values can be tweaked.
mwse.saveConfig("MWSE", userConfig)


-------------------------------------------------
-- Extend our base API: tes3
-------------------------------------------------

function tes3.claimSpellEffectId(name, id)
	assert(table.find(tes3.effect, id) == nil, "Effect ID is not unique.")
	assert(tes3.effect[name] == nil, "Effect name is not unique.")
	tes3.effect[name] = id
end

-- Store the root installation folder.
tes3.installDirectory = lfs.currentdir()

local safeObjectHandle = require("mwse_safeObjectHandle")
function tes3.makeSafeObjectHandle(object)
	return safeObjectHandle.new(object)
end


-------------------------------------------------
-- Extend our base API: tes3ui
-------------------------------------------------

function tes3ui.log(str, ...)
	tes3ui.logToConsole(tostring(str):format(...), false)
end


-------------------------------------------------
-- Usertype Extensions: tes3uiElement
-------------------------------------------------

-- Create a button composed of images that has a mouse over and mouse pressed state.
function tes3uiElement:createImageButton(params)
	-- Get the button block params.
	local blockParams = params.blockParams or {
		id = params.id,
	}
	local idleParams = params.idleParams or {
		id = params.idleId,
		path = params.idle,
	}
	local overParams = params.overParams or {
		id = params.overId,
		path = params.over,
	}
	local pressedParams = params.pressedParams or {
		id = params.pressedId,
		path = params.pressed,
	}

	-- Create our parent block.
	local buttonBlock = self:createBlock(blockParams)
	buttonBlock.autoWidth = true
	buttonBlock.autoHeight = true

	-- Create our child buttons using the params provided.
	local buttonIdle = buttonBlock:createImage(idleParams)
	local buttonOver = buttonBlock:createImage(overParams)
	local buttonPressed = buttonBlock:createImage(pressedParams)

	-- Prevent any of the above-created buttons from consuming the mouse events.
	buttonIdle.consumeMouseEvents = false
	buttonOver.consumeMouseEvents = false
	buttonPressed.consumeMouseEvents = false

	-- Hide the over/pressed buttons for now.
	buttonOver.visible = false
	buttonPressed.visible = false

	-- Create the functions to hide/show buttons based on mouse state.
	buttonBlock:register("mouseOver", function()
		buttonIdle.visible = false
		buttonOver.visible = true
		buttonPressed.visible = false
	end)
	buttonBlock:register("mouseLeave", function()
		buttonIdle.visible = true
		buttonOver.visible = false
		buttonPressed.visible = false
	end)
	buttonBlock:register("mouseDown", function()
		buttonIdle.visible = false
		buttonOver.visible = false
		buttonPressed.visible = true
	end)
	buttonBlock:register("mouseRelease", function()
		buttonIdle.visible = false
		buttonOver.visible = true
		buttonPressed.visible = false
	end)

	-- Return the created block.
	return buttonBlock
end

function tes3uiElement:sortChildren(fn)
	-- Sort our children list.
	local children = self.children
	table.sort(children, fn)

	-- Rearrange children.
	for i, child in ipairs(children) do
		self:reorderChildren(i, child, 1)
	end
end


-------------------------------------------------
-- Setup debugger if necessary
-------------------------------------------------

local targetDebugger = os.getenv("MWSE_LUA_DEBUGGER")
if (targetDebugger == "vscode-debuggee") then
	-- Start up our debuggee.
	local debuggee = require('vscode-debuggee')
	local startResult, breakerType = debuggee.start(json)
	mwse.log("[MWSE-Lua] vscode-debuggee start -> Result: %s, Type: %s", startResult, breakerType)

	-- Overwrite the mwse.log function to also print to the debug console.
	mwse.log = function(str, ...)
		local message = tostring(str):format(...)
		print(message)
		debuggee.print("log", message)
	end

	-- Poll every frame.
	event.register("enterFrame", debuggee.poll, { priority = 9001 })
end


-- Report that we're initialized.
mwse.log("MWSE Lua interface initialized.")
