local function onMagicEffectRemoved(e)
	-- Make sure we are only showing messages for effects that are applied to the player.
	if e.mobile ~= tes3.mobilePlayer then return end

	-- Get the name of the magic effect that has been removed.
	local effectName = tes3.getMagicEffect(e.effect.id).name

	-- Get the name of the spell/enchantment/alchemy this magic effect belongs to.
	local sourceName = e.source.name

	-- Show a message containing the effect and spell/enchantment/alchemy names to the player.
	tes3.messageBox("Effect '%s' from '%s' has been removed.", effectName, sourceName)
end
event.register(tes3.event.magicEffectRemoved, onMagicEffectRemoved)