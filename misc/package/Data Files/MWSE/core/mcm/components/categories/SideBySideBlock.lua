--[[
	Displays settings side by side instead of vertically
	Best used for small buttons with no labels
]] --

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.categories.Category")

--- @class mwseMCMSideBySideBlock
local SideBySideBlock = Parent:new()
SideBySideBlock.__index = SideBySideBlock

--- @param parentBlock tes3uiElement
function SideBySideBlock:createSubcomponentsContainer(parentBlock)
	Parent.createSubcomponentsContainer(self, parentBlock)
	self.elements.subcomponentsContainer.flowDirection = tes3.flowDirection.leftToRight
end

return SideBySideBlock
