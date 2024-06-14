local mcm = {}
---@deprecated
mcm.noParent = true
mcm.version = 1.5

--- @param template mwseMCMTemplate
function mcm.register(template)
	local modConfig = {}

	--- @param container tes3uiElement
	modConfig.onCreate = function(container)
		template:create(container)
		modConfig.onClose = template.onClose
	end
	mwse.log("%s mod config registered", template.name)
	mwse.registerModConfig(template.name, modConfig)
end

--- @param keybind mwseKeyCombo
--- @return boolean pressed
function mcm.testKeyBind(keybind)
	local inputController = tes3.worldController.inputController
	return inputController:isKeyDown(keybind.keyCode) and keybind.isShiftDown == inputController:isShiftDown() and
	       keybind.isAltDown == inputController:isAltDown() and keybind.isControlDown == inputController:isControlDown()
end

local mouseWheelDirectionName = {
	[1] = "Mouse wheel up",
	[-1] = "Mouse wheel down",
}

--- @param mouseWheel integer|nil
--- @return string|nil result
function mcm.getMouseWheelName(mouseWheel)
	if not mouseWheel then
		return
	end
	-- Support directly passing mouseWheelEventData.delta
	mouseWheel = math.clamp(mouseWheel, -1, 1)
	local name = mouseWheelDirectionName[mouseWheel]
	if name then
		return mwse.mcm.i18n(name)
	end
end

local mouseButtonName = {
	[0] = "Left mouse button",
	[1] = "Right mouse button",
	[2] = "Middle mouse button",
}

--- @param buttonIndex number|nil
--- @return string|nil result
function mcm.getMouseButtonName(buttonIndex)
	-- Only work with button indices supported by the inputController
	if not buttonIndex or buttonIndex > 7 or buttonIndex < 0 then
		return
	end
	local name = mouseButtonName[buttonIndex]
	if name then
		return mwse.mcm.i18n(name)
	end

	return string.format(mwse.mcm.i18n("Mouse %s"), buttonIndex)
end

--- @param keyCombo mwseKeyCombo|mwseKeyMouseCombo
--- @return string|nil result
function mcm.getKeyComboName(keyCombo)
	local keyCode = keyCombo.keyCode
	local comboText = tes3.getKeyName(keyCode) or
	                  mcm.getMouseWheelName(keyCombo.mouseWheel) or
	                  mcm.getMouseButtonName(keyCombo.mouseButton)

	-- No base name, nothing to do.
	if not comboText then
		return
	end

	local hasAlt = (keyCombo.isAltDown and keyCode ~= tes3.scanCode.lAlt
	                                   and keyCode ~= tes3.scanCode.rAlt)
	local hasShift = (keyCombo.isShiftDown and keyCode ~= tes3.scanCode.lShift
	                                       and keyCode ~= tes3.scanCode.rShift)
	local hasCtrl = (keyCombo.isControlDown and keyCode ~= tes3.scanCode.lCtrl
	                                        and keyCode ~= tes3.scanCode.rCtrl)
	local prefixes = {}
	if hasShift then table.insert(prefixes, "Shift") end
	if hasAlt then table.insert(prefixes, "Alt") end
	if hasCtrl then table.insert(prefixes, "Ctrl") end
	table.insert(prefixes, comboText)
	return table.concat(prefixes, " - ")
end

-- Depreciated
function mcm.registerModData(mcmData)
	-- object returned to be used in modConfigMenu
	local modConfig = {}

	---CREATE MCM---
	--- @param container tes3uiElement
	function modConfig.onCreate(container)
		local templateClass = mcmData.template or "Template"
		local templatePath = ("mcm.components.templates." .. templateClass)
		local template = require(templatePath):new(mcmData) --[[@as mwseMCMTemplate]]
		template:create(container)
		modConfig.onClose = template.onClose
	end

	mwse.log("%s mod config registered", mcmData.name)

	return modConfig
end

-- Depreciated
function mcm.registerMCM(mcmData)
	local newMCM = mcm.registerModData(mcmData)
	mwse.registerModConfig(mcmData.name, newMCM)
end

--[[
	Check if key being accessed is in the form "create{class}" where
	{class} is a component or variable class.

	If only component data was sent as a parameter, create the new
	component instance. If a parentBlock was also passed, then also
	create the element on the parent.

]]--

local strLengthCreate = string.len("create")


mcm.components = {}
mcm.variables = {}
local prefixLength = string.len("Data Files\\MWSE\\core\\")

-- Store component/variable paths so we only have to iterate the directory once. 
-- (Testing has shown this results in a noticeable boost in performance.)

local componentPaths = {}
local variablePaths = {}

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\components\\") do
	-- For example, when adding the path of `mwseMCMPage`:
	-- The key   will be:  "Page" 
	--        instead of:  "Page.lua"
	-- The value will be:  "mcm.components.pages.Page" 
	-- 		  instead of:  "data files\\mwse\\core\\mcm\\components\\pages\\Page.lua"
	componentPaths[fileName:sub(1, fileName:len() - 4)] = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")
end

for filePath, dir, fileName in lfs.walkdir("data files\\mwse\\core\\mcm\\variables\\") do
	variablePaths[fileName:sub(1, fileName:len() - 4)] = filePath:sub(1 + prefixLength, filePath:len() - 4):gsub("[\\/]", ".")
end

---@protected
---@param className string The name of the class of the `mwseMCMComponent` to search for.
---@return mwseMCMComponent?
function mcm.getComponentClass(className)
	local class = mcm.components[className]
	if class then return class end
	if className == "HyperLink" then
		return mcm.getComponentClass("Hyperlink")
	end
	if className == "SidebarPage" then
		return mcm.getComponentClass("SideBarPage")
	end
	local luaPath = componentPaths[className]
	if luaPath then
		class = include(luaPath)
		if class and type(class) == "table" then
			class.class = className -- Store it now so we don't have to do this every time.
			mcm.components[className] = class
			return class
		end
	end
end

---@protected
---@param className string The name of the class of the `mwseMCMVariable` to search for.
---@return mwseMCMVariable?
function mcm.getVariableClass(className)
	local class = mcm.variables[className]
	if class then return class end
	local luaPath = variablePaths[className]
	if luaPath then
		class = include(luaPath)
		if class and type(class) == "table" then
			class.class = className -- Store it now so we don't have to do it every time.
			mcm.variables[className] = class
			return class
		end
	end
end



-- Add the `create<Component|Variable>` functions.
-- This will be done via the `__index` metamethod as follows:
--	1. The first time a `create` function is called, it will trigger the `mcm.getComponent` function.
--		- This function will search the `mcm\\components` folder until it finds the relevant component.
--		- It will then cache that component so it only needs to search the filetree once.
--		- The component will be returned by `getComponent` (if it exists)
--	2. If no component is returned by `mcm.getComponent`, a similar search will be done on `mwseMCMVariable`s.
--  3. If a `mwseMCMComponent` or a `mwseMCMVariable` is returned by Step 1 or Step 2., we will:
--		- Create a new function processes the given data and returns a new instance of the relevant class.
--		- Store this new function in the `mcm` table (so this whole process only happens one time per call to `create<Component|Variable>`).
--		- Return this new function.
setmetatable(mcm, {__index=function (_, key) ---@param key string
	if key:sub(1, strLengthCreate) ~= "create" then return end

	local className = key:sub(strLengthCreate + 1)
	
	-- First check if it's a component.
	local componentClass = mcm.getComponentClass(className)
	if componentClass then

		-- Store the function so we don't have to recreate it every time.
		mcm[key] = function(param1, param2)
			local data, parent = param1, nil
			if param2 then
				data = param2
				-- Add check for mcm field to deal with using `:` instead of `.`
				if param1 ~= mcm then
					parent = param1
				end
			end
			data = data or "---"
			if type(data) == "string" then
				if componentClass.componentType == "Template" then
					data = { name = data }
				else
					data = { label = data }
				end
			end
			local component = componentClass:new(data)
			if parent then
				component:create(parent)
			end
			return component
		end
		return mcm[key]
	end
	-- Now check if it's a variable.
	local variableClass = mcm.getVariableClass(className)
	if variableClass then
		-- Store the function so we don't have to recreate it every time.
		mcm[key] = function(param1, param2)
			return variableClass:new(param2 or param1)
		end
		return mcm[key]
	end
end})


return mcm
