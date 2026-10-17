--[[
	A setting is a component that can be interacted with, such as a button or text input.
	It can have, but doesn't require, an associated variable.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local utils = require("mcm.utils")
local Parent = require("mcm.components.Component")


--- @class mwseMCMSetting
local Setting = Parent:new()
Setting.__index = Setting
Setting.componentType = "Setting"
Setting.restartRequired = false
Setting.restartRequiredMessage = mwse.mcm.i18n("The game must be restarted before this change will come into effect.")

--- @param data mwseMCMSetting.new.data|nil
--- @return mwseMCMSetting
function Setting:new(data)
	--- @diagnostic disable: param-type-mismatch
	local t = Parent.new(self, data) --[[@as mwseMCMSetting]]
	utils.getOrInheritVariableData(t)
	return t
end

function Setting:insertMouseovers(element)
	table.insert(self.mouseOvers, element)
	for _, child in ipairs(element.children or {}) do
		self:insertMouseovers(child)
	end
end

function Setting:setVariableValue(newValue)
	self.variable.value = newValue
	self:update()
end

function Setting:resetToDefault()
	local variable = self.variable
	if variable and variable.defaultSetting ~= nil then
		self:setVariableValue(variable.defaultSetting)
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

-- Returns the string that should be shown in the MouseOverInfo
---@return string?
function Setting:getMouseOverText()
	local var = self.variable
	local shouldAddDefaults = (self.showDefaultSetting and var and var.defaultSetting ~= nil)

	if not shouldAddDefaults then
		return self.description -- This has type `string|nil`
	end

	-- Now we add defaults to the description.
	local defaultStr = self:convertToLabelValue(var.defaultSetting)

	-- No description exists yet? Then we'll only write the default value.
	if not self.description then
		return string.format("%s: %s.", mwse.mcm.i18n("Default"), defaultStr)
	end

	return string.format(
		"%s\n\n\z
		 %s: %s.",
		self.description,
		mwse.mcm.i18n("Default"), defaultStr
	)
end


---@param searchText string The text being searched, in lowercase
---@param caseSensitive boolean
---@return boolean
function Setting:searchTextMatches(searchText, caseSensitive)
	if Parent.searchTextMatches(self, searchText, caseSensitive) then
		return true
	end

	-- Search variable text and defaultSetting text
	if self.variable then
		local value = self.variable.value
		local default = self.variable.defaultSetting
		-- Only search `default`if it exists and is different from `value`.
		if default ~= value then
			local defaultText = tostring(self:convertToLabelValue(default))
			defaultText = caseSensitive and defaultText or defaultText:lower()
			if defaultText:find(searchText, 1, true) then
				return true
			end
		end

		local valueText = tostring(self:convertToLabelValue(value))
		valueText = caseSensitive and valueText or valueText:lower()
		return valueText:find(searchText, 1, true) ~= nil
	end

	return false
end

return Setting
