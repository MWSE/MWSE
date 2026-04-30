local function onMagicAbsorb(e)
    if e.target == tes3.player then
        e.absorbChance = math.max(0, e.absorbChance - 25)
    end
end
event.register(tes3.event.magicAbsorb, onMagicAbsorb)
