tes3mobileNPC
====================================================================================================

A mobile object for an NPC.

Properties
----------------------------------------------------------------------------------------------------

`acrobatics`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's acrobatics skill statistic.

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

`alchemy`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's alchemy skill statistic.

`alteration`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's alteration skill statistic.

`animationController`_ (`tes3actorAnimationController`_)
    Read-only. No description available.

`armorRating`_ (`number`_)
    Read-only. The actor's current armour rating, taking equipment condition into account. Armour mitigation can be automatically applied to damage by using the applyDamage function.

Armour mitigation calculation:
x = damage / (damage + target.armorRating)
damage *= max(fCombatArmorMinMult, x)
if damage < 1 then damage = 1 end

`armorer`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's armorer skill statistic.

`athletics`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's athletics skill statistic.

`attackBonus`_ (`number`_)
    Direct access to the actor's attack bonus effect attribute.

`attacked`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor has been attacked.

`attributes`_ (`table`_)
    Read-only. Access to a table of 8 tes3statistic objects for the actor's attributes.

`axe`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's axe skill statistic.

`barterGold`_ (`number`_)
    The current amount of gold that the actor has access to for bartering.

`blind`_ (`number`_)
    Direct access to the actor's blind effect attribute.

`block`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's block skill statistic.

`bluntWeapon`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's blunt weapon skill statistic.

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

`conjuration`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's conjuration skill statistic.

`corpseHourstamp`_ (`number`_)
    No description available.

`currentEnchantedItem`_ (`tes3equipmentStack`_)
    Read-only. The currently equipped enchanted item that the actor will use.

`currentSpell`_ (`tes3spell`_)
    Read-only. The currently equipped spell that the actor will use.

`destruction`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's destruction skill statistic.

`effectAttributes`_ (`table`_)
    Read-only. Access to a table of 24 numbers for the actor's effect attributes.

`enchant`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's enchant skill statistic.

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

`forceJump`_ (`boolean`_)
    Toggle flag for if the NPC jumps.

`forceMoveJump`_ (`boolean`_)
    Toggle flag for if the NPC moves and jumps.

`forceRun`_ (`boolean`_)
    Toggle flag for if the NPC runs.

`forceSneak`_ (`boolean`_)
    Toggle flag for if the NPC sneaks.

`friendlyActors`_ (`tes3iterator`_)
    Read-only. A collection of other tes3mobileActors that this actor considers friendly.

`greetDuration`_ (`number`_)
    No description available.

`greetTimer`_ (`number`_)
    Read-only. No description available.

`handToHand`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's hand to hand skill statistic.

`health`_ (`tes3statistic`_)
    Read-only. Access to the actor's health statistic.

`heavyArmor`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's heavy armor skill statistic.

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

`illusion`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's illusion skill statistic.

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

`lightArmor`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's light armor skill statistic.

`longBlade`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's long blade skill statistic.

`luck`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's luck attribute statistic.

`magicka`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka statistic.

`magickaMultiplier`_ (`tes3statistic`_)
    Read-only. Access to the actor's magicka multiplier statistic.

`marksman`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's marksman skill statistic.

`mediumArmor`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's medium armor skill statistic.

`mercantile`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's mercantile skill statistic.

`mobToMobCollision`_ (`boolean`_)
    Allows modifying if this actor will collide with other actors. When true (default), the actor cannot move through other actors. When false, the actor is allowed to move through other actors, and other actors can move through it.

May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.

`moveSpeed`_ (`number`_)
    Read-only. The calculated base movement speed.

`movementCollision`_ (`boolean`_)
    Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.

`movementFlags`_ (`number`_)
    Read-only. Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.

`mysticism`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's mysticism skill statistic.

`nextActionWeight`_ (`number`_)
    Read-only. No description available.

`object`_ (`tes3npcInstance`_)
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

`restoration`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's restoration skill statistic.

`runSpeed`_ (`number`_)
    Read-only. The calculated run movement speed.

`sanctuary`_ (`number`_)
    Direct access to the actor's sanctuary effect attribute.

`scanInterval`_ (`number`_)
    Read-only. No description available.

`scanTimer`_ (`number`_)
    Read-only. No description available.

`security`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's security skill statistic.

`shield`_ (`number`_)
    Direct access to the actor's shield effect attribute.

`shortBlade`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's short blade skill statistic.

`silence`_ (`number`_)
    Direct access to the actor's silence effect attribute.

`skills`_ (`table`_)
    Read-only. An array-style table with access to the twenty seven NPC skill statistics.

`sneak`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's sneak skill statistic.

`sound`_ (`number`_)
    Direct access to the actor's sound effect attribute.

`spear`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's spear skill statistic.

`speechcraft`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's speechcraft skill statistic.

`speed`_ (`tes3statistic`_)
    Read-only. Direct access to the actor's speed attribute statistic.

`spellReadied`_ (`boolean`_)
    Read-only. Friendly access to the actor's flag that controls if the actor has a spell readied.

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

`unarmored`_ (`tes3skillStatistic`_)
    Read-only. Direct access to the NPC's unarmored skill statistic.

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

    tes3mobileNPC/acrobatics
    tes3mobileNPC/actionBeforeCombat
    tes3mobileNPC/actionData
    tes3mobileNPC/activeAI
    tes3mobileNPC/activeMagicEffectList
    tes3mobileNPC/actorType
    tes3mobileNPC/agility
    tes3mobileNPC/aiPlanner
    tes3mobileNPC/alarm
    tes3mobileNPC/alchemy
    tes3mobileNPC/alteration
    tes3mobileNPC/animationController
    tes3mobileNPC/armorRating
    tes3mobileNPC/armorer
    tes3mobileNPC/athletics
    tes3mobileNPC/attackBonus
    tes3mobileNPC/attacked
    tes3mobileNPC/attributes
    tes3mobileNPC/axe
    tes3mobileNPC/barterGold
    tes3mobileNPC/blind
    tes3mobileNPC/block
    tes3mobileNPC/bluntWeapon
    tes3mobileNPC/boundSize
    tes3mobileNPC/cell
    tes3mobileNPC/cellX
    tes3mobileNPC/cellY
    tes3mobileNPC/chameleon
    tes3mobileNPC/collidingReference
    tes3mobileNPC/combatSession
    tes3mobileNPC/conjuration
    tes3mobileNPC/corpseHourstamp
    tes3mobileNPC/currentEnchantedItem
    tes3mobileNPC/currentSpell
    tes3mobileNPC/destruction
    tes3mobileNPC/effectAttributes
    tes3mobileNPC/enchant
    tes3mobileNPC/encumbrance
    tes3mobileNPC/endurance
    tes3mobileNPC/facing
    tes3mobileNPC/fatigue
    tes3mobileNPC/fight
    tes3mobileNPC/flags
    tes3mobileNPC/flee
    tes3mobileNPC/flySpeed
    tes3mobileNPC/forceJump
    tes3mobileNPC/forceMoveJump
    tes3mobileNPC/forceRun
    tes3mobileNPC/forceSneak
    tes3mobileNPC/friendlyActors
    tes3mobileNPC/greetDuration
    tes3mobileNPC/greetTimer
    tes3mobileNPC/handToHand
    tes3mobileNPC/health
    tes3mobileNPC/heavyArmor
    tes3mobileNPC/height
    tes3mobileNPC/hello
    tes3mobileNPC/holdBreathTime
    tes3mobileNPC/hostileActors
    tes3mobileNPC/idleAnim
    tes3mobileNPC/illusion
    tes3mobileNPC/impulseVelocity
    tes3mobileNPC/inCombat
    tes3mobileNPC/intelligence
    tes3mobileNPC/invisibility
    tes3mobileNPC/isCrittable
    tes3mobileNPC/isDead
    tes3mobileNPC/isFalling
    tes3mobileNPC/isFlying
    tes3mobileNPC/isJumping
    tes3mobileNPC/isMovingBack
    tes3mobileNPC/isMovingForward
    tes3mobileNPC/isMovingLeft
    tes3mobileNPC/isMovingRight
    tes3mobileNPC/isPlayerDetected
    tes3mobileNPC/isPlayerHidden
    tes3mobileNPC/isRunning
    tes3mobileNPC/isSneaking
    tes3mobileNPC/isSwimming
    tes3mobileNPC/isTurningLeft
    tes3mobileNPC/isTurningRight
    tes3mobileNPC/isWalking
    tes3mobileNPC/jump
    tes3mobileNPC/lastGroundZ
    tes3mobileNPC/levitate
    tes3mobileNPC/lightArmor
    tes3mobileNPC/longBlade
    tes3mobileNPC/luck
    tes3mobileNPC/magicka
    tes3mobileNPC/magickaMultiplier
    tes3mobileNPC/marksman
    tes3mobileNPC/mediumArmor
    tes3mobileNPC/mercantile
    tes3mobileNPC/mobToMobCollision
    tes3mobileNPC/moveSpeed
    tes3mobileNPC/movementCollision
    tes3mobileNPC/movementFlags
    tes3mobileNPC/mysticism
    tes3mobileNPC/nextActionWeight
    tes3mobileNPC/object
    tes3mobileNPC/objectType
    tes3mobileNPC/paralyze
    tes3mobileNPC/personality
    tes3mobileNPC/playerDistance
    tes3mobileNPC/position
    tes3mobileNPC/prevMovementFlags
    tes3mobileNPC/readiedAmmo
    tes3mobileNPC/readiedAmmoCount
    tes3mobileNPC/readiedShield
    tes3mobileNPC/readiedWeapon
    tes3mobileNPC/reference
    tes3mobileNPC/resistBlightDisease
    tes3mobileNPC/resistCommonDisease
    tes3mobileNPC/resistCorprus
    tes3mobileNPC/resistFire
    tes3mobileNPC/resistFrost
    tes3mobileNPC/resistMagicka
    tes3mobileNPC/resistNormalWeapons
    tes3mobileNPC/resistParalysis
    tes3mobileNPC/resistPoison
    tes3mobileNPC/resistShock
    tes3mobileNPC/restoration
    tes3mobileNPC/runSpeed
    tes3mobileNPC/sanctuary
    tes3mobileNPC/scanInterval
    tes3mobileNPC/scanTimer
    tes3mobileNPC/security
    tes3mobileNPC/shield
    tes3mobileNPC/shortBlade
    tes3mobileNPC/silence
    tes3mobileNPC/skills
    tes3mobileNPC/sneak
    tes3mobileNPC/sound
    tes3mobileNPC/spear
    tes3mobileNPC/speechcraft
    tes3mobileNPC/speed
    tes3mobileNPC/spellReadied
    tes3mobileNPC/strength
    tes3mobileNPC/swiftSwim
    tes3mobileNPC/swimRunSpeed
    tes3mobileNPC/swimSpeed
    tes3mobileNPC/talkedTo
    tes3mobileNPC/torchSlot
    tes3mobileNPC/unarmored
    tes3mobileNPC/underwater
    tes3mobileNPC/velocity
    tes3mobileNPC/walkSpeed
    tes3mobileNPC/waterBreathing
    tes3mobileNPC/waterWalking
    tes3mobileNPC/weaponDrawn
    tes3mobileNPC/werewolf
    tes3mobileNPC/width
    tes3mobileNPC/willpower

.. _`acrobatics`: tes3mobileNPC/acrobatics.html
.. _`actionBeforeCombat`: tes3mobileNPC/actionBeforeCombat.html
.. _`actionData`: tes3mobileNPC/actionData.html
.. _`activeAI`: tes3mobileNPC/activeAI.html
.. _`activeMagicEffectList`: tes3mobileNPC/activeMagicEffectList.html
.. _`actorType`: tes3mobileNPC/actorType.html
.. _`agility`: tes3mobileNPC/agility.html
.. _`aiPlanner`: tes3mobileNPC/aiPlanner.html
.. _`alarm`: tes3mobileNPC/alarm.html
.. _`alchemy`: tes3mobileNPC/alchemy.html
.. _`alteration`: tes3mobileNPC/alteration.html
.. _`animationController`: tes3mobileNPC/animationController.html
.. _`armorRating`: tes3mobileNPC/armorRating.html
.. _`armorer`: tes3mobileNPC/armorer.html
.. _`athletics`: tes3mobileNPC/athletics.html
.. _`attackBonus`: tes3mobileNPC/attackBonus.html
.. _`attacked`: tes3mobileNPC/attacked.html
.. _`attributes`: tes3mobileNPC/attributes.html
.. _`axe`: tes3mobileNPC/axe.html
.. _`barterGold`: tes3mobileNPC/barterGold.html
.. _`blind`: tes3mobileNPC/blind.html
.. _`block`: tes3mobileNPC/block.html
.. _`bluntWeapon`: tes3mobileNPC/bluntWeapon.html
.. _`boundSize`: tes3mobileNPC/boundSize.html
.. _`cell`: tes3mobileNPC/cell.html
.. _`cellX`: tes3mobileNPC/cellX.html
.. _`cellY`: tes3mobileNPC/cellY.html
.. _`chameleon`: tes3mobileNPC/chameleon.html
.. _`collidingReference`: tes3mobileNPC/collidingReference.html
.. _`combatSession`: tes3mobileNPC/combatSession.html
.. _`conjuration`: tes3mobileNPC/conjuration.html
.. _`corpseHourstamp`: tes3mobileNPC/corpseHourstamp.html
.. _`currentEnchantedItem`: tes3mobileNPC/currentEnchantedItem.html
.. _`currentSpell`: tes3mobileNPC/currentSpell.html
.. _`destruction`: tes3mobileNPC/destruction.html
.. _`effectAttributes`: tes3mobileNPC/effectAttributes.html
.. _`enchant`: tes3mobileNPC/enchant.html
.. _`encumbrance`: tes3mobileNPC/encumbrance.html
.. _`endurance`: tes3mobileNPC/endurance.html
.. _`facing`: tes3mobileNPC/facing.html
.. _`fatigue`: tes3mobileNPC/fatigue.html
.. _`fight`: tes3mobileNPC/fight.html
.. _`flags`: tes3mobileNPC/flags.html
.. _`flee`: tes3mobileNPC/flee.html
.. _`flySpeed`: tes3mobileNPC/flySpeed.html
.. _`forceJump`: tes3mobileNPC/forceJump.html
.. _`forceMoveJump`: tes3mobileNPC/forceMoveJump.html
.. _`forceRun`: tes3mobileNPC/forceRun.html
.. _`forceSneak`: tes3mobileNPC/forceSneak.html
.. _`friendlyActors`: tes3mobileNPC/friendlyActors.html
.. _`greetDuration`: tes3mobileNPC/greetDuration.html
.. _`greetTimer`: tes3mobileNPC/greetTimer.html
.. _`handToHand`: tes3mobileNPC/handToHand.html
.. _`health`: tes3mobileNPC/health.html
.. _`heavyArmor`: tes3mobileNPC/heavyArmor.html
.. _`height`: tes3mobileNPC/height.html
.. _`hello`: tes3mobileNPC/hello.html
.. _`holdBreathTime`: tes3mobileNPC/holdBreathTime.html
.. _`hostileActors`: tes3mobileNPC/hostileActors.html
.. _`idleAnim`: tes3mobileNPC/idleAnim.html
.. _`illusion`: tes3mobileNPC/illusion.html
.. _`impulseVelocity`: tes3mobileNPC/impulseVelocity.html
.. _`inCombat`: tes3mobileNPC/inCombat.html
.. _`intelligence`: tes3mobileNPC/intelligence.html
.. _`invisibility`: tes3mobileNPC/invisibility.html
.. _`isCrittable`: tes3mobileNPC/isCrittable.html
.. _`isDead`: tes3mobileNPC/isDead.html
.. _`isFalling`: tes3mobileNPC/isFalling.html
.. _`isFlying`: tes3mobileNPC/isFlying.html
.. _`isJumping`: tes3mobileNPC/isJumping.html
.. _`isMovingBack`: tes3mobileNPC/isMovingBack.html
.. _`isMovingForward`: tes3mobileNPC/isMovingForward.html
.. _`isMovingLeft`: tes3mobileNPC/isMovingLeft.html
.. _`isMovingRight`: tes3mobileNPC/isMovingRight.html
.. _`isPlayerDetected`: tes3mobileNPC/isPlayerDetected.html
.. _`isPlayerHidden`: tes3mobileNPC/isPlayerHidden.html
.. _`isRunning`: tes3mobileNPC/isRunning.html
.. _`isSneaking`: tes3mobileNPC/isSneaking.html
.. _`isSwimming`: tes3mobileNPC/isSwimming.html
.. _`isTurningLeft`: tes3mobileNPC/isTurningLeft.html
.. _`isTurningRight`: tes3mobileNPC/isTurningRight.html
.. _`isWalking`: tes3mobileNPC/isWalking.html
.. _`jump`: tes3mobileNPC/jump.html
.. _`lastGroundZ`: tes3mobileNPC/lastGroundZ.html
.. _`levitate`: tes3mobileNPC/levitate.html
.. _`lightArmor`: tes3mobileNPC/lightArmor.html
.. _`longBlade`: tes3mobileNPC/longBlade.html
.. _`luck`: tes3mobileNPC/luck.html
.. _`magicka`: tes3mobileNPC/magicka.html
.. _`magickaMultiplier`: tes3mobileNPC/magickaMultiplier.html
.. _`marksman`: tes3mobileNPC/marksman.html
.. _`mediumArmor`: tes3mobileNPC/mediumArmor.html
.. _`mercantile`: tes3mobileNPC/mercantile.html
.. _`mobToMobCollision`: tes3mobileNPC/mobToMobCollision.html
.. _`moveSpeed`: tes3mobileNPC/moveSpeed.html
.. _`movementCollision`: tes3mobileNPC/movementCollision.html
.. _`movementFlags`: tes3mobileNPC/movementFlags.html
.. _`mysticism`: tes3mobileNPC/mysticism.html
.. _`nextActionWeight`: tes3mobileNPC/nextActionWeight.html
.. _`object`: tes3mobileNPC/object.html
.. _`objectType`: tes3mobileNPC/objectType.html
.. _`paralyze`: tes3mobileNPC/paralyze.html
.. _`personality`: tes3mobileNPC/personality.html
.. _`playerDistance`: tes3mobileNPC/playerDistance.html
.. _`position`: tes3mobileNPC/position.html
.. _`prevMovementFlags`: tes3mobileNPC/prevMovementFlags.html
.. _`readiedAmmo`: tes3mobileNPC/readiedAmmo.html
.. _`readiedAmmoCount`: tes3mobileNPC/readiedAmmoCount.html
.. _`readiedShield`: tes3mobileNPC/readiedShield.html
.. _`readiedWeapon`: tes3mobileNPC/readiedWeapon.html
.. _`reference`: tes3mobileNPC/reference.html
.. _`resistBlightDisease`: tes3mobileNPC/resistBlightDisease.html
.. _`resistCommonDisease`: tes3mobileNPC/resistCommonDisease.html
.. _`resistCorprus`: tes3mobileNPC/resistCorprus.html
.. _`resistFire`: tes3mobileNPC/resistFire.html
.. _`resistFrost`: tes3mobileNPC/resistFrost.html
.. _`resistMagicka`: tes3mobileNPC/resistMagicka.html
.. _`resistNormalWeapons`: tes3mobileNPC/resistNormalWeapons.html
.. _`resistParalysis`: tes3mobileNPC/resistParalysis.html
.. _`resistPoison`: tes3mobileNPC/resistPoison.html
.. _`resistShock`: tes3mobileNPC/resistShock.html
.. _`restoration`: tes3mobileNPC/restoration.html
.. _`runSpeed`: tes3mobileNPC/runSpeed.html
.. _`sanctuary`: tes3mobileNPC/sanctuary.html
.. _`scanInterval`: tes3mobileNPC/scanInterval.html
.. _`scanTimer`: tes3mobileNPC/scanTimer.html
.. _`security`: tes3mobileNPC/security.html
.. _`shield`: tes3mobileNPC/shield.html
.. _`shortBlade`: tes3mobileNPC/shortBlade.html
.. _`silence`: tes3mobileNPC/silence.html
.. _`skills`: tes3mobileNPC/skills.html
.. _`sneak`: tes3mobileNPC/sneak.html
.. _`sound`: tes3mobileNPC/sound.html
.. _`spear`: tes3mobileNPC/spear.html
.. _`speechcraft`: tes3mobileNPC/speechcraft.html
.. _`speed`: tes3mobileNPC/speed.html
.. _`spellReadied`: tes3mobileNPC/spellReadied.html
.. _`strength`: tes3mobileNPC/strength.html
.. _`swiftSwim`: tes3mobileNPC/swiftSwim.html
.. _`swimRunSpeed`: tes3mobileNPC/swimRunSpeed.html
.. _`swimSpeed`: tes3mobileNPC/swimSpeed.html
.. _`talkedTo`: tes3mobileNPC/talkedTo.html
.. _`torchSlot`: tes3mobileNPC/torchSlot.html
.. _`unarmored`: tes3mobileNPC/unarmored.html
.. _`underwater`: tes3mobileNPC/underwater.html
.. _`velocity`: tes3mobileNPC/velocity.html
.. _`walkSpeed`: tes3mobileNPC/walkSpeed.html
.. _`waterBreathing`: tes3mobileNPC/waterBreathing.html
.. _`waterWalking`: tes3mobileNPC/waterWalking.html
.. _`weaponDrawn`: tes3mobileNPC/weaponDrawn.html
.. _`werewolf`: tes3mobileNPC/werewolf.html
.. _`width`: tes3mobileNPC/width.html
.. _`willpower`: tes3mobileNPC/willpower.html

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

`equip`_ (`boolean`_)
    Equips an item, optionally adding the item if needed. If the best match is already equipped, it does not perform an unequip-equip cycle, but does return true.

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

`unequip`_ (`boolean`_)
    Unequips one or more items from the actor.

`updateDerivedStatistics`_
    Updates statistics derived from attributes, which are magicka, fatigue, and encumbrance. Will also update the UI if used on the player. Normally handled automatically when you use tes3.modStatistic.

`updateOpacity`_
    Updates the actor's visual opacity. Used after modifying applied chameleon or invisiblity effects.

.. toctree::
    :hidden:

    tes3mobileNPC/applyDamage
    tes3mobileNPC/applyFatigueDamage
    tes3mobileNPC/applyHealthDamage
    tes3mobileNPC/calcEffectiveDamage
    tes3mobileNPC/equip
    tes3mobileNPC/getActiveMagicEffects
    tes3mobileNPC/getBootsWeight
    tes3mobileNPC/getFatigueTerm
    tes3mobileNPC/getPowerUseTimestamp
    tes3mobileNPC/getSkillStatistic
    tes3mobileNPC/getSkillValue
    tes3mobileNPC/getViewToActor
    tes3mobileNPC/getViewToPoint
    tes3mobileNPC/getViewToPointWithFacing
    tes3mobileNPC/getWeaponSpeed
    tes3mobileNPC/hasFreeAction
    tes3mobileNPC/hasUsedPower
    tes3mobileNPC/isAffectedByObject
    tes3mobileNPC/rechargePower
    tes3mobileNPC/setPowerUseTimestamp
    tes3mobileNPC/startCombat
    tes3mobileNPC/startDialogue
    tes3mobileNPC/stopCombat
    tes3mobileNPC/unequip
    tes3mobileNPC/updateDerivedStatistics
    tes3mobileNPC/updateOpacity

.. _`applyDamage`: tes3mobileNPC/applyDamage.html
.. _`applyFatigueDamage`: tes3mobileNPC/applyFatigueDamage.html
.. _`applyHealthDamage`: tes3mobileNPC/applyHealthDamage.html
.. _`calcEffectiveDamage`: tes3mobileNPC/calcEffectiveDamage.html
.. _`equip`: tes3mobileNPC/equip.html
.. _`getActiveMagicEffects`: tes3mobileNPC/getActiveMagicEffects.html
.. _`getBootsWeight`: tes3mobileNPC/getBootsWeight.html
.. _`getFatigueTerm`: tes3mobileNPC/getFatigueTerm.html
.. _`getPowerUseTimestamp`: tes3mobileNPC/getPowerUseTimestamp.html
.. _`getSkillStatistic`: tes3mobileNPC/getSkillStatistic.html
.. _`getSkillValue`: tes3mobileNPC/getSkillValue.html
.. _`getViewToActor`: tes3mobileNPC/getViewToActor.html
.. _`getViewToPoint`: tes3mobileNPC/getViewToPoint.html
.. _`getViewToPointWithFacing`: tes3mobileNPC/getViewToPointWithFacing.html
.. _`getWeaponSpeed`: tes3mobileNPC/getWeaponSpeed.html
.. _`hasFreeAction`: tes3mobileNPC/hasFreeAction.html
.. _`hasUsedPower`: tes3mobileNPC/hasUsedPower.html
.. _`isAffectedByObject`: tes3mobileNPC/isAffectedByObject.html
.. _`rechargePower`: tes3mobileNPC/rechargePower.html
.. _`setPowerUseTimestamp`: tes3mobileNPC/setPowerUseTimestamp.html
.. _`startCombat`: tes3mobileNPC/startCombat.html
.. _`startDialogue`: tes3mobileNPC/startDialogue.html
.. _`stopCombat`: tes3mobileNPC/stopCombat.html
.. _`unequip`: tes3mobileNPC/unequip.html
.. _`updateDerivedStatistics`: tes3mobileNPC/updateDerivedStatistics.html
.. _`updateOpacity`: tes3mobileNPC/updateOpacity.html

.. _`boolean`: ../../lua/type/boolean.html
.. _`number`: ../../lua/type/number.html
.. _`table`: ../../lua/type/table.html
.. _`tes3actionData`: ../../lua/type/tes3actionData.html
.. _`tes3actorAnimationController`: ../../lua/type/tes3actorAnimationController.html
.. _`tes3cell`: ../../lua/type/tes3cell.html
.. _`tes3combatSession`: ../../lua/type/tes3combatSession.html
.. _`tes3equipmentStack`: ../../lua/type/tes3equipmentStack.html
.. _`tes3iterator`: ../../lua/type/tes3iterator.html
.. _`tes3npcInstance`: ../../lua/type/tes3npcInstance.html
.. _`tes3reference`: ../../lua/type/tes3reference.html
.. _`tes3spell`: ../../lua/type/tes3spell.html
.. _`tes3statistic`: ../../lua/type/tes3statistic.html
.. _`tes3vector3`: ../../lua/type/tes3vector3.html
