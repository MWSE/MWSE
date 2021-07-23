--
-- EmmyLua meta files generator for MWSE-lua definitions.
--
print("Starting build of EmmyLua meta files...")

local metaFolder = "..\\misc\\package\\Data Files\\MWSE\\core\\meta\\"

local defaultNoDescriptionText = "No description yet available."

-- 
-- Utility functions.
-- 

local function log(fmt, ...)
	print(fmt:format(...))
end

local lfs = require("lfs")
local json = require("dkjson")

local definitionsFolder = lfs.currentdir() .. "\\definitions"
log("Definitions folder: %s", definitionsFolder)

function json.loadfile(fileName)
	-- Load the contents of the file.
	local f = io.open(fileName, "r")
	if (f == nil) then
		return nil
	end
	local fileContents = f:read("*all")
	f:close()

	-- Return decoded json.
	return json.decode(fileContents)
end

function json.savefile(fileName, object, config)
	local f = assert(io.open(fileName, "w"))
	f:write(json.encode(object, config))
	f:close()
end

local function isTableEmpty(t)
	for _, _ in pairs(t) do
		return false
	end
	return true
end

local function getSortedKeys(t, sortFn)
	local keys = {}
	for k, _ in pairs(t) do
		table.insert(keys, k)
	end
	table.sort(keys, sortFn)
	return keys
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
	else
		copy = t
	end
	return copy
end

local match = string.match
local function trimString(s)
	return match(s, '^()%s*$') and '' or match(s, '^%s*(.*%S)')
end

local function splitString(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end
	local t = {}
	for str in string.gmatch(inputstr, "([^" .. sep .. "]+)") do
		table.insert(t, str)
	end
	return t
end

local function isDirectory(path)
	if (type(path) ~= "string") then
		return false
	end

	local attributes = lfs.attributes(path)
	return (attributes and attributes.mode == "directory")
end

local function copyFile(src, dst)
	local source = io.open(src, "r")
	local destination = io.open(dst, "w")

	destination:write(source:read("*a"))

	source:close()
	destination:close()
end

local urlBase = "https://mwse.readthedocs.io/en/latest/lua"

local function getPackageLink(package)
	local tokens = { urlBase, package.key .. ".html" }
	local up = package.parent
	while (up) do
		table.insert(tokens, 2, up.key)
		if (up.parent == nil) then
			if (up.type == "lib") then
				table.insert(tokens, 2, "api")
			elseif (up.type == "class") then
				table.insert(tokens, 2, "type")
			end
		end
		up = up.parent
	end
	return table.concat(tokens, "/")
end

local function writeExamples(package, file)
	if (package.examples) then
		file:write(string.format("---\n--- [Examples available in online documentation](%s).\n", getPackageLink(package)))
	end
end

local function formatLineBreaks(str)
	return string.gsub(str, "\n", "\n--- ")
end

local function formatDescription(description)
	return "--- " .. formatLineBreaks(description)
end

local function getFullPackageNamespace(package)
	if (package.parent) then
		return getFullPackageNamespace(package.parent) .. "." .. package.key
	end
	return package.key
end

local function getConsistentReturnValues(package)
	if (type(package.returns) == "string" and package.valuetype == nil) then
		return { { name = "result", type = package.returns } }
	elseif (type(package.returns) == "string" and package.valuetype ~= nil) then
		return { { name = package.returns, type = package.valuetype } }
	elseif (package.valuetype) then
		return { { name = "result", type = package.valuetype } }
	elseif (type(package.returns) == "table") then
		if (#package.returns == 1) then
			return { package.returns }
		end
		return package.returns
	end
end

local function getRemappedType(package)
	if (package.type == "lib") then
		return (package.namespace or package.key) .. "lib"
	end
	return (package.namespace or package.key)
end

local function getParamNames(package)
	local params = {}
	for _, param in ipairs(package.arguments or {}) do
		table.insert(params, param.name or "unknown")
	end
	return params
end

local function writeFunction(package, file, namespaceOverride)
	file:write(formatDescription(package.description or defaultNoDescriptionText) .. "\n")
	writeExamples(package, file)

	for i, argument in ipairs(package.arguments or {}) do
		local type = argument.type
		local description = argument.description or defaultNoDescriptionText
		if (i == 1 and argument.tableParams) then
			type = package.namespace .. ".params"
			description = "This table accepts the following values:"
			for _, tableArgument in ipairs(argument.tableParams) do
				description = description .. string.format("\n\n``%s``: %s — %s", tableArgument.name or "unknown", tableArgument.type or "any", formatLineBreaks(tableArgument.description or defaultNoDescriptionText))
			end
		end
		file:write(string.format("--- @param %s %s %s\n", argument.name or "unknown", type, formatLineBreaks(description)))
	end

	for _, returnPackage in ipairs(getConsistentReturnValues(package) or {}) do
		file:write(string.format("--- @return %s %s %s\n", returnPackage.type or "any", returnPackage.name or "result", returnPackage.description or defaultNoDescriptionText))
	end

	file:write(string.format("function %s(%s) end\n\n", namespaceOverride or package.namespace, table.concat(getParamNames(package), ", ")))

	if (package.arguments and #package.arguments == 1 and package.arguments[1].tableParams) then
		file:write(string.format("---Table parameter definitions for ``%s``.\n", package.namespace))
		file:write(string.format("--- @class %s.params\n", package.namespace))
		for _, param in ipairs(package.arguments[1].tableParams) do
			file:write(string.format("--- @field %s %s %s\n", param.name, param.type, param.description or defaultNoDescriptionText))
		end
		file:write("\n")
	end
end

-- 
-- Compile data
-- 

local function compileEntry(folder, key, parent)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	local package = dofile(path)
	if (package == nil) then
		error("Could not execute typed entry: " .. path)
	end

	-- Setup basic package info.
	package.key = key
	package.folder = folder
	package.parent = parent
	package.namespace = getFullPackageNamespace(package)

	-- Add to things.
	local collection = package.type .. "s"
	parent[collection] = parent[collection] or {}
	table.insert(parent[collection], package)
end

local function compile(folder, key, owningCollection, acceptedType)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	log("Compiling: " .. path .. " ...")
	local package = dofile(path)
	package.key = key

	-- We only care about libraries for now.
	if (package.type ~= acceptedType) then
		return
	end

	-- Write out children.
	for entry in lfs.dir(folder .. "\\" .. key) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			compileEntry(folder .. "\\" .. key, entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), package)
		end
	end

	-- Store it for later.
	owningCollection[key] = package
end

local libraries = {}
for entry in lfs.dir(definitionsFolder .. "\\global") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		compile(definitionsFolder .. "\\global", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), libraries, "lib")
	end
end

local classes = {}
for entry in lfs.dir(definitionsFolder .. "\\namedTypes") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		compile(definitionsFolder .. "\\namedTypes", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), classes, "class")
	end
end

-- 
-- Building
-- 

local outFolders = {
	lib = metaFolder .. "lib\\",
	class = metaFolder .. "class\\",
}

local function buildParentChain(className)
	local package = assert(classes[className])
	if (package.inherits) then
		return className .. ", " .. buildParentChain(package.inherits)
	end
	return className
end

local function build(key, collection, type)
	-- Load our base package.
	log("Building " .. type .. ": " .. key .. " ...")
	local package = collection[key]

	-- We only care about the given type for now.
	if (not package or package.type ~= type) then
		return
	end

	-- Get the package.
	local outPath = assert(outFolders[type]) .. key .. ".lua"
	local file = assert(io.open(outPath, "w"))

	-- Mark the file as a meta file.
	file:write("--- @meta\n\n")

	-- Write description.
	file:write(formatDescription(package.description or defaultNoDescriptionText) .. "\n")
	writeExamples(package, file)
	if (type == "lib") then
		file:write(string.format("--- @class %slib\n", key))
	elseif (type == "class") then
		file:write(string.format("--- @class %s%s\n", key, package.inherits and (" : " .. buildParentChain(package.inherits)) or ""))
	end

	-- Write out fields.
	for _, value in ipairs(package.values or {}) do
		file:write(string.format("--- @field %s %s %s\n", value.key, value.valuetype or "any", formatLineBreaks(value.description or defaultNoDescriptionText)))
	end

	-- Finalize the main class definition.
	file:write(string.format("%s = {}\n\n", key))

	-- Write out functions.
	for _, value in ipairs(package.functions or {}) do
		writeFunction(value, file)
	end

	-- Write out methods.
	if (package.type == "class") then
		for _, value in ipairs(package.methods or {}) do
			writeFunction(value, file, package.key .. ":" .. value.key)
		end
	end

	-- Close up shop.
	file:close()
end

for entry in lfs.dir(definitionsFolder .. "\\global") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		build(entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), libraries, "lib")
	end
end

--
-- Classes
--

for entry in lfs.dir(definitionsFolder .. "\\namedTypes") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		build(entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), classes, "class")
	end
end
