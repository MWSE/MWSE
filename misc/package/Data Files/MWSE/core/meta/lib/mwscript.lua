--- @meta

--- The mwscript library allows vanilla mwscript functions to be called. This is not always ideal, and this library is deprecated. Avoid using it if at all possible.
--- @class mwscriptlib
mwscript = {}

--- Wrapper for the Activate mwscript function.
--- @param params mwscript.activate.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
function mwscript.activate(params) end

---Table parameter definitions for ``mwscript.activate``.
--- @class mwscript.activate.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.

--- Wrapper for the AddItem mwscript function.
--- @param params mwscript.addItem.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — The item to be added.
--- 
--- ``count``: number — The number of items to be added.
--- @return any result No description yet available.
function mwscript.addItem(params) end

---Table parameter definitions for ``mwscript.addItem``.
--- @class mwscript.addItem.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string The item to be added.
--- @field count number The number of items to be added.

--- Wrapper for the AddSoulGem mwscript function.
--- @param params mwscript.addSoulGem.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``creature``: tes3creature|string — The creature to be stored in the soul gem.
--- 
--- ``soulgem``: tes3misc|string — The soul gem to store the soul in.
--- @return any result No description yet available.
function mwscript.addSoulGem(params) end

---Table parameter definitions for ``mwscript.addSoulGem``.
--- @class mwscript.addSoulGem.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field creature tes3creature|string The creature to be stored in the soul gem.
--- @field soulgem tes3misc|string The soul gem to store the soul in.

--- Wrapper for the AddItem mwscript function.
--- @param params mwscript.addSpell.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``spell``: tes3spell|string — No description yet available.
--- @return any result No description yet available.
function mwscript.addSpell(params) end

---Table parameter definitions for ``mwscript.addSpell``.
--- @class mwscript.addSpell.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Wrapper for the AddToLevCreature mwscript function.
--- @param params mwscript.addToLevCreature.params This table accepts the following values:
--- 
--- ``list``: tes3leveledCreature|string — Leveled creature list to add a creature to.
--- 
--- ``creature``: tes3actor|string — Creature to add to the list.
--- 
--- ``level``: number — Minimum level for the creature to spawn.
--- @return any result No description yet available.
function mwscript.addToLevCreature(params) end

---Table parameter definitions for ``mwscript.addToLevCreature``.
--- @class mwscript.addToLevCreature.params
--- @field list tes3leveledCreature|string Leveled creature list to add a creature to.
--- @field creature tes3actor|string Creature to add to the list.
--- @field level number Minimum level for the creature to spawn.

--- Wrapper for the AddToLevItem mwscript function.
--- @param params mwscript.addToLevItem.params This table accepts the following values:
--- 
--- ``list``: tes3leveledItem|string — Leveled item list to add a creature to.
--- 
--- ``item``: tes3item|string — Item to add to the list.
--- 
--- ``level``: number — Minimum level for the item to spawn.
--- @return any result No description yet available.
function mwscript.addToLevItem(params) end

---Table parameter definitions for ``mwscript.addToLevItem``.
--- @class mwscript.addToLevItem.params
--- @field list tes3leveledItem|string Leveled item list to add a creature to.
--- @field item tes3item|string Item to add to the list.
--- @field level number Minimum level for the item to spawn.

--- Wrapper for the AddItem mwscript function.
--- @param params mwscript.addTopic.params This table accepts the following values:
--- 
--- ``topic``: tes3dialogue|string — No description yet available.
--- @return any result No description yet available.
function mwscript.addTopic(params) end

---Table parameter definitions for ``mwscript.addTopic``.
--- @class mwscript.addTopic.params
--- @field topic tes3dialogue|string No description yet available.

--- Wrapper for the AITravel mwscript function.
--- @param params mwscript.aiTravel.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``x``: number — No description yet available.
--- 
--- ``y``: number — No description yet available.
--- 
--- ``z``: number — No description yet available.
--- @return any result No description yet available.
function mwscript.aiTravel(params) end

---Table parameter definitions for ``mwscript.aiTravel``.
--- @class mwscript.aiTravel.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field x number No description yet available.
--- @field y number No description yet available.
--- @field z number No description yet available.

--- Wrapper for the Disable mwscript function.
--- @param params mwscript.disable.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``modify``: boolean — Set the object as modified.
--- @return any result No description yet available.
function mwscript.disable(params) end

---Table parameter definitions for ``mwscript.disable``.
--- @class mwscript.disable.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field modify boolean Set the object as modified.

--- Wrapper for the Drop mwscript function.
--- @param params mwscript.drop.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — The item to be dropped.
--- 
--- ``count``: number — The number of items to drop.
--- @return any result No description yet available.
function mwscript.drop(params) end

---Table parameter definitions for ``mwscript.drop``.
--- @class mwscript.drop.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string The item to be dropped.
--- @field count number The number of items to drop.

--- Wrapper for the Enable mwscript function.
--- @param params mwscript.enable.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``modify``: boolean — Set the object as modified.
--- @return any result No description yet available.
function mwscript.enable(params) end

---Table parameter definitions for ``mwscript.enable``.
--- @class mwscript.enable.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field modify boolean Set the object as modified.

--- Wrapper for the Equip mwscript function.
--- @param params mwscript.equip.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — The item to be equipped.
--- @return any result No description yet available.
function mwscript.equip(params) end

---Table parameter definitions for ``mwscript.equip``.
--- @class mwscript.equip.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string The item to be equipped.

--- Wrapper for the ExplodeSpell mwscript function.
--- @param params mwscript.explodeSpell.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``spell``: tes3spell|string — No description yet available.
--- @return any result No description yet available.
function mwscript.explodeSpell(params) end

---Table parameter definitions for ``mwscript.explodeSpell``.
--- @class mwscript.explodeSpell.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Wrapper for the GetButtonPressed mwscript function.
--- @return any result No description yet available.
function mwscript.getButtonPressed() end

--- Wrapper for the GetDetected mwscript function.
--- @param params mwscript.getDetected.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``target``: tes3reference|tes3mobileActor|string — Actor to check detection for.
--- @return boolean result No description yet available.
function mwscript.getDetected(params) end

---Table parameter definitions for ``mwscript.getDetected``.
--- @class mwscript.getDetected.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|string Actor to check detection for.

--- Wrapper for the GetDisabled mwscript function.
--- @param params mwscript.getDisabled.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @return boolean result No description yet available.
function mwscript.getDisabled(params) end

---Table parameter definitions for ``mwscript.getDisabled``.
--- @class mwscript.getDisabled.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.

--- Wrapper for the GetDistance mwscript function.
--- @param params mwscript.getDistance.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``target``: tes3reference|tes3mobileActor|string — Actor to check distance to.
--- @return boolean result No description yet available.
function mwscript.getDistance(params) end

---Table parameter definitions for ``mwscript.getDistance``.
--- @class mwscript.getDistance.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|string Actor to check distance to.

--- Wrapper for the GetItemCount mwscript function.
--- @param params mwscript.getItemCount.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — No description yet available.
--- @return number result No description yet available.
function mwscript.getItemCount(params) end

---Table parameter definitions for ``mwscript.getItemCount``.
--- @class mwscript.getItemCount.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string No description yet available.

--- Wrapper for the GetPCJumping mwscript function.
--- @return boolean result No description yet available.
function mwscript.getPCJumping() end

--- Wrapper for the GetPCRunning mwscript function.
--- @return boolean result No description yet available.
function mwscript.getPCRunning() end

--- Wrapper for the GetPCSneaking mwscript function.
--- @return boolean result No description yet available.
function mwscript.getPCSneaking() end

--- Returns the script target for the currently running Morrowind script, if any.
--- @return tes3reference result No description yet available.
function mwscript.getReference() end

--- Returns the currently running Morrowind script, if any.
--- @return tes3script result No description yet available.
function mwscript.getScript() end

--- Wrapper for the GetSpellEffects mwscript function.
--- @param params mwscript.getSpellEffects.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``spell``: tes3spell|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.getSpellEffects(params) end

---Table parameter definitions for ``mwscript.getSpellEffects``.
--- @class mwscript.getSpellEffects.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Wrapper for the HasItemEquipped mwscript function.
--- @param params mwscript.hasItemEquipped.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — The item to be added.
--- @return boolean result No description yet available.
function mwscript.hasItemEquipped(params) end

---Table parameter definitions for ``mwscript.hasItemEquipped``.
--- @class mwscript.hasItemEquipped.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string The item to be added.

--- Wrapper for the PlaceAtPC mwscript function.
--- @param params mwscript.placeAtPC.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``object``: tes3object|string — The object to place.
--- 
--- ``count``: number — No description yet available.
--- 
--- ``distance``: number — No description yet available.
--- 
--- ``direction``: number — No description yet available.
--- @return any result No description yet available.
function mwscript.placeAtPC(params) end

---Table parameter definitions for ``mwscript.placeAtPC``.
--- @class mwscript.placeAtPC.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field object tes3object|string The object to place.
--- @field count number No description yet available.
--- @field distance number No description yet available.
--- @field direction number No description yet available.

--- Wrapper for the PlaySound mwscript function.
--- @param params mwscript.playSound.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``sound``: tes3sound|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.playSound(params) end

---Table parameter definitions for ``mwscript.playSound``.
--- @class mwscript.playSound.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field sound tes3sound|string No description yet available.

--- Wrapper for the Position mwscript function.
--- @param params mwscript.position.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``cell``: string — No description yet available.
--- 
--- ``x``: number — No description yet available.
--- 
--- ``y``: number — No description yet available.
--- 
--- ``z``: number — No description yet available.
--- 
--- ``rotation``: number — No description yet available.
--- @return any result No description yet available.
function mwscript.position(params) end

---Table parameter definitions for ``mwscript.position``.
--- @class mwscript.position.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field cell string No description yet available.
--- @field x number No description yet available.
--- @field y number No description yet available.
--- @field z number No description yet available.
--- @field rotation number No description yet available.

--- Wrapper for the PositionCell mwscript function.
--- @param params mwscript.positionCell.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``cell``: string — No description yet available.
--- 
--- ``x``: number — No description yet available.
--- 
--- ``y``: number — No description yet available.
--- 
--- ``z``: number — No description yet available.
--- 
--- ``rotation``: number — No description yet available.
--- @return any result No description yet available.
function mwscript.positionCell(params) end

---Table parameter definitions for ``mwscript.positionCell``.
--- @class mwscript.positionCell.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field cell string No description yet available.
--- @field x number No description yet available.
--- @field y number No description yet available.
--- @field z number No description yet available.
--- @field rotation number No description yet available.

--- Wrapper for the RemoveItem mwscript function.
--- @param params mwscript.removeItem.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``item``: tes3item|string — The item to be added.
--- 
--- ``count``: number — The number of items to be added.
--- @return any result No description yet available.
function mwscript.removeItem(params) end

---Table parameter definitions for ``mwscript.removeItem``.
--- @class mwscript.removeItem.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3item|string The item to be added.
--- @field count number The number of items to be added.

--- Wrapper for the RemoveSpell mwscript function.
--- @param params mwscript.removeSpell.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``spell``: tes3spell|string — No description yet available.
--- @return any result No description yet available.
function mwscript.removeSpell(params) end

---Table parameter definitions for ``mwscript.removeSpell``.
--- @class mwscript.removeSpell.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Wrapper for the ScriptRunning mwscript function.
--- @param params mwscript.scriptRunning.params This table accepts the following values:
--- 
--- ``script``: tes3script|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.scriptRunning(params) end

---Table parameter definitions for ``mwscript.scriptRunning``.
--- @class mwscript.scriptRunning.params
--- @field script tes3script|string No description yet available.

--- Wrapper for the SetLevel mwscript function.
--- @param params mwscript.setLevel.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``level``: number — No description yet available.
--- @return any result No description yet available.
function mwscript.setLevel(params) end

---Table parameter definitions for ``mwscript.setLevel``.
--- @class mwscript.setLevel.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field level number No description yet available.

--- Wrapper for the StartCombat mwscript function.
--- @param params mwscript.startCombat.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``target``: tes3reference|tes3mobileActor|string — Actor to start combat with.
--- @return any result No description yet available.
function mwscript.startCombat(params) end

---Table parameter definitions for ``mwscript.startCombat``.
--- @class mwscript.startCombat.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|string Actor to start combat with.

--- Wrapper for the StartCombat mwscript function.
--- @param params mwscript.startScript.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``script``: tes3script|string — No description yet available.
--- @return any result No description yet available.
function mwscript.startScript(params) end

---Table parameter definitions for ``mwscript.startScript``.
--- @class mwscript.startScript.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field script tes3script|string No description yet available.

--- Wrapper for the StopCombat mwscript function.
--- @param params mwscript.stopCombat.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``target``: tes3reference|tes3mobileActor|string — Actor to stop combat with.
--- @return any result No description yet available.
function mwscript.stopCombat(params) end

---Table parameter definitions for ``mwscript.stopCombat``.
--- @class mwscript.stopCombat.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|string Actor to stop combat with.

--- Wrapper for the StartCombat mwscript function.
--- @param params mwscript.stopScript.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``script``: tes3script|string — No description yet available.
--- @return any result No description yet available.
function mwscript.stopScript(params) end

---Table parameter definitions for ``mwscript.stopScript``.
--- @class mwscript.stopScript.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field script tes3script|string No description yet available.

--- Wrapper for the StopSound mwscript function.
--- @param params mwscript.stopSound.params This table accepts the following values:
--- 
--- ``reference``: tes3reference|tes3mobileActor|string — The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- ``sound``: tes3sound|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.stopSound(params) end

---Table parameter definitions for ``mwscript.stopSound``.
--- @class mwscript.stopSound.params
--- @field reference tes3reference|tes3mobileActor|string The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field sound tes3sound|string No description yet available.

--- Wrapper for the WakeUpPC mwscript function.
function mwscript.wakeUpPC() end

