local this = {}


--- @param n number
function this.inUnitRange(n)
	if n < 0 or n > 1 then
		return false
	end
	return true
end

--- @param pixel mwseColorTable|ffiImagePixel
function this.pixel(pixel)
	if not pixel.r
	or not this.inUnitRange(pixel.r)
	or not pixel.g
	or not this.inUnitRange(pixel.g)
	or not pixel.b
	or not this.inUnitRange(pixel.b) then
		return false
	end
	return true
end

local EPSILON = 1e-5

--- Returns a copy with all the channels clamped to unit range.
--- @param color mwseColorTable|ffiImagePixel
--- @return mwseColorTable
function this.clampColor(color)
	return {
		r = math.clamp(color.r, EPSILON, 1),
		g = math.clamp(color.g, EPSILON, 1),
		b = math.clamp(color.b, EPSILON, 1),
	}
end


return this
