--- @meta

--- A leveled creature game object.
--- @class tes3leveledItem : tes3physicalObject, tes3object, tes3baseObject
--- @field calculateForEachItem boolean If true, each entry is rolled in the leveled list when resolving items. This can result in multiple items from one list being added.
--- @field calculateFromAllLevels boolean If true, the leveled list is picked from the entire level range, instead of just the highest.
--- @field chanceForNothing number The percent chance, from 0 to 100, for no object to be chosen.
--- @field count number The number of possible options in the leveled object container.
--- @field flags number A numerical representation of bit flags for the object.
--- @field list tes3iterator The collection that itself, containing tes3leveledListNodes.
tes3leveledItem = {}

--- Chooses a random item from the list, based on the player's level.
--- @return tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3weapon result No description yet available.
function tes3leveledItem:pickFrom() end

