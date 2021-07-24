projectileExpire
====================================================================================================

The projectileExpire event fires just prior to a fired projectile expiring.

Related events: `projectileHitActor`_, `projectileHitObject`_, `projectileHitTerrain`_

Event Data
----------------------------------------------------------------------------------------------------

firingWeapon
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3weapon`_. Read-only. The weapon that fired the projectile.

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileProjectile`_. Read-only. The mobile projectile that is expiring.

firingReference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. Reference to the actor that fired the projectile.

.. _`projectileHitActor`: ../../lua/event/projectileHitActor.html
.. _`projectileHitObject`: ../../lua/event/projectileHitObject.html
.. _`projectileHitTerrain`: ../../lua/event/projectileHitTerrain.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3weapon`: ../../lua/type/tes3weapon.html
