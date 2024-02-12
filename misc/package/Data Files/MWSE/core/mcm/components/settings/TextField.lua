--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

-- Parent Class
local Parent = require("mcm.components.settings.Setting")

--- Class Object
--- @class mwseMCMTextField
local TextField = Parent:new()
TextField.buttonText = mwse.mcm.i18n("Submit")
TextField.sNewValue = mwse.mcm.i18n("New value: '%s'")
TextField.defaultSetting = ""

function TextField:enable()
	self.elements.inputField.text = self.variable.value and self:convertToLabelValue(self.variable.value) or ""

	--- @param e tes3uiElement
	local function registerAcquireTextInput(e)
		e:register("mouseClick", function()
			tes3ui.acquireTextInput(self.elements.inputField)
		end)
		for _, element in ipairs(e.children or {}) do
			registerAcquireTextInput(element)
		end
	end
	registerAcquireTextInput(self.elements.border)

	self.elements.submitButton:register("mouseClick", function(e)
		self:press()
	end)
	self.elements.submitButton.widget.state = 1
end

function TextField:disable()
	Parent.disable(self)
	self.elements.inputField.color = tes3ui.getPalette("disabled_color")
end

function TextField:update()
	self.variable.value = self.elements.inputField.text

	-- Do this after changing the variable so the callback is correct
	Parent.update(self)
end

function TextField:press()
	-- HINT: this could be overridden for a
	-- confirmation message before calling update
	self:update()
end

function TextField:callback()
	-- default messageBox on update
	tes3.messageBox(self.sNewValue, self:convertToLabelValue(self.variable.value))
end

--- @param parentBlock tes3uiElement
function TextField:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.paddingRight = self.indent -- * 2
end

--- @param parentBlock tes3uiElement
function TextField:createSubmitButton(parentBlock)
	local button = parentBlock:createButton({ id = tes3ui.registerID("TextField_SubmitButton"), text = self.buttonText })
	button.borderAllSides = 0
	button.paddingAllSides = 2
	button.heightProportional = 1.0
	button.widget.state = 2
	self.elements.submitButton = button
end

--- @param parentBlock tes3uiElement
function TextField:createInnerContainer(parentBlock)
	Parent.createInnerContainer(self, parentBlock)
	self.elements.innerContainer.flowDirection = "left_to_right"

end

--- @param parentBlock tes3uiElement
function TextField:makeComponent(parentBlock)
	local border = parentBlock:createThinBorder()
	border.widthProportional = 1.0
	border.autoHeight = true
	border.flowDirection = "left_to_right"
	if self.minHeight then
		border.minHeight = self.minHeight
	end

	-- Backwards compatibility support.
	if (self.variable.numbersOnly ~= nil) then
		if (self.numbersOnly == nil) then
			self.numbersOnly = self.variable.numbersOnly
		end
		self.variable.numbersOnly = nil --- @diagnostic disable-line
	end

	local inputField = border:createTextInput({
		text = self.variable.defaultSetting or "",
		numeric = self.numbersOnly,
	})
	inputField.widthProportional = 1.0
	inputField.autoHeight = true
	inputField.widget.lengthLimit = nil
	inputField.widget.eraseOnFirstKey = false
	inputField.borderLeft = 5
	inputField.borderBottom = 4
	inputField.borderTop = 2
	inputField.consumeMouseEvents = false
	inputField.wrapText = true

	self:createSubmitButton(parentBlock)

	inputField:register("keyEnter", function()
		self:update()
	end)

	self.elements.border = border
	self.elements.inputField = inputField

	self:insertMouseovers(self.elements.border)
end

return TextField
