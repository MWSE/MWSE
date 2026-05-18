--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Dropdown")

local pathResolver = require("logger.pathResolver")


--- @class mwseMCMLogLevelOptions : mwseMCMDropdown
--- @field logger mwseLogger
local LogLevelOptions = Parent:new()
LogLevelOptions.__index = LogLevelOptions

LogLevelOptions.label = mwse.mcm.i18n("Logging Level")
LogLevelOptions.options = {
	{ label = mwse.mcm.i18n("Trace"), value = mwse.logLevel.trace },
	{ label = mwse.mcm.i18n("Debug"), value = mwse.logLevel.debug },
	{ label = mwse.mcm.i18n("Info"),  value = mwse.logLevel.info },
	{ label = mwse.mcm.i18n("Warn"),  value = mwse.logLevel.warn },
	{ label = mwse.mcm.i18n("Error"), value = mwse.logLevel.error },
	{ label = mwse.mcm.i18n("None"),  value = mwse.logLevel.none },
}
LogLevelOptions.description = mwse.mcm.i18n("Log level options description")

---@class LogOptions.new.data : mwseMCMDropdown.new.data
---@field logger mwseLogger|string|nil

---@param data LogOptions.new.data
---@return mwseMCMLogLevelOptions
function LogLevelOptions:new(data)
	local logger = data.logger

	if logger == nil then
		local modName, autoModDir = pathResolver.getModNameAndDirAndFilepath(2)
		local modDir = autoModDir or modName
		logger = mwse.Logger.get(modDir)
	elseif type(logger) == "string" then
		--- @cast logger string
		-- Support resolving the logger by passing the mod's name.
		logger = mwse.Logger.getLogger(logger) --- @diagnostic disable-line
	end

	data.logger = logger --- @diagnostic disable-line: assign-type-mismatch
	if not data.logger or (type(data.logger.setLevel) ~= "function") then
		error("[LogLevelOptions] Couldn't resolve a valid logger.")
	end

	return Parent.new(self, data) --[[@as mwseMCMLogLevelOptions]]
end

function LogLevelOptions:update()
	self.logger:setLevel(self.variable.value)
	Parent.update(self)
end

return LogLevelOptions
