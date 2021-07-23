Create a new `tes3static`_ object which will be stored as part of the current saved game.

The example below create a static object that could be used to create a placeable reference later on :
.. code-block:: lua
	:linenos:

local staticId = "create_static_id"

local static = tes3static.create( {
  id = staticId,
  mesh = [[d\door_dwrv_main00.nif]]
} )
