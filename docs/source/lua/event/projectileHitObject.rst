projectileHitObject
====================================================================================================

The projectileHitObject event fires when a projectile collides with an object.

Event Data
----------------------------------------------------------------------------------------------------

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileProjectile`_. Read-only. The mobile projectile that is expiring.

firingReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the actor that fired the projectile.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the object that was hit.

collisionPoint
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3vector3`_. Read-only. The collision point of the mobile projectile.

firingWeapon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weapon`_. Read-only. The weapon that fired the projectile.

.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
