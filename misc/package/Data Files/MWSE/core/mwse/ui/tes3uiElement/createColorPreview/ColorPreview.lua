local ffi = require("ffi")

local Base = require("mwse.ui.tes3uiElement.createColorPicker.Base")
local Image = require("mwse.ui.tes3uiElement.createColorPicker.Image")
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")
local UIID = require("mwse.ui.tes3uiElement.createColorPreview.uiid")

-- Defined in colorUtils\init.lua
local ffiPixel = ffi.typeof("RGB") --[[@as fun(init: ffiImagePixelInit?): ffiImagePixel]]

--- @class tes3uiColorPreview
local ColorPreview = Base:new()

--- @param data tes3uiElement.createColorPreview.params
--- @return tes3uiColorPreview
function ColorPreview:new(data)
	local t = Base:new(data)
	setmetatable(t, self)

	t.color = ffiPixel({ data.color.r, data.color.g, data.color.b })
	t.alpha = data.alpha or 1.0

	t.checkerboard = Image:new({
		width = data.width,
		height = data.height
	})
	t.checkerboard:toCheckerboard(data.checkerSize, data.lightGray, data.darkGray)
	t.image = Image:new({
		width = data.width,
		height = data.height,
	})
	colorUtils.generatePreviewImage(t.color, t.alpha, t.image, t.checkerboard)

	t.texture = niPixelData.new(data.width, data.height):createSourceTexture()
	t.texture.isStatic = false

	self.__index = self
	return t
end


--- @param newColor mwseColorTable|ffiImagePixel
--- @param alpha number?
function ColorPreview:setColor(newColor, alpha)
	newColor = ffiPixel({ newColor.r, newColor.g, newColor.b })
	alpha = alpha or 1.0
	self.color = newColor
	self.alpha = alpha

	-- self.element is set by the tes3uiElement:makeLuaWidget.
	local container = self.element
	local standardPreview = container:findChild(UIID.rect)
	standardPreview.color = { newColor.r, newColor.g, newColor.b }

	local colorPreview = container:findChild(UIID.image)
	-- Not every preview has checkered preview.
	if not colorPreview then return end
	colorUtils.generatePreviewImage(newColor, alpha, self.image, self.checkerboard)
	colorPreview.texture.pixelData:setPixelsFloat(self.image:toPixelBufferFloat())
end

function ColorPreview:getAlpha()
	return self.alpha
end

--- @return mwseColorTable
function ColorPreview:getColor()
	local c = self.color
	return { r = c.r, g = c.g, b = c.b }
end

function ColorPreview:getColorAlpha()
	return self:getColor(), self:getAlpha()
end

--- @return mwseColorATable
function ColorPreview:getRGBA()
	local c = self.color
	return { r = c.r, g = c.g, b = c.b, a = self.alpha }
end

return ColorPreview
