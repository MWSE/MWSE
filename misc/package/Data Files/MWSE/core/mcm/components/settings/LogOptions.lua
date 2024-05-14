
local Parent = require("mcm.components.settings.Dropdown")
local Logger = require("logging.logger")

--- @class mwseMCMLogOptions : mwseMCMDropdown
--- @field logger mwseLogger
local LogOptions = Parent:new()
LogOptions.options = {
	{ label = mwse.mcm.i18n("Trace"), value = "TRACE" },
	{ label = mwse.mcm.i18n("Debug"), value = "DEBUG" },
	{ label = mwse.mcm.i18n("Info"),  value = "INFO" },
	{ label = mwse.mcm.i18n("Warn"),  value = "WARN" },
	{ label = mwse.mcm.i18n("Error"), value = "ERROR" },
	{ label = mwse.mcm.i18n("None"),  value = "NONE" },
}
LogOptions.description = mwse.mcm.i18n("Log options description")

---@class LogOptions.new.data
---@field logger mwseLogger|string

---@param data LogOptions.new.data
---@return mwseMCMLogOptions
function LogOptions:new(data)
	local logger = data.logger
	if not logger then
		error("No logger provided!", 2)
	end
	if type(logger) == "string" then
		--- @cast logger string
		local result = Logger.getLogger(logger)
		if not result then
			error(string.format("Couldn't get any logger for \"%s\".", logger), 2)
		end
		data.logger = result
	else
		assert(type(logger.setLogLevel) == "function", "Invalid logger passed!")
	end
	--- @diagnostic disable-next-line: param-type-mismatch, return-type-mismatch
	return Parent.new(self, data)
end

function LogOptions:updateLogLevel()
	self.logger:setLogLevel(self.variable.value)
end

function LogOptions:update()
	self:updateLogLevel()
	Parent.update(self)
end

return LogOptions
