--[[
	An abstract Button Setting for sharing code between various key/mouse binders.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Button")

--- @class mwseMCMBinder
local Binder = Parent:new()
Binder.__index = Binder
Binder.allowCombinations = true

local popupId = tes3ui.registerID("KeyMouseBinderPopup")
local labelId = tes3ui.registerID("KeyMouseBinderLabel")

--- @param keyCombo mwseKeyMouseCombo
function Binder:isUnbound(keyCombo)
	local keyBound = keyCombo.keyCode ~= false
	local buttonBound = keyCombo.mouseButton ~= false
	local wheelBound = keyCombo.mouseWheel ~= false

	if keyBound or buttonBound or wheelBound then
		return false
	end

	return true
end

--- @param keyCombo mwseKeyMouseCombo
--- @return string result
function Binder:getComboString(keyCombo)
	if self:isUnbound(keyCombo) then
		return mwse.mcm.i18n("Unbound action")
	end
	local comboText = mwse.mcm.getKeyComboName(keyCombo)

	-- Add failsafe for malformed keyCombos.
	if not comboText then
		mwse.log("[MCMBinder]: couldn't resolve any text for the given combo:\n%s", json.encode(keyCombo))
		comboText = string.format("{%s}", mwse.mcm.i18n("unknown key"))
	end

	return comboText
end

Binder.convertToLabelValue = Binder.getComboString

--- @param e keyDownEventData|mouseButtonDownEventData|mouseWheelEventData|mwseKeyMouseCombo
function Binder:getKeyComboFromEventData(e)
	local wheel = e.delta and math.clamp(e.delta, -1, 1)
	local result = {
		keyCode = e.keyCode or false,
		mouseButton = e.button or false,
		mouseWheel = wheel or false,
	}

	if self.allowCombinations then
		result.isAltDown = e.isAltDown or false
		result.isShiftDown = e.isShiftDown or false
		result.isControlDown = e.isControlDown or false
	end

	return result
end

--- @param keyCombo mwseKeyMouseCombo
function Binder:keySelected(keyCombo)
	self.variable.value = keyCombo
	self:update()
	-- After the text on the button changes, it's width may be changed significantly that is can
	-- overlap with the label. Updating layout will make sure there is no overlap.
	self.elements.outerContainer:updateLayout()
end

--- @param keyBind mwseKeyMouseCombo
function Binder:updatePopupLabel(keyBind)
	local menu = tes3ui.findMenu(popupId)
	if not menu then
		return false
	end
	local label = menu:findChild(labelId)
	if not label then
		return false
	end

	label.text = self:getComboString(keyBind)
	label:updateLayout()
	return true
end

-- Used to get the proper version of help text based on which input can be used in the Binder.
local eventToText = {
	[tes3.event.keyDown] = "Press any key to set as new binding.",
	[tes3.event.mouseButtonDown] = "Press any mouse button to set as new binding.",
	[tes3.event.mouseWheel] = "Scroll with the mouse wheel to set as new binding.",
	[tes3.event.keyDown .. tes3.event.mouseButtonDown] = "Press any key or mouse button to set as new binding.",
	[tes3.event.keyDown .. tes3.event.mouseWheel] =
		"Press any key or scroll with the mouse wheel to set as new binding.",
	[tes3.event.mouseButtonDown .. tes3.event.mouseWheel] =
		"Press any mouse button or scroll with the mouse wheel to set as new binding.",
	[tes3.event.keyDown .. tes3.event.mouseButtonDown .. tes3.event.mouseWheel] =
		"Press any key, mouse button or scroll with the mouse wheel to set as new binding.",
}
local function sorter(a, b)
	return a < b
end

function Binder:getHelpText()
	local events = {}
	for eventId, _ in pairs(self.observeEvents) do
		table.insert(events, eventId)
	end
	table.sort(events, sorter)
	local key = table.concat(events)
	local start = mwse.mcm.i18n(eventToText[key])
	return start .. " " .. mwse.mcm.i18n("Help text")
end

function Binder:press()
	self.newKeybind = self.variable.value

	--- @type function
	local endInteraction

	local menu = tes3ui.createMenu({ id = popupId, fixedFrame = true })
	menu.absolutePosAlignX = 0.5
	menu.absolutePosAlignY = 0.5
	menu.autoWidth = true
	menu.autoHeight = true
	menu.alpha = tes3.worldController.menuAlpha
	menu.maxWidth = 450

	local block = menu:createBlock()
	block.autoWidth = true
	block.autoHeight = true
	block.paddingAllSides = 8
	block.flowDirection = tes3.flowDirection.topToBottom
	block.maxWidth = 440

	local headerText = mwse.mcm.i18n("SET NEW KEYBIND")
	if self.keybindName then
		headerText = string.format(mwse.mcm.i18n("SET %s KEYBIND"), self.keybindName)
	end
	local header = block:createLabel({
		text = headerText
	})
	header.justifyText = tes3.justifyText.center
	header.color = tes3ui.getPalette(tes3.palette.headerColor)
	header.borderAllSides = 4

	local infoBlock = block:createBlock()
	infoBlock.width = 400
	infoBlock.autoHeight = true
	infoBlock.flowDirection = tes3.flowDirection.topToBottom
	infoBlock.borderAllSides = 4
	local helpText = infoBlock:createLabel({
		text = self:getHelpText()
	})
	helpText.wrapText = true

	local label = block:createLabel({
		id = labelId,
		text = self:getComboString(self.newKeybind)
	})
	label.justifyText = tes3.justifyText.center
	label.borderTop = 8

	local buttonBlock = menu:createBlock()
	buttonBlock.autoWidth = true
	buttonBlock.autoHeight = true
	buttonBlock.paddingAllSides = 8
	buttonBlock.flowDirection = tes3.flowDirection.leftToRight

	local cancel = buttonBlock:createButton({ text = self.sCancel })
	cancel:registerAfter(tes3.uiEvent.mouseClick, function(e)
		-- Don't set new keybind on clicking Cancel button.
		self.newKeybind = self.variable.value
		endInteraction()
	end)

	local clear = buttonBlock:createButton({ text = mwse.mcm.i18n("Clear") })
	clear:registerAfter(tes3.uiEvent.mouseClick, function(e)
		-- Unbind the keybind on clicking Clear button.
		self.newKeybind = self:getKeyComboFromEventData({})
		self:updatePopupLabel(self.newKeybind)
	end)

	local ok = buttonBlock:createButton({ text = self.sOK })
	ok:registerAfter(tes3.uiEvent.mouseClick, function(e)
		endInteraction()
	end)

	-- Make sure we don't capture Left mouse button while clicking on any of the buttons.
	local inputBlocked = false
	local function blockInput()
		inputBlocked = true
	end
	local function unblockInput()
		inputBlocked = false
	end
	---@param buttons tes3uiElement[]
	local function registerBlockFunctions(buttons)
		for _, button in ipairs(buttons) do
			button:registerBefore(tes3.uiEvent.mouseOver, blockInput)
			button:registerAfter(tes3.uiEvent.mouseLeave, unblockInput)
		end
	end
	registerBlockFunctions({ cancel, clear, ok })

	tes3ui.enterMenuMode(popupId)
	menu:updateLayout()

	--- @param e keyDownEventData|mouseButtonDownEventData|mouseWheelEventData
	local function observeInput(e)
		if e.button and inputBlocked then
			return
		end
		self.newKeybind = self:getKeyComboFromEventData(e)
		self:updatePopupLabel(self.newKeybind)
	end

	for eventId, _ in pairs(self.observeEvents) do
		event.register(eventId, observeInput)
	end

	endInteraction = function()
		for eventId, _ in pairs(self.observeEvents) do
			event.unregister(eventId, observeInput)
		end

		local popup = tes3ui.findMenu(popupId)
		if popup then
			popup:destroy()
		end

		self:keySelected(self.newKeybind)
		self.newKeybind = nil
	end
end

-- UI methods


--- @param parentBlock tes3uiElement
function Binder:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.autoWidth = false
	-- self.elements.outerContainer.borderRight = self.indent
end

return Binder
