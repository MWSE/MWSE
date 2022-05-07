--- @meta
--- @diagnostic disable:undefined-doc-name

--- A weapon game object.
--- @class tes3weapon : tes3item, tes3physicalObject, tes3object, tes3baseObject
--- @field chopMax number Chop damage inflicted at maximum weapon swing.
--- @field chopMin number Chop damage inflicted at minimum weapon swing.
--- @field enchantCapacity number The object's enchantment capacity.
--- @field enchantment tes3enchantment The enchantment used by the object.
--- @field flags number Access to the weapon flag bitfield. Known flags can be accessed via other weapon properties.
--- @field hasDurability boolean *Read-only*. Whether the weapon type has durability. Thrown weapons and ammunition are used once fired, and therefore do not use durability.
--- @field icon string The path to the object's icon.
--- @field ignoresNormalWeaponResistance boolean Access to the flag that controls if this weapon bypasses the "Resist normal weapons" magic effect.
--- @field isAmmo boolean *Read-only*. If true, the weapon is a type of ammunition.
--- @field isMelee boolean *Read-only*. If true, the weapon has a melee weapon type.
--- @field isOneHanded boolean *Read-only*. If true, the weapon is a one-handed weapon.
--- @field isProjectile boolean *Read-only*. If true, the weapon is a projectile.
--- @field isRanged boolean *Read-only*. If true, the weapon is a ranged weapon.
--- @field isSilver boolean If true, the weapon has a silver material flag.
--- @field isTwoHanded boolean *Read-only*. If true, the weapon is a two-handed melee weapon.
--- @field maxCondition number The object's maximum condition.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field reach number The weapon's relative reach. A value of `1.0` is the standard reach. It is multiplied by the GMST `fCombatDistance` to find the attack reach in world units.
--- @field script tes3script *Read-only*. The script that runs on the object.
--- @field skill tes3skill Gets the skill data for the attack skill used by the weapon.
--- @field skillId number Gets the skill ID for the attack skill used by the weapon. Maps to values in [`tes3.skill`](https://mwse.github.io/MWSE/references/skills/) namespace.
--- @field slashMax number Slash damage inflicted at maximum weapon swing.
--- @field slashMin number Slash damage inflicted at minimum weapon swing.
--- @field speed number The relative attack speed of the weapon. Controls attack timing and animation speed. Standard speed is `1.0`.
--- @field thrustMax number Thrust damage inflicted at maximum weapon swing.
--- @field thrustMin number Thrust damage inflicted at minimum weapon swing.
--- @field type number *Read-only*. A number representing the weapon type. Matches values from the [`tes3.weaponType`](https://mwse.github.io/MWSE/references/weapon-types/) table.
--- @field typeName string *Read-only*. The player-facing name for the weapon type.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
tes3weapon = {}

--- Creates a copy of this object.
--- @param params tes3weapon.createCopy.params This table accepts the following values:
--- 
--- `id`: string? — *Optional*. The new object's ID. If one is not provided, a randomly generated one will be used.
--- 
--- `addToObjectList`: boolean? — *Default*: `true`. If true, the object will be added to the data handler. If this is false, the new object may not have a randomly generated ID. Do not use this without knowing the implications.
--- 
--- `sourceless`: boolean? — *Default*: `false`. If true, the object will be made sourceless, and will not be serialized to the save game. If the object is copied outside of a save game, the object will **always** be sourceless.
--- @return tes3weapon newObject No description yet available.
function tes3weapon:createCopy(params) end

---Table parameter definitions for `tes3weapon.createCopy`.
--- @class tes3weapon.createCopy.params
--- @field id string? *Optional*. The new object's ID. If one is not provided, a randomly generated one will be used.
--- @field addToObjectList boolean? *Default*: `true`. If true, the object will be added to the data handler. If this is false, the new object may not have a randomly generated ID. Do not use this without knowing the implications.
--- @field sourceless boolean? *Default*: `false`. If true, the object will be made sourceless, and will not be serialized to the save game. If the object is copied outside of a save game, the object will **always** be sourceless.

--- Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.
--- @param flagBit number No description yet available.
--- @return number result No description yet available.
function tes3weapon:getMaterialFlag(flagBit) end

--- Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.
--- @param flagBit number No description yet available.
--- @param value boolean No description yet available.
function tes3weapon:setMaterialFlag(flagBit, value) end
