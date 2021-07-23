create
====================================================================================================

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

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

id (`string`_)
    Optional. The new object's ID. Must be unique if provided.

name (`string`_)
    Default: ``Miscellaneous item``. The new object's name.

script (`tes3script`_)
    Optional. A script to attach to the object.

mesh (`string`_)
    Default: ````. The mesh to use for the object.

objectFlags (`number`_)
    Default: ``0``. The object flags initially set. Force set as modified.

weight  (`tes3script`_)
    Default: ``0``. The weight of the object.

value  (`string`_)
    Default: ``0``. The value of the object.

flags  (`number`_)
    Default: ``0``. The object flags initially set.

.. _`number`: ../../../lua/type/number.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3misc`: ../../../lua/type/tes3misc.html
