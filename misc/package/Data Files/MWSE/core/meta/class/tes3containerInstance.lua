--- @meta

--- A container object that has been cloned. Typically represents a container that has been instanced by being opened by the player.
--- @class tes3containerInstance : tes3actor, tes3physicalObject, tes3object, tes3baseObject
--- @field baseObject tes3object The base container object that the instance inherits from.
--- @field isInstance boolean Always returns true.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field organic boolean Determines if the container's organic flag is enabled.
--- @field respawns boolean Determines if the container's respawn flag is enabled.
--- @field script tes3script The script that runs on the object.
tes3containerInstance = {}

