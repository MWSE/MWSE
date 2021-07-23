tes3.addArmorSlot
====================================================================================================

Registers a new armor slot. Adds a new place for armor pieces with a matching slot number to equip to.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

slot (`number`_)
    Armor slot number. A number greater than 10 to configure a slot for.

name (`string`_)
    No description available.

weight (`number`_)
    Default: ``0``. A stand-in for the armor base weight value, typically controlled by a GMST (e.g. iHelmWeight).

scalar (`number`_)
    Default: ``0.1``. A multiplier with range 0.0-1.0 that controls how much of an item's armor value applies to a character's overall armor rating. For comparison, standard chest armor uses 0.3, helmets, greaves and pauldrons use 0.1, and gauntlets use 0.05.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
