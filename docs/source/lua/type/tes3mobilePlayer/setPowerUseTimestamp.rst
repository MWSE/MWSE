setPowerUseTimestamp
====================================================================================================

Sets the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

power (`tes3spell`_)
    The spell object for the power.

timestamp (`number`_)
    The timestamp of the moment the power was casted, or 24 hours before the recharge point. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().

.. _`number`: ../../../lua/type/number.html
.. _`tes3spell`: ../../../lua/type/tes3spell.html
