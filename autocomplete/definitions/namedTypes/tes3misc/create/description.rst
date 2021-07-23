Create a new `tes3misc`_ object which will be stored as part of the current saved game.

The example below create a misc item object that could be used to create a placeable reference later on :
.. code-block:: lua
	:linenos:

local miscItemId = "create_misc_item_id"
local miscItemName = "fabulous misc item"

local miscItem = tes3misc.create( {
  id = miscItemId,
  name = miscItemName
} )

local miscItem = tes3.createObject({
  objectType = tes3.objectType.miscItem,
  id = miscItemId,
  name = miscItemName,
  -- mesh, icons and other properties can be specified later with properties
})
