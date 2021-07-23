tes3mobileCreature
====================================================================================================

A mobile object for a creature.

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

`combat`_ (`tes3statistic`_)
    Read-only. Direct access to the creature's combat statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all combat skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.

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

`flySpeed`_ (`number`_)
    Read-only. The calculated fly movement speed.

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

`magic`_ (`tes3statistic`_)
    Read-only. Direct access to the creature's magic statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all magic skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.

`magicka`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka statistic.

`magickaMultiplier`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka multiplier statistic.

`mobToMobCollision`_ (`boolean`_)
    Allows modifying if this actor will collide with other actors. When true (default), the actor cannot move through other actors. When false, the actor is allowed to move through other actors, and other actors can move through it.

May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.

`moveSpeed`_ (`number`_)
    Read-only. The calculated base movement speed.

`movementCollision`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`nextActionWeight`_ (`number`_)
    Read-only. No description available.

`object`_ (`tes3creatureInstance`_)
    Read-only. The actor object that maps to this mobile.

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

`runSpeed`_ (`number`_)
    Read-only. The calculated run movement speed.

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

`skills`_ (`table`_)
    Read-only. An array-style table with access to the three creature skill statistics.

`sound`_ (`number`_)
    Direct access to the actor's sound effect attribute.

`speed`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's speed attribute statistic.

`spellReadied`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor has a spell readied.

`stealth`_ (`tes3statistic`_)
    Read-only. Direct access to the creature's stealth statistic. This is a creature-specific generalization of skills, where this statistic is used in place of all stealth skills. To get an individual skill value by skill ID, see the getSkillValue or getSkillStatistic methods.

`strength`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's strength attribute statistic.

`swiftSwim`_ (`number`_)
    Direct access to the actor's swift swim effect attribute.

`swimRunSpeed`_ (`number`_)
    Read-only. The calculated swim movement speed while running.

`swimSpeed`_ (`number`_)
    Read-only. The calculated swim movement speed.

`talkedTo`_ (`boolean`_)
    Direct access to the actor's flag that shows it was recently talked to.

`torchSlot`_ (`tes3equipmentStack`_)
    The currently equipped light.

`underwater`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor is under water.

`velocity`_ (`tes3vector3`_)
    A vector that represents the 3D velocity of the object.

`walkSpeed`_ (`number`_)
    Read-only. The calculated walk movement speed.

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

    tes3mobileCreature/actionBeforeCombat
    tes3mobileCreature/actionData
    tes3mobileCreature/activeAI
    tes3mobileCreature/activeMagicEffectList
    tes3mobileCreature/actorType
    tes3mobileCreature/agility
    tes3mobileCreature/aiPlanner
    tes3mobileCreature/alarm
    tes3mobileCreature/animationController
    tes3mobileCreature/armorRating
    tes3mobileCreature/attackBonus
    tes3mobileCreature/attacked
    tes3mobileCreature/attributes
    tes3mobileCreature/barterGold
    tes3mobileCreature/blind
    tes3mobileCreature/boundSize
    tes3mobileCreature/cell
    tes3mobileCreature/cellX
    tes3mobileCreature/cellY
    tes3mobileCreature/chameleon
    tes3mobileCreature/collidingReference
    tes3mobileCreature/combat
    tes3mobileCreature/combatSession
    tes3mobileCreature/corpseHourstamp
    tes3mobileCreature/currentEnchantedItem
    tes3mobileCreature/currentSpell
    tes3mobileCreature/effectAttributes
    tes3mobileCreature/encumbrance
    tes3mobileCreature/endurance
    tes3mobileCreature/facing
    tes3mobileCreature/fatigue
    tes3mobileCreature/fight
    tes3mobileCreature/flags
    tes3mobileCreature/flee
    tes3mobileCreature/flySpeed
    tes3mobileCreature/friendlyActors
    tes3mobileCreature/greetDuration
    tes3mobileCreature/greetTimer
    tes3mobileCreature/health
    tes3mobileCreature/height
    tes3mobileCreature/hello
    tes3mobileCreature/holdBreathTime
    tes3mobileCreature/hostileActors
    tes3mobileCreature/idleAnim
    tes3mobileCreature/impulseVelocity
    tes3mobileCreature/inCombat
    tes3mobileCreature/intelligence
    tes3mobileCreature/invisibility
    tes3mobileCreature/isCrittable
    tes3mobileCreature/isDead
    tes3mobileCreature/isFalling
    tes3mobileCreature/isFlying
    tes3mobileCreature/isJumping
    tes3mobileCreature/isMovingBack
    tes3mobileCreature/isMovingForward
    tes3mobileCreature/isMovingLeft
    tes3mobileCreature/isMovingRight
    tes3mobileCreature/isPlayerDetected
    tes3mobileCreature/isPlayerHidden
    tes3mobileCreature/isRunning
    tes3mobileCreature/isSneaking
    tes3mobileCreature/isSwimming
    tes3mobileCreature/isTurningLeft
    tes3mobileCreature/isTurningRight
    tes3mobileCreature/isWalking
    tes3mobileCreature/jump
    tes3mobileCreature/lastGroundZ
    tes3mobileCreature/levitate
    tes3mobileCreature/luck
    tes3mobileCreature/magic
    tes3mobileCreature/magicka
    tes3mobileCreature/magickaMultiplier
    tes3mobileCreature/mobToMobCollision
    tes3mobileCreature/moveSpeed
    tes3mobileCreature/movementCollision
    tes3mobileCreature/movementFlags
    tes3mobileCreature/nextActionWeight
    tes3mobileCreature/object
    tes3mobileCreature/objectType
    tes3mobileCreature/paralyze
    tes3mobileCreature/personality
    tes3mobileCreature/playerDistance
    tes3mobileCreature/position
    tes3mobileCreature/prevMovementFlags
    tes3mobileCreature/readiedAmmo
    tes3mobileCreature/readiedAmmoCount
    tes3mobileCreature/readiedShield
    tes3mobileCreature/readiedWeapon
    tes3mobileCreature/reference
    tes3mobileCreature/resistBlightDisease
    tes3mobileCreature/resistCommonDisease
    tes3mobileCreature/resistCorprus
    tes3mobileCreature/resistFire
    tes3mobileCreature/resistFrost
    tes3mobileCreature/resistMagicka
    tes3mobileCreature/resistNormalWeapons
    tes3mobileCreature/resistParalysis
    tes3mobileCreature/resistPoison
    tes3mobileCreature/resistShock
    tes3mobileCreature/runSpeed
    tes3mobileCreature/sanctuary
    tes3mobileCreature/scanInterval
    tes3mobileCreature/scanTimer
    tes3mobileCreature/shield
    tes3mobileCreature/silence
    tes3mobileCreature/skills
    tes3mobileCreature/sound
    tes3mobileCreature/speed
    tes3mobileCreature/spellReadied
    tes3mobileCreature/stealth
    tes3mobileCreature/strength
    tes3mobileCreature/swiftSwim
    tes3mobileCreature/swimRunSpeed
    tes3mobileCreature/swimSpeed
    tes3mobileCreature/talkedTo
    tes3mobileCreature/torchSlot
    tes3mobileCreature/underwater
    tes3mobileCreature/velocity
    tes3mobileCreature/walkSpeed
    tes3mobileCreature/waterBreathing
    tes3mobileCreature/waterWalking
    tes3mobileCreature/weaponDrawn
    tes3mobileCreature/werewolf
    tes3mobileCreature/width
    tes3mobileCreature/willpower

.. _`actionBeforeCombat`: tes3mobileCreature/actionBeforeCombat.html
.. _`actionData`: tes3mobileCreature/actionData.html
.. _`activeAI`: tes3mobileCreature/activeAI.html
.. _`activeMagicEffectList`: tes3mobileCreature/activeMagicEffectList.html
.. _`actorType`: tes3mobileCreature/actorType.html
.. _`agility`: tes3mobileCreature/agility.html
.. _`aiPlanner`: tes3mobileCreature/aiPlanner.html
.. _`alarm`: tes3mobileCreature/alarm.html
.. _`animationController`: tes3mobileCreature/animationController.html
.. _`armorRating`: tes3mobileCreature/armorRating.html
.. _`attackBonus`: tes3mobileCreature/attackBonus.html
.. _`attacked`: tes3mobileCreature/attacked.html
.. _`attributes`: tes3mobileCreature/attributes.html
.. _`barterGold`: tes3mobileCreature/barterGold.html
.. _`blind`: tes3mobileCreature/blind.html
.. _`boundSize`: tes3mobileCreature/boundSize.html
.. _`cell`: tes3mobileCreature/cell.html
.. _`cellX`: tes3mobileCreature/cellX.html
.. _`cellY`: tes3mobileCreature/cellY.html
.. _`chameleon`: tes3mobileCreature/chameleon.html
.. _`collidingReference`: tes3mobileCreature/collidingReference.html
.. _`combat`: tes3mobileCreature/combat.html
.. _`combatSession`: tes3mobileCreature/combatSession.html
.. _`corpseHourstamp`: tes3mobileCreature/corpseHourstamp.html
.. _`currentEnchantedItem`: tes3mobileCreature/currentEnchantedItem.html
.. _`currentSpell`: tes3mobileCreature/currentSpell.html
.. _`effectAttributes`: tes3mobileCreature/effectAttributes.html
.. _`encumbrance`: tes3mobileCreature/encumbrance.html
.. _`endurance`: tes3mobileCreature/endurance.html
.. _`facing`: tes3mobileCreature/facing.html
.. _`fatigue`: tes3mobileCreature/fatigue.html
.. _`fight`: tes3mobileCreature/fight.html
.. _`flags`: tes3mobileCreature/flags.html
.. _`flee`: tes3mobileCreature/flee.html
.. _`flySpeed`: tes3mobileCreature/flySpeed.html
.. _`friendlyActors`: tes3mobileCreature/friendlyActors.html
.. _`greetDuration`: tes3mobileCreature/greetDuration.html
.. _`greetTimer`: tes3mobileCreature/greetTimer.html
.. _`health`: tes3mobileCreature/health.html
.. _`height`: tes3mobileCreature/height.html
.. _`hello`: tes3mobileCreature/hello.html
.. _`holdBreathTime`: tes3mobileCreature/holdBreathTime.html
.. _`hostileActors`: tes3mobileCreature/hostileActors.html
.. _`idleAnim`: tes3mobileCreature/idleAnim.html
.. _`impulseVelocity`: tes3mobileCreature/impulseVelocity.html
.. _`inCombat`: tes3mobileCreature/inCombat.html
.. _`intelligence`: tes3mobileCreature/intelligence.html
.. _`invisibility`: tes3mobileCreature/invisibility.html
.. _`isCrittable`: tes3mobileCreature/isCrittable.html
.. _`isDead`: tes3mobileCreature/isDead.html
.. _`isFalling`: tes3mobileCreature/isFalling.html
.. _`isFlying`: tes3mobileCreature/isFlying.html
.. _`isJumping`: tes3mobileCreature/isJumping.html
.. _`isMovingBack`: tes3mobileCreature/isMovingBack.html
.. _`isMovingForward`: tes3mobileCreature/isMovingForward.html
.. _`isMovingLeft`: tes3mobileCreature/isMovingLeft.html
.. _`isMovingRight`: tes3mobileCreature/isMovingRight.html
.. _`isPlayerDetected`: tes3mobileCreature/isPlayerDetected.html
.. _`isPlayerHidden`: tes3mobileCreature/isPlayerHidden.html
.. _`isRunning`: tes3mobileCreature/isRunning.html
.. _`isSneaking`: tes3mobileCreature/isSneaking.html
.. _`isSwimming`: tes3mobileCreature/isSwimming.html
.. _`isTurningLeft`: tes3mobileCreature/isTurningLeft.html
.. _`isTurningRight`: tes3mobileCreature/isTurningRight.html
.. _`isWalking`: tes3mobileCreature/isWalking.html
.. _`jump`: tes3mobileCreature/jump.html
.. _`lastGroundZ`: tes3mobileCreature/lastGroundZ.html
.. _`levitate`: tes3mobileCreature/levitate.html
.. _`luck`: tes3mobileCreature/luck.html
.. _`magic`: tes3mobileCreature/magic.html
.. _`magicka`: tes3mobileCreature/magicka.html
.. _`magickaMultiplier`: tes3mobileCreature/magickaMultiplier.html
.. _`mobToMobCollision`: tes3mobileCreature/mobToMobCollision.html
.. _`moveSpeed`: tes3mobileCreature/moveSpeed.html
.. _`movementCollision`: tes3mobileCreature/movementCollision.html
.. _`movementFlags`: tes3mobileCreature/movementFlags.html
.. _`nextActionWeight`: tes3mobileCreature/nextActionWeight.html
.. _`object`: tes3mobileCreature/object.html
.. _`objectType`: tes3mobileCreature/objectType.html
.. _`paralyze`: tes3mobileCreature/paralyze.html
.. _`personality`: tes3mobileCreature/personality.html
.. _`playerDistance`: tes3mobileCreature/playerDistance.html
.. _`position`: tes3mobileCreature/position.html
.. _`prevMovementFlags`: tes3mobileCreature/prevMovementFlags.html
.. _`readiedAmmo`: tes3mobileCreature/readiedAmmo.html
.. _`readiedAmmoCount`: tes3mobileCreature/readiedAmmoCount.html
.. _`readiedShield`: tes3mobileCreature/readiedShield.html
.. _`readiedWeapon`: tes3mobileCreature/readiedWeapon.html
.. _`reference`: tes3mobileCreature/reference.html
.. _`resistBlightDisease`: tes3mobileCreature/resistBlightDisease.html
.. _`resistCommonDisease`: tes3mobileCreature/resistCommonDisease.html
.. _`resistCorprus`: tes3mobileCreature/resistCorprus.html
.. _`resistFire`: tes3mobileCreature/resistFire.html
.. _`resistFrost`: tes3mobileCreature/resistFrost.html
.. _`resistMagicka`: tes3mobileCreature/resistMagicka.html
.. _`resistNormalWeapons`: tes3mobileCreature/resistNormalWeapons.html
.. _`resistParalysis`: tes3mobileCreature/resistParalysis.html
.. _`resistPoison`: tes3mobileCreature/resistPoison.html
.. _`resistShock`: tes3mobileCreature/resistShock.html
.. _`runSpeed`: tes3mobileCreature/runSpeed.html
.. _`sanctuary`: tes3mobileCreature/sanctuary.html
.. _`scanInterval`: tes3mobileCreature/scanInterval.html
.. _`scanTimer`: tes3mobileCreature/scanTimer.html
.. _`shield`: tes3mobileCreature/shield.html
.. _`silence`: tes3mobileCreature/silence.html
.. _`skills`: tes3mobileCreature/skills.html
.. _`sound`: tes3mobileCreature/sound.html
.. _`speed`: tes3mobileCreature/speed.html
.. _`spellReadied`: tes3mobileCreature/spellReadied.html
.. _`stealth`: tes3mobileCreature/stealth.html
.. _`strength`: tes3mobileCreature/strength.html
.. _`swiftSwim`: tes3mobileCreature/swiftSwim.html
.. _`swimRunSpeed`: tes3mobileCreature/swimRunSpeed.html
.. _`swimSpeed`: tes3mobileCreature/swimSpeed.html
.. _`talkedTo`: tes3mobileCreature/talkedTo.html
.. _`torchSlot`: tes3mobileCreature/torchSlot.html
.. _`underwater`: tes3mobileCreature/underwater.html
.. _`velocity`: tes3mobileCreature/velocity.html
.. _`walkSpeed`: tes3mobileCreature/walkSpeed.html
.. _`waterBreathing`: tes3mobileCreature/waterBreathing.html
.. _`waterWalking`: tes3mobileCreature/waterWalking.html
.. _`weaponDrawn`: tes3mobileCreature/weaponDrawn.html
.. _`werewolf`: tes3mobileCreature/werewolf.html
.. _`width`: tes3mobileCreature/width.html
.. _`willpower`: tes3mobileCreature/willpower.html

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

    tes3mobileCreature/applyDamage
    tes3mobileCreature/applyFatigueDamage
    tes3mobileCreature/applyHealthDamage
    tes3mobileCreature/calcEffectiveDamage
    tes3mobileCreature/getActiveMagicEffects
    tes3mobileCreature/getBootsWeight
    tes3mobileCreature/getFatigueTerm
    tes3mobileCreature/getPowerUseTimestamp
    tes3mobileCreature/getSkillStatistic
    tes3mobileCreature/getSkillValue
    tes3mobileCreature/getViewToActor
    tes3mobileCreature/getViewToPoint
    tes3mobileCreature/getViewToPointWithFacing
    tes3mobileCreature/getWeaponSpeed
    tes3mobileCreature/hasFreeAction
    tes3mobileCreature/hasUsedPower
    tes3mobileCreature/isAffectedByObject
    tes3mobileCreature/rechargePower
    tes3mobileCreature/setPowerUseTimestamp
    tes3mobileCreature/startCombat
    tes3mobileCreature/startDialogue
    tes3mobileCreature/stopCombat
    tes3mobileCreature/updateDerivedStatistics
    tes3mobileCreature/updateOpacity

.. _`applyDamage`: tes3mobileCreature/applyDamage.html
.. _`applyFatigueDamage`: tes3mobileCreature/applyFatigueDamage.html
.. _`applyHealthDamage`: tes3mobileCreature/applyHealthDamage.html
.. _`calcEffectiveDamage`: tes3mobileCreature/calcEffectiveDamage.html
.. _`getActiveMagicEffects`: tes3mobileCreature/getActiveMagicEffects.html
.. _`getBootsWeight`: tes3mobileCreature/getBootsWeight.html
.. _`getFatigueTerm`: tes3mobileCreature/getFatigueTerm.html
.. _`getPowerUseTimestamp`: tes3mobileCreature/getPowerUseTimestamp.html
.. _`getSkillStatistic`: tes3mobileCreature/getSkillStatistic.html
.. _`getSkillValue`: tes3mobileCreature/getSkillValue.html
.. _`getViewToActor`: tes3mobileCreature/getViewToActor.html
.. _`getViewToPoint`: tes3mobileCreature/getViewToPoint.html
.. _`getViewToPointWithFacing`: tes3mobileCreature/getViewToPointWithFacing.html
.. _`getWeaponSpeed`: tes3mobileCreature/getWeaponSpeed.html
.. _`hasFreeAction`: tes3mobileCreature/hasFreeAction.html
.. _`hasUsedPower`: tes3mobileCreature/hasUsedPower.html
.. _`isAffectedByObject`: tes3mobileCreature/isAffectedByObject.html
.. _`rechargePower`: tes3mobileCreature/rechargePower.html
.. _`setPowerUseTimestamp`: tes3mobileCreature/setPowerUseTimestamp.html
.. _`startCombat`: tes3mobileCreature/startCombat.html
.. _`startDialogue`: tes3mobileCreature/startDialogue.html
.. _`stopCombat`: tes3mobileCreature/stopCombat.html
.. _`updateDerivedStatistics`: tes3mobileCreature/updateDerivedStatistics.html
.. _`updateOpacity`: tes3mobileCreature/updateOpacity.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actionData`: ../../lua/type/tes3actionData.html
.. _`tes3actorAnimationController`: ../../lua/type/tes3actorAnimationController.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3combatSession`: ../../lua/type/tes3combatSession.html
.. _`tes3creatureInstance`: ../../lua/type/tes3creatureInstance.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
.. _`tes3statistic`: ../../lua/type/tes3statistic.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
