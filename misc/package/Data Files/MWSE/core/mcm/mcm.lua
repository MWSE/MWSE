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

local utils = require("mcm.utils")
local strLengthCreate = string.len("create")

--[[Add the `create<Component|Variable>` functions.
This will be done via the `__index` metamethod as follows:
1. The first time a `create` function is called, it will try to fetch the relevant class.
	- This is done by calling `utils.getComponentClass` and `utils.getVariableClass`
2. If a class was returned in Step 1:
	- Create a new function sanitizes the input data and returns a new instance of the relevant class.
	- Store this new function in the `mcm` table (so this whole process only happens one time per call to `create<Componen|Variablet>`).
	- Return this new function (as the return-value of the `__index` metamethod).
3. If a class WAS NOT returned in Step 1, then do nothing.
]]
---@param key string
setmetatable(mcm, {__index = function(_, key)
	if key:sub(1, strLengthCreate) ~= "create" then return end

	local className = key:sub(strLengthCreate + 1)

	-- First check if it's a component.
	local componentClass = utils.getComponentClass(className)
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

			-- Sanitize data
			if not data then
				data = { label = "---"}
			elseif type(data) == "string" then
				if componentClass.componentType == "Template" then
					data = { name = data }
				else
					data = { label = data}
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
	local variableClass = utils.getVariableClass(className)
	if variableClass then

		-- Store the function so we don't have to recreate it every time.
		mcm[key] = function(param1, param2)
			return variableClass:new(param2 or param1)
		end
		return mcm[key]
	end
end})


return mcm
