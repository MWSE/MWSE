getPowerUseTimestamp
====================================================================================================

Finds the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().

Returns
----------------------------------------------------------------------------------------------------

`number`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

power (`tes3spell`_)
    The spell object for the power.

.. _`number`: ../../../lua/type/number.html
.. _`tes3spell`: ../../../lua/type/tes3spell.html
