tes3.persuade
====================================================================================================

Attempts a persuasion attempt on an actor, potentially adjusting their disposition. Returns true if the attempt was a success.

Returns
----------------------------------------------------------------------------------------------------

`boolean`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

actor (`tes3mobileActor`_, `tes3reference`_, `string`_)
    The actor to try to persuade.

index (`number`_)
    Optional. If an index is provided, 0-indexed with the following results: admire, intimidate, taunt, bribe (10), bribe (100), bribe (1000).

modifier (`number`_)
    Optional. If no index is provided, this is the direct modifier to try.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
