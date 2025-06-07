


--
-- Widget metatable.
--

local metatable = {}

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
	assert(tab, string.format("No tab with the given ID '%s' exists.", id))

	local contents = self:getContentsBlock()
	local content = contents:findChild(string.format("TabContents:%s", id))
	assert(content, string.format("No contents for tab with the given ID '%s' exists.", id))

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
end

--- @param e tes3uiEventData
local function onClickTab(e)
	--- @type tes3uiTabContainer
	local widget = e.source.parent.parent.widget
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
	tab:registerAfter(tes3.uiEvent.mouseClick, onClickTab)

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

	local element = self:createBlock({ id = params.id })
	element.flowDirection = tes3.flowDirection.topToBottom

	local tabsBlock = element:createBlock({ id = element.name and string.format("%s:TabContainer", element.name) or "MWSE:TabContainer" })
	tabsBlock.widthProportional = 1.0
	tabsBlock.childAlignX = 0.5
	tabsBlock.autoHeight = true
	tabsBlock.flowDirection = tes3.flowDirection.leftToRight

	local contentsBlock = element:createBlock({ id = element.name and string.format("%s:TabContentsContainer", element.name) or "MWSE:TabContentsContainer" })
	contentsBlock.heightProportional = 1.0
	contentsBlock.widthProportional = 1.0
	contentsBlock.flowDirection = tes3.flowDirection.topToBottom

	element:makeLuaWidget("tabContainer", { rawdata = { element = element, tabs = tabsBlock, contents = contentsBlock } })

	return element
end

tes3ui.defineLuaWidget({
	name = "tabContainer",
	metatable = metatable,
})
