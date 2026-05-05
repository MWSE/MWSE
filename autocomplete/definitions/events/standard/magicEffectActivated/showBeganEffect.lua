local function onMagicEffectActivated(e)
	if e.target ~= tes3.player then return end

	local effectName = tes3.getMagicEffect(e.effect.id).name
	local sourceName = e.source.name

	tes3.messageBox("Effect '%s' from '%s' began.", effectName, sourceName)
end
event.register(tes3.event.magicEffectActivated, onMagicEffectActivated)
