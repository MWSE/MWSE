lerp
====================================================================================================

Calculates the interpolated color between this color and another, given a transition.

Returns
----------------------------------------------------------------------------------------------------

`niColor`_. The calculated value.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

tocolor (`niColor`_)
    The color to interpolate towards.

transition (`number`_)
    The interpolation value. Must be between 0.0 (closer to this color) and 1.0 (closer to the other color).

.. _`niColor`: ../../../lua/type/niColor.html
.. _`number`: ../../../lua/type/number.html
