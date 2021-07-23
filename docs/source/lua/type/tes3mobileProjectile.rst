tes3mobileProjectile
====================================================================================================

A mobile object for a physical projectile.

Properties
----------------------------------------------------------------------------------------------------

`animTime`_ (`number`_)
    The time index for the projectile model's animation controllers.

`attackSwing`_ (`number`_)
    The attack swing value of the attacker at the time of releasing the projectile.

`boundSize`_ (`tes3vector3`_)
    Read-only. A vector that shows the size of the bounding box in each direction.

`cellX`_ (`number`_)
    Read-only. The X grid coordinate of the cell the mobile is in.

`cellY`_ (`number`_)
    Read-only. The Y grid coordinate of the cell the mobile is in.

`damage`_ (`number`_)
    Access to the raw damage that this projectile will inflict.

`expire`_ (`number`_)
    A flag that causes the projectile to expire when set to 1.

`firingMobile`_ (`tes3mobileActor`_)
    Read-only. The mobile that fired this projectile.

`firingWeapon`_ (`tes3weapon`_)
    Read-only. The weapon that fired this projectile.

`flags`_ (`number`_)
    Read-only. Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`height`_ (`number`_)
    Read-only. The height of the mobile above the ground.

`impulseVelocity`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

`initialSpeed`_ (`number`_)
    Read-only. The initial speed that the projectile was launched at.

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

`velocity`_ (`tes3vector3`_)
    Access to the current velocity of the projectile.

.. toctree::
    :hidden:

    tes3mobileProjectile/animTime
    tes3mobileProjectile/attackSwing
    tes3mobileProjectile/boundSize
    tes3mobileProjectile/cellX
    tes3mobileProjectile/cellY
    tes3mobileProjectile/damage
    tes3mobileProjectile/expire
    tes3mobileProjectile/firingMobile
    tes3mobileProjectile/firingWeapon
    tes3mobileProjectile/flags
    tes3mobileProjectile/height
    tes3mobileProjectile/impulseVelocity
    tes3mobileProjectile/initialSpeed
    tes3mobileProjectile/movementCollision
    tes3mobileProjectile/movementFlags
    tes3mobileProjectile/objectType
    tes3mobileProjectile/playerDistance
    tes3mobileProjectile/position
    tes3mobileProjectile/prevMovementFlags
    tes3mobileProjectile/reference
    tes3mobileProjectile/velocity
    tes3mobileProjectile/velocity

.. _`animTime`: tes3mobileProjectile/animTime.html
.. _`attackSwing`: tes3mobileProjectile/attackSwing.html
.. _`boundSize`: tes3mobileProjectile/boundSize.html
.. _`cellX`: tes3mobileProjectile/cellX.html
.. _`cellY`: tes3mobileProjectile/cellY.html
.. _`damage`: tes3mobileProjectile/damage.html
.. _`expire`: tes3mobileProjectile/expire.html
.. _`firingMobile`: tes3mobileProjectile/firingMobile.html
.. _`firingWeapon`: tes3mobileProjectile/firingWeapon.html
.. _`flags`: tes3mobileProjectile/flags.html
.. _`height`: tes3mobileProjectile/height.html
.. _`impulseVelocity`: tes3mobileProjectile/impulseVelocity.html
.. _`initialSpeed`: tes3mobileProjectile/initialSpeed.html
.. _`movementCollision`: tes3mobileProjectile/movementCollision.html
.. _`movementFlags`: tes3mobileProjectile/movementFlags.html
.. _`objectType`: tes3mobileProjectile/objectType.html
.. _`playerDistance`: tes3mobileProjectile/playerDistance.html
.. _`position`: tes3mobileProjectile/position.html
.. _`prevMovementFlags`: tes3mobileProjectile/prevMovementFlags.html
.. _`reference`: tes3mobileProjectile/reference.html
.. _`velocity`: tes3mobileProjectile/velocity.html
.. _`velocity`: tes3mobileProjectile/velocity.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
