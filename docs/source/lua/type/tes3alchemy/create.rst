create
====================================================================================================

Creates a new alchemy item, which will be stored as part of the current saved game.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    The new object's ID. Must be unique.

name (`string`_)
    Default: ``"Potion"``. The new item's name.

script (`tes3script`_)
    Optional. A script to attach to the object.

mesh (`string`_)
    Default: ``"m\Misc_Potion_Bargain_01.nif"``. The mesh to use for the object.

icon (`string`_)
    Default: ``"m\Tx_potion_bargain_01.nif"``. The icon to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

weight (`number`_)
    Default: ``0``. The new item's weight.

value (`number`_)
    Default: ``0``. The new item's value.

flags (`number`_)
    Default: ``0``. The new alchemy item's flags.

effects (`table`_)
    A table of effects described, providing values for id, skill, attribute, range, radius, duration, min, and/or max.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
