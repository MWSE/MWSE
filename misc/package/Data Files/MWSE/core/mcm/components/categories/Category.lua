--[[
	Category
		--Base class for category and page type
	A category is a simple container that holds infos, settings and other categories
	Categories can be nested infinitely
	A basic category has a label and an indented block of components

	An example definition of a category:

			{
				class = "Category",
				label = "Label", --optional but recommended
				description = "This is a category example", --optional, used for mouseOvers
				components = {
					... --list of components
				}
			}
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local utils = require("mcm.utils")
local Parent = require("mcm.components.Component")

--- @class mwseMCMCategory
local Category = Parent:new()
-- Note: `Category.__index` metamethod is defined below.

Category.componentType = "Category"
-- Category.childSpacing = 20
-- Category.childIndent = 40
-- CONTROL METHODS

--- @param data mwseMCMCategory.new.data|nil
--- @return mwseMCMCategory
function Category:new(data)
	--- @diagnostic disable-next-line: param-type-mismatch
	local t = Parent.new(self, data) --[[@as mwseMCMCategory]]
	t.components = t.components or {}

	--- @cast t mwseMCMCategory

	local parent = t.parentComponent
	if not parent then return t end

	if t.showDefaultSetting == nil then
		-- Using `rawget` so we don't inherit a default value
		t.showDefaultSetting = rawget(parent, "showDefaultSetting")
	end

	local configKey = t.configKey
	if not t.config and parent.config then
		t.config = parent.config[configKey] or parent.config
	end

	if not t.defaultConfig and parent.defaultConfig then
		t.defaultConfig = parent.defaultConfig[configKey] or parent.defaultConfig
	end
	return t
end


function Category:resetToDefault()
	for _, component in ipairs(self.components) do
		component:resetToDefault()
	end
end

function Category:disable()
	Parent.disable(self)
	for _, element in ipairs(self.elements.subcomponentsContainer.children) do
		if element.color then
			element.color = tes3ui.getPalette(tes3.palette.disabledColor)
		end
	end
end

function Category:enable()
	if self.elements.label then
		self.elements.label.color = tes3ui.getPalette(tes3.palette.headerColor)
	end
end

function Category:update()
	for _, component in ipairs(self.components) do
		if component.update then
			component:update()
		end
	end
end

function Category:checkDisabled()
	-- allow the user to override the behavior
	if self.inGameOnly then
		return not tes3.player
	end

	-- dont disable if there are no subcomponents
	if table.empty(self.components) then return false end

	-- dont disable if one subcomponent isn't disabled
	for _, component in ipairs(self.components) do
		if not component:checkDisabled() then
			return false
		end
	end
	-- disable if there are nested components and they're all disabled
	return true
end

-- UI METHODS

--- @param parentBlock tes3uiElement
function Category:createSubcomponentsContainer(parentBlock)
	local subcomponentsContainer = parentBlock:createBlock({ id = tes3ui.registerID("Category_ContentsContainer") })
	subcomponentsContainer.flowDirection = tes3.flowDirection.topToBottom
	subcomponentsContainer.widthProportional = parentBlock.widthProportional
	subcomponentsContainer.heightProportional = parentBlock.heightProportional
	subcomponentsContainer.autoHeight = parentBlock.autoHeight
	subcomponentsContainer.autoWidth = parentBlock.autoWidth
	self.elements.subcomponentsContainer = subcomponentsContainer
end

--- @param parentBlock tes3uiElement
--- @param components mwseMCMComponent.new.data[]
function Category:createSubcomponents(parentBlock, components)
	for _, component in pairs(components or {}) do

		-- Make sure it's actually a `Component`.
		if not component.componentType then
			local componentClass = utils.getComponentClass(component.class)
			if not componentClass then
				error(string.format("Could not intialize component %q", component.label))
			end
			component.parentComponent = self
			componentClass:new(component) -- Modifies in-place, which is why it's okay to use in this loop.
		end

		--- @cast component +mwseMCMComponent
		component:create(parentBlock)
	end
end

--- @param parentBlock tes3uiElement
function Category:createContentsContainer(parentBlock)
	self:createLabel(parentBlock)
	self:createInnerContainer(parentBlock)
	self:createSubcomponentsContainer(self.elements.innerContainer)
	self:createSubcomponents(self.elements.subcomponentsContainer, self.components)
	parentBlock:getTopLevelMenu():updateLayout()
end

function Category.__index(tbl, key)
	-- If the `key` starts with `"create"`, and if there's an `mwse.mcm.create<Component>` method,
	-- Make a new `Category.create<Component>` method.
	-- Otherwise, look the value up in the `metatable`.

	-- This should always be true.
	-- But it still needs to be tested on larger mod lists before the PR is merged.
	assert(getmetatable(tbl) == Category)

	-- Look up missing values in `Category`
	local categoryValue = Category[key]
	if categoryValue ~= nil then
		return categoryValue
	end

	-- If the value wasn't found, check if `key` is of the form `create<ComponentClass>`.
	-- e.g., check for stuff like `myCategory:createSlider`.


	-- This will be equal to e.g., `mwse.mcm.createSlider`.
	local mcmCreateComponent = mwse.mcm[key]
	if mcmCreateComponent == nil or not key:startswith("create") then
		return
	end

	categoryValue = function(self, componentData)
		if not componentData then
			componentData = {}
		elseif type(componentData) == "string" then
			componentData = { label = componentData }
		end
		componentData.parentComponent = self
		local component = mcmCreateComponent(componentData)
		table.insert(self.components, component)
		return component
	end
	-- Cache the create function so we don't have to reconstruct it every time.
	Category[key] = categoryValue

	return categoryValue
end

return Category
