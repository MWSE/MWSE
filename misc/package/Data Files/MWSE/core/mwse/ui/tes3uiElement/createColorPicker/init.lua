local ffi = require("ffi")

local ColorPicker = require("mwse.ui.tes3uiElement.createColorPicker.ColorPicker")
local CONSTANTS = require("mwse.ui.tes3uiElement.createColorPicker.constants")
local format = require("mwse.ui.tes3uiElement.createColorPicker.formatHelpers")
local colorUtils = require("mwse.ui.tes3uiElement.createColorPicker.colorUtils")
local UIID = require("mwse.ui.tes3uiElement.createColorPicker.uiid")
local validate = require("mwse.ui.tes3uiElement.createColorPicker.validate")

-- Defined in colorUtils\init.lua
local ffiPixel = ffi.typeof("RGB") --[[@as fun(init: ffiImagePixelInit?): ffiImagePixel]]
local i18n = mwse.loadTranslations("..")

--- Textures used in color picker are always next power of 2 and then scaled down.
--- @param size integer
local function getScaleFactor(size)
	return size / math.nextPowerOfTwo(size)
end

--- @param outerContainer tes3uiElement
--- @param label string
local function createPreviewLabel(outerContainer, label)
	local labelContainer = outerContainer:createBlock({
		id = tes3ui.registerID("ColorPicker_color_preview_label_container")
	})
	labelContainer.flowDirection = tes3.flowDirection.topToBottom
	labelContainer.autoWidth = true
	labelContainer.autoHeight = true
	labelContainer.paddingAllSides = 8
	labelContainer:createLabel({
		id = tes3ui.registerID("ColorPicker_color_preview_" .. label ),
		text = i18n(label)
	})
end

--- @param params tes3uiElement.createColorPicker.params
--- @param parent tes3uiElement
--- @param color ffiImagePixel
--- @param alpha number
--- @param label string
--- @param labelOnTop boolean
--- @param onClickCallback? fun(e: tes3uiEventData)
local function createPreview(params, parent, color, alpha, label, labelOnTop, onClickCallback)
	-- We don't want to create references to color.
	local color = ffiPixel({ color.r, color.g, color.b })
	local outerContainer = parent:createBlock({ id = tes3ui.registerID("ColorPicker_color_preview_outer_container") })
	outerContainer.flowDirection = tes3.flowDirection.topToBottom
	outerContainer.autoWidth = true
	outerContainer.autoHeight = true

	if labelOnTop then
		createPreviewLabel(outerContainer, label)
	end

	local flowDirection = tes3.flowDirection.leftToRight
	-- If we don't show original preview then we make current preview vertical.
	if not params.showOriginal then
		flowDirection = tes3.flowDirection.topToBottom
	end
	local preview = outerContainer:createColorPreview({
		id = UIID.preview[string.lower(label)],
		color = color,
		alpha = alpha,
		width = params.previewWidth,
		height = params.previewHeight,
		hasAlphaPreview = params.alpha,
		flowDirection = flowDirection,
	})
	preview.borderLeft = 8
	preview.borderRight = 8

	if not labelOnTop then
		createPreviewLabel(outerContainer, label)
	end

	if not onClickCallback then return end
	preview:register(tes3.uiEvent.mouseDown, onClickCallback)
end


--- @param parent tes3uiElement
--- @param id string
--- @param absolutePosAlignX number
--- @param absolutePosAlignY number
local function createIndicator(parent, id, absolutePosAlignX, absolutePosAlignY)
	local indicator = parent:createImage({
		id = UIID.indicator[id],
		path = CONSTANTS.INDICATOR_TEXTURE,
	})
	indicator.color = CONSTANTS.INDICATOR_COLOR
	indicator.absolutePosAlignX = absolutePosAlignX
	indicator.absolutePosAlignY = absolutePosAlignY
	indicator:setLuaData("mwse:indicatorID", id)
	return indicator
end

local function isShiftDown()
	return tes3.worldController.inputController:isShiftDown()
end

local SV_EPSILON = 0.01
local HUE_EPSILON = 1e-3

--- @param params tes3uiElement.createColorPicker.params
--- @param picker tes3uiColorPicker
--- @param parent tes3uiElement
local function createPickerBlock(params, picker, parent)
	local initialColor = ffiPixel({ params.initialColor.r, params.initialColor.g, params.initialColor.b })

	local mainRow = parent:createBlock({ id = tes3ui.registerID("ColorPicker_picker_row_container") })
	if params.vertical then
		mainRow.flowDirection = tes3.flowDirection.topToBottom
	else
		mainRow.flowDirection = tes3.flowDirection.leftToRight
	end
	mainRow.autoHeight = true
	mainRow.autoWidth = true
	mainRow.widthProportional = 1.0
	mainRow.paddingAllSides = 4

	local initialHSV = colorUtils.sRGBtoHSV(initialColor)
	local mainIndicatorInitialAbsolutePosAlignX = initialHSV.s
	local mainIndicatorInitialAbsolutePosAlignY = 1 - initialHSV.v
	local hueIndicatorInitialAbsolutePosAlignY = initialHSV.h / 360


	local pickerContainer = mainRow:createBlock({ id = tes3ui.registerID("ColorPicker_main_picker_container") })
	pickerContainer.autoHeight = true
	pickerContainer.autoWidth = true
	pickerContainer.flowDirection = tes3.flowDirection.topToBottom

	local mainPicker = pickerContainer:createRect({
		id = UIID.mainPicker,
		color = { 1, 1, 1 },
	})
	mainPicker.borderTop = 8
	mainPicker.borderLeft = 8
	mainPicker.borderRight = 8
	mainPicker.width = params.mainWidth
	mainPicker.height = params.height
	mainPicker.imageScaleX = getScaleFactor(params.mainWidth)
	mainPicker.imageScaleY = getScaleFactor(params.height)

	mainPicker.texture = picker.textures.main
	mainPicker.imageFilter = false
	mainPicker.texture.pixelData:setPixelsFloat(picker.mainImage:toPixelBufferFloat())
	mainPicker:register(tes3.uiEvent.mouseDown, function(e)
		tes3ui.captureMouseDrag(true)
	end)
	mainPicker:register(tes3.uiEvent.mouseRelease, function(e)
		tes3ui.captureMouseDrag(false)
	end)
	local mainIndicator = createIndicator(
		mainPicker,
		"main",
		mainIndicatorInitialAbsolutePosAlignX,
		mainIndicatorInitialAbsolutePosAlignY
	)

	local slider
	if params.showSaturationSlider then
		slider = pickerContainer:createSlider({
			id = UIID.indicator.slider,
			step = 1,
			jump = 1,
			current = mainIndicatorInitialAbsolutePosAlignX * CONSTANTS.SLIDER_SCALE,
			max = CONSTANTS.SLIDER_SCALE,
		})
		slider.width = picker.mainWidth
		slider.borderBottom = 8
		slider.borderLeft = 8
		slider.borderRight = 8
		slider:setLuaData("mwse:indicatorID", "slider")
	end

	local otherPickersContainer = mainRow
	-- In vertical configuration, we create other pickers in the second row below main picker.
	if params.vertical then
		otherPickersContainer = mainRow:createBlock({
			id = tes3ui.registerID("ColorPicker_other_pickers_container")
		})
		otherPickersContainer.flowDirection = tes3.flowDirection.leftToRight
		otherPickersContainer.autoWidth = true
		otherPickersContainer.autoHeight = true
	end


	local saturationPicker
	if params.showSaturationPicker then
		saturationPicker = otherPickersContainer:createRect({
			id = UIID.saturationPicker,
			color = { 1, 1, 1 },
		})
		saturationPicker.borderAllSides = 8
		saturationPicker.width = params.hueWidth
		saturationPicker.height = params.height
		saturationPicker.imageScaleX = getScaleFactor(params.hueWidth)
		saturationPicker.imageScaleY = getScaleFactor(params.height)


		saturationPicker.texture = picker.textures.saturation
		saturationPicker.imageFilter = false
		saturationPicker.texture.pixelData:setPixelsFloat(picker.saturationBar:toPixelBufferFloat())
		saturationPicker:register(tes3.uiEvent.mouseDown, function(e)
			tes3ui.captureMouseDrag(true)
		end)
		saturationPicker:register(tes3.uiEvent.mouseRelease, function(e)
			tes3ui.captureMouseDrag(false)
		end)
		local saturationIndicator = createIndicator(
			saturationPicker, "saturation", 0.5, 1 - initialHSV.s
		)
	end

	local huePicker = otherPickersContainer:createRect({
		id = tes3ui.registerID("ColorPicker_hue_picker"),
		color = { 1, 1, 1 },
	})
	huePicker.borderAllSides = 8
	huePicker.width = params.hueWidth
	huePicker.height = params.height
	huePicker.imageScaleX = getScaleFactor(params.hueWidth)
	huePicker.imageScaleY = getScaleFactor(params.height)


	huePicker.texture = picker.textures.hue
	huePicker.imageFilter = false
	huePicker.texture.pixelData:setPixelsFloat(picker.hueBar:toPixelBufferFloat())
	huePicker:register(tes3.uiEvent.mouseDown, function(e)
		tes3ui.captureMouseDrag(true)
	end)
	huePicker:register(tes3.uiEvent.mouseRelease, function(e)
		tes3ui.captureMouseDrag(false)
	end)
	local hueIndicator = createIndicator(
		huePicker,
		"hue",
		0.5,
		hueIndicatorInitialAbsolutePosAlignY
	)

	local alphaPicker, alphaIndicator
	if params.alpha then
		alphaPicker = otherPickersContainer:createRect({
			id = tes3ui.registerID("ColorPicker_alpha_picker"),
			color = { 1, 1, 1 },
		})
		alphaPicker.borderAllSides = 8
		alphaPicker.width = params.hueWidth
		alphaPicker.height = params.height
		alphaPicker.imageScaleX = getScaleFactor(params.hueWidth)
		alphaPicker.imageScaleY = getScaleFactor(params.height)

		alphaPicker.texture = picker.textures.alpha
		alphaPicker.imageFilter = false
		alphaPicker.texture.pixelData:setPixelsFloat(picker.alphaBar:toPixelBufferFloat())
		alphaPicker:register(tes3.uiEvent.mouseDown, function(e)
			tes3ui.captureMouseDrag(true)
		end)
		alphaPicker:register(tes3.uiEvent.mouseRelease, function(e)
			tes3ui.captureMouseDrag(false)
		end)

		alphaIndicator = createIndicator(alphaPicker, "alpha", 0.5, 1 - params.initialAlpha)
	end

	local previewContainer
	if params.showPreviews then
		previewContainer = otherPickersContainer:createBlock({ id = UIID.preview.topContainer })
		previewContainer.flowDirection = tes3.flowDirection.topToBottom
		previewContainer.autoWidth = true
		previewContainer.autoHeight = true
		createPreview(params, previewContainer, initialColor, params.initialAlpha, "Current", true)
	end

	-- Implement picking behavior
	mainPicker:register(tes3.uiEvent.mouseStillPressed, function(e)
		local x = math.clamp(e.relativeX, 1, mainPicker.width)
		local y = math.clamp(e.relativeY, 1, mainPicker.height)

		local current = picker:getColor()
		local pickedHSV = colorUtils.sRGBtoHSV(ffiPixel({ current.r, current.g, current.b }))
		local s = math.remap(x, 1, mainPicker.width, SV_EPSILON, 1)
		local v = math.clamp(1 - math.remap(y, 1, mainPicker.height, 0, 1), SV_EPSILON, 1)
		pickedHSV.s = s
		-- Only allow horizontal movement (saturation) while shift is pressed.
		if not isShiftDown() then
			pickedHSV.v = v
		end
		local pickedColor = colorUtils.HSVtosRGB(pickedHSV)

		picker:colorSelected(pickedColor, picker:getAlpha())
		parent:triggerEvent(tes3.uiEvent.colorChanged)
	end)
	if params.showSaturationSlider then
		slider:register(tes3.uiEvent.partScrollBarChanged, function(e)
			local x = math.clamp((slider.widget.current / CONSTANTS.SLIDER_SCALE) * mainPicker.width, 1, mainPicker.width)
			local current = picker:getColor()
			local pickedHSV = colorUtils.sRGBtoHSV(ffiPixel({ current.r, current.g, current.b }))
			pickedHSV.s = x / mainPicker.width

			local pickedColor = colorUtils.HSVtosRGB(pickedHSV)
			picker:colorSelected(pickedColor, picker:getAlpha())
			parent:triggerEvent(tes3.uiEvent.colorChanged)
		end)
	end

	huePicker:register(tes3.uiEvent.mouseStillPressed, function(e)
		local y = math.clamp(e.relativeY, 1, huePicker.height)
		local current = picker:getColor()
		local currentHSV = colorUtils.sRGBtoHSV(ffiPixel({ current.r, current.g, current.b }))
		-- We don't pick the color from the very bottom in the hue picker.
		-- The hue indicator would jump to the top of the hue picker if it was dragged all the way down.
		-- I (C3pa) suppose this happens because of numerical instability of multiple conversions from sRGB -> HSV.
		currentHSV.h = math.remap(y, 1, huePicker.height, 0, 360 - HUE_EPSILON)

		local pickedColor = colorUtils.HSVtosRGB(currentHSV)
		picker:hueChanged(pickedColor, picker:getAlpha())
		parent:triggerEvent(tes3.uiEvent.colorChanged)
	end)

	if params.showSaturationPicker then
		saturationPicker:register(tes3.uiEvent.mouseStillPressed, function(e)
			local y = math.clamp(e.relativeY, 1, saturationPicker.height)
			local current = picker:getColor()
			local currentHSV = colorUtils.sRGBtoHSV(ffiPixel({ current.r, current.g, current.b }))
			currentHSV.s = math.clamp(1 - math.remap(y, 1, saturationPicker.height, 0, 1), SV_EPSILON, 1)

			local pickedColor = colorUtils.HSVtosRGB(currentHSV)
			picker:colorSelected(pickedColor, picker:getAlpha())
			parent:triggerEvent(tes3.uiEvent.colorChanged)
		end)
	end

	if params.alpha then
		alphaPicker:register(tes3.uiEvent.mouseStillPressed, function(e)
			local y = math.clamp(e.relativeY / alphaPicker.height, 0, 1)
			picker:colorSelected(picker:getColor(), 1 - y)
			parent:triggerEvent(tes3.uiEvent.colorChanged)
		end)
	end

	if params.showPreviews and params.showOriginal then
		--- @param e tes3uiEventData
		local function resetColor(e)
			picker:hueChanged(params.initialColor, params.initialAlpha)
			parent:triggerEvent(tes3.uiEvent.colorChanged)
		end
		createPreview(params, previewContainer, initialColor, params.initialAlpha, "Original", false, resetColor)
	end

	return {
		mainPicker = mainPicker,
		huePicker = huePicker,
		alphaPicker = alphaPicker,
	}
end

--- Returns the channel value by reading `text` property of given TextInput. Returned value is in range of [0, 1].
--- @param input tes3uiElement
local function getInputValue(input)
	local text = input.text
	local hasAlpha = input:getLuaData("mwse:hasAlpha")

	-- Clearing the text input will set the color to 0.
	if text == "" then
		text = hasAlpha and "00000000" or "000000"
	end

	-- Clamp the text length to the correct value.
	local expectedLength = hasAlpha and 8 or 6
	local actualLength = string.len(text)
	local delta = actualLength - expectedLength

	if delta > 0 then
		text = string.sub(text, 1, expectedLength)
	elseif delta < 0 then
		-- If user cleared some characters, fill with 0.
		text = text .. string.rep('0', math.abs(delta))
	end

	return text
end

--- @param params tes3uiElement.createColorPicker.params
--- @param picker tes3uiColorPicker
--- @param parent tes3uiElement
local function createDataBlock(params, picker, parent)
	local dataRow = parent:createThinBorder({ id = UIID.dataRowContainer })
	dataRow.flowDirection = tes3.flowDirection.leftToRight
	dataRow.autoHeight = true
	dataRow.autoWidth = true
	dataRow.widthProportional = 1.0
	dataRow.borderLeft = 12
	dataRow.paddingAllSides = 4
	dataRow.childAlignY = 0.5

	local text = i18n("RGB: #")
	local inputText = format.pixelToHex(params.initialColor)
	if params.alpha then
		text = i18n("ARGB: #")
		local initialColor = table.copy(params.initialColor) --[[@as mwseColorATable]]
		initialColor.a = params.initialAlpha
		inputText = format.pixelToHex(initialColor)
	end
	local label = dataRow:createLabel({
		id = tes3ui.registerID("ColorPicker_data_row_label"),
		text = text
	})
	label.borderLeft = 4

	local input = dataRow:createTextInput({
		id = UIID.textInput,
		text = inputText,
	})
	input:setLuaData("mwse:hasAlpha", params.alpha or false)
	input.autoWidth = true
	input.widthProportional = 1.0

	-- Update color after new value was entered.
	input:registerAfter(tes3.uiEvent.keyEnter, function(e)
		local newColor, alpha = format.hexToPixel(getInputValue(input))
		picker:hueChanged(newColor, alpha)
		parent:triggerEvent(tes3.uiEvent.colorChanged)
	end)

	local copyButton = dataRow:createButton({
		id = tes3ui.registerID("ColorPicker_data_row_copy_button"),
		text = i18n("Copy"),
	})
	copyButton:register(tes3.uiEvent.mouseClick, function(e)
		local text = getInputValue(input)
		os.setClipboardText(text)
		tes3.messageBox(i18n("%%q copied to clipboard."), text)
	end)

	return {
		input = input,
	}
end

---@param params tes3uiElement.createColorPicker.params?
---@return tes3uiElement result
function tes3uiElement:createColorPicker(params)
	params = params or {}
	assert(type(params) == "table", "Invalid parameters provided.")
	params = table.deepcopy(params) --[[@as tes3uiElement.createColorPicker.params]]
	params.alpha = table.get(params, "alpha", false)
	params.height = table.get(params, "height", 256)
	params.mainWidth = table.get(params, "mainWidth", 256)
	params.hueWidth = table.get(params, "hueWidth", 32)
	params.showPreviews = table.get(params, "showPreviews", true)
	params.showOriginal = table.get(params, "showOriginal", true)
	params.showDataRow = table.get(params, "showDataRow", true)
	params.showSaturationSlider = table.get(params, "showSaturationSlider", true)
	params.showSaturationPicker = table.get(params, "showSaturationPicker", true)
	params.vertical = table.get(params, "vertical", false)
	params.initialAlpha = table.get(params, "initialAlpha", 1.0)
	params.initialColor = table.get(params, "initialColor", { r = 1.0, g = 1.0, b = 1.0 })
	-- Make sure the colors are in [0, 1] range.
	params.initialAlpha = math.clamp(params.initialAlpha, 0, 1)
	params.initialColor = validate.clampColor(params.initialColor)

	-- When picker doesn't have checkerd preview, but has original preview let's make default
	-- color preview width double the normal, so the current and original previews form a square.
	if params.alpha == false and params.showOriginal then
		params.previewWidth = table.get(params, "previewWidth", 128)
	end

	local picker = ColorPicker:new({
		height = params.height,
		mainWidth = params.mainWidth,
		hueWidth = params.hueWidth,
		initialColor = params.initialColor,
		initialAlpha = params.initialAlpha,
	})

	local container = self:createBlock({ id = params.id })
	container.autoWidth = true
	container.autoHeight = true
	container.widthProportional = 1.0
	container.flowDirection = tes3.flowDirection.topToBottom
	createPickerBlock(params, picker, container)

	if params.showDataRow then
		createDataBlock(params, picker, container)
	end


	container:makeLuaWidget("colorPicker", picker)

	return container
end

tes3ui.defineLuaWidget({
	name = "colorPicker",
	metatable = ColorPicker,
})
