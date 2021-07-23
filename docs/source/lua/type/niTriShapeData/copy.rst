copy
====================================================================================================

Creates a copy of the data. An optional table of filters can be passed to remove information from the copy.

Returns
----------------------------------------------------------------------------------------------------

`niTriShapeData`_. The duplicated data.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

normals (`boolean`_)
    Default: ``true``. If false, the geometry data's normals will be absent from the copy.

colors (`boolean`_)
    Default: ``true``. If false, the geometry data's colors will be absent from the copy.

texCoords (`boolean`_)
    Default: ``true``. If false, the geometry data's texture coordinates will be absent from the copy.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`niTriShapeData`: ../../../lua/type/niTriShapeData.html
