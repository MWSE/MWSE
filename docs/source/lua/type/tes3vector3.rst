tes3vector3
====================================================================================================

A simple trio of floating-point numbers.

Properties
----------------------------------------------------------------------------------------------------

`b`_ (`number`_)
    The third value in the vector. An alias for z.

`g`_ (`number`_)
    The second value in the vector. An alias for y.

`r`_ (`number`_)
    The first value in the vector. An alias for x.

`x`_ (`number`_)
    The first value in the vector.

`y`_ (`number`_)
    The second value in the vector.

`z`_ (`number`_)
    The third value in the vector.

.. toctree::
    :hidden:

    tes3vector3/b
    tes3vector3/g
    tes3vector3/r
    tes3vector3/x
    tes3vector3/y
    tes3vector3/z

.. _`b`: tes3vector3/b.html
.. _`g`: tes3vector3/g.html
.. _`r`: tes3vector3/r.html
.. _`x`: tes3vector3/x.html
.. _`y`: tes3vector3/y.html
.. _`z`: tes3vector3/z.html

Methods
----------------------------------------------------------------------------------------------------

`copy`_ (`tes3vector3`_)
    Creates a copy of the vector.

`cross`_ (`tes3vector3`_)
    Calculates the cross product with another vector.

`distance`_ (`number`_)
    Calculates the distance to another vector.

`dot`_ (`tes3vector3`_)
    Calculates the dot product with another vector.

`heightDifference`_ (`number`_)
    Calculates the vertical distance to another vector.

`interpolate`_ (`tes3vector3`_)
    Calculates the interpolated position against the target vector using the distance parameter.

`length`_ (`number`_)
    Calculates the length of the vector.

`lerp`_
    Calculates the interpolated vector between this vector and another, given a transition.

`negate`_
    Negates all values in the vector.

`normalize`_ (`bool`_)
    Normalize the vector in-place, or set its components to zero if normalization is not possible. Returns true if the vector was successfully normalized.

`normalized`_ (`tes3vector3`_)
    Get a normalized copy of the vector.

`outerProduct`_ (`tes3matrix33`_)
    Calculates the outer product with another vector.

.. toctree::
    :hidden:

    tes3vector3/copy
    tes3vector3/cross
    tes3vector3/distance
    tes3vector3/dot
    tes3vector3/heightDifference
    tes3vector3/interpolate
    tes3vector3/length
    tes3vector3/lerp
    tes3vector3/negate
    tes3vector3/normalize
    tes3vector3/normalized
    tes3vector3/outerProduct

.. _`copy`: tes3vector3/copy.html
.. _`cross`: tes3vector3/cross.html
.. _`distance`: tes3vector3/distance.html
.. _`dot`: tes3vector3/dot.html
.. _`heightDifference`: tes3vector3/heightDifference.html
.. _`interpolate`: tes3vector3/interpolate.html
.. _`length`: tes3vector3/length.html
.. _`lerp`: tes3vector3/lerp.html
.. _`negate`: tes3vector3/negate.html
.. _`normalize`: tes3vector3/normalize.html
.. _`normalized`: tes3vector3/normalized.html
.. _`outerProduct`: tes3vector3/outerProduct.html

.. _`bool`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`tes3matrix33`: ../../lua/type/tes3matrix33.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
