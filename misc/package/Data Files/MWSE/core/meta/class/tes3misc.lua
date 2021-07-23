--- @meta

--- A core miscellaneous object.
--- @class tes3misc : tes3physicalObject, tes3object, tes3baseObject
--- @field icon string The path to the object's icon.
--- @field isKey boolean Access to the flag determining if this item is recognized as a key.
--- @field isSoulGem boolean Determines if this item is a soul gem.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field script tes3script The script that runs on the object.
--- @field soulGemData tes3soulGemData Fetches related soul gem data, if this item is a soul gem.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
tes3misc = {}

--- Creates a new misc item object, which will be stored as part of the current saved game.
--- @param params tes3misc.create.params This table accepts the following values:
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
--- 
--- ``weight ``: tes3script — The weight of the object.
--- 
--- ``value ``: string — The value of the object.
--- 
--- ``flags ``: number — The object flags initially set.
function tes3misc.create(params) end

---Table parameter definitions for ``tes3misc.create``.
--- @class tes3misc.create.params
--- @field id string The new object's ID. Must be unique if provided.
--- @field name string The new object's name.
--- @field script tes3script A script to attach to the object.
--- @field mesh string The mesh to use for the object.
--- @field objectFlags number The object flags initially set. Force set as modified.
--- @field weight  tes3script The weight of the object.
--- @field value  string The value of the object.
--- @field flags  number The object flags initially set.

