local this = {}

local invalidObjectMessage = "safeObjectHandle: This object has been invalidated."

local function isValidObject(object)
	return object ~= nil and isSafe(object)
end

local function getValidObject(handle)
	local object = rawget(handle, "_object")
	assert(isValidObject(object), invalidObjectMessage)
	return object
end

-- Create a new safe object handle.
function this.new(object)
	if (object == nil) then
		return nil
	end

	return setmetatable({ _object = object }, this)
end

function this:valid()
	return isValidObject(rawget(self, "_object"))
end

function this:getObject()
	if (not self:valid()) then
		return nil
	end

	return rawget(self, "_object")
end

-- Allow this handle to index into the object, with validity checking.
function this:__index(key)
	local handleValue = rawget(this, key)
	if (handleValue ~= nil) then
		return handleValue
	end

	local object = getValidObject(self)
	local result = object[key]
	if (type(result) == "function") then
		return function(_, ...)
			local currentObject = getValidObject(self)
			return currentObject[key](currentObject, ...)
		end
	end

	return result
end

-- Allow this handle to set values on the object, with validity checking.
function this:__newindex(key, value)
	local object = getValidObject(self)
	object[key] = value
end

-- Don't compare against this table. Compare against the object instead.
function this:__eq(value)
	local object = rawget(self, "_object")
	if (not isValidObject(object)) then
		return value == nil
	end

	return object == value
end

-- Add tostring() support.
function this:__tostring()
	local object = rawget(self, "_object")
	if (not isValidObject(object)) then
		return "nil"
	end

	return tostring(object)
end

-- Add json support.
function this:__tojson()
	local object = rawget(self, "_object")
	if (not isValidObject(object)) then
		return "null"
	end

	return object:__tojson()
end

return this
