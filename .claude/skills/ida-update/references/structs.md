# Morrowind.exe — Key Struct Field Offsets

Quick-reference for the most-accessed structs. All offsets hex. Sizes given in parentheses.

**Sections:** Mobile Actors · AI/Combat · Animation · World Objects · Subsystems · Magic · Script · Scene Properties · Collections · Misc Game Objects

---

## MobileObject (0x80) — base of all simulated actors
```
+0x00  InterfaceMACH*    vtbl
+0x04  char[4]           tag              'MACT'/'MACH'/'MACR'/'MACP'
+0x08  u16               movementFlags
+0x0a  u16               previousMovementFlags
+0x10  u32               mobileFlags
+0x14  Reference*        reference
+0x18  MobileCollision*  arrayCollisionResults
+0x1c  i16               cellX
+0x1e  i16               cellY
+0x2c  float             height
+0x30  vec2              boundSize
+0x3c  vec3              velocity
+0x48  vec3              impulseVelocity
+0x54  vec3              positionAtLastLightUpdate
+0x60  NiCollisionGroup* collisionGroup
+0x64  float             framestepDistanceMoved
+0x68  vec3              framestepDeltaPosition
+0x7c  u8                countCollisionResults
```

## MACT (0x3b0) — all actors
```
+0x00  MobileObject      mobile           (base struct inlined)
+0x80  TList             listTargetActors
+0x94  TList             listFriendlyActors
+0xa8  float             scanTimer
+0xac  float             scanInterval
+0xb0  float             greetTimer
+0xb4  vec3              targetActorInitialPosition
+0xc0  AIPersistentBehavior persistedAIState
+0xc8  AIPlanner*        aiPlanner
+0xcc  ActionData        actionData       (size 0x70)
+0x13c ActionData        actionPreviousBeforeCombat
+0x1b8 MACT::PRDT*       prdt
+0x1bc MACT::PathPlanner* pathPlanner
+0x1c0 CombatSession*    aiCombatSession
+0x1c4 StdList           activeMagicEffects
+0x1d4 MobileCollision   collision_1D4
+0x224 i8                startCombatReason
+0x225 char              aiBehaviourState_saved
+0x228 MobileActorType   actorType
+0x244 AnimControllerUnion animController
+0x254 Attribute         strength
+0x260 Attribute         intelligence
+0x26c Attribute         willpower
+0x278 Attribute         agility
+0x284 Attribute         speed
+0x290 Attribute         endurance
+0x29c Attribute         personality
+0x2a8 Attribute         luck
+0x2b4 Attribute         health
+0x2c0 Attribute         magicka
+0x2cc Attribute         encumbrance
+0x2d8 Attribute         fatigue
+0x2f0 int[24]           effectAttributes
+0x350 int               fight
+0x354 int               flee
+0x358 int               hello
+0x35c int               alarm
+0x360 int               barterGold
+0x370 float             holdBreathTime
+0x378 MagicSourceCombo  currentMagic
+0x388 EquipmentStack*   readiedWeapon
+0x38c EquipmentStack*   readiedAmmo
+0x390 EquipmentStack*   readiedShield
+0x394 EquipmentStack*   torchSlot
+0x39c NiNode*           arrowBone
+0x3a4 vec3              position_3A4
```

## MACH (0x56c) : MACT
```
+0x3b0 AttributeSkill[27] skills
+0x560 NPCClone*         npcClone
+0x564 u8                isVampire
+0x565 u8                flagForceSneak
+0x566 u8                flagForceRun
+0x567 u8                flagForceJump
```

## MACR (0x3d8) : MACT
```
+0x3b0 CreatureClone*    creatureClone
+0x3b4 Attribute         skillCombat
+0x3c0 Attribute         skillMagic
+0x3cc Attribute         skillStealth
```

## MACP (0x694) : MACH
```
+0x56c int[8]            levelupPerAttributeCount
+0x58c int[3]            levelupPerSpecialization
+0x598 PlayerBounty*     bounty
+0x5b0 char              flagControlsDisabled
+0x5b1 char              flagJumpingDisabled
+0x5b2 char              flagMouselookDisabled
+0x5b4 char              flagVanityDisabled
+0x5b5 char              flagAttackDisabled
+0x5b6 char              flagMagicDisabled
+0x5b8 char              flagAutoRun
+0x5b9 char              flagSleeping
+0x5bb char              flagWaiting
+0x5f4 float[27]         skillProgress
+0x660 Reference*        refr1stPerson
+0x664 NPCBase*          npc1stPerson
+0x670 Birthsign*        birthsign
+0x678 MarkRecallPosition* pMarkLocation
+0x67c vec3              lastPositionOfFogOfWarUpdate
+0x690 GlobalVar*        globKnownWerewolf
```

## AIPlanner (0x98)
```
+0x00  AIPackageConfig*  aiPackageSetup
+0x04  MACT*             mobileActor
+0x08  int               refCount
+0x0c  AIPackage*[32]    aiPackages
+0x8c  int               nextOpenPackageIndex
+0x90  int               currentPackageIndex
+0x94  float             deltaTime
```

## AIPackage (0x3c) — base
```
+0x00  InterfaceAIPackage* vtbl
+0x04  char              usingPathPlanner_maybe
+0x05  char              moving
+0x08  float             startGameHour
+0x0c  MACT*             targetActor
+0x10  AIPackageType     packageType
+0x18  u16               duration
+0x1c  uint              distance
+0x20  int               hourOfDay
+0x24  vec3              targetPosition
+0x30  char              done
+0x31  char              started
+0x32  char              reset
+0x33  char              finalized
+0x34  MACT*             owningActor
+0x38  Cell*             destinationCell
```

## CombatSession (0x4c)
```
+0x00  float             combatDistance
+0x04  char              field_4
+0x08  int               ammoDamage
+0x0c  MACT*             mobile
+0x10  EquipmentStack*   selectedWeapon
+0x18  EquipmentStack*   selectedShield
+0x1c  ItemStack*        selectedItem
+0x20  Spell*            nextSpell
+0x28  CombatSessionNextAction nextAction
+0x2c  float             spellPriority
+0x30  float             alchemyPriority
+0x34  StdList_Generic   spells
+0x40  float             combatDelayTimer
+0x44  float             combatDelayNextHitTime
+0x48  char              maybe_allowPotionUse
+0x49  char              maybe_canUseSpell
+0x4a  char              maybe_canUseEnchantedItem
```

## Reference (0x50)
```
+0x00  Entity            super
+0x28  EntityUnion       entity          tagged union: STAT/DOOR/CONT/NPC_/CREA/etc.
+0x2c  vec3              orientation     Euler angles (radians)
+0x38  vec3              position
+0x44  RefrData_Base*    attachments     linked list head
+0x48  int               sourceID
+0x4c  int               targetID
```

## NiObject (0x8)
```
+0x00  InterfaceNiObjectUnion* vtbl
+0x04  int               refCount
```

## NiAVObject (0x90) — inherits NiObjectNET → NiObject
```
+0x14  u16               flags           bit 1 = AppCulled (hidden from renderer)
+0x18  NiNode*           parentNode
+0x1c  NiBound           worldBound      centre(vec3@+0x1c) + radius(float@+0x28)
+0x2c  LocalTransform    localTransform  translate(vec3) + rotate(matrix33) + scale(float)
+0x40  NiTransform       worldTransform  world-space equivalent
+0x74  NiAVObject::Physics* pkVelocities
+0x78  NiBoundingVolume* modelABV
+0x7c  NiBoundingVolume* worldABV
+0x80  fn*               collideCallback
+0x88  NiPropertyList    propertyList
```

## NiNode (0xb0) : NiAVObject
```
+0x90  TArray            kChildren       data/count/capacity
+0xa8  NiDynamicEffectList effectList
```

## ActorAnimationData (0x7e4)
```
+0x00  NiNode*           sgActorNode
+0x04  NiNode*           sgMRTNode       Movement Root Translation keyframe node
+0x08  vec3              positionDeltaOfMRT
+0x14  NiNode*           sgSpine1
+0x18  NiNode*           sgSpine2
+0x1c  float             spineAngle
+0x20  NiNode*           sgHead
+0x38  u8[3]             currentAnimGroups  [lower, upper, special]
+0x3c  int[3]            currentActionIndices
+0x48  int[3]            loopCounts
+0x58  float[3]          timings
+0x64  float             deltaTime
+0x68  AnimationGroup*[150] animGroups
+0x2c0 NiKeyframeManager* manager
+0x2c4 AnimationKeyframeGroup[3] keyframeSources
+0x314 char[150]         animGroupSourceIndices
+0x3aa i16[150]          approxRootMovementSpeed
+0x4d8 float             movementSpeed
+0x4dc float             weaponSpeed
+0x7dc u8                nextAnimGroup
```

## WorldController (0x374) — key fields
```
+0x00  int               framesSinceLastFPSMeasure
+0x14  float             framesPerSecond
+0x2c  float             deltaTime
+0x30  NiDX8Renderer*    renderer
+0x34  AudioController*  audio
+0x4c  InputController*  inputController
+0x58  WeatherController* weatherController
+0x5c  MobManager*       mobManager
+0x70  ActiveMagicManager* activeMagicManager
+0x9c  float             worldController::aiDistance
+0xa8  GlobalVar*        gvarGameHour
+0xd6  bool              flagMenuMode
+0xd8  bool              collisionEnabled
+0xda  bool              disableAI
+0xf4  NiNode*           nodeCursor
+0x124 WorldControllerRenderCamera worldCamera
+0x150 WorldControllerRenderCamera armCamera  (1st-person)
+0x2e0 MapController*    mapController
+0x2e4 MenuController*   menuController
+0x330 float             deadFloatScale
+0x33c TList*            listActiveGlobalScripts
+0x33c TList*            listAllActors
```

## DataHandler (0xb558) — key fields
```
+0x00  RecordsHandler*   records
+0x04  ExteriorData*[3][3] arrayExteriorCellData  (3×3 loaded grid)
+0x28  ExteriorData*[5][5] backgroundLoadExteriorCellData
+0x8c  NiNode*           sgWorldObjectRoot
+0x90  NiNode*           sgWorldPickObjectRoot
+0x94  NiNode*           sgWorldLandscapeRoot
+0x98  NiDirectionalLight* sgSunlight
+0xa0  int               centralGridX
+0xa4  int               centralGridY
+0xac  Cell*             currentInteriorCell     NULL if outdoors
+0xb4c0 int              collisionCentreX
+0xb4e8 THashMap*        textureCacheHashMap
+0xb540 Cell*            currentPCCell
```

## MobManager (0x8c)
```
+0x00  NiCollisionGroup* mobCollisionGroup
+0x04  vec3              gravity
+0x10  vec3              terminalVelocity
+0x1c  float             dotProductOfMaxClimableSlope
+0x24  ProcessManager*   processManager
+0x28  ProjectileManager* projectileManager
+0x30  TList             listPropColliders
+0x44  NiCriticalSection propCollisionCriticalSection
+0x68  NiCriticalSection mobCollisionCriticalSection
```

## ProcessManager (0x830)
```
+0x00  MACP*             player
+0x04  TList             listAIPlanners       — iterated for head-tracking
+0x1c  NiCriticalSection plannerListCriticalSection
+0x58  int               countPlanners
+0x5c  AIPlanner*[500]   arrayPlanners        — iterated for simulateActor()
+0x82c float             aiDistance
```

`ProcessManager::update()` @ `0x56f4d0`:
- Locks `plannerListCriticalSection`, iterates `listAIPlanners` for head-tracking pass
- Unlocks, runs `AIPlanner::simulateActor()` on all active planners in `arrayPlanners`
- `AIPlanner::simulateActor()` → `mobileActor->vtbl->simulate()` → `MACT::simulate()`
- Skip condition: actor not `MobileActorFlags_ActiveInSimulation` or ref is `RecordFlags_Disabled`

---

## Magic System

### MagicSourceInstance (0x12c)
```
+0x00  Record            super
+0x10  float             overrideCastChance
+0x14  Reference*        referenceTarget
+0x18  char              bypassResistances
+0x1c  THashMapMagicEffectInstance[8] tempMagicEffectInstance
+0x9c  SpellProjectile*  magicProjectile
+0xa0  MagicSourceCombo  magicSourceCombo
+0xa8  uint              serialNumber         — key in ActiveMagicManager's StdMap
+0xac  float             corprusHoursSinceLastWorsen
+0xb0  MagicSourceInstance::Data data
+0x104 Sound*[8]         soundEffects
+0x124 float             countOnTargetEffects
+0x128 Reference*        mcpAddedField_originalTarget   (MWSE extension)
```

### MagicEffectInstance (0x38)
```
+0x00  Reference*        target
+0x04  float             resistedPercent
+0x08  uint              magnitude
+0x0c  float             timeActive
+0x10  float             cumulativeMagnitude
+0x14  MagicEffectState  state             Retired = retire triggers cleanup
+0x18  VFXInstance*      visualEffect
+0x1c  EquipmentStack*[2] createdData
+0x24  EquipmentStack*   lastUsedWeapon
+0x28  EquipmentStack*   lastUsedArmour
+0x2c  EquipmentStack*   lastUsedShield
+0x30  EquipmentStack*   lastUsedLight
+0x34  EquipmentStack*   lastUsedEnchItem
```

### MagicEffectData (0x110)
```
+0x00  Record            super
+0x10  int               effectID
+0x14  char*             description
+0x1c  char[32]          iconPath
+0x3c  char[32]          particleTexPath
+0x5c  char[32]          castSoundID
+0x7c  char[32]          boltSoundID
+0x9c  char[32]          hitSoundID
+0xbc  char[32]          areaSoundID
+0xdc  EntityPhysical*   vfxCast
+0xe0  EntityPhysical*   vfxBolt
+0xe4  EntityPhysical*   vfxHit
+0xe8  EntityPhysical*   vfxArea
+0xec  int               spellSchool
+0xf0  float             baseCost
+0xf4  int               flags
+0xf8  int[3]            colour
+0x104 float             size / speed / sizeCap
```

### ActiveMagicManager (0x38)
```
+0x00  NiNode*           sgWorldSpellRoot
+0x04  char              flagNoProcess
+0x08  StdMap            mapSerialToMagicSourceInstance   — keyed by serialNumber
+0x18  StdMap            mapItemDataToSerial
+0x28  StdMap            mapReferenceToSerial
```
`processAllMagics()` @ `0x455290`: iterates `mapSerialToMagicSourceInstance`, calls
`MagicSourceInstance::process(deltaTime)` on each; removes retired instances.
`MagicSourceInstance::process()` @ `0x514380` — the 0x19d7-byte per-frame effect handler.

---

## Script System

### ScriptRecord (0x70)
```
+0x00  Record            super
+0x10  char[32]          id
+0x30  int               shortVarCount
+0x34  int               longVarCount
+0x38  int               floatVarCount
+0x3c  int               scriptDataSize
+0x40  int               localVarSize
+0x44  char**            shortVarNames
+0x48  char**            longVarNames
+0x4c  char**            floatVarNames
+0x58  u8*               bytecode
+0x5c  ScriptLocalData   scriptLocalData
```

Key functions:
- `ScriptRecord::execute()` @ `0x5028a0` — main execution entry, called from global-script loop and `TES3Game::mainLoop`
- `ScriptRecord::scriptOp()` @ `0x505770` — bytecode opcode dispatch (0x7ed8 bytes — the interpreter core)
- `ScriptRecord::evalExpression()` @ `0x4ffcc0` — expression evaluator
- `ScriptRecord::getParams()` @ `0x500510` — parameter parsing

---

## Collections

### TList (0x14)
```
+0x00  vtbl*
+0x04  int               count
+0x08  TList::Node*      firstNode
+0x0c  TList::Node*      lastNode
+0x10  TList::Node*      iterator          — cursor for iterateBegin/iterateNext
```
Iteration: `TList::iterateBegin(&list)` returns first `Node*`; `node->next` walks forward.
Each node has `data` (TaggedListNode union with typed accessors) and `next`/`prev`.

### TArray (0x18)
```
+0x00  vtbl*
+0x04  void**            storage
+0x08  int               storageCount      — capacity
+0x0c  uint              endIndex
+0x10  int               filledCount       — number of valid entries
+0x14  int               growByCount
```

### THashMap (0x10)
```
+0x00  vtbl*
+0x04  int               count
+0x08  uint              bucketCount
+0x0c  THashMap::Node**  arrayBuckets
```

### Inventory (0x1c)
```
+0x00  char              flags
+0x04  TList             items             — list of ItemStack
+0x18  EntityLight*      internalLight
```

### ItemStack (0xc)
```
+0x00  int               count
+0x04  EntityUnion       item              — tagged pointer to Entity record
+0x08  TArray*           itemDataArray     — array of ItemData* (one per instance)
```

### EquipmentStack (0x8)
```
+0x00  EntityUnion       item
+0x04  ItemData*         itemData
```

### ItemData (0x1c)
```
+0x00  int               stackCount
+0x04  ItemData::owner   (union: NPC*, Faction*)
+0x08  ItemData::lockVariable_rankReq
+0x0c  ItemData::condition_lightTime
+0x10  ItemData::enchantCharge_soul
+0x14  ScriptRecord*     script
+0x18  ScriptLocalData*  scriptData
```

---

## ActionData (0x70) — combat state per actor
```
+0x00  i16               fightAttrCombatChange
+0x02  i16               dispositionCombatChange
+0x04  float             attackSwing
+0x08  fn*               swingTimer
+0x0c  float             physicalDamage
+0x10  char              aiBehaviourState
+0x11  AnimStateAttack   animStateAttack
+0x12  char              blockingState
+0x13  AnimGroupID       animGroupNextStun
+0x14  PhysicalAttackType physicalAttackType
+0x16  AnimGroupID       animGroupCurrentAction
+0x18  i16[3]            quantizedHitAngle (x/y/z)
+0x20  MACT*             targetActor
+0x24  MACT*             hitTarget
+0x2c  Entity*           stolenFromFactionOrNPC
+0x34  MobileProjectile* nockedProjectile
+0x38  i16               countAIPackages
+0x3c  vec3              position_3C
+0x48  vec3              vec_48
+0x54  vec3              maybe_positionBeforeCombat
+0x60  vec3              walkDestination
+0x6c  float             lastWitnessedCrimeTimestamp
```

## Attribute (0xc)
```
+0x00  vtbl*
+0x04  float             base
+0x08  float             current           — base + modifiers; clamp to [0, 100] or similar
```
No `modifier` field — modifiers are tracked via `effectAttributes[]` on MACT.

## ActorAnimController (0xd4) — `MACT::animController` union member
```
+0x00  vtbl*
+0x04  char              useAnimationDelta  — gates root-motion output
+0x08  AnimGroupID       animGroupAttack
+0x0c  ActorAnimPlaybackType playbackType
+0x34  float             speedMultiplier
+0x38  MACT*             mobileActor
+0x3c  ActorAnimationData* animationAttachment
+0x40  NiPointer         sgAlphaProperty
+0x44  NiPointer         sgMaterialProperty
+0x4c  matrix33          groundPlaneRotation
+0x70  matrix33          swimmingFlyingRotation
+0x98  AnimGroupID       animGroupMovement
+0x99  AnimGroupID       animGroupIdle
+0xa4  BodySection       sectionLowerBody
+0xb4  BodySection       sectionUpperBody
+0xc4  BodySection       sectionLeftArm
```

## AnimationGroup (0x2c)
```
+0x00  Record            super
+0x10  u8                animationGroupId
+0x14  int               actionCount
+0x18  int*              actionFrameNumbers
+0x1c  float*            actionTimes
+0x20  AnimationGroup*   nextGroup
+0x24  uint              soundGenCount
+0x28  AnimationGroupSoundGenKey* soundGenNotes
```

## AnimationKeyframeGroup (0xc) — lower/upper/leftArm layer sources
```
+0x00  NiSequence*       seqLower
+0x04  NiSequence*       seqUpper
+0x08  NiSequence*       seqLeftArm
```

## NiKeyframeManager (0x78) : NiTimeController
```
+0x00  NiTimeController  super
+0x34  THashMap          kfSeqMap
+0x44  char              cumulative
+0x48  matrix33          globalScaleRotation
+0x6c  vec3              globalTranslation
```

## BSAnimationManager (0xc8) : NiNode
```
+0x00  NiNode            super
+0xb0  TArray            managedNodes
```
`BSAnimationManager::Update(mgr, cumulativeTime, camera)` — drives all animation in a cell/camera.

## MobileCollision (0x40) — single collision result record
```
+0x00  char              valid
+0x04  float             time
+0x08  vec3              point
+0x14  vec3              objectPosAtCollision
+0x20  vec3              velocity
+0x2c  NiPointer         sgColliderRoot
+0x30  Reference*        colliderReference
+0x38  i16[3]            quantizedNormal
+0x3e  char              collisionType
+0x3f  char              processingState
```

## AIPackage subclasses
```
AIPackageTravel (0x5c)   +0x3c vec3 destination,  +0x58 PathPlanner*
AIPackageWander (0x98)   +0x3c vec3 wanderDest,   +0x50 PathPlanner*, +0x54 PathGrid*, +0x58 AIPackageWanderIdle[8]
AIPackageEscort (0x50)   +0x3c vec3 destination,  +0x4c PathPlanner*
AIPackageFollow (0x68)   +0x3c vec3 destination,  +0x4c StdList, +0x64 float followDistance
```

## Spell (0xf8) and Enchantment (0xf8)
```
Spell:
+0x28  char*             stringID
+0x2c  char*             name
+0x30  char              castType         (0=spell, 1=ability, 2=blight, 3=disease, 4=curse, 5=power)
+0x32  u16               magickaCost
+0x34  SpellEffect[8]    effects
+0xf4  u16               flags

Enchantment:
+0x2e  u16               chargeCost
+0x30  u16               maxCharge
+0x34  SpellEffect[8]    effects
```

## NiPick (0x38) — ray-cast query object
```
+0x00  PickType          ePickType        FIND_FIRST / FIND_ALL
+0x04  SortType          eSortType        NO_SORT / SORT
+0x08  IntersectType     eIntersectType   INTERSECT_TRIANGLE / INTERSECT_ABV
+0x10  char              bFrontOnly
+0x11  char              bObserveAppCullFlag
+0x14  NiPointer         spRoot           — root node to test against
+0x18  TArray            pickResults      — array of NiPickRecord*
+0x34  char              bReturnTexture / bReturnNormal / bReturnSmoothNormal / bReturnColor
```

## NiPickRecord (0x38)
```
+0x00  NiPointer         spObject         — hit scene node
+0x08  vec3              intersect        — world-space hit point
+0x14  float             distance
+0x18  i16               usTriangleIndex
+0x1a  i16[3]            usVertexIndex
+0x28  vec3              normal
```

## RecordsHandler (0xb3ac) — key fields
```
+0x00  int               nextLoadIndex
+0x04  TES3File*         tes3FileSavegame
+0x0c  EntityList*       pEntityList_AllEntities
+0x14  ModelLoader*      modelLoader
+0x18  GameSetting**     pGMSTs
+0x1c  TList*            pRacesList ... (many lists for each record type)
+0x4c  CharSkill[27]     skills           — base skill data array
+0x5c8 MagicEffectData[143] magicEffects  — indexed by MagicEffect enum
+0xa73c RecordsHandler_DefaultAnimGroup[3] baseBeastAnimationGroups
+0xae60 StlListHead*     pStlList_TesFiles
+0xae64 TES3File*[256]   arrayMods
+0xb268 THashMap**       allObjectsById
+0xb375 char             bIsLoadingSaving
+0xb384 Reference*       refrPlayerSaveGame
```
