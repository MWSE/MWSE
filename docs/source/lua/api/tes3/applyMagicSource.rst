tes3.applyMagicSource
====================================================================================================



Returns
----------------------------------------------------------------------------------------------------

`tes3magicSourceInstance`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    No description available.

source (`tes3object`_)
    Optional. A magic source to apply.

name (`string`_)
    Optional. If applying alchemy as a source, you can specifiy a name for the magic source.

effects (`table`_)
    Optional. A table of custom effects to apply as a potion. Maximal number of effects is 8.

createCopy (`boolean`_)
    Default: ``false``. This parameter controls whether the function will return the original magic source or a copy of the magic source. This parameter is only used if source is alchemy.

fromStack (`tes3equipmentStack`_)
    Optional. The piece of equipment this magic source is coming from. The fromStack has to be an already equipped item from tes3actor.equipment. This will probably change in the future.

castChance (`number`_)
    Default: ``This parameter allows overriding the casting chance of the magic source.``. No description available.

target (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. The target of the magic.

bypassResistances (`boolean`_)
    Default: ``false``. Is this effect going to bypass magic resistance?

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3equipmentStack`: ../../../lua/type/tes3equipmentStack.html
.. _`tes3magicSourceInstance`: ../../../lua/type/tes3magicSourceInstance.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3object`: ../../../lua/type/tes3object.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
