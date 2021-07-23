projectileHitTerrain
====================================================================================================

The projectileHitTerrain event fires when a projectile collides with terrain.

Event Data
----------------------------------------------------------------------------------------------------

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileProjectile`_. Read-only. The mobile projectile that is expiring.

position
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3vector3`_. Read-only. The position of the mobile projectile.

firingWeapon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weapon`_. Read-only. The weapon that fired the projectile.

collisionPoint
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3vector3`_. Read-only. The collision point of the mobile projectile.

firingReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the actor that fired the projectile.

velocity
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3vector3`_. Read-only. The velocity of the mobile projectile.

.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
