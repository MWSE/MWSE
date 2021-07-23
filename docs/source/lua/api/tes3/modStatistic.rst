tes3.modStatistic
====================================================================================================

Modifies a statistic on a given actor. This should be used instead of manually setting values on the game structures, to ensure that events and GUI elements are properly handled. Either skill, attribute, or name must be provided.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

attribute (`number`_)
    Optional. The attribute to set.

base (`number`_)
    Optional. If set, the base value will be modified.

current (`number`_)
    Optional. If set, the current value will be modified.

limit (`boolean`_)
    Default: ``false``. If set, the attribute won't rise above 100 or fall below 0.

name (`string`_)
    Optional. A generic name of an attribute to set.

reference (`tes3mobileActor`_, `tes3reference`_, `string`_)
    No description available.

skill (`number`_)
    Optional. The skill to set.

value (`number`_)
    Optional. If set, both the base and current value will be modified.

Examples
----------------------------------------------------------------------------------------------------

Decrease Health of an Actor
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, the current health of an actor, the player, is decreased by 10.

.. code-block:: lua

    tes3.modStatistic({
        reference = tes3.mobilePlayer,
        name = "health",
        current = -10
    })


.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
