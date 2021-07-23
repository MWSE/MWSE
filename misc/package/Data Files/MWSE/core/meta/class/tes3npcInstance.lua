--- @meta

--- An NPC object that has been cloned. Typically represents an NPC that has been instanced in the world.
--- @class tes3npcInstance : tes3actor, tes3physicalObject, tes3object, tes3baseObject
--- @field aiConfig tes3aiConfig A substructure off of actors that contains information on the current AI configuration.
--- @field attributes table Quick access to the base NPC's attributes.
--- @field barterGold number Quick access to the base NPC's base amount of barter gold.
--- @field baseObject tes3npc Access to the base NPC object.
--- @field class tes3class Quick access to the base NPC's class.
--- @field disposition number The actor's effective disposition, with all modifiers applied.
--- @field faction tes3faction Quick access to the base NPC's faction.
--- @field factionIndex number No description yet available.
--- @field fatigue number Quick access to the base NPC's fatigue.
--- @field health number Quick access to the base NPC's health.
--- @field isAttacked boolean If true, the actor's attacked flag is set.
--- @field isEssential boolean If true, the actor's essential flag is set.
--- @field isInstance boolean Always returns true.
--- @field isRespawn boolean If true, the actor's respawn flag is set.
--- @field level number Quick access to the base NPC's level.
--- @field magicka number Quick access to the base NPC's magicka.
--- @field name number Quick access to the base NPC's name.
--- @field race tes3race Quick access to the base NPC's race.
--- @field script tes3script Quick access to the base NPC's script.
--- @field skills table Quick access to the base NPC's skills.
--- @field spells tes3spellList Quick access to the base NPC's spell list. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, spell in pairs(npc.spells.iterator) do print(spell.name) end
tes3npcInstance = {}

