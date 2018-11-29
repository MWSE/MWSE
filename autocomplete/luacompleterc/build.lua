--
-- .luacompleterc and .luacheckrc generator for MWSE-Lua.
--

local function log(fmt, ...)
	print (fmt:format(...))
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

function isTableEmpty(t)
	for _, _ in pairs(t) do
		return false
	end
	return true
end

function copyTable(t, d)
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

local function isDirectory(path)
	if (type(path) ~= "string") then
		return false
	end

	local attributes = lfs.attributes(path)
	return (attributes and attributes.mode == "directory")
end

local typeRemaps = {
	["class"] = "table",
	["lib"] = "table",
	["method"] = "function",
}

local standardTypes = {
	["function"] = true,
	["table"] = true,
	["number"] = true,
	["boolean"] = true,
	["string"] = true,
	["unknown"] = true,
	["ref"] = true,
}

local definitionCount = 0

local global = {}
local namedTypes = {}
local derivedTypes = {}

local function copyDefinition(destination, package, parent)
	copyTable(parent, destination)
	copyTable(package, destination)

	local tempFields = {}
	if (parent.fields) then
		copyTable(parent.fields, tempFields)
		if (package.fields) then
			copyTable(package.fields, tempFields)
		end
		destination.fields = tempFields
	end

	local tempIndex = {}
	if (parent.metatable and parent.metatable.fields and parent.metatable.fields.__index) then
		copyTable(parent.metatable.fields.__index, tempIndex)
		if (package.metatable and package.metatable.fields and package.metatable.fields.__index) then
			copyTable(package.metatable.fields.__index, tempIndex)
		end
		destination.metatable.fields.__index = tempIndex
	end
end

local function insertField(parent, key, package)
	if (parent.fields == nil) then
		parent.fields = {}
	end
	parent.fields[key] = package
end

local function insertMethod(parent, key, package)
	if (parent.metatable == nil) then
		parent.metatable = { type = "table", fields = { __index = { type = "table", fields = {} } } }
	end
	parent.metatable.fields.__index.fields[key] = package
end

local function setupFunction(package, raw)
	if (raw.arguments == nil or #raw.arguments == 0) then
		return
	end

	package.args = {}
	package.argTypes = {}

	if (type(raw.arguments) == "table" and #raw.arguments == 1 and raw.arguments[1].tableParams) then
		-- Insert the single param.
		table.insert(package.args, { name = "params", type = "table" })
		table.insert(package.argTypes, "table")

		-- Add it to the description.
		package.description = package.description .. "\n\nAccepts parameters through a table with the given keys:"
		for _, param in pairs(raw.arguments[1].tableParams) do
			package.description = package.description .. "\n * (" .. param.name .. ": " .. param.type .. ")"
			if (param.description) then
				package.description = package.description .. " " .. param.description
			end
			if (param.default) then
				package.description = package.description .. " Default: " .. tostring(param.default)
			elseif (param.optional) then
				package.description = package.description .. " Optional."
			end
		end

	else
		for _, arg in ipairs(raw.arguments) do
			local namePackage = { name = arg.name }

			if (arg.type == "variadic") then
				namePackage.name = "..."
				namePackage.displayName = "..."
			elseif (arg.default ~= nil) then
				namePackage.displayName = string.format("%s = %s", arg.name, tostring(arg.default))
			end

			if (arg.optional) then
				namePackage.displayName = string.format("[%s]", namePackage.displayName or arg.name)
			end

			table.insert(package.args, namePackage)
			table.insert(package.argTypes, arg.type)
		end
	end

	if (type(raw.valuetype) == "nil") then
		package.returnTypes = nil
	elseif (type(raw.valuetype) == "string") then
		if (standardTypes[raw.valuetype]) then
			package.returnTypes = { { type = raw.valuetype } }
		else
			package.returnTypes = { { type = "ref", name = raw.valuetype } }
		end
	elseif (type(raw.returns) == "table") then
		package.returnTypes = {}

		for _, v in pairs(raw.returns) do
			if (standardTypes[v.type]) then
				table.insert(package.returnTypes, { type = v.type })
			else
				table.insert(package.returnTypes, { type = "ref", name = v.type })
			end
		end

		if (#package.returnTypes == 0) then
			package.returnTypes = nil
		end
	end
end

local function buildAPI(folder, key, parentPackage, parentRawPackage)
	-- Get our base package that we'll translate to a zerobrane package.
	local path = folder .. "\\" .. key .. ".lua"
	log("Building definition: " .. path:sub(#definitionsFolder+2) .. " ...")
	local rawPackage = dofile(path)

	local rawType = rawPackage.type
	local package = {
		type = typeRemaps[rawType] or rawType,
	}

	if (rawPackage.markdown) then
		package.description = rawPackage.markdown
		package.descriptionPlain = rawPackage.description
	else
		package.description = rawPackage.description
	end

	if (rawType == "lib" or rawType == "class") then
		-- Look to see if a directory exists for children.
		local childrenDirectory = folder .. "\\" .. key
		if (isDirectory(childrenDirectory)) then
			for entry in lfs.dir(childrenDirectory) do
				local extension = entry:match("[^.]+$")
				if (extension == "lua") then
					buildAPI(childrenDirectory, entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), package, rawPackage)
				end
			end
		end

		insertField(parentPackage, key, package)
	end

	if (rawType == "class" and rawPackage.inherits) then
		derivedTypes[key] = rawPackage.inherits
	end

	if (rawType == "function") then
		assert(parentRawPackage == nil or parentRawPackage.type == "lib" or parentRawPackage.type == "class")
		setupFunction(package, rawPackage)
		insertField(parentPackage, key, package)
	elseif (rawType == "method") then
		assert(parentRawPackage.type == "class")
		setupFunction(package, rawPackage)
		insertMethod(parentPackage, key, package)

		-- Ensure that methods have "self" as the first parameter.
		if (package.args) then
			table.insert(package.args, 1, { name = "self" })
		else
			package.args = { { name = "self" } }
		end
	elseif (rawType == "value") then
		if (standardTypes[rawPackage.valuetype]) then
			package.type = typeRemaps[rawPackage.valuetype] or rawPackage.valuetype
			insertField(parentPackage, key, package)
		else
			package.type = "ref"
			package.name = rawPackage.valuetype
			insertField(parentPackage, key, package)
		end
	end

	-- Increment definition count.
	definitionCount = definitionCount + 1
end

-- Go through our types.
for entry in lfs.dir(definitionsFolder .. "\\namedTypes") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		buildAPI(definitionsFolder .. "\\namedTypes", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), namedTypes)
	end
end

-- Go through and build our API table.
for entry in lfs.dir(definitionsFolder .. "\\global") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		buildAPI(definitionsFolder .. "\\global", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), global)
	end
end

-- Add TES3 constants.
local tes3Fields = global.fields.tes3.fields
local tes3ConstantsFolder = lfs.currentdir() .. "\\..\\misc\\package\\Data Files\\MWSE\\core\\tes3"
for entry in lfs.dir(tes3ConstantsFolder) do
	local extension = entry:match("[^.]+$")
	if (extension == "lua" and entry ~= "init.lua") then
		local key = entry:match("[^/]+$"):sub(1, -1 * (#extension + 2))
		local rawPackage = dofile(tes3ConstantsFolder .. "\\" .. entry)
		tes3Fields[key] = { type = "table", fields = {} }
		for k, v in pairs(rawPackage) do
			tes3Fields[key].fields[k] = { name = k, type = type(v) }
		end
	end
end

-- Let us know how many definitions we figured out.
log("Finished parsing %d definitions.", definitionCount)

-- Patch in types.
local outJson = json.loadfile(lfs.currentdir() .. "\\luacompleterc\\base.luacompleterc")
for k, v in pairs(namedTypes.fields) do
	log("Patching type: %s", k)
	if (outJson.namedTypes[k]) then
		error("Unhandled clashing type: " .. k)
	else
		outJson.namedTypes[k] = v
	end
end

-- Patch derived types.
while (not isTableEmpty(derivedTypes)) do
	local removeList = {}
	for k, v in pairs(derivedTypes) do
		if (derivedTypes[v] == nil) then
			local newDefinition = {}
			copyDefinition(newDefinition, outJson.namedTypes[k], outJson.namedTypes[v])
			outJson.namedTypes[k] = newDefinition
			log("Set type %s to be derived from %s.", k, v)
			table.insert(removeList, k)
		end
	end

	for _, v in pairs(removeList) do
		derivedTypes[v] = nil
	end
end

-- Patch in globals.
for k, v in pairs(global.fields) do
	log("Patching global: %s", k)
	if (outJson.global.fields[k]) then
		local existing = outJson.global.fields[k]
		for i, j in pairs(v.fields) do
			existing.fields[i] = j
		end
	else
		outJson.global.fields[k] = v
	end
end

-- We're done for the autocomplete, so write it out.
local outputFileAutocomplete = lfs.currentdir() .. "\\..\\misc\\package\\Data Files\\MWSE\\.luacompleterc"
json.savefile(outputFileAutocomplete, outJson, { indent = true })
log("Saved to file: %s", outputFileAutocomplete)

-- Get the .luacheckrc file ready to write.
local outputFileLuaCheck = lfs.currentdir() .. "\\..\\misc\\package\\Data Files\\MWSE\\.luacheckrc"
local fLuaCheckRC = io.open(outputFileLuaCheck, "w")

-- Start off by dumping the base file.
local basePackage = io.open(lfs.currentdir() .. "\\luacompleterc\\base.luacheckrc", "r")
fLuaCheckRC:write(basePackage:read("*a"))
basePackage:close()

-- Write out the global string.
local luaCheckGlobals = {}
for k, _ in pairs(global.fields) do
	table.insert(luaCheckGlobals, k)
end
for k, _ in pairs(namedTypes.fields) do
	table.insert(luaCheckGlobals, k)
end
fLuaCheckRC:write(string.format("globals = {\n\t\"%s\"\n}", table.concat(luaCheckGlobals, "\",\n\t\"")))

-- We're done writting .luacheckrc, close it up.
fLuaCheckRC:close()
