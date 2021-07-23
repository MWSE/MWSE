--- @meta

--- An alchemy game object.
--- @class tes3alchemy : tes3physicalObject, tes3object, tes3baseObject
--- @field autoCalc boolean If set, the value of the object is automatically calculated.
--- @field effects table An array-style table of the tes3effect data on the object.
--- @field icon string The path to the object's icon.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field script tes3script The script that runs on the object.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
tes3alchemy = {}

--- Creates a new alchemy item, which will be stored as part of the current saved game.
--- @param params tes3alchemy.create.params This table accepts the following values:
--- 
--- ``id``: string — The new object's ID. Must be unique.
--- 
--- ``name``: string — The new item's name.
--- 
--- ``script``: tes3script — A script to attach to the object.
--- 
--- ``mesh``: string — The mesh to use for the object.
--- 
--- ``icon``: string — The icon to use for the object.
--- 
--- ``objectFlags``: number — The object flags initially set. Force set as modified.
--- 
--- ``weight``: number — The new item's weight.
--- 
--- ``value``: number — The new item's value.
--- 
--- ``flags``: number — The new alchemy item's flags.
--- 
--- ``effects``: table — A table of effects described, providing values for id, skill, attribute, range, radius, duration, min, and/or max.
function tes3alchemy.create(params) end

---Table parameter definitions for ``tes3alchemy.create``.
--- @class tes3alchemy.create.params
--- @field id string The new object's ID. Must be unique.
--- @field name string The new item's name.
--- @field script tes3script A script to attach to the object.
--- @field mesh string The mesh to use for the object.
--- @field icon string The icon to use for the object.
--- @field objectFlags number The object flags initially set. Force set as modified.
--- @field weight number The new item's weight.
--- @field value number The new item's value.
--- @field flags number The new alchemy item's flags.
--- @field effects table A table of effects described, providing values for id, skill, attribute, range, radius, duration, min, and/or max.

