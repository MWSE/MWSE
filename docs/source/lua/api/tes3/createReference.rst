tes3.createReference
====================================================================================================

Similar to mwscript's PlaceAtPC or PlaceAtMe, this creates a new reference in the game world.

Returns
----------------------------------------------------------------------------------------------------

`tes3reference`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

object (`tes3physicalObject`_, `string`_)
    The object to create a reference of.

position (`tes3vector3`_, `table`_)
    The location to create the reference at.

orientation (`tes3vector3`_, `table`_)
    The new orientation for the created reference.

cell (`tes3cell`_, `string`_, `table`_)
    Optional. The cell to create the reference in. This is only needed for interior cells.

scale (`number`_)
    Default: ``1``. A scale for the reference.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3cell`: ../../../lua/type/tes3cell.html
.. _`tes3physicalObject`: ../../../lua/type/tes3physicalObject.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
