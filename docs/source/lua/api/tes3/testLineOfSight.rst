tes3.testLineOfSight
====================================================================================================

Checks if there is a clear line of sight between two references, or two points in space, in the same method that the game uses for actor vision. You should pass two references, or two positions and heights.

Only certain object types count as blocking for the purposes of the test:
Statics, activators, containers, doors, and lights with non-zero weight.

The collision root node is used for testing, if present in the model.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference1 (`tes3reference`_)
    Position of the starting point of the LoS check. For actors, this point is set to position of this reference's head. For other objects the starting point is at the top of its bounding box.

reference2 (`tes3reference`_)
    Position of the ending point of the LoS check. For actors, this point is set to position of this reference's head. For other objects the ending point is at the top of its bounding box.

position1 (`tes3vector3`_, `table`_)
    Position of the starting point of the LoS check. Modified by height1.

height1 (`tes3vector3`_, `table`_)
    Optional. Moves the starting point upwards (+Z direction) by this amount. Normally used to simulate head height from a position that is on the ground. Defaults to 0.

position2 (`tes3vector3`_, `table`_)
    Position of the ending point of the LoS check. Modified by height2.

height2 (`tes3vector3`_, `table`_)
    Optional. Moves the starting point upwards (+Z direction) by this amount. Normally used to simulate head height from a position that is on the ground. Defaults to 0.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
