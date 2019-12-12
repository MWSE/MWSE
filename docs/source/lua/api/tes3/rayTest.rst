tes3.rayTest
====================================================================================================

Preforms a ray test and returns various information related to the result(s). If findAll is set, the result will be a table of results, otherwise only the first result is returned.

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
	
root (`node *`_)
	Default: ``tes3.game.worldSceneGraphRoot`` Node pointer to node scene.
	
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

    local results = tes3.rayTest{ tes3.getCameraPosition(), direction = tes3.getCameraVector(), findAll = true };
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


.. _`tes3creature`: ../../../lua/type/tes3creature.html
.. _`niObject`: ../../../lua/type/niObject.html
.. _`tes3npc`: ../../../lua/type/tes3npc.html
.. _`tes3book`: ../../../lua/type/tes3book.html
.. _`tes3matrix33`: ../../../lua/type/tes3matrix33.html
.. _`tes3actor`: ../../../lua/type/tes3actor.html
.. _`tes3inputConfig`: ../../../lua/type/tes3inputConfig.html
.. _`tes3itemStack`: ../../../lua/type/tes3itemStack.html
.. _`tes3globalVariable`: ../../../lua/type/tes3globalVariable.html
.. _`tes3containerInstance`: ../../../lua/type/tes3containerInstance.html
.. _`tes3magicSourceInstance`: ../../../lua/type/tes3magicSourceInstance.html
.. _`niAVObject`: ../../../lua/type/niAVObject.html
.. _`tes3iterator`: ../../../lua/type/tes3iterator.html
.. _`tes3raceHeightWeight`: ../../../lua/type/tes3raceHeightWeight.html
.. _`tes3class`: ../../../lua/type/tes3class.html
.. _`tes3mobileProjectile`: ../../../lua/type/tes3mobileProjectile.html
.. _`tes3apparatus`: ../../../lua/type/tes3apparatus.html
.. _`tes3door`: ../../../lua/type/tes3door.html
.. _`tes3directInputMouseState`: ../../../lua/type/tes3directInputMouseState.html
.. _`niRTTI`: ../../../lua/type/niRTTI.html
.. _`niObjectNET`: ../../../lua/type/niObjectNET.html
.. _`tes3armor`: ../../../lua/type/tes3armor.html
.. _`tes3npcInstance`: ../../../lua/type/tes3npcInstance.html
.. _`tes3dataHandler`: ../../../lua/type/tes3dataHandler.html
.. _`tes3rangeInt`: ../../../lua/type/tes3rangeInt.html
.. _`tes3dialogueInfo`: ../../../lua/type/tes3dialogueInfo.html
.. _`tes3dialogue`: ../../../lua/type/tes3dialogue.html
.. _`tes3gameFile`: ../../../lua/type/tes3gameFile.html
.. _`tes3faction`: ../../../lua/type/tes3faction.html
.. _`tes3wearablePart`: ../../../lua/type/tes3wearablePart.html
.. _`tes3inputController`: ../../../lua/type/tes3inputController.html
.. _`tes3lockpick`: ../../../lua/type/tes3lockpick.html
.. _`tes3combatSession`: ../../../lua/type/tes3combatSession.html
.. _`boolean`: ../../../lua/type/boolean.html
.. _`tes3vector4`: ../../../lua/type/tes3vector4.html
.. _`tes3magicEffect`: ../../../lua/type/tes3magicEffect.html
.. _`string`: ../../../lua/type/string.html
.. _`tes3referenceList`: ../../../lua/type/tes3referenceList.html
.. _`tes3iteratorNode`: ../../../lua/type/tes3iteratorNode.html
.. _`tes3fader`: ../../../lua/type/tes3fader.html
.. _`tes3quest`: ../../../lua/type/tes3quest.html
.. _`tes3nonDynamicData`: ../../../lua/type/tes3nonDynamicData.html
.. _`tes3ingredient`: ../../../lua/type/tes3ingredient.html
.. _`tes3race`: ../../../lua/type/tes3race.html
.. _`tes3gameSetting`: ../../../lua/type/tes3gameSetting.html
.. _`tes3vector2`: ../../../lua/type/tes3vector2.html
.. _`table`: ../../../lua/type/table.html
.. _`tes3travelDestinationNode`: ../../../lua/type/tes3travelDestinationNode.html
.. _`tes3transform`: ../../../lua/type/tes3transform.html
.. _`tes3mobileNPC`: ../../../lua/type/tes3mobileNPC.html
.. _`tes3soulGemData`: ../../../lua/type/tes3soulGemData.html
.. _`tes3vector3`: ../../../lua/type/tes3vector3.html
.. _`tes3reference`: ../../../lua/type/tes3reference.html
.. _`tes3raceSkillBonus`: ../../../lua/type/tes3raceSkillBonus.html
.. _`tes3activator`: ../../../lua/type/tes3activator.html
.. _`tes3raceBodyParts`: ../../../lua/type/tes3raceBodyParts.html
.. _`tes3inventory`: ../../../lua/type/tes3inventory.html
.. _`tes3boundingBox`: ../../../lua/type/tes3boundingBox.html
.. _`tes3markData`: ../../../lua/type/tes3markData.html
.. _`tes3raceBaseAttribute`: ../../../lua/type/tes3raceBaseAttribute.html
.. _`tes3creatureInstance`: ../../../lua/type/tes3creatureInstance.html
.. _`tes3effect`: ../../../lua/type/tes3effect.html
.. _`tes3game`: ../../../lua/type/tes3game.html
.. _`tes3probe`: ../../../lua/type/tes3probe.html
.. _`tes3physicalObject`: ../../../lua/type/tes3physicalObject.html
.. _`tes3object`: ../../../lua/type/tes3object.html
.. _`nil`: ../../../lua/type/nil.html
.. _`number`: ../../../lua/type/number.html
.. _`tes3moon`: ../../../lua/type/tes3moon.html
.. _`tes3mobilePlayer`: ../../../lua/type/tes3mobilePlayer.html
.. _`tes3mobileObject`: ../../../lua/type/tes3mobileObject.html
.. _`tes3misc`: ../../../lua/type/tes3misc.html
.. _`tes3leveledListNode`: ../../../lua/type/tes3leveledListNode.html
.. _`tes3mobileCreature`: ../../../lua/type/tes3mobileCreature.html
.. _`tes3mobileActor`: ../../../lua/type/tes3mobileActor.html
.. _`function`: ../../../lua/type/function.html
.. _`tes3magicEffectInstance`: ../../../lua/type/tes3magicEffectInstance.html
.. _`tes3baseObject`: ../../../lua/type/tes3baseObject.html
.. _`tes3bodyPart`: ../../../lua/type/tes3bodyPart.html
.. _`tes3factionRank`: ../../../lua/type/tes3factionRank.html
.. _`mwseTimer`: ../../../lua/type/mwseTimer.html
.. _`tes3container`: ../../../lua/type/tes3container.html
.. _`tes3packedColor`: ../../../lua/type/tes3packedColor.html
.. _`bool`: ../../../lua/type/boolean.html
.. _`tes3equipmentStack`: ../../../lua/type/tes3equipmentStack.html
.. _`tes3clothing`: ../../../lua/type/tes3clothing.html
.. _`mwseTimerController`: ../../../lua/type/mwseTimerController.html
.. _`tes3leveledCreature`: ../../../lua/type/tes3leveledCreature.html
.. _`tes3lockNode`: ../../../lua/type/tes3lockNode.html
.. _`tes3activeMagicEffect`: ../../../lua/type/tes3activeMagicEffect.html
.. _`tes3cellExteriorData`: ../../../lua/type/tes3cellExteriorData.html
.. _`tes3light`: ../../../lua/type/tes3light.html
.. _`tes3leveledItem`: ../../../lua/type/tes3leveledItem.html
.. _`tes3alchemy`: ../../../lua/type/tes3alchemy.html
.. _`tes3enchantment`: ../../../lua/type/tes3enchantment.html
.. _`tes3cell`: ../../../lua/type/tes3cell.html
.. _`tes3actionData`: ../../../lua/type/tes3actionData.html
.. _`tes3itemData`: ../../../lua/type/tes3itemData.html
.. _`tes3factionReaction`: ../../../lua/type/tes3factionReaction.html
