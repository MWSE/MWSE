--- @meta

--- A leveled creature game object.
--- @class tes3leveledCreature : tes3physicalObject, tes3object, tes3baseObject
--- @field calculateFromAllLevels boolean If true, the leveled list is picked from the entire level range, instead of just the highest.
--- @field chanceForNothing number The percent chance, from 0 to 100, for no object to be chosen.
--- @field count number The number of possible options in the leveled object container.
--- @field flags number A numerical representation of bit flags for the object.
--- @field list tes3iterator The collection that itself, containing tes3leveledListNodes.
tes3leveledCreature = {}

--- Chooses a random item from the list, based on the player's level.
--- @return tes3creature result No description yet available.
function tes3leveledCreature:pickFrom() end

