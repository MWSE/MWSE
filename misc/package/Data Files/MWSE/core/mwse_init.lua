-- First, look for objects in the core folder. DLL files may also exist in the root folder.
package.path = "./Data Files/MWSE/core/?.lua;./Data Files/MWSE/core/?/init.lua;"
package.cpath = "?.dll;./Data Files/MWSE/core/?.dll;"

-- Next, look in the library folder.
package.path = package.path .. "./Data Files/MWSE/lib/?.lua;./Data Files/MWSE/lib/?/init.lua;"
package.cpath = package.cpath .. "./Data Files/MWSE/lib/?.dll;"

-- Third, look in the mods folder.
package.path = package.path .. "./Data Files/MWSE/mods/?.lua;./Data Files/MWSE/mods/?/init.lua;"
package.cpath = package.cpath .. "./Data Files/MWSE/mods/?.dll;"

-- Provide backwards compatibility for old versions of MWSE 2.1. This will be removed before a stable release.
package.path = package.path .. "./Data Files/MWSE/lua/?.lua;./Data Files/MWSE/lua/?/init.lua;"
package.cpath = package.cpath .. "./Data Files/MWSE/lua/?.dll;"

function include(moduleName)
	local status, result = pcall(require, moduleName)
	if (status) then
		return result
	end
end

-------------------------------------------------
-- Extend base API: math
-------------------------------------------------

-- Seed random number generator.
-- There are reports that the first few results aren't random enough. Try and likely fail to make people happy.
math.randomseed(os.time())
for i = 1, 10 do
	math.random()
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

-- The # operator only really makes sense for continuous arrays. Get the real value.
function table.size(t)
	local count = 0
	for _ in pairs(t) do
		count = count + 1
	end
	return count
end

function table.choice(t)
	-- We need to get a list of all of our keys first.
	local keys = {}
	for k in pairs(t) do
		table.insert(keys, k)
	end

	-- Now we want to get a random key, and return the value for that key.
	local key = keys[math.random(#keys)]
	return t[key], key
end

function table.find(t, n)
	for i, v in pairs(t) do
		if (v == n) then
			return i
		end
	end
end

function table.removevalue(t, v)
	local i = table.find(t, v)
	if (i ~= nil) then
		table.remove(t, i)
		return true
	end
	return false
end

function table.copy(t, d)
	if (d == nil) then
		d = {}
	elseif (type(t) ~= "table" or type(d) ~= "table") then
		error("Arguments for table.copy must be tables.")
	end

	for k, v in pairs(t) do
		d[k] = v
	end

	return d
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
local default_fcompval = function( value ) return value end
local fcompf = function( a,b ) return a < b end
local fcompr = function( a,b ) return a > b end
function table.binsearch( t,value,fcompval,reversed )
	-- Initialise functions
	local fcompval = fcompval or default_fcompval
	local fcomp = reversed and fcompr or fcompf
	--  Initialise numbers
	local iStart,iEnd,iMid = 1,#t,0
	-- Binary Search
	while iStart <= iEnd do
		-- calculate middle
		iMid = math.floor( (iStart+iEnd)/2 )
		-- get compare value
		local value2 = fcompval( t[iMid] )
		-- get all values that match
		if value == value2 then
			local tfound,num = { iMid,iMid },iMid - 1
			while value == fcompval( t[num] ) do
				tfound[1],num = num,num - 1
			end
			num = iMid + 1
			while value == fcompval( t[num] ) do
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
function table.bininsert(t, value, fcomp)
	-- Initialise compare function
	local fcomp = fcomp or fcomp_default
	--  Initialise numbers
	local iStart,iEnd,iMid,iState = 1,#t,1,0
	-- Get insert position
	while iStart <= iEnd do
		-- calculate middle
		iMid = math.floor( (iStart+iEnd)/2 )
		-- compare
		if fcomp( value,t[iMid] ) then
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

function string.endswith(haystack, needle)
	return needle=='' or string.sub(haystack, -string.len(needle)) == needle
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
			local path = dir .. "/" .. file
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


-------------------------------------------------
-- Global includes
-------------------------------------------------

_G.tes3 = require("tes3.init")
_G.event = require("event")
_G.json = require("dkjson")

-------------------------------------------------
-- Extend our base API: json
-------------------------------------------------

function json.loadfile(fileName)
	-- Allow optional suffix, for 'lfs.dir()' compatiblity.
	if not fileName:lower():find("%.json$") then
		fileName = fileName .. ".json"
	end

	-- Load the contents of the file.
	local f = io.open("Data Files/MWSE/" .. fileName, "r")
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

-------------------------------------------------
-- Extend our base API: mge
-------------------------------------------------

function mge.getUIScale()
	-- MGE XE uses uniform scaling, so we only need check the width.
	return mge.getScreenWidth() / tes3.getWorldController().viewWidth
end


-------------------------------------------------
-- Extend our base API: mwse
-------------------------------------------------

function mwse.log(str, ...)
	print(str:format(...))
end

function mwse.loadConfig(fileName)
	return json.loadfile(string.format("config/%s", fileName))
end

function mwse.saveConfig(fileName, object, config)
	if (fileName and object) then
		json.savefile(string.format("config/%s", fileName), object, config or { indent = true })
	end
end


-------------------------------------------------
-- Extend our base API: mwse
-------------------------------------------------

function tes3ui.log(str, ...)
	tes3ui.logToConsole(str:format(...), false)
end


-------------------------------------------------
-- Usertype Extensions: tes3uiElement
-------------------------------------------------

-- Create a button composed of images that has a mouse over and mouse pressed state.
function tes3uiElement:createImageButton(params)
	-- Create our parent block.
	local buttonBlock = self:createBlock({ id = params.id })
	buttonBlock.autoWidth = true
	buttonBlock.autoHeight = true

	-- Create our child buttons using the params provided.
	local buttonIdle = buttonBlock:createImage({ path = params.idle })
	local buttonOver = buttonBlock:createImage({ path = params.over })
	local buttonPressed = buttonBlock:createImage({ path = params.pressed })

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

-------------------------------------------------

-- Report that we're initialized.
print("MWSE Lua interface initialized.")
