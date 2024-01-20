--
-- ReadTheDocs generator for MWSE-lua definitions.
--

local lfs = require("lfs")
local common = require("builders.common")
common.defaultNoDescriptionText = "No description available."

common.log("Starting build of ReadTheDocs rst files...")

-- Recreate output folders.
local docsSourceFolder = lfs.join(common.pathAutocomplete, "..\\docs\\source")
local docsLuaSourceFolder = lfs.join(docsSourceFolder, "lua")

local rstHeaders = {
	"====================================================================================================",
	"----------------------------------------------------------------------------------------------------",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
}


--
-- Handle link caching to optimize RTD build times.
--
-- We have to do some silly things to get the lua plugin to know we extend the file api.
--

local writtenLinks = {}

local _io_open = io.open

local updatedMetatable = false
function io.open(filename, mode)
	local handle = _io_open(filename, mode)
	if (handle and not updatedMetatable) then
		local metatable	= getmetatable(handle)

		function metatable:cachedwrite(str, ...)
			local output = str:format(...)
			writtenLinks[self] = writtenLinks[self] or {}
			for capture in string.gmatch(output, "`.-`_") do
				writtenLinks[self][string.sub(capture, 2, -3)] = true
			end
			return self:write(output)
		end

		function metatable:uncachedwrite(str, ...)
			return self:write(str:format(...))
		end

		updatedMetatable = true
	end
	return handle
end

--- @diagnostic disable-next-line
--- @class file*
local file = {}
function file:cachedwrite(str, ...) end
function file:uncachedwrite(str, ...) end
file = nil


--
-- Compile data
--

--- @type table<string, packageLib>
local libraries = {}
common.compilePath(lfs.join(common.pathDefinitions, "global"), libraries, "lib")

--- @type table<string, packageClass>
local classes = {}
common.compilePath(lfs.join(common.pathDefinitions, "namedTypes"), classes, "class")

--- @type table<string, package>
local events = {}
-- common.compilePath(lfs.join(common.pathDefinitions, "events\\standard"), events, "event")


--
-- Build output
--

local function breakoutMultipleTypes(str)
	return "`" .. table.concat(string.split(str, "|"), "`_, `") .. "`_"
end

local function getPackageDescription(package)
	local prefix = ""
	if (package.type == "value" and package.valuetype) then
		prefix = prefix .. breakoutMultipleTypes(package.valuetype) .. ". "
	end
	if (package.readOnly) then
		prefix = prefix .. "Read-only. "
	end
	return prefix .. (package.description or common.defaultNoDescriptionText)
end

local function getArgumentDescription(argument)
	local prefix = ""
	if (argument.default ~= nil) then
		prefix = prefix .. string.format("Default: ``%s``. ", tostring(argument.default))
	elseif (argument.optional) then
		prefix = prefix .. "Optional. "
	end
	return prefix .. (argument.description or common.defaultNoDescriptionText)
end

--- comment
--- @param package package
--- @return number
local function getParentCount(package)
	local count = 0
	while (package.parent) do
		count = count + 1
		package = package.parent
	end
	return count
end

--- comment
--- @param package table
--- @param field any
--- @param results any
--- @return table
local function getPackageComponentsDictionary(package, field, results)
	results = results or {}

	local onThis = package[field]
	if (onThis) then
		for k, v in pairs(onThis) do
			if (results[k] == nil) then
				results[k] = v
			end
		end
	end

	if (package.inherits and classes[package.inherits]) then
		return getPackageComponentsDictionary(classes[package.inherits], field, results)
	end

	return results
end

--- comment
--- @param package table
--- @param field any
--- @param results any
--- @return table
local function getPackageComponentsArray(package, field, results)
	results = results or {}

	local onThis = package[field]
	if (onThis) then
		for _, v in ipairs(table.values(onThis)) do
			if (results[v.key] == nil) then
				results[v.key] = v
			end
		end
	end

	if (package.inherits and classes[package.inherits]) then
		return getPackageComponentsArray(classes[package.inherits], field, results)
	end

	return results
end

local function sortPackagesByKey(A, B)
	return A.key:lower() < B.key:lower()
end

--- comment
--- @param package packageClass
--- @param outDir string
local function build(package, outDir)
	-- Load our base package.
	common.log("Building %s: %s ...", package.type, package.key)

	-- Get the package.
	local outPath = lfs.join(outDir, package.key .. ".rst")
	local file = assert(io.open(outPath, "w+"))

	--
	local isParentLibrary = package.parent and package.parent.type == "lib"
	file:cachedwrite("%s\n%s\n\n", isParentLibrary and package.namespace or package.key, rstHeaders[1])
	file:cachedwrite("%s\n\n", getPackageDescription(package))

	if (package.type == "function" or package.type == "method") then
		-- Show return values.
		local returns = common.getConsistentReturnValues(package)
		if (returns) then
			file:cachedwrite("Returns\n%s\n\n", rstHeaders[2])
			if (#returns > 1) then
				file:cachedwrite("The function has more than one return value.\n\n")
			end
			for _, ret in ipairs(returns) do -- changed to `ipairs`
				local name, type = common.getNameAndType(ret)
				file:cachedwrite("%s (%s)\n    %s\n\n", name, breakoutMultipleTypes(type), getArgumentDescription(ret))
			end
			-- file:cachedwrite("\n") -- TODO: Re-add this once output is matched.
		end

		-- Show function arguments.
		if (package.arguments and #package.arguments > 0) then
			file:cachedwrite("Parameters\n%s\n\n", rstHeaders[2])
			if (package.arguments[1].tableParams) then
				file:cachedwrite("Accepts parameters through a table with the given keys:\n\n")
				for _, arg in ipairs(package.arguments[1].tableParams) do
					local name, type = common.getNameAndType(arg)
					file:cachedwrite("%s (%s)\n    %s\n\n", name, breakoutMultipleTypes(type), getArgumentDescription(arg))
				end
			else
				file:cachedwrite("Accepts parameters in the following order:\n\n")
				for _, arg in ipairs(package.arguments) do
					local name, type = common.getNameAndType(arg)
					file:cachedwrite("%s (%s)\n    %s\n\n",name, breakoutMultipleTypes(type), getArgumentDescription(arg))
				end
			end
			-- file:cachedwrite("\n") -- TODO: Re-add this once output is matched.
		end
	elseif (package.type == "lib" or package.type == "class") then
		if (package.type == "class" and package.inherits) then
			local types = {}
			local inherits = classes[package.inherits]
			while (inherits) do
				table.insert(types, inherits.key)
				inherits = classes[inherits.inherits]
			end
			file:cachedwrite("This type inherits from the following parent types: %s\n\n", breakoutMultipleTypes(table.concat(types, "|")))
		end

		--
		local values = table.values(getPackageComponentsArray(package, "values"), sortPackagesByKey)
		if (#values > 0) then
			local subtitle = package.type == "lib" and "Values" or "Properties"
			file:cachedwrite("%s\n%s\n\n", subtitle, rstHeaders[2])
			for _, fn in ipairs(values) do
				file:uncachedwrite("`%s <%s/%s.html>`_", fn.key, fn.parent.key, fn.key)
				file:cachedwrite(" (%s)\n    %s\n\n", breakoutMultipleTypes(fn.valuetype or "any"), getArgumentDescription(fn))
			end
			file:cachedwrite("\n\n")
			file:cachedwrite(".. toctree::\n")
			file:cachedwrite("    :hidden:\n")
			file:cachedwrite("    :maxdepth: 1\n\n")
			for _, fn in ipairs(values) do
				if (fn.parent == package) then
					file:cachedwrite("    %s/%s\n", package.key, fn.key)
				end
			end
			file:cachedwrite("\n")
		end

		--
		local methods = table.values(getPackageComponentsArray(package, "methods"), sortPackagesByKey)
		if (#methods > 0) then
			file:cachedwrite("Methods\n%s\n\n", rstHeaders[2])
			for _, fn in ipairs(methods) do
				file:uncachedwrite("`%s <%s/%s.html>`_", fn.key, fn.parent.key, fn.key)
				file:cachedwrite(" (%s)\n    %s\n\n", breakoutMultipleTypes(fn.type or "any"), getArgumentDescription(fn))
			end
			file:cachedwrite("\n\n")
			file:cachedwrite(".. toctree::\n")
			file:cachedwrite("    :hidden:\n")
			file:cachedwrite("    :maxdepth: 1\n\n")
			for _, fn in ipairs(methods) do
				if (fn.parent == package) then
					file:cachedwrite("    %s/%s\n", package.key, fn.key)
				end
			end
			file:cachedwrite("\n")
		end

		--
		local functions = table.values(getPackageComponentsArray(package, "functions"), sortPackagesByKey)
		if (#functions > 0) then
			file:cachedwrite("Functions\n%s\n\n", rstHeaders[2])
			for _, fn in ipairs(functions) do
				file:uncachedwrite("`%s <%s/%s.html>`_", fn.key, fn.parent.key, fn.key)
				file:cachedwrite(" (%s)\n    %s\n\n", breakoutMultipleTypes(fn.type or "any"), getArgumentDescription(fn))
			end
			file:cachedwrite("\n\n")
			file:cachedwrite(".. toctree::\n")
			file:cachedwrite("    :hidden:\n")
			file:cachedwrite("    :maxdepth: 1\n\n")
			for _, fn in ipairs(functions) do
				if (fn.parent == package) then
					file:cachedwrite("    %s/%s\n", package.key, fn.key)
				end
			end
			file:cachedwrite("\n")
		end
	elseif (package.type == "event") then
		-- TODO: Flush this out if we don't don't migrate to GitHub pages.
	end

	-- Write out the links we've written.
	local linkPositionReset = string.rep("../", getParentCount(package) + 2)
	for _, link in ipairs(table.keys(writtenLinks[file], true)) do
		file:cachedwrite(".. _`%s`: %slua/type/%s.html\n", link, linkPositionReset, link)
	end

	-- Close up shop.
	writtenLinks[file] = nil
	file:close()

	-- Write out children in a subdirectory.
	if (package.children and not table.empty(package.children)) then
		lfs.mkdir(lfs.join(outDir, package.key))
		for _, child in pairs(package.children) do
			build(child, lfs.join(outDir, package.key))
		end
	end
end

--- comment
--- @param collection any
--- @param outdir any
local function buildBuilder(collection, outdir)
	lfs.remakedir(lfs.join(docsLuaSourceFolder, outdir))
	for _, package in pairs(collection) do
		build(package, lfs.join(docsLuaSourceFolder, outdir))
	end
end

buildBuilder(libraries, "api")
buildBuilder(classes, "type")
-- buildBuilder(events, "event")
