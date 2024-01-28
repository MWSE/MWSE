--[[
	Slider:
		A Slider Setting
		Combines a text label with a slider widget. For tes3uiSlider the value range is [0, max].
		MCM sliders allow specifying minimal value different than 0. The implementation adds/subtracts
		`self.min` when reading/writing to the current tes3uiSlider's `widget.current` to account for
		that offset (so tes3uiSlider's value range is [0, self.max - self.min]). In addition, children
		may implement support for different conversions (e.g. to support floating-point values). 
		This is accomplished by overloading the `convertToWidgetValue` and `convertToVariableValue` methods.

		- The `Slider:convertToWidgetValue(variableValue)` method is responsible for taking in a variable value
		and outputting the appropriate value to use on the slider widget.

		- The `Slider:convertToSliderValue(widgetValue)` method is responsible for taking in a value stored 
		in a slider widget, and outputting the corresponding variable value.

		Usually, children of this component implement some of the following methods:
		- convertToWidgetValue - convert variable value to widget value
		- convertToSliderValue - convert widget value to slider value
		- updateValueLabel - customize how the current variable value should be formatted in the widget display text.
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.settings.Setting")

--- @class mwseMCMSlider
local Slider = Parent:new()
Slider.min = 0
Slider.max = 100
Slider.step = 1
Slider.jump = 5


function Slider:convertToWidgetValue(variableValue)
	return variableValue - self.min
end

function Slider:convertToVariableValue(widgetValue)
	return widgetValue + self.min
end

function Slider:updateValueLabel()
	local labelElement = self.elements.label

	if string.find(self.label, "%s", nil, true) then
		labelElement.text = self.label:format(self.variable.value)
	else
		labelElement.text = string.format("%s: %s", self.label, self.variable.value)
	end
end

function Slider:updateVariableValue()
	if self.elements.slider then
		local widgetValue = self.elements.slider.widget.current
		self.variable.value = self:convertToVariableValue(widgetValue)
	end
end

-- update the value stored in the slider to the value stored in the variable
function Slider:updateWidgetValue()
	if self.elements.slider then
		local widgetValue = self:convertToWidgetValue(self.variable.value)
		self.elements.slider.widget.current = widgetValue
	end
end



function Slider:update()
	self:updateVariableValue()
	Parent.update(self)
end

--- @param element tes3uiElement
function Slider:registerSliderElement(element)
	-- click
	element:register(tes3.uiEvent.mouseClick, function(e)
		self:update()
	end)
	-- drag
	element:register(tes3.uiEvent.mouseRelease, function(e)
		self:update()
	end)
end



function Slider:enable()
	Parent.enable(self)
	if self.variable.value then
		self:updateWidgetValue()
		self:updateValueLabel()
	end

	-- Register slider elements so that the value only updates when the mouse is released
	for _, sliderElement in ipairs(self.elements.slider.children) do
		self:registerSliderElement(sliderElement)
		for _, innerElement in ipairs(sliderElement.children) do
			self:registerSliderElement(innerElement)
		end
	end

	-- But we want the label to update in real time so you can see where it's going to end up
	self.elements.slider:register(tes3.uiEvent.partScrollBarChanged, function(e)
		self:updateVariableValue()
		self:updateValueLabel()
	end)
end

function Slider:disable()
	Parent.disable(self)

	self.elements.slider.children[2].children[1].visible = false
end

-- UI creation functions

--- @param parentBlock tes3uiElement
function Slider:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.borderRight = self.indent -- * 2
end

--- @param parentBlock tes3uiElement
function Slider:createLabel(parentBlock)
	Parent.createLabel(self, parentBlock)
	self:updateValueLabel()
end

--- @param parentBlock tes3uiElement
function Slider:makeComponent(parentBlock)
	local sliderBlock = parentBlock:createBlock()
	sliderBlock.flowDirection = tes3.flowDirection.leftToRight
	sliderBlock.autoHeight = true
	sliderBlock.widthProportional = 1.0

	local minwidgetValue = self:convertToWidgetValue(self.min)

	--[[in most settings, `convertToWidgetValue` will be a function of the form
		`f(x) = C * (x - min)`
	where `min == self.min` and `C` is a constant.
	in this setting, 
		range 	== f(max) - f(min)
				== C * (max - min) - C * (min - min)
				== C * (max - min)
	]]
	local range = self:convertToWidgetValue(self.max) - minwidgetValue



	local slider = sliderBlock:createSlider({ current = 0, max = range })
	slider.widthProportional = 1.0

	-- Set custom values from setting data
	--[[ continuing the example from earlier, if `f(x) = C * x + min` then
		widget.step	== f(step + min) - f(min)
					== C * (step + min - min) - C * (min - min)
					== C * step

	similarly, `widget.jump == C * jump`
	]]
	slider.widget.step = self:convertToWidgetValue(self.step + self.min) - minwidgetValue
	slider.widget.jump = self:convertToWidgetValue(self.jump + self.min) - minwidgetValue

	self.elements.slider = slider
	self.elements.sliderBlock = sliderBlock

	-- add mouseovers
	table.insert(self.mouseOvers, sliderBlock)
	-- Add every piece of the slider to the mouseOvers
	for _, sliderElement in ipairs(slider.children) do
		table.insert(self.mouseOvers, sliderElement)
		for _, innerElement in ipairs(sliderElement.children) do
			table.insert(self.mouseOvers, innerElement)
		end
	end
end

return Slider
