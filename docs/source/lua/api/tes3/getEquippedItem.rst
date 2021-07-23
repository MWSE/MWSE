tes3.getEquippedItem
====================================================================================================

Returns an actor's equipped item stack, provided a given filter

Returns
----------------------------------------------------------------------------------------------------

`tes3equipmentStack`_.

Parameters
----------------------------------------------------------------------------------------------------

Accepts parameters through a table with the given keys:

actor (`tes3reference`_, `tes3mobileActor`_, `tes3actor`_)
    No description available.

enchanted (`boolean`_)
    Optional. If true, filters to enchanted items.

objectType (`number`_)
    Optional. Maps to tes3.objectType constants. Used to filter equipment by type.

slot (`number`_)
    Optional. Maps to tes3.armorSlot or tes3.clothingSlot. Used to filter equipment by slot.

type (`number`_)
    Optional. Maps to tes3.weaponType. Used to filter equipment by type.

Examples
----------------------------------------------------------------------------------------------------

Get Player’s Shield
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

This example shows the player’s shield.

.. code-block:: lua

    local equippedShieldStack = tes3.getEquippedItem({ actor = tes3.player, objectType = tes3.objectType.armor, slot = tes3.armorSlot.shield })
    if (equippedShieldStack) then
        mwse.log("Equipped shield: %s", equippedShieldStack.object.id)
    else
        mwse.log("No shield equipped.")
    end


Get Player’s Equipped Light
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

In this example, we print the object ID of the player’s equipped light source.

.. code-block:: lua

    local equippedLightStack = tes3.getEquippedItem({ actor = tes3.player, objectType = tes3.objectType.light })
    if (equippedLightStack) then
        mwse.log("Equipped light: %s", equippedLightStack.object.id)
    else
        mwse.log("No light equipped.")
    end


.. _`boolean`: ../../../lua/type/boolean.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3actor`: ../../../lua/type/tes3actor.html
.. _`tes3equipmentStack`: ../../../lua/type/tes3equipmentStack.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
