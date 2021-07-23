local this = {}

-- A map of tes3objects to their unsafe handle.
local handles = {}

-- Remove handles on object invalidated.
local function onObjectInvalidated(e)
	local handle = handles[e.object]
	if (handle) then
		rawset(handle, "_object", nil)
		handles[e.object] = nil
	end
end
event.register("objectInvalidated", onObjectInvalidated)

-- Create a new unsafe reference.
function this.new(object)
	if (object == nil) then
		return nil
	end

	-- Return a previous handle if applicable.
	if (handles[object]) then
		return handles[object]
	end

	local raw = { _object = object }
	function raw:valid() return rawget(self, "_object") ~= nil end
	function raw:getObject() return rawget(self, "_object") end

	-- Create a metatable redirect.
	local handle = setmetatable(raw, this)
	handles[object] = handle

    return handle
end

-- Allow this handle to index into the reference, with some checking.
function this:__index(key)
	local valueRaw = rawget(self, key)
	if (valueRaw) then
		return valueRaw
	end

	local reference = assert(rawget(self, "_object"), "unsafe_object: This object has been invalidated.")
	local result = reference[key]
	if (type(result) == "function") then
		return function(self, ...)
			return result(reference, ...)
		end
	end
	return result
end

-- Allow this handle to set values of the reference, with some checking.
function this:__newindex(key, value)
	local reference = assert(rawget(self, "_object"), "unsafe_object: This object has been invalidated.")
	reference[key] = value
end

-- Don't compare against this table. Compare against the reference instead.
function this:__eq(value)
	return rawget(self, "_object") == value
end

-- Add tostring() support.
function this:__tostring()
	return tostring(rawget(self, "_object"))
end

-- Add json support.
function this:__tojson()
	local reference = rawget(self, "_object")
	if (reference) then
		return reference:__tojson()
	else
		return "null"
	end
	return rawget(self, "_object"):__tojson()
end

return this