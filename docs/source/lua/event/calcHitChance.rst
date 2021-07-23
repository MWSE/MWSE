calcHitChance
====================================================================================================

This event is raised when determining the hit chance for an actor.

Event Data
----------------------------------------------------------------------------------------------------

attackerMobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile who is making the attack.

targetMobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile who is being attacked. May not always be available.

target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A shortcut to the target mobile's reference. May not always be available.

hitChance
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. The hit chance for the actor. This may be adjusted.

attacker
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A shortcut to the mobile's reference.

.. _`number`: ../../lua/type/number.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
