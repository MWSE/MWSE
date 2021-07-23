tes3.findClosestExteriorReferenceOfObject
====================================================================================================

Using the same engine function used by the vanilla intervention spells, locations the first reference of a given object in the exterior world.

Returns
----------------------------------------------------------------------------------------------------

`tes3reference`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

object (`tes3physicalObject`_, `string`_)
    The object to search for.

position (`tes3vector3`_)
    Optional. The position to search from. Must be an exterior position. Defaults to the last exterior position of the player if no position is provided.

.. _`string`: ../../../lua/type/string.html
.. _`tes3physicalObject`: ../../../lua/type/tes3physicalObject.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
