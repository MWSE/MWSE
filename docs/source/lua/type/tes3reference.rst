tes3reference
====================================================================================================

A reference is a sort of container structure for objects. It holds a base object, as well as various variables associated with that object that make it unique.

For example, many doors may share the same base object. However, each door reference might have a different owner, different lock/trap statuses, etc. that make the object unique.

Properties
----------------------------------------------------------------------------------------------------

`activationReference`_ (`tes3reference`_)
    The current reference, if any, that this reference will activate.

`attachments`_ (`table`_)
    Read-only. A table with friendly named access to all supported attachments.

`baseObject`_ (`tes3physicalObject`_)
    Read-only. This is similar to the object field, but is guaranteed to provide the base-most object. If object is an actor clone, the base actor will be given instead.

`bodyPartManager`_ (`tes3bodyPartManager`_, `nil`_)
    Read-only. Access to the reference's body part manager, if available. Typically this is only available on NPC references.

`cell`_ (`tes3cell`_)
    Read-only. The cell that the reference is currently in.

`context`_ (`tes3scriptContext`_)
    Read-only. Access to the script context for this reference and its associated script.

`data`_ (`table`_)
    A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For item references, this is the same table as on the tes3itemData structure. To store data that doesn't get serialized to/from the save, use tempData.

`deleted`_ (`boolean`_)
    The deleted state of the object.

`destination`_ (`tes3travelDestinationNode`_, `nil`_)
    Read-only. Returns the travel destination node for this reference, or nil. This can be used to determine where a given door links to.

`disabled`_ (`boolean`_)
    The disabled state of the object.

`facing`_ (`number`_)
    Convenient access to the z-component of the reference's orientation. Setting the facing sets the reference as modified.

`hasNoCollision`_ (`boolean`_)
    Sets the no-collision flag on this reference, and recalculates collision groups. Use the setNoCollisionFlag function to manage collision group recalculation instead.

`id`_ (`string`_)
    The unique identifier for the object.

`isDead`_ (`boolean`_, `nil`_)
    Read-only. Returns true if the object is dead, false if they are alive, or nil if that couldn't be determined.

`isEmpty`_ (`boolean`_)
    Friendly access onto the reference's empty inventory flag.

`isLeveledSpawn`_ (`boolean`_)
    Read-only. If true, this reference was created as a result of a leveled spawn.

`isRespawn`_ (`boolean`_)
    Read-only. If true, the references respawn flag is set.

`itemData`_ (`tes3itemData`_)
    Gets or sets the attached itemData for this reference. If set to nil, the item data will be unhooked but not deleted.

`leveledBaseReference`_ (`tes3reference`_, `nil`_)
    Read-only. If this reference is a leveled spawn, this is the leveled creature spawn reference. If this reference wasn't the result of a leveled spawn, the value is nil.

`light`_ (`niPointLight`_)
    Read-only. Direct access to the scene graph light, if a dynamic light is set.

`lockNode`_ (`tes3lockNode`_)
    Read-only. Quick access to the reference's lock node, if any.

`mobile`_ (`tes3mobileCreature`_, `tes3mobileNPC`_, `tes3mobilePlayer`_, `tes3mobileProjectile`_, `tes3mobileSpell`_, `nil`_)
    Read-only. Access to the attached mobile object, if applicable.

`modified`_ (`boolean`_)
    The modification state of the object since the last save.

`nextInCollection`_ (`tes3object`_)
    The next object in parent collection's list.

`nextNode`_ (`tes3reference`_)
    Read-only. The next reference in the parent reference list.

`nodeData`_ (`tes3reference`_)
    Read-only. Redundant access to this object, for iterating over a tes3referenceList.

`object`_ (`tes3physicalObject`_)
    Read-only. The object that the reference is for, such as a weapon, armor, or actor.

`objectFlags`_ (`number`_)
    The raw flags of the object.

`objectType`_ (`number`_)
    The type of object. Maps to values in tes3.objectType.

`orientation`_ (`tes3vector3`_)
    Access to the reference's orientation. Setting the orientation sets the reference as modified.

`owningCollection`_ (`tes3referenceList`_)
    The collection responsible for holding this object.

`position`_ (`tes3vector3`_)
    Access to the reference's position. Setting the position sets the reference as modified.

`previousInCollection`_ (`tes3object`_)
    The previous object in parent collection's list.

`previousNode`_ (`tes3reference`_)
    Read-only. The previous reference in the parent reference list.

`scale`_ (`number`_)
    The object's scale.

`sceneNode`_ (`niNode`_)
    The scene graph node for this object.

`sceneNode`_ (`niNode`_)
    Read-only. The scene graph node that the reference uses for rendering.

`sceneReference`_ (`niNode`_)
    The scene graph reference node for this object.

`sourceMod`_ (`string`_)
    The filename of the mod that owns this object.

`stackSize`_ (`number`_)
    Access to the size of a stack, if the reference represents one or more items.

`supportsLuaData`_ (`boolean`_)
    If true, references of this object can store temporary or persistent lua data.

`supportsLuaData`_ (`boolean`_)
    If true, this reference can store temporary or persistent lua data.

`tempData`_ (`table`_)
    As with the data field, a generic lua table that data can be written to. No information in this table will persist into saves. For item references, this is the same table as on the tes3itemData structure.

.. toctree::
    :hidden:

    tes3reference/activationReference
    tes3reference/attachments
    tes3reference/baseObject
    tes3reference/bodyPartManager
    tes3reference/cell
    tes3reference/context
    tes3reference/data
    tes3reference/deleted
    tes3reference/destination
    tes3reference/disabled
    tes3reference/facing
    tes3reference/hasNoCollision
    tes3reference/id
    tes3reference/isDead
    tes3reference/isEmpty
    tes3reference/isLeveledSpawn
    tes3reference/isRespawn
    tes3reference/itemData
    tes3reference/leveledBaseReference
    tes3reference/light
    tes3reference/lockNode
    tes3reference/mobile
    tes3reference/modified
    tes3reference/nextInCollection
    tes3reference/nextNode
    tes3reference/nodeData
    tes3reference/object
    tes3reference/objectFlags
    tes3reference/objectType
    tes3reference/orientation
    tes3reference/owningCollection
    tes3reference/position
    tes3reference/previousInCollection
    tes3reference/previousNode
    tes3reference/scale
    tes3reference/sceneNode
    tes3reference/sceneNode
    tes3reference/sceneReference
    tes3reference/sourceMod
    tes3reference/stackSize
    tes3reference/supportsLuaData
    tes3reference/supportsLuaData
    tes3reference/tempData

.. _`activationReference`: tes3reference/activationReference.html
.. _`attachments`: tes3reference/attachments.html
.. _`baseObject`: tes3reference/baseObject.html
.. _`bodyPartManager`: tes3reference/bodyPartManager.html
.. _`cell`: tes3reference/cell.html
.. _`context`: tes3reference/context.html
.. _`data`: tes3reference/data.html
.. _`deleted`: tes3reference/deleted.html
.. _`destination`: tes3reference/destination.html
.. _`disabled`: tes3reference/disabled.html
.. _`facing`: tes3reference/facing.html
.. _`hasNoCollision`: tes3reference/hasNoCollision.html
.. _`id`: tes3reference/id.html
.. _`isDead`: tes3reference/isDead.html
.. _`isEmpty`: tes3reference/isEmpty.html
.. _`isLeveledSpawn`: tes3reference/isLeveledSpawn.html
.. _`isRespawn`: tes3reference/isRespawn.html
.. _`itemData`: tes3reference/itemData.html
.. _`leveledBaseReference`: tes3reference/leveledBaseReference.html
.. _`light`: tes3reference/light.html
.. _`lockNode`: tes3reference/lockNode.html
.. _`mobile`: tes3reference/mobile.html
.. _`modified`: tes3reference/modified.html
.. _`nextInCollection`: tes3reference/nextInCollection.html
.. _`nextNode`: tes3reference/nextNode.html
.. _`nodeData`: tes3reference/nodeData.html
.. _`object`: tes3reference/object.html
.. _`objectFlags`: tes3reference/objectFlags.html
.. _`objectType`: tes3reference/objectType.html
.. _`orientation`: tes3reference/orientation.html
.. _`owningCollection`: tes3reference/owningCollection.html
.. _`position`: tes3reference/position.html
.. _`previousInCollection`: tes3reference/previousInCollection.html
.. _`previousNode`: tes3reference/previousNode.html
.. _`scale`: tes3reference/scale.html
.. _`sceneNode`: tes3reference/sceneNode.html
.. _`sceneNode`: tes3reference/sceneNode.html
.. _`sceneReference`: tes3reference/sceneReference.html
.. _`sourceMod`: tes3reference/sourceMod.html
.. _`stackSize`: tes3reference/stackSize.html
.. _`supportsLuaData`: tes3reference/supportsLuaData.html
.. _`supportsLuaData`: tes3reference/supportsLuaData.html
.. _`tempData`: tes3reference/tempData.html

Methods
----------------------------------------------------------------------------------------------------

`activate`_
    Causes this reference to activate another. This will lead them to go through doors, pick up items, etc.

`clearActionFlag`_
    Unsets a bit in the reference's action data attachment

`clone`_ (`boolean`_)
    Clones a reference for a base actor into a reference to an instance of that actor. For example, this will force a container to resolve its leveled items and have its own unique inventory.

`deleteDynamicLightAttachment`_
    Deletes the dynamic light attachment, if it exists. This will automatically detach the dynamic light from affected nodes.

`detachDynamicLightFromAffectedNodes`_
    Removes the dynamic light from any affected scene graph nodes, but will not delete the associated attachment.

`disable`_
    Hides the reference, stops all scripts for it, detaches all dynamic lights and sounds, and closes any mobile simulation. Sets the reference as modified implicitly.

`enable`_
    Shows the reference, restarts all scripts for it, reattaches all dynamic lights and sounds, and restarts any mobile simulation if the player is close enough. Sets the reference as modified implicitly.

`getAngleTo`_
    Calculates the angle from this reference's current facing to the target reference.

`getAttachedDynamicLight`_ (`tes3lightNode`_)
    Fetches the dynamic light attachment.

`getOrCreateAttachedDynamicLight`_ (`tes3lightNode`_)
    Fetches the dynamic light attachment. If there isn't one, a new one will be created with the given light and value.

    If no light is supplied as an argument, a point light of radius 512 will be automatically created.
    
    If the light is not attached to any part of the scene graph yet, the point light will be placed as a child of the "attachLight" subnode of the model, or a child of the model if "attachLight" is not found.

`onCloseInventory`_ (`boolean`_)
    A function that tells the game that an inventory has been closed, if you are simulating inventory operations. It has the capability of un-cloning an inventory if it has not been modified. Typically not used outside of specific purposes.

`setActionFlag`_
    Sets a bit in the reference's action data attachment

`setDynamicLighting`_
    Sets the dynamic lighting state of the reference using the global data handler.

`setNoCollisionFlag`_
    Sets if this reference has active collision. This is preferable to the hasNoCollision property, if are manipulating multiple objects at once and do not want to constantly recalculate collision groups.

`testActionFlag`_ (`boolean`_)
    Returns the flag's value in the reference's action data attachment

`updateEquipment`_
    Causes the reference, if of an actor, to reevaluate its equipment choices and equip items it should.

`updateLighting`_
    Updates the lighting of the reference using the global data handler.

`updateSceneGraph`_
    Updates the reference's local rotation matrix, propagates position changes to the scene graph, and sets the reference's modified flag.

.. toctree::
    :hidden:

    tes3reference/activate
    tes3reference/clearActionFlag
    tes3reference/clone
    tes3reference/deleteDynamicLightAttachment
    tes3reference/detachDynamicLightFromAffectedNodes
    tes3reference/disable
    tes3reference/enable
    tes3reference/getAngleTo
    tes3reference/getAttachedDynamicLight
    tes3reference/getOrCreateAttachedDynamicLight
    tes3reference/onCloseInventory
    tes3reference/setActionFlag
    tes3reference/setDynamicLighting
    tes3reference/setNoCollisionFlag
    tes3reference/testActionFlag
    tes3reference/updateEquipment
    tes3reference/updateLighting
    tes3reference/updateSceneGraph

.. _`activate`: tes3reference/activate.html
.. _`clearActionFlag`: tes3reference/clearActionFlag.html
.. _`clone`: tes3reference/clone.html
.. _`deleteDynamicLightAttachment`: tes3reference/deleteDynamicLightAttachment.html
.. _`detachDynamicLightFromAffectedNodes`: tes3reference/detachDynamicLightFromAffectedNodes.html
.. _`disable`: tes3reference/disable.html
.. _`enable`: tes3reference/enable.html
.. _`getAngleTo`: tes3reference/getAngleTo.html
.. _`getAttachedDynamicLight`: tes3reference/getAttachedDynamicLight.html
.. _`getOrCreateAttachedDynamicLight`: tes3reference/getOrCreateAttachedDynamicLight.html
.. _`onCloseInventory`: tes3reference/onCloseInventory.html
.. _`setActionFlag`: tes3reference/setActionFlag.html
.. _`setDynamicLighting`: tes3reference/setDynamicLighting.html
.. _`setNoCollisionFlag`: tes3reference/setNoCollisionFlag.html
.. _`testActionFlag`: tes3reference/testActionFlag.html
.. _`updateEquipment`: tes3reference/updateEquipment.html
.. _`updateLighting`: tes3reference/updateLighting.html
.. _`updateSceneGraph`: tes3reference/updateSceneGraph.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`niNode`: ../../lua/type/niNode.html
.. _`niPointLight`: ../../lua/type/niPointLight.html
.. _`nil`: ../../lua/type/nil.html
.. _`number`: ../../lua/type/number.html
.. _`string`: ../../lua/type/string.html
.. _`table`: ../../lua/type/table.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3itemData`: ../../lua/type/tes3itemData.html
.. _`tes3lockNode`: ../../lua/type/tes3lockNode.html
.. _`tes3mobileCreature`: ../../lua/type/tes3mobileCreature.html
.. _`tes3mobileNPC`: ../../lua/type/tes3mobileNPC.html
.. _`tes3mobilePlayer`: ../../lua/type/tes3mobilePlayer.html
.. _`tes3mobileProjectile`: ../../lua/type/tes3mobileProjectile.html
.. _`tes3object`: ../../lua/type/tes3object.html
.. _`tes3physicalObject`: ../../lua/type/tes3physicalObject.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3referenceList`: ../../lua/type/tes3referenceList.html
.. _`tes3travelDestinationNode`: ../../lua/type/tes3travelDestinationNode.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
