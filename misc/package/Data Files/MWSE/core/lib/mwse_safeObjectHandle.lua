local SafeHandle = {}

-- used by instances of the SafeHandle class
local handleMetatable = {}

-- A map of tes3objects to their unsafe handle.
local handles = {} ---@type table<tes3object, table> stores one safe handle per object

--- Remove handles on object invalidated.
--- @param e objectInvalidatedEventData
local function onObjectInvalidated(e)
	local handle = handles[e.object]
	if (handle) then
		rawset(handle, "_object", nil)
		handles[e.object] = nil
	end
end
event.register("objectInvalidated", onObjectInvalidated)

-- Create a new unsafe reference.
function SafeHandle.new(object)
	if (object == nil) then
		return
	end

	-- Create a new handle if necessary.
	if handles[object] == nil then 
		handles[object] = setmetatable({ _object = object }, handleMetatable)
	end

	-- Return the handle for this object.
	return handles[object]
end

function SafeHandle:isValid()
	local object = rawget(self, "_object")
	return object ~= nil and not object.deleted
end

-- Backwards compatibility
SafeHandle.valid = SafeHandle.isValid

function SafeHandle:getObject()
	return rawget(self, "_object")
end

-- Fill out the metatable.

function handleMetatable.__index(handle, key)
	-- Try to look things up in the `SafeHandle` class.
	local val = rawget(SafeHandle, key)
	if val ~= nil then return val end

	-- Try to look things up in the stored object, if it still exists.
	local storedObject = SafeHandle.getObject(handle)
	assert(storedObject, "unsafe_object: This object has been invalidated.")

	val = storedObject[key]

	-- If `val` is a method, send `storedObject` in as the first parameter, rather than `hanadle`
	if (type(val) == "function") then
		return function(self, ...)
			return val(storedObject, ...)
		end
	end
	return val
end

-- Allow this handle to set values of the reference, with some checking.
function handleMetatable.__newindex(handle, key, value)
	local object = SafeHandle.getObject(handle)
	assert(object, "Unsafe Object: This object has been invalidated.")
	object[key] = value
end


-- Add tostring() support.
function handleMetatable.__tostring(handle)
	return tostring(handle:getObject())
end

-- Add json support.
function handleMetatable.__tojson(handle)
	local object = handle:getObject()
	-- try to call the `__tojson` metamethod, if it exists
	if object == nil then
		return "null"
	end

	local objectMetatable = getmetatable(object)
	
	-- Check if it has a `__tojson` metamethod.
	if objectMetatable and objectMetatable.__tojson then
		return objectMetatable.__tojson(object)
	end

	-- Check if it inherited a `__tojson` metamethod.
	if object.__tojson then 
		return object:__tojson()
	end

	-- Otherwise, use `tostring`.
	return tostring(object)
end

return SafeHandle