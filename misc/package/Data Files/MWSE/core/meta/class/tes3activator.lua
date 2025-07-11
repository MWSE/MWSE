--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- An activator game object.
--- @class tes3activator : tes3physicalObject, tes3object, tes3baseObject
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field script tes3script The script that runs on the object.
tes3activator = {}

--- Use [`tes3.createObject`](https://mwse.github.io/MWSE/apis/tes3/#tes3createobject) instead. This function created a new activator object, which will be stored as part of the current saved game.
--- @deprecated
--- @param params tes3activator.create.params This table accepts the following values:
--- 
--- `id?`: string — *Optional*. The new object's ID. Must be unique if provided.
--- 
--- `name?`: string — *Default*: `Activator`. The new object's name.
--- 
--- `script?`: tes3script — *Optional*. A script to attach to the object.
--- 
--- `mesh?`: string — *Default*: ``. The mesh to use for the object.
--- 
--- `objectFlags?`: number — *Default*: `0`. The object flags initially set. Force set as modified.
--- @return tes3activator newObject No description yet available.
function tes3activator.create(params) end

---Table parameter definitions for `tes3activator.create`.
--- @class tes3activator.create.params
--- @field id? string *Optional*. The new object's ID. Must be unique if provided.
--- @field name? string *Default*: `Activator`. The new object's name.
--- @field script? tes3script *Optional*. A script to attach to the object.
--- @field mesh? string *Default*: ``. The mesh to use for the object.
--- @field objectFlags? number *Default*: `0`. The object flags initially set. Force set as modified.

--- Creates a copy of this object.
--- @param params tes3activator.createCopy.params This table accepts the following values:
--- 
--- `id?`: string — *Optional*. The new object's ID. If one is not provided, a randomly generated one will be used.
--- 
--- `addToObjectList?`: boolean — *Default*: `true`. If true, the object will be added to the data handler. If this is false, the new object may not have a randomly generated ID. Do not use this without knowing the implications.
--- 
--- `sourceless?`: boolean — *Default*: `false`. If true, the object will be made sourceless, and will not be serialized to the save game. If the object is copied outside of a save game, the object will **always** be sourceless.
--- @return tes3activator newObject No description yet available.
function tes3activator:createCopy(params) end

---Table parameter definitions for `tes3activator.createCopy`.
--- @class tes3activator.createCopy.params
--- @field id? string *Optional*. The new object's ID. If one is not provided, a randomly generated one will be used.
--- @field addToObjectList? boolean *Default*: `true`. If true, the object will be added to the data handler. If this is false, the new object may not have a randomly generated ID. Do not use this without knowing the implications.
--- @field sourceless? boolean *Default*: `false`. If true, the object will be made sourceless, and will not be serialized to the save game. If the object is copied outside of a save game, the object will **always** be sourceless.

