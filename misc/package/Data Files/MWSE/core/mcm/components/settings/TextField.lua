--- These types have annotations in the core\meta\ folder. Let's stop the warning spam here in the implementation.
--- The warnings arise because each field set here is also 'set' in the annotations in the core\meta\ folder.
--- @diagnostic disable: duplicate-set-field

-- Parent Class
local Parent = require("mcm.components.settings.Setting")

--- Class Object
--- @class mwseMCMTextField
local TextField = Parent:new()
TextField.__index = TextField

TextField.buttonText = mwse.mcm.i18n("Submit")
TextField.sNewValue = mwse.mcm.i18n("New value: '%s'")
TextField.defaultSetting = ""

function TextField:enable()
	self.elements.inputField.text = self.variable.value and self:convertToLabelValue(self.variable.value) or ""

	--- @param e tes3uiElement
	local function registerAcquireTextInput(e)
		e:register(tes3.uiEvent.mouseClick, function()
			tes3ui.acquireTextInput(self.elements.inputField)
		end)
		for _, element in ipairs(e.children or {}) do
			registerAcquireTextInput(element)
		end
	end
	registerAcquireTextInput(self.elements.border)

	self.elements.submitButton:register(tes3.uiEvent.mouseClick, function(e)
		self:press()
	end)
	self.elements.submitButton.widget.state = tes3.uiState.normal
end

function TextField:disable()
	Parent.disable(self)
	self.elements.inputField.color = tes3ui.getPalette(tes3.palette.disabledColor)
end

function TextField:setVariableValue(newValue)
	self.elements.inputField.text = newValue
	Parent.update(self)
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

-- Returns the string that should be shown in the MouseOverInfo
--- @return string?
function TextField:getMouseOverText()
	local var = self.variable
	local shouldAddDefaults = (self.showDefaultSetting and var and var.defaultSetting ~= nil)

	if not shouldAddDefaults then
		return self.description -- This has type `string|nil`
	end

	-- Now we add defaults to the description.
	local defaultStr = self:convertToLabelValue(var.defaultSetting)

	-- No description exists yet? Then we'll only write the default value.
	if not self.description then
		return string.format("%s: \"%s\".", mwse.mcm.i18n("Default"), defaultStr)
	end

	return string.format(
		"%s\n\n\z
		 %s: \"%s\".",
		self.description,
		mwse.mcm.i18n("Default"), defaultStr
	)
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
	self.elements.innerContainer.flowDirection = tes3.flowDirection.leftToRight

end

--- @param parentBlock tes3uiElement
function TextField:makeComponent(parentBlock)
	local border = parentBlock:createThinBorder()
	border.widthProportional = 1.0
	border.autoHeight = true
	border.flowDirection = tes3.flowDirection.leftToRight
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

	inputField:register(tes3.uiEvent.keyEnter, function()
		self:update()
	end)

	self.elements.border = border
	self.elements.inputField = inputField

	self:insertMouseovers(self.elements.border)
end

return TextField
