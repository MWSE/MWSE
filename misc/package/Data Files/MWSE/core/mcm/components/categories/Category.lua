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
local Parent = require("mcm.components.Component")
--- @class mwseMCMCategory
local Category = Parent:new()
Category.componentType = "Category"
-- Category.childSpacing = 20
-- Category.childIndent = 40
-- CONTROL METHODS

--- @param data mwseMCMCategory.new.data|nil
--- @return mwseMCMCategory
function Category:new(data)
	local t = Parent:new(data)
	t.components = t.components or {}

	setmetatable(t, self)
	t.__index = self.__index
	--- @cast t mwseMCMCategory

	return t
end

function Category:disable()
	Parent.disable(self)
	for _, element in ipairs(self.elements.subcomponentsContainer.children) do
		if element.color then
			element.color = tes3ui.getPalette("disabled_color")
		end
	end
end

function Category:enable()
	if self.elements.label then
		self.elements.label.color = tes3ui.getPalette("header_color")
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
	subcomponentsContainer.flowDirection = "top_to_bottom"
	subcomponentsContainer.widthProportional = parentBlock.widthProportional
	subcomponentsContainer.heightProportional = parentBlock.heightProportional
	subcomponentsContainer.autoHeight = parentBlock.autoHeight
	subcomponentsContainer.autoWidth = parentBlock.autoWidth
	self.elements.subcomponentsContainer = subcomponentsContainer
end

--- @param parentBlock tes3uiElement
--- @param components mwseMCMComponent.getComponent.componentData[]
function Category:createSubcomponents(parentBlock, components)
	for _, component in pairs(components or {}) do
		component.parentComponent = self
		local newComponent = self:getComponent(component)

		newComponent:create(parentBlock)
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
	
	if not key:startswith("create") or mwse.mcm[key] == nil then
		return getmetatable(tbl)[key]
	end

	Category[key] = function(self, data)
		if not data then
			data = {}
		elseif type(data) == "string" then
			data = { label = data }
		end
		data.parentComponent = self
		local component = mwse.mcm[key](data)
		table.insert(self.components, component)
		return component
	end

	return Category[key]
end

return Category
