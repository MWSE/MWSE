local validate = require("mwse.ui.tes3uiElement.createColorPicker.validate")

mwse.color = {}

--- Formats given RGB(A) pixel into an HTML hex code.
--- @param rgb mwseColorTable|mwseColorATable|niColor|niColorA|ffiImagePixel
function mwse.color.rgbToHex(rgb)
	assert(validate.pixel(rgb), "Given RGB color channels must be in [0, 1] range.")
	if rgb.a then
		assert(validate.inUnitRange(rgb.a), "Given alpha channel must be in [0, 1] range.")
	end

	if rgb.a then
		return string.format("%02X%02X%02X%02X", rgb.a * 255, rgb.r * 255, rgb.g * 255, rgb.b * 255)
	else
		return string.format("%02X%02X%02X", rgb.r * 255, rgb.g * 255, rgb.b * 255)
	end
end

local ARGB_HEX_CODE_LEN = 8

--- Parses given HTML hex code into an RGB(A) color.
--- @param hex string
--- @return mwseColorTable|mwseColorATable
function mwse.color.hexToRgb(hex)
	assert(type(hex) == "string", "Invalid 'hex' parameter provided. Must be a string.")
	local n = tonumber(hex, 16)
	assert(n ~= nil, string.format("Can't convert given %q hex code to RGB.", hex))
	local rgb = {
		r = bit.rshift(bit.band(n, 0xFF0000), 16) / 255,
		g = bit.rshift(bit.band(n, 0x00FF00),  8) / 255,
		b =            bit.band(n, 0x0000FF)      / 255
	}

	if string.len(hex) == ARGB_HEX_CODE_LEN then
		rgb.a = bit.rshift(n, 24) / 255
	end

	return rgb
end
