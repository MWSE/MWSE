---@param e spellResistedEventData
local function onSpellResisted(e)
	-- We only want this to apply to spells. This event can also trigger for other sources like potions/enchantments.
	if e.source.objectType ~= tes3.objectType.spell then return end

	-- Add the resisted spell to the actor that resisted it.
	tes3.addSpell{
		reference = e.reference,
		spell = e.source --[[@as tes3spell]]
	}

	-- If it is the player that learned the spell, show a message with the name of the spell.
	if e.reference ~= tes3.player then return end
	tes3.messageBox("You learned the '%s' spell.", e.source.name)
end
event.register(tes3.event.spellResisted, onSpellResisted)