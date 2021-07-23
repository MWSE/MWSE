--[[
	MCMData structure:
	variable = {
		id							 
		path			--OPTIONAL: path from tes3.player.data if it's nested in tables,
		class
		defaultSetting
	}
]]--

local Parent = require("mcm.variables.Variable")
--Class object
local PlayerDataVar =  Parent:new()
PlayerDataVar.inGameOnly = true


function PlayerDataVar:get()
	if tes3.player then
		local current = tes3.player.data
		local previous
		for v in string.gmatch(self.path, "[^%.]+") do
			if not current[v] then
				current[v] = {}
			end
			current = current[v]
		end
		if current[self.id] == nil then
			current[self.id] = self.defaultSetting
		end
		return current[self.id]
	else
		return self.defaultSetting
	end

	return nil
end


function PlayerDataVar:set(newValue)
	if tes3.player then
		local converter = self.converter
		if (converter) then
			newValue = converter(newValue)
		end

		local table = tes3.player.data
		for v in string.gmatch(self.path, "[^%.]+") do
			table = table[v]
		end
		
		table[self.id] = newValue
	end
end
 
return PlayerDataVar