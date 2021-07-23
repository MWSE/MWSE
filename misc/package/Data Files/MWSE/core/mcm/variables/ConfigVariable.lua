local Parent = require("mcm.variables.Variable")

local ConfigVariable = Parent:new()
ConfigVariable.inGameOnly = false


function ConfigVariable:get()
	local config = mwse.loadConfig(self.path)
	
	--initialise config file if doesn't exist
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


function ConfigVariable:set(newValue)
	local converter = self.converter
	if (converter) then
		newValue = converter(newValue)
	end

	local config = mwse.loadConfig(self.path)
	config[self.id] = newValue
	mwse.saveConfig(self.path, config)

end


return ConfigVariable