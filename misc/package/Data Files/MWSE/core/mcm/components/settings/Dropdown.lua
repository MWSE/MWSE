--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Setting")

--- @class mwseMCMDropdown
local Dropdown = Parent:new()
Dropdown.idleColor = tes3ui.getPalette(tes3.palette.normalColor)
Dropdown.overColor = tes3ui.getPalette(tes3.palette.normalOverColor)
Dropdown.pressedColor = tes3ui.getPalette(tes3.palette.normalPressedColor)


function Dropdown.new(class, data)
	local obj = Parent.new(class, data) --[[@as mwseMCMDropdown]]
	obj.selectedOption = obj:getOption()
	return obj
end

function Dropdown:getOption(optionValue)
	optionValue = optionValue or self.variable and self.variable.value

	if not optionValue then
		return
	end

	for _, option in ipairs(self.options) do
		if option.value == optionValue then
			return option
		end
	end
end

function Dropdown:setVariableValue(newValue)
	local option = self:getOption(newValue)
	self:selectOption(option)
end

function Dropdown:enable()
	Parent.enable(self)

	self.selectedOption = self.selectedOption or self:getOption()

	self.elements.textBox.text = self.selectedOption and self.selectedOption.label
	self.elements.textBox.color = self.idleColor
	self.elements.textBox:register(tes3.uiEvent.mouseClick, function()
		self:createDropdown()
	end)
end

--- @param option mwseMCMDropdownOption
function Dropdown:selectOption(option)
	self.elements.dropdownParent:destroyChildren()
	self.dropdownActive = false
	-- No new option selected? Don't execute the callback.
	if self.selectedOption == option then
		return
	end
	self.selectedOption = option
	self.variable.value = option.value
	self.elements.textBox.text = option.label

	if option.callback then
		option.callback(self)
	end
	self:update()
end

function Dropdown:createDropdown()
	if not self.dropdownActive then
		self.dropdownActive = true
		-- Create dropdown
		local dropdown = self.elements.dropdownParent:createThinBorder()
		dropdown.flowDirection = tes3.flowDirection.topToBottom
		dropdown.autoHeight = true
		dropdown.widthProportional = 1.0
		dropdown.paddingAllSides = 6
		dropdown.borderTop = 0
		for _, option in ipairs(self.options) do

			local listItem = dropdown:createTextSelect({ text = option.label })

			listItem.widthProportional = 1.0
			listItem.autoHeight = true
			listItem.borderBottom = 3
			listItem.widget.idle = self.idleColor
			listItem.widget.over = self.overColor
			listItem.widget.pressed = self.pressedColor

			listItem:register(tes3.uiEvent.mouseClick, function()
				self:selectOption(option)
				dropdown:getTopLevelMenu():updateLayout()
			end)
		end
		self.elements.dropdown = dropdown
		dropdown:getTopLevelMenu():updateLayout()

		-- Show the setting description when picking an option
		self:registerMouseOverElements(dropdown.children)
		self:registerMouseOverElements({dropdown})

		-- Destroy dropdown
	else
		self.elements.dropdownParent:destroyChildren()
		self.dropdownActive = false
		self.elements.dropdownParent:getTopLevelMenu():updateLayout()
	end

	--- @param element tes3uiElement
	local function recursiveContentsChanged(element)
		if not element then
			return
		end

		local widget = element.widget
		if widget and widget.contentsChanged then
			--- @cast widget tes3uiScrollPane
			widget:contentsChanged()
		end
		recursiveContentsChanged(element.parent)
	end
	-- Recursively go back to parent and call contentsChanged because scrolling is affected.
	recursiveContentsChanged(self.elements.outerContainer.parent)

end

--- @param parentBlock tes3uiElement
function Dropdown:makeComponent(parentBlock)

	local border = parentBlock:createThinBorder()
	border.widthProportional = 1.0
	border.autoHeight = true
	border.paddingLeft = 4
	border.paddingTop = 2
	border.paddingBottom = 4
	border.borderTop = 2
	self.elements.border = border

	local textBox = border:createTextSelect({ text = "---" })
	self.elements.textBox = textBox
	-- Show the setting description when hovering over the text box.
	table.insert(self.mouseOvers, textBox)

	textBox.color = tes3ui.getPalette(tes3.palette.disabledColor)
	textBox.widget.idle = self.idleColor
	textBox.widget.over = self.overColor
	textBox.widget.pressed = self.pressedColor
	textBox.widthProportional = 1.0
	textBox.borderAllSides = 2

	local dropdownParent = parentBlock:createBlock()
	dropdownParent.flowDirection = tes3.flowDirection.topToBottom
	dropdownParent.widthProportional = 1.0
	dropdownParent.autoHeight = true
	self.elements.dropdownParent = dropdownParent

end

--- @param parentBlock tes3uiElement
function Dropdown:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.paddingRight = self.indent -- * 2
end

function Dropdown:convertToLabelValue(variableValue)
	-- Find the matching option and return its label.
	local option = self:getOption(variableValue)
	return option and option.label
end


return Dropdown
