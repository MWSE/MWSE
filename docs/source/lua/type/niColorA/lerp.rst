lerp
====================================================================================================

Calculates the interpolated color between this color and another, given a transition.

Returns
----------------------------------------------------------------------------------------------------

`niColorA`_. The calculated value.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

tocolor (`niColorA`_)
    The color to interpolate towards.

transition (`number`_)
    The interpolation value. Must be between 0.0 (closer to this color) and 1.0 (closer to the other color).

.. _`niColorA`: ../../../lua/type/niColorA.html
.. _`number`: ../../../lua/type/number.html
