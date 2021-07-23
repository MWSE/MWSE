table.traverse
====================================================================================================

This function is used to iterate over values and its subtables in the table.

Each "node" is an object with a children table of other "nodes", each of which might have their own children. For example, a sceneNode is made up of niNodes, and each niNodes can have a list of niNode children. This is best used for recursive data structures like UI elements and sceneNodes etc.

Returns
----------------------------------------------------------------------------------------------------

`iterator`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters in the following order:

t (`table`_)
    A table to transverse.

k (`unknown`_)
    Default: ``children``. The key of a table inside t object.

Examples
----------------------------------------------------------------------------------------------------

Iterate over all scene nodes attached to player.
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In the example below, function onLoaded() will be called when the game has been successfully loaded.

For each scene nodes attached to the player, its type (node.RTTI.name) and name (node.name), will be printed to MWSE.log.

.. code-block:: lua

    local function onLoaded()
        mwse.log("Player's scene graph:")
        for node in table.traverse({tes3.player.sceneNode}) do
            mwse.log("%s : %s", node.RTTI.name, node.name)
        end
    end

    event.register("loaded", onLoaded)


.. _`table`: ../../../lua/type/table.html
