
--
-- Widget metatable.
--

local metatable = {}

local arrowPrevious = {
	id = "MWSE:TabContainer:PreviousButton",
	idle = "textures/mwse/menu_arrow_prev.tga",
	over = "textures/mwse/menu_arrow_prev.tga",
	pressed = "textures/mwse/menu_arrow_prev_pressed.tga",
}

local arrowNext = {
	id = "MWSE:TabContainer:NextButton",
	idle = "textures/mwse/menu_arrow_next.tga",
	over = "textures/mwse/menu_arrow_next.tga",
	pressed = "textures/mwse/menu_arrow_next_pressed.tga",
}

--- @class tes3uiTabContainerRawData
--- @field contents tes3uiElement
--- @field currentTab string
--- @field element tes3uiElement
--- @field nextArrow tes3uiElement
--- @field previousArrow tes3uiElement
--- @field showArrows boolean
--- @field tabs tes3uiElement
--- @field tabsScrollPane tes3uiElement

--- @param widget tes3uiTabContainer
--- @return tes3uiTabContainerRawData
local function getRawData(widget)
	return widget.rawdata --[[@as tes3uiTabContainerRawData]]
end

--- @param element tes3uiElement
--- @param imageParams table
--- @return tes3uiElement
local function createArrow(element, imageParams)
	local arrow = element:createImageButton(imageParams)
	arrow.height = 32
	arrow.childOffsetY = 2
	return arrow
end

--- @param widget tes3uiTabContainer
--- @param tab tes3uiElement
local function scrollToTab(widget, tab)
	local rawdata = getRawData(widget)
	if (not rawdata.showArrows) then
		return
	end

	local scrollPane = rawdata.tabsScrollPane
	if (not scrollPane) then
		return
	end

	scrollPane.widget:contentsChanged()
	scrollPane.widget:scrollIntoView(tab)
end

--- @param widget tes3uiTabContainer
--- @param direction number
local function scrollTabPage(widget, direction)
	local rawdata = getRawData(widget)
	local scrollPane = rawdata.tabsScrollPane
	if (not scrollPane) then
		return
	end

	widget.element:updateLayout()
	scrollPane.widget:contentsChanged()

	local content = scrollPane:getContentElement()
	local pageWidth = scrollPane.width
	local maxPosition = math.max(0, content.width - pageWidth)
	local currentPosition = scrollPane.widget.positionX or 0

	scrollPane.widget.positionX = math.clamp(currentPosition + pageWidth * direction, 0, maxPosition)
end

--- @param widget tes3uiTabContainer
--- @param direction number
local function scrollToAdjacentTab(widget, direction)
	local rawdata = getRawData(widget)
	local scrollPane = rawdata.tabsScrollPane
	if (not scrollPane) then
		return
	end

	widget.element:updateLayout()
	scrollPane.widget:contentsChanged()

	local currentPosition = scrollPane.widget.positionX or 0
	local viewEnd = currentPosition + scrollPane.width
	local tabs = widget:getTabsBlock().children

	if (direction > 0) then
		for _, tab in ipairs(tabs) do
			if ((tab.positionX + tab.width) > viewEnd) then
				scrollPane.widget:scrollIntoView(tab)
				return
			end
		end
	else
		for i = #tabs, 1, -1 do
			local tab = tabs[i]
			if (tab.positionX < currentPosition) then
				scrollPane.widget:scrollIntoView(tab)
				return
			end
		end
	end
end

function metatable:__index(key)
	-- First look for functions defined on the metatable.
	local method = metatable[key]
	if (method) then
		return method
	end

	-- Otherwise look for a get function.
	local getter = metatable["get_" .. key]
	if (getter) then
		return getter(self)
	end

	error(string.format("Invalid access to property '%s'. This property does not exist.", key))
end

function metatable:__newindex(key, value)
	-- Look for a setter function.
	local setter = metatable["set_" .. key]
	if (setter) then
		return setter(self, value)
	end

	error(string.format("Invalid access to property '%s'. This property is read-only.", key), 2)
end

function metatable:get_currentTab()
	return self.rawdata.currentTab
end

--- @param id string
--- @return string
local function getTabElementId(id)
	return string.format("Tab:%s", id)
end

--- @param id string
--- @return string
local function getTabContentsElementId(id)
	return string.format("TabContents:%s", id)
end

--- @param widget tes3uiTabContainer
--- @param id string
--- @return tes3uiElement?
local function getTab(widget, id)
	return widget:getTabsBlock():findChild(getTabElementId(id))
end

--- @param widget tes3uiTabContainer
--- @param id string
--- @return tes3uiElement?
local function getTabContents(widget, id)
	return widget:getContentsBlock():findChild(getTabContentsElementId(id))
end

--- @param tab tes3uiElement
--- @return boolean
local function isTabSelectable(tab)
	return tab.visible and not tab.disabled
end

--- @param widget tes3uiTabContainer
--- @param startIndex number
--- @return tes3uiElement?
local function getNextSelectableTab(widget, startIndex)
	local tabsChildren = widget:getTabsBlock().children
	if (#tabsChildren == 0) then
		return nil
	end

	for offset = 0, #tabsChildren - 1 do
		local index = ((startIndex + offset - 1) % #tabsChildren) + 1
		local tab = tabsChildren[index]
		if (isTabSelectable(tab)) then
			return tab
		end
	end
end

--- @param widget tes3uiTabContainer
local function clearCurrentTab(widget)
	for _, content in ipairs(widget:getContentsBlock().children) do
		if (content.visible) then
			content:triggerEvent(tes3.uiEvent.tabUnfocus)
		end
		content.visible = false
	end

	getRawData(widget).currentTab = nil
	widget.element:updateLayout()
end

--- @param widget tes3uiTabContainer
--- @param id string
--- @return boolean changed
local function setCurrentTab(widget, id)
	local rawdata = getRawData(widget)
	if (rawdata.currentTab == id) then
		return false
	end

	local tabs = widget:getTabsBlock()
	local tab = getTab(widget, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end
	if (not isTabSelectable(tab)) then
		error(string.format("Tab with ID '%s' is not selectable.", id))
	end

	local contents = widget:getContentsBlock()
	local content = getTabContents(widget, id)
	if not content then
		error(string.format("No contents for tab with the given ID '%s' exists.", id))
	end

	for _, t in ipairs(tabs.children) do
		if (t == tab) then
			t.widget.textElement.color = tes3ui.getPalette(tes3.palette.normalColor)
			t.widget.idle = tes3ui.getPalette(tes3.palette.normalColor)
		else
			t.widget.textElement.color = tes3ui.getPalette(tes3.palette.disabledColor)
			t.widget.idle = tes3ui.getPalette(tes3.palette.disabledColor)
		end
		t:updateLayout()
	end

	for _, c in ipairs(contents.children) do
		if not c.visible and c == content then
			c:triggerEvent(tes3.uiEvent.tabFocus)
		end
		if c.visible and c ~= content then
			c:triggerEvent(tes3.uiEvent.tabUnfocus)
		end
		c.visible = (c == content)
	end

	rawdata.currentTab = id

	widget.element:updateLayout()
	scrollToTab(widget, tab)

	return true
end

function metatable:set_currentTab(id)
	setCurrentTab(self, id)
end

--- @param widget tes3uiTabContainer
--- @param preferredPosition number
--- @return boolean changed
local function selectAvailableTab(widget, preferredPosition)
	local tab = getNextSelectableTab(widget, preferredPosition)
	if (not tab) then
		local changed = getRawData(widget).currentTab ~= nil
		clearCurrentTab(widget)
		return changed
	end

	return setCurrentTab(widget, tab:getLuaData("MWSE:TabID"))
end

--- @param e tes3uiEventData
local function onClickTab(e)
	--- @type tes3uiTabContainer
	local widget = e.source:getLuaData("MWSE:TabContainerWidget")
	if (not isTabSelectable(e.source)) then
		return
	end
	if (setCurrentTab(widget, e.source:getLuaData("MWSE:TabID"))) then
		widget.element:triggerEvent(tes3.uiEvent.valueChanged)
	end
end

function metatable:getTab(id)
	return getTab(self, id)
end

function metatable:getTabContents(id)
	return getTabContents(self, id)
end

function metatable:addTab(params)
	assert(type(params) == "table", "Invalid parameters provided.")
	assert(params.id, "A tab must be given an ID.")
	params.name = params.name or params.id

	local tabs = self:getTabsBlock()
	if (getTab(self, params.id)) then
		error(string.format("A tab with the ID '%s' already exists.", params.id))
	end

	local tab = tabs:createButton({ id = getTabElementId(params.id), text = params.name })
	tab.widget.textElement.color = tes3ui.getPalette(tes3.palette.disabledColor)
	tab.widget.idle = tes3ui.getPalette(tes3.palette.disabledColor)
	tab:setLuaData("MWSE:TabID", params.id)
	tab:setLuaData("MWSE:TabContainerWidget", self)
	tab:registerAfter(tes3.uiEvent.mouseClick, onClickTab)
	tab:register(tes3.uiEvent.mouseScrollDown, function()
		scrollToAdjacentTab(self, 1)
	end)
	tab:register(tes3.uiEvent.mouseScrollUp, function()
		scrollToAdjacentTab(self, -1)
	end)

	local contents = self:getContentsBlock()
	local block = contents:createBlock({ id = getTabContentsElementId(params.id) })
	block:setLuaData("MWSE:TabID", params.id)
	block.widthProportional = 1.0
	block.heightProportional = 1.0
	block.visible = false

	if (self.currentTab == nil) then
		tab.widget.textElement.color = tes3ui.getPalette(tes3.palette.normalColor)
		tab.widget.idle = tes3ui.getPalette(tes3.palette.normalColor)
		block.visible = true
		self.rawdata.currentTab = params.id
	end

	return block
end

function metatable:removeTab(id)
	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	local content = getTabContents(self, id)
	if not content then
		error(string.format("No contents for tab with the given ID '%s' exists.", id))
	end

	local position = self:getTabPosition(id)
	local wasCurrent = self.currentTab == id
	if (wasCurrent and content.visible) then
		content:triggerEvent(tes3.uiEvent.tabUnfocus)
	end
	tab:destroy()
	content:destroy()

	local changed = false
	if (wasCurrent) then
		changed = selectAvailableTab(self, position)
	end

	self.element:updateLayout()
	if (changed) then
		self.element:triggerEvent(tes3.uiEvent.valueChanged)
	end
end

function metatable:getTabPosition(id)
	local tab = getTab(self, id)
	if not tab then
		return nil
	end

	for index, child in ipairs(self:getTabsBlock().children) do
		if (child == tab) then
			return index
		end
	end
end

function metatable:setTabPosition(id, position)
	assert(type(position) == "number", "Invalid tab position provided.")
	assert(position == math.floor(position), "Invalid tab position provided.")

	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	local content = getTabContents(self, id)
	if not content then
		error(string.format("No contents for tab with the given ID '%s' exists.", id))
	end

	local tabsChildren = self:getTabsBlock().children
	local contentsChildren = self:getContentsBlock().children
	assert(position >= 1 and position <= #tabsChildren, "Invalid tab position provided.")

	local currentPosition = self:getTabPosition(id)
	if (currentPosition == position) then
		return
	end

	local beforeTab = tabsChildren[position]
	local beforeContent = contentsChildren[position]
	if (currentPosition and currentPosition < position) then
		beforeTab = tabsChildren[position + 1]
		beforeContent = contentsChildren[position + 1]
	end

	if (beforeTab) then
		tab:reorder({ before = beforeTab })
	else
		tab:reorder({ after = tabsChildren[#tabsChildren] })
	end

	if (beforeContent) then
		content:reorder({ before = beforeContent })
	else
		content:reorder({ after = contentsChildren[#contentsChildren] })
	end

	self.element:updateLayout()
end

function metatable:getTabHidden(id)
	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	return not tab.visible
end

function metatable:setTabHidden(id, hidden)
	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	hidden = hidden == true
	if ((not tab.visible) == hidden) then
		return
	end

	local position = self:getTabPosition(id)
	tab.visible = not hidden

	local changed = false
	if (hidden and self.currentTab == id) then
		changed = selectAvailableTab(self, position)
	elseif ((not hidden) and self.currentTab == nil and not tab.disabled) then
		changed = setCurrentTab(self, id)
	end

	self.element:updateLayout()
	if (changed) then
		self.element:triggerEvent(tes3.uiEvent.valueChanged)
	end
end

function metatable:getTabDisabled(id)
	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	return tab.disabled == true
end

function metatable:setTabDisabled(id, disabled)
	local tab = getTab(self, id)
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	disabled = disabled == true
	if ((tab.disabled == true) == disabled) then
		return
	end

	local position = self:getTabPosition(id)
	tab.disabled = disabled

	local changed = false
	if (disabled and self.currentTab == id) then
		changed = selectAvailableTab(self, position)
	elseif ((not disabled) and self.currentTab == nil and tab.visible) then
		changed = setCurrentTab(self, id)
	end

	self.element:updateLayout()
	if (changed) then
		self.element:triggerEvent(tes3.uiEvent.valueChanged)
	end
end

--- @param widget tes3uiTabContainer
--- @param direction number
local function advanceTab(widget, direction)
	local tabsElement = widget:getTabsBlock()
	local tabsChildren = tabsElement.children
	if (#tabsChildren == 0) then
		return
	end

	local currentTabElement
	if (widget.currentTab) then
		currentTabElement = getTab(widget, widget.currentTab)
	end
	local currentIndex = currentTabElement and table.find(tabsChildren, currentTabElement) or 0
	if (currentIndex == 0 and direction < 0) then
		currentIndex = 1
	end

	for offset = 1, #tabsChildren do
		local nextIndex = currentIndex + offset * direction
		while (nextIndex > #tabsChildren) do
			nextIndex = nextIndex - #tabsChildren
		end
		while (nextIndex < 1) do
			nextIndex = nextIndex + #tabsChildren
		end

		local nextTab = tabsChildren[nextIndex]
		if (isTabSelectable(nextTab)) then
			local nextId = nextTab:getLuaData("MWSE:TabID")
			if (setCurrentTab(widget, nextId)) then
				widget.element:triggerEvent(tes3.uiEvent.valueChanged)
			end
			return
		end
	end
end

function metatable:nextTab()
	advanceTab(self, 1)
end

function metatable:previousTab()
	advanceTab(self, -1)
end

--- @return tes3uiElement
function metatable:get_element()
	return self.rawdata.element
end

--- @return tes3uiElement
function metatable:getTabsBlock()
	return self.rawdata.tabs
end

--- @return tes3uiElement
function metatable:getContentsBlock()
	return self.rawdata.contents
end


--
-- Base element creation.
--

--- @diagnostic disable-next-line
function tes3uiElement:createTabContainer(params)
	-- Validate params.
	assert(type(params) == "table", "Invalid parameters provided.")
	params.showArrows = table.get(params, "showArrows", false) ~= false

	local element = self:createBlock({ id = params.id })
	element.flowDirection = tes3.flowDirection.topToBottom

	local outerTabsBlock = element:createBlock({ id = element.name and string.format("%s:OuterTabContainer", element.name) or "MWSE:OuterTabContainer" })
	outerTabsBlock.widthProportional = 1.0
	outerTabsBlock.autoHeight = true
	outerTabsBlock.flowDirection = tes3.flowDirection.leftToRight

	local previousArrow
	if (params.showArrows) then
		previousArrow = createArrow(outerTabsBlock, arrowPrevious)
	end

	local tabsScrollPane = outerTabsBlock:createHorizontalScrollPane({ id = element.name and string.format("%s:TabScrollPane", element.name) or "MWSE:TabScrollPane" })
	tabsScrollPane.contentPath = nil
	tabsScrollPane.paddingAllSides = 0
	tabsScrollPane.widthProportional = 1.0
	tabsScrollPane.autoHeight = true
	tabsScrollPane.minHeight = 32
	tabsScrollPane.widget.scrollbarVisible = false
	if (tabsScrollPane.widget.horizontalScrollBar) then
		tabsScrollPane.widget.horizontalScrollBar.visible = false
	end

	local tabsContent = tabsScrollPane:getContentElement()
	tabsContent.flowDirection = tes3.flowDirection.leftToRight
	tabsContent.autoWidth = true
	tabsContent.autoHeight = true

	local tabsBlock = tabsContent:createBlock({ id = element.name and string.format("%s:TabContainer", element.name) or "MWSE:TabContainer" })
	tabsBlock.flowDirection = tes3.flowDirection.leftToRight
	tabsBlock.autoWidth = true
	tabsBlock.autoHeight = true

	local nextArrow
	if (params.showArrows) then
		nextArrow = createArrow(outerTabsBlock, arrowNext)
	end

	local contentsBlock = element:createBlock({ id = element.name and string.format("%s:TabContentsContainer", element.name) or "MWSE:TabContentsContainer" })
	contentsBlock.heightProportional = 1.0
	contentsBlock.widthProportional = 1.0
	contentsBlock.flowDirection = tes3.flowDirection.topToBottom

	local widget = element:makeLuaWidget("tabContainer", {
		rawdata = {
			element = element,
			tabs = tabsBlock,
			tabsScrollPane = tabsScrollPane,
			contents = contentsBlock,
			showArrows = params.showArrows,
			previousArrow = previousArrow,
			nextArrow = nextArrow
		},
	})

	for _, scrollWheelElement in ipairs({ tabsScrollPane, tabsContent, tabsBlock }) do
		scrollWheelElement:register(tes3.uiEvent.mouseScrollDown, function()
			scrollToAdjacentTab(widget, 1)
		end)
		scrollWheelElement:register(tes3.uiEvent.mouseScrollUp, function()
			scrollToAdjacentTab(widget, -1)
		end)
	end

	if (params.showArrows) then
		nextArrow:register(tes3.uiEvent.mouseClick, function()
			scrollTabPage(widget, 1)
		end)

		previousArrow:register(tes3.uiEvent.mouseClick, function()
			scrollTabPage(widget, -1)
		end)
	end

	return element
end

tes3ui.defineLuaWidget({
	name = "tabContainer",
	metatable = metatable,
})
