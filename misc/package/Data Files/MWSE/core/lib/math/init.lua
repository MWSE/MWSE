
-- Seed random number generator.
math.randomseed(os.time())

function math.lerp(v0, v1, t)
	return (1 - t) * v0 + t * v1;
end

function math.clamp(value, low, high)
	if (low > high) then
		low, high = high, low
	end
	return math.max(low, math.min(high, value))
end

function math.remap(value, lowIn, highIn, lowOut, highOut)
	return lowOut + (value - lowIn) * (highOut - lowOut) / (highIn - lowIn)
end

function math.round(value, digits)
	local mult = 10 ^ (digits or 0)
	return math.floor(value * mult + 0.5) / mult
end

function math.isclose(a, b, absoluteTolerance, relativeTolerance)
	absoluteTolerance = absoluteTolerance or math.epsilon
	relativeTolerance = relativeTolerance or 1e-9
	return math.abs(a-b) <= math.max(relativeTolerance * math.max(math.abs(a), math.abs(b)), absoluteTolerance)
end

function math.nextPowerOfTwo(value)
	return math.pow(2, math.ceil(math.log(value) / math.log(2)))
end

-------------------------------------------------
-- Extend base API: math.ease
-------------------------------------------------

math.ease = require("math.ease")
