


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
local function updateArrowVisibility(widget)
	if (not widget.rawdata.showArrows) then
		return
	end

	local visible = #widget:getTabsBlock().children > 1
	widget.rawdata.previousArrow.visible = visible
	widget.rawdata.nextArrow.visible = visible
end

--- @param widget tes3uiTabContainer
--- @param tab tes3uiElement
local function scrollToTab(widget, tab)
	if (not widget.rawdata.showArrows) then
		return
	end

	local scrollPane = widget.rawdata.tabsScrollPane
	if (not scrollPane) then
		return
	end

	scrollPane.widget:contentsChanged()
	scrollPane.widget:scrollIntoView(tab)
end

--- @param widget tes3uiTabContainer
--- @param direction number
local function scrollTabPage(widget, direction)
	local scrollPane = widget.rawdata.tabsScrollPane
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
	local scrollPane = widget.rawdata.tabsScrollPane
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

function metatable:set_currentTab(id)
	local tabs = self:getTabsBlock()
	local tab = tabs:findChild(string.format("Tab:%s", id))
	if not tab then
		error(string.format("No tab with the given ID '%s' exists.", id))
	end

	local contents = self:getContentsBlock()
	local content = contents:findChild(string.format("TabContents:%s", id))
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

	self.rawdata.currentTab = id

	self.element:updateLayout()
	scrollToTab(self, tab)
end

--- @param e tes3uiEventData
local function onClickTab(e)
	--- @type tes3uiTabContainer
	local widget = e.source:getLuaData("MWSE:TabContainerWidget")
	widget.currentTab = e.source:getLuaData("MWSE:TabID")
	widget.element:triggerEvent(tes3.uiEvent.valueChanged)
end

function metatable:addTab(params)
	assert(type(params) == "table", "Invalid parameters provided.")
	assert(params.id, "A tab must be given an ID.")
	params.name = params.name or params.id

	local tabs = self:getTabsBlock()
	if (tabs:findChild(string.format("Tab:%s", params.id))) then
		error(string.format("A tab with the ID '%s' already exists.", params.id))
	end

	local tab = tabs:createButton({ id = string.format("Tab:%s", params.id), text = params.name })
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
	local block = contents:createBlock({ id = string.format("TabContents:%s", params.id) })
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

	updateArrowVisibility(self)

	return block
end

function metatable:nextTab()
	local tabsElement = self:getTabsBlock()
	local tabsChildren = tabsElement.children

	local currentTabElement = tabsElement:findChild(string.format("Tab:%s", self.currentTab))
	local currentIndex = table.find(tabsChildren, currentTabElement)

	local nextIndex = currentIndex + 1
	if (nextIndex > #tabsChildren) then
		nextIndex = 1
	end

	local nextId = tabsChildren[nextIndex]:getLuaData("MWSE:TabID")
	self.currentTab = nextId
end

function metatable:previousTab()
	local tabsElement = self:getTabsBlock()
	local tabsChildren = tabsElement.children

	local currentTabElement = tabsElement:findChild(string.format("Tab:%s", self.currentTab))
	local currentIndex = table.find(tabsChildren, currentTabElement)

	local nextIndex = currentIndex - 1
	if (nextIndex < 1) then
		nextIndex = #tabsChildren
	end

	local nextId = tabsChildren[nextIndex]:getLuaData("MWSE:TabID")
	self.currentTab = nextId
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

	local widget = element:makeLuaWidget("tabContainer", { rawdata = { element = element, tabs = tabsBlock, tabsScrollPane = tabsScrollPane, contents = contentsBlock, showArrows = params.showArrows, previousArrow = previousArrow, nextArrow = nextArrow } })

	for _, scrollWheelElement in ipairs({ tabsScrollPane, tabsContent, tabsBlock }) do
		scrollWheelElement:register(tes3.uiEvent.mouseScrollDown, function()
			scrollToAdjacentTab(widget, 1)
		end)
		scrollWheelElement:register(tes3.uiEvent.mouseScrollUp, function()
			scrollToAdjacentTab(widget, -1)
		end)
	end

	if (params.showArrows) then
		previousArrow.visible = false
		nextArrow.visible = false

		nextArrow:register(tes3.uiEvent.mouseClick, function()
			scrollTabPage(widget, 1)
		end)

		previousArrow:register(tes3.uiEvent.mouseClick, function()
			scrollTabPage(widget, -1)
		end)

		element:registerAfter(tes3.uiEvent.update, function()
			updateArrowVisibility(widget)
			tabsScrollPane.widget:contentsChanged()
		end)
	end

	return element
end

tes3ui.defineLuaWidget({
	name = "tabContainer",
	metatable = metatable,
})
