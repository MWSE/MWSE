tes3.rayTest
====================================================================================================

Performs a ray test and returns various information related to the result(s). If findAll is set, the result will be a table of results, otherwise only the first result is returned.

Returns
----------------------------------------------------------------------------------------------------

`niPickRecord`_, `table`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

position (`tes3vector3`_, `table`_)
    Position of the ray origin.

direction (`tes3vector3`_, `table`_)
    Direction of the ray. Does not have to be unit length.

findAll (`boolean`_)
    Default: ``false``. If true, the ray test won't stop after the first result.

maxDistance (`number`_)
    Optional. The maximum distance that the test will run.

sort (`boolean`_)
    Default: ``true``. If true, the results will be sorted by distance from the origin position.

useModelBounds (`boolean`_)
    Default: ``false``. If true, model bounds will be tested for intersection. Otherwise triangles will be used.

useModelCoordinates (`boolean`_)
    Default: ``false``. If true, model coordinates will be used instead of world coordinates.

useBackTriangles (`boolean`_)
    Default: ``false``. Include intersections with back-facing triangles.

observeAppCullFlag (`boolean`_)
    Default: ``true``. Ignore intersections with culled (hidden) models.

root (`node*`_)
    Default: ``tes3.game.worldSceneGraphRoot``. Node pointer to node scene.

ignoreSkinned (`boolean`_)
    Default: ``false``. Ignore results from skinned objects.

returnColor (`boolean`_)
    Default: ``false``. Calculate and return the vertex color at intersections.

returnNormal (`boolean`_)
    Default: ``true``. Calculate and return the vertex normal at intersections.

returnSmoothNormal (`boolean`_)
    Default: ``false``. Use normal interpolation for calculating vertex normals.

returnTexture (`boolean`_)
    Default: ``false``. Calculate and return the texture coordinate at intersections.

ignore (`table`_)
    Optional. An array of references and/or scene graph nodes to cull from the result(s).

Examples
----------------------------------------------------------------------------------------------------

Multiple Results
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example performs a ray test and displays all results in the entire ray test, rather than ending at the first object hit.

.. code-block:: lua

    local results = tes3.rayTest{ tes3.getCameraPosition(), direction = tes3.getCameraVector(), findAll = true }
    if results then
        for i, hit in pairs(results) do
            mwse.log("Ray hit #%d: %s", i, hit.reference or "<non-reference>");
        end
    end


Get Camera Target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example performs a ray test to see what the camera is currently looking at.

.. code-block:: lua

    local hitResult = tes3.rayTest({ position = tes3.getCameraPosition(), direction = tes3.getCameraVector() })
    local hitReference = hitResult and hitResult.reference
    if (hitReference == nil) then
        return
    end

    tes3.messageBox("The camera is looking at a '%s'", hitReference.object.name or hitReference.object.id)


Get Activation Target
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example performs a ray test to match the normal activation target test. Unlike ``tes3.getPlayerTarget()`` this will return objects not normally available for activation.

.. code-block:: lua

    local hitResult = tes3.rayTest({ position = tes3.getPlayerEyePosition(), direction = tes3.getPlayerEyeVector() })
    local hitReference = hitResult and hitResult.reference
    if (hitReference == nil) then
        return
    end

    tes3.messageBox("The player is looking at a '%s'", hitReference.object.name or hitReference.object.id)


Save rayTest result for use at a later point
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

If you plan to use the results of rayTest, you should make sure it still exists. For example, an object which was in a list of results of rayTest can get unloaded when the player changes cells and become invalid, so it shouldn't be accessed.

.. code-block:: lua

    local result = tes3.rayTest{ -- result can get invalidated
        position = tes3.getPlayerEyePosition(),
        direction = tes3.getPlayerEyeVector(),
        ignore = { tes3.player }
    }

    if result then
        local ref = tes3.makeSafeObjectHandle(result.reference)
    end

    -- Before using ref, now we can chack if it is valid
    if ref:valid() then
        -- Now we can safely do something with ref

    end


.. _`boolean`: ../../../lua/type/boolean.html
.. _`niPickRecord`: ../../../lua/type/niPickRecord.html
.. _`number`: ../../../lua/type/number.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
