tes3mobileActor
====================================================================================================

A mobile object for a creature, NPC, or the player.

Properties
----------------------------------------------------------------------------------------------------

`actionBeforeCombat`_ (`tes3actionData`_)
    Read-only. Action data stored before the actor entered combat.

`actionData`_ (`tes3actionData`_)
    Read-only. Current action data. Pre-combat action data is stored in the actionBeforeCombat property.

`activeAI`_ (`boolean`_)
    Friendly access to the actor's flag that controls if AI is active.

`activeMagicEffectList`_ (`table`_)
    Read-only. The active magic effects on the actor, from which all others can be accessed. A table with tes3activeMagicEffect items.

`actorType`_ (`number`_)
    Read-only. The type of the mobile actor. 0 is a creature, 1 is an NPC, 2 is the player.

`agility`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's agility attribute statistic.

`aiPlanner`_ (`tes3aiPlanner`_)
    Read-only. Access to the mobile's AI planner and AI package information.

`alarm`_ (`number`_)
    The actor's alarm AI value.

`animationController`_ (`tes3actorAnimationController`_)
    Read-only. No description available.

`armorRating`_ (`number`_)
    Read-only. The actor's current armour rating, taking equipment condition into account. Armour mitigation can be automatically applied to damage by using the applyDamage function.

Armour mitigation calculation:
x = damage / (damage + target.armorRating)
damage *= max(fCombatArmorMinMult, x)
if damage < 1 then damage = 1 end

`attackBonus`_ (`number`_)
    Direct access to the actor's attack bonus effect attribute.

`attacked`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor has been attacked.

`attributes`_ (`table`_)
    Read-only. Access to a table of 8 tes3statistic objects for the actor's attributes.

`barterGold`_ (`number`_)
    The current amount of gold that the actor has access to for bartering.

`blind`_ (`number`_)
    Direct access to the actor's blind effect attribute.

`boundSize`_ (`tes3vector3`_)
    Read-only. A vector that shows the size of the bounding box in each direction.

`cell`_ (`tes3cell`_)
    Read-only. Fetches the cell that the actor is in.

`cellX`_ (`number`_)
    Read-only. The X grid coordinate of the cell the mobile is in.

`cellY`_ (`number`_)
    Read-only. The Y grid coordinate of the cell the mobile is in.

`chameleon`_ (`number`_)
    Direct access to the actor's chameleon effect attribute.

`collidingReference`_ (`tes3reference`_)
    Read-only. The reference that the mobile has collided with this frame.

`combatSession`_ (`tes3combatSession`_)
    Read-only. Combat session data. This exists while the actor is in combat to provide memory for AI combat decisions.

`corpseHourstamp`_ (`number`_)
    No description available.

`currentEnchantedItem`_ (`tes3equipmentStack`_)
    Read-only. The currently equipped enchanted item that the actor will use.

`currentSpell`_ (`tes3spell`_)
    Read-only. The currently equipped spell that the actor will use.

`effectAttributes`_ (`table`_)
    Read-only. Access to a table of 24 numbers for the actor's effect attributes.

`encumbrance`_ (`tes3statistic`_)
    Read-only. Access to the actor's encumbrance statistic.

`endurance`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's endurance attribute statistic.

`facing`_ (`number`_)
    Read-only. The facing of the actor, in radians.

`fatigue`_ (`tes3statistic`_)
    Read-only. Access to the actor's fatigue statistic.

`fight`_ (`number`_)
    The actor's fight AI value.

`flags`_ (`number`_)
    Read-only. Access to the root mobile object flags, represented as an integer. Should not be accessed directly.

`flee`_ (`number`_)
    The actor's flee AI value.

`friendlyActors`_ (`tes3iterator`_)
    Read-only. A collection of other tes3mobileActors that this actor considers friendly.

`greetDuration`_ (`number`_)
    No description available.

`greetTimer`_ (`number`_)
    Read-only. No description available.

`health`_ (`tes3statistic`_)
    Read-only. Access to the actor's health statistic.

`height`_ (`number`_)
    Read-only. The height of the mobile above the ground.

`hello`_ (`number`_)
    The actor's hello AI value.

`holdBreathTime`_ (`number`_)
    No description available.

`hostileActors`_ (`tes3iterator`_)
    Read-only. A collection of other tes3mobileActors that this actor considers hostile.

`idleAnim`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor is using their idle animation.

`impulseVelocity`_ (`tes3vector3`_)
    A vector that represents the 3D acceleration of the object.

`inCombat`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor is in combat.

`intelligence`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's intelligence attribute statistic.

`invisibility`_ (`number`_)
    Direct access to the actor's invisibility effect attribute.

`isCrittable`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor can be critically hit.

`isDead`_ (`boolean`_)
    Read-only. True if the actor is dead.

`isFalling`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is falling. This is when the actor is falling without having jumped, e.g. if they walked off a ledge.

`isFlying`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is flying.

`isJumping`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is jumping.

`isMovingBack`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving backwards.

`isMovingForward`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving forwards.

`isMovingLeft`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving left.

`isMovingRight`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is moving right.

`isPlayerDetected`_ (`boolean`_)
    Direct access to the actor's flag showing the player was detected on the last detection check.

`isPlayerHidden`_ (`boolean`_)
    Direct access to the actor's flag showing the player was hidden on the last detection check.

`isRunning`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is running.

`isSneaking`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is sneaking.

`isSwimming`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is swimming.

`isTurningLeft`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is turning left.

`isTurningRight`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is turning right.

`isWalking`_ (`boolean`_)
    Direct access to the actor's current movement flags, showing if the actor is walking.

`jump`_ (`number`_)
    Direct access to the actor's jump effect attribute.

`lastGroundZ`_ (`number`_)
    No description available.

`levitate`_ (`number`_)
    Direct access to the actor's levitate effect attribute.

`luck`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's luck attribute statistic.

`magicka`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka statistic.

`magickaMultiplier`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka multiplier statistic.

`mobToMobCollision`_ (`boolean`_)
    Allows modifying if this actor will collide with other actors. When true (default), the actor cannot move through other actors. When false, the actor is allowed to move through other actors, and other actors can move through it.

May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.

`movementCollision`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`nextActionWeight`_ (`number`_)
    Read-only. No description available.

`objectType`_ (`number`_)
    Read-only. The type of mobile object. Maps to values in tes3.objectType.

`paralyze`_ (`number`_)
    Direct access to the actor's paralyze effect attribute.

`personality`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's personality attribute statistic.

`playerDistance`_ (`number`_)
    The distance to the player. Updated every frame when the mobile is in an active cell.

`position`_ (`tes3vector3`_)
    A vector that represents the 3D position of the object.

`prevMovementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.

`readiedAmmo`_ (`tes3equipmentStack`_)
    The currently equipped ammo.

`readiedAmmoCount`_ (`number`_)
    The number of ammo equipped for the readied ammo.

`readiedShield`_ (`tes3equipmentStack`_)
    The currently equipped shield.

`readiedWeapon`_ (`tes3equipmentStack`_)
    The currently equipped weapon.

`reference`_ (`tes3reference`_)
    Read-only. Access to the reference object for the mobile, if any.

`resistBlightDisease`_ (`number`_)
    Direct access to the actor's blight disease resistance effect attribute.

`resistCommonDisease`_ (`number`_)
    Direct access to the actor's common disease resistance effect attribute.

`resistCorprus`_ (`number`_)
    Direct access to the actor's corprus disease resistance effect attribute.

`resistFire`_ (`number`_)
    Direct access to the actor's fire resistance effect attribute.

`resistFrost`_ (`number`_)
    Direct access to the actor's frost resistance effect attribute.

`resistMagicka`_ (`number`_)
    Direct access to the actor's magicka resistance effect attribute.

`resistNormalWeapons`_ (`number`_)
    Direct access to the actor's normal weapon resistance effect attribute.

`resistParalysis`_ (`number`_)
    Direct access to the actor's paralysis resistance effect attribute.

`resistPoison`_ (`number`_)
    Direct access to the actor's poison resistance effect attribute.

`resistShock`_ (`number`_)
    Direct access to the actor's shock resistance effect attribute.

`sanctuary`_ (`number`_)
    Direct access to the actor's sanctuary effect attribute.

`scanInterval`_ (`number`_)
    Read-only. No description available.

`scanTimer`_ (`number`_)
    Read-only. No description available.

`shield`_ (`number`_)
    Direct access to the actor's shield effect attribute.

`silence`_ (`number`_)
    Direct access to the actor's silence effect attribute.

`sound`_ (`number`_)
    Direct access to the actor's sound effect attribute.

`speed`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's speed attribute statistic.

`spellReadied`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor has a spell readied.

`strength`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's strength attribute statistic.

`swiftSwim`_ (`number`_)
    Direct access to the actor's swift swim effect attribute.

`talkedTo`_ (`boolean`_)
    Direct access to the actor's flag that shows it was recently talked to.

`torchSlot`_ (`tes3equipmentStack`_)
    The currently equipped light.

`underwater`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor is under water.

`velocity`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.

`waterBreathing`_ (`number`_)
    Direct access to the actor's water breathing effect attribute.

`waterWalking`_ (`number`_)
    Direct access to the actor's water walking effect attribute.

`weaponDrawn`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that shows if the weapon model is visible. When readying a weapon, there is a short period of time at the start of the animation, where the weapon is not visible yet. This flag will only be set after this initial stage is done. This flag is still set with hand-to-hand even though it doesn't use a model. Setting this to false while a weapon is drawn will normally cause the actor to play its weapon draw animation again.

`werewolf`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor in werewolf form.

`width`_ (`number`_)
    Read-only. No description available.

`willpower`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's willpower attribute statistic.

.. toctree::
    :hidden:

    tes3mobileActor/actionBeforeCombat
    tes3mobileActor/actionData
    tes3mobileActor/activeAI
    tes3mobileActor/activeMagicEffectList
    tes3mobileActor/actorType
    tes3mobileActor/agility
    tes3mobileActor/aiPlanner
    tes3mobileActor/alarm
    tes3mobileActor/animationController
    tes3mobileActor/armorRating
    tes3mobileActor/attackBonus
    tes3mobileActor/attacked
    tes3mobileActor/attributes
    tes3mobileActor/barterGold
    tes3mobileActor/blind
    tes3mobileActor/boundSize
    tes3mobileActor/cell
    tes3mobileActor/cellX
    tes3mobileActor/cellY
    tes3mobileActor/chameleon
    tes3mobileActor/collidingReference
    tes3mobileActor/combatSession
    tes3mobileActor/corpseHourstamp
    tes3mobileActor/currentEnchantedItem
    tes3mobileActor/currentSpell
    tes3mobileActor/effectAttributes
    tes3mobileActor/encumbrance
    tes3mobileActor/endurance
    tes3mobileActor/facing
    tes3mobileActor/fatigue
    tes3mobileActor/fight
    tes3mobileActor/flags
    tes3mobileActor/flee
    tes3mobileActor/friendlyActors
    tes3mobileActor/greetDuration
    tes3mobileActor/greetTimer
    tes3mobileActor/health
    tes3mobileActor/height
    tes3mobileActor/hello
    tes3mobileActor/holdBreathTime
    tes3mobileActor/hostileActors
    tes3mobileActor/idleAnim
    tes3mobileActor/impulseVelocity
    tes3mobileActor/inCombat
    tes3mobileActor/intelligence
    tes3mobileActor/invisibility
    tes3mobileActor/isCrittable
    tes3mobileActor/isDead
    tes3mobileActor/isFalling
    tes3mobileActor/isFlying
    tes3mobileActor/isJumping
    tes3mobileActor/isMovingBack
    tes3mobileActor/isMovingForward
    tes3mobileActor/isMovingLeft
    tes3mobileActor/isMovingRight
    tes3mobileActor/isPlayerDetected
    tes3mobileActor/isPlayerHidden
    tes3mobileActor/isRunning
    tes3mobileActor/isSneaking
    tes3mobileActor/isSwimming
    tes3mobileActor/isTurningLeft
    tes3mobileActor/isTurningRight
    tes3mobileActor/isWalking
    tes3mobileActor/jump
    tes3mobileActor/lastGroundZ
    tes3mobileActor/levitate
    tes3mobileActor/luck
    tes3mobileActor/magicka
    tes3mobileActor/magickaMultiplier
    tes3mobileActor/mobToMobCollision
    tes3mobileActor/movementCollision
    tes3mobileActor/movementFlags
    tes3mobileActor/nextActionWeight
    tes3mobileActor/objectType
    tes3mobileActor/paralyze
    tes3mobileActor/personality
    tes3mobileActor/playerDistance
    tes3mobileActor/position
    tes3mobileActor/prevMovementFlags
    tes3mobileActor/readiedAmmo
    tes3mobileActor/readiedAmmoCount
    tes3mobileActor/readiedShield
    tes3mobileActor/readiedWeapon
    tes3mobileActor/reference
    tes3mobileActor/resistBlightDisease
    tes3mobileActor/resistCommonDisease
    tes3mobileActor/resistCorprus
    tes3mobileActor/resistFire
    tes3mobileActor/resistFrost
    tes3mobileActor/resistMagicka
    tes3mobileActor/resistNormalWeapons
    tes3mobileActor/resistParalysis
    tes3mobileActor/resistPoison
    tes3mobileActor/resistShock
    tes3mobileActor/sanctuary
    tes3mobileActor/scanInterval
    tes3mobileActor/scanTimer
    tes3mobileActor/shield
    tes3mobileActor/silence
    tes3mobileActor/sound
    tes3mobileActor/speed
    tes3mobileActor/spellReadied
    tes3mobileActor/strength
    tes3mobileActor/swiftSwim
    tes3mobileActor/talkedTo
    tes3mobileActor/torchSlot
    tes3mobileActor/underwater
    tes3mobileActor/velocity
    tes3mobileActor/waterBreathing
    tes3mobileActor/waterWalking
    tes3mobileActor/weaponDrawn
    tes3mobileActor/werewolf
    tes3mobileActor/width
    tes3mobileActor/willpower

.. _`actionBeforeCombat`: tes3mobileActor/actionBeforeCombat.html
.. _`actionData`: tes3mobileActor/actionData.html
.. _`activeAI`: tes3mobileActor/activeAI.html
.. _`activeMagicEffectList`: tes3mobileActor/activeMagicEffectList.html
.. _`actorType`: tes3mobileActor/actorType.html
.. _`agility`: tes3mobileActor/agility.html
.. _`aiPlanner`: tes3mobileActor/aiPlanner.html
.. _`alarm`: tes3mobileActor/alarm.html
.. _`animationController`: tes3mobileActor/animationController.html
.. _`armorRating`: tes3mobileActor/armorRating.html
.. _`attackBonus`: tes3mobileActor/attackBonus.html
.. _`attacked`: tes3mobileActor/attacked.html
.. _`attributes`: tes3mobileActor/attributes.html
.. _`barterGold`: tes3mobileActor/barterGold.html
.. _`blind`: tes3mobileActor/blind.html
.. _`boundSize`: tes3mobileActor/boundSize.html
.. _`cell`: tes3mobileActor/cell.html
.. _`cellX`: tes3mobileActor/cellX.html
.. _`cellY`: tes3mobileActor/cellY.html
.. _`chameleon`: tes3mobileActor/chameleon.html
.. _`collidingReference`: tes3mobileActor/collidingReference.html
.. _`combatSession`: tes3mobileActor/combatSession.html
.. _`corpseHourstamp`: tes3mobileActor/corpseHourstamp.html
.. _`currentEnchantedItem`: tes3mobileActor/currentEnchantedItem.html
.. _`currentSpell`: tes3mobileActor/currentSpell.html
.. _`effectAttributes`: tes3mobileActor/effectAttributes.html
.. _`encumbrance`: tes3mobileActor/encumbrance.html
.. _`endurance`: tes3mobileActor/endurance.html
.. _`facing`: tes3mobileActor/facing.html
.. _`fatigue`: tes3mobileActor/fatigue.html
.. _`fight`: tes3mobileActor/fight.html
.. _`flags`: tes3mobileActor/flags.html
.. _`flee`: tes3mobileActor/flee.html
.. _`friendlyActors`: tes3mobileActor/friendlyActors.html
.. _`greetDuration`: tes3mobileActor/greetDuration.html
.. _`greetTimer`: tes3mobileActor/greetTimer.html
.. _`health`: tes3mobileActor/health.html
.. _`height`: tes3mobileActor/height.html
.. _`hello`: tes3mobileActor/hello.html
.. _`holdBreathTime`: tes3mobileActor/holdBreathTime.html
.. _`hostileActors`: tes3mobileActor/hostileActors.html
.. _`idleAnim`: tes3mobileActor/idleAnim.html
.. _`impulseVelocity`: tes3mobileActor/impulseVelocity.html
.. _`inCombat`: tes3mobileActor/inCombat.html
.. _`intelligence`: tes3mobileActor/intelligence.html
.. _`invisibility`: tes3mobileActor/invisibility.html
.. _`isCrittable`: tes3mobileActor/isCrittable.html
.. _`isDead`: tes3mobileActor/isDead.html
.. _`isFalling`: tes3mobileActor/isFalling.html
.. _`isFlying`: tes3mobileActor/isFlying.html
.. _`isJumping`: tes3mobileActor/isJumping.html
.. _`isMovingBack`: tes3mobileActor/isMovingBack.html
.. _`isMovingForward`: tes3mobileActor/isMovingForward.html
.. _`isMovingLeft`: tes3mobileActor/isMovingLeft.html
.. _`isMovingRight`: tes3mobileActor/isMovingRight.html
.. _`isPlayerDetected`: tes3mobileActor/isPlayerDetected.html
.. _`isPlayerHidden`: tes3mobileActor/isPlayerHidden.html
.. _`isRunning`: tes3mobileActor/isRunning.html
.. _`isSneaking`: tes3mobileActor/isSneaking.html
.. _`isSwimming`: tes3mobileActor/isSwimming.html
.. _`isTurningLeft`: tes3mobileActor/isTurningLeft.html
.. _`isTurningRight`: tes3mobileActor/isTurningRight.html
.. _`isWalking`: tes3mobileActor/isWalking.html
.. _`jump`: tes3mobileActor/jump.html
.. _`lastGroundZ`: tes3mobileActor/lastGroundZ.html
.. _`levitate`: tes3mobileActor/levitate.html
.. _`luck`: tes3mobileActor/luck.html
.. _`magicka`: tes3mobileActor/magicka.html
.. _`magickaMultiplier`: tes3mobileActor/magickaMultiplier.html
.. _`mobToMobCollision`: tes3mobileActor/mobToMobCollision.html
.. _`movementCollision`: tes3mobileActor/movementCollision.html
.. _`movementFlags`: tes3mobileActor/movementFlags.html
.. _`nextActionWeight`: tes3mobileActor/nextActionWeight.html
.. _`objectType`: tes3mobileActor/objectType.html
.. _`paralyze`: tes3mobileActor/paralyze.html
.. _`personality`: tes3mobileActor/personality.html
.. _`playerDistance`: tes3mobileActor/playerDistance.html
.. _`position`: tes3mobileActor/position.html
.. _`prevMovementFlags`: tes3mobileActor/prevMovementFlags.html
.. _`readiedAmmo`: tes3mobileActor/readiedAmmo.html
.. _`readiedAmmoCount`: tes3mobileActor/readiedAmmoCount.html
.. _`readiedShield`: tes3mobileActor/readiedShield.html
.. _`readiedWeapon`: tes3mobileActor/readiedWeapon.html
.. _`reference`: tes3mobileActor/reference.html
.. _`resistBlightDisease`: tes3mobileActor/resistBlightDisease.html
.. _`resistCommonDisease`: tes3mobileActor/resistCommonDisease.html
.. _`resistCorprus`: tes3mobileActor/resistCorprus.html
.. _`resistFire`: tes3mobileActor/resistFire.html
.. _`resistFrost`: tes3mobileActor/resistFrost.html
.. _`resistMagicka`: tes3mobileActor/resistMagicka.html
.. _`resistNormalWeapons`: tes3mobileActor/resistNormalWeapons.html
.. _`resistParalysis`: tes3mobileActor/resistParalysis.html
.. _`resistPoison`: tes3mobileActor/resistPoison.html
.. _`resistShock`: tes3mobileActor/resistShock.html
.. _`sanctuary`: tes3mobileActor/sanctuary.html
.. _`scanInterval`: tes3mobileActor/scanInterval.html
.. _`scanTimer`: tes3mobileActor/scanTimer.html
.. _`shield`: tes3mobileActor/shield.html
.. _`silence`: tes3mobileActor/silence.html
.. _`sound`: tes3mobileActor/sound.html
.. _`speed`: tes3mobileActor/speed.html
.. _`spellReadied`: tes3mobileActor/spellReadied.html
.. _`strength`: tes3mobileActor/strength.html
.. _`swiftSwim`: tes3mobileActor/swiftSwim.html
.. _`talkedTo`: tes3mobileActor/talkedTo.html
.. _`torchSlot`: tes3mobileActor/torchSlot.html
.. _`underwater`: tes3mobileActor/underwater.html
.. _`velocity`: tes3mobileActor/velocity.html
.. _`waterBreathing`: tes3mobileActor/waterBreathing.html
.. _`waterWalking`: tes3mobileActor/waterWalking.html
.. _`weaponDrawn`: tes3mobileActor/weaponDrawn.html
.. _`werewolf`: tes3mobileActor/werewolf.html
.. _`width`: tes3mobileActor/width.html
.. _`willpower`: tes3mobileActor/willpower.html

Methods
----------------------------------------------------------------------------------------------------

`applyDamage`_ (`number`_)
    Damages the actor, with options to control mitigation and difficulty scaling. Invokes the 'damage' and 'damaged' events, with 'script' source. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

`applyFatigueDamage`_ (`number`_)
    Damages the actor's fatigue, with accompanying reaction from the reciever. Invokes the 'damageHandToHand' and 'damagedHandToHand' events, with 'script' source. Returns the actual fatigue damage done.

`applyHealthDamage`_ (`boolean`_)
    **Deprecated, please use applyDamage instead.**

Damages the actor.

`calcEffectiveDamage`_ (`number`_)
    Calculates the damage that would be inflicted to an actor after armor and/or resistance. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.

`getActiveMagicEffects`_ (`table`_)
    Fetches a filtered list of the active magic effects on the actor. Returns a table with tes3activeMagicEffect items.

`getBootsWeight`_ (`number`_)
    Read-only. Gets the weight of the boots equipped on the actor, or 0 if no boots are equipped.

`getFatigueTerm`_ (`number`_)
    Gets the fatigue-based skill scaling term used by many game mechanics, based on the actor's current and maximum fatigue. It is equal to ``max(0, fFatigueBase - fFatigueMult * max(0, 1 - fatigue.current/fatigue.base))``

`getPowerUseTimestamp`_ (`number`_)
    Finds the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp. The timestamp units are hours. The current time as a timestamp can be accessed at tes3.getSimulationTimestamp().

`getSkillStatistic`_ (`tes3skillStatistic`_)
    Fetches the statistic object of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.

`getSkillValue`_ (`number`_)
    Fetches the current value of a skill with a given index. This is the way to access skills for any type of actor, as creatures have a limited version of the skill system. Note that creatures share a statistic between multiple skills (they only have combat, magic, and stealth stats), so many values will be the same.

`getViewToActor`_ (`number`_)
    No description available.

`getViewToPoint`_ (`number`_)
    No description available.

`getViewToPointWithFacing`_ (`number`_)
    No description available.

`getWeaponSpeed`_ (`number`_)
    Fetches the weapon speed of the actor's currently equipped weapon, or 1.0 if no weapon is equipped.

`hasFreeAction`_ (`boolean`_)
    If true, the actor isn't paralyzed, dead, stunned, or otherwise unable to take action.

`hasUsedPower`_ (`boolean`_)
    Check if a power has been used and is recharging.

`isAffectedByObject`_ (`boolean`_)
    Determines if the actor is currently being affected by a given alchemy, enchantment, or spell.

`rechargePower`_ (`number`_)
    Makes a power immediately available for casting again.

`setPowerUseTimestamp`_
    Sets the timestamp a recharging power was used. Powers recharge 24 hours after this timestamp.

`startCombat`_
    Forces the actor into combat with another actor.

`startDialogue`_
    Starts dialogue with this actor for the player.

`stopCombat`_
    Ends combat for the actor.

`updateDerivedStatistics`_
    Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.

`updateOpacity`_
    Updates the actor's visual opacity. Used after modifying applied chameleon or invisiblity effects.

.. toctree::
    :hidden:

    tes3mobileActor/applyDamage
    tes3mobileActor/applyFatigueDamage
    tes3mobileActor/applyHealthDamage
    tes3mobileActor/calcEffectiveDamage
    tes3mobileActor/getActiveMagicEffects
    tes3mobileActor/getBootsWeight
    tes3mobileActor/getFatigueTerm
    tes3mobileActor/getPowerUseTimestamp
    tes3mobileActor/getSkillStatistic
    tes3mobileActor/getSkillValue
    tes3mobileActor/getViewToActor
    tes3mobileActor/getViewToPoint
    tes3mobileActor/getViewToPointWithFacing
    tes3mobileActor/getWeaponSpeed
    tes3mobileActor/hasFreeAction
    tes3mobileActor/hasUsedPower
    tes3mobileActor/isAffectedByObject
    tes3mobileActor/rechargePower
    tes3mobileActor/setPowerUseTimestamp
    tes3mobileActor/startCombat
    tes3mobileActor/startDialogue
    tes3mobileActor/stopCombat
    tes3mobileActor/updateDerivedStatistics
    tes3mobileActor/updateOpacity

.. _`applyDamage`: tes3mobileActor/applyDamage.html
.. _`applyFatigueDamage`: tes3mobileActor/applyFatigueDamage.html
.. _`applyHealthDamage`: tes3mobileActor/applyHealthDamage.html
.. _`calcEffectiveDamage`: tes3mobileActor/calcEffectiveDamage.html
.. _`getActiveMagicEffects`: tes3mobileActor/getActiveMagicEffects.html
.. _`getBootsWeight`: tes3mobileActor/getBootsWeight.html
.. _`getFatigueTerm`: tes3mobileActor/getFatigueTerm.html
.. _`getPowerUseTimestamp`: tes3mobileActor/getPowerUseTimestamp.html
.. _`getSkillStatistic`: tes3mobileActor/getSkillStatistic.html
.. _`getSkillValue`: tes3mobileActor/getSkillValue.html
.. _`getViewToActor`: tes3mobileActor/getViewToActor.html
.. _`getViewToPoint`: tes3mobileActor/getViewToPoint.html
.. _`getViewToPointWithFacing`: tes3mobileActor/getViewToPointWithFacing.html
.. _`getWeaponSpeed`: tes3mobileActor/getWeaponSpeed.html
.. _`hasFreeAction`: tes3mobileActor/hasFreeAction.html
.. _`hasUsedPower`: tes3mobileActor/hasUsedPower.html
.. _`isAffectedByObject`: tes3mobileActor/isAffectedByObject.html
.. _`rechargePower`: tes3mobileActor/rechargePower.html
.. _`setPowerUseTimestamp`: tes3mobileActor/setPowerUseTimestamp.html
.. _`startCombat`: tes3mobileActor/startCombat.html
.. _`startDialogue`: tes3mobileActor/startDialogue.html
.. _`stopCombat`: tes3mobileActor/stopCombat.html
.. _`updateDerivedStatistics`: tes3mobileActor/updateDerivedStatistics.html
.. _`updateOpacity`: tes3mobileActor/updateOpacity.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actionData`: ../../lua/type/tes3actionData.html
.. _`tes3actorAnimationController`: ../../lua/type/tes3actorAnimationController.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3combatSession`: ../../lua/type/tes3combatSession.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
.. _`tes3statistic`: ../../lua/type/tes3statistic.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
