--[[
	Base Object for all MCM components, such as categories and settings
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

-- MCM Components can't be used before "initialized" event as they read GMST values.
if not tes3.isInitialized() then
	error(debug.traceback(
		"Trying to use an MCM Component before \"modConfigReady\" event triggered!"
	))
end

--- @class mwseMCMComponent
local Component = {}
Component.__index = Component

Component.componentType = "Component"
Component.paddingBottom = 4
Component.indent = 12
Component.sOK = tes3.findGMST(tes3.gmst.sOK).value --[[@as string]]
Component.sCancel = tes3.findGMST(tes3.gmst.sCancel).value --[[@as string]]
Component.sYes = tes3.findGMST(tes3.gmst.sYes).value --[[@as string]]
Component.sNo = tes3.findGMST(tes3.gmst.sNo).value --[[@as string]]
Component.sOn = tes3.findGMST(tes3.gmst.sOn).value --[[@as string]]
Component.sOff = tes3.findGMST(tes3.gmst.sOff).value --[[@as string]]

-- CONTROL METHODS

--- @param data mwseMCMComponent.new.data?
--- @return mwseMCMComponent component
function Component:new(data)
	-- Warning: Unlike every other method, the `self` parameter in the `new` methods
	-- refers to a _subclass_ of `Component`, rather than an _instance_ of `Component`.
	-- So, `setmetatable(t, self)` should be read as `setmetatable(t, Subclass)`.


	local t = data or {}

	if t.parentComponent then
		t.indent = t.parentComponent.childIndent or t.indent
		t.paddingBottom = t.parentComponent.childSpacing or t.paddingBottom
	end

	setmetatable(t, self)
	--- @cast t mwseMCMComponent
	return t
end

-- Prints the component table to the log
--- @param component table?
function Component:printComponent(component)
	mwse.log("{")
	for key, val in pairs(component or self) do
		if type(val) ~= "table" and type(val) ~= "function" then
			local maxLength = 50
			local shortenedValue = (string.len(val) < maxLength) and val or (string.sub(val, 1, maxLength) .. "...")
			mwse.log("	%s: %s", key, shortenedValue)
		end
	end
	mwse.log("}")
end


--- @alias mwseMCMComponentClass
---| "Category" # Categories
---| "SideBySideBlock"
---| "ActiveInfo" # Infos
---| "Hyperlink"
---| "Info"
---| "MouseOverInfo"
---| "ExclusionsPage" # Pages
---| "FilterPage"
---| "MouseOverPage"
---| "Page"
---| "SideBarPage"
---| "Button" # Settings
---| "DecimalSlider"
---| "Dropdown"
---| "KeyBinder"
---| "OnOffButton"
---| "ParagraphField"
---| "Setting"
---| "Slider"
---| "TextField"
---| "YesNoButton"
---| "Template" # Templates


--- @param mouseOverList tes3uiElement[]?
function Component:registerMouseOverElements(mouseOverList)
	for _, element in ipairs(mouseOverList or {}) do
		element:register("mouseOver", function(e)
			event.trigger("MCM:MouseOver", {component = self})
			e.source:forwardEvent(e)
		end)
		element:register("mouseLeave", function(e)
			event.trigger("MCM:MouseLeave")
			e.source:forwardEvent(e)
		end)
	end
end

function Component:disable()
	if self.elements.label then
		self.elements.label.color = tes3ui.getPalette("disabled_color")
	end
end

function Component:enable()
	if self.elements.label then
		self.elements.label.color = tes3ui.getPalette("normal_color")
	end
end

--- @return boolean result
function Component:checkDisabled()
	local disabled = (self.inGameOnly == true and not tes3.player)
	return disabled
end

-- UI METHODS

--- @param parentBlock tes3uiElement
function Component:createLabelBlock(parentBlock)
	local block = parentBlock:createBlock({ id = tes3ui.registerID("LabelBlock") })
	block.flowDirection = "top_to_bottom"
	-- if parentBlock.flowDirection == "top_to_bottom" then
	block.widthProportional = 1.0
	-- else
	--	block.autoWidth = true
	-- end
	block.autoHeight = true

	self.elements.labelBlock = block
	table.insert(self.mouseOvers, block)
end

--- @param parentBlock tes3uiElement
function Component:createLabel(parentBlock)
	if not self.label then
		return
	end

	self:createLabelBlock(parentBlock)

	local id = ("Label: " .. self.label)
	local label = self.elements.labelBlock:createLabel({ id = tes3ui.registerID(id), text = self.label })
	label.borderBottom = self.paddingBottom
	label.borderAllSides = 0
	label.paddingAllSides = 0
	label.wrapText = true
	label.widthProportional = 1.0
	label.childAlignY = 0.5

	self.elements.label = label
	table.insert(self.mouseOvers, label)
end

--[[
	Wraps up the entire component
]]
--- @param parentBlock tes3uiElement
function Component:createOuterContainer(parentBlock)
	local outerContainer
	outerContainer = parentBlock:createBlock({ id = tes3ui.registerID("OuterContainer") })
	outerContainer.flowDirection = "top_to_bottom"

	outerContainer.autoWidth = true

	outerContainer.widthProportional = 1.0

	outerContainer.autoHeight = true
	outerContainer.paddingBottom = self.paddingBottom * 2

	self.elements.outerContainer = outerContainer
	table.insert(self.mouseOvers, outerContainer)
end

--- @param parentBlock tes3uiElement
function Component:createInnerContainer(parentBlock)
	local innerContainer = parentBlock:createBlock({ id = tes3ui.registerID("InnerContainer") })
	innerContainer.widthProportional = parentBlock.widthProportional
	innerContainer.autoWidth = parentBlock.autoWidth
	innerContainer.heightProportional = parentBlock.heightProportional
	innerContainer.autoHeight = parentBlock.autoHeight
	innerContainer.flowDirection = parentBlock.flowDirection
	if self.label then
		innerContainer.paddingLeft = self.indent
	end
	self.elements.innerContainer = innerContainer
end

--- @param parentBlock tes3uiElement
function Component:create(parentBlock)

	self.elements = {}
	self.mouseOvers = {}

	self:createOuterContainer(parentBlock)

	self:createContentsContainer(self.elements.outerContainer)

	if self:checkDisabled() then
		self:disable()
	else
		self:enable()
	end

	-- Register mouse overs
	self:registerMouseOverElements(self.mouseOvers)

	-- Can define a custom formatting function to make adjustments to any element saved
	-- in self.elements
	if self.postCreate then
		self:postCreate()
	end
end

---@param searchText string The text being searched, in lowercase
---@param caseSensitive boolean
---@return boolean
function Component:searchTextMatches(searchText, caseSensitive)
	-- Return true if the searchtext matches the label or description, and false otherwise.
	local label = caseSensitive and self.label or self.label:lower()
	local description = self.description and (caseSensitive and self.description or self.description:lower())

	return label:find(searchText, 1, true) ~= nil
		or description ~= nil and description:find(searchText, 1, true) ~= nil
end

-- Returns the string that should be shown in the MouseOverInfo
---@return string?
function Component:getMouseOverText()
	return self.description
end

return Component
