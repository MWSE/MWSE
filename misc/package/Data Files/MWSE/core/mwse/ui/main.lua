
--
-- Custom widget support.
--

--- @type table<string, table>
local luaWidgetTypes = {}

-- Allow elements to make lua widgets.
function tes3uiElement:makeLuaWidget(typeName, data)
	local metatable = assert(luaWidgetTypes[typeName], "No lua widget exists with that type.")

	-- Allow going back to the original element.
	data.element = self

	-- Tell MWSE what kind of widget this is.
	local widget = setmetatable(data, metatable)
	self:setPropertyProperty("is_part", "MWSE:LuaWidget")
	self:setLuaData("MWSE:WidgetData", widget)
	self:setLuaData("MWSE:WidgetTypeName", typeName)

	return widget
end

-- Allow defining custom lua widgets.
function tes3ui.defineLuaWidget(params)
	assert(params, "No parameters provided.")
	assert(type(params.name) == "string", "Invalid 'name' parameter provided.")
	assert(type(params.metatable) == "table", "Invalid 'metatable' parameter provided.")
	assert(luaWidgetTypes[params.name] == nil, "A lua widget of this type has already been defined.")

	luaWidgetTypes[params.name] = params.metatable
end


--
-- Simple API extensions.
--

function tes3ui.log(str, ...)
	tes3ui.logToConsole(tostring(str):format(...), false)
end

function tes3ui.showNotifyMenu(str, ...)
	return tes3.messageBox(tostring(str):format(...))
end


--
-- Complex API extensions, hosted in separate files.
--

-- tes3ui extensions.
dofile("mwse.ui.api.showColorPickerMenu")
dofile("mwse.ui.api.showMessageMenu")

-- tes3uiElement extensions.
dofile("mwse.ui.tes3uiElement.copy")
dofile("mwse.ui.tes3uiElement.createColorPicker")
dofile("mwse.ui.tes3uiElement.createColorPreview")
dofile("mwse.ui.tes3uiElement.createCycleButton")
dofile("mwse.ui.tes3uiElement.createHyperlink")
dofile("mwse.ui.tes3uiElement.createImageButton")
dofile("mwse.ui.tes3uiElement.createTabContainer")
dofile("mwse.ui.tes3uiElement.createTextInput")
dofile("mwse.ui.tes3uiElement.move")
dofile("mwse.ui.tes3uiElement.sortChildren")
