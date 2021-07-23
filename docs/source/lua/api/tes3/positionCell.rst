tes3.positionCell
====================================================================================================

Positions a reference to another place.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Default: ``tes3.mobilePlayer``. The reference to reposition.

cell (`tes3cell`_)
    Optional. The cell to move the reference to. If not provided, the reference will be moved to a cell in the exterior worldspace at the position provided.

position (`tes3vector3`_, `table`_)
    The position to move the reference to.

orientation (`tes3vector3`_, `table`_)
    Optional. The position to move the reference to.

forceCellChange (`boolean`_)
    Default: ``false``. When true, forces the game to update a reference that has moved within a single cell, as if it was moved into a new cell.

suppressFader (`boolean`_)
    Default: ``false``. When moving the player, can be used to prevent the fade in and out visual effect.

teleportCompanions (`boolean`_)
    Default: ``true``. If used on the player, determines if companions should also be teleported.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3cell`: ../../../lua/type/tes3cell.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
