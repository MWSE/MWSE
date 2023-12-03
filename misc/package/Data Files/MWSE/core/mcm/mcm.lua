local mcm = {}
mcm.noParent = true
mcm.version = 1.5

function mcm:new()
	local t = {}
	setmetatable(t, self)
	t.__index = mcm.__index
	return t
end

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

--- @param keyCode integer|nil
--- @return string|nil letter
function mcm.getKeyCodeLetter(keyCode)
	local letter = table.find(tes3.scanCode, keyCode)
	local returnString = tes3.scanCodeToNumber[keyCode] or letter
	if returnString then
		return string.upper(returnString)
	end
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

function mcm.__index(tbl, key)

	local meta = getmetatable(tbl)
	if string.sub(key, 1, strLengthCreate) == "create" then

		local class = string.sub(key, strLengthCreate + 1)
		local component

		local classPaths = require("mcm.classPaths")
		for _, path in pairs(classPaths.all) do

			local classPath = path .. class
			local fullPath = lfs.currentdir() .. classPaths.basePath .. classPath .. ".lua"
			local fileExists = lfs.fileexists(fullPath)
			if fileExists then
				component = require(classPath)
			end

			if component then
				--- @cast component mwseMCMComponent
				return function(param1, param2)
					local parent = nil
					local data = nil
					if param2 then
						parent = param1
						data = param2
					else
						data = param1
					end
					if not data then
						data = "---"
					end
					if type(data) == "string" then
						if component.componentType == "Template" then
							data = { name = data }
						else
							data = { label = data }
						end
					end
					data.class = class

					component = component:new(data)
					-- Add check for mcm field to deal with using `:` instead of `.`
					if parent and parent.noParent ~= true then
						component:create(parent)
					end
					return component
				end
			end
		end
	end

	return meta[key]
end

return mcm:new()
