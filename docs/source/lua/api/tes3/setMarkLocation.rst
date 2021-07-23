tes3.setMarkLocation
====================================================================================================

This function sets the player's mark location to the one specified.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

position (`tes3vector3`_)
    Coordinates of the mark's position.

rotation (`number`_)
    Default: ``Player's current rotation.``. This argument controls which direction the player's mark location will be facing.

cell (`tes3cell`_)
    Optional. A cell in which the mark should be placed.

.. _`number`: ../../../lua/type/number.html
.. _`tes3cell`: ../../../lua/type/tes3cell.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
