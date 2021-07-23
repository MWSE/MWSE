--- @meta

--- An activator game object.
--- @class tes3activator : tes3physicalObject, tes3object, tes3baseObject
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field script tes3script The script that runs on the object.
tes3activator = {}

--- Creates a new activator object, which will be stored as part of the current saved game.
--- @param params tes3activator.create.params This table accepts the following values:
--- 
--- ``id``: string — The new object's ID. Must be unique if provided.
--- 
--- ``name``: string — The new object's name.
--- 
--- ``script``: tes3script — A script to attach to the object.
--- 
--- ``mesh``: string — The mesh to use for the object.
--- 
--- ``objectFlags``: number — The object flags initially set. Force set as modified.
function tes3activator.create(params) end

---Table parameter definitions for ``tes3activator.create``.
--- @class tes3activator.create.params
--- @field id string The new object's ID. Must be unique if provided.
--- @field name string The new object's name.
--- @field script tes3script A script to attach to the object.
--- @field mesh string The mesh to use for the object.
--- @field objectFlags number The object flags initially set. Force set as modified.

