--[[
	A setting is a component that can be interacted with, such as a button or text input.
	It can have, but doesn't require, an associated variable.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local fileUtils = require("mcm.fileUtils")
local Parent = require("mcm.components.Component")

--- @class mwseMCMSetting
local Setting = Parent:new()
Setting.componentType = "Setting"
Setting.restartRequired = false
Setting.restartRequiredMessage = mwse.mcm.i18n("The game must be restarted before this change will come into effect.")

--- @param data mwseMCMSetting.new.data|nil
--- @return mwseMCMSetting
function Setting:new(data)
	local t = Parent:new(data)

	if data and data.variable then
		-- create setting variable
		t.variable.defaultSetting = t.variable.defaultSetting or t.defaultSetting
		t.variable = fileUtils.getVariableClass(t.variable.class):new(t.variable)
	end

	setmetatable(t, self)
	self.__index = self
	--- @cast t mwseMCMSetting
	return t
end

function Setting:insertMouseovers(element)
	table.insert(self.mouseOvers, element)
	for _, child in ipairs(element.children or {}) do
		self:insertMouseovers(child)
	end
end

function Setting:update()
	if self.restartRequired then

		tes3.messageBox { message = self.restartRequiredMessage, buttons = { self.sOK } }
	end
	if self.callback then
		self:callback()
	end
end

function Setting:checkDisabled()
	-- override the variable
	if self.inGameOnly ~= nil then
		return not tes3.player and self.inGameOnly
	end
	-- Components with variable
	local disabled = (self.variable and self.variable.inGameOnly == true and not tes3.player)
	return disabled --[[@as boolean]]
end

--- @param parentBlock tes3uiElement
function Setting:createContentsContainer(parentBlock)
	self:createLabel(parentBlock)
	self:createInnerContainer(parentBlock)
	self:makeComponent(self.elements.innerContainer)
end


function Setting:convertToLabelValue(variableValue)
	return variableValue
end


--- This will recursively go through your MCM and append the text "Default = ___" to the description of each setting.
-- The default value will be pulled from `self.variable.defaultSetting`
function Setting:addDefaultsToDescriptions()

	local var = self.variable
	if not var or var.class ~= "TableVariable" then return end
	---@cast var mwseMCMTableVariable

	local defaultVal = var.defaultSetting
	if defaultVal == nil then return end

	-- String representation of the default value, according to this setting.
	local defaultStr = self:convertToLabelValue(defaultVal)

	if self.description == nil then
		self.description = string.format(
			"%s: %s", 
			mwse.mcm.i18n("Default"), defaultStr
		)
	else
		self.description = string.format(
			"%s\n\n%s: %s", 
			self.description, mwse.mcm.i18n("Default"), defaultStr
		)
	end
end

return Setting
