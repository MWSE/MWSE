tes3.random
====================================================================================================

Returns a value from Morrowind's random number generator. This is not preferrable to lua's math.random function, but may be necessary for reproducing Morrowind's generation.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

seed (`number`_)
    Optional. If provided, it the number generator is seeded with this value. Pointers to objects may be used, such as a reference's sceneNode, to create a consistent if less than random seed.

.. _`number`: ../../../lua/type/number.html
