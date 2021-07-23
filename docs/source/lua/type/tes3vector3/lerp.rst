lerp
====================================================================================================

Calculates the interpolated vector between this vector and another, given a transition.

Returns
----------------------------------------------------------------------------------------------------

`tes3vector3`_. The calculated value.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

toVector (`tes3vector3`_)
    The vector to interpolate towards.

transition (`number`_)
    The interpolation value. Must be between 0.0 (closer to this vector) and 1.0 (closer to the other vector).

.. _`number`: ../../../lua/type/number.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
