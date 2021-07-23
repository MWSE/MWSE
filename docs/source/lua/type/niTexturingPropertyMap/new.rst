new
====================================================================================================

Creates a new basic or bump map.

Returns
----------------------------------------------------------------------------------------------------

`niTexturingPropertyMap`_. The created basic or bump map.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

texture (`niTexture`_)
    Optional. If provided, sets the map texture to the given value.

clampMode (`number`_)
    Default: ``WRAP_S_WRAP_T``. The clamp mode to set the map to use.

filterMode (`number`_)
    Default: ``TRILERP``. The filter mode to set the map to use.

textCoords (`number`_)
    Default: ``0``. The texture coordinates to set the map to use.

isBumpMap (`boolean`_)
    Default: ``false``. If true, a bump map will be created instead.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`niTexturingPropertyMap`: ../../../lua/type/niTexturingPropertyMap.html
.. _`number`: ../../../lua/type/number.html
