tes3.createObject
====================================================================================================

Create an object and returns it. The created object will be part of the saved game. Supported object types are those that have their own create function, such as tes3activator for example.

Returns
----------------------------------------------------------------------------------------------------

`tes3BaseObject`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

objectType (`number`_)
    Maps to tes3.objectType constants. Used to filter object type to create.

Examples
----------------------------------------------------------------------------------------------------

Creates a tes3misc object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The example below create a misc item object that could be used to create a placeable reference later on.

.. code-block:: lua

    local miscItemId = "create_misc_item_id"
    local miscItemName = "create_misc_item_name"

    mwse.log( "creating a misc item of id %s", miscItemId )

    -- local miscItem = tes3misc.create( {
    --   id = miscItemId,
    --   name = miscItemName
    -- } )

    local miscItem = tes3.createObject({
      objectType = tes3.objectType.miscItem,
      id = miscItemId,
      name = miscItemName
    })

    if( miscItem ~= nil ) then
      mwse.log( "misc item created : id is %s", miscItemId )
    else
      mwse.log( "failed to create a misc item of id %s", miscItemId )
      return
    end

    tes3.createReference( {
      object = miscItem,
      position = tes3.getPlayerEyePosition(),
      orientation = 0,
      cell = tes3.getPlayerCell()
    } )

    mwse.log( "created a reference of a misc item of id %s", miscItem.id )


Creates a tes3static object
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

The example below create a static object that could be used to create a placeable reference later on.

.. code-block:: lua

    local staticId = "create_static_id"

    mwse.log( "creating a static of id %s", staticId )

    -- local static = tes3static.create( {
    --   id = staticId,
    --   mesh = "d\\door_dwrv_main00.nif"
    -- } )

    local static = tes3.createObject( {
      objectType = tes3.objectType.static,
      id = staticId,
      mesh = [[d\door_dwrv_main00.nif]]
    } )

    if( static ~= nil ) then
      mwse.log( "static created : id is %s", staticId )
    else
      mwse.log( "failed to create a static of id %s", staticId )
      return
    end

    tes3.createReference( {
      object = static,
      position = tes3.getPlayerEyePosition(),
      orientation = 0,
      cell = tes3.getPlayerCell()
    } )

    mwse.log( "created a reference of static of id %s", static.id )


.. _`number`: ../../../lua/type/number.html
