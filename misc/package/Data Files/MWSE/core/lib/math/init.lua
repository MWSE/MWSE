--- @diagnostic disable: duplicate-set-field

-- Seed random number generator.
math.randomseed(os.time())

--- @param v0 number
--- @param v1 number
--- @param t number
--- @return number result
function math.lerp(v0, v1, t)
	return (1 - t) * v0 + t * v1;
end

--- @param value number
--- @param min number
--- @param max number
--- @return number result
function math.clamp(value, min, max)
	if (min > max) then
		min, max = max, min
	end
	return math.max(min, math.min(max, value))
end

--- @param value number
--- @param lowIn number
--- @param highIn number
--- @param lowOut number
--- @param highOut number
--- @return number result
function math.remap(value, lowIn, highIn, lowOut, highOut)
	return lowOut + (value - lowIn) * (highOut - lowOut) / (highIn - lowIn)
end

--- @param value number
--- @param digits? number
--- @return number result
function math.round(value, digits)
	local mult = 10 ^ (digits or 0)
	return math.floor(value * mult + 0.5) / mult
end

--- @param a number
--- @param b number
--- @param absoluteTolerance? number
--- @param relativeTolerance? number
--- @return boolean result
function math.isclose(a, b, absoluteTolerance, relativeTolerance)
	absoluteTolerance = absoluteTolerance or math.epsilon
	relativeTolerance = relativeTolerance or 1e-9
	return math.abs(a-b) <= math.max(relativeTolerance * math.max(math.abs(a), math.abs(b)), absoluteTolerance)
end

--- @param value number
--- @return integer result
function math.nextPowerOfTwo(value)
	return math.pow(2, math.ceil(math.log(value) / math.log(2)))
end

-------------------------------------------------
-- Extend base API: math.ease
-------------------------------------------------

math.ease = require("math.ease")
