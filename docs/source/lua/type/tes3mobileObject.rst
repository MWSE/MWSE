tes3mobileObject
====================================================================================================

The base object from which all other mobiles (AI/movement using) structures derive.

Properties
----------------------------------------------------------------------------------------------------

`boundSize`_ (`tes3vector3`_)
    Read-only. A vector that shows the size of the bounding box in each direction.

`cellX`_ (`number`_)
    Read-only. The X grid coordinate of the cell the mobile is in.

`cellY`_ (`number`_)
    Read-only. The Y grid coordinate of the cell the mobile is in.

`flags`_ (`number`_)
    Read-only. Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`height`_ (`number`_)
    Read-only. The height of the mobile above the ground.

`impulseVelocity`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

`movementCollision`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`objectType`_ (`number`_)
    Read-only. The type of mobile object. Maps to values in tes3.objectType.

`playerDistance`_ (`number`_)
    The distance to the player. Updated every frame when the mobile is in an active cell.

`position`_ (`tes3vector3`_)
    A vector that represents the 3D position of the object.

`prevMovementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.

`reference`_ (`tes3reference`_)
    Read-only. Access to the reference object for the mobile, if any.

`velocity`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.

.. toctree::
    :hidden:

    tes3mobileObject/boundSize
    tes3mobileObject/cellX
    tes3mobileObject/cellY
    tes3mobileObject/flags
    tes3mobileObject/height
    tes3mobileObject/impulseVelocity
    tes3mobileObject/movementCollision
    tes3mobileObject/movementFlags
    tes3mobileObject/objectType
    tes3mobileObject/playerDistance
    tes3mobileObject/position
    tes3mobileObject/prevMovementFlags
    tes3mobileObject/reference
    tes3mobileObject/velocity

.. _`boundSize`: tes3mobileObject/boundSize.html
.. _`cellX`: tes3mobileObject/cellX.html
.. _`cellY`: tes3mobileObject/cellY.html
.. _`flags`: tes3mobileObject/flags.html
.. _`height`: tes3mobileObject/height.html
.. _`impulseVelocity`: tes3mobileObject/impulseVelocity.html
.. _`movementCollision`: tes3mobileObject/movementCollision.html
.. _`movementFlags`: tes3mobileObject/movementFlags.html
.. _`objectType`: tes3mobileObject/objectType.html
.. _`playerDistance`: tes3mobileObject/playerDistance.html
.. _`position`: tes3mobileObject/position.html
.. _`prevMovementFlags`: tes3mobileObject/prevMovementFlags.html
.. _`reference`: tes3mobileObject/reference.html
.. _`velocity`: tes3mobileObject/velocity.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
