--- @param parent tes3uiElement
--- @return tes3uiElement input
local function createSeachBox(parent)
	local searchBox = parent:createThinBorder()
	searchBox.autoHeight = true
	searchBox.autoWidth = true
	searchBox.widthProportional = 1.0
	searchBox.paddingAllSides = 8
	searchBox.borderBottom = 8
	searchBox.borderTop = 8

	local input = searchBox:createTextInput({
		autoFocus = true,
		placeholderText = "Search...",
	})
	input.autoWidth = true

	searchBox:registerAfter(tes3.uiEvent.mouseClick, function(e)
		tes3ui.acquireTextInput(input)
	end)

	return input
end
