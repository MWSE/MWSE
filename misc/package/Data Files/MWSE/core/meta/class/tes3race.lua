--- @meta

--- A core object representing a character race.
--- @class tes3race : tes3baseObject
--- @field abilities tes3spellList A list of abilities that NPCs of that race have access to. It is a tes3spellList, which is a list wrapper with helper functions. The actual list is accessed with .iterator. e.g. for _, ability in pairs(race.abilities.iterator) do print(ability.name) end
--- @field baseAttributes table Array-style table access to base 8 attributes for the race. Each element in the array is a tes3raceBaseAttribute.
--- @field description string Loads from disk and returns the description of the race.
--- @field femaleBody tes3raceBodyParts Access to all the body parts that will be used for female actors of this race.
--- @field flags number Raw bit-based flags.
--- @field height tes3raceHeightWeight Access to the the height pair for males/females of the race.
--- @field isBeast boolean Access to the beast race flag.
--- @field isPlayable boolean Access to the playable race flag.
--- @field maleBody tes3raceBodyParts Access to all the body parts that will be used for male actors of this race.
--- @field name string The player-facing name for the object.
--- @field skillBonuses table Array-style table access for 7 skill bonuses for the race. Each element in the array is a tes3raceSkillBonus.
--- @field weight tes3raceHeightWeight Access to the the height pair for males/females of the race.
tes3race = {}

