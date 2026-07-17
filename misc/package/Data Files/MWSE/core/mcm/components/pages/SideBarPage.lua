--[[
	Top level category with a side bar that shows text for any component
	hovered over.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.pages.Page")

local Category = require("mcm.components.categories.Category")
local Info = require("mcm.components.infos.Info")
local MouseOverInfo = require("mcm.components.infos.MouseOverInfo")
local MouseOverPage = require("mcm.components.pages.MouseOverPage")

--- @class mwseMCMSideBarPage
--- @field sidebarComponents mwseMCMComponent[] *Deprecated*
local SideBarPage = Parent:new()
SideBarPage.__index = SideBarPage
SideBarPage.triggerOn = "MCM:MouseOver"
SideBarPage.triggerOff = "MCM:MouseLeave"

--- @param data mwseMCMSideBarPage.new.data|nil
--- @return mwseMCMSideBarPage page
function SideBarPage:new(data)
	--- @diagnostic disable-next-line: param-type-mismatch
	local t = Parent.new(self, data) --[[@as mwseMCMSideBarPage]]
	t.sidebar = MouseOverPage:new({ parentComponent = self})

	return t

end

--- @param parentBlock tes3uiElement
function SideBarPage:createSidetoSideBlock(parentBlock)
	local sideToSideBlock = parentBlock:createBlock()
	sideToSideBlock.flowDirection = tes3.flowDirection.leftToRight
	sideToSideBlock.heightProportional = 1.0
	sideToSideBlock.widthProportional = 1.0
	self.elements.sideToSideBlock = sideToSideBlock
end

--- @param parentBlock tes3uiElement
function SideBarPage:createLeftColumn(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
end

--- Sidebar
--- @param parentBlock tes3uiElement
function SideBarPage:createRightColumn(parentBlock)

	self.sidebar:create(parentBlock)
	local defaultView = self.sidebar.elements.subcomponentsContainer
	local mouseoverView = self.sidebar.elements.mouseOverBlock

	-- For backwards compatibility, add sidebarComponents elements
	if self.sidebarComponents then
		self:createSubcomponents(defaultView, self.sidebarComponents)

		-- or description
	elseif self.description then
		-- By default, sidebar is a mouseOver description pane
		local sidebarInfo = Info:new({
			-- label = self.label,
			text = self.description,
			parentComponent = self
		})
		sidebarInfo:create(defaultView)
	end

	-- MouseOverInfo shows descriptions of settings.
	local mouseOver = MouseOverInfo:new({
		text = self.description or "",
		parentComponent = self
	})
	mouseOver:create(mouseoverView)
	mouseOver.elements.outerContainer.visible = false
	self.elements.mouseOver = mouseOver

	--- event to hide default and show mouseover
	--- @param e { component: mwseMCMComponent }
	local function doMouseOver(e)
		local component = e.component
		-- This results in `component:getMouseOverText()` getting called twice
		-- per mouseover update. Not sure of a nice way around that.
		-- This should be fine for most implementations of `convertToLabelValue`.
		if component:getMouseOverText() ~= nil then
			mouseOver.elements.outerContainer.visible = true
			defaultView.visible = false
		end
	end

	-- event to hide mouseover and show default
	local function doMouseLeave()
		mouseOver.elements.outerContainer.visible = false
		defaultView.visible = true
	end

	-- register events
	event.register(self.triggerOn, doMouseOver)
	event.register(self.triggerOff, doMouseLeave)
	parentBlock:register(tes3.uiEvent.destroy, function()
		event.unregister(self.triggerOn, doMouseOver)
		event.unregister(self.triggerOff, doMouseLeave)
	end)

	-- Add Reset button
	if self.showReset then
		local rightColumnBorder = defaultView.parent
		self:createResetButtonContainer(rightColumnBorder)
		self:createResetButton(self.elements.resetContainer)
	end
end

--- @param parentBlock tes3uiElement
function SideBarPage:createOuterContainer(parentBlock)
	self:createSidetoSideBlock(parentBlock)
	self:createLeftColumn(self.elements.sideToSideBlock)
	self:createRightColumn(self.elements.sideToSideBlock)
end

--- Make sure we don't inherit createContentsContainer from Page since
--- we don't want to have the Reset button on the left list.
--- @param parentBlock tes3uiElement
function SideBarPage:createContentsContainer(parentBlock)
	Category.createContentsContainer(self, parentBlock)
end

return SideBarPage
