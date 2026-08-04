--[[
	A clickable hyperlink
]]--

--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

local Parent = require("mcm.components.infos.Info")

--- @class mwseMCMHyperlink
--- @field exec string *Deprecated*
local Hyperlink = Parent:new()
Hyperlink.__index = Hyperlink

--- @param parentBlock tes3uiElement
function Hyperlink:makeComponent(parentBlock)
	-- Convert legacy exec param to a URL.
	if (not self.url and self.exec) then
		self.url = self.exec:gsub("^start ", "")
	end

	if not self.text or not self.url then
		mwse.log("ERROR: Text field missing for the following setting: ")
		self:printComponent()
	end

	local link = parentBlock:createHyperlink({
		text = self.text,
		url = self.url,
	})

	link.borderRight = self.indent -- * 2
	link.wrapText = true
	link.autoHeight = true
	link.autoWidth = true
	link.widthProportional = 1.0
	self.elements.info = link
	return link
end

return Hyperlink
