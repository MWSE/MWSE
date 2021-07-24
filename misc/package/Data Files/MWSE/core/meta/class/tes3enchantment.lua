--- @meta

--- An enchantment game object.
--- @class tes3enchantment : tes3object, tes3baseObject
--- @field castType number The enchantment's cast type. Maps to tes3.enchantmentType.* constants.
--- @field chargeCost number The cost of using the enchantment.
--- @field effects table An array-style table of the tes3effect data on the object.
--- @field flags number A bit field for the enchantment's flags.
--- @field maxCharge number The maximum charge for the associated enchantment.
tes3enchantment = {}

--- Creates a new enchantment object, which will be stored as part of the current saved game.
--- @param params tes3enchantment.create.params This table accepts the following values:
--- 
--- ``id``: string — The new object's ID. Must be unique if provided.
--- 
--- ``castType``: number — The enchantment castType. See tes3.enchantmentType.
--- 
--- ``chargeCost``: number — The new enchantment charge cost. Must be greater than 0.
--- 
--- ``maxCharge``: number — The new enchantment maximum charge. Must be greater than 0
--- 
--- ``flags``: number — The new enchantment flags.
--- 
--- ``objectFlags``: number — The object flags initially set. Force set as modified.
function tes3enchantment.create(params) end

---Table parameter definitions for ``tes3enchantment.create``.
--- @class tes3enchantment.create.params
--- @field id string The new object's ID. Must be unique if provided.
--- @field castType number The enchantment castType. See tes3.enchantmentType.
--- @field chargeCost number The new enchantment charge cost. Must be greater than 0.
--- @field maxCharge number The new enchantment maximum charge. Must be greater than 0
--- @field flags number The new enchantment flags.
--- @field objectFlags number The object flags initially set. Force set as modified.

--- Returns the amount of effects the tes3enchantment object has.
--- @return number count No description yet available.
function tes3enchantment:getActiveEffectCount() end

--- Returns the index of a first effect of a given effectId in the parent tes3enchantment object.
--- @param effectId number The effectID to perform a check for. Maps to tes3.effect.* constants.
--- @return number index No description yet available.
function tes3enchantment:getFirstIndexOfEffect(effectId) end

