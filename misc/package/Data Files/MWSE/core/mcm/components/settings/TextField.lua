--Parent Class
local Parent = require("mcm.components.settings.Setting")
--Class Object
local TextField = Parent:new()
TextField.buttonText = "Submit"
TextField.sNumbersOnly = "Value must be a number."
TextField.sNewValue = "New value: '%s'"
TextField.defaultSetting = ""
function TextField:enable()
	self.elements.inputField.text = self.variable.value or ""

	self.elements.border:register(
		"mouseClick", 
		function()
			tes3ui.acquireTextInput(self.elements.inputField)
		end
	)
	local function registerAcquireTextInput(e)
		e:register(
			"mouseClick", 
			function()
				tes3ui.acquireTextInput(self.elements.inputField)
			end
		)
        if e.children then
            for _, element in ipairs(e.children) do
                registerAcquireTextInput(element)
            end
        end
	end
	registerAcquireTextInput(self.elements.inputField)

	self.elements.submitButton:register(
		"mouseClick", 
		function(e)
			self:press()
		end
	) 
	self.elements.submitButton.widget.state = 1
end


function TextField:disable()
	Parent.disable(self)
	self.elements.inputField.color = tes3ui.getPalette("disabled_color")
end


function TextField:update()
	local isNumber = (
		tonumber(self.elements.inputField.text)
	)
	if self.variable.numbersOnly and not isNumber then
		local sOk = tes3.findGMST(tes3.gmst.sOK).value
		tes3.messageBox({
			message = self.sNumbersOnly,
			buttons = { sOk }
		})
		self.elements.inputField.text = self.variable.value 
		return
	end
	self.variable.value = self.elements.inputField.text
	--Do this after changing the variable so the callback is correct
	Parent.update(self)
end

function TextField:press()
	--HINT: this could be overridden for a 
	--confirmation message before calling update
	self:update()
end  

function TextField:callback()
	--default messageBox on update
	tes3.messageBox(self.sNewValue, self.variable.value)
end


function TextField:createOuterContainer(parentBlock)
	Parent.createOuterContainer(self, parentBlock)
	self.elements.outerContainer.paddingRight = self.indent -- * 2
end

function TextField:createSubmitButton(parentBlock)
	local button = parentBlock:createButton({ id = tes3ui.registerID("TextField_SubmitButton"), text = self.buttonText })
	button.borderAllSides = 0
	button.paddingAllSides = 2
	button.heightProportional = 1.0
	button.widget.state = 2
	self.elements.submitButton = button
end

function TextField:createInnerContainer(parentBlock)
	Parent.createInnerContainer(self, parentBlock)
	self.elements.innerContainer.flowDirection = "left_to_right"
	
end



function TextField:makeComponent(parentBlock)
	local border = parentBlock:createThinBorder()
	border.widthProportional = 1.0
	border.autoHeight = true
	border.flowDirection = "left_to_right"
	if self.minHeight then border.minHeight = self.minHeight end

	local inputField = border:createTextInput()
	inputField.text = self.variable.defaultSetting or ""
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

	inputField:register("keyEnter",
		function()
			self:update()
		end
	)

	self.elements.border = border
	self.elements.inputField = inputField

	table.insert(self.mouseOvers, self.elements.border)
	table.insert(self.mouseOvers, self.elements.inputField)
	table.insert(self.mouseOvers, self.elements.label)


end


return TextField