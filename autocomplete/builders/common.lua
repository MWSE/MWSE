---@diagnostic disable: duplicate-set-field
local common = {}

--- A wrapper around `print` that allows format strings.
--- @param fmt string The format string.
--- @param ... any? Arguments for formatting.
function common.log(fmt, ...)
	print(fmt:format(...))
end


--
-- String library extensions
--

--- @param str string
--- @param sep string
--- @return string[] parts
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

--- @param s string
--- @return string trimmed
function string.trim(s)
	return string.match(s, '^()%s*$') and '' or string.match(s, '^%s*(.*%S)')
end
getmetatable("").trim = string.trim

--- Returns true if the string starts with a given substring.
--- @param haystack string The string to check.
--- @param needle string The starting value to check.
--- @return boolean
function string.startswith(haystack, needle)
	return string.sub(haystack, 1, string.len(needle)) == needle
end
getmetatable("").startswith = string.startswith

--- Returns true if the string ends with a given substring.
--- @param haystack string The string to check.
--- @param needle string The ending value to check.
--- @return boolean
function string.endswith(haystack, needle)
	return needle == '' or string.sub(haystack, -string.len(needle)) == needle
end
getmetatable("").endswith = string.endswith


--
-- Table library extensions
--

--- @param t table
--- @return boolean
function table.empty(t)
	for _, _ in pairs(t) do
		return false
	end
	return true
end

---@generic K, V
--- @param t {[K]: V}
--- @param value V
--- @return K?
function table.find(t, value)
	for i, v in pairs(t) do
		if (v == value) then
			return i
		end
	end
end

---@generic K, V, T
--- @param t {[K]: V}|T
--- @param sort boolean|nil|(fun(a: V, b: V): boolean)
--- @return K[]
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

---@generic K, V, T
--- @param t {[K]: V}|T
--- @param sort boolean|nil|(fun(a: V, b: V): boolean)
--- @return V[]
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

---@generic K, V, T
---@param t {[K]: V}|T
---@param d {[K]: V}|T|nil
---@return {[K]: V}|T
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

---@generic K, V, T
---@param t {[K]: V}|T
---@return {[K]: V}|T
function table.deepcopy(t)
	local copy = nil
	if type(t) == "table" then
		copy = {}
		for k, v in next, t, nil do
			copy[table.deepcopy(k)] = table.deepcopy(v)
		end
		setmetatable(copy, table.deepcopy(getmetatable(t)))
	else
		copy = t
	end
	return copy
end

---@generic V, T
---@param t V[]|T
--- @param value V
--- @return boolean successful
function table.removevalue(t, value)
	local i = table.find(t, value)
	if (i ~= nil) then
		table.remove(t, i)
		return true
	end
	return false
end


--
-- json library extensions
--

local json = require("dkjson")

--- @param path string
--- @return table|nil decoded
function json.loadfile(path)
	-- Load the contents of the file.
	local f = io.open(path, "r")
	if (f == nil) then
		return nil
	end
	local fileContents = f:read("*all")
	f:close()

	-- Return decoded json.
	return json.decode(fileContents) --[[@as table?]]
end

--- @param path string
--- @param object table
--- @param config table
function json.savefile(path, object, config)
	local f = assert(io.open(path, "w"))
	f:write(json.encode(object, config) --[[@as string?]])
	f:close()
end


--
-- io library extensions
--

--- Creates a file with the given contents.
--- @param path string
--- @param content string
function io.createwith(path, content)
	local file = assert(io.open(path, "w"))
	file:write(content)
	file:close()
end



--
-- lfs library extensions
--

---@class common.lfs : lfslib
local lfs = require("lfs")

--- @param ... string Parts of a file path.
--- @return string path The result of joining each `part` with a `\\`.
function lfs.join(...)
	return table.concat({ ... }, "\\")
end

--- @param path string
--- @return boolean
function lfs.isdir(path)
	return lfs.attributes(path, "mode") == "directory"
end

--- @param path string
--- @return boolean
function lfs.isfile(path)
	return lfs.attributes(path, "mode") == "directory"
end

---@param from string File to copy from.
---@param to string File to copy to.
function lfs.copyfile(from, to)
	local source = io.open(from, "r")
	if not source then return end

	local destination = io.open(to, "w")
	if not destination then return end

	destination:write(source:read("*a"))

	source:close()
	destination:close()
end

-- Cache the original lfs.rmdir and replace it with a version that supports recursion.
lfs.rmdir_old = lfs.rmdir

--- @param dir string
--- @param recursive boolean? Default: `false`
--- @return boolean
function lfs.rmdir(dir, recursive)
	-- Default to not being recursive.
	if (recursive) then
		for file in lfs.dir(dir) do
			local path = dir .. "\\" .. file
			if (file ~= "." and file ~= "..") then
				if (lfs.attributes(path, "mode") == "file") then
					os.remove(path)
				elseif (lfs.attributes(path, "mode") == "directory") then
					lfs.rmdir(path, true)
				end
			end
		end
	end

	-- Call the original function at the end.
	return lfs.rmdir_old(dir)
end

--- @param path string
function lfs.remakedir(path)
	assert(lfs.rmdir(path, true))
	assert(lfs.mkdir(path))
end


--
-- Add some common paths and urls.
--

--- The path to the autocomplete folder. `MWSE/autocomplete`
common.pathAutocomplete = lfs.currentdir() --[[@as string]]

--- The path to the definitions folder. `MWSE/autocomplete/definitions`
--- @type string
common.pathDefinitions = lfs.join(common.pathAutocomplete, "definitions")

--- @param ... string
--- @return string
function common.urlJoin(...)
	local paths = { ... }
	return table.concat(paths, "/")
end

common.urlBase = "https://mwse.github.io/MWSE"


--
-- Reused language strings
--

common.defaultNoDescriptionText = "No description yet available."
common.defaultExperimentalAPIWarning = [[

!!! warning
	This part of the API isn't fully understood yet and thus is considered experimental. That means that there can be breaking changes requiring the code using this part of the API to be rewritten. The MWSE team will not make any effort to keep backward compatibility with the mods using experimental APIs.

]]

--- @param package package
--- @param useDefault boolean|nil Use default descriptions?
--- @return string description This will be `nil` if there were no `parts`.
function common.getDescriptionString(package, useDefault)
	if (useDefault == nil) then
		useDefault = true
	end

	local descriptionBits = {}

	if (package.readOnly) then
		table.insert(descriptionBits, "*Read-only*.")
	end

	if (package.default ~= nil) then
		table.insert(descriptionBits, string.format("*Default*: `%s`.", tostring(package.default)))
	elseif (package.optional) then
		table.insert(descriptionBits, "*Optional*.")
	end

	if (package.experimental) then
		table.insert(descriptionBits, common.defaultExperimentalAPIWarning)
	end

	if (package.description) then
		table.insert(descriptionBits, package.description)
	elseif (useDefault) then
		table.insert(descriptionBits, common.defaultNoDescriptionText)
	end

	if (#descriptionBits > 0) then
		return table.concat(descriptionBits, " ")
	end

	-- According to the annotations induced by `emmy.lua`, something should be returned here.
end

--
-- Complex type helpers
--

--- @class complexType
--- @field input string The string originally fed to the complex type.
--- @field type string The underlying type.
--- @field isArray boolean If true, the type is a boolean.
--- @field isOptional boolean If true, the type is optional.
local complexType = {}

function complexType:toTypeString()
	return common.makeTypeString(self.type, self.isOptional, self.isArray)
end


--- Parse a string, return a `complexType`
--- @param input string
--- @return complexType
function common.makeComplexType(input)
	local complex = setmetatable({ input = input, type = input }, complexType)

	while (true) do
		local isOptional = complex.type:endswith("?")
		local isArray = complex.type:endswith("[]")

		if (isOptional) then
			complex.type = string.sub(complex.type, 1, -2)
			complex.isOptional = true
		elseif (isArray) then
			complex.type = string.sub(complex.type, 1, -3)
			complex.isArray = true
		else
			break
		end
	end

	return complex
end

---@param type string
---@param isOptional boolean?
---@param isArray boolean?
function common.makeTypeString(type, isOptional, isArray)
	return string.format("%s%s%s", type, isOptional and "?" or "", isArray and "[]" or "")
end


--
-- Package compilation
--

---@alias packageType
---|"class"
---|"function"
---|"method"
---|"value"
---|"lib"
---|"event"

--- @class exampleTable
--- @field title string|nil The example title.
--- @field description string|nil The description of the example.

--- @class package
--- @field name string
--- @field readOnly boolean
--- @field optional boolean? Is this parameter optional?
--- @field default string|integer|nil
--- @field experimental boolean
--- @field description string?
--- @field key string The name of the file that generated this package.
--- @field type packageType The type definition for the package.
--- @field folder string The folder that the package was created from.
--- @field parent package The package this package is a child of.
--- @field namespace string The full namespace of the package.
--- @field deprecated boolean Allows marking definitions as deprecated. Those definitions aren't written to the web documentation.
--- @field examples table<string, exampleTable>|nil A table containing the examples. Keys are the example's name/path to the example file.
--- @field libs package[]|nil For libraries with sub-namespaces such as mwse.mcm, etc., this array contians the nested namespaces.

-- This corresponds to a documentation file with the [`"value"` type](https://github.com/MWSE/MWSE/blob/master/docs/type-definitions-guide.md#value-definitions).
---@class packageValue : package
---@field valuetype string
---@field link string


-- Basic function argument. These are things that can be inside `tableParams`.
---@class packageFunctionArgument.Simple : package

-- Basic function argument. Includes the possibility of having `tableParams
---@class packageFunctionArgument.Complex : package
---@field tableParams packageFunctionArgument.Simple[]

-- Parameter or return value for a `packageFunction` or `packageMethod`.
-- This comes in two flavors: `Simple` and `Complex`.
---@alias packageFunctionArgument packageFunctionArgument.Simple|packageFunctionArgument.Complex


-- This corresponds to a documentation file with the [`"function"` type](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md#function-definitions).
--- @class packageFunction : packageValue
--- @field arguments packageFunctionArgument[]
--- @field returns packageFunctionArgument|packageFunctionArgument[]|nil

-- This corresponds to a documentation file with the [`"method"` type](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md#function-definitions).
--- @class packageMethod : packageFunction

-- This corresponds to a documentation file with the [`"lib"` type](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md#library-definitions).
--- @class packageLib : package
--- @field children table<string, package>|nil
--- @field functions packageFunction[]|nil
--- @field values packageValue[]|nil
--- @field typeValues packageValue[]|nil

---@class packageOverload
---@field rightType string
---@field resultType string
---@field description string

-- This corresponds to a documentation file with the [`"operator"` type](https://github.com/MWSE/MWSE/blob/master/docs/operator-definitions-guide.md).
---@class packageOperator : package
---@field overloads packageOverload[]

--- @class packageClass : packageLib
--- @field inherits string The class that this class descends from.
--- @field operators packageOperator[]|nil
--- @field isAbstract boolean
--- @field methods packageMethod[]|nil
--- @field allDescendents table<string, packageClass>
--- @field directDescendents table<string, packageClass>
--- @field allDescendentKeys string

-- This corresponds to a documentation file with the [`"event"` type](https://github.com/MWSE/MWSE/blob/master/docs/event-definitions-guide.md).
---@class packageEvent : package
---@field filter string
---@field blockable boolean
---@field related string[]
---@field eventData table<string, packageFunctionArgument>

--- comment
--- @param package packageFunction
--- @return packageFunctionArgument[]|nil
function common.getConsistentReturnValues(package)
	if type(package.returns) == "string" then
		if package.valuetype == nil then
			return { { name = "result", type = package.returns } }
		end
		return { { name = package.returns, type = package.valuetype } }
	end

	if (package.valuetype) then
		return { { name = "result", type = package.valuetype } }
	end

	if type(package.returns) ~= "table" then return end

	if (package.returns.name or package.returns.type) then
		return { package.returns }
	elseif (#package.returns > 0) then
		return package.returns
	end
	error("Invalid parameters table.")
end

--- comment
--- @param package package
--- @return string
local function getFullPackageNamespace(package)
	if (package.parent) then
		return (package.parent.namespace or getFullPackageNamespace(package.parent)) .. "." .. package.key
	end
	return package.key
end

--- comment
--- @param folder string
--- @param key string
--- @param parent package
function common.compileEntry(folder, key, parent)
	-- Load our base package.
	local path = lfs.join(folder, key .. ".lua")
	--- @type package
	local package = dofile(path)
	if (package == nil) then
		error("Could not execute typed entry: " .. path)
	end

	-- Setup basic package info.
	package.key = key
	package.folder = folder
	package.parent = parent
	package.namespace = getFullPackageNamespace(package)

	-- Setup children access on parents.
	-- NOTE: the `children` field does not appear to ever be used (except in `readthedocs.lua`).
	parent.children = parent.children or {}
	parent.children[key] = package

	-- Add to things.
	local collection = package.type .. "s"
	parent[collection] = parent[collection] or {}
	table.insert(parent[collection], package)

	-- Write out sub-libraries.
	if (package.type == "lib") then
		for entry in lfs.dir(lfs.join(folder, key)) do
			local extension = entry:match("[^.]+$")
			if (extension == "lua") then
				common.compileEntry(
					lfs.join(folder, key),
					entry:match("[^/]+$"):sub(1, -1 * (extension:len() + 2)),
					package
				)
			end
		end
	end
end

--- comment
--- @param folder string
--- @param key string
--- @param owningCollection table<string, package>
--- @param acceptedType string?
function common.compile(folder, key, owningCollection, acceptedType)
	-- Load our base package.
	local path = lfs.join(folder, key .. ".lua")
	common.log("Compiling %s: %s ...", acceptedType or "package", key)
	local package = dofile(path)
	package.key = key
	package.namespace = getFullPackageNamespace(package)
	package.folder = folder

	-- We only care about libraries for now.
	if (acceptedType and package.type ~= acceptedType) then
		return
	end

	-- Write out sub-libraries.
	if (acceptedType ~= "event") then
		for entry in lfs.dir(lfs.join(folder, key)) do
			local extension = entry:match("[^.]+$")
			if (extension == "lua") then
				common.compileEntry(
					lfs.join(folder, key),
					entry:match("[^/]+$"):sub(1, -1 * (extension:len() + 2)),
					package
				)
			end
		end
	end

	-- Store it for later.
	owningCollection[key] = package
end

--- comment
--- @param path string
--- @param owningCollection table<string, package>
--- @param acceptedType string?
function common.compilePath(path, owningCollection, acceptedType)
	for entry in lfs.dir(path) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			common.compile(
				path,
				entry:match("[^/]+$") :sub(1, -1 * (extension:len() + 2)),
				owningCollection,
				acceptedType
			)
		end
	end

	if (acceptedType == "class") then ---@cast owningCollection packageClass
		common.compileInheritances(owningCollection)
	end
end

--- Figure out inheritances.
--- @param classes table<string, packageClass>
function common.compileInheritances(classes)
	for _, class in pairs(classes) do
		if (class.inherits) then
			local parent = classes[class.inherits]
			if (parent) then
				parent.directDescendents = parent.directDescendents or {}
				parent.directDescendents[class.key] = class

				while (parent) do
					parent.allDescendents = parent.allDescendents or {}
					parent.allDescendents[class.key] = class
					parent = classes[parent.inherits]
				end
			end
		end
	end

	-- Update allDescendentKeys
	-- Explosions. I should have kept calling them explosion keys. Way cooler.
	for _, class in pairs(classes) do
		if (class.allDescendents) then
			local allDescendentKeys = {}
			for _, descendent in pairs(class.allDescendents) do
				if (not descendent.isAbstract) then
					table.insert(allDescendentKeys, descendent.key)
				end
			end
			if (#allDescendentKeys > 0) then
				table.insert(allDescendentKeys, class.key)
				table.sort(allDescendentKeys)
				class.allDescendentKeys = table.concat(allDescendentKeys, "|")
			end
		end
	end
end

--- A map for enumerations files. The layout is:
--- ```lua
--- {
--- 	["ni"] = {
--- 		["animCycleType"] = "path\\to\\enum\\file\\enumNamespace\\enumName.lua"
--- 	},
--- 	["mge"] = {},
--- 	...
--- }
--- ```
--- @class libraryEnumerations
--- @field [string] table<string, string>

---@type libraryEnumerations
local enumerations = {}
local fileBlacklist = {
	["init"] = true,
}

--- Returns a map of all the enumeration files and paths to those files in the following format:
--- `["lib.enumName"] = "path.to.enum`
--- @param namespace string
--- @return table<string, string> For example: `["tes3.actorType"] = "path\\to\\enum`
function common.getEnumerationsMap(namespace)
	local enums = enumerations[namespace]
	if enums then
		return enums
	end

	local directory = lfs.join(common.pathAutocomplete, "..", "misc", "package", "Data Files", "MWSE", "core", "lib", namespace)
	for entry in lfs.dir(directory) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			local filename = entry:match("[^/]+$"):sub(1, -1 * (extension:len() + 2))
			if (not fileBlacklist[filename]) then
				enums = enums or {}
				enums[filename] = lfs.join(directory, entry)
			end
		end
	end
	return enums
end


return common
