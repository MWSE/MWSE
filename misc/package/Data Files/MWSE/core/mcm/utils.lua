-- Fetches Components and Variables by class name.
-- Intended for internal use only.
local utils = {}

-- Cache all `Component` and `Variable` classes, to minimize disk usage.
local components = {} ---@type table<string, mwseMCMComponent>
local variables = {}  ---@type table<string, mwseMCMVariable>


-- Store component/variable paths so we only have to iterate the directory once.
-- Due to circular dependencies, these files cannot be `require`d immediately. So instead, the filepaths will be stored
--      ahead of time, and then `require`d as needed.
-- Files get `require`d in by the `utils.get<Component|Variable>Class` functions. (They actually get `include`d for stability reasons.)
-- Once a file has been `require`d by the `utils.get<Component|Variable>Class` functions, its path will be removed from these tables.
-- (Testing has shown that this approach results in a noticeable boost in performance.)

local componentPaths = {} --- @type table<string, string>
local variablePaths = {}  --- @type table<string, string>

local prefixLength = string.len("Data Files\\MWSE\\core\\")

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\components\\") do
	-- For example, when adding the path of `mwseMCMPage`:
	-- "Page" instead of "Page.lua"
	local className = fileName:sub(1, fileName:len() - 4)

	-- "mcm.components.pages.Page" instead of "data files\\mwse\\core\\mcm\\components\\pages\\Page.lua"
	local luaPath = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")

	componentPaths[className] = luaPath
end
-- Add backwards compatibility by redirecting old component names to new ones.
componentPaths.template = componentPaths.Template
componentPaths.HyperLink = componentPaths.Hyperlink
componentPaths.SidebarPage = componentPaths.SideBarPage

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\variables\\") do
	local className = fileName:sub(1, fileName:len() - 4)
	local luaPath = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")

	variablePaths[className] = luaPath
end



--- @protected
--- @param className string The name of the class of the `mwseMCMComponent` to search for.
--- @return mwseMCMComponent?
function utils.getComponentClass(className)
	local class = components[className]
	if class then return class end

	local luaPath = componentPaths[className]
	if not luaPath then return end
	class = include(luaPath)
	if not class or type(class) ~= "table" then
		mwse.log('[MCM: ERROR] Could not find the filepath of "%s"!\n%s', luaPath, debug.traceback())
		return
	end

	-- Initialize the `class` field of this `class`, and cache the class in the `components` table.
	class.class = className
	components[className] = class

	componentPaths[className] = nil -- Won't be needing this anymore.

	return class
end

--- @protected
--- @param className string The name of the class of the `mwseMCMVariable` to search for.
--- @return mwseMCMVariable?
function utils.getVariableClass(className)
	local class = variables[className]
	if class then return class end
	local luaPath = variablePaths[className]

	if not luaPath then return end
	class = include(luaPath)
	if not class or type(class) ~= "table" then
		mwse.log('[MCM: ERROR] Could not find the filepath of "%s"!\n%s', luaPath, debug.traceback())
		return
	end

	-- Initialize the `class` field of this `class`, and cache the class in the `variables` table.
	class.class = className
	variables[className] = class

	variablePaths[className] = nil -- Won't be needing this anymore.

	return class
end

-- Update the new setting so that:
-- 1) it inherits `config`, `defaultConfig`, and `showDefaultSetting` values from parent component.
-- 2) its `variable` is properly initialized (or created from `config` and `configKey` parameters, if applicable).
--- @param setting mwseMCMSetting|mwseMCMExclusionsPage
function utils.getOrInheritVariableData(setting)
	local configKey = setting.configKey
	local parent = setting.parentComponent
	if parent and setting.showDefaultSetting == nil then
		-- Using `rawget` so we don't inherit a default value
		setting.showDefaultSetting = rawget(parent, "showDefaultSetting")
	end

	local config = setting.config or parent and parent.config
	local defaultConfig = setting.defaultConfig or parent and parent.defaultConfig

	-- Get the default setting. Include `nil` checks so we can handle it being `false`.
	local defaultSetting = setting.variable and setting.variable.defaultSetting
	if defaultSetting == nil then
		defaultSetting = setting.defaultSetting
	end
	-- Let's try again if we have to.
	if defaultSetting == nil and defaultConfig and configKey then
		defaultSetting = defaultConfig[configKey]
	end

	-- No variable? Let's make one.
	if setting.variable == nil and config and configKey then
		setting.variable = mwse.mcm.createTableVariable({
			id = configKey,
			table = config,
			converter = setting.converter,
			inGameOnly = setting.inGameOnly,
			defaultSetting = defaultSetting,
			restartRequired = setting.restartRequired,
			restartRequiredMessage = setting.restartRequiredMessage
		})
	-- Variable provided? Let's update it for backwards compatibility.
	elseif type(setting.variable) == "table" then
		setting.variable.defaultSetting = defaultSetting
		setting.variable.converter = setting.variable.converter or setting.converter
		--- @diagnostic disable-next-line: param-type-mismatch
		setting.variable = utils.getVariableClass(setting.variable.class):new(setting.variable)
	end
end


return utils
