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

-- Check if the keys of `t1` are a subset of the keys of `t2`.
local function isSubset(t1, t2)
	-- Something is a subset if it's not not a subset
    for k in pairs(t1) do
        if t2[k] == nil then return false end
    end
    return true
end

-- Finds a default value, given a variable. 
-- This function handles nested subtables, but it is sometimes incorrect.
---@param var mwseMCMTableVariable
---@param defaultTable table A table nested inside a `defaultConfig`. 
-- This could be equal to `defaultConfig` itself
---@return any defaultValue
local function findRecursive(var, defaultTable)

    -- A loaded `configTable` could have keys that are not in the default table.
    -- But, if it was loaded with `mwse.loadConfig`, then every key in `configTable` will be in `defaultTable`.
	-- So, the subset check is a heuristic that helps to increase the likelihood of picking the correct subtable.
    if defaultTable[var.id] ~= nil and isSubset(defaultTable, var.table) then
        return defaultTable[var.id]
    end

	-- Key not in `defaultTable`, or `defaultTable` wasn't a good match? Check subtables.
    for _, defaultVal in pairs(defaultTable) do
        if type(defaultVal) == "table" then
            local res = findRecursive(var, defaultVal)
            if res ~= nil then 
                return res
            end
        end
    end
end

--- This will recursively go through your MCM and append the text "Default = ___" to the description of each setting.
---@param defaultConfig table? the default config of your mod. if not provided, it will try to be retrieved, 
-- using the path "config.default"
function Setting:addDefaultsToDescriptions(defaultConfig)
    if not defaultConfig then return end

	local var = self.variable
	if not var or var.class ~= "TableVariable" then return end
	---@cast var mwseMCMTableVariable

	local defaultVal = var.defaultSetting
	if defaultVal == nil then
		defaultVal = findRecursive(var, defaultConfig)
	end
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
