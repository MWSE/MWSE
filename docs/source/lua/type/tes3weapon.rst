tes3weapon
====================================================================================================

A weapon game object.

Properties
----------------------------------------------------------------------------------------------------

`boundingBox`_ (`tes3boundingBox`_)
    The bounding box for the object.

`chopMax`_ (`number`_)
    Chop damage inflicted at maximum weapon swing.

`chopMin`_ (`number`_)
    Chop damage inflicted at minimum weapon swing.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`enchantCapacity`_ (`number`_)
    The object's enchantment capacity.

`enchantment`_ (`tes3enchantment`_)
    The enchantment used by the object.

`flags`_ (`number`_)
    Access to the weapon flag bitfield. Known flags can be accessed via other weapon properties.

`hasDurability`_ (`boolean`_)
    Read-only. Whether the weapon type has durability. Thrown weapons and ammunition are used once fired, and therefore do not use durability.

`icon`_ (`string`_)
    The path to the object's icon.

`id`_ (`string`_)
    The unique identifier for the object.

`ignoresNormalWeaponResistance`_ (`boolean`_)
    Access to the flag that controls if this weapon bypasses the "Resist normal weapons" magic effect.

`isAmmo`_ (`boolean`_)
    Read-only. If true, the weapon is a type of ammunition.

`isMelee`_ (`boolean`_)
    Read-only. If true, the weapon has a melee weapon type.

`isOneHanded`_ (`boolean`_)
    Read-only. If true, the weapon is a one-handed weapon.

`isProjectile`_ (`boolean`_)
    Read-only. If true, the weapon is a projectile.

`isRanged`_ (`boolean`_)
    Read-only. If true, the weapon is a ranged weapon.

`isSilver`_ (`boolean`_)
    If true, the weapon has a silver material flag.

`isTwoHanded`_ (`boolean`_)
    Read-only. If true, the weapon is a two-handed melee weapon.

`maxCondition`_ (`number`_)
    The object's maximum condition.

`mesh`_ (`string`_)
    The path to the object's mesh.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`name`_ (`string`_)
    The player-facing name for the object.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`reach`_ (`number`_)
    The weapon's relative reach. A value of 1.0 is the standard reach. It is multiplied by the GMST fCombatDistance to find the attack reach in world units.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`script`_ (`tes3script`_)
    Read-only. The script that runs on the object.

`skill`_ (`tes3skill`_)
    Gets the skill data for the attack skill used by the weapon.

`skillId`_ (`number`_)
    Gets the skill ID for the attack skill used by the weapon.

`slashMax`_ (`number`_)
    Slash damage inflicted at maximum weapon swing.

`slashMin`_ (`number`_)
    Slash damage inflicted at minimum weapon swing.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`speed`_ (`number`_)
    The relative attack speed of the weapon. Controls attack timing and animation speed. Standard speed is 1.0.

`stolenList`_ (`tes3iterator`_)
    A list of actors that the object has been stolen from.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`thrustMax`_ (`number`_)
    Thrust damage inflicted at maximum weapon swing.

`thrustMin`_ (`number`_)
    Thrust damage inflicted at minimum weapon swing.

`type`_ (`number`_)
    Read-only. A number representing the weapon type. Matches values from the tes3.weaponType table.

`typeName`_ (`string`_)
    Read-only. The player-facing name for the weapon type.

`value`_ (`number`_)
    The value of the object.

`weight`_ (`number`_)
    The weight of the object.

.. toctree::
    :hidden:

    tes3weapon/boundingBox
    tes3weapon/chopMax
    tes3weapon/chopMin
    tes3weapon/deleted
    tes3weapon/disabled
    tes3weapon/enchantCapacity
    tes3weapon/enchantment
    tes3weapon/flags
    tes3weapon/hasDurability
    tes3weapon/icon
    tes3weapon/id
    tes3weapon/ignoresNormalWeaponResistance
    tes3weapon/isAmmo
    tes3weapon/isMelee
    tes3weapon/isOneHanded
    tes3weapon/isProjectile
    tes3weapon/isRanged
    tes3weapon/isSilver
    tes3weapon/isTwoHanded
    tes3weapon/maxCondition
    tes3weapon/mesh
    tes3weapon/modified
    tes3weapon/name
    tes3weapon/nextInCollection
    tes3weapon/objectFlags
    tes3weapon/objectType
    tes3weapon/owningCollection
    tes3weapon/previousInCollection
    tes3weapon/reach
    tes3weapon/scale
    tes3weapon/sceneNode
    tes3weapon/sceneReference
    tes3weapon/script
    tes3weapon/skill
    tes3weapon/skillId
    tes3weapon/slashMax
    tes3weapon/slashMin
    tes3weapon/sourceMod
    tes3weapon/speed
    tes3weapon/stolenList
    tes3weapon/supportsLuaData
    tes3weapon/thrustMax
    tes3weapon/thrustMin
    tes3weapon/type
    tes3weapon/typeName
    tes3weapon/value
    tes3weapon/weight

.. _`boundingBox`: tes3weapon/boundingBox.html
.. _`chopMax`: tes3weapon/chopMax.html
.. _`chopMin`: tes3weapon/chopMin.html
.. _`deleted`: tes3weapon/deleted.html
.. _`disabled`: tes3weapon/disabled.html
.. _`enchantCapacity`: tes3weapon/enchantCapacity.html
.. _`enchantment`: tes3weapon/enchantment.html
.. _`flags`: tes3weapon/flags.html
.. _`hasDurability`: tes3weapon/hasDurability.html
.. _`icon`: tes3weapon/icon.html
.. _`id`: tes3weapon/id.html
.. _`ignoresNormalWeaponResistance`: tes3weapon/ignoresNormalWeaponResistance.html
.. _`isAmmo`: tes3weapon/isAmmo.html
.. _`isMelee`: tes3weapon/isMelee.html
.. _`isOneHanded`: tes3weapon/isOneHanded.html
.. _`isProjectile`: tes3weapon/isProjectile.html
.. _`isRanged`: tes3weapon/isRanged.html
.. _`isSilver`: tes3weapon/isSilver.html
.. _`isTwoHanded`: tes3weapon/isTwoHanded.html
.. _`maxCondition`: tes3weapon/maxCondition.html
.. _`mesh`: tes3weapon/mesh.html
.. _`modified`: tes3weapon/modified.html
.. _`name`: tes3weapon/name.html
.. _`nextInCollection`: tes3weapon/nextInCollection.html
.. _`objectFlags`: tes3weapon/objectFlags.html
.. _`objectType`: tes3weapon/objectType.html
.. _`owningCollection`: tes3weapon/owningCollection.html
.. _`previousInCollection`: tes3weapon/previousInCollection.html
.. _`reach`: tes3weapon/reach.html
.. _`scale`: tes3weapon/scale.html
.. _`sceneNode`: tes3weapon/sceneNode.html
.. _`sceneReference`: tes3weapon/sceneReference.html
.. _`script`: tes3weapon/script.html
.. _`skill`: tes3weapon/skill.html
.. _`skillId`: tes3weapon/skillId.html
.. _`slashMax`: tes3weapon/slashMax.html
.. _`slashMin`: tes3weapon/slashMin.html
.. _`sourceMod`: tes3weapon/sourceMod.html
.. _`speed`: tes3weapon/speed.html
.. _`stolenList`: tes3weapon/stolenList.html
.. _`supportsLuaData`: tes3weapon/supportsLuaData.html
.. _`thrustMax`: tes3weapon/thrustMax.html
.. _`thrustMin`: tes3weapon/thrustMin.html
.. _`type`: tes3weapon/type.html
.. _`typeName`: tes3weapon/typeName.html
.. _`value`: tes3weapon/value.html
.. _`weight`: tes3weapon/weight.html

Methods
----------------------------------------------------------------------------------------------------

`getMaterialFlag`_ (`number`_)
    Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.

`setMaterialFlag`_
    Access to the material flag bitfield. Known material flags can be accessed via other weapon properties.

.. toctree::
    :hidden:

    tes3weapon/getMaterialFlag
    tes3weapon/setMaterialFlag

.. _`getMaterialFlag`: tes3weapon/getMaterialFlag.html
.. _`setMaterialFlag`: tes3weapon/setMaterialFlag.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`tes3boundingBox`: ../../lua/type/tes3boundingBox.html
.. _`tes3enchantment`: ../../lua/type/tes3enchantment.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
