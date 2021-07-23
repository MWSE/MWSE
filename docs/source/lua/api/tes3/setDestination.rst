tes3.setDestination
====================================================================================================

Sets or changes the destination of a door to a new location.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_)
    The door reference that will be updated.

position (`tes3vector`_, `table`_)
    The new coordinates of the transition.

orientation (`tes3vector`_, `table`_)
    The new rotation to use after transition.

cell (`tes3cell`_, `string`_)
    Optional. The cell to transition to, if transitioning to an interior.

.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3cell`: ../../../lua/type/tes3cell.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
