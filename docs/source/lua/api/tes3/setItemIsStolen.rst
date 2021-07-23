tes3.setItemIsStolen
====================================================================================================

This function changes an item's stolen flag. Morrowind handles stealing by marking the base item (not the inventory stack) with with NPCs that you have stolen that item from. The NPC will recognize an item is stolen if they are marked as stolen on the base item.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

item (`tes3item`_)
    The item whose stolen flag to modify.

from (`tes3baseObject`_)
    The location the item is stolen from.

stolen (`boolean`_)
    Default: ``true``. If this parameter is set to true, the item will be flagged as stolen. Otherwise, the item's stolen flag will be removed.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`tes3baseObject`: ../../../lua/type/tes3baseObject.html
.. _`tes3item`: ../../../lua/type/tes3item.html
