--[[
	Button Setting for binding a key/mouse combination. Variable returned in the form:
		{
			keyCode = tes3.scanCode/nil,
			isAltDown = true/false,
			isShiftDown = true/false,
			isControlDown = true/false,
			mouseWheel = integer/nil,
			mouseButton = number/nil,
		}
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Button")

--- @class mwseMCMKeyBinder
local KeyBinder = Parent:new()
KeyBinder.allowCombinations = true
KeyBinder.allowMouse = false



--- @param keyCode integer|nil
--- @return string|nil letter
function KeyBinder:getLetter(keyCode)
	local letter = table.find(tes3.scanCode, keyCode)
	local returnString = tes3.scanCodeToNumber[keyCode] or letter
	if returnString then
		return string.upper(returnString)
	end
end

local mouseWheelDirectionName = {
	[1] = "Mouse wheel up",
	[-1] = "Mouse wheel down",
}

--- @param wheel integer|nil
--- @return string|nil result
function KeyBinder:getMouseWheelText(wheel)
	local name = mouseWheelDirectionName[wheel]
	if name then
		return mwse.mcm.i18n(name)
	end
end

local mouseButtonName = {
	[0] = "Left mouse button",
	[1] = "Right mouse button",
	[2] = "Middle mouse button",
}

--- @param buttonIndex number|nil
--- @return string|nil result
function KeyBinder:getMouseButtonText(buttonIndex)
	-- Only work with button indices supporte by the inputController
	if not buttonIndex or buttonIndex > 7 or buttonIndex < 0 then
		return
	end
	local name = mouseButtonName[buttonIndex]
	if name then
		return mwse.mcm.i18n(name)
	end

	return string.format(mwse.mcm.i18n("Mouse %s"), buttonIndex)
end

--- @param keyCombo mwseKeyMouseCombo
--- @return string result
function KeyBinder:getComboString(keyCombo)
	-- Add failsafe for malformed keyCombos
	local hasMouse = keyCombo.mouseButton or keyCombo.mouseWheel
	if hasMouse and not self.allowMouse then
		mwse.log("[KeyBinder: WARN]: A KeyBinder with allowMouse = false got a key combination containing " ..
			"mouseButton/mouseWheel. The combination is:\n%s", json.encode(keyCombo))
		-- Make it a soft error
		keyCombo.mouseButton = false
		keyCombo.mouseWheel = false
	end

	local hasModifier = keyCombo.isAltDown or keyCombo.isShiftDown or keyCombo.isControlDown
	if hasModifier and not self.allowCombinations then
		mwse.log("[KeyBinder: WARN]: A KeyBinder with allowCombinations = false got a key combination containing " ..
			"modifer key. The combination is:\n%s", json.encode(keyCombo))
		-- Make it a soft error: don't show the modifier in the KeyBinder button. It stays in the config.
		keyCombo.isAltDown = false
		keyCombo.isShiftDown = false
		keyCombo.isControlDown = false
	end

	local comboText = mwse.mcm.getKeyComboName(keyCombo) or
	                  string.format("{%s}", mwse.mcm.i18n("unknown key"))

	return comboText
end

KeyBinder.convertToLabelValue = KeyBinder.getComboString

--- @param e keyUpEventData|mouseButtonDownEventData|mouseWheelEventData
function KeyBinder:keySelected(e)
	local variable = self.variable.value
	variable.keyCode = e.keyCode or false

	if self.allowMouse then
		local wheel = e.delta and math.clamp(e.delta, -1, 1)
		variable.mouseWheel = wheel or false
		variable.mouseButton = e.button or false
	end

	if self.allowCombinations then
		variable.isAltDown = e.isAltDown
		variable.isShiftDown = e.isShiftDown
		variable.isControlDown = e.isControlDown
	end

	self:update()
	self.elements.outerContainer:updateLayout()
end

local popupId = tes3ui.registerID("KeyBinderPopup")

--- @return tes3uiElement menu
function KeyBinder:createPopupMenu()
	local menu = tes3ui.findHelpLayerMenu(popupId)

	if not menu then
		menu = tes3ui.createMenu({ id = popupId, fixedFrame = true })
		menu.absolutePosAlignX = 0.5
		menu.absolutePosAlignY = 0.5
		menu.autoWidth = true
		menu.autoHeight = true
		menu.alpha = tes3.worldController.menuAlpha

		local block = menu:createBlock()
		block.autoWidth = true
		block.autoHeight = true
		block.paddingAllSides = 8
		block.flowDirection = tes3.flowDirection.topToBottom

		local headerText = mwse.mcm.i18n("SET NEW KEYBIND.")
		if self.keybindName then
			headerText = string.format(mwse.mcm.i18n("SET %s KEYBIND."), self.keybindName)
		end
		local header = block:createLabel({
			text = headerText
		})
		header.color = tes3ui.getPalette(tes3.palette.headerColor)

		block:createLabel({
			text = mwse.mcm.i18n("Press any key to set the bind or ESC to cancel.")
		})

		tes3ui.enterMenuMode(popupId)
	end
	menu:getTopLevelMenu():updateLayout()
	return menu
end


function KeyBinder:showKeyBindMessage()
	self:createPopupMenu()

	--- @param e keyUpEventData|mouseButtonDownEventData|mouseWheelEventData
	local function waitInput(e)
		-- Unregister this function once we got some input
		event.unregister(tes3.event.keyUp, waitInput)
		if self.allowMouse then
			event.unregister(tes3.event.mouseButtonDown, waitInput)
			event.unregister(tes3.event.mouseWheel, waitInput)
		end

		local popup = tes3ui.findMenu(popupId)
		if popup then
			popup:destroy()
		end

		-- Allow closing the menu using escape, wihout binding anything
		if e.keyCode == tes3.scanCode.esc then
			return
		end

		self:keySelected(e)
	end

	event.register(tes3.event.keyUp, waitInput)
	if self.allowMouse then
		event.register(tes3.event.mouseButtonDown, waitInput)
		event.register(tes3.event.mouseWheel, waitInput)
	end
end

function KeyBinder:press()
	-- Display message to change keybinding
	self:showKeyBindMessage()
end

-- UI methods


--- @param parentBlock tes3uiElement
function KeyBinder:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.autoWidth = false
	-- self.elements.outerContainer.borderRight = self.indent
end

return KeyBinder
