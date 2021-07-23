--- @meta

--- A mobile object for a creature, NPC, or the player.
--- @class tes3mobileActor : tes3mobileObject
--- @field actionBeforeCombat tes3actionData Action data stored before the actor entered combat.
--- @field actionData tes3actionData Current action data. Pre-combat action data is stored in the actionBeforeCombat property.
--- @field activeAI boolean Friendly access to the actor's flag that controls if AI is active.
--- @field activeMagicEffectList table The active magic effects on the actor, from which all others can be accessed. A table with tes3activeMagicEffect items.
--- @field actorType number The type of the mobile actor. 0 is a creature, 1 is an NPC, 2 is the player.
--- @field agility tes3statistic Direct access to the actor's agility attribute statistic.
--- @field aiPlanner tes3aiPlanner Access to the mobile's AI planner and AI package information.
--- @field alarm number The actor's alarm AI value.
--- @field animationController tes3actorAnimationController No description yet available.
--- @field armorRating number The actor's current armour rating, taking equipment condition into account. Armour mitigation can be automatically applied to damage by using the applyDamage function.
--- 
--- Armour mitigation calculation:
--- x = damage / (damage + target.armorRating)
--- damage *= max(fCombatArmorMinMult, x)
--- if damage < 1 then damage = 1 end
--- @field attackBonus number Direct access to the actor's attack bonus effect attribute.
--- @field attacked boolean Friendly access to the actor's flag that controls if the actor has been attacked.
--- @field attributes table Access to a table of 8 tes3statistic objects for the actor's attributes.
--- @field barterGold number The current amount of gold that the actor has access to for bartering.
--- @field blind number Direct access to the actor's blind effect attribute.
--- @field cell tes3cell Fetches the cell that the actor is in.
--- @field chameleon number Direct access to the actor's chameleon effect attribute.
--- @field collidingReference tes3reference The reference that the mobile has collided with this frame.
--- @field combatSession tes3combatSession Combat session data. This exists while the actor is in combat to provide memory for AI combat decisions.
--- @field corpseHourstamp number No description yet available.
--- @field currentEnchantedItem tes3equipmentStack The currently equipped enchanted item that the actor will use.
--- @field currentSpell tes3spell The currently equipped spell that the actor will use.
--- @field effectAttributes table Access to a table of 24 numbers for the actor's effect attributes.
--- @field encumbrance tes3statistic Access to the actor's encumbrance statistic.
--- @field endurance tes3statistic Direct access to the actor's endurance attribute statistic.
--- @field facing number The facing of the actor, in radians.
--- @field fatigue tes3statistic Access to the actor's fatigue statistic.
--- @field fight number The actor's fight AI value.
--- @field flee number The actor's flee AI value.
--- @field friendlyActors tes3iterator A collection of other tes3mobileActors that this actor considers friendly.
--- @field greetDuration number No description yet available.
--- @field greetTimer number No description yet available.
--- @field health tes3statistic Access to the actor's health statistic.
--- @field hello number The actor's hello AI value.
--- @field holdBreathTime number No description yet available.
--- @field hostileActors tes3iterator A collection of other tes3mobileActors that this actor considers hostile.
--- @field idleAnim boolean Friendly access to the actor's flag that controls if the actor is using their idle animation.
--- @field inCombat boolean Friendly access to the actor's flag that controls if the actor is in combat.
--- @field intelligence tes3statistic Direct access to the actor's intelligence attribute statistic.
--- @field invisibility number Direct access to the actor's invisibility effect attribute.
--- @field isCrittable boolean Friendly access to the actor's flag that controls if the actor can be critically hit.
--- @field isDead boolean True if the actor is dead.
--- @field isFalling boolean Direct access to the actor's current movement flags, showing if the actor is falling. This is when the actor is falling without having jumped, e.g. if they walked off a ledge.
--- @field isFlying boolean Direct access to the actor's current movement flags, showing if the actor is flying.
--- @field isJumping boolean Direct access to the actor's current movement flags, showing if the actor is jumping.
--- @field isMovingBack boolean Direct access to the actor's current movement flags, showing if the actor is moving backwards.
--- @field isMovingForward boolean Direct access to the actor's current movement flags, showing if the actor is moving forwards.
--- @field isMovingLeft boolean Direct access to the actor's current movement flags, showing if the actor is moving left.
--- @field isMovingRight boolean Direct access to the actor's current movement flags, showing if the actor is moving right.
--- @field isPlayerDetected boolean Direct access to the actor's flag showing the player was detected on the last detection check.
--- @field isPlayerHidden boolean Direct access to the actor's flag showing the player was hidden on the last detection check.
--- @field isRunning boolean Direct access to the actor's current movement flags, showing if the actor is running.
--- @field isSneaking boolean Direct access to the actor's current movement flags, showing if the actor is sneaking.
--- @field isSwimming boolean Direct access to the actor's current movement flags, showing if the actor is swimming.
--- @field isTurningLeft boolean Direct access to the actor's current movement flags, showing if the actor is turning left.
--- @field isTurningRight boolean Direct access to the actor's current movement flags, showing if the actor is turning right.
--- @field isWalking boolean Direct access to the actor's current movement flags, showing if the actor is walking.
--- @field jump number Direct access to the actor's jump effect attribute.
--- @field lastGroundZ number No description yet available.
--- @field levitate number Direct access to the actor's levitate effect attribute.
--- @field luck tes3statistic Direct access to the actor's luck attribute statistic.
--- @field magicka tes3statistic Access to the actor's magicka statistic.
--- @field magickaMultiplier tes3statistic Access to the actor's magicka multiplier statistic.
--- @field mobToMobCollision boolean Allows modifying if this actor will collide with other actors. When true (default), the actor cannot move through other actors. When false, the actor is allowed to move through other actors, and other actors can move through it.
--- 
--- May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.
--- @field nextActionWeight number No description yet available.
--- @field paralyze number Direct access to the actor's paralyze effect attribute.
--- @field personality tes3statistic Direct access to the actor's personality attribute statistic.
--- @field readiedAmmo tes3equipmentStack The currently equipped ammo.
--- @field readiedAmmoCount number The number of ammo equipped for the readied ammo.
--- @field readiedShield tes3equipmentStack The currently equipped shield.
--- @field readiedWeapon tes3equipmentStack The currently equipped weapon.
--- @field resistBlightDisease number Direct access to the actor's blight disease resistance effect attribute.
--- @field resistCommonDisease number Direct access to the actor's common disease resistance effect attribute.
--- @field resistCorprus number Direct access to the actor's corprus disease resistance effect attribute.
--- @field resistFire number Direct access to the actor's fire resistance effect attribute.
--- @field resistFrost number Direct access to the actor's frost resistance effect attribute.
--- @field resistMagicka number Direct access to the actor's magicka resistance effect attribute.
--- @field resistNormalWeapons number Direct access to the actor's normal weapon resistance effect attribute.
--- @field resistParalysis number Direct access to the actor's paralysis resistance effect attribute.
--- @field resistPoison number Direct access to the actor's poison resistance effect attribute.
--- @field resistShock number Direct access to the actor's shock resistance effect attribute.
--- @field sanctuary number Direct access to the actor's sanctuary effect attribute.
--- @field scanInterval number No description yet available.
--- @field scanTimer number No description yet available.
--- @field shield number Direct access to the actor's shield effect attribute.
--- @field silence number Direct access to the actor's silence effect attribute.
--- @field sound number Direct access to the actor's sound effect attribute.
--- @field speed tes3statistic Direct access to the actor's speed attribute statistic.
--- @field spellReadied boolean Friendly access to the actor's flag that controls if the actor has a spell readied.
--- @field strength tes3statistic Direct access to the actor's strength attribute statistic.
--- @field swiftSwim number Direct access to the actor's swift swim effect attribute.
--- @field talkedTo boolean Direct access to the actor's flag that shows it was recently talked to.
--- @field torchSlot tes3equipmentStack The currently equipped light.
--- @field underwater boolean Friendly access to the actor's flag that controls if the actor is under water.
--- @field waterBreathing number Direct access to the actor's water breathing effect attribute.
--- @field waterWalking number Direct access to the actor's water walking effect attribute.
--- @field weaponDrawn boolean Friendly access to the actor's flag that shows if the weapon model is visible. When readying a weapon, there is a short period of time at the start of the animation, where the weapon is not visible yet. This flag will only be set after this initial stage is done. This flag is still set with hand-to-hand even though it doesn't use a model. Setting this to false while a weapon is drawn will normally cause the actor to play its weapon draw animation again.
--- @field werewolf boolean Friendly access to the actor's flag that controls if the actor in werewolf form.
--- @field width number No description yet available.
--- @field willpower tes3statistic Direct access to the actor's willpower attribute statistic.
tes3mobileActor = {}

--- Damages the actor, with options to control mitigation and difficulty scaling. Invokes the 'damage' and 'damaged' events, with 'script' source. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.
--- @param params tes3mobileActor.applyDamage.params This table accepts the following values:
--- 
--- ``damage``: number — The amount of damage to apply.
--- 
--- ``applyArmor``: boolean — If armor should mitigate the incoming damage. If the player is the target, armor experience will be gained.
--- 
--- ``resistAttribute``: number — The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.
--- 
--- ``applyDifficulty``: boolean — If the game difficulty modifier should be applied. Must be used with the 'playerAttack' argument to apply the correct modifier.
--- 
--- ``playerAttack``: boolean — If the attack came from the player. Used for difficulty calculation.
--- 
--- ``doNotChangeHealth``: boolean — If all armor effects except the health change should be applied. These include hit sounds, armor condition damage, and player experience gain from being hit.
--- @return number result No description yet available.
function tes3mobileActor:applyDamage(params) end

---Table parameter definitions for ``tes3mobileActor.applyDamage``.
--- @class tes3mobileActor.applyDamage.params
--- @field damage number The amount of damage to apply.
--- @field applyArmor boolean If armor should mitigate the incoming damage. If the player is the target, armor experience will be gained.
--- @field resistAttribute number The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.
--- @field applyDifficulty boolean If the game difficulty modifier should be applied. Must be used with the 'playerAttack' argument to apply the correct modifier.
--- @field playerAttack boolean If the attack came from the player. Used for difficulty calculation.
--- @field doNotChangeHealth boolean If all armor effects except the health change should be applied. These include hit sounds, armor condition damage, and player experience gain from being hit.

--- Damages the actor's fatigue, with accompanying reaction from the reciever. Invokes the 'damageHandToHand' and 'damagedHandToHand' events, with 'script' source. Returns the actual fatigue damage done.
--- @param fatigueDamage number The amount of fatigue damage to apply.
--- @param swing number The attack swing magnitude, range 0-1. Only modifies hit volume.
--- @param alwaysPlayHitVoice boolean Always play the hit reaction voiceover.
--- @return number result No description yet available.
function tes3mobileActor:applyFatigueDamage(fatigueDamage, swing, alwaysPlayHitVoice) end

--- **Deprecated, please use applyDamage instead.**
--- 
--- Damages the actor.
--- @param damage number The amount of damage to apply.
--- @param isPlayerAttack boolean Used to calculate difficulty scaling.
--- @param scaleWithDifficulty boolean Apply difficulty scaling to the damage.
--- @param doNotChangeHealth boolean Do all the audiovisual effects of damage without the health change.
--- @return boolean result No description yet available.
function tes3mobileActor:applyHealthDamage(damage, isPlayerAttack, scaleWithDifficulty, doNotChangeHealth) end

--- Calculates the damage that would be inflicted to an actor after armor and/or resistance. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.
--- @param params tes3mobileActor.calcEffectiveDamage.params This table accepts the following values:
--- 
--- ``damage``: number — The amount of damage to apply.
--- 
--- ``applyArmor``: boolean — If armor should mitigate the incoming damage.
--- 
--- ``resistAttribute``: number — The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.
--- @return number result No description yet available.
function tes3mobileActor:calcEffectiveDamage(params) end

---Table parameter definitions for ``tes3mobileActor.calcEffectiveDamage``.
--- @class tes3mobileActor.calcEffectiveDamage.params
--- @field damage number The amount of damage to apply.
--- @field applyArmor boolean If armor should mitigate the incoming damage.
--- @field resistAttribute number The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes.

--- Fetches a filtered list of the active magic effects on the actor. Returns a table with tes3activeMagicEffect items.
--- @param params tes3mobileActor.getActiveMagicEffects.params This table accepts the following values:
--- 
--- ``effect``: number — The magic effect ID to search for.
--- 
--- ``serial``: number — The magic instance serial to search for.
--- @return table result No description yet available.
function tes3mobileActor:getActiveMagicEffects(params) end

---Table parameter definitions for ``tes3mobileActor.getActiveMagicEffects``.
--- @class tes3mobileActor.getActiveMagicEffects.params
--- @field effect number The magic effect ID to search for.
--- @field serial number The magic instance serial to search for.

--- Gets the weight of the boots equipped on the actor, or 0 if no boots are equipped.
--- @return number result No description yet available.
function tes3mobileActor:getBootsWeight() end

--- Gets the fatigue-based skill scaling term used by many game mechanics, based on the actor's current and maximum fatigue. It is equal to ``max(0, fFatigueBase - fFatigueMult * max(0, 1 - fatigue.current/fatigue.base))``
--- @return number result No description yet available.
function tes3mobileActor:getFatigueTerm() end

--- Finds the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().
--- @param power tes3spell The spell object for the power.
--- @return number timestamp No description yet available.
function tes3mobileActor:getPowerUseTimestamp(power) end

--- Fetches the statistic object of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.
--- @param skillId number The index of the skill statistic to fetch.
--- @return tes3skillStatistic result No description yet available.
function tes3mobileActor:getSkillStatistic(skillId) end

--- Fetches the current value of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.
--- @param skillId number The index of the skill statistic's value to fetch.
--- @return number result No description yet available.
function tes3mobileActor:getSkillValue(skillId) end

--- No description yet available.
--- @return number result No description yet available.
function tes3mobileActor:getViewToActor() end

--- No description yet available.
--- @return number result No description yet available.
function tes3mobileActor:getViewToPoint() end

--- No description yet available.
--- @return number result No description yet available.
function tes3mobileActor:getViewToPointWithFacing() end

--- Fetches the weapon speed of the actor's currently equipped weapon, or 1.0 if no weapon is equipped.
--- @return number result No description yet available.
function tes3mobileActor:getWeaponSpeed() end

--- If true, the actor isn't paralyzed, dead, stunned, or otherwise unable to take action.
--- @return boolean result No description yet available.
function tes3mobileActor:hasFreeAction() end

--- Check if a power has been used and is recharging.
--- @param power tes3spell The spell object for the power.
--- @return boolean result No description yet available.
function tes3mobileActor:hasUsedPower(power) end

--- Determines if the actor is currently being affected by a given alchemy, enchantment, or spell.
--- @param object tes3alchemy|tes3enchantment|tes3spell The magic to check for.
--- @return boolean result No description yet available.
function tes3mobileActor:isAffectedByObject(object) end

--- Makes a power immediately available for casting again.
--- @param power tes3spell The spell object for the power.
--- @return number result No description yet available.
function tes3mobileActor:rechargePower(power) end

--- Sets the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp.
--- @param power tes3spell The spell object for the power.
--- @param timestamp number The timestamp of the moment the power was casted, or 24 hours before the recharge point. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().
function tes3mobileActor:setPowerUseTimestamp(power, timestamp) end

--- Forces the actor into combat with another actor.
--- @param target tes3mobileActor No description yet available.
function tes3mobileActor:startCombat(target) end

--- Starts dialogue with this actor for the player.
function tes3mobileActor:startDialogue() end

--- Ends combat for the actor.
--- @param force boolean If false, the function won't stop combat if the actor has other valid hostile targets.
function tes3mobileActor:stopCombat(force) end

--- Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.
--- @param attribute tes3statistic Limits the update to statistics derived from this attribute.  e.g. ``mobile:updateDerivedStatistics(mobile.strength)``. If not present, all derived statistics will be updated.
function tes3mobileActor:updateDerivedStatistics(attribute) end

--- Updates the actor's visual opacity. Used after modifying applied chameleon or invisiblity effects.
function tes3mobileActor:updateOpacity() end

