--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local utils = require("mcm.utils")

local Parent = require("mcm.components.Component")

--- Class object
--- @class mwseMCMTemplate
local Template = Parent:new()

Template.componentType = "Template"

--- @param data mwseMCMTemplate.new.data
--- @return mwseMCMTemplate template
function Template:new(data)
	data.name = data.name or data.label
	local t = Parent:new(data)
	setmetatable(t, self)

	-- Create Pages
	local pages = {}
	t.pages = t.pages or {}
	for _, page in ipairs(t.pages) do
		-- Make sure it's actually a `Page`.
		if not page.componentType then
			local componentClass = utils.getComponentClass(page.class or "Page")
			if not componentClass then
				error(string.format("Could not intialize page %q", page.label))
			end
			page.parentComponent = self
			page = componentClass:new(page)
		end
		table.insert(pages, page)
	end
	t.pages = pages

	self.__index = Template.__index
	return t --[[@as mwseMCMTemplate]]
end

--- @param fileName string
--- @param config unknown
function Template:saveOnClose(fileName, config)
	self.onClose = function()
		mwse.saveConfig(fileName, config)
	end
end

--- This can be replaced with fuzzy or wildcard matching
--- @param str string
--- @param substr string
local function ciContains(str, substr)
	return (str:lower():find(substr, 1, true)) and true or false
end

--- @param searchText string
--- @param component mwseMCMCategory|mwseMCMSideBarPage|mwseMCMSetting|mwseMCMInfo
--- @param fields string[]
--- @return boolean?
local function searchFields(searchText, component, fields)
	for _, key in ipairs(fields) do
		local text = component[key]
		if text and ciContains(text, searchText) then
			return true
		end
	end
end

--- Recursively iterates over all the subcomponents and returns true if searchText
--- matches a label or description of a setting
--- @param searchText string
--- @param component mwseMCMCategory|mwseMCMSideBarPage|mwseMCMSetting|mwseMCMInfo
--- @param searchLabels boolean
--- @param searchDescriptions boolean
--- @return boolean?
local function searchComponentRecursive(searchText, component, searchLabels, searchDescriptions)
	-- Most components have a label. Infos and Hyperlinks have self.text.
	if searchLabels and searchFields(searchText, component, { "label", "text" }) then
		return true
	end

	-- Most components have a description.
	if searchDescriptions and searchFields(searchText, component, { "description" }) then
		return true
	end

	-- Search through the settings on each page or nested category
	for _, subcomp in ipairs(component.components or {}) do
		if searchComponentRecursive(searchText, subcomp, searchLabels, searchDescriptions) then
			return true
		end
	end

	-- Search default description in SidebarPage
	local sidebar = component.sidebar
	if sidebar and searchComponentRecursive(searchText, sidebar, searchLabels, searchDescriptions) then
		return true
	end

	-- Backwards compatibility for mods using `sidebarComponents` in SidebarPages
	for _, subcomp in ipairs(component.sidebarComponents or {}) do
		--- @cast subcomp mwseMCMSetting
		if searchComponentRecursive(searchText, subcomp, searchLabels, searchDescriptions) then
			return true
		end
	end
end

--- @param searchText string
--- @return boolean result
function Template:onSearchInternal(searchText)
	local searchLabels = table.get(self, "searchChildLabels", true) --[[@as boolean]]
	local searchDescriptions = table.get(self, "searchChildDescriptions", true) --[[@as boolean]]

	-- Go through and search children.
	if (searchLabels or searchDescriptions) then
		for _, page in ipairs(self.pages) do
			if searchComponentRecursive(searchText, page, searchLabels, searchDescriptions) then
				return true
			end
		end
	end

	-- Do we have a custom search handler?
	if (self.onSearch and self.onSearch(searchText)) then
		return true
	end

	return false
end

--- @param callback nil|fun(searchText: string): boolean
function Template:setCustomSearchHandler(callback)
	self.onSearch = callback
end

--- @param parentBlock tes3uiElement
function Template:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.heightProportional = 1.0
	self.elements.outerContainer.paddingBottom = 0
	self.elements.outerContainer.paddingLeft = 0
	self.elements.outerContainer.paddingRight = 0
end

--- @param parentBlock tes3uiElement
function Template:createLabel(parentBlock)
	-- header image
	local headerBlock = parentBlock:createBlock()
	headerBlock.autoHeight = true
	headerBlock.widthProportional = 1.0
	local imagePath = self.headerImagePath
	if imagePath then
		local headerImage = headerBlock:createImage({ path = imagePath })
		headerImage.absolutePosAlignX = 0.5
		headerImage.autoHeight = true
		headerImage.widthProportional = 1.0
		headerImage.imageScaleX = 0.5
		headerImage.imageScaleY = 0.5
	elseif self.label then
		headerBlock.borderAllSides = 10
		local title = headerBlock:createLabel({ text = self.label })
		title.color = tes3ui.getPalette(tes3.palette.headerColor)
	end

end

--- @param thisPage mwseMCMExclusionsPage|mwseMCMFilterPage|mwseMCMMouseOverPage|mwseMCMPage|mwseMCMSideBarPage
function Template:clickTab(thisPage)
	local pageBlock = self.elements.pageBlock
	
	-- Clear previous page
	pageBlock:destroyChildren()
	-- Create new page
	thisPage:create(pageBlock)
	-- Set new page to current
	self.currentPage = thisPage

	local tabsBlock = self.elements.tabsBlock

	-- Update the tabs (if there are any)
	if tabsBlock then
		-- Disable tabs and tally width
		local totalWidth = 0
		for _id, page in pairs(self.pages) do
			local tab = tabsBlock:findChild(page.tabUID)
			tab.widget.state =  tes3.uiState.normal
			totalWidth = totalWidth + tab.width
		end
		-- Enable tab for this page
		local tab = tabsBlock:findChild(thisPage.tabUID)
		tab.widget.state = tes3.uiState.active

		-- Ensure tabs are visible.
		tabsBlock.childOffsetX = 0
		tabsBlock:updateLayout()
		tabsBlock.childOffsetX = math.clamp(-tab.positionX, -totalWidth + tabsBlock.width, 0)

		self:padTabBlock()
	end

	-- update view
	pageBlock:getTopLevelMenu():updateLayout()
end

--- @param button tes3uiElement
local function formatTabButton(button)
	button.borderAllSides = 0
	button.paddingTop = 4
	button.paddingLeft = 8
	button.paddingRight = 8
	button.paddingBottom = 6
end

local arrowPrevious = {
	id = "MCM_PreviousButton",
	idle = "textures/mwse/menu_arrow_prev.tga",
	over = "textures/mwse/menu_arrow_prev.tga",
	pressed = "textures/mwse/menu_arrow_prev_pressed.tga",
}

local arrowNext = {
	id = "MCM_NextButton",
	idle = "textures/mwse/menu_arrow_next.tga",
	over = "textures/mwse/menu_arrow_next.tga",
	pressed = "textures/mwse/menu_arrow_next_pressed.tga",
}

--- @param element tes3uiElement
local function createArrow(element, imageParams)
	local arrow = element:createImageButton(imageParams)
	arrow.height = 32
	arrow.childOffsetY = 2
	return arrow
end

--- @param page mwseMCMExclusionsPage|mwseMCMFilterPage|mwseMCMMouseOverPage|mwseMCMPage|mwseMCMSideBarPage
function Template:createTab(page)
	local button = self.elements.tabsBlock:createButton({ id = page.tabUID, text = page.label })
	formatTabButton(button)
	button:register(tes3.uiEvent.mouseClick, function()
		self:clickTab(page)
	end)
end

function Template:padTabBlock()
	local totalWidth = 0
	for _, page in pairs(self.pages) do
		local tab = self.elements.tabsBlock:findChild(page.tabUID)
		totalWidth = totalWidth + tab.width
	end

	self.elements.tabsBlock.borderRight = self.elements.tabsBlock.parent.width - totalWidth
	self.elements.tabsBlock.parent:updateLayout()
end

--- @param parentBlock tes3uiElement
function Template:createTabsBlock(parentBlock)
	local outerTabsBlock = parentBlock:createBlock()
	self.elements.outerTabsBlock = outerTabsBlock
	outerTabsBlock.autoHeight = true
	outerTabsBlock.widthProportional = 1.0

	-- Create a tab for each page (no need if only one page)
	if #self.pages <= 1 then
		return
	end

	-- Previous Button
	self.elements.previousTabButton = createArrow(outerTabsBlock, arrowPrevious)

	local subBlock = outerTabsBlock:createBlock()
	subBlock.autoHeight = true
	subBlock.widthProportional = 1
	-- Create page tab buttons
	local tabsBlock = subBlock:createThinBorder()
	self.elements.tabsBlock = tabsBlock
	tabsBlock.autoHeight = true
	tabsBlock.widthProportional = 1
	tabsBlock.childOffsetX = 0
	for _, page in ipairs(self.pages) do
		self:createTab(page)
	end
	local firstTab = parentBlock:findChild(self.pages[1].tabUID)
	firstTab.widget.state = tes3.uiState.active

	-- Next Button
	self.elements.nextTabButton = createArrow(outerTabsBlock, arrowNext)

	-- Pagination
	self.elements.nextTabButton:register(tes3.uiEvent.mouseClick, function()
		-- Move active tab forward 1
		for i, page in ipairs(self.pages) do
			local tab = tabsBlock:findChild(page.tabUID)
			if tab.widget.state == tes3.uiState.active then
				self:clickTab(self.pages[table.wrapindex(self.pages, i + 1)])
				break
			end
		end
	end)

	self.elements.previousTabButton:register(tes3.uiEvent.mouseClick, function()
		-- Move active tab back 1
		for i, page in ipairs(self.pages) do
			local tab = tabsBlock:findChild(page.tabUID)
			if tab.widget.state == tes3.uiState.active then
				self:clickTab(self.pages[table.wrapindex(self.pages, i - 1)])
				break
			end
		end
	end)

	parentBlock:registerAfter(tes3.uiEvent.update, function() self:padTabBlock() end)
	outerTabsBlock:updateLayout()
	self:padTabBlock()
end

--- @param parentBlock tes3uiElement
function Template:createSubcomponentsContainer(parentBlock)
	local pageBlock = parentBlock:createBlock()
	pageBlock.heightProportional = 1.0
	pageBlock.widthProportional = 1.0
	pageBlock.flowDirection = tes3.flowDirection.leftToRight
	self.elements.pageBlock = pageBlock
	self:clickTab(self.currentPage or self.pages[1])
end

--- @param parentBlock tes3uiElement
function Template:createContentsContainer(parentBlock)
	self:createLabel(parentBlock)
	self:createTabsBlock(parentBlock)
	self:createSubcomponentsContainer(parentBlock)
end

function Template:register()
	---@type mwse.registerModConfig.package
	local mcm = { template = self }

	--- @param container tes3uiElement
	mcm.onCreate = function(container)
		self:create(container)
		mcm.onClose = self.onClose
	end

	--- @param searchText string
	--- @return boolean
	mcm.onSearch = function(searchText)
		return self:onSearchInternal(searchText)
	end

	mwse.registerModConfig(self.name, mcm)
	mwse.log("%s mod config registered", self.name)
end

function Template.__index(tbl, key)
	-- If the `key` starts with `"create"`, and if there's an `mwse.mcm.create<Component>` method, 
	-- Make a new `Template.create<Component>` method.
	-- Otherwise, look the value up in the `metatable`.
	
	if not key:startswith("create") or mwse.mcm[key] == nil then
		return getmetatable(tbl)[key]
	end

	Template[key] = function(self, data)
		if not data then
			data = {}
		elseif type(data) == "string" then
			data = { label = data }
		end
		data.parentComponent = self
		local component = mwse.mcm[key](data)
		table.insert(self.pages, component)
		return component
	end

	return Template[key]
end

return Template
