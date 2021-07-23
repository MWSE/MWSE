--[[
	Base class for variables used by mcm
	To create a subclass, simply create it using Variable:new() 
	and override the get/set functions.
	Within get/set functions, use self.id to get the variable id.

	e.g
		local Subclass = require("mcm.variables.Variable"):new()
		function Subclass:get()
			--DO STUFF
		end
		function Subvlass:set(newValue)
			--DO STUFF
		end
		return Subclass
]]--

local Variable = {}
Variable.componentType = "Variable"
Variable.inGameOnly = false
Variable.restartRequiredMessage = "The game must be restarted before this change will come into effect."
function Variable:new(variable)
	local t = variable or {}
	if type(t) == "string" then
		t = { id = t }
	end
	setmetatable(t, self)
	self.__index = Variable.__index
	self.__newindex = Variable.__newindex
	return t
end


function Variable:get()
	return rawget(self, "value")
end

function Variable:set(newValue)
	local converter = self.converter
	if (converter) then
		newValue = converter(newValue)
	end
	
	rawset(self, "value", newValue)
end
 
function Variable.__index(tbl, key)
	local meta = getmetatable(tbl)
	if key == "value" then
		return tbl:get()
	end
	return meta[key]
end

function Variable:__newindex(key, value)
   meta = getmetatable(self)
   if key == "value" then
		if self.restartRequired then
			local sOk = tes3.findGMST(tes3.gmst.sOK).value
			tes3.messageBox{
				message = self.restartRequiredMessage,
				buttons = { sOk }
			}
		end
		self:set(value)
   else
	  rawset(self, key, value)
   end
end

return Variable