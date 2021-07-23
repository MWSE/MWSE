tes3.loadMesh
====================================================================================================

Loads a mesh file and provides a scene graph object.

Returns
----------------------------------------------------------------------------------------------------

`niNode`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

path (`string`_)
    Path, relative to Data Files/Meshes.

useCache (`boolean`_)
    Default: ``true``. If false, a new object will be created even if it had been previously loaded.

.. _`boolean`: ../../../lua/type/boolean.html
.. _`niNode`: ../../../lua/type/niNode.html
.. _`string`: ../../../lua/type/string.html
