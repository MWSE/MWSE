--- @meta

--- A static game object.
--- @class tes3static : tes3physicalObject, tes3object, tes3baseObject
--- @field mesh string The path to the object's mesh.
tes3static = {}

--- Creates a new static object, which will be stored as part of the current saved game.
--- @param params tes3static.create.params This table accepts the following values:
--- 
--- ``id``: string — The new object's ID. Must be unique if provided.
--- 
--- ``mesh``: string — The mesh to use for the object.
--- 
--- ``objectFlags``: number — The object flags initially set. Force set as modified.
function tes3static.create(params) end

---Table parameter definitions for ``tes3static.create``.
--- @class tes3static.create.params
--- @field id string The new object's ID. Must be unique if provided.
--- @field mesh string The mesh to use for the object.
--- @field objectFlags number The object flags initially set. Force set as modified.

