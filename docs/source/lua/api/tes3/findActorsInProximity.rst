tes3.findActorsInProximity
====================================================================================================

Searches for active mobile actors which are within ``range`` distance from the ``reference`` or ``position`` argument. It only finds mobiles which have active AI, and can include the player. This function is used by the game for area-of-effect hits. It has a small amount of overhead, so try not to use it too much.

Returns
----------------------------------------------------------------------------------------------------

`table`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    Optional. The position to search from, taken from a reference. Uses the position of the centre of the body if the reference is an actor.

position (`tes3vector3`_)
    Optional. The position to search from.

range (`number`_)
    The distance around the position to search. An actor is included if the centre of their body is in range.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
