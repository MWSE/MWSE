getEffectInstance
====================================================================================================

Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.

Returns
----------------------------------------------------------------------------------------------------

`tes3magicEffectInstance`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

index (`number`_)
    The index in the effect list to fetch, between 0 and 7.

target (`tes3reference`_)
    The target actor for the effect.

.. _`number`: ../../../lua/type/number.html
.. _`tes3magicEffectInstance`: ../../../lua/type/tes3magicEffectInstance.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
