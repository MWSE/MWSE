tes3.setOwner
====================================================================================================

This function sets the owner of a reference.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

reference (`tes3reference`_, `tes3mobileActor`_, `string`_)
    A reference whose owner to set.

remove (`boolean`_)
    Default: ``false``. If this parameter is set to true, reference's owner field will be removed.

owner (`tes3npc`_, `tes3faction`_, `string`_)
    Assigns this NPC or a faction as the owner of the reference.

requiredGlobal (`tes3globalVariable`_)
    Optional. If owner is set to NPC, requiredGlobal variable can be set.

requiredRank (`number`_)
    Default: ``0``. If owner is set to faction, requitedRank variable controls minimal rank in faction the player has to have to be able to freely take the reference.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3faction`: ../../../lua/type/tes3faction.html
.. _`tes3globalVariable`: ../../../lua/type/tes3globalVariable.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3npc`: ../../../lua/type/tes3npc.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
