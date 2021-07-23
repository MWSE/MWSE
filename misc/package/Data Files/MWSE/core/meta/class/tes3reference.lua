--- @meta

--- A reference is a sort of container structure for objects. It holds a base object, as well as various variables associated with that object that make it unique.
--- 
--- For example, many doors may share the same base object. However, each door reference might have a different owner, different lock/trap statuses, etc. that make the object unique.
--- @class tes3reference : tes3object, tes3baseObject
--- @field activationReference tes3reference The current reference, if any, that this reference will activate.
--- @field attachments table A table with friendly named access to all supported attachments.
--- @field baseObject tes3physicalObject This is similar to the object field, but is guaranteed to provide the base-most object. If object is an actor clone, the base actor will be given instead.
--- @field bodyPartManager tes3bodyPartManager|nil Access to the reference's body part manager, if available. Typically this is only available on NPC references.
--- @field cell tes3cell The cell that the reference is currently in.
--- @field context tes3scriptContext Access to the script context for this reference and its associated script.
--- @field data table A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For item references, this is the same table as on the tes3itemData structure. To store data that doesn't get serialized to/from the save, use tempData.
--- @field destination tes3travelDestinationNode|nil Returns the travel destination node for this reference, or nil. This can be used to determine where a given door links to.
--- @field facing number Convenient access to the z-component of the reference's orientation. Setting the facing sets the reference as modified.
--- @field hasNoCollision boolean Sets the no-collision flag on this reference, and recalculates collision groups. Use the setNoCollisionFlag function to manage collision group recalculation instead.
--- @field isDead boolean|nil Returns true if the object is dead, false if they are alive, or nil if that couldn't be determined.
--- @field isEmpty boolean Friendly access onto the reference's empty inventory flag.
--- @field isLeveledSpawn boolean If true, this reference was created as a result of a leveled spawn.
--- @field isRespawn boolean If true, the references respawn flag is set.
--- @field itemData tes3itemData Gets or sets the attached itemData for this reference. If set to nil, the item data will be unhooked but not deleted.
--- @field leveledBaseReference tes3reference|nil If this reference is a leveled spawn, this is the leveled creature spawn reference. If this reference wasn't the result of a leveled spawn, the value is nil.
--- @field light niPointLight Direct access to the scene graph light, if a dynamic light is set.
--- @field lockNode tes3lockNode Quick access to the reference's lock node, if any.
--- @field mobile tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3mobileProjectile|tes3mobileSpell|nil Access to the attached mobile object, if applicable.
--- @field nextNode tes3reference The next reference in the parent reference list.
--- @field nodeData tes3reference Redundant access to this object, for iterating over a tes3referenceList.
--- @field object tes3physicalObject The object that the reference is for, such as a weapon, armor, or actor.
--- @field orientation tes3vector3 Access to the reference's orientation. Setting the orientation sets the reference as modified.
--- @field position tes3vector3 Access to the reference's position. Setting the position sets the reference as modified.
--- @field previousNode tes3reference The previous reference in the parent reference list.
--- @field sceneNode niNode The scene graph node that the reference uses for rendering.
--- @field stackSize number Access to the size of a stack, if the reference represents one or more items.
--- @field supportsLuaData boolean If true, this reference can store temporary or persistent lua data.
--- @field tempData table As with the data field, a generic lua table that data can be written to. No information in this table will persist into saves. For item references, this is the same table as on the tes3itemData structure.
tes3reference = {}

--- Causes this reference to activate another. This will lead them to go through doors, pick up items, etc.
--- @param reference tes3reference The other reference to activate.
function tes3reference:activate(reference) end

--- Unsets a bit in the reference's action data attachment
--- @param flagIndex number The action flag to clear.
function tes3reference:clearActionFlag(flagIndex) end

--- Clones a reference for a base actor into a reference to an instance of that actor. For example, this will force a container to resolve its leveled items and have its own unique inventory.
--- @return boolean result No description yet available.
function tes3reference:clone() end

--- Deletes the dynamic light attachment, if it exists. This will automatically detach the dynamic light from affected nodes.
--- @param removeLightFromParent boolean If true, the dynamic light is removed from its parent node as well.
function tes3reference:deleteDynamicLightAttachment(removeLightFromParent) end

--- Removes the dynamic light from any affected scene graph nodes, but will not delete the associated attachment.
function tes3reference:detachDynamicLightFromAffectedNodes() end

--- Hides the reference, stops all scripts for it, detaches all dynamic lights and sounds, and closes any mobile simulation. Sets the reference as modified implicitly.
--- @return any result No description yet available.
function tes3reference:disable() end

--- Shows the reference, restarts all scripts for it, reattaches all dynamic lights and sounds, and restarts any mobile simulation if the player is close enough. Sets the reference as modified implicitly.
--- @return any result No description yet available.
function tes3reference:enable() end

--- Calculates the angle from this reference's current facing to the target reference.
--- @param reference tes3reference The reference to calculate the angle to.
--- @return any result No description yet available.
function tes3reference:getAngleTo(reference) end

--- Fetches the dynamic light attachment.
--- @return tes3lightNode result No description yet available.
function tes3reference:getAttachedDynamicLight() end

--- Fetches the dynamic light attachment. If there isn't one, a new one will be created with the given light and value.
--- 
---     If no light is supplied as an argument, a point light of radius 512 will be automatically created.
---     
---     If the light is not attached to any part of the scene graph yet, the point light will be placed as a child of the "attachLight" subnode of the model, or a child of the model if "attachLight" is not found.
--- @param light niPointLight No description yet available.
--- @param phase number No description yet available.
--- @return tes3lightNode result No description yet available.
function tes3reference:getOrCreateAttachedDynamicLight(light, phase) end

--- A function that tells the game that an inventory has been closed, if you are simulating inventory operations. It has the capability of un-cloning an inventory if it has not been modified. Typically not used outside of specific purposes.
--- @return boolean result No description yet available.
function tes3reference:onCloseInventory() end

--- Sets a bit in the reference's action data attachment
--- @param flagIndex number The action flag to clear.
function tes3reference:setActionFlag(flagIndex) end

--- Sets the dynamic lighting state of the reference using the global data handler.
function tes3reference:setDynamicLighting() end

--- Sets if this reference has active collision. This is preferable to the hasNoCollision property, if are manipulating multiple objects at once and do not want to constantly recalculate collision groups.
--- @param hasNoCollision boolean If true, the reference no longer has collision.
--- @param updateCollisions boolean If true, collision groups for the active cells are recalculated.
function tes3reference:setNoCollisionFlag(hasNoCollision, updateCollisions) end

--- Returns the flag's value in the reference's action data attachment
--- @param flagIndex number The action flag to clear.
--- @return boolean result No description yet available.
function tes3reference:testActionFlag(flagIndex) end

--- Causes the reference, if of an actor, to reevaluate its equipment choices and equip items it should.
function tes3reference:updateEquipment() end

--- Updates the lighting of the reference using the global data handler.
function tes3reference:updateLighting() end

--- Updates the reference's local rotation matrix, propagates position changes to the scene graph, and sets the reference's modified flag.
function tes3reference:updateSceneGraph() end

