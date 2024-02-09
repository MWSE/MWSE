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

--- @return string result
function KeyBinder:getText()
	return self:getComboString(self.variable.value)
end

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
	-- Returns "SHIFT-X" if shift is held down but the active key is not Shift,
	-- otherwise just "X" (X being the key being pressed)
	-- And so on for Alt and Ctrl

	local keyCode = keyCombo.keyCode
	local comboText = self:getLetter(keyCode)

	if self.allowMouse then
		comboText = comboText or
		            self:getMouseWheelText(keyCombo.mouseWheel) or
		            self:getMouseButtonText(keyCombo.mouseButton)
	end

	-- Add failsafe for malformed keyCombos
	if not comboText then
		local inspect = require("inspect")
		mwse.log("[KeyBinder]: couldn't resolve any text for the given combo:\n%s", inspect(keyCombo))
		comboText = string.format("{%s}", mwse.mcm.i18n("unknown key"))
	end

	-- if you set allowCombinations to false, nothing functionally changes
	-- but the player doesn't see the prefix
	if not self.allowCombinations then
		return comboText
	end

	local hasAlt = (keyCombo.isAltDown and keyCode ~= tes3.scanCode.lAlt
	                                   and keyCode ~= tes3.scanCode.rAlt)
	local hasShift = (keyCombo.isShiftDown and keyCode ~= tes3.scanCode.lShift
	                                       and keyCode ~= tes3.scanCode.rShift)
	local hasCtrl = (keyCombo.isControlDown and keyCode ~= tes3.scanCode.lCtrl
	                                        and keyCode ~= tes3.scanCode.rCtrl)
	local prefixes = {}
	if hasShift then table.insert(prefixes, "Shift") end
	if hasAlt then table.insert(prefixes, "Alt") end
	if hasCtrl then table.insert(prefixes, "Ctrl") end
	table.insert(prefixes, comboText)
	return table.concat(prefixes, " - ")
end


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

	self:setText(self:getText())
	if self.callback then
		self:callback()
	end
end

local popupId = tes3ui.registerID("KeyBinderPopup")

--- @return tes3uiElement menu
function KeyBinder:createMenu()
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

function KeyBinder:closeMenu()
	local menu = tes3ui.findMenu(popupId)
	if menu then
		menu:destroy()
	end
end

--- @param eventId string
--- @param callback function
--- @param options? event.isRegistered.options
local function unregisterEvent(eventId, callback, options)
	if event.isRegistered(eventId, callback, options) then
		event.unregister(eventId, callback, options --[[@as event.unregister.options]])
	end
end

function KeyBinder:showKeyBindMessage()
	self:createMenu()

	--- @param e keyUpEventData|mouseButtonDownEventData|mouseWheelEventData
	local function waitInput(e)
		-- Unregister this function once we got some input
		unregisterEvent(tes3.event.keyUp, waitInput)
		unregisterEvent(tes3.event.mouseButtonDown, waitInput)
		unregisterEvent(tes3.event.mouseWheel, waitInput)

		-- Allow closing the menu using escape
		if e.keyCode == tes3.scanCode.esc then
			self:closeMenu()
			return
		end

		self:keySelected(e)
		self:closeMenu()
	end

	event.register(tes3.event.keyUp, waitInput)
	if self.allowMouse then
		event.register(tes3.event.mouseButtonDown, waitInput)
		event.register(tes3.event.mouseWheel, waitInput)
	end
end

function KeyBinder:update()
	-- Display message to change keybinding
	self:showKeyBindMessage()
end

-- UI methods


--- @param parentBlock tes3uiElement
function KeyBinder:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.autoWidth = false
	self.elements.outerContainer.widthProportional = 1.0
	-- self.elements.outerContainer.borderRight = self.indent
end

return KeyBinder
