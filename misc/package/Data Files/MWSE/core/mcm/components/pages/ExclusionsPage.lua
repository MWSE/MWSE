--[[
	A very specialised, but still flexible page type that allows you
	to create filters on objects, plugins and more, and move them
	between allowed and blocked lists.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local utils = require("mcm.utils")
local Parent = require("mcm.components.pages.Page")

--- @class mwseMCMExclusionsPage
local ExclusionsPage = Parent:new()
-- public fields
ExclusionsPage.label = mwse.mcm.i18n("Exclusions")
ExclusionsPage.rightListLabel = mwse.mcm.i18n("Allowed")
ExclusionsPage.leftListLabel = mwse.mcm.i18n("Blocked")
ExclusionsPage.toggleText = mwse.mcm.i18n("Toggle Filtered")

-- private fields
local itemID = tes3ui.registerID("ExclusionListItem")
local placeholderText = mwse.mcm.i18n("Search...")

--- Constructor
--- @param data mwseMCMExclusionsPage.new.data?
--- @return mwseMCMExclusionsPage page
function ExclusionsPage:new(data)
	local t = {}
	if data then
		t = data --[[@as mwseMCMExclusionsPage]]
		local tabUID = ("Page_" .. t.label)
		t.tabUID = tes3ui.registerID(tabUID)
		utils.getOrInheritVariableData(t)
	end
	setmetatable(t, self)
	self.__index = self
	return t
end

local function getSortedModList()
	local list = tes3.getModList()
	for i, name in pairs(list) do
		list[i] = name:lower()
	end
	table.sort(list)
	return list
end

--- @class mwseMCMExclusionsPage.local.getSortedObjectList.params
--- @field objectType integer|integer[] Maps to values tes3.objectType enumeration
--- @field objectFilters table<string, unknown>
--- @field noScripted boolean

--- @param params mwseMCMExclusionsPage.local.getSortedObjectList.params
--- @return string[]
local function getSortedObjectList(params)
	local list = {}

	for obj in tes3.iterateObjects(params.objectType) do
		local doAdd = true
		-- Check that all filters match
		for field, value in pairs(params.objectFilters or {}) do
			if obj[field] ~= value then
				doAdd = false
			end
		end

		if params.noScripted and obj--[[@as tes3activator]].script ~= nil then
			doAdd = false
		end

		if doAdd then
			list[#list + 1] = obj.id:lower()
		end
	end
	table.sort(list)
	return list
end

function ExclusionsPage:resetToDefault()
	if self.variable.defaultSetting == nil then
		return
	end
	-- Make sure we copy defaultSetting so that self.variable.value doesn't become the reference to this table.
	self.variable.value = table.copy(self.variable.defaultSetting)
	self.elements.outerContainer.parent:destroyChildren()
	self:create(self.elements.outerContainer.parent)
end

function ExclusionsPage:resetSearchBars()
	self.elements.searchBarInput.rightList.text = ""
	self.elements.searchBarInput.leftList.text = ""
	self.elements.searchBarInput.rightList:triggerEvent(tes3.uiEvent.keyPress)
	self.elements.searchBarInput.leftList:triggerEvent(tes3.uiEvent.keyPress)
end

--- @param e tes3uiEventData
function ExclusionsPage:toggle(e)
	-- toggle an item between blocked / allowed

	-- delete element
	local list = e.source.parent.parent.parent
	local text = e.source.text
	e.source:destroy()

	-- toggle blocked
	if list == self.elements.leftList then
		list = self.elements.rightList
		local var = self.variable.value
		var[text] = false
		self.variable.value = var
	else
		list = self.elements.leftList
		local var = self.variable.value
		var[text] = true
		self.variable.value = var
	end
	--- @cast list tes3uiElement

	-- create element
	list:createTextSelect{ id = itemID, text = text }:register(tes3.uiEvent.mouseClick, function(e)
		self:toggle(e)
	end)

	-- update sorting
	local container = list:getContentElement()
	container:sortChildren(function(a, b)
			return a.text < b.text
	end)

	-- update display
	self.elements.outerContainer:getTopLevelMenu():updateLayout()
end

--- @param listName mwseMCMExclusionsPageListId
function ExclusionsPage:updateSearch(listName)

	local searchString = self.elements.searchBarInput[listName].text:lower() --[[@as string]]
	local thisList = self.elements[listName] --[[@as tes3uiElement]]
	local child = thisList:findChild(itemID)

	if child then
		local itemList = child.parent.children
		for _, item in ipairs(itemList) do
			if item.text:lower():find(searchString, 1, true) then
				item.visible = true
			else
				item.visible = false
			end
		end
	end

	self.elements[listName].widget:contentsChanged()
	self.elements.outerContainer:getTopLevelMenu():updateLayout()
end

--- @param items string[]
function ExclusionsPage:distributeLeft(items)
	-- distribute items between blocked / allowed

	self.elements.leftList:getContentElement():destroyChildren()

	if self.showAllBlocked then
		-- show all items
		for name, blocked in pairs(self.variable.value) do
			if blocked then
				self.elements.leftList:createTextSelect{ id = itemID, text = name }:register(tes3.uiEvent.mouseClick, function(e)
					self:toggle(e)
				end)
			end
		end
	else
		for i, name in pairs(items) do
			if self.variable.value[name] then
				self.elements.leftList:createTextSelect{ id = itemID, text = name }:register(tes3.uiEvent.mouseClick, function(e)
					self:toggle(e)
				end)
			end
		end
	end
end

--- @param items string[]
function ExclusionsPage:distributeRight(items)
	-- distribute items between blocked / allowed

	self.elements.rightList:getContentElement():destroyChildren()
	for i, name in pairs(items) do
		if not self.variable.value[name] then
			self.elements.rightList:createTextSelect{ id = itemID, text = name }:register(tes3.uiEvent.mouseClick, function(e)
				self:toggle(e)
			end)
		end
	end
end

--- @param listName mwseMCMExclusionsPageListId
function ExclusionsPage:toggleFiltered(listName)
	-- Move all items currently filtered to opposite list

	local thisList = self.elements[listName] --[[@as tes3uiElement]]
	local child = thisList:findChild(itemID)

	if child then
		local itemList = child.parent.children
		for _, item in ipairs(itemList) do
			if item.visible then
				local list = item.parent.parent.parent
				if list == self.elements.leftList then
					list = self.elements.rightList
					local var = self.variable.value
					var[item.text] = false
					self.variable.value = var
				else
					list = self.elements.leftList
					local var = self.variable.value
					var[item.text] = true
					self.variable.value = var
				end
			end
		end
	end
	--destroy and recreate page
	self.elements.outerContainer.parent:destroyChildren()
	self:create(self.elements.outerContainer.parent)
end

--- @param filter tes3uiElement
function ExclusionsPage:clickFilter(filter)

	-- Turn all filters off
	for id, button in pairs(self.elements.filterList.children) do
		button.widget.state = tes3.uiState.normal
	end
	-- turn this filter back on
	filter.widget.state = tes3.uiState.active
end

-- UI creation functions

--- @param parentBlock tes3uiElement
--- @param listName mwseMCMExclusionsPageListId
function ExclusionsPage:createSearchBar(parentBlock, listName)

	local searchBlock = parentBlock:createBlock()
	searchBlock.flowDirection = tes3.flowDirection.leftToRight
	searchBlock.autoHeight = true
	searchBlock.widthProportional = 1.0
	searchBlock.borderBottom = self.indent

	local searchBar = searchBlock:createThinBorder({ id = tes3ui.registerID("ExclusionsSearchBar") })
	searchBar.autoHeight = true
	searchBar.widthProportional = 1.0

	-- Create the search input itself.
	local input = searchBar:createTextInput({ id = tes3ui.registerID("ExclusionsSearchInput") })
	input.color = tes3ui.getPalette(tes3.palette.disabledColor)
	input.text = placeholderText
	input.borderLeft = 5
	input.borderRight = 5
	input.borderTop = 2
	input.borderBottom = 4
	input.widget.eraseOnFirstKey = true
	input.consumeMouseEvents = false

	-- Set up the events to control text input control.
	input:register(tes3.uiEvent.keyPress, function(e)
		local inputController = tes3.worldController.inputController
		local pressedTab = (inputController:isKeyDown(tes3.scanCode.tab))
		local pressedDelete = (inputController:isKeyDown(tes3.scanCode.delete)) or (inputController:isKeyDown(tes3.scanCode.backspace))
		local backspacedNothing = pressedDelete and input.text == placeholderText
		if pressedTab then
			-- Prevent alt-tabbing from creating spacing.
			return
		elseif backspacedNothing then
			-- Prevent backspacing into nothing.
			return
		end

		input:forwardEvent(e)

		input.color = tes3ui.getPalette(tes3.palette.normalColor)
		self:updateSearch(listName)
		input:updateLayout()
		if input.text == "" then
			input.text = placeholderText
			input.color = tes3ui.getPalette(tes3.palette.disabledColor)
		end
	end)

	-- Pressing enter applies toggle to all items currenty filtered
	input:register(tes3.uiEvent.keyEnter, function()
		self:toggleFiltered(listName)
	end)

	searchBar:register(tes3.uiEvent.mouseClick, function()
		tes3ui.acquireTextInput(input)
	end)

	-- Add button to exclude all currently filtered items
	local toggleButton = searchBlock:createButton({ text = self.toggleText })
	toggleButton.heightProportional = 1.0
	-- toggleButton.alignY = 0.0
	toggleButton.borderAllSides = 0
	toggleButton.paddingAllSides = 2
	toggleButton:register(tes3.uiEvent.mouseClick, function()
		self:toggleFiltered(listName)
	end)

	-- Set a table to contain both search bars
	self.elements.searchBar = self.elements.searchBar or {}
	self.elements.searchBarInput = self.elements.searchBarInput or {}
	self.elements.searchBar[listName] = searchBar
	self.elements.searchBarInput[listName] = input

end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createResetButtonContainer(parentBlock)
	local grow = parentBlock:createBlock({ id = tes3ui.registerID("Reset_LeftGrow") })
	grow.autoWidth = true
	grow.autoHeight = true

	local resetContainer = parentBlock:createBlock({ id = tes3ui.registerID("Reset_InnerContainer") })
	resetContainer.flowDirection = tes3.flowDirection.leftToRight
	resetContainer.autoWidth = true
	resetContainer.autoHeight = true
	resetContainer.widthProportional = 1.0
	resetContainer.childAlignX = 1.0
	self.elements.resetContainer = resetContainer
end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createFiltersSection(parentBlock)

	local block = parentBlock:createBlock{}
	block.flowDirection = tes3.flowDirection.topToBottom
	block.autoWidth = true
	block.heightProportional = 1.0
	block.borderTop = 13
	block.borderLeft = self.indent
	block.borderRight = self.indent

	local filterList = block:createBlock{ id = tes3ui.registerID("FilterList") }
	filterList.flowDirection = tes3.flowDirection.topToBottom
	filterList.autoWidth = true
	filterList.heightProportional = 1.0

	filterList.borderTop = 3

	-- Add buttons for each filter
	for _, filter in ipairs(self.filters) do
		local button = filterList:createButton{ text = filter.label }
		button.widthProportional = 1.0
		button.borderBottom = 5
		-- get callback
		local getItemsCallback
		if filter.type == "Plugin" then
			getItemsCallback = getSortedModList
		elseif filter.type == "Object" then
			getItemsCallback = (function()
				return getSortedObjectList({
					objectType = filter.objectType,
					objectFilters = filter.objectFilters,
					noScripted = filter.noScripted,
				})
			end)
		else
			-- No type defined, must be custom
			if not filter.callback then
				mwse.log("ERROR: no custom callback defined for %s", self.label)
			end
			getItemsCallback = filter.callback
		end

		-- Register clicking filter button
		button:register(tes3.uiEvent.mouseClick, function(e)
			local items = getItemsCallback()
			self:clickFilter(button)
			self:distributeLeft(items)
			self:distributeRight(items)
			self:resetSearchBars()
		end)

	end

	if #self.filters <= 1 then
		filterList.visible = false
	end

	self.elements.filterList = filterList
end

--- @alias mwseMCMExclusionsPageListId
---| "leftList"
---| "rightList"

--- @param parentBlock tes3uiElement
--- @param listId mwseMCMExclusionsPageListId
function ExclusionsPage:createList(parentBlock, listId)

	if listId ~= "leftList" and listId ~= "rightList" then
		mwse.log("ERROR: param 2 of createList must be 'leftList' or 'rightList'.")
		return
	end
	local labelId = (listId .. "Label") --[[@as "leftListLabel"|"rightListLabel"]]

	local block = parentBlock:createBlock{}
	block.flowDirection = tes3.flowDirection.topToBottom
	block.widthProportional = 1.0
	block.heightProportional = 1.0

	local labelText = (self[labelId] .. ":")
	local label = block:createLabel{ text = labelText }
	label.borderBottom = 2
	label.color = tes3ui.getPalette(tes3.palette.headerColor)

	self:createSearchBar(block, listId)

	-- Create actual list
	local list = block:createVerticalScrollPane{}
	list.widthProportional = 1.0
	list.heightProportional = 1.0
	list.paddingLeft = 8
	self.elements[listId] = list

	if self.showReset and listId == "leftList" then
		self:createResetButtonContainer(block)
		self:createResetButton(self.elements.resetContainer)
	end

end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createOuterContainer(parentBlock)
	local outerContainer = parentBlock:createThinBorder({ id = tes3ui.registerID("Category_OuterContainer") })
	outerContainer.flowDirection = tes3.flowDirection.topToBottom
	outerContainer.widthProportional = 1.0
	outerContainer.heightProportional = 1.0

	-- VerticalScrollPanes add 4 padding
	-- Because we are using a thinBorder, we match it here
	outerContainer.paddingLeft = 4 + self.indent
	outerContainer.paddingRight = 4 + self.indent
	outerContainer.paddingBottom = 4
	outerContainer.paddingTop = self.indent + 4
	self.elements.outerContainer = outerContainer
end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createLabel(parentBlock)
	Parent.createLabel(self, parentBlock)
	if self.elements.label then
		self.elements.label.color = tes3ui.getPalette(tes3.palette.headerColor)
	end
end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createDescription(parentBlock)
	if not self.description then
		return
	end

	local description = parentBlock:createLabel{ text = self.description }
	-- description.heightProportional = -1
	description.autoHeight = true
	description.widthProportional = 1.0
	description.wrapText = true
	description.borderLeft = self.indent
	description.borderRight = self.indent
	self.elements.description = description
end

--- @param parentBlock tes3uiElement
function ExclusionsPage:createSections(parentBlock)
	local sections = parentBlock:createBlock{}
	sections.flowDirection = tes3.flowDirection.leftToRight
	sections.widthProportional = 1.0
	sections.heightProportional = 1.0
	sections.paddingAllSides = self.indent
	self.elements.sections = sections
end

--- @param parentBlock tes3uiElement
function ExclusionsPage:create(parentBlock)
	self.elements = {}
	self.mouseOvers = {}

	self:createOuterContainer(parentBlock)
	self:createLabel(self.elements.outerContainer)
	self:createDescription(self.elements.outerContainer)
	self:createSections(self.elements.outerContainer)
	self:createList(self.elements.sections, "leftList")
	self:createFiltersSection(self.elements.sections)
	self:createList(self.elements.sections, "rightList")

	-- default to first filter
	self.elements.filterList.children[1]:triggerEvent(tes3.uiEvent.mouseClick)
end

return ExclusionsPage
