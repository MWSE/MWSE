--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.variables.Variable")

--- @class mwseMCMConfigVariable
local ConfigVariable = Parent:new()

--- @return unknown value
function ConfigVariable:get()
	local config = mwse.loadConfig(self.path)

	-- initialise config file if doesn't exist
	if not config then
		mwse.log("Config file '%s' does not exist. Creating new file", self.path)
		config = {}
		mwse.saveConfig(self.path, config)
	end
	if self.defaultSetting and config[self.id] == nil then
		mwse.log("ConfigVariable '%s' does not exist. Initialising to %s", self.id, self.defaultSetting)
		config[self.id] = self.defaultSetting
		mwse.saveConfig(self.path, config)
	end
	return config[self.id]
end

--- @param newValue unknown
function ConfigVariable:set(newValue)
	if (self.converter) then
		newValue = self.converter(newValue)
	end

	local config = mwse.loadConfig(self.path)
	config[self.id] = newValue
	mwse.saveConfig(self.path, config)

end

return ConfigVariable
