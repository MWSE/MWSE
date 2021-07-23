cameraHeight
====================================================================================================

`number`_. Access to the player camera height, used for both the first person and third person camera (specifically the orbit centre). It is normally placed at head height by the game, on loading, or when a race is selected. The camera height can be reset to its regular value by setting this property to nil.

Examples
----------------------------------------------------------------------------------------------------

Adjust the camera to match shrunken player scale
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

.. code-block:: lua

    local function shrink()
        -- Shrink player.
        tes3.player.scale = 0.5
        -- Reset camera height to default value.
        tes3.mobilePlayer.cameraHeight = nil
        -- Modify camera height to match scale.
        tes3.mobilePlayer.cameraHeight = tes3.player.scale * tes3.mobilePlayer.cameraHeight
    end

    local function unshrink()
        tes3.player.scale = 1
        tes3.mobilePlayer.cameraHeight = nil
    end


.. _`number`: ../../../lua/type/number.html
