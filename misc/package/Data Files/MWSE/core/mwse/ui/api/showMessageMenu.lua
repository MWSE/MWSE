local common = require("mwse.common")

--
-- Define common IDs.
--

local uiids = {
	menu = tes3ui.registerID("MenuMessage"),
	header = tes3ui.registerID("MenuMessage_Header"),
	message = tes3ui.registerID("MenuMessage_Message"),
    customBlock = tes3ui.registerID("MenuMessage_CustomBlock"),
	buttonBlock = tes3ui.registerID("MenuMessage_ButtonBlock"),
	button = tes3ui.registerID("MenuMessage_Button"),
	navigationBlock = tes3ui.registerID("MenuMessage_NavigationBlock"),
	previousButton = tes3ui.registerID("MenuMessage_PreviousPageButton"),
	cancelButton = tes3ui.registerID("MenuMessage_CancelButton"),
	nextButton = tes3ui.registerID("MenuMessage_NextPageButton"),
}


--
-- Callbacks for UI elements.
--

--- @param e tes3uiEventData
--- @return tes3ui.showMessageMenu.params
local function callbackGetMenuData(e)
	return e.source:getTopLevelMenu():getLuaData("mwse:messageData")
end

--- @param e tes3uiEventData
local function callbackCloseMenu(e)
	local menu = e.source:getTopLevelMenu()
	local menuData = callbackGetMenuData(e)
	
	if menuData.leaveMenuMode then
		tes3ui.leaveMenuMode()
	end

	menu:destroy()
end

--- @param e tes3uiEventData
local function callbackWithParamCallback(e)
	-- Fire off our callback.
	local buttonData = e.source:getLuaData("mwse:buttonData") --- @type tes3ui.showMessageMenu.params.button
	if (buttonData.callback) then
		local menuData = callbackGetMenuData(e)

		-- Close message menu before the callback, so that the callback may open a message menu without conflicts.
		-- tes3.messageBox will try to re-use an existing MenuMessage menu, but that is not compatible with this implementation.
		-- Specifically, tes3.messageBox can crash without child ids MenuMessage_image, MenuMessage_brick_frame, MenuMessage_button_layout.
		callbackCloseMenu(e)

		buttonData.callback(menuData.callbackParams)
	else
		-- Always close the menu after.
		callbackCloseMenu(e)
	end
end

--- @param e tes3uiEventData
local function callbackButtonHelp(e)
	local menuData = callbackGetMenuData(e)
	local buttonData = e.source:getLuaData("mwse:buttonData") --- @type tes3ui.showMessageMenu.params.button

	-- Get our tooltip creator.
	local tooltipCreator = buttonData.tooltip
	if (e.source.disabled) then
		tooltipCreator = buttonData.tooltipDisabled
	end

	-- Ensure we got something valid.
	tooltipCreator = common.resolveDynamicValue(tooltipCreator, menuData.callbackParams)
	if (not tooltipCreator) then
		return
	end
	--- @cast tooltipCreator tes3ui.showMessageMenu.params.tooltip

	-- Create our base tooltip.
	local tooltip = tes3ui.createTooltipMenu()
	local outerBlock = tooltip:createBlock()
	outerBlock.flowDirection = "top_to_bottom"
	outerBlock.paddingTop = 6
	outerBlock.paddingBottom = 12
	outerBlock.paddingLeft = 6
	outerBlock.paddingRight = 6
	outerBlock.maxWidth = 300
	outerBlock.autoWidth = true
	outerBlock.autoHeight = true

	-- Make our header.
	if tooltipCreator.header then
		local headerLabel = outerBlock:createLabel({
			text = common.resolveDynamicText(tooltipCreator.header, menuData.callbackParams),
		})
		headerLabel.autoHeight = true
		headerLabel.width = 285
		headerLabel.color = tes3ui.getPalette("header_color")
		headerLabel.wrapText = true
	end

	-- Make our normal text.
	if tooltipCreator.text then
		local descriptionLabel = outerBlock:createLabel({
			text = common.resolveDynamicText(tooltipCreator.text, menuData.callbackParams),
		})
		descriptionLabel.autoHeight = true
		descriptionLabel.width = 285
		descriptionLabel.wrapText = true
	end

	-- Fire off an event so the tooltip can be further extended.
	if (tooltipCreator.callback) then
		tooltipCreator.callback({
			data = menuData,
			button = buttonData,
			tooltip = outerBlock,
		})
	end

	tooltip:updateLayout()
end

-- Forward declare the recreateMenu function.
--- @type fun(menu: tes3uiElement)
local recreateMenu

--- @param e tes3uiEventData
local function callbackPrevious(e)
	local params = callbackGetMenuData(e)
	params.page = math.max(params.page - 1, 1)
	recreateMenu(e.source:getTopLevelMenu())
end

--- @param e tes3uiEventData
local function callbackNext(e)
	local params = callbackGetMenuData(e)
	params.page = params.page + 1
	recreateMenu(e.source:getTopLevelMenu())
end

--- @param e tes3uiEventData
local function callbackCancel(e)
	-- Fire off our callback.
	local params = callbackGetMenuData(e)
	if (params.cancelCallback) then
		params.cancelCallback()
	end

	-- Always close the menu after.
	callbackCloseMenu(e)
end

--- @param parent tes3uiElement
--- @param buttonData tes3ui.showMessageMenu.params.button
--- @param messageData tes3ui.showMessageMenu.params
--- @return tes3uiElement
local function addButton(parent, buttonData, messageData)
	local button = parent:createButton({
		id = uiids.button,
		text = common.resolveDynamicText(buttonData.text)
	})

	-- Store associated button data.
	button:setLuaData("mwse:buttonData", buttonData)

	-- Set disabled if there are requirements.
	if (buttonData.enableRequirements and not buttonData.enableRequirements(messageData.callbackParams)) then
		common.ui.disable(button)
	end

	-- Setup callbacks.
	button:register("mouseClick", callbackWithParamCallback)
	button:register("help", callbackButtonHelp)

	return button
end


--
-- Main menu creation.
--

--- @param menu tes3uiElement
recreateMenu = function(menu)
	-- Get our data.
	local messageData = menu:getLuaData("mwse:messageData") --- @type tes3ui.showMessageMenu.params

	-- Purge all children.
	local contentElement = menu:getContentElement()
	contentElement:destroyChildren()

	-- Add header.
	if (messageData.header) then
		local label = contentElement:createLabel({
			id = uiids.header,
			text = common.resolveDynamicText(messageData.header)
		})
		label.color = tes3ui.getPalette("header_color")
	end

    -- Add custom element
    if (messageData.customBlock) then
        --create outer block to put custom element in
        local customBlock = contentElement:createBlock()
        customBlock.flowDirection = "top_to_bottom"
        customBlock.autoHeight = true
        customBlock.autoWidth = true
        messageData.customBlock(customBlock)
    end

	-- Add main message text.
	if (messageData.message) then
		local label = contentElement:createLabel({
			id = uiids.message,
			text = common.resolveDynamicText(messageData.message)
		})
		label.wrapText = true
	end

	-- Prevent wrapped text from bugging out.
	contentElement:updateLayout()

	-- Create button block
	local buttonsBlock = contentElement:createBlock()
	buttonsBlock.flowDirection = "top_to_bottom"
	buttonsBlock.autoHeight = true
	buttonsBlock.autoWidth = true
	buttonsBlock.childAlignX = 0.5

	-- Get an array of showable buttons.
	local showableButtons = {}
	for _, button in ipairs(messageData.buttons) do
		if (button.showRequirements == nil or button.showRequirements(messageData.callbackParams)) then
			table.insert(showableButtons, button)
		end
	end

	-- Get which indecies to show.
	local startIndex = (messageData.page - 1) * messageData.pageSize + 1
	local endIndex = startIndex + messageData.pageSize - 1
	if (showableButtons[startIndex] == nil) then
		messageData.page = 1
		startIndex = 1
		endIndex = messageData.pageSize
	end

	-- Create our buttons.
	for i = startIndex, endIndex do
		local buttonData = showableButtons[i]
		if (buttonData) then
			addButton(buttonsBlock, buttonData, messageData)
		end
	end

	-- Add navigation/cancel buttons.
	local bottomButtonsBlock = contentElement:createBlock()
	bottomButtonsBlock.flowDirection = "left_to_right"
	bottomButtonsBlock.borderTop = 4
	bottomButtonsBlock.autoHeight = true
	bottomButtonsBlock.autoWidth = true

	-- Add previous button.
	local showPreviousNextButtons = #showableButtons > messageData.pageSize
	if (showPreviousNextButtons) then
		local prevButton = bottomButtonsBlock:createButton({ text = "<" })
		if (messageData.page == 1) then
			common.ui.disable(prevButton)
		else
			prevButton:register("mouseClick", callbackPrevious)
		end
	end

	-- Add cancel button
	if (messageData.cancels) then
		local cancelButton = bottomButtonsBlock:createButton({
			id = uiids.cancelButton,
			text = messageData.cancelText or tes3.findGMST(tes3.gmst.sCancel).value
		})
		cancelButton:register("mouseClick", callbackCancel)
	end

	-- Add next button.
	if (showPreviousNextButtons) then
		local nextButton = bottomButtonsBlock:createButton({ text = ">" })
		if (showableButtons[endIndex + 1] == nil) then
			common.ui.disable(nextButton)
		else
			nextButton:register("mouseClick", callbackNext)
		end
	end

	-- Were there no buttons, and no cancel button? Prevent the player from getting menu-trapped.
	if (#buttonsBlock.children + #bottomButtonsBlock.children == 0) then
		local escapeButton = bottomButtonsBlock:createButton({
			id = uiids.cancelButton,
			text = tes3.findGMST(tes3.gmst.sOK).value
		})
		escapeButton:register("mouseClick", callbackCloseMenu)
	end

	-- Nuke our bottom bar if it isn't doing anything.
	if (#bottomButtonsBlock.children == 0) then
		bottomButtonsBlock:destroy()
		bottomButtonsBlock = nil
	end

	menu:updateLayout()
end

--- @param button tes3ui.showMessageMenu.params.button
local function validateButtonParam(button)
	assert(button.text, "Invalid button parameter. Must have valid text.")
end

---@param params tes3ui.showMessageMenu.params
function tes3ui.showMessageMenu(params)
	-- Validate parameters.
	assert(type(params) == "table", "Invalid parameters passed to function.")
	assert(type(params.buttons) == "table", "Invalid 'buttons' parameter provided. Must be a table.")
	assert(#params.buttons > 0, "Invalid 'buttons' parameter provided. Must be an array-style table with at least one element.")
	for _, button in ipairs(params.buttons) do
		validateButtonParam(button)
	end

	-- Prevent callback params from being copied.
	local callbackParams = params.callbackParams
	params.callbackParams = nil

	-- Clone the params so modders can reuse params between calls.
	local copiedParams = table.deepcopy(params)
	copiedParams.callbackParams = callbackParams

	-- Restore original callback param for table reuse.
	params.callbackParams = callbackParams

	-- Create menu.
	local menu = tes3ui.createMenu({ id = copiedParams.id or uiids.menu, fixedFrame = true })
	local contentElement = menu:getContentElement()
	contentElement.maxWidth = params.maxWidth or 400
	contentElement.childAlignX = 0.5
	tes3ui.enterMenuMode(menu.id)

	-- Store our params as lua data.
	menu:setLuaData("mwse:messageData", copiedParams)

	-- Initialize some default values.
	copiedParams.page = copiedParams.page or 1
	copiedParams.pageSize = copiedParams.pageSize or 30
	if copiedParams.leaveMenuMode == nil then
		-- Default to leaving menu mode for backwards compatibility.
		copiedParams.leaveMenuMode = true
	end

	-- Create our menu.
	recreateMenu(menu)

	return menu
end
