--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This event is triggered when the player selects any spell or enchant, or when the current magic selection is deselected.
--- @class magicSelectionChangedEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field item tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3object|tes3probe|tes3reference|tes3repairTool|tes3spell|tes3static|tes3weapon|nil *Read-only*. When the source is an enchantment, this is the item that holds the enchantment.
--- @field source tes3alchemy|tes3enchantment|tes3spell|nil *Read-only*. The magic source. When magic is deselected, this is nil.
