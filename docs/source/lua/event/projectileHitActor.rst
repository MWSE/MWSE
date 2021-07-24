projectileHitActor
====================================================================================================

The projectileHitActor event fires when a projectile collides with an actor.

Related events: `projectileHitObject`_, `projectileHitTerrain`_, `projectileExpire`_

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

.. _`projectileExpire`: ../../lua/event/projectileExpire.html
.. _`projectileHitObject`: ../../lua/event/projectileHitObject.html
.. _`projectileHitTerrain`: ../../lua/event/projectileHitTerrain.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
