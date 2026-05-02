local common = {}

common.eventCallback = require("mwse.common.ui.eventCallback")

--- Sets the disabled flag and a disabled color on the element.
--- If the element supports states, it also sets the disabled state.
--- @param element tes3uiElement
function common.enable(element)
	if (not element.disabled) then
		return
	end

	element.disabled = true
	element.color = tes3ui.getPalette(tes3.palette.activeColor)

	local widget = element.widget
	if (widget and widget.state) then
		widget.state = tes3.uiState.active
	end
end

--- Sets the disabled flag and a disabled color on the element.
--- If the element supports states, it also sets the disabled state.
--- @param element tes3uiElement
function common.disable(element)
	if (element.disabled) then
		return
	end

	element.disabled = true
	element.color = tes3ui.getPalette(tes3.palette.disabledColor)

	local widget = element.widget
	if (widget and widget.state) then
		widget.state = tes3.uiState.disabled
	end
end

return common
