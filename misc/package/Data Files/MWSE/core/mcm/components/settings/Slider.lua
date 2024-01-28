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

		- The `Slider:convertToVariableValue(widgetValue)` method is responsible for taking in a value stored 
		in a slider widget, and outputting the corresponding variable value.

		Usually, children of this component implement some of the following methods:
		- convertToWidgetValue - convert variable value to widget value
		- convertToVariableValue - convert widget value to slider value
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


-- `y == C * x + a` ~> (y - a) / C == x

function Slider:convertToVariableValue(widgetValue)
	-- e.g., consider  `min == 10`. then 
	local a = self:convertToWidgetValue(0) 		-- `a == -10`
	local C = self:convertToWidgetValue(1) - a	-- `C == -9 - (-10) == 1
	return (widgetValue - a) / C				-- `returnVal == widgetValue + 10`
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
	-- if the variable exists, use it to update the widget and the displayed label
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

	--[[We know that `convertToWidgetValue` is a function `f` of the form 
		
		`f(x) == C * x - K * min`

	and we know that `f(m) == 0`, where `m` is the minimum config value. So, the relationship between `m` and `min` is 
	`C * m = K * min`.

	For the purposes of calculating the slider `range`, `jump`, and `step`, we want to pretend `C` and `K` are the same.
	We could do this by setting

		`g(x) = f(K / C * x) == K * x - K * min`,

	But there's a problem here: we only know what `K` is whenever `min` is not zero.
	We can bypass this problem by temporarily setting `min = 1`, so that `K == f(0)`.
	Likewise, we can get `C` by setting `min = 0`.
	(Note that we want to use `K` instead of `C` because `K` specifies the MCM values.)
	]]

	local min = self.min
	self.min = 1
	local K = self:convertToWidgetValue(0)
	self.min = 0 -- this is an easy way to get `C`, but there are others.
	local C = self:convertToWidgetValue(1)
	self.min = min

	
	--[[`g` is a copy of `convertToWidgetValue` that treats `self.min`, `self.max`, `self.step`, and `self.jump`
	as if those values were the corresponding values that get stored in the config.
	
	e.g., in the case of a `PercentageSlider` with `self.min = 10` and `self.max = 100`, we'd have 
		`C == 100` and `K == 1`, so `K / C == 1 / 100`. The effect of this is that 
	 	`g(self.min) == self:convertToWidgetValue(0.1) == 0` and 
		`g(self.max) == self:convertToWidgetValue(1) == 90`

	So, the ranges can be calculated correctly.
	]]
	local function g(x) return self:convertToWidgetValue((K / C) * x) end



	local sliderBlock = parentBlock:createBlock()
	sliderBlock.flowDirection = tes3.flowDirection.leftToRight
	sliderBlock.autoHeight = true
	sliderBlock.widthProportional = 1.0


	local range = g(self.max)

	local slider = sliderBlock:createSlider({ current = 0, max = range })
	slider.widthProportional = 1.0

	-- Set custom values from setting data
	-- get the `step` and `jump` by starting from `self.min`, incrementing a bit, then converting
	-- to the slider settings, then see where we end up
	slider.widget.step = g(self.step + self.min)
	slider.widget.jump = g(self.jump + self.min)

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
