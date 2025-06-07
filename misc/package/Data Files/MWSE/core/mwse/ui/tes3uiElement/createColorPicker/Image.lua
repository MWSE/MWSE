local ffi = require("ffi")

local Base = require("mwse.ui.tes3uiElement.createColorPicker.Base")
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")

local niPixelData_BYTES_PER_PIXEL = 4

--- An image helper class.
--- @class Image
--- @field width integer
--- @field height integer
--- @field data ffiImagePixel[]
--- @field alphas number[]
local Image = Base:new()

--- @class Image.new.data
--- @field width integer **Remember, to use it as an engine texture use power of 2 dimensions.**
--- @field height integer **Remember, to use it as an engine texture use power of 2 dimensions.**
--- @field data? ffiImagePixel[]
--- @field alphas? number[]

--- @alias ffiImagePixelInit number[]|mwseColorTable

--- @class HSV
--- @field h number Hue in range [0, 360)
--- @field s number Saturation in range [0, 1]
--- @field v number Value/brightness in range [0, 1]

--- @alias ffiHSVInit number[]|HSV

-- Defined in colorUtils\init.lua
local ffiPixel = ffi.typeof("RGB") --[[@as fun(init: ffiImagePixelInit?): ffiImagePixel]]
local ffiHSV = ffi.typeof("HSV") --[[@as fun(init: ffiHSVInit?): ffiHSV]]
-- Creates a 0-indexed array of ffiImagePixel
local ffiPixelArray = ffi.typeof("RGB[?]") --[[@as fun(nelem: integer, init: ffiImagePixelInit[]?): ffiImagePixel[]=]]
local ffiDoubleArray = ffi.typeof("double[?]") --[[@as fun(nelem: integer, init: number[])]]

--- @param data Image.new.data
--- @return Image
function Image:new(data)
	local t = Base:new(data)
	setmetatable(t, self)

	local size = t.width * t.height
	if t.data == nil then
		t.data = ffiPixelArray(size + 1)
	-- Convert given table initializer into a C-array.
	elseif not ffi.istype("RGB[?]", t.data) then
		t.data = ffiPixelArray(size + 1, t.data)
	end

	if t.alphas == nil then
		t.alphas = ffiDoubleArray(size + 1, { 1.0 })
		-- Initialize the alphas to 1.0.
		for y = 0, t.height - 1 do
			local offset = y * t.width
			for x = 1, t.width do
				t.alphas[offset + x] = 1.0
			end
		end
	-- Convert given table initializer into a C-array.
	elseif not ffi.istype("double[?]", t.alphas) then
		t.alphas = ffiDoubleArray(size + 1, t.alphas)
	end

	self.__index = self
	return t
end

--- @protected
--- @param y integer
function Image:getOffset(y)
	return y * self.width
end

--- Returns a copy of a pixel with given coordinates.
--- @param x number Horizontal coordinate
--- @param y number Vertical coordinate
function Image:getPixel(x, y)
	-- Slider at the bottom of main picker has higher precision than color picker width.
	-- So, we could feed in non integer coordinates. Let's make sure these are integers.
	x = math.floor(x)
	y = math.floor(y)
	local offset = self:getOffset(y - 1)
	return self.data[offset + x]
end

--- @param x integer Horizontal coordinate
--- @param y integer Vertical coordinate
--- @param color ffiImagePixel
function Image:setPixel(x, y, color)
	local offset = self:getOffset(y - 1)
	self.data[offset + x] = color
end

--- Modifies the Image in place.
--- @param color ffiImagePixel
--- @param alpha number?
function Image:fillColor(color, alpha)
	alpha = alpha or 1

	for y = 0, self.height - 1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			self.data[offset + x] = color
			self.alphas[offset + x] = alpha
		end
	end
end

--- Modifies the Image in place.
--- @param rowIndex integer
--- @param color ffiImagePixel
--- @param alpha number?
function Image:fillRow(rowIndex, color, alpha)
	alpha = alpha or 1

	local offset = self:getOffset(rowIndex - 1)
	for x = 1, self.width do
		self.data[offset + x] = color
		self.alphas[offset + x] = alpha
	end
end

--- Modifies the Image in place. Fills the image into a vertical hue bar. HSV value at the top is:
--- `{ H = 0, s = saturation, v = value }`, at the bottom `{ H = 360, s = saturation, v = value }`
--- @param saturation number? *Default: 0.55*. In range [0, 1].
--- @param value number? *Default: 0.9*. In range [0, 1].
function Image:verticalHueBar(saturation, value)
	local hsv = ffiHSV({ 0, saturation or 0.55, value or 0.9 })

	-- Lower level fill method will account for the 0-indexing of the underlying data array.
	for y = 1, self.height do
		local t = y / self.height

		-- We lerp to 359.9999 since HSV { 360, 1.0, 1.0 } results in { r = 0, g = 0, b = 0 }
		-- at the bottom of the hue picker which is a undesirable.
		hsv.h = math.lerp(0, 359.9999, t)
		local rowColor = colorUtils.HSVtosRGB(hsv)
		self:fillRow(y, rowColor)
	end
end

--- Modifies the Image in place. Fills the image into a vertical saturation bar. HSV value at the top is:
--- `{ H = hue, s = 1.0, v = value }`, at the bottom `{ H = hue, s = 0.0, v = value }`
--- @param hue number In range [0, 360)
--- @param value number In range [0, 1]
function Image:verticalSaturationBar(hue, value)
	colorUtils.generateSaturationPicker(hue, value, self)
end

--- Generates main picker image for given Hue.
--- @param hue number Hue in range [0, 360)
function Image:mainPicker(hue)
	colorUtils.generateMainPicker(hue, self)
end

--- Modifies the Image in place.
--- @param topColor mwseColorATable
--- @param bottomColor mwseColorATable
function Image:verticalGradient(topColor, bottomColor)
	topColor.a = topColor.a or 1
	bottomColor.a = bottomColor.a or 1

	-- Lower level fillRow will account for the 0-based indexing of the underlying data array.
	for y = 1, self.height do
		local t = y / self.height

		local color = ffiPixel({
			math.lerp(topColor.r, bottomColor.r, t),
			math.lerp(topColor.g, bottomColor.g, t),
			math.lerp(topColor.b, bottomColor.b, t),
		})
		self:fillRow(
			y, color,
			math.lerp(topColor.a, bottomColor.a, t)
		)
	end
end

--- Creates a checkered pattern.
--- @param size integer? *Default: 16*. The size of single square in pixels.
--- @param lightGray mwseColorTable? *Default: { r = 0.7, g = 0.7, b = 0.7 }*
--- @param darkGray mwseColorTable? *Default: { r = 0.5, g = 0.5, b = 0.5 }*
function Image:toCheckerboard(size, lightGray, darkGray)
	size = size or 16
	local doubleSize = 2 * size
	local light = ffiPixel({ 0.7, 0.7, 0.7 })
	if lightGray then
		-- LuaJIT will do automatic type conversion for us.
		light = lightGray --[[@as ffiImagePixel]]
	end
	local dark = ffiPixel({ 0.5, 0.5, 0.5 })
	if darkGray then
		-- LuaJIT will do automatic type conversion for us.
		dark = darkGray --[[@as ffiImagePixel]]
	end

	for y = 0, self.height - 1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			if ((y % doubleSize) < size) then
				-- -1 is compensation for indexing starting at 1.
				if (((x - 1) % doubleSize) < size) then
					self.data[offset + x] = light
				else
					self.data[offset + x] = dark
				end
			else
				if (((x - 1) % doubleSize) < size) then
					self.data[offset + x] = dark
				else
					self.data[offset + x] = light
				end
			end

		end
	end
end

function Image:copy()
	local size = self.height * self.width
	local data = ffiPixelArray(size + 1)
	local pixelBufferSize = ffi.sizeof("RGB[?]", size)
	ffi.copy(data, self.data, pixelBufferSize)

	local alphas = ffiDoubleArray(size + 1, { 1.0 })
	local alphasSize = ffi.sizeof("double[?]", size)
	ffi.copy(alphas, self.alphas, alphasSize)


	local new = Image:new({
		height = self.height,
		width = self.width,
		data = data,
		alphas = alphas,
	})

	return new
end

-- https://en.wikipedia.org/wiki/Alpha_compositing#Description
local function colorBlend(cA, cB, alphaA, alphaB, inverse)
	local alphaO = alphaA + alphaB * inverse
	return (cA * alphaA + cB * alphaB * inverse) / alphaO
end

--- Returns a copy with the result of blending between `self` and given `image`.
--- @param background Image
--- @param copy boolean? If true, won't modify `self`, but will return the result of blend operation in a Image copy.
function Image:blend(background, copy)
	local sameWidth = self.width == background.width
	local sameHeight = self.height == background.height
	assert(sameWidth, "Images must be of same width.")
	assert(sameHeight, "Images must be of same height.")

	local data = self.data
	local alphas = self.alphas
	--- @type Image|nil
	local new
	if copy then
		new = self:copy()
		data = new.data
		alphas = new.alphas
	end

	for y = 0, self.height - 1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			local i = offset + x
			local pixel1 = data[i]
			local pixel2 = background.data[i]
			local alpha1 = alphas[i]
			local alpha2 = background.alphas[i]

			local inverse = 1 - alpha1
			pixel1.r = colorBlend(pixel1.r, pixel2.r, alpha1, alpha2, inverse)
			pixel1.g = colorBlend(pixel1.g, pixel2.g, alpha1, alpha2, inverse)
			pixel1.b = colorBlend(pixel1.b, pixel2.b, alpha1, alpha2, inverse)
			alpha1 = alpha1 + alpha2 * inverse
			data[i], alphas[i] = pixel1, alpha1
		end
	end
	if copy then
		return new
	end
end


--- For feeding data straight to `niPixelData:setPixelsFloat`.
function Image:toPixelBufferFloat()
	local size = self.width * self.height
	local buffer = table.new(size * niPixelData_BYTES_PER_PIXEL, 0)
	local stride = 0

	for y = 0, self.height - 1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			local pixel = self.data[offset + x]
			buffer[stride + 1] = pixel.r
			buffer[stride + 2] = pixel.g
			buffer[stride + 3] = pixel.b
			buffer[stride + 4] = self.alphas[offset + x]
			stride = stride + 4
		end
	end

	return buffer
end

--- For feeding data straight to `niPixelData:setPixelsByte`.
function Image:toPixelBufferByte()
	local size = self.width * self.height
	local buffer = table.new(size * niPixelData_BYTES_PER_PIXEL, 0)
	local stride = 0

	for y = 0, self.height - 1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			local pixel = self.data[offset + x]
			buffer[stride + 1] = pixel.r * 255
			buffer[stride + 2] = pixel.g * 255
			buffer[stride + 3] = pixel.b * 255
			buffer[stride + 4] = self.alphas[offset + x] * 255
			stride = stride + 4
		end
	end

	return buffer
end

--- @param file file*
--- @param ... integer
local function writeBytes(file, ...)
	file:write(string.char(...))
end

--- @param filename string
function Image:saveBMP(filename)
	local file = io.open(filename, "wb")
	if not file then
		error(string.format("Can't open %q. Traceback:%s", filename, debug.traceback()))
	end
	do -- BitmapFileHeader
		-- char[] bfType = "BM"
		file:write("BM")
		-- u32 bfSize = 60054
		writeBytes(file, 0x96, 0xEA, 0x00, 0x00)
		-- u16 bfReserved1
		writeBytes(file, 0x00, 0x00)
		-- u16 bf Reserved2
		writeBytes(file, 0x00, 0x00)
		-- u32 brOffBits = 54
		writeBytes(file, 0x036, 0x00, 0x00, 0x00)
	end
	do -- BitmapInfoHeaderV1
		-- u32 biSize = 40
		writeBytes(file, 0x28, 0x00, 0x00, 0x00)
		-- s32 biWidth = 200
		writeBytes(file, 0xC8, 0x00, 0x00, 0x00)
		-- s32 biHeight = 100
		writeBytes(file, 0x64, 0x00, 0x00, 0x00)
		-- u16 biPlanes = 1
		writeBytes(file, 0x01, 0x00)
		-- u16 biBitCount = 24
		writeBytes(file, 0x18, 0x00)
		-- u32 biCompression = 0 // BI_RGB
		writeBytes(file, 0x00, 0x00, 0x00, 0x00)
		-- u32 biSizeImage = 60000
		writeBytes(file, 0x60, 0xEA, 0x00, 0x00)
		-- s32 biXPelsPerMeter = 0
		writeBytes(file, 0x00, 0x00, 0x00, 0x00)
		-- s32 biYPelsPerMeter = 0
		writeBytes(file, 0x00, 0x00, 0x00, 0x00)
		-- u32 biClrUsed = 0
		writeBytes(file, 0x00, 0x00, 0x00, 0x00)
		-- u32 biClrImportant = 0
		writeBytes(file, 0x00, 0x00, 0x00, 0x00)
	end

	for y = self.height - 1, 0, -1 do
		local offset = self:getOffset(y)
		for x = 1, self.width do
			local pixel = self.data[offset + x]
			local alpha = self.alphas[offset + x]
			local b = math.round(255 * pixel.b)
			local g = math.round(255 * pixel.g)
			local r = math.round(255 * pixel.r)
			writeBytes(file, b, g, r)
		end
	end
	file:close()
end

return Image
