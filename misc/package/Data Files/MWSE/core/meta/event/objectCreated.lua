--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This event is called when a new object is created.
--- @class objectCreatedEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field copiedFrom tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3object|tes3probe|tes3reference|tes3repairTool|tes3spell|tes3static|tes3weapon *Read-only*. If the object was created by first copying another object, that object is available here.
--- @field object tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3baseObject|tes3birthsign|tes3bodyPart|tes3book|tes3cell|tes3class|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3dialogue|tes3dialogueInfo|tes3door|tes3enchantment|tes3faction|tes3gameSetting|tes3globalVariable|tes3ingredient|tes3land|tes3landTexture|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3magicSourceInstance|tes3misc|tes3npc|tes3npcInstance|tes3pathGrid|tes3probe|tes3quest|tes3race|tes3reference|tes3region|tes3repairTool|tes3script|tes3skill|tes3sound|tes3soundGenerator|tes3spell|tes3startScript|tes3static|tes3weapon *Read-only*. The object that was created.
