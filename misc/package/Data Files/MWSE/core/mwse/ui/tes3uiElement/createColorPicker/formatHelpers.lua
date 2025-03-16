local ffi = require("ffi")

-- This is the first file that declares `ffiPixel = ffi.typeof("RGB")`.
-- We need these declarations first:
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")
-- Defined in colorUtils\init.lua
local ffiPixel = ffi.typeof("RGB") --[[@as fun(init: ffiImagePixelInit?): ffiImagePixel]]

local this = {}

--- @param p ffiImagePixel|mwseColorTable
function this.pixel(p)
	return string.format("{ r = %.3f, g = %.3f, b = %.3f }", p.r, p.g, p.b)
end

--- @param c ffiHSV
function this.hsv(c)
	return string.format("{ h = %.3f, s = %.3f, v = %.3f }", c.h, c.s, c.v)
end

--- @param image Image
function this.imageData(image)
	local r = {}
	for y = 0, image.height - 1 do
		local offset = y * image.width
		for x = 1, image.width do
			table.insert(r, this.pixel(image.data[offset + x]))
		end
	end
	return "{" .. table.concat(r, "\n\t") .. "}"
end

--- Formats given RGB(A) pixel into an HTML hex code.
--- @param pixel mwseColorTable|mwseColorATable|ffiImagePixel
function this.pixelToHex(pixel)
	if pixel.a then
		return string.format("%02X%02X%02X%02X", pixel.a * 255, pixel.r * 255, pixel.g * 255, pixel.b * 255)
	else
		return string.format("%02X%02X%02X", pixel.r * 255, pixel.g * 255, pixel.b * 255)
	end
end

local ARGB_HEX_CODE_LEN = 8

--- Parses given HTML hex code into an RGB(A) pixel.
--- @param str string
--- @return ffiImagePixel, number?
function this.hexToPixel(str)
	local n = tonumber(str, 16)
	local c = ffiPixel({
		r = bit.rshift(bit.band(n, 0x00FF0000), 16) / 255,
		g = bit.rshift(bit.band(n, 0x0000FF00),  8) / 255,
		b = bit.band(n, 0x000000FF) / 255,
	})
	if string.len(str) == ARGB_HEX_CODE_LEN then
		return c, bit.rshift(n, 24) / 255
	end
	return c
end

return this
