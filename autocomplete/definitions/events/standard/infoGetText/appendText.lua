-- This will make Ra'virr give the player 2 Moon Sugar when the conversation is initiated

local function onInfoGetText(e)
	if e.info.actor.id ~= "ra'virr" then
		return
	end

	if e.info.type ~= tes3.dialogueType.greeting then
		return
	end

	-- e.text is empty by default.
	e.text = e:loadOriginalText() .. "\n\nSweet sugar! Have some."
	tes3.addItem({
		reference = tes3.player,
		item = "ingred_moon_sugar_01",
		count = 2,
	})
end

event.register(tes3.event.infoGetText, onInfoGetText)