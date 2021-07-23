local Parent = require("mcm.components.settings.TextField")
local ParagraphField = Parent:new()


function ParagraphField:enable()
	self.elements.inputField.text = self.variable.value

	local function registerAcquireText(element)
		element:register(
			"mouseClick", 
			function()
				tes3ui.acquireTextInput(self.elements.inputField)
			end
		)
	end

	registerAcquireText(self.elements.textFrame)
	registerAcquireText(self.elements.textInput)
end

function ParagraphField:registerEnterKey(element)
	element:register("keyEnter",
		function()
			local inputController = tes3.worldController.inputController
			local holdingShift = (
				inputController:isKeyDown(tes3.scanCode.lShift) or
				inputController:isKeyDown(tes3.scanCode.rShift)
			)
			if not holdingShift then
				self:update()
			else
				element.text = element.text .. "\n"
			end
		end
	)
end

function ParagraphField:makeComponent(parentBlock)
	local border = parentBlock:createBlock()
	border.widthProportional = 1.0
	border.autoHeight = true

	local inputField = border:createParagraphInput()
	inputField.color = tes3ui.getPalette("disabled_color")
	inputField.text = "(In-Game Only)"
	inputField.widthProportional = 1.0
	inputField.widget.lengthLimit = nil


	
	self.elements.border = border
	self.elements.inputField = inputField
	local scrollPane = inputField:findChild(tes3ui.registerID("PartParagraphInput_scroll_pane"))
	local textFrame = inputField:findChild(tes3ui.registerID("PartScrollPane_outer_frame"))
	local textInput = inputField:findChild(tes3ui.registerID("PartParagraphInput_text_input"))
	self.elements.textFrame = textFrame
	self.elements.scrollPane = scrollPane
	self.elements.textInput = textInput

	if self.height then scrollPane.parent.height = self.height end


	self:registerEnterKey(inputField)


	--mouseOvers
	table.insert(self.mouseOvers, self.elements.label)
	local function addMouseovers(children)
		for _, element in ipairs(children) do
			table.insert(self.mouseOvers, element)
			if element.children then
				addMouseovers(element.children)
			end
		end
	end
	addMouseovers(inputField.children)
	
end


return ParagraphField