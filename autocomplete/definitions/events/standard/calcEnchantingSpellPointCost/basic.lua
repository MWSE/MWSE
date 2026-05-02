local function onCalcEnchantingSpellPointCost(e)
    if e.castType == tes3.enchantmentType.constantEffect then
        e.spellPointCost = e.spellPointCost * 0.9
    end
end

event.register(tes3.event.calcEnchantingSpellPointCost, onCalcEnchantingSpellPointCost)
