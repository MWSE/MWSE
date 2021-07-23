--
-- ReadTheDocs generator for MWSE-lua definitions.
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
   return match(s,'^()%s*$') and '' or match(s,'^%s*(.*%S)')
end

local function splitString(inputstr, sep)
	if sep == nil then
		sep = "%s"
	end
	local t={}
	for str in string.gmatch(inputstr, "([^"..sep.."]+)") do
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

local standardTypes = {
	["function"] = true,
	["table"] = true,
	["number"] = true,
	["boolean"] = true,
	["string"] = true,
	["unknown"] = true,
	["ref"] = true,
}

local rstHeaders = {
	"====================================================================================================",
	"----------------------------------------------------------------------------------------------------",
	"~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~",
}

local typeLinks = {
	["bool"] = "lua/type/boolean",
	["boolean"] = "lua/type/boolean",
	["function"] = "lua/type/function",
	["nil"] = "lua/type/nil",
	["number"] = "lua/type/number",
	["string"] = "lua/type/string",
	["table"] = "lua/type/table",
}

for entry in lfs.dir(definitionsFolder .. "\\namedTypes") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		local name = entry:match("[^/]+$"):sub(1, -1 * (#extension + 2))
		typeLinks[name] = "lua/type/" .. name
	end
end


local function breakoutMultipleTypes(str)
	return "`" .. table.concat(splitString(str, "|"), "`_, `") .. "`_"
end

-- 
-- Handle link caching to optimize RTD build times.
-- 

local cachingLinks = -1
local writtenLinks = {}
local originalFileWrite = nil

local originalIOOpen = io.open
local fileMetatable = nil

-- Handle nested caching begin/stop
local function beginCachingLinks()
	cachingLinks = cachingLinks + 1
end

local function isCachingLinks()
    return cachingLinks > -1
end

local function stopCachingLinks()
	cachingLinks = cachingLinks - 1
end

local function isLinkCached(file, link)
	return writtenLinks[file] and writtenLinks[file][link] == true
end

local function cachedWrite(self, str)
	if (isCachingLinks()) then
		writtenLinks[self] = writtenLinks[self] or {}
		for capture in string.gmatch(str, "`.-`_") do
			writtenLinks[self][string.sub(capture, 2, -3)] = true
		end
	end

	return originalFileWrite(self, str)
end

function io.open(...)
	local file = originalIOOpen(...)
	if (file) then
		if (not originalFileWrite) then
			local mt = getmetatable(file)
			originalFileWrite = mt.write
			mt.write = cachedWrite
		end
		writtenLinks[file] = writtenLinks[file] or {}
	end
	return file
end

-- 
-- Events
-- 

local function buildEvent(folder, key)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	log("Building event: " .. key .. " ...")
	local package = dofile(path)

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\event\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Print out the main description.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (lfs.attributes(packagePath .. "\\" .. "description.rst", "mode") == "file") then
		local rstFilePath = packagePath .. "description.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (package.description) then
		file:write(package.description .. "\n\n")
	else
		file:write("No description is currently available for this event." .. "\n\n")
	end

	-- Write out event data.
	if (package.eventData) then
		file:write("Event Data\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.eventData) do
			file:write(k .. "\n" .. rstHeaders[3] .. "\n\n")

			if (typeLinks[v.type]) then
				file:write(breakoutMultipleTypes(v.type) .. ". ")
			else
				file:write(v.type .. ". ")
			end

			if (v.readOnly) then
				file:write("Read-only. ")
			end

			file:write(v.description .. "\n\n")
		end
	end

	-- Write out examples.
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in pairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

for entry in lfs.dir(definitionsFolder .. "\\events\\standard") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		buildEvent(definitionsFolder .. "\\events\\standard", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)))
	end
end

-- 
-- API
-- 

local function getStandardizedReturn(raw)
	if (type(raw.valuetype) == "string") then
		return { { type = raw.valuetype, name = raw.returns, description = raw.returnDescription } }
	elseif (type(raw.returns) == "table") then
		local returns = {}

		for _, v in pairs(raw.returns) do
			table.insert(returns, v)
		end

		if (#returns == 0) then
			return nil
		end

		return returns
	end
end

local function buildAPIEntryForFunction(package)
	local key = package.key
	local folder = package.folder
	local parent = package.parent
	parent.functions = parent.functions or {}
	table.insert(parent.functions, package)

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\api\\" .. parent.key .. "\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(parent.key .. "." .. key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Print out the main description.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (lfs.attributes(packagePath .. "\\" .. "description.rst", "mode") == "file") then
		local rstFilePath = packagePath .. "description.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (package.description) then
		file:write(package.description .. "\n\n")
	else
		file:write("No description is currently available." .. "\n\n")
	end

	-- Print out the return value.
	local returns = getStandardizedReturn(package)
	if (lfs.attributes(packagePath .. "\\" .. "returns.rst", "mode") == "file") then
		file:write("Returns\n" .. rstHeaders[2] .. "\n\n")
		local rstFilePath = packagePath .. "returns.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (returns) then
		file:write("Returns\n" .. rstHeaders[2] .. "\n\n")
		for _, r in pairs(returns) do
			if (r.description) then
				file:write(breakoutMultipleTypes(r.type) .. ". " .. r.description .. "\n\n")
			else
				file:write(breakoutMultipleTypes(r.type) .. ".\n\n")
			end
		end
	end

	-- Print out parameters.
	if (package.arguments) then
		file:write("Parameters\n" .. rstHeaders[2] .. "\n\n")
		if (type(package.arguments) == "table" and #package.arguments == 1 and package.arguments[1].tableParams) then
			-- Add it to the description.
			file:write("Accepts parameters through a table with the given keys:\n\n")
			for _, param in pairs(package.arguments[1].tableParams) do
				file:write((param.name or "...") .. " (" .. breakoutMultipleTypes(param.type) .. ")\n")
				file:write("    ")
				if (param.default ~= nil) then
					file:write("Default: ``" .. tostring(param.default) .. "``")
				elseif (param.optional) then
					file:write("Optional")
				end
				
				if ((param.default ~= nil) or param.optional) then
					file:write(". " .. (param.description or "No description available.") .. "\n\n")
				else
					file:write((param.description or "No description available.") .. "\n\n")
				end
			end
		else
			file:write("Accepts parameters in the following order:\n\n")
			for _, param in ipairs(package.arguments) do
				file:write((param.name or "...") .. " (" .. breakoutMultipleTypes(param.type) .. ")\n")
				file:write("    ")
				if (param.default ~= nil) then
					file:write("Default: ``" .. tostring(param.default) .. "``")
				elseif (param.optional) then
					file:write("Optional")
				end
				
				if ((param.default ~= nil) or param.optional) then
					file:write(". " .. (param.description or "No description available.") .. "\n\n")
				else
					file:write((param.description or "No description available.") .. "\n\n")
				end
			end
		end
	end

	-- Write out examples.
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in ipairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

local function buildAPIEntryForValue(package)
	local key = package.key
	local folder = package.folder
	local parent = package.parent
	parent.values = parent.values or {}
	table.insert(parent.values, package)

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\api\\" .. parent.key .. "\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(parent.key .. "." .. key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Print out the main description.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (lfs.attributes(packagePath .. "\\" .. "description.rst", "mode") == "file") then
		local rstFilePath = packagePath .. "description.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (package.description) then
		if (package.valuetype) then
			file:write(breakoutMultipleTypes(package.valuetype) .. ". " .. package.description .. "\n\n")
		else
			file:write(package.description .. "\n\n")
		end
	else
		file:write("No description is currently available." .. "\n\n")
	end

	-- Write out examples.
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in pairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

local apiEntryDispatchers = {
	["function"] = buildAPIEntryForFunction,
	["value"] = buildAPIEntryForValue,
}

local function buildAPIEntry(folder, key, parent)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	local package = dofile(path)
	package.key = key
	package.folder = folder
	package.parent = parent

	local dispatch = apiEntryDispatchers[package.type]
	if (dispatch == nil) then
		log("%s: No dispatcher found for type: %s", path, package.type)
		return
	end

	dispatch(package)
end

local function buildAPI(folder, key)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	log("Building API: " .. key .. " ...")
	local package = dofile(path)
	package.key = key

	-- We only care about libraries for now.
	if (package.type ~= "lib") then
		return
	end

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\api\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Write out the main header.
	file:write(key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Write description.
	file:write((package.description .. "\n\n") or "No description yet available.\n\n")

	-- Create a folder for children.
	lfs.mkdir("..\\docs\\source\\lua\\api\\" .. key)

	-- Write out children.
	for entry in lfs.dir(folder .. "\\" .. key) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			buildAPIEntry(folder .. "\\" .. key, entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), package)
		end
	end

	-- Write TOCs.
	if (package.values) then
		file:write("Values\n" .. rstHeaders[2] .. "\n\n")
		file:write(".. toctree::\n    :maxdepth: 1\n\n")
		for _, k in pairs(package.values) do
			file:write("    " .. key .. "/" .. k.key .. "\n")
		end
		file:write("\n")
	end
	if (package.functions) then
		file:write("Functions\n" .. rstHeaders[2] .. "\n\n")
		file:write(".. toctree::\n    :maxdepth: 1\n\n")
		for _, k in pairs(package.functions) do
			file:write("    " .. key .. "/" .. k.key .. "\n")
		end
		file:write("\n")
	end

	-- Close up shop.
	file:close()
end

for entry in lfs.dir(definitionsFolder .. "\\global") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		buildAPI(definitionsFolder .. "\\global", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)))
	end
end

--
-- Named Types
--

local function buildNamedTypeEntryForValue(package)
	local key = package.key
	local folder = package.folder
	local parent = package.parent
	parent.values = parent.values or {}
	table.insert(parent.values, package)

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\type\\" .. parent.key .. "\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Print out the main description.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (lfs.attributes(packagePath .. "\\" .. "description.rst", "mode") == "file") then
		local rstFilePath = packagePath .. "description.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (package.description) then
		if (package.valuetype) then
			file:write(breakoutMultipleTypes(package.valuetype) .. ". " .. package.description .. "\n\n")
		else
			file:write(package.description .. "\n\n")
		end
	else
		file:write("No description is currently available." .. "\n\n")
	end

	-- Write out examples.
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in pairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

local function buildNamedTypeEntryForFunction(package)
	local key = package.key
	local folder = package.folder
	local parent = package.parent

	if (package.type == "method") then
		parent.methods = parent.methods or {}
		table.insert(parent.methods, package)
	else
		parent.functions = parent.functions or {}
		table.insert(parent.functions, package)
	end

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\type\\" .. parent.key .. "\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Print out the main description.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (lfs.attributes(packagePath .. "\\" .. "description.rst", "mode") == "file") then
		local rstFilePath = packagePath .. "description.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (package.description) then
		file:write(package.description .. "\n\n")
	else
		file:write("No description is currently available." .. "\n\n")
	end

	-- Print out the return value.
	local returns = getStandardizedReturn(package)
	if (lfs.attributes(packagePath .. "\\" .. "returns.rst", "mode") == "file") then
		file:write("Returns\n" .. rstHeaders[2] .. "\n\n")
		local rstFilePath = packagePath .. "returns.rst"
		local rstFile = io.open(rstFilePath, "r")
		file:write(trimString(rstFile:read("*a")) .. "\n\n")
		rstFile:close()
	elseif (returns) then
		file:write("Returns\n" .. rstHeaders[2] .. "\n\n")
		for _, r in pairs(returns) do
			if (r.description) then
				file:write(breakoutMultipleTypes(r.type) .. ". " .. r.description .. "\n\n")
			else
				file:write(breakoutMultipleTypes(r.type) .. ".\n\n")
			end
		end
	end

	-- Print out parameters.
	if (package.arguments) then
		file:write("Parameters\n" .. rstHeaders[2] .. "\n\n")
		if (type(package.arguments) == "table" and #package.arguments == 1 and package.arguments[1].tableParams) then
			-- Add it to the description.
			file:write("Accepts parameters through a table with the given keys:\n\n")
			for _, param in pairs(package.arguments[1].tableParams) do
				file:write((param.name or "...") .. " (" .. breakoutMultipleTypes(param.type) .. ")\n")
				file:write("    ")
				if (param.default ~= nil) then
					file:write("Default: ``" .. tostring(param.default) .. "``")
				elseif (param.optional) then
					file:write("Optional")
				end
				
				if ((param.default ~= nil) or param.optional) then
					file:write(". " .. (param.description or "No description available.") .. "\n\n")
				else
					file:write((param.description or "No description available.") .. "\n\n")
				end
			end
		else
			file:write("Accepts parameters in the following order:\n\n")
			for _, param in ipairs(package.arguments) do
				file:write((param.name or "...") .. " (" .. breakoutMultipleTypes(param.type) .. ")\n")
				file:write("    ")
				if (param.default ~= nil) then
					file:write("Default: ``" .. tostring(param.default) .. "``")
				elseif (param.optional) then
					file:write("Optional")
				end
				
				if ((param.default ~= nil) or param.optional) then
					file:write(". " .. (param.description or "No description available.") .. "\n\n")
				else
					file:write((param.description or "No description available.") .. "\n\n")
				end
			end
		end
	end

	-- Write out examples.
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in pairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

local typeEntryDispatchers = {
	["value"] = buildNamedTypeEntryForValue,
	["method"] = buildNamedTypeEntryForFunction,
	["function"] = buildNamedTypeEntryForFunction,
}

local function buildNamedTypeEntry(folder, key, parent)
	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	local package = dofile(path)
	if (package == nil) then
		error("Could not execute typed entry: " .. path)
	end

	package.key = key
	package.folder = folder
	package.parent = parent

	local dispatch = typeEntryDispatchers[package.type]
	if (dispatch == nil) then
		log("%s: No dispatcher found for type: %s", path, package.type)
		return
	end

	dispatch(package)
end

local builtTypes = {}

local function sortByKey(a, b)
	return a.key < b.key
end

local function writeOutPackageEntries(t, file, package, title)
	if (t == nil or #t == 0) then
		return
	end

	file:write(title .. "\n" .. rstHeaders[2] .. "\n\n")

	-- 
	for _, v in ipairs(t) do
		if (v.valuetype) then
			file:write("`" .. v.key .. "`_ (" .. breakoutMultipleTypes(v.valuetype) .. ")\n")
		else
			file:write("`" .. v.key .. "`_\n")
		end
		file:write("    ")
		if (v.readOnly) then
			file:write("Read-only. ")
		end
		file:write((v.brief or v.description or "No description available.") .. "\n\n")
	end

	-- Build out hidden TOC tree.
	file:write(".. toctree::\n    :hidden:\n\n")
	for _, v in ipairs(t) do
		file:write("    " .. package.key .. "/" .. v.key .. "\n")
	end
	file:write("\n")

	-- Also put in link definitions.
	for _, v in ipairs(t) do
		file:write(".. _`" .. v.key .. "`: " .. package.key .. "/" .. v.key .. ".html\n")
	end
	file:write("\n")
end

local function buildNamedType(folder, key)
	-- Skip redundantly building the same package twice.
	if (builtTypes[key]) then
		return
	end

	-- Load our base package.
	local path = folder .. "\\" .. key .. ".lua"
	log("Building type: " .. key .. " ...")
	local package = dofile(path)
	package.key = key
	builtTypes[key] = package

	-- We only care about libraries for now.
	if (package.type ~= "class") then
		log("%s: Skipping type '%s'", path, key)
		return
	end

	-- Build the inherited class if it hasn't been built already.
	if (package.inherits and not builtTypes[package.inherits]) then
		buildNamedType(folder, package.inherits)
	end

	-- Open the output file.
	local outPath = "..\\docs\\source\\lua\\type\\" .. key .. ".rst"
	local file = io.open(outPath, "w")

	-- Keep track of what links are written.
	beginCachingLinks()

	-- Write out the main header.
	file:write(key .. "\n" .. rstHeaders[1] .. "\n\n")

	-- Write description.
	file:write((package.description .. "\n\n") or "No description yet available.\n\n")

	-- Create a folder for children.
	lfs.mkdir("..\\docs\\source\\lua\\type\\" .. key)

	-- Write out inherited children.
	if (package.inherits) then
		-- Copy over raw files.
		local inheritedFolder = "..\\docs\\source\\lua\\type\\" .. package.inherits
		for entry in lfs.dir(inheritedFolder) do
			local extension = entry:match("[^.]+$")
			if (extension == "rst") then
				local inheritedPath = inheritedFolder .. "\\" .. entry
				local localPath = "..\\docs\\source\\lua\\type\\" .. key .. "\\" .. entry
				copyFile(inheritedPath, localPath)
			end
		end

		-- Copy over properties.
		local inheritedType = builtTypes[package.inherits]
		if (inheritedType.values) then
			package.values = package.values or {}
			for k, v in pairs(inheritedType.values) do
				package.values[k] = v
			end
		end
		if (inheritedType.methods) then
			package.methods = package.methods or {}
			for k, v in pairs(inheritedType.methods) do
				package.methods[k] = v
			end
		end
		if (inheritedType.functions) then
			package.functions = package.functions or {}
			for k, v in pairs(inheritedType.functions) do
				package.functions[k] = v
			end
		end
	end

	-- Write out children.
	for entry in lfs.dir(folder .. "\\" .. key) do
		local extension = entry:match("[^.]+$")
		if (extension == "lua") then
			buildNamedTypeEntry(folder .. "\\" .. key, entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)), package)
		end
	end

	-- Sort tables by contained keys.
	if (package.values) then table.sort(package.values, sortByKey) end
	if (package.methods) then table.sort(package.methods, sortByKey) end
	if (package.functions) then table.sort(package.functions, sortByKey) end
	if (package.examples) then table.sort(package.examples, sortByKey) end

	-- Write out properties, etc.
	writeOutPackageEntries(package.values, file, package, "Properties")
	writeOutPackageEntries(package.methods, file, package, "Methods")
	writeOutPackageEntries(package.functions, file, package, "Functions")

	-- Write out examples.
	local packagePath = folder .. "\\" .. key .. "\\"
	if (package.examples) then
		file:write("Examples\n" .. rstHeaders[2] .. "\n\n")
		for k, v in pairs(package.examples) do
			file:write((v.title or k) .. "\n" .. rstHeaders[3] .. "\n\n")

			if (v.description) then
				file:write(v.description .. "\n\n")
			end

			file:write(".. code-block:: " .. (v.language or "lua") .. "\n\n")

			local exampleFilePath = packagePath .. key .. "\\" .. k .. "." .. (v.extension or "lua")
			for line in io.lines(exampleFilePath) do
				if (trimString(line) ~= "") then
					file:write("    " .. line:gsub("\t", "    ") .. "\n")
				else
					file:write("\n")
				end
			end
			file:write("\n\n")
		end
	end

	-- Restore original write function.
	stopCachingLinks()

	-- Write out any link information.
	package.links = package.links or {}
	for _, k in ipairs(getSortedKeys(package.links)) do
		if isLinkCached(file, k) then
			file:write(".. _`" .. k .. "`: ../../" .. package.links[k] .. ".html\n")
		end
	end

	-- Also write out all our type links.
	for _, k in pairs(getSortedKeys(typeLinks)) do
		if (not package.links[k] and isLinkCached(file, k)) then
			file:write(".. _`" .. k .. "`: ../../" .. typeLinks[k] .. ".html\n")
		end
	end

	-- Close up shop.
	file:close()
end

for entry in lfs.dir(definitionsFolder .. "\\namedTypes") do
	local extension = entry:match("[^.]+$")
	if (extension == "lua") then
		buildNamedType(definitionsFolder .. "\\namedTypes", entry:match("[^/]+$"):sub(1, -1 * (#extension + 2)))
	end
end
