local Parent = require("mcm.variables.Variable")

local TableVariable = Parent:new()

function TableVariable:get()
	if self.table[self.id] == nil then 
		self.table[self.id] = self.defaultSetting
	end
	return self.table[self.id]
end

function TableVariable:set(newVal)
	local converter = self.converter
	if (converter) then
		newVal = converter(newVal)
	end
	
	self.table[self.id] = newVal
end

return TableVariable 