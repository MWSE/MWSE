--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Setting")

--- @class mwseMCMButton
local Button = Parent:new()
Button.disabledText = "---"
Button.leftSide = true
Button.buttonText = "---"

--- Determines what text is displayed on the button
--- @return string buttonText
function Button:getText()
	if self.variable then
		return tostring(self:convertToLabelValue(self.variable.value))
	end
	return self.buttonText
end

--- @param newText string
function Button:setText(newText)
	self.elements.button.text = newText
end

function Button:disable()
	Parent.disable(self)
	self.elements.button.widget.state = tes3.uiState.disabled
end

function Button:update()
	self:setText(self:getText())
	Parent.update(self)
end

function Button:press()
	self:update()
end

function Button:enable()
	Parent.enable(self)
	self:setText(self:getText())
	self.elements.button:register(tes3.uiEvent.mouseClick, function(e)
		self:press()
	end)
end

--- @param parentBlock tes3uiElement
function Button:makeComponent(parentBlock)
	local buttonText = self.buttonText or self.disabledText
	local button = parentBlock:createButton({ id = tes3ui.registerID("Button"), text = buttonText })
	button.borderAllSides = 0
	button.borderRight = self.indent
	button.autoWidth = true
	self.elements.button = button
	table.insert(self.mouseOvers, button)
end

--- @param parentBlock tes3uiElement
function Button:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	-- A bit weird but it seems to line buttons up better with other settings
	self.elements.outerContainer.borderTop = self.paddingBottom
	self.elements.outerContainer.borderBottom = 0
	self.elements.outerContainer.flowDirection = tes3.flowDirection.leftToRight
end

--- @param parentBlock tes3uiElement
function Button:createInnerContainer(parentBlock)
	Parent.createInnerContainer(self, parentBlock)
	self.elements.innerContainer.paddingLeft = 0
end

--- @param parentBlock tes3uiElement
function Button:createContentsContainer(parentBlock)
	self:createInnerContainer(parentBlock)
	if self.leftSide then
		self:makeComponent(self.elements.innerContainer)
	end
	self:createLabel(self.elements.innerContainer)
	if not self.leftSide then
		self:makeComponent(self.elements.innerContainer)
	end
end

return Button
