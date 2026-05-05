local function onMagicEffectEnded(e)
	if e.target ~= tes3.player then return end

	local effectName = tes3.getMagicEffect(e.effect.id).name
	local sourceName = e.source.name

	tes3.messageBox("Effect '%s' from '%s' ended.", effectName, sourceName)
end
event.register(tes3.event.magicEffectEnded, onMagicEffectEnded)
