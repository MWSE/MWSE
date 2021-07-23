projectileHitActor
====================================================================================================

The projectileHitActor event fires when a projectile collides with an actor.

Event Data
----------------------------------------------------------------------------------------------------

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the actor that was hit.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileProjectile`_. Read-only. The mobile projectile that is expiring.

firingWeapon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weapon`_. Read-only. The weapon that fired the projectile.

firingReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the actor that fired the projectile.

.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
