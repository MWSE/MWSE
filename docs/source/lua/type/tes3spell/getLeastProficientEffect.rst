getLeastProficientEffect
====================================================================================================

Returns the effect of the spell that a given actor is least proficient with.

Returns
----------------------------------------------------------------------------------------------------

`tes3effect`_, `nil`_. The least proficient effect, or nil if the spell has no valid effects.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

actor (`tes3mobileActor`_, `tes3reference`_, `tes3npc`_, `tes3npcInstance`_)
    The actor to calculate for. A mobile actor is preferred, as passing only a tes3npc will not use up to date skill information.

.. _`nil`: ../../../lua/type/nil.html
.. _`tes3effect`: ../../../lua/type/tes3effect.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3npc`: ../../../lua/type/tes3npc.html
.. _`tes3npcInstance`: ../../../lua/type/tes3npcInstance.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
