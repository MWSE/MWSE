
--
-- Common utility functions.
--

local function validateOption(option)
	assert(type(option) == "table", "Invalid option provided. Must be a table.")
	assert(type(option.text) == "string", "Invalid 'text' provided for option.")
end

local function validateOptions(options)
	assert(type(options) == "table", "Invalid options provided. Must be a table.")
	assert(#options > 0, "Invalid options provided. Must have at least one option.")
	for _, option in ipairs(options) do
		validateOption(option)
	end
end


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

function metatable:get_index()
	return self.rawdata.index
end

function metatable:set_index(value)
	-- Set value with wrapping.
	value = table.wrapindex(self.options, value)

	-- Don't do redundant work.
	if (self.index == value) then
		return
	end

	local element = self.element
	local text = self:getTextElement()
	local button = self:getButtonElement()

	self.rawdata.index = value
	self.text = self.options[value].text

	local color = self.options[value].color or tes3ui.getPalette(tes3.palette.normalColor)
	button.idle = color
	text.color = color

	element:getTopLevelMenu():updateLayout()
	element:triggerEvent("valueChanged")
end

function metatable:get_value()
	return self.options[self.index].value
end

function metatable:set_value(value)
	local index = self:getIndexForValue(value)
	assert(index, "No option exists for this value.")
	self.index = index
end

function metatable:get_text()
	return self:getTextElement().text
end

function metatable:set_text(value)
	self:getTextElement().text = value
end

function metatable:get_options()
	return self.rawdata.options
end

function metatable:set_options(value)
	validateOptions(value)
	self.rawdata.options = value
	self.index = 1
end

function metatable:addOption(option)
	validateOption(option)
	table.insert(self.options, option)
end

function metatable:getIndexForValue(value)
	for i, option in ipairs(self.rawdata.options) do
		if (option.value == value) then
			return i
		end
	end
end

function metatable:getTextElement()
	return self.element:findChild("PartButton_text_ptr")
end

function metatable:getButtonElement()
	return self.rawdata.button
end

function metatable:previous()
	self.index = self.index - 1
end

function metatable:next()
	self.index = self.index + 1
end


--
-- Base element creation.
--

--- @param e tes3uiEventData
local function onCycleButtonClicked(e)
	local element = e.source
	element.widget:next()
end

function tes3uiElement:createCycleButton(params)
	-- Validate params.
	assert(type(params) == "table", "Invalid parameters provided.")
	validateOptions(params.options)
	if (params.index) then
		assert(type(params.index) == "number", "Invalid 'index' parameter provided. Must be a valid index into the options table.")
		assert(type(params.options[params.index]) == "table", "Invalid 'index' parameter provided. Must be a valid index into the options table.")
	end

	-- Create and define basic properties.
	local button = self:createButton({ id = params.id })

	-- Define as a custom widget.
	button:makeLuaWidget("cycleButton", { rawdata = { button = button.widget } })
	button.widget.options = params.options
	button.widget.index = params.index or 1

	-- Register callbacks.
	button:registerAfter("mouseClick", onCycleButtonClicked)

	return button
end

tes3ui.defineLuaWidget({
	name = "cycleButton",
	metatable = metatable,
})
