--
-- Provides lua-written extensions to the UI library.
--

local common = require("mwse.common")

---@param e tes3uiEventData
local function preTextInputKeyPress(e)
	local element = e.source
	local keyPressed = common.ui.eventCallbackHelper.getKeyPressed(e)
	local characterEntered = common.ui.eventCallbackHelper.getCharacterPressed(e)

	-- Prevent tabs from inserting themselves for when alt-tabbing.
	if (keyPressed == 9) then
		return false
	end

	-- Are we in the placeholder state? Prevent garbage inputs.
	local placeholding = element:getLuaData("placeholding") --- @type boolean
	if (placeholding and characterEntered == nil) then
		return false
	end

	-- Is this field numbers-only?
	if (characterEntered and element:getLuaData("numeric") and characterEntered and not tonumber(characterEntered)) then
		-- minus sign is okay so long as it's the first character
		if characterEntered == "-" then
			if element.text and #element.text > 0 then
				return false
			end
		-- periods are okay so long as there aren't any others 
		elseif characterEntered == "." then
			if element.text and element.text:find("%.") then
				return false
			end
		else
			return false
		end
	end

	-- Update previous text.
	element:setLuaData("previousText", element.text)
end

---@param e tes3uiEventData
local function postTextInputKeyPress(e)
	local element = e.source

	-- Check if we need to change back to placeholder text.
	local placeholderText = element:getLuaData("placeholderText") --- @type string?
	if (placeholderText and element.text == "") then
		element.text = placeholderText
		element.color = tes3ui.getPalette("disabled_color")
		element:setLuaData("placeholding", true)

		-- Raise textCleared event.
		element:triggerEvent("textCleared")

		-- Update previous text.
		element:setLuaData("previousText", element.text)

		-- We don't need to do anything else from here.
		return
	end

	-- Ungray the text.
	element.color = tes3ui.getPalette("normal_color")
	element:setLuaData("placeholding", false)

	-- Raise textUpdated event.
	local previousText = element:getLuaData("previousText") --- @type string?
	if (element.text ~= previousText) then
		element:triggerEvent("textUpdated")
	end

	-- Update previous text.
	element:setLuaData("previousText", element.text)

	element:getTopLevelMenu():updateLayout()
end

function tes3uiElement:createTextInput(params)
	params = params or {}

	-- Create our element.
	local element = self:_createTextInput(params) ---@diagnostic disable-line
	local asWidget = element.widget

	-- Basic property setting.
	if (params.text) then
		element.text = params.text
	end

	-- Detect if we need to do any special handling.
	local addStandardKeyEvents = false

	-- Allow placeholder text.
	local placeholderText = params.placeholderText
	if (placeholderText) then
		element:setLuaData("placeholderText", placeholderText)
		addStandardKeyEvents = true

		-- If we weren't given text, set to the placeholder text.
		if (params.text == nil) then
			element.text = placeholderText
		end

		-- Fix color if we are using the placeholder text.
		if (params.text == nil or params.text == placeholderText) then
			element.color = tes3ui.getPalette("disabled_color")
			element:setLuaData("placeholding", true)
		end

		asWidget.eraseOnFirstKey = true
	end

	-- Only allow numbers.
	if (params.numeric) then
		addStandardKeyEvents = true
		element:setLuaData("numeric", true)
	end

	-- Handle focus.
	element:registerAfter("mouseClick", common.ui.eventCallback.acquireTextInput)
	if (params.autoFocus) then
		tes3ui.acquireTextInput(element)
	end

	if (addStandardKeyEvents) then
		element:registerBefore("keyPress", preTextInputKeyPress)
		element:registerAfter("keyPress", postTextInputKeyPress)
	end

	return element
end
