local Base = {}

function Base:new(data)
	local o = data or {}
	-- Bind the new object to the Base class
	setmetatable(o, self)
	self.__index = self
	return o
end

return Base
