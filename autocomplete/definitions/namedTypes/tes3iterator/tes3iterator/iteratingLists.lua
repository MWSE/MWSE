-- Check player equipment
for _, stack in pairs(tes3.player.object.equipment) do
    -- stack is type tes3equipmentStack, as tes3.player.object.equipment is a list of tes3equipmentStack.
    mwse.log("Equipment object ID: %s", stack.object.id)
end

-- tes3.player.object.spells is a tes3spellList with helper functions.
-- The actual list is spells.iterator.
for _, spell in pairs(tes3.player.object.spells.iterator) do
    -- spell is type tes3spell, as tes3.player.object.spells.iterator is a list of spell.
    mwse.log("Spell %s, cost %d", spell.name, spell.magickaCost)
end
