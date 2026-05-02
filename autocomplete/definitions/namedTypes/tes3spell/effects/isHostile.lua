
--- @param magicSource tes3spell|tes3enchantment|tes3alchemy
local function isSpellHostile(magicSource)
	for _, effect in ipairs(magicSource.effects) do
		if (effect.object.isHarmful) then
			-- If one of the spell's effects is harmful, then
			-- `true` is returned and function ends here.
			return true
		end
	end
	-- If no harmful effect was found then return `false`.
	return false
end
