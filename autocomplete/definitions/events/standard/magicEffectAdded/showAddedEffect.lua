local function onMagicEffectAdded(e)
	if e.target ~= tes3.player then return end

	local effectName = tes3.getMagicEffect(e.effect.id).name
	local sourceName = e.source.name

	tes3.messageBox("Effect '%s' from '%s' was added.", effectName, sourceName)
end
event.register(tes3.event.magicEffectAdded, onMagicEffectAdded)
