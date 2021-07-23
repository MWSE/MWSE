--Parent class
local Parent = require("mcm.variables.Variable")
--Class object
local GlobalVar = Parent:new()
GlobalVar.inGameOnly = true


function GlobalVar:get()
	return tes3.findGlobal(self.id).value
end

function GlobalVar:set(newValue)
	local converter = self.converter
	if (converter) then
		newValue = converter(newValue)
	end
	
	tes3.findGlobal(self.id).value = newValue
end

return GlobalVar 