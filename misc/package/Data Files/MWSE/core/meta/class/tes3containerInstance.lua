--- @meta

--- A container object that has been cloned. Typically represents a container that has been instanced by being opened by the player.
--- @class tes3containerInstance : tes3actor, tes3physicalObject, tes3object, tes3baseObject
--- @field baseObject tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3reference|tes3spell|tes3static|tes3weapon The base container object that the instance inherits from.
--- @field isInstance boolean Always returns true.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field organic boolean Determines if the container's organic flag is enabled.
--- @field respawns boolean Determines if the container's respawn flag is enabled.
--- @field script tes3script The script that runs on the object.
tes3containerInstance = {}

