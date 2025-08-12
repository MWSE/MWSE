local ffi = require("ffi")

local Base = require("mwse.ui.tes3uiElement.createColorPicker.Base")
local CONSTANTS = require("mwse.ui.tes3uiElement.createColorPicker.constants")
local format = require("mwse.ui.tes3uiElement.createColorPicker.formatHelpers")
local Image = require("mwse.ui.tes3uiElement.createColorPicker.Image")
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")
local UIID = require("mwse.ui.tes3uiElement.createColorPicker.uiid")

-- Defined in colorUtils\init.lua
local ffiPixel = ffi.typeof("RGB") --[[@as fun(init: ffiImagePixelInit?): ffiImagePixel]]


--- @class tes3uiColorPicker
local ColorPicker = Base:new()

--- @class tes3uiColorPicker.new.data
--- @field mainWidth integer Width of the main picker. **Remember, to use it as an engine texture use power of 2 dimensions.**
--- @field height integer Height of all the picker widgets. **Remember, to use it as an engine texture use power of 2 dimensions.**
--- @field hueWidth integer Width of hue and alpha pickers. **Remember, to use it as an engine texture use power of 2 dimensions.**
--- @field initialColor mwseColorTable
--- @field initialAlpha number? *Default*: 1.0

--- @param data tes3uiColorPicker.new.data
--- @return tes3uiColorPicker
function ColorPicker:new(data)
	local t = Base:new(data)
	setmetatable(t, self)

	t.initialAlpha = table.get(data, "initialAlpha", 1.0)
	t.currentColor = ffiPixel({ data.initialColor.r, data.initialColor.g, data.initialColor.b })
	t.currentAlpha = t.initialAlpha

	-- Make sure texture dimensions are powers of 2.
	-- We leave the passed UI sizes in picker member variables and use local variables here.
	local height = math.nextPowerOfTwo(data.height)
	local mainWidth = math.nextPowerOfTwo(data.mainWidth)
	local hueWidth = math.nextPowerOfTwo(data.hueWidth)

	t.mainImage = Image:new({
		width = mainWidth,
		height = height,
	})
	local hsv = colorUtils.sRGBtoHSV(t.currentColor)
	t.mainImage:mainPicker(hsv.h)

	t.hueBar = Image:new({
		width = hueWidth,
		height = height,
	})
	t.hueBar:verticalHueBar()

	t.saturationBar = Image:new({
		width = hueWidth,
		height = height,
	})
	t.saturationBar:verticalSaturationBar(hsv.h, hsv.v)

	t.alphaCheckerboard = Image:new({
		width = hueWidth,
		height = height,
	})
	t.alphaCheckerboard:toCheckerboard()

	t.alphaBar = Image:new({
		width = hueWidth,
		height = height,
	})
	t.alphaBar:verticalGradient(
		{ r = 0.25, g = 0.25, b = 0.25, a = 1.0 },
		{ r = 1.0,  g = 1.0,  b = 1.0,  a = 0.0 }
	)
	t.alphaBar = t.alphaBar:blend(t.alphaCheckerboard, true) --[[@as Image]]

	-- Create textures for this Color Picker
	t.textures = {
		main = niPixelData.new(mainWidth, height):createSourceTexture(),
		hue = niPixelData.new(hueWidth, height):createSourceTexture(),
		alpha = niPixelData.new(hueWidth, height):createSourceTexture(),
		saturation = niPixelData.new(hueWidth, height):createSourceTexture(),
	}
	for _, texture in pairs(t.textures) do
		texture.isStatic = false
	end

	self.__index = self
	return t
end

function ColorPicker:getAlpha()
	return self.currentAlpha
end

--- @return mwseColorTable
function ColorPicker:getColor()
	local c = self.currentColor
	return { r = c.r, g = c.g, b = c.b }
end

function ColorPicker:getColorAlpha()
	return self:getColor(), self:getAlpha()
end

--- @return mwseColorATable
function ColorPicker:getRGBA()
	local c = self.currentColor
	return { r = c.r, g = c.g, b = c.b, a = self.currentAlpha }
end

--- @return number[] arrayRGB
function ColorPicker:getColorArray()
	local c = self.currentColor
	return { c.r, c.g, c.b }
end

--- @alias IndicatorID
---| "main"
---| "hue"
---| "alpha"
---| "saturation"
---| "slider"

--- @param parent tes3uiElement
local function getIndicators(parent)
	--- @type table<IndicatorID, tes3uiElement>
	local indicators = {}
	for _, UIID in pairs(UIID.indicator) do
		local indicator = parent:findChild(UIID)
		-- Not every Color Picker will have alpha indicator or saturation slider.
		if indicator then
			local id = indicator:getLuaData("mwse:indicatorID")
			indicators[id] = indicator
		end
	end
	return indicators
end

--- @private
--- @param hsv ffiHSV
--- @param alpha number?
function ColorPicker:updateIndicatorPositions(hsv, alpha)
	local indicators = getIndicators(self.element)

	-- Update main picker's indicator
	indicators.main.absolutePosAlignX = hsv.s
	indicators.main.absolutePosAlignY = 1 - hsv.v
	-- Update main picker's slider
	local slider = indicators.slider
	if slider then
		slider.widget.current = hsv.s * CONSTANTS.SLIDER_SCALE
	end

	if indicators.saturation then
		indicators.saturation.absolutePosAlignY = 1 - hsv.s
	end

	-- Update hue indicator
	indicators.hue.absolutePosAlignY = hsv.h / 360

	-- Update alpha indicator
	if indicators.alpha then
		indicators.alpha.absolutePosAlignY = 1 - alpha
	end
end

--- @private
--- @param newColor ffiImagePixel
--- @param alpha number
function ColorPicker:updatePreview(newColor, alpha)
	-- Not every color picker has preview widgets.
	local previewsContainer = self.element:findChild(UIID.preview.topContainer)
	if not previewsContainer then return end
	local previewElement = previewsContainer:findChild(UIID.preview.current)
	if not previewElement then return end
	local preview = previewElement.widget --[[@as tes3uiColorPreview]]
	preview:setColor(newColor, alpha)
end

--- @private
--- @param newColor ffiImagePixel|mwseColorTable|mwseColorATable
--- @param alpha number
function ColorPicker:updateValueInput(newColor, alpha)
	-- Not all color pickers have value input.
	local input = self.element:findChild(UIID.textInput)
	if not input then return end

	-- Make sure we don't get NaNs in color text inputs. We clamp alpha here.
	if alpha then
		alpha = math.clamp(alpha, 0.0000001, 1.0)
	end

	local newText = ""
	if input:getLuaData("mwse:hasAlpha") then
		newText = format.pixelToHex({
			r = newColor.r,
			g = newColor.g,
			b = newColor.b,
			a = alpha,
		})
	else
		newText = format.pixelToHex({
			r = newColor.r,
			g = newColor.g,
			b = newColor.b,
		})
	end
	input.text = newText
end

--- @private
--- @param newColor ffiImagePixel
--- @param alpha number
function ColorPicker:setColor(newColor, alpha)
	self.currentColor = newColor
	self.currentAlpha = alpha
end

--- Usually used when after a color was picked in the main, saturation or alpha pickers.
--- @param newColor mwseColorTable|ffiImagePixel
--- @param alpha number
function ColorPicker:colorSelected(newColor, alpha)
	-- Make sure we don't create reference to the picked pixel.
	newColor = ffiPixel({ newColor.r, newColor.g, newColor.b })
	alpha = alpha or 1.0
	self:setColor(newColor, alpha)
	self:updatePreview(newColor, alpha)
	self:updateValueInput(newColor, alpha)

	local hsv = colorUtils.sRGBtoHSV(newColor)
	self:updateIndicatorPositions(hsv, alpha)

	local saturationPicker = self.element:findChild(UIID.saturationPicker)
	if saturationPicker then
		self.saturationBar:verticalSaturationBar(hsv.h, hsv.v)
		saturationPicker.texture.pixelData:setPixelsFloat(self.saturationBar:toPixelBufferFloat())
	end

	self.element:getTopLevelMenu():updateLayout()
end

--- Updates all the elements of color picker. It's more expensive than `colorSelected` since it will
--- regenerate the image for main picker.
--- @param newColor mwseColorTable|ffiImagePixel
--- @param alpha number
function ColorPicker:hueChanged(newColor, alpha)
	-- Make sure we don't create reference to the picked pixel.
	newColor = ffiPixel({ newColor.r, newColor.g, newColor.b })
	self:colorSelected(newColor, alpha)
	-- Now, also need to regenerate the image for the main picker since the Hue changed.
	local hsv = colorUtils.sRGBtoHSV(newColor)
	self.mainImage:mainPicker(hsv.h)
	local mainPicker = self.element:findChild(UIID.mainPicker)
	mainPicker.texture.pixelData:setPixelsFloat(self.mainImage:toPixelBufferFloat())
end

return ColorPicker
