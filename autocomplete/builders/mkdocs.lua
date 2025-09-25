--
-- mkdocs generator for MWSE-lua definitions.
--

local lfs = require("lfs")
local common = require("builders.common")

common.log("Starting build of mkdocs source files...")

-- Recreate meta folder.
local docsSourceFolder = lfs.join(common.pathAutocomplete, "..\\docs\\source")
lfs.remakedir(lfs.join(docsSourceFolder, "apis"))
lfs.remakedir(lfs.join(docsSourceFolder, "types"))
lfs.remakedir(lfs.join(docsSourceFolder, "events"))

-- Base containers to hold our compiled data.
---@type table<string, package>
local globals = {}
---@type table<string, packageClass>
local classes = {}
---@type table<string, package>
local events = {}
---@type table<string, string>
local typeLinks = {
	-- The automatic link resolving doesn't work for these enumerations.
	-- These came as warnings in the output of the mkdocs - Start Server task defined at: "docs/.vscode/tasks.json"
	["ni.animCycleType"] = "[ni.animCycleType](../references/ni/animation-cycle-types.md)",
	["ni.animType"] = "[ni.animType](../references/ni/animation-types.md)",
	["ni.cameraClearFlags"] = "[ni.cameraClearFlags](../references/ni/camera-clear-flags.md)",
	["ni.eulerRotKeyOrder"] = "[ni.eulerRotKeyOrder](../references/ni/euler-rotation-key-orders.md)",
	["ni.lookAtControllerAxis"] = "[ni.lookAtControllerAxis](../references/ni/look-at-controller-axes.md)",
	["ni.pickIntersectType"] = "[ni.pickIntersectType](../references/ni/pick-intersection-types.md)",
	["ni.textureFormatPrefsAlphaFormat"] = "[ni.textureFormatPrefsAlphaFormat](../references/ni/texture-format-preference-alpha-formats.md)",
	["ni.textureFormatPrefsMipFlag"] = "[ni.textureFormatPrefsMipFlag](../references/ni/texture-format-preference-mip-flags.md)",
	["ni.textureFormatPrefsPixelLayout"] = "[ni.textureFormatPrefsPixelLayout](../references/ni/texture-format-preference-pixel-layouts.md)",
	["ni.zBufferPropertyTestFunction"] = "[ni.zBufferPropertyTestFunction](../references/ni/z-buffer-property-test-functions.md)",
	["tes3.armorWeightClass"] = "[tes3.armorWeightClass](../references/armor-weight-classes.md)",
	["tes3.dialogueFilterContext"] = "[tes3.dialogueFilterContext](../references/dialogue-filter-context.md)",
	["tes3.effect"] = "[tes3.effect](../references/magic-effects.md)",
	["tes3.soundGenType"] = "[tes3.soundGenType](../references/sound-generator-types.md)",
	["tes3.gmst"] = "[tes3.gmst](../references/gmst.md)",
	["tes3.justifyText"] = "[tes3.justifyText](../references/justify-text.md)",
	["tes3.partIndex"] = "[tes3.partIndex](../references/part-indices.md)",
	["tes3.soundMix"] = "[tes3.soundMix](../references/sound-mix-types.md)",
	["tes3.uiElementType"] = "[tes3.uiElementType](../references/tes3uiElement-types.md)",
	["tes3.uiProperty"] = "[tes3.uiProperty](../references/ui-properties.md)",
	["tes3.weather"] = "[tes3.weather](../references/weather-types.md)",
}

--
-- Utility functions.
--

common.log("Definitions folder: %s", common.pathDefinitions)


--
-- Compile data
--

common.compilePath(lfs.join(common.pathDefinitions, "global"), globals)
common.compilePath(lfs.join(common.pathDefinitions, "namedTypes"), classes, "class")
common.compilePath(lfs.join(common.pathDefinitions, "events", "standard"), events, "event")


--
-- Building
--

--- @param packages package[]
--- @return package[] packages The provided packages without the deprecated fields.
local function removeDeprecated(packages)
	for i, package in ipairs(packages) do
		if package.deprecated then
			packages[i] = nil
		end
	end
	-- Make sure there are no gaps in the returned array
	return table.values(packages)
end

---@param className string
---@return string
local function buildParentChain(className)
	local package = assert(classes[className])
	local ret = ""
	if (classes[className]) then
		ret = string.format("[%s](../types/%s.md)", className, className)
	else
		ret = className
	end
	if (package.inherits) then
		ret = ret .. ", " .. buildParentChain(package.inherits)
	end
	return ret
end

--- @param enum string
--- @return string
local function splitCamelCase(enum)
	-- Make the first letter uppercase
	enum = enum:gsub("^%l", string.upper)
	-- Insert dash between lowercase and uppercase character
	enum = enum:gsub("(%l)(%u)", "%1-%2")
	return enum:lower()
end

--- @param type string Supports array annotation. For example: "tes3weather[]".
--- @return string
local function getTypeLink(type)
	if typeLinks[type] then
		return typeLinks[type]
	end

	local valueType = type:match("[%w%.]+")
	local isArray = type:endswith("[]")
	local namespace, field = type:match("([_%a][_%w]+)%.(.*)") --[[@as string]]
	if isArray then
		-- Exclude the ending square brackets [].
		namespace, field = type:sub(1, -3):match("([_%a][_%w]+)%.(.*)") --[[@as string]]
	end
	local enums = common.getEnumerationsMap(namespace)
	local isEnum = enums and enums[field]

	if classes[valueType] then
		typeLinks[type] = string.format("[%s](../types/%s.md)%s", valueType, valueType, isArray and "[]" or "")
	elseif isEnum then
		local enumName = namespace .. "." .. field
		if isArray then
			local cached = typeLinks[enumName]
			if cached then
				typeLinks[type] = cached .. "[]"
				return typeLinks[type]
			end
		end
		local filename = splitCamelCase(field)
		local basepath = "../references/"
		-- Reference pages for enumeration tables in tes3 namespace are one folder up
		if namespace ~= "tes3" then
			basepath = basepath .. namespace .. "/"
		end
		typeLinks[type] = string.format("[%s](%s%ss.md)%s", enumName, basepath, filename, isArray and "[]" or "")
	else
		-- Cache the types without any entry in the docs (e.g. primitive Lua types, string constants, etc.)
		typeLinks[type] = type
	end

	return typeLinks[type]
end

--- This function converts a union of types to an array. E.g.:<br>
--- `"table<integer, tes3ref|tes3mobile>|fun(self: infoGetTextEventData|string): boolean, string|integer"` to:
--- ```lua
--- {
---    "table<integer, tes3ref|tes3mobile>",
---    "fun(self: infoGetTextEventData|string): boolean, string",
---    "integer",
--- }
--- ```
--- @param union string
--- @return string[]
local function breakoutUnion(union)
	local types = {}

	while union ~= "" do
		if union:startswith("|") then
			union = union:sub(2, -1)
		end

		local i, _, tableStr = union:find("(table%b<>)([^|]*)")
		if i == 1 then -- A table found, e.g. table<integer, tes3reference|tes3actor>
			table.insert(types, tableStr)
			union = union:sub(tableStr:len() + 1)
			goto continue
		end

		local i, _, functionParams, functionReturns = union:find("(fun%b())([^|]*)")
		if i == 1 then
			local functionStr = functionParams .. functionReturns
			table.insert(types, functionStr)
			union = union:sub(functionStr:len() + 1)
			goto continue
		end

		local type = union:match("[^|]+")
		table.insert(types, type)
		union = union:sub(type:len() + 1)
		::continue::
	end
	return types
end

--- @param type string
--- @param nested boolean?
--- @return string
local function breakoutTypeLinks(type, nested)
	local types = breakoutUnion(type)

	for i, t in ipairs(types) do
		-- Support "table<x, y>" as type, in HTML < and > signs have a special meaning.
		-- Use "&lt;" and "&gt;" instead.
		if t:startswith("table<") then
			local keyType, valueType = t:match("(%b<,) (.*)>")
			-- Let's remove the "<" from the beginning and "," at the end
			keyType = keyType:sub(2, -2)
			keyType = breakoutTypeLinks(keyType, true)
			valueType = breakoutTypeLinks(valueType, true)

			types[i] = string.format("table&lt;%s, %s&gt;", keyType, valueType)
		elseif t:startswith("fun(") then
			-- Let's take "fun(e: mwseTimerCallbackData, param2): boolean, string"
			-- params: "(e: mwseTimerCallbackData, param2)",
			-- ret: "boolean, string"
			local params, ret = t:match("fun(%b())[:]?[%s]?(.*)")

			-- Handle the parameters
			-- "e: mwseTimerCallbackData, param2"
			params = params:sub(2, -2)
			local p = {}
			for i, paramStr in ipairs(params:split(",")) do
				-- paramStr: "e: mwseTimerCallbackData", " param2"
				paramStr = paramStr:trim()
				-- name: "e", type: "mwseTimerCallbackData" or
				-- name: "param2", type: nil
				local name, type = table.unpack(paramStr:split("%s?:%s?"))
				p[i] = ("%s%s"):format(
					name,
					type and (": " .. breakoutTypeLinks(type, true)) or ""
				)
			end
			params = table.concat(p, ", ")

			-- Handle the return values
			if ret then
				local r = {}
				for i, returnStr in ipairs(ret:split(", ")) do
					-- returnStr: "boolean", "string"
					r[i] = breakoutTypeLinks(returnStr, true)
				end
				ret = table.concat(r, ", ")
			end

			types[i] = string.format("fun(%s)%s",
				params or "",
				(ret ~= "") and (": " .. ret) or ""
			)
		else
			types[i] = getTypeLink(t)
		end
	end
	return table.concat(types, nested and "|" or ", ")
end

--- Converts the `related` table in event definitions to a set of markdown buttons:
--- https://squidfunk.github.io/mkdocs-material/reference/buttons/#adding-buttons
--- @param related string[]
--- @return string
local function relatedButtons(related)
	local ret = { "\n## Related events\n\n" }
	for _, eventName in ipairs(related) do
		ret[#ret + 1] = string.format("[%s](./%s.md){ .md-button }", eventName, eventName)
	end
	return table.concat(ret)
end

--- comment
--- @param package package|packageClass
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

	-- Check if it's a `packageClass`
	if (package.inherits and classes[package.inherits]) then
		return getPackageComponentsArray(classes[package.inherits], field, results)
	end

	return results
end

---@param A package
---@param B package
---@return boolean
local function sortPackagesByKey(A, B)
	return A.key:lower() < B.key:lower()
end

---@param file file* the IO file
---@param argument packageFunctionArgument
---@param indent string?
local function writeArgument(file, argument, indent)
	indent = indent or ""

	file:write(string.format("%s* `%s`", indent, argument.name or "unnamed"))

	if (argument.type) then
		file:write(string.format(" (%s)", breakoutTypeLinks(argument.type)))
	end

	local description = common.getDescriptionString(argument, false)
	if (description) then
		file:write(string.format(": %s", description))
	end

	file:write("\n")

	if (argument.tableParams) then
		for _, tableArg in ipairs(argument.tableParams) do
			writeArgument(file, tableArg, "\t" .. indent)
		end
	end
end

---@param argument packageFunctionArgument
local function getArgumentCode(argument)
	if (argument.tableParams) then
		local tableArgs = {}
		for _, arg in ipairs(argument.tableParams) do
			table.insert(tableArgs, string.format("%s = ...", arg.name))
		end
		return string.format("{ %s }", table.concat(tableArgs, ", "))
	end
	return argument.name or "unknown"
end


---@param file file*
---@param package package
---@param from package
local function writeSubPackage(file, package, from) end

local operatorToTitle = {
	unm = "Unary minus (`-`)",
	add = "Addition (`+`)",
	sub = "Subtraction (`-`)",
	mul = "Multiplication (`*`)",
	div = "Division (`/`)",
	idiv = "Floor division (`//`)",
	mod = "Modulo (`%`)",
	pow = "Exponentiation (`^`)",
	concat = "Concatenation (`..`)",
	len = "Length (`#`)",
	eq = "Equality (`==`)",
}

---@param file file*
---@param operator packageOperator
---@param package package
local function writeOperatorPackage(file, operator, package)
	file:write(string.format("### %s\n\n", operatorToTitle[operator.key]))

	local notUnary = operator.overloads[1].rightType and true

	if (notUnary) then
		file:write("| Left operand type | Right operand type | Result type | Description |\n")
		file:write("| ----------------- | ------------------ | ----------- | ----------- |\n")
		for _, overload in ipairs(operator.overloads) do
			file:write(string.format("| %s | %s | %s | %s |\n",
				getTypeLink(package.namespace),
				getTypeLink(overload.rightType),
				getTypeLink(overload.resultType),
				overload.description or "")
			)
		end
	else
		file:write("| Result type | Description |\n")
		file:write("| ----------- | ----------- |\n")
		for _, overload in ipairs(operator.overloads) do
			file:write(string.format("| %s | %s |\n", getTypeLink(overload.resultType), overload.description or ""))
		end
	end

	file:write("\n")
end

--- This function is used to write out examples of a package.
--- @param file file* The file to write to.
--- @param package package The package whose examples will be written out.
local function writeExamples(file, package)
	local exampleType = "???"
	if (package.type == "event") then
		file:write("## Examples\n\n")
		exampleType = "!!!"
	end

	local exampleKeys = table.keys(package.examples, true)
	for _, name in ipairs(exampleKeys) do
		local example = package.examples[name]
		file:write(string.format("%s example \"Example: %s\"\n\n", exampleType, example.title or name))
		if (example.description) then
			file:write(string.format("\t%s\n\n", string.gsub(example.description, "\n\n", "\n\n\t")))
		end
		file:write(string.format("\t```lua\n"))

		local path = nil
		if (package.type == "class") then
			path = lfs.join(package.folder, package.key, package.key, name .. ".lua")
		else
			path = lfs.join(package.folder, package.key, name .. ".lua")
		end
		for line in io.lines(path) do
			file:write(string.format("\t%s\n", line))
		end
		file:write(string.format("\n\t```\n\n"))
	end
end

--- This function is used to write out properties, methods, functions, and operators of a package.
--- @param file file* The file to write to.
--- @param package package The package whose fields that will be written out.
--- @param field string The name of the fields to write. Can be "values", "methods", "functions", and "operators"
--- @param fieldName string The name of the section for the fields.
--- @param writeFunction fun(file: file*, package: package, from: package) The function that write a single package definition.
--- @param writeRule boolean If true a horizontal rule will be written before the section.
--- @return boolean written True if at least one field was written to file.
local function writeFields(file, package, field, fieldName, writeFunction, writeRule)
	local fields = table.values(getPackageComponentsArray(package, field), sortPackagesByKey)
	fields = removeDeprecated(fields)
	local count = #fields
	-- No field that aren't deprecated? Nothing to do here.
	if (count == 0) then
		return false
	end

	if (writeRule) then
		file:write("***\n\n")
	end

	file:write(string.format("## %s\n\n", fieldName))

	for i, field in ipairs(fields) do
		if (not field.deprecated) then
			writeFunction(file, field, package)
			if (i < count) then
				file:write("***\n\n")
			end
		end
	end
	return true
end

--- @param file file*
--- @param package package|packageEvent|packageClass|packageFunction
local function writePackageDetails(file, package)
	-- Write description.
	file:write(string.format("%s\n\n", common.getDescriptionString(package)))
	if (package.type == "class") then
		if (package.inherits) then
			file:write(string.format("This type inherits the following: %s.\n", buildParentChain(package.inherits)))
		end

		-- Write class examples before the methods and properties
		if (package.examples) then
			writeExamples(file, package)
		end
	elseif (package.type == "event") then
		file:write(string.format("```lua\n--- @param e %sEventData\nlocal function %sCallback(e)\nend\nevent.register(tes3.event.%s, %sCallback)\n```\n\n", package.key, package.key, package.key, package.key))
		if (package.filter) then
			file:write(string.format("!!! tip\n\tThis event can be filtered based on the **`%s`** event data.\n\n", package.filter))
		end
		if (package.blockable) then
			file:write("!!! tip\n\tThis event supports blocking by setting `e.block` to `true` or returning `false`. Blocking the event prevents vanilla behavior from happening. For example, blocking an `equip` event prevents the item from being equipped.\n\n")
		end
		file:write("!!! tip\n\tAn event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.\n\n")
	end

	local needsHorizontalRule = false

	-- Write out fields.
	needsHorizontalRule = (
		writeFields(file, package, "values", "Properties", writeSubPackage, needsHorizontalRule)
		or needsHorizontalRule
	)

	needsHorizontalRule = (
		writeFields(file, package, "typeValues", "Type Properties", writeSubPackage, needsHorizontalRule)
		or needsHorizontalRule
	)

	-- Write out methods.
	needsHorizontalRule = (
		writeFields(file, package, "methods", "Methods", writeSubPackage, needsHorizontalRule)
		or needsHorizontalRule
	)

	-- Write out functions.
	needsHorizontalRule = (
		writeFields(file, package, "functions", "Functions", writeSubPackage, needsHorizontalRule)
		or needsHorizontalRule
	)

	-- Write out operators.
	needsHorizontalRule = (
		writeFields(file, package, "operators", "Math Operations", writeOperatorPackage, needsHorizontalRule)
		or needsHorizontalRule
	)

	---@diagnostic disable-next-line: param-type-mismatch
	local returns = common.getConsistentReturnValues(package)
	if (package.type == "method" or package.type == "function") then
		if (package.link) then
			file:write(string.format("[Official documentation](%s).\n\n", package.link))
		end
		---@cast package packageFunction|packageMethod
		file:write(string.format("```lua\n", package.namespace))
		if (returns) then
			local returnNames = {}
			for i, ret in ipairs(returns) do
				table.insert(returnNames, ret.name or string.format("unnamed%d", i))
			end
			file:write(string.format("local %s = ", table.concat(returnNames, ", ")))
		end
		if (package.parent) then
			if (package.type == "method") then
				file:write(string.format("%s:%s(", "myObject", package.key))
			else
				file:write(string.format("%s(", package.namespace))
			end
		else
			file:write(string.format("%s(", package.key))
		end
		if (package.arguments) then
			local args = {}
			for _, arg in pairs(package.arguments) do
				table.insert(args, getArgumentCode(arg))
			end
			file:write(table.concat(args, ", "))
		end
		file:write(string.format(")\n```\n\n", package.namespace))
	end

	if (package.arguments) then
		file:write(string.format("**Parameters**:\n\n"))
		for _, argument in ipairs(package.arguments) do
			writeArgument(file, argument, "")
		end
		file:write("\n")
	end

	if (returns) then
		file:write(string.format("**Returns**:\n\n"))
		for _, ret in ipairs(returns) do
			writeArgument(file, ret, "")
		end
		file:write("\n")
	end

	-- Events are more top-level, need to do special handling...
	if (package.type == "event") then
		-- Write out event data.
		if (package.eventData) then
			file:write("## Event Data\n\n")
			local eventDataKeys = table.keys(package.eventData, true)
			for _, key in ipairs(eventDataKeys) do
				local data = package.eventData[key]
				data.name = key
				writeArgument(file, data)
			end
			file:write("\n")
		end
	end

	-- Class examples were written before
	if (package.examples and not (package.type == "class")) then
		writeExamples(file, package)
	end

	if (package.type == "event" and package.related) then
		file:write(relatedButtons(package.related), "\n\n")
	end
end

---@type {functions: string[], classes: string[]}
local identifierStems = {
	functions = {
		"get", "set", "mod",
		"is", "has", "can",
		"open", "close",
		"add", "remove",
		"enable", "disable",
		"apply", "update",
		"find", "show",
		"create", "delete",
		"test", "toggle",
	},
	classes = {
		"ni", "tes3ui", "tes3",
	}
}
---@param file file*
---@param key string
---@param stems string[]
local function writeSearchTerms(file, key, stems)
	-- Hidden search terms, to work around deficiencies in lunr.
	-- Include lower-cased variants of the identifier.
	-- Include lower-cased stemmed variant, unless it is a single character.
	local stemmedKey = nil
	for _, stem in ipairs(stems) do
		if (key:startswith(stem) and key ~= stem) then
			stemmedKey = string.sub(key, 1 + stem:len(), -1):lower()
			break
		end
	end

	file:write("<div class=\"search_terms\" style=\"display: none\">")
	local lowercaseKey = key:lower()
	if (stemmedKey and stemmedKey:len() > 1) then
		file:write(string.format("%s, %s", lowercaseKey, stemmedKey))
	else
		file:write(string.format("%s", lowercaseKey))
	end
	file:write("</div>\n\n")
end

---@param file file*
---@param package package
---@param from package
function writeSubPackage(file, package, from)
	-- Don't document deprecated APIs on the website.
	if (package.deprecated) then
		return
	end

	local key = package.key
	if (from.type == "lib") then
		key = string.format("%s.%s", from.namespace, package.key)
	end

	-- Add title and hidden search terms.
	file:write(string.format("### `%s`\n", key))
	writeSearchTerms(file, package.key, identifierStems.functions)

	writePackageDetails(file, package)
end

---@param package package
---@param outDir string
local function build(package, outDir)
	-- Load our base package.
	common.log("Building " .. package.type .. ": " .. package.namespace .. " ...")

	-- Get the package.
	local outPath = lfs.join(docsSourceFolder, outDir, package.namespace .. ".md")

	-- Create the file.
	local file = assert(io.open(outPath, "w"))

	-- Add title and hidden search terms. The heading has to be before any other line to be recognized as the title.
	-- This avoids mkdocs using the auto-capitalized filename as the sidebar title.
	file:write(string.format("# %s\n", package.namespace))
	writeSearchTerms(file, package.namespace, identifierStems.classes)

	-- Make it clear this is auto-generated.
	file:write("<!---\n")
	file:write("\tThis file is autogenerated. Do not edit this file manually. Your changes will be ignored.\n")
	file:write("\tMore information: https://github.com/MWSE/MWSE/tree/master/docs\n")
	file:write("-->\n\n")

	-- Warn of deprecated packages.
	if (package.deprecated) then
		file:write("!!! warning\n\tThis API is deprecated. See below for more information about what to use instead.\n\n")
	end

	-- Let's inline nested sub-libs to ensure that sub-globals are built.
	for _, lib in ipairs(package.libs or {}) do

		for _, child in ipairs(lib.functions or {}) do
			child.key = string.format("%s.%s", lib.key, child.key)
			table.insert(package.functions, child)
		end
	end

	writePackageDetails(file, package)

	-- Close up shop.
	file:close()
end

for _, package in pairs(globals) do
	build(package, "apis")
end

for _, package in pairs(classes) do
	build(package, "types")
end

for _, package in pairs(events) do
	build(package, "events")
end

--
-- Custom file setup.
--

-- Add our .pages files for retitling directories.
io.createwith(lfs.join(docsSourceFolder, "apis", ".pages"), "title: APIs")
