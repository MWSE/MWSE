tes3.hasOwnershipAccess
====================================================================================================

Determines if a reference has access to another object, including its inventory. References have access to their own things, and the player has access to dead NPC's items.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Default: ``tes3.player``. The actor to check permissions for.

target (`tes3reference`_, `tes3mobileActor`_, `string`_)
    The reference to check access of.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
