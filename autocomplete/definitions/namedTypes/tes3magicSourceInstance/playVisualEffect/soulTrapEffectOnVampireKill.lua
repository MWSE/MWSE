local function onDamaged(e)
	-- Check if we killed our target with this damage.
	if not e.killingBlow then return end

	-- Iterate through the killed target's active magic effects.
	for _, activeMagicEffect in pairs(e.mobile.activeMagicEffectList) do
		-- Check if the target is a vampire.
		if activeMagicEffect.effectId == tes3.effect.vampirism then
			-- Play the soul trap visual effect at the position of the target.
			activeMagicEffect.instance:playVisualEffect{
				effectIndex = 0,
				position = e.mobile.position,
				visual = "VFX_Soul_Trap"
			}
			break
		end
	end
end

event.register(tes3.event.damaged, onDamaged)