# Morrowind.exe Architecture Reference

**Binary:** `Morrowind.exe` (MD5: `4d19f7a36b0ccfcc3562f536da2bec26`, image base `0x400000`)  
**Compiler:** MSVC — all methods use `__thiscall` (ECX = `this`); calling convention matters for prototypes.

---

## Table of Contents
1. [Global Singletons](#global-singletons)
2. [Game Object Class Hierarchy](#game-object-class-hierarchy)
3. [Mobile Actor Hierarchy](#mobile-actor-hierarchy)
4. [Scene Graph Hierarchy (NetImmerse)](#scene-graph-hierarchy-netimmerse)
5. [Main Game Loop](#main-game-loop)
6. [AI System](#ai-system)
7. [Animation System](#animation-system)
8. [Reference Attachment System](#reference-attachment-system)
9. [Collision & Physics](#collision--physics)
10. [Data Collections](#data-collections)
11. [Rendering Pipeline](#rendering-pipeline)

---

## Global Singletons

| Symbol | Address | Type | Purpose |
|--------|---------|------|---------|
| `worldController` | `0x7c67dc` | `WorldController*` | Frame timing, cameras, subsystem pointers |
| `dataHandler` | `0x7c67e0` | `DataHandler*` | Cell management, collision grids, scene roots |

`TES3Game` is accessed via a static function / vtable, not a named global.

---

## Game Object Class Hierarchy

```
Entity (abstract, game-record base)
├── EntityPhysical
│   ├── ActorCommon                   — has Inventory, wornObjects, actorFlags
│   │   ├── NPCBase (0xf0)            — template: level, skills[8], attributes[27], race/class/faction
│   │   ├── NPCClone (0x78)           — instance: baseNPC*, disposition, aiPackageConfig
│   │   ├── CreatureBase
│   │   └── CreatureClone
│   ├── ContainerBase / ContainerClone
│   └── EntityDoor
├── ItemCommon
│   ├── EntityBook, EntityMisc, EntityLight
│   ├── Weapon, Armour, Clothing, Apparatus
│   ├── Ingredient, LockpickProbe, RepairTool
│   └── ItemBase
└── EntityAnimated, EntityActivator

Reference (0x50)                      — placed world instance of any Entity
  super:       Entity
  entity:      EntityUnion            — tagged union pointer to the Entity record
  orientation: vec3                   — Euler angles (radians)
  position:    vec3                   — world position
  attachments: RefrData_Base*         — linked list of typed attachment nodes
  sourceID:    int
  targetID:    int
```

---

## Mobile Actor Hierarchy

All mobile actors share a common layout. The `tag` field (char[4]) at offset `+0x4` in `MobileObject` identifies the concrete type.

```
MobileObject (0x80)           tag='MACT'/'MACH'/'MACR'/'MACP'
  vtbl:               InterfaceMACH*   (vtbl_MACT @ 0x74ab4c)
  movementFlags:      u16
  mobileFlags:        u32
  reference:          Reference*       — back-pointer to placed instance
  height, boundSize:  float, vec2
  velocity:           vec3
  impulseVelocity:    vec3
  collisionGroup:     NiCollisionGroup*

MACT (0x3b0) : MobileObject            — "Mobile ACTor", all actors
  listTargetActors:   TList            (+0x80)
  listFriendlyActors: TList            (+0x94)
  aiPlanner:          AIPlanner*       (+0xc8)
  actionData:         ActionData       (+0xcc)
  aiCombatSession:    CombatSession*   (+0x1c0)
  activeMagicEffects: StdList          (+0x1c4)
  collision_1D4:      MobileCollision  (+0x1d4)
  fight/flee/hello/alarm: int          (+0x350..0x35c)
  attributes:         Attribute[9]     (+0x254..0x2e4)  — strength/int/wil/agi/spd/end/per/lck/health/magicka/enc/fat
  effectAttributes:   int[24]          (+0x2f0)
  readiedWeapon:      EquipmentStack*  (+0x388)
  readiedAmmo:        EquipmentStack*  (+0x38c)
  readiedShield:      EquipmentStack*  (+0x390)
  animController:     AnimControllerUnion (+0x244)

MACH (0x56c) : MACT                    — "Mobile ACtor Human" (NPCs)
  skills[27]:         AttributeSkill   (+0x3b0)
  npcClone:           NPCClone*        (+0x560)

MACR (0x3d8) : MACT                    — "Mobile ACtor cReature"
  creatureClone:      CreatureClone*   (+0x3b0)
  skillCombat/Magic/Stealth: Attribute (+0x3b4/0x3c0/0x3cc)

MACP (0x694) : MACH                    — "Mobile ACtor Player"
  levelupPerAttributeCount[8]: int     (+0x56c)
  levelupPerSpecialization[3]: int     (+0x58c)
  bounty:             PlayerBounty*    (+0x598)
  flags (controls/jump/mouselook/etc.)  (+0x5b0)
  skillProgress[27]:  float            (+0x5f4)
  refr1stPerson:      Reference*       (+0x660)
  npc1stPerson:       NPCBase*         (+0x664)
  birthsign:          Birthsign*       (+0x670)
  markLocation:       MarkRecallPosition* (+0x678)
```

**Key accessors:**
- `WorldController::getMACP()` → `0x40ff20` — returns the player `MACP*`
- Cast chain: `MobileObject` → check `tag[0]=='M'&&tag[1]=='A'` then `tag[2]` for C/H/P

---

## Scene Graph Hierarchy (NetImmerse)

```
NiObject (0x8)
  vtbl: InterfaceNiObjectUnion*
  refCount: int                        — managed via addRef / release pattern

NiObjectNET : NiObject
  name: char*
  timeControllers: NiTimeController*   — linked list of keyframe controllers
  extraDataList: NiExtraData*

NiAVObject (0x90) : NiObjectNET
  flags: u16                           — AppCulled (bit 1), etc.
  parentNode: NiNode*
  worldBound: NiBound                  — sphere: centre(vec3) + radius(float)
  localTransform: LocalTransform       — translation(vec3) + rotation(matrix33) + scale(float)
  worldTransform: NiTransform
  pkVelocities: NiAVObject::Physics*   — swept physics data
  modelABV, worldABV: NiBoundingVolume*
  collideCallback: fn ptr
  propertyList: NiPropertyList

NiNode (0xb0) : NiAVObject
  kChildren: TArray                    — child scene nodes
  effectList: NiDynamicEffectList

Specializations of NiNode:
  NiSwitchNode, NiCamera, NiDynamicEffect, NiTextureEffect

NiTriBasedGeom : NiAVObject            — renderable geometry
  NiTriShape                           — standard triangle mesh
  NiTriShapeData                       — vertex/index buffer
```

**Scene root layout (TES3Game):**
```
TES3Game::worldRoot          (NiNode)   — entire scene
  worldObjectRoot                        — statics, activators, refs
  worldPickObjectRoot                    — physics-collision proxies
  worldLandRoot                          — terrain
  debugRoot                              — dev overlays
```

**DataHandler owns separate scene roots:**
```
sgWorldObjectRoot, sgWorldPickObjectRoot, sgWorldLandscapeRoot
sgSunlight (NiDirectionalLight)
sgFogProperty (NiFogProperty)
```

---

## Main Game Loop

```
WinMain
└── TES3Game::run()
    └── WorldController::mainLoopBeforeInput()   @ 0x40f610
        ├── updateDeltaTime()                    @ 0x453630  — timeGetTime(); caps at 200ms
        ├── patchReadKeyStateMapZoom()
        ├── updateMusic()                        @ 0x40f7c0
        ├── [if !menuMode] executeGlobalScripts()
        ├── [if !menuMode] DataHandler::testCellsCycle()   — streaming
        ├── [if !menuMode] DataHandler::simulate(cumTime)  @ 0x486ff0
        │   └── Cell::simulate()                 @ 0x4e3490
        │       ├── BSAnimationManager::Update() — cell animation
        │       └── BSAnimationManager::Update() — pick objects
        ├── BSAnimationManager::Update() — worldCamera
        ├── BSAnimationManager::Update() — armCamera (1st person)
        ├── SplashController::processRipplesBlood()
        ├── ActiveMagicManager::processAllMagics()
        └── WorldController::rechargerMagicItems()

[separately per actor, each frame — called from MobManager or equivalent driver]
ProcessManager::update()             @ 0x56f4d0
├── Lock plannerListCriticalSection
├── Iterate listAIPlanners → head-tracking pass per active actor:
│     Actor heads track: player (if not invisible/chameleon≥75)
│                        or combat target (if in combat and target visible)
│                        or nearest other actor (if not invisible)
├── Unlock
├── patchParticleAttachOnSimulateActor(player->aiPlanner)   [MWSE hook]
├── Lock plannerListCriticalSection
├── Iterate arrayPlanners[0..countPlanners]:
│     Skip if: !MobileActorFlags_ActiveInSimulation
│              or reference has RecordFlags_Disabled
│     → AIPlanner::simulateActor()   @ 0x564f60
│          → mobileActor->vtbl->simulate()
│          → MACT::simulate()        @ 0x524070
└── Unlock

calculateMovement()                  @ 0x56f250  — integrates velocity → position deltas
resolveCollisions()                  @ 0x56f440  — resolves accumulated collision results
```

**Delta time:** `worldController->deltaTime` (float, seconds, max 0.2). Cumulative animation time in `global_animationSyncCumulativeTime`.

**Skip conditions for actor simulation:** An actor is skipped if `MobileActorFlags_ActiveInSimulation` is unset, OR if its `Reference::recordFlags` has `RecordFlags_Disabled` set. Both checks run even when `ActiveInSimulation` is true — the disabled-ref check is an extra safety gate.

---

## AI System

```
AIPlanner (0x98)
  mobileActor:          MACT*
  aiPackages[32]:       AIPackage*      — fixed-size ring of packages
  nextOpenPackageIndex: int
  currentPackageIndex:  int
  deltaTime:            float

AIPackage (0x3c)   — base class
  vtbl:             InterfaceAIPackage* — virtual: run, isDone, testIfDone, etc.
  packageType:      AIPackageType
  targetActor:      MACT*              — actor being targeted (escort/follow target etc.)
  duration:         u16                — in game-hours
  distance:         uint
  targetPosition:   vec3
  done/started/reset/finalized: char
  owningActor:      MACT*
  destinationCell:  Cell*

  Subclasses: AIPackageWander, AIPackageTravel, AIPackageEscort,
              AIPackageFollow, AIPackageActivate

CombatSession (0x4c)                   — created by MACT::startCombat (0x530470)
  mobile:            MACT*             (+0x0c)
  selectedWeapon:    EquipmentStack*   (+0x10)
  selectedShield:    EquipmentStack*   (+0x18)
  nextSpell:         Spell*            (+0x20)
  nextAction:        CombatSessionNextAction (+0x28)
  spellPriority:     float             (+0x2c)
  alchemyPriority:   float             (+0x30)
  combatDelayTimer:  float             (+0x40)
  maybe_allowPotionUse: char           (+0x48)

CombatSession::determineNextAction()   @ 0x538f00
  — Runs three weightings:
    nextActionPhysicalWeighting()     @ 0x5375a0
    nextActionCastWeighting()         @ 0x538b80
    nextActionFleeWeighting()         @ 0x538d60
  — Picks highest priority action
  — Flee threshold: health ≤ 100 triggers flee weighting
```

**AIBehaviour state enum** (stored in `MACT::aiBehaviourState_saved`):
`Attack, Flee, Alarmed, Undecided, Hello, Greet, FaceDestination, CorpseFloatingUp, CorpseFalling, ScriptedCast`

---

## Magic System

```
ActiveMagicManager (0x38)            @ worldController->activeMagicManager (+0x70)
  sgWorldSpellRoot: NiNode*          — parent for all in-flight spell VFX
  flagNoProcess: char                — suspends processing when set
  mapSerialToMagicSourceInstance: StdMap  — keyed by uint serialNumber
  mapItemDataToSerial: StdMap
  mapReferenceToSerial: StdMap

Per-frame flow:
  ActiveMagicManager::processAllMagics(deltaTime)   @ 0x455290
    for each MagicSourceInstance in map:
      MagicSourceInstance::process(deltaTime)        @ 0x514380  (0x19d7 bytes)
    retire any instances whose state == MagicEffectState_Retired

MagicSourceInstance (0x12c)
  serialNumber: uint                 — globally unique ID; key in the map
  referenceTarget: Reference*        — target of the spell
  magicSourceCombo: MagicSourceCombo — which spell/enchant/ingredient is the source
  data: MagicSourceInstance::Data    (+0xb0) — effect-instance array
  magicProjectile: SpellProjectile*  — NULL for self/touch spells

MagicEffectInstance (0x38)           — one per active effect on a target
  target: Reference*
  magnitude: uint                    — final magnitude after resist roll
  timeActive: float
  state: MagicEffectState            — tracks progression; Retired triggers cleanup
  visualEffect: VFXInstance*
```

**Key entry points:**
- `ActiveMagicManager::activateMagic()` @ `0x454a60` — casts a spell onto a target
- `MagicSourceInstance::spellHit()` @ `0x516d90` — processes a projectile/touch hit
- `MagicSourceInstance::spellEffectEvent()` @ `0x518460` — applies individual effect magnitudes
- `ActiveMagicManager::retireSpellBySerial()` @ `0x455070` — removes a completed magic
- `MagicSourceInstance::retire()` @ `0x5127e0` — cleans up VFX, bound items, summoned creatures

---

## Script System

```
ScriptRecord (0x70)
  id: char[32]                       — script name (not null-terminated after 32 chars)
  shortVarCount/longVarCount/floatVarCount: int
  scriptDataSize: int                — bytecode length in bytes
  shortVarNames/longVarNames/floatVarNames: char**
  bytecode: u8*                      — compiled Morrowind script bytecode
  scriptLocalData: ScriptLocalData   — runtime variable state

Execution entry:
  ScriptRecord::execute(script, reference, arg3, arg4, arg5)   @ 0x5028a0
    — Called from: global-script loop (worldController->listActiveGlobalScripts)
                   mainLoop for player, NPC dialogue, triggers
    — Sets up execution context, then calls scriptOp() in a loop

  ScriptRecord::scriptOp(...)                                   @ 0x505770
    — 0x7ed8-byte opcode dispatch table; handles all ~200 Morrowind script opcodes
    — One call per bytecode instruction; loops until RETURN or END opcode
    — Reads parameters via ScriptRecord::getParams()            @ 0x500510

  ScriptRecord::evalExpression()                                @ 0x4ffcc0
    — Evaluates arithmetic/comparison expressions mid-script
    — Returns float result

Local variable access:
  getLocalShortValue()   @ 0x4ffb90
  getLocalLongValue()    @ 0x4ffc00
  getLocalFloatValue()   @ 0x4ffc70
```

**GlobalScript pattern:** `WorldController::listActiveGlobalScripts` is a `TList` of
`GlobalScript*` nodes. Each `GlobalScript` holds a `ScriptRecord*` + optional `Reference*`
target. Executed every frame in `mainLoopBeforeInput` when not in menu mode.

---

## Animation System

```
ActorAnimationData (0x7e4)            — attached to Reference via attachment node
  sgActorNode:     NiNode*            (+0x0)
  sgMRTNode:       NiNode*            (+0x4)   — Movement Root Translation node
  positionDeltaOfMRT: vec3            (+0x8)   — per-frame accumulated delta
  currentAnimGroups[3]: u8            (+0x38)  — lower/upper/special layer
  animGroups[150]: AnimationGroup*    (+0x68)
  manager:         NiKeyframeManager* (+0x2c0)
  approxRootMovementSpeed[150]: i16   (+0x3aa) — pre-baked speed (units/sec) per loop group
  movementSpeed:   float              (+0x4d8)

Root motion pipeline:
  calcRootMovement(animGroup)         @ 0x46fd80  — load-time: bakes approxRootMovementSpeed
  updateMovementRoot(timing, inout)   @ 0x470320  — per sub-step: accumulates positionDeltaOfMRT
  getAnimationDelta(out, worldBasis)  @ 0x470400  — per frame: rotates delta to world space
  ActorAnimController::getAnimationDelta @ 0x53eed0 — gated by useAnimationDelta flag

mergeAnimGroups()                     @ 0x4708d0  — called at load; triggers calcRootMovement
```

---

## Reference Attachment System

`Reference::attachments` is a singly-linked list of `RefrData_Base*` nodes. Each node carries a type tag. Access via typed accessor methods:

| Index | Accessor | Content |
|-------|---------|---------|
| 1 | `getAttachment1BodyParts()` | Body-part node mapping |
| 2 | `getAttachment2Light()` | Dynamic light effect |
| 3 | `getAttachment3Security()` | Lock/key state |
| 4 | `getAttachment4LeveledSource()` | Leveled list source |
| 5 | `getAttachment5LoadDoor()` | Door destination |
| 6 | `getAttachment6ItemData()` | Item condition/charge |
| Mobile | `getAnimationDataAttachment()` | `ActorAnimationData*` |

---

## Collision & Physics

```
DataHandler::array_listsCollisionGrid  (+0xc0)   — 2304 TList entries (48×48 grid)
  collisionCentreX/Y                   (+0xb4c0/b4c4)
MobileCollision                         — per-actor collision result record
MobManager                              @ WorldController::mobManager (+0x5c)
  processMobs()                        @ 0x563870 — each frame
  processProps()                       @ 0x563af0

NiTriBasedGeom SAT collision (tri-vs-tri CCD):
  FindCollisionsTriVsTri               @ 0x6f0f90  — dispatcher
  SAT_solveTriVsTri                    @ 0x7007e0  — full SAT loop (flags 16/128/256/512/1024/0x8000)
  SAT_testEdgeCrossAxis                @ 0x702d50  — edge×edge separating axis test
  SAT_testSlabInterval                 @ 0x701040  — swept slab TOI; result → *(this+560)
  sortThreeFloats_withIndices          @ 0x700e70  — sort 3 floats, record permutation
```

---

## Projectile System

```
ProjectileManager                      @ mobManager->projectileManager (+0x28)

Per-frame:
  ProjectileManager::update()          @ 0x575410
    → calculateMovement()              @ 0x575350  — integrates projectile velocity
    → resolveCollisions()              @ 0x5753a0  — handles hits

Types:
  addMarksmanProjectile()              @ 0x575100  — physical arrow/bolt/thrown
  addSpellProjectile()                 @ 0x5751b0  — spell bolt (links to MagicSourceInstance)
  disableProjectile()                  @ 0x575310  — retire without collision callback

MobileProjectile (stored in ActionData::nockedProjectile at MACT+0x1ac+0x34)
  — represents a nocked arrow before release
SpellProjectile (stored in MagicSourceInstance::magicProjectile)
  — links in-flight spell bolt to its source instance
```

---

## Weather System

```
WeatherController (0x1f0)              @ worldController->weather (+0x3c)
  — 97 fields, 70 methods
  — Manages sky rendering, precipitation, weather transitions, sun/moon/stars

Scene nodes owned:
  sgSunVis:           NiNode*          (+0x00) — sun visibility switch
  sgSunBase:          NiNode*          (+0x04) — sun disc geometry
  sgSunGlare:         NiNode*          (+0x08) — glare overlay
  sgSkyRoot:          NiNode*          (+0x4c) — root of entire sky scene
  sgSkyNight:         NiNode*          (+0x50) — night sky (stars)
  sgSkyAtmosphere:    NiNode*          (+0x54) — atmosphere dome
  sgSkyClouds:        NiNode*          (+0x58) — cloud layer
  sgRain:             NiNode*          (+0x5c)
  sgSnowRoot:         NiNode*          (+0x60)
  sgStormRoot:        NiNode*          (+0x6c) — ash/blight storm geometry
  sgAshCloud:         NiNode*          (+0x70)
  sgBlightCloud:      NiNode*          (+0x74)
  sgBlizzard:         NiNode*          (+0x78)
  sgTriAtmosphere:    NiTriShape*      (+0x7c) — atmosphere mesh vertices (colour-blended)
  sgTriCloudsCurrent: NiTriShape*      (+0x80) — current weather cloud mesh
  sgTriCloudsNext:    NiTriShape*      (+0x84) — next weather cloud mesh (for transitions)
  sgSkyLight:         NiLight*         (+0xac) — ambient sky light

Weather state:
  arrayWeathers[10]:  Weather*         (+0x14) — one record per weather type (index = type ID)
  currentWeather:     Weather*         (+0x3c)
  nextWeather:        Weather*         (+0x40)
  secundaMoonData:    Moon*            (+0x44)
  masserMoonData:     Moon*            (+0x48)

Colour state (updated per frame):
  currentSkyColor:    NiColor          (+0x90) — current interpolated sky colour (RGB floats)
  currentFogColor:    NiColor          (+0x9c) — current interpolated fog colour

Wind & precipitation:
  windVelocityCurrWeather: vec3        (+0xb8)
  windVelocityNextWeather: vec3        (+0xc4)
  precipitationGravity:    float       (+0x17c)
  snowGravityScale:        float       (+0x180)
  activeRainParticles:     int         (+0x13c)
  activeSnowParticles:     int         (+0x140)
  listActiveParticles:     TList       (+0x144) — active precipitation particle nodes
  listInactiveParticles:   TList       (+0x158) — pooled inactive particles

Weather transition timing:
  hoursBetweenWeatherChanges: float    (+0x16c)
  transitionScalar:    float           (+0x170) — 0.0 (current) → 1.0 (next), drives colour lerps
  hoursRemaining:      float           (+0x174) — hours until next weather change

Sun timing (all floats, in game hours):
  sunriseHour, sunsetHour              (+0xdc, +0xe0)
  sunriseDuration, sunsetDuration      (+0xe4, +0xe8)
  ambientPreSunriseTime, etc.          (+0xf0..+0xff) — ambient transition windows
  fogPreSunriseTime, etc.              (+0x100..+0x10f) — fog transition windows

Underwater:
  underwaterSunriseFog..NightFog: float (+0x1a0..+0x1ac)
  underwaterCol:       vec3            (+0x1b4)
  underwaterColWeight: float           (+0x1c0)

Other:
  pickSunglare:        NiPick*         (+0xa8) — ray test for sun occlusion
  region:              Region*         (+0x1d0) — current region (drives weather probability)
  dataHandler:         DataHandler*    (+0x1d4)
  soundUnderwater:     Sound*          (+0x1d8)
  isSunOccluded:       char            (+0x1ec)
```

**Key entry points:**
- `WeatherController::updateTick()` @ `0x440c80` — main per-frame update (0x935 bytes): drives colour lerps, particle update, moon/star visibility, sun damage, glare tests
- `WeatherController::updateColours()` @ `0x43e000` — interpolates all sky/fog/ambient/sun colours based on `transitionScalar` and time-of-day phase
- `WeatherController::transition()` @ `0x441a10` — initiates a weather transition: sets `nextWeather`, resets `transitionScalar`
- `WeatherController::switch()` @ `0x441c40` — completes a transition: promotes `nextWeather` → `currentWeather`
- `WeatherController::checkWeatherCycle()` @ `0x441990` — called per frame; fires `switch()` when `hoursRemaining` reaches zero
- `WeatherController::calcSunDamage()` @ `0x440630` — applies sun damage to player in exterior cells during the day
- `WeatherController::isStormy()` @ `0x452dc0` — returns true if current weather type is Ash/Blight/Snow storm
- `WeatherController::updateParticles()` @ `0x452ae0` — manages pool of active/inactive precipitation particles
- `WeatherController::syncSkyDomePosition()` @ `0x43b8e0` — keeps sky dome centred on camera each frame
- `WeatherController::sunGlareTests()` @ `0x440250` — fires NiPick rays to determine if sun is occluded

**Lerp methods (pattern — one per colour channel × time-of-day phase):**
- `lerpAmbientSunrise/Day/Sunset/Night` @ `0x442610..0x4429d0`
- `lerpSunSunrise/Day/Sunset/Night` @ `0x442b40..0x4430c0`
- `lerpSkySunrise/Day/Sunset/Night` @ `0x443350..0x4438b0`
- `lerpFogSunrise/Day/Sunset/Night` @ `0x4438b0..0x443fb0`
All follow the same pattern: LERP between `currentWeather->colourX` and `nextWeather->colourX` using `transitionScalar`, then blend result by time-of-day fraction.

---

## Audio System

```
AudioController                        @ worldController->audio (+0x34)
  — Wraps DirectSound8 (DSOUND.DLL)
  — All methods map 1:1 to DirectSound buffer operations

Key methods:
  LoadSoundFile()    @ 0x401db0   — loads WAV into a DirectSoundBuffer
  PlaySoundBuffer()  @ 0x402820   — plays with volume/3D positioning
  StopSoundBuffer()  @ 0x402980
  SetSoundBufferVolume()     @ 0x4029f0
  SetSoundBufferPosition()   @ 0x402b50  — 3D positional audio
  SetSoundBufferVelocity()   @ 0x402e50  — Doppler source velocity

Sound* entity record links to an AudioController-managed buffer.
DataHandler::soundEvents / tempSoundEvents / lightSoundEvents (at +0xb4d0)
  — TList* of pending sound events processed each frame
```

---

## Data Collections

| Type | Description |
|------|-------------|
| `TList` | Doubly-linked list with `head/count`; iterate via `TList::iterateBegin/iterateNext` |
| `TArray` | Flat array with `data*/count/capacity` |
| `THashMap` | Hash map; typed variants `THashMapPowers`, `THashMapMagicEffectInstance` |
| `StdList_*` | STL-compatible list variants |
| `EntityList` | Specialized list of `Reference*` (used for cell actor/ref lists) |
| `NiPropertyList` | Linked list of `NiProperty*` on scene nodes |

**Cell layout:**
```
Cell (0x94)
  name:              char*
  cellFlags:         u8           — interior/exterior, water, etc.
  data:              VariantData  — union: InteriorData (ambient/fog) or ExteriorData (gridX/Y)
  refrlistActors:    EntityList   (+0x30)
  refrlistPersistentRefs: EntityList (+0x40)
  refrlistTemporaryRefs: EntityList  (+0x58)
  animationManager:  BSAnimationManager*
  pickObjectsRoot:   BSAnimationManager*
  pathgrid:          PathGrid*
  waterLevel_or_region: Region*
```

---

## Rendering Pipeline

### Frame Render Flow

Each frame, `TES3Game::renderNextFrame` drives the following sequence:

```
TES3Game::renderNextFrame
  │
  ├─ ShadowManager::prepareShadows       @ 0x435110
  │    — builds per-light shadow geometry before main scene draw
  │
  ├─ WaterController::animate            @ 0x51C880
  │    — advances ripple/rain simulation; updates wave UV offsets
  │    — calls invalidateSkyReflection() when sky changes
  │
  ├─ NiCamera::Click (worldCamera)
  │    — culls and renders main scene (exterior/interior)
  │    — per visible object:
  │        NiDX8TexturePass::resetForNewFrame  @ 0x6b2fa0
  │        NiDX8Renderer::RenderShape
  │        → NiDX8Renderer::DrawPrimitive / DrawSkinnedPrimitive
  │
  ├─ WaterController::renderWater
  │    — rendered after main scene; water is alpha-blended over geometry
  │
  ├─ NiCamera::Click (armCamera)
  │    — 1st-person arms pass; draws player weapon/hands on top of scene
  │
  └─ NiCamera::Click (menuCamera)
       — UI/HUD pass
```

### Cameras

| Name | Purpose |
|------|---------|
| `worldCamera` | Main scene — exterior terrain, interiors, all actors |
| `armCamera` | 1st-person arms overlay; drawn after worldCamera so arms clip nothing |
| `menuCamera` | 2D HUD and menu geometry |

All three are `NiCamera*` owned by `WorldController`.

---

### Shadow System (ShadowManager)

`ShadowManager` is a singleton subsystem attached to `WorldController`.

```
ShadowManager fields:
  list_4:              TList          — one ShadowListEntry per active shadow-casting light
  sgShadowOverlay:     NiNode*        — overlay quad composited over scene after draw
  sgSceneObjects:      NiNode*        — shadow-receiver geometry copied here for projection
  frameUpdateInterval: int            — throttle: shadows rebuilt only every N frames
```

`prepareShadows()` @ `0x435110`:
- Iterates `list_4` to find lights that cast shadows.
- For each light, projects receiver geometry into the light's clip space.
- Composites result via `sgShadowOverlay` (a full-screen quad rendered after main pass).
- Rebuilds are throttled by `frameUpdateInterval` — shadow maps are not re-rendered every frame.

Key functions:

| Address | Name | Purpose |
|---------|------|---------|
| `0x435020` | `ShadowManager::findEntryByNode` | Looks up a `ShadowListEntry` by scene node |
| `0x4350A0` | `ShadowManager::update` | Per-frame driver: calls `prepareShadows` if interval elapsed |
| `0x435110` | `ShadowManager::prepareShadows` | Main shadow projection pass |
| `0x4357A0` | `ShadowManager::createShadowOverlayQuad` | Builds the fullscreen composite quad |
| `0x4360F0` | `ShadowManager::setEnabled` | Enables/disables the system |
| `0x473910` | `ShadowManager::getSgPointer58_addRef` | NiPointer getter — reads `this+0x58`, AddRefs |
| `0x473C60` | `ShadowManager::getSgPointer5C_addRef` | NiPointer getter — reads `this+0x5C`, AddRefs |

---

### Water Rendering (WaterController)

Water has two render paths selected at startup by hardware capability:

#### Pixel Shader Path (D3D PS 1.x + VS 1.x + 4 textures + 8 blend stages)
- Surface geometry: `TES3WaterNVLake` — a single high-res mesh.
- Sky reflection: `nvSkyRenderToTexture` (RTT) renders the sky dome into a texture each frame, then samples it on the water surface. Reflection is invalidated (and RTT re-rendered) whenever the sky changes via `invalidateSkyReflection()` @ `0x51E150`.
- Above/below water detection: water plane equation tests camera position to switch from refraction to underwater fog mode.

#### Non-Pixel-Shader Path (fallback)
- Surface geometry: tiled `NiTriShape` grid created by `createSurfaceGeometry()` @ `0x51E240`.
- Animation: `NiFlipController` cycles through a pre-baked texture sequence to fake wave movement.
- No real-time sky reflection; uses a static environment map.

#### Hardware capability check
`WaterController::getSupportsPixelShader` queries `D3DCAPS8` (obtained from the device):

| D3DCAPS8 field | Offset | Requirement |
|----------------|--------|-------------|
| VertexShaderVersion | `+0xC4` | ≥ 1.x |
| MaxSimultaneousTextures | `+0x98` | ≥ 4 |
| PixelShaderVersion | `+0xCC` | ≥ 1.x |
| MaxTextureBlendStages | `+0x9C` | ≥ 8 |

All four must pass for the pixel shader path to activate.

Key functions:

| Address | Name | Purpose |
|---------|------|---------|
| `0x51C880` | `WaterController::animate` | Per-frame: advances ripple/rain simulation, updates UV offsets |
| `0x51DAB0` | `WaterController::setRainFrequency` | Sets ripple emission rate for current rain intensity |
| `0x51E150` | `WaterController::invalidateSkyReflection` | Marks sky RTT dirty; triggers re-render next frame |
| `0x51E240` | `WaterController::createSurfaceGeometry` | Builds tiled NiTriShape grid (non-PS path) |
| `0x51E860` | `WaterController::updateRippleParams` | Updates ripple amplitude/frequency uniforms |

---

### Texture Multi-Pass System (NiDX8TexturePass)

Each renderable object owns an `NiDX8TexturePass` that manages texture-stage assignment across multiple D3D8 render sub-passes.

```
NiDX8TexturePass fields:
  TArray_18:   TArray of NiDX8TexturePass::$1*   — pool of sub-pass descriptors
  field_C:     int                                — current active sub-pass index
  count_10:    int                                — total sub-pass capacity
  stage banks: 5 banks (4 banks of 8 stages + field_624)
                                                  — texture stage state for all sub-passes
```

**Sub-pass overflow**: when a new sub-pass is needed and `count_10` is exhausted, `acquireSubpassWithCapacity()` grows `TArray_18` by 2 slots (not doubled — growth is +2 each time).

**Per-object reset** (`resetForNewFrame` @ `0x6b2fa0`):
- Called before every `RenderShape` call.
- Resets all 5 stage banks to default state.
- Releases all renderer references held by prior sub-passes.
- Reinstalls 5 per-pass callbacks in the render queue.
- Cost: proportional to the number of active sub-passes — objects with many textures are more expensive to reset.

Key functions:

| Address | Name | Purpose |
|---------|------|---------|
| `0x6b2fa0` | `NiDX8TexturePass::resetForNewFrame` | Per-object reset before RenderShape |
| `0x6b38a0` | `NiDX8TexturePass::initRenderer` | Binds the D3D device, sets up stage state |
| `0x6b3ab0` | `NiDX8TexturePass::setStageObject` | Assigns a texture to a specific stage slot |
| `0x6b3b30` | `NiDX8TexturePass::acquireSubpassWithCapacity` | Allocates a new sub-pass (grows pool by 2 on overflow) |
| `0x6b3d70` | `NiDX8TexturePass::initOrCheckSubpass` | Initialises a sub-pass or validates its capacity |
| `0x6b3ee0` | `NiDX8TexturePass::acquireSubpassForPixelShader` | Allocates a sub-pass reserved for PS use |

---

### Device / Adapter Setup (NiDX8)

These functions run at startup during D3D device enumeration and resolution selection:

| Address | Name | Purpose |
|---------|------|---------|
| `0x6b1940` | `NiDX8DeviceDesc::ScreenFormatInfo::findClosestDepthStencilFormat` | Picks the best depth/stencil format for a back-buffer format |
| `0x6b1ec0` | `NiDX8DeviceDesc::getScreenFormatEnum` | Maps a D3DFORMAT value to the engine's internal format enum |
| `0x6b21d0` | `NiDX8DeviceDesc::findClosestDepthStencilForScreenFormat` | Selects depth/stencil format for a given screen format |
| `0x6b25d0` | `NiDX8AdapterDesc::findClosestMode` | Finds the closest supported display mode (resolution + refresh) to requested |
| `0x4f62b0` | `NiDX8SelectResolutionTES_static::populateAdapterComboBox` | Fills adapter combo-box in display settings dialog |
| `0x4f6370` | `NiDX8SelectResolutionTES_static::populateResolutionComboBox` | Fills resolution combo-box for selected adapter |
| `0x4f64e0` | `NiDX8SelectResolutionTES_static::collectResolutionStrings` | Builds list of display mode strings for the resolution combo |

---

### Known Optimization Opportunities

1. **No render-state sorting** — objects are submitted to `DrawPrimitive` in scene-graph order. Batching by material/shader would eliminate thousands of redundant D3D state changes per frame.
2. **Per-object `resetForNewFrame` teardown** — releases and reinstalls stage callbacks for every object before draw. A dirty-state approach (only flush changed stages) would eliminate most of this cost.
3. **`TArray` growth-by-2 for multi-pass** — growing a sub-pass pool by 2 slots at a time causes repeated reallocation for objects with many texture passes. Starting capacity or growth factor should be larger.
4. **Unthrottled sky RTT** — `invalidateSkyReflection` is called whenever sky state changes. For fast transitions (storm, teleport), this can fire multiple times per frame. One dirty flag per frame is sufficient.
5. **Shadow hysteresis** — shadows are rebuilt every `frameUpdateInterval` frames unconditionally. A change-detection test (camera moved? lights moved?) could skip rebuilds entirely for static scenes.
6. **Skinned VB repacking** — `DrawSkinnedPrimitive` repacks vertex buffers per draw call. Caching the last-used bone matrix set and skipping repacks when unchanged would help animated crowds.
