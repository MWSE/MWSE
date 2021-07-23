tes3mobileSpellProjectile
====================================================================================================

A mobile object for a spell projectile.

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

`rotationSpeed`_ (`number`_)
    The angular speed that the projectile rotates around its local Y axis.

`velocity`_ (`tes3vector3`_)
    Access to the current velocity of the projectile.

`velocity`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.

.. toctree::
    :hidden:

    tes3mobileSpellProjectile/animTime
    tes3mobileSpellProjectile/attackSwing
    tes3mobileSpellProjectile/boundSize
    tes3mobileSpellProjectile/cellX
    tes3mobileSpellProjectile/cellY
    tes3mobileSpellProjectile/damage
    tes3mobileSpellProjectile/expire
    tes3mobileSpellProjectile/firingMobile
    tes3mobileSpellProjectile/firingWeapon
    tes3mobileSpellProjectile/flags
    tes3mobileSpellProjectile/height
    tes3mobileSpellProjectile/impulseVelocity
    tes3mobileSpellProjectile/initialSpeed
    tes3mobileSpellProjectile/movementCollision
    tes3mobileSpellProjectile/movementFlags
    tes3mobileSpellProjectile/objectType
    tes3mobileSpellProjectile/playerDistance
    tes3mobileSpellProjectile/position
    tes3mobileSpellProjectile/prevMovementFlags
    tes3mobileSpellProjectile/reference
    tes3mobileSpellProjectile/rotationSpeed
    tes3mobileSpellProjectile/velocity
    tes3mobileSpellProjectile/velocity

.. _`animTime`: tes3mobileSpellProjectile/animTime.html
.. _`attackSwing`: tes3mobileSpellProjectile/attackSwing.html
.. _`boundSize`: tes3mobileSpellProjectile/boundSize.html
.. _`cellX`: tes3mobileSpellProjectile/cellX.html
.. _`cellY`: tes3mobileSpellProjectile/cellY.html
.. _`damage`: tes3mobileSpellProjectile/damage.html
.. _`expire`: tes3mobileSpellProjectile/expire.html
.. _`firingMobile`: tes3mobileSpellProjectile/firingMobile.html
.. _`firingWeapon`: tes3mobileSpellProjectile/firingWeapon.html
.. _`flags`: tes3mobileSpellProjectile/flags.html
.. _`height`: tes3mobileSpellProjectile/height.html
.. _`impulseVelocity`: tes3mobileSpellProjectile/impulseVelocity.html
.. _`initialSpeed`: tes3mobileSpellProjectile/initialSpeed.html
.. _`movementCollision`: tes3mobileSpellProjectile/movementCollision.html
.. _`movementFlags`: tes3mobileSpellProjectile/movementFlags.html
.. _`objectType`: tes3mobileSpellProjectile/objectType.html
.. _`playerDistance`: tes3mobileSpellProjectile/playerDistance.html
.. _`position`: tes3mobileSpellProjectile/position.html
.. _`prevMovementFlags`: tes3mobileSpellProjectile/prevMovementFlags.html
.. _`reference`: tes3mobileSpellProjectile/reference.html
.. _`rotationSpeed`: tes3mobileSpellProjectile/rotationSpeed.html
.. _`velocity`: tes3mobileSpellProjectile/velocity.html
.. _`velocity`: tes3mobileSpellProjectile/velocity.html

Methods
----------------------------------------------------------------------------------------------------

`explode`_
    Detonates the projectile, as if it hit a static object. The effects of detonation are not applied immediately, but later in the frame, at the same time as projectile collisions. This ensures consistent behaviour of the magic system.

.. toctree::
    :hidden:

    tes3mobileSpellProjectile/explode

.. _`explode`: tes3mobileSpellProjectile/explode.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
