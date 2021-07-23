--- @meta

--- A container object that has not been cloned. Typically represents the raw information edited in the construction set.
--- @class tes3container : tes3actor, tes3physicalObject, tes3object, tes3baseObject
--- @field capacity number The amount of weight that the container can hold.
--- @field isInstance boolean Always returns false.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field organic boolean Determines if the container's organic flag is enabled.
--- @field respawns boolean Determines if the container's respawn flag is enabled.
--- @field script tes3script The script that runs on the object.
tes3container = {}

