calcArmorRating
====================================================================================================

This event is fired before an actor's armor rating has been calculated, and can be used to override the armor that the actor is given.

Event Data
----------------------------------------------------------------------------------------------------

mobile
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3mobileActor`_. Read-only. The mobile whose AR is being calculated. May not always be available.

armor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3armor`_. Read-only. The armor piece whose AR is being calculated.

armorRating
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`number`_. If set, the given armor rating will be used instead of the one calculated.

reference
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3reference`_. Read-only. A shortcut to the mobile's reference. May not always be available.

npc
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

`tes3npc`_. Read-only. The NPC object whose AR is being calculated. May not always be available.

.. _`number`: ../../lua/type/number.html
.. _`tes3armor`: ../../lua/type/tes3armor.html
.. _`tes3mobileActor`: ../../lua/type/tes3mobileActor.html
.. _`tes3npc`: ../../lua/type/tes3npc.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
