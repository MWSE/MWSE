create
====================================================================================================

Create a new `tes3static`_ object which will be stored as part of the current saved game.

The example below create a static object that could be used to create a placeable reference later on :
.. code-block:: lua
	:linenos:

local staticId = "create_static_id"

local static = tes3static.create( {
  id = staticId,
  mesh = [[d\door_dwrv_main00.nif]]
} )

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

mesh (`string`_)
    Default: ````. The mesh to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3static`: ../../../lua/type/tes3static.html
