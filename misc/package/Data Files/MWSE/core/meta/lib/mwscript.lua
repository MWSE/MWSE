--- @meta
--- @diagnostic disable:undefined-doc-name

--- The mwscript library allows vanilla mwscript functions to be called. This is not always ideal, and this library is deprecated. Avoid using it if at all possible.
--- @class mwscriptlib
mwscript = {}

--- Use [`tes3reference:activate()`](https://mwse.github.io/MWSE/types/tes3reference/#activate) or [`tes3.setAIActivate()`](https://mwse.github.io/MWSE/apis/tes3/#tes3setaiactivate) instead. Wrapper for the `Activate` mwscript function.
--- @deprecated
--- @param params mwscript.activate.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
function mwscript.activate(params) end

---Table parameter definitions for `mwscript.activate`.
--- @class mwscript.activate.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.

--- Use [`tes3.addItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3additem), [`tes3inventory:addItem()`](https://mwse.github.io/MWSE/types/tes3inventory/#additem), or depending on your needs [`tes3mobileActor:equip()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#equip) instead. Wrapper for the `AddItem` mwscript function.
--- @deprecated
--- @param params mwscript.addItem.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — The item to be added.
--- 
--- `count`: number? — *Default*: `1`. The number of items to be added.
--- @return boolean executed No description yet available.
function mwscript.addItem(params) end

---Table parameter definitions for `mwscript.addItem`.
--- @class mwscript.addItem.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to be added.
--- @field count number? *Default*: `1`. The number of items to be added.

--- use [`tes3.addItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3additem) with the `soul` parameter instead. Wrapper for the `AddSoulGem` mwscript function.
--- @deprecated
--- @param params mwscript.addSoulGem.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `creature`: tes3creature|string — The creature to be stored in the soul gem.
--- 
--- `soulgem`: tes3misc|string — The soul gem to store the soul in.
--- @return boolean executed No description yet available.
function mwscript.addSoulGem(params) end

---Table parameter definitions for `mwscript.addSoulGem`.
--- @class mwscript.addSoulGem.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field creature tes3creature|string The creature to be stored in the soul gem.
--- @field soulgem tes3misc|string The soul gem to store the soul in.

--- Use [`tes3.addSpell()`](https://mwse.github.io/MWSE/apis/tes3/#tes3addspell) or [`tes3spellList:add()`](https://mwse.github.io/MWSE/types/tes3spellList/#add) instead. Wrapper for the `AddSpell` mwscript function.
--- @deprecated
--- @param params mwscript.addSpell.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `spell`: tes3spell|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.addSpell(params) end

---Table parameter definitions for `mwscript.addSpell`.
--- @class mwscript.addSpell.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Use [`tes3leveledCreature:insert()`](https://mwse.github.io/MWSE/types/tes3leveledCreature/#insert) instead. Wrapper for the `AddToLevCreature` mwscript function.
--- @deprecated
--- @param params mwscript.addToLevCreature.params This table accepts the following values:
--- 
--- `list`: tes3leveledCreature|string — Leveled creature list to add a creature to.
--- 
--- `creature`: tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3npc|tes3npcInstance|string — Creature to add to the list.
--- 
--- `level`: number? — *Default*: `0`. Minimum level for the creature to spawn.
--- @return boolean executed No description yet available.
function mwscript.addToLevCreature(params) end

---Table parameter definitions for `mwscript.addToLevCreature`.
--- @class mwscript.addToLevCreature.params
--- @field list tes3leveledCreature|string Leveled creature list to add a creature to.
--- @field creature tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3npc|tes3npcInstance|string Creature to add to the list.
--- @field level number? *Default*: `0`. Minimum level for the creature to spawn.

--- Use [`tes3leveledItem:insert()`](https://mwse.github.io/MWSE/types/tes3leveledItem/#insert) instead. Wrapper for the `AddToLevItem` mwscript function.
--- @deprecated
--- @param params mwscript.addToLevItem.params This table accepts the following values:
--- 
--- `list`: tes3leveledItem|string — Leveled item list to add a creature to.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — Item to add to the list.
--- 
--- `level`: number? — *Default*: `0`. Minimum level for the item to spawn.
--- @return boolean executed No description yet available.
function mwscript.addToLevItem(params) end

---Table parameter definitions for `mwscript.addToLevItem`.
--- @class mwscript.addToLevItem.params
--- @field list tes3leveledItem|string Leveled item list to add a creature to.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string Item to add to the list.
--- @field level number? *Default*: `0`. Minimum level for the item to spawn.

--- Wrapper for the `AddTopic` mwscript function.
--- @deprecated
--- @param params mwscript.addTopic.params This table accepts the following values:
--- 
--- `topic`: tes3dialogue|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.addTopic(params) end

---Table parameter definitions for `mwscript.addTopic`.
--- @class mwscript.addTopic.params
--- @field topic tes3dialogue|string No description yet available.

--- Use [`tes3.setAITravel()`](https://mwse.github.io/MWSE/apis/tes3/#tes3setaitravel) instead. Wrapper for the `AITravel` mwscript function.
--- @deprecated
--- @param params mwscript.aiTravel.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `x`: number — No description yet available.
--- 
--- `y`: number — No description yet available.
--- 
--- `z`: number — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.aiTravel(params) end

---Table parameter definitions for `mwscript.aiTravel`.
--- @class mwscript.aiTravel.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field x number No description yet available.
--- @field y number No description yet available.
--- @field z number No description yet available.

--- Use [`tes3reference:disable()`](https://mwse.github.io/MWSE/types/tes3reference/#disable) or [`tes3.setEnabled()`](https://mwse.github.io/MWSE/apis/tes3/#tes3setenabled) instead. Wrapper for the `Disable` mwscript function.
--- @deprecated
--- @param params mwscript.disable.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `modify`: boolean? — *Default*: `true`. Set the object as modified.
--- @return boolean executed No description yet available.
function mwscript.disable(params) end

---Table parameter definitions for `mwscript.disable`.
--- @class mwscript.disable.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field modify boolean? *Default*: `true`. Set the object as modified.

--- Use [`tes3.dropItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3dropitem) or [`tes3inventory:dropItem()`](https://mwse.github.io/MWSE/types/tes3inventory/#dropitem) instead. Wrapper for the `Drop` mwscript function.
--- @deprecated
--- @param params mwscript.drop.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — The item to be dropped.
--- 
--- `count`: number? — *Default*: `1`. The number of items to drop.
--- @return boolean executed No description yet available.
function mwscript.drop(params) end

---Table parameter definitions for `mwscript.drop`.
--- @class mwscript.drop.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to be dropped.
--- @field count number? *Default*: `1`. The number of items to drop.

--- Use [`tes3.setEnabled()`](https://mwse.github.io/MWSE/apis/tes3/#tes3setenabled) or [`tes3reference:enable()`](https://mwse.github.io/MWSE/types/tes3reference/#enable) instead. Wrapper for the `Enable` mwscript function.
--- @deprecated
--- @param params mwscript.enable.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `modify`: boolean? — *Default*: `true`. Set the object as modified.
--- @return boolean executed No description yet available.
function mwscript.enable(params) end

---Table parameter definitions for `mwscript.enable`.
--- @class mwscript.enable.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field modify boolean? *Default*: `true`. Set the object as modified.

--- Use [`tes3mobileActor:equip()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#equip) or [`tes3mobileActor.combatSession:changeEquipment()`](https://mwse.github.io/MWSE/types/tes3combatSession/#changeequipment) instead. Wrapper for the `Equip` mwscript function.
--- @deprecated
--- @param params mwscript.equip.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — The item to be equipped.
--- @return boolean executed No description yet available.
function mwscript.equip(params) end

---Table parameter definitions for `mwscript.equip`.
--- @class mwscript.equip.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to be equipped.

--- Use [`tes3.cast()`](https://mwse.github.io/MWSE/apis/tes3/#tes3cast) instead. Wrapper for the `ExplodeSpell` mwscript function.
--- @deprecated
--- @param params mwscript.explodeSpell.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `spell`: tes3spell|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.explodeSpell(params) end

---Table parameter definitions for `mwscript.explodeSpell`.
--- @class mwscript.explodeSpell.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Use [`tes3.messageBox()`](https://mwse.github.io/MWSE/apis/tes3/#tes3messagebox) instead. Wrapper for the `GetButtonPressed` mwscript function.
--- @deprecated
--- @return number buttonIndex No description yet available.
function mwscript.getButtonPressed() end

--- Use [`tes3mobileActor.isPlayerDetected`](https://mwse.github.io/MWSE/types/tes3mobileActor/#isplayerdetected) or depending on what you are doing, [`detectSneak`](https://mwse.github.io/MWSE/events/detectSneak/) event may be desirable. Wrapper for the `GetDetected` mwscript function.
--- @deprecated
--- @param params mwscript.getDetected.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `target`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string — Actor to check detection for.
--- @return boolean result No description yet available.
function mwscript.getDetected(params) end

---Table parameter definitions for `mwscript.getDetected`.
--- @class mwscript.getDetected.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string Actor to check detection for.

--- Use [`object.disabled`](https://mwse.github.io/MWSE/types/tes3baseObject/#disabled) on any object inheriting from `tes3baseObject`. Wrapper for the `GetDisabled` mwscript function.
--- @deprecated
--- @param params mwscript.getDisabled.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @return boolean result No description yet available.
function mwscript.getDisabled(params) end

---Table parameter definitions for `mwscript.getDisabled`.
--- @class mwscript.getDisabled.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.

--- Use [`tes3reference.position`](https://mwse.github.io/MWSE/types/tes3reference/#position) or [`tes3mobileActor.position`](https://mwse.github.io/MWSE/types/tes3mobileActor/#position) to compute the distance or depending on the desired effect, [`tes3mobileActor.playerDistance`](https://mwse.github.io/MWSE/types/tes3mobileActor/#playerdistance). Wrapper for the `GetDistance` mwscript function.
--- @deprecated
--- @param params mwscript.getDistance.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `target`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string — Actor to check distance to.
--- @return boolean result No description yet available.
function mwscript.getDistance(params) end

---Table parameter definitions for `mwscript.getDistance`.
--- @class mwscript.getDistance.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string Actor to check distance to.

--- Use [`tes3.getItemCount()`](https://mwse.github.io/MWSE/apis/tes3/#tes3getitemcount) instead. Wrapper for the `GetItemCount` mwscript function.
--- @deprecated
--- @param params mwscript.getItemCount.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — No description yet available.
--- @return number result No description yet available.
function mwscript.getItemCount(params) end

---Table parameter definitions for `mwscript.getItemCount`.
--- @class mwscript.getItemCount.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string No description yet available.

--- Use [`tes3.mobilePlayer.isJumping`](https://mwse.github.io/MWSE/types/tes3mobilePlayer/#isjumping) instead. Wrapper for the `GetPCJumping` mwscript function.
--- @deprecated
--- @return boolean result No description yet available.
function mwscript.getPCJumping() end

--- Use [`tes3.mobilePlayer.isRunning`](https://mwse.github.io/MWSE/types/tes3mobilePlayer/#isrunning) instead. Wrapper for the `GetPCRunning` mwscript function.
--- @deprecated
--- @return boolean result No description yet available.
function mwscript.getPCRunning() end

--- Use [`tes3.mobilePlayer.isSneaking`](https://mwse.github.io/MWSE/types/tes3mobilePlayer/#issneaking) instead. Wrapper for the `GetPCSneaking` mwscript function.
--- @deprecated
--- @return boolean result No description yet available.
function mwscript.getPCSneaking() end

--- Use [`tes3globalScript.reference`](https://mwse.github.io/MWSE/types/tes3globalScript/#reference) instead. Returns the script target for the currently running Morrowind script, if any.
--- @deprecated
--- @return tes3reference result No description yet available.
function mwscript.getReference() end

--- Returns the currently running Morrowind script, if any.
--- @deprecated
--- @return tes3script result No description yet available.
function mwscript.getScript() end

--- Use  [`tes3.isAffectedBy()`](https://mwse.github.io/MWSE/apis/tes3/#tes3isaffectedby) or [`tes3mobileActor:isAffectedByObject()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#isaffectedbyobject) instead. Wrapper for the `GetSpellEffects` mwscript function.
--- @deprecated
--- @param params mwscript.getSpellEffects.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `spell`: tes3spell|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.getSpellEffects(params) end

---Table parameter definitions for `mwscript.getSpellEffects`.
--- @class mwscript.getSpellEffects.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Use [`tes3actor:hasItemEquipped()`](https://mwse.github.io/MWSE/types/tes3actor/#hasitemequipped) or [`tes3.getEquippedItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3getequippeditem) instead. Wrapper for the `HasItemEquipped` mwscript function.
--- @deprecated
--- @param params mwscript.hasItemEquipped.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — The item to check for.
--- @return boolean result No description yet available.
function mwscript.hasItemEquipped(params) end

---Table parameter definitions for `mwscript.hasItemEquipped`.
--- @class mwscript.hasItemEquipped.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to check for.

--- Use [`tes3.createReference()`](https://mwse.github.io/MWSE/apis/tes3/#tes3createreference) instead. Wrapper for the `PlaceAtPC` mwscript function.
--- @deprecated
--- @param params mwscript.placeAtPC.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `object`: tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3reference|tes3repairTool|tes3spell|tes3static|tes3weapon|string — The object to place.
--- 
--- `count`: number? — *Default*: `1`. No description yet available.
--- 
--- `distance`: number? — *Default*: `256`. No description yet available.
--- 
--- `direction`: number? — *Default*: `1`. No description yet available.
--- @return tes3reference lastPlacedReference No description yet available.
function mwscript.placeAtPC(params) end

---Table parameter definitions for `mwscript.placeAtPC`.
--- @class mwscript.placeAtPC.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field object tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3reference|tes3repairTool|tes3spell|tes3static|tes3weapon|string The object to place.
--- @field count number? *Default*: `1`. No description yet available.
--- @field distance number? *Default*: `256`. No description yet available.
--- @field direction number? *Default*: `1`. No description yet available.

--- Use [`tes3.playSound()`](https://mwse.github.io/MWSE/apis/tes3/#tes3playsound), [`tes3sound:play()`](https://mwse.github.io/MWSE/types/tes3sound/#play) method, or [`tes3.playItemPickupSound()`](https://mwse.github.io/MWSE/apis/tes3/#tes3playitempickupsound) instead. Wrapper for the `PlaySound` mwscript function.
--- @deprecated
--- @param params mwscript.playSound.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `sound`: tes3sound|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.playSound(params) end

---Table parameter definitions for `mwscript.playSound`.
--- @class mwscript.playSound.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field sound tes3sound|string No description yet available.

--- Set [`tes3reference.position`](https://mwse.github.io/MWSE/types/tes3reference/#position) or [`tes3mobileActor.position`](https://mwse.github.io/MWSE/types/tes3mobileActor/#position) to desired new position instead, or use [`tes3.positionCell()`](https://mwse.github.io/MWSE/apis/tes3/#tes3positioncell). Wrapper for the `Position` mwscript function.
--- @deprecated
--- @param params mwscript.position.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `cell`: string — No description yet available.
--- 
--- `x`: number? — *Default*: `0`. No description yet available.
--- 
--- `y`: number? — *Default*: `0`. No description yet available.
--- 
--- `z`: number? — *Default*: `0`. No description yet available.
--- 
--- `rotation`: number? — *Default*: `0`. No description yet available.
--- @return boolean executed No description yet available.
function mwscript.position(params) end

---Table parameter definitions for `mwscript.position`.
--- @class mwscript.position.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field cell string No description yet available.
--- @field x number? *Default*: `0`. No description yet available.
--- @field y number? *Default*: `0`. No description yet available.
--- @field z number? *Default*: `0`. No description yet available.
--- @field rotation number? *Default*: `0`. No description yet available.

--- Use [`tes3.positionCell()`](https://mwse.github.io/MWSE/apis/tes3/#tes3positioncell) instead. Wrapper for the `PositionCell` mwscript function.
--- @deprecated
--- @param params mwscript.positionCell.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `cell`: string — No description yet available.
--- 
--- `x`: number? — *Default*: `0`. No description yet available.
--- 
--- `y`: number? — *Default*: `0`. No description yet available.
--- 
--- `z`: number? — *Default*: `0`. No description yet available.
--- 
--- `rotation`: number? — *Default*: `0`. No description yet available.
--- @return boolean executed No description yet available.
function mwscript.positionCell(params) end

---Table parameter definitions for `mwscript.positionCell`.
--- @class mwscript.positionCell.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field cell string No description yet available.
--- @field x number? *Default*: `0`. No description yet available.
--- @field y number? *Default*: `0`. No description yet available.
--- @field z number? *Default*: `0`. No description yet available.
--- @field rotation number? *Default*: `0`. No description yet available.

--- Use [`tes3.removeItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3removeitem), [`tes3.removeItemData()`](https://mwse.github.io/MWSE/apis/tes3/#tes3removeitemdata) or [`tes3inventory:removeItem()`](https://mwse.github.io/MWSE/types/tes3inventory/#removeitem) instead. Wrapper for the `RemoveItem` mwscript function.
--- @deprecated
--- @param params mwscript.removeItem.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string — The item to be removed.
--- 
--- `count`: number? — *Default*: `1`. The number of items to be removed.
--- @return boolean executed No description yet available.
function mwscript.removeItem(params) end

---Table parameter definitions for `mwscript.removeItem`.
--- @class mwscript.removeItem.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to be removed.
--- @field count number? *Default*: `1`. The number of items to be removed.

--- Use [`tes3.removeSpell()`](https://mwse.github.io/MWSE/apis/tes3/#tes3removespell) or [`tes3spellList:remove()`](https://mwse.github.io/MWSE/types/tes3spellList/#remove) instead. Wrapper for the `RemoveSpell` mwscript function.
--- @deprecated
--- @param params mwscript.removeSpell.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `spell`: tes3spell|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.removeSpell(params) end

---Table parameter definitions for `mwscript.removeSpell`.
--- @class mwscript.removeSpell.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field spell tes3spell|string No description yet available.

--- Wrapper for the `ScriptRunning` mwscript function.
--- @deprecated
--- @param params mwscript.scriptRunning.params This table accepts the following values:
--- 
--- `script`: tes3script|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.scriptRunning(params) end

---Table parameter definitions for `mwscript.scriptRunning`.
--- @class mwscript.scriptRunning.params
--- @field script tes3script|string No description yet available.

--- Use [`tes3npc.level`](https://mwse.github.io/MWSE/types/tes3npc/#level) instead. Wrapper for the `SetLevel` mwscript function.
--- @deprecated
--- @param params mwscript.setLevel.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `level`: number — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.setLevel(params) end

---Table parameter definitions for `mwscript.setLevel`.
--- @class mwscript.setLevel.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field level number No description yet available.

--- Use [`tes3mobileActor:startCombat()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#startcombat) instead. Wrapper for the `StartCombat` mwscript function.
--- @deprecated
--- @param params mwscript.startCombat.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `target`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string — Actor to start combat with.
--- @return boolean executed No description yet available.
function mwscript.startCombat(params) end

---Table parameter definitions for `mwscript.startCombat`.
--- @class mwscript.startCombat.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string Actor to start combat with.

--- Use [`tes3.runLegacyScript()`](https://mwse.github.io/MWSE/apis/tes3/?h=script#tes3runlegacyscript) instead. Wrapper for the `StartCombat` mwscript function.
--- @deprecated
--- @param params mwscript.startScript.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `script`: tes3script|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.startScript(params) end

---Table parameter definitions for `mwscript.startScript`.
--- @class mwscript.startScript.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field script tes3script|string No description yet available.

--- Use [`tes3mobileActor:stopCombat()`](https://mwse.github.io/MWSE/types/tes3mobileActor/#stopcombat) instead. Wrapper for the `StopCombat` mwscript function.
--- @deprecated
--- @param params mwscript.stopCombat.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `target`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string — Actor to stop combat with.
--- @return boolean executed No description yet available.
function mwscript.stopCombat(params) end

---Table parameter definitions for `mwscript.stopCombat`.
--- @class mwscript.stopCombat.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field target tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string Actor to stop combat with.

--- Wrapper for the `StopCombat` mwscript function.
--- @deprecated
--- @param params mwscript.stopScript.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `script`: tes3script|string — No description yet available.
--- @return boolean executed No description yet available.
function mwscript.stopScript(params) end

---Table parameter definitions for `mwscript.stopScript`.
--- @class mwscript.stopScript.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field script tes3script|string No description yet available.

--- Use [`tes3.removeSound()`](https://mwse.github.io/MWSE/apis/tes3/#tes3removesound) or [`tes3sound:stop()`](https://mwse.github.io/MWSE/types/tes3sound/#stop) instead. Wrapper for the `StopSound` mwscript function.
--- @deprecated
--- @param params mwscript.stopSound.params This table accepts the following values:
--- 
--- `reference`: tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil — *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- 
--- `sound`: tes3sound|string — No description yet available.
--- @return boolean result No description yet available.
function mwscript.stopSound(params) end

---Table parameter definitions for `mwscript.stopSound`.
--- @class mwscript.stopSound.params
--- @field reference tes3reference|tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|string|nil *Optional*. The target reference for this command to be executed on. Defaults to the normal script execution reference.
--- @field sound tes3sound|string No description yet available.

--- Use [`tes3.wakeUp()`](https://mwse.github.io/MWSE/apis/tes3/#tes3wakeup) instead. Wrapper for the `WakeUpPC` mwscript function.
--- @deprecated
function mwscript.wakeUpPC() end
