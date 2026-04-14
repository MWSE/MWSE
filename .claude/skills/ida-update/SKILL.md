---
name: ida-update
description: >
  Reverse engineer Morrowind.exe functions and correctly update the IDA Pro database via
  ida-pro-mcp. Use this skill whenever the user asks to: investigate a sub_ function,
  identify an unknown function, rename functions or variables, add comments to IDA,
  trace call graphs, understand what a function does, decompile engine code, update the
  IDA database, or explore any part of the Morrowind.exe codebase. Also use it when asked
  to "look into", "figure out", "identify", or "trace" any address or function name.
---

# IDA Database Update Skill — Morrowind.exe

You are reverse engineering **Morrowind.exe** (MD5: `4d19f7a36b0ccfcc3562f536da2bec26`,
image base `0x400000`) using **IDA Pro via MCP tools**. This skill gives you the
architecture knowledge and workflow to do that correctly and efficiently.

## Before You Start — Load Context

Read the reference files when needed. They contain hard-won knowledge that will save
you from re-deriving things the hard way:

- [references/architecture.md](references/architecture.md) — full class hierarchy,
  main game loop, per-actor simulation, AI, magic, script, animation, projectile,
  audio, weather/sky, collision systems, global singletons
- [references/structs.md](references/structs.md) — field offsets for all major structs:
  MACT/MACH/MACR/MACP, AIPlanner, AIPackage subclasses, CombatSession, ActionData,
  Attribute, ActorAnimController, ActorAnimationData, AnimationGroup, BSAnimationManager,
  NiKeyframeManager, Reference, NiObject/NiAVObject/NiNode, NiPick/NiPickRecord,
  NiProperty subtypes, WorldController, DataHandler, RecordsHandler, MobManager,
  ProcessManager, MagicSourceInstance, MagicEffectInstance, MagicEffectData,
  ActiveMagicManager, ScriptRecord, TList/TArray/THashMap, Inventory, ItemStack,
  EquipmentStack, ItemData, Spell, Enchantment, MobileCollision
- [references/naming-conventions.md](references/naming-conventions.md) — naming rules,
  comment style, common patterns: `patchXxx` MWSE hooks, simulation gate,
  invisibility/chameleon check, effectAttributes indexing, StdMap red-black tree,
  RecordsHandler record lists, Attribute vs effectAttribute, AnimStateAttack guard

Read a reference file when you are about to work in that area. Don't load all three
upfront — only load what the current task touches.

---

## Core Workflow

### 1. Orient Before Decompiling

Before touching the decompiler, gather context:

```
get_callers(address)     → who calls this? gives you the domain immediately
get_callees(address)     → what does it call? gives you its dependencies
get_xrefs_to(address)    → data refs (vtable slots, atexit, .CRT$XCU table)
```

If `get_callers` returns nothing: check `get_xrefs_to`. No code callers usually means
the function is a callback stored in a table (vtable slot, `.CRT$XCU` initializer,
`atexit` registration) — trace the data reference to identify context.

### 2. Decompile Sequentially — Never in Parallel

**Critical:** IDA crashes if you send concurrent decompile requests.
Always decompile one function at a time, wait for the result, then decompile the next.

```
decompile_function(address)   → returns pseudocode
```

Read the pseudocode carefully. Look for:
- The `this` argument type (first param in `__thiscall`) — identifies the class
- Struct field accesses at known offsets (cross-reference against `structs.md`)
- Calls to named functions — gives semantic context
- Return type and value — `bool`, pointer, void?

### 3. Identify the Function

Match what you see against known patterns in `naming-conventions.md`. Ask:

- Does the function manipulate a specific struct? → it belongs to that class
- Does it match a known pattern (ctor/dtor, refcount assign, TList iteration,
  SAT axis test, atexit destructor, `.CRT$XCU` initializer)?
- What does the caller expect from the return value?

Trace 2–3 levels of callees only when the immediate function's context is insufficient.
Stop tracing when you reach already-named functions or the pattern is clear.

### 4. Apply Updates to IDA

Always apply in this order:

```
1. rename_function(address, "ClassName::methodName")
2. set_function_prototype(address, "__thiscall sig")   [if meaningful]
3. rename_local_variable(address, "vN", "descriptiveName")  [for key vars]
4. set_comment(address, "explanation")                  [at entry point]
5. add_comment(address+offset, "inline note")           [at key lines]
6. rename_global_variable(address, "vtbl_ClassName")    [if it's a vtable/global]
```

When a `rename_local_variable` call fails (compiler-merged register variable):
- Do not retry with the same name
- Add an inline comment at the usage site instead

### 5. Update Reference Files (when significant new knowledge is gained)

If you discover a previously unknown struct layout, class relationship, or global
singleton, update the appropriate reference file so future sessions benefit.
Do this **after** all IDA updates are applied, not before.

---

## Key Architecture Facts (Quick Reference)

### Global Singletons
| Symbol | Address | Type |
|--------|---------|------|
| `worldController` | `0x7c67dc` | `WorldController*` |
| `dataHandler` | `0x7c67e0` | `DataHandler*` |

### Mobile Actor Tag → Type
The `char[4] tag` at `MobileObject+0x04` identifies the concrete type:
- `'MACT'` → `MACT` (base actor, any NPC/creature)
- `'MACH'` → `MACH` (human/NPC with skills)
- `'MACR'` → `MACR` (creature)
- `'MACP'` → `MACP` (player)

Cast from `MobileObject*` to `MACT*` — MACT's first field **is** a MobileObject inline.

### Key Field Shortcuts
- Actor's `Reference*` → `MobileObject::reference` (`+0x14`)
- Actor's `AIPlanner*` → `MACT::aiPlanner` (`+0xc8`)
- Actor's combat state → `MACT::aiCombatSession` (`+0x1c0`)
- NPC's base record → `MACH::npcClone->baseNPC` (`+0x560 → +0x6c`)
- Player → `WorldController::getMACP()` @ `0x40ff20`
- MRT animation node → `ActorAnimationData::sgMRTNode` (`+0x04`)

### vtable Pattern
Every class's vtable is a global named `vtbl_ClassName`. The vtable pointer is always
at offset `+0x00`. Virtual dispatch looks like:
```c
actor->vtbl->methodName(actor, arg1, arg2)
```

### NetImmerse Refcount Pattern
Any time you see this sequence, the field is a `NiPointer<T>`:
```c
if (old) { if (--old->refCount == 0) old->vtbl->deleting_dtor(old, 1); }
new->refCount++;
field = new;
```

### SEH/Thunk Functions — Leave Alone
Functions named `*_SEH`, `j_*`, `j_j_*` are auto-generated. Do not rename them.
They appear in callee lists but are just wrappers around the real function.

---

## Naming Rules (Summary)

```
ClassName::methodName           ← standard method
game_functionName               ← free utility function
ScriptHelpers::ClassName::m     ← script-facing wrapper
vtbl_ClassName                  ← vtable global
global_ClassName_fieldName      ← class-associated global

Constructors:    ClassName::ctor / ClassName::ctor_args
Destructors:     ClassName::dtor / ClassName::deleting_dtor
```

Full conventions including comment style → [references/naming-conventions.md](references/naming-conventions.md)

---

## Common Patterns to Recognise Quickly

### `.CRT$XCU` Static Initializer (no callers, data xref from init table)
```
sub_XXXXXX()   ← stored in .CRT$XCU — static initializer before main()
  ctor(&global)
  atexit(sub_YYYYYY)

sub_YYYYYY()   ← registered via atexit — destructor at process exit
  if (global) { --global->refCount == 0 ? deleting_dtor(1) : ...; global = null; }
```
Name as: `ClassName_static_init_fieldName` / `ClassName_atexit_release_fieldName`

### SAT Edge-Cross-Axis Test
Cross product of two edge vectors → dot-project vertices → sort 3 floats → slab interval.
Belongs to `NiTriBasedGeom::FindCollisionsTriVsTri` @ `0x6f0f90`.
See already-named: `SAT_testEdgeCrossAxis` @ `0x702d50`, `SAT_solveTriVsTri` @ `0x7007e0`.

### AI Package Virtual Dispatch
```c
package->vtbl->run(package, owningActor, deltaTime)
package->vtbl->testIfDone(package)
```
Package types: `AIPackageWander`, `AIPackageTravel`, `AIPackageEscort`,
`AIPackageFollow`, `AIPackageActivate`.

### TList Iteration
```c
for (n = TList::iterateBegin(list); n; n = TList::iterateNext(list))
    process(n->data.asXxx);
```

### 3×3 Exterior Cell Loop
```c
ExteriorData **v = dataHandler->arrayExteriorCellData[0];
for (int i = 9; i--; v++)
    if ((*v)->loadingFlags == 1) { cell = (*v)->cell; ... }
```

---

## What We've Already Reverse Engineered

Read these MD files in the repo root for detailed pseudocode of finished work:

| File | Functions Documented |
|------|---------------------|
| `animRootMovement.md` | `calcRootMovement`, `updateMovementRoot`, `getAnimationDelta`, `mergeAnimGroups` |
| `combatAI.md` | `MACT::startCombat`, `aiActorCombat`, `CombatSession::ctor`, `determineNextAction`, three weighting functions |
| `getLineOfSight.md` | `game_getLineOfSight`, `rayVsCellNode`, `rayVsEntity` |
| `sub_6E7610.md` | NiZBufferProperty static init / atexit chain |

**Already-renamed functions of note:**

*Collision / geometry (NiTriBasedGeom, NiBoundingVolume):*
- `NiTriBasedGeom::SAT_testEdgeCrossAxis` @ `0x702d50`
- `NiTriBasedGeom::SAT_solveTriVsTri` @ `0x7007e0`
- `NiTriBasedGeom::SAT_testSlabInterval` @ `0x701040`
- `sortThreeFloats_withIndices` @ `0x700e70`
- `NiBoundingVolume::closestPointOnTetrahedron` @ `0x73d0c0` — GJK tetrahedron barycentric solver
- `NiBoundingVolume::closestPointOnTetrahedronFaces` @ `0x73d460` — GJK face-loop dispatcher
- `NiBoundingVolume::closestPointOnTriangle` @ `0x73db90` — Eberly 7-region Voronoi triangle solver
- `NiBoundingVolume::closestPointsBetweenTriangles` @ `0x73e190` — GJK triangle-triangle distance kernel (39 edge-pair + 15 face tests)

*D3D8 renderer (NiDX8 / NiD3D):*

- `NiDX8RenderState::initFromDeviceCaps` @ `0x6b8930`
- `NiDX8ConfigurableTexturePipeline::initAlphaStateTemplates` @ `0x6b90e0`
- `NiDX8ConfigurableTexturePipeline::initFilteringAndAddressCaps` @ `0x6ba840`
- `NiDX8ConfigurableTexturePipeline::initPipelineState` @ `0x6b9ea0`
- `NiDX8ConfigurableTexturePipeline::buildPass` / `loadBinary` / `selectAndLinkPass` / `buildSkinPartitionPass` / `applyPixelShader` / `applyVertexShader` / `ensureAndRegisterStreamables`
- `NiDX8TexturePass::applyPixelShader` / `applyVertexShader`
- `NiDX8TexturePass::_1::setStageAtIndex` / `appendStage` / `appendStagePair` / `getAvailableSlotCounts` / `setNiObjectField48`
- `NiDX8TextureManager::clearHashMap` @ `0x6bc8a0`
- `NiDX8IndexBufferManager::getOrBuildQuadIndexBuffer` @ `0x6bcfe0`
- `NiDX8IndexBufferManager::buildLineSegmentIndexBuffer` @ `0x6bd190`
- `NiD3DShaderInterface::buildVertexBuffer` @ `0x6bf0b0`

*NIF streaming / image / animation:*

- `NiStream::clearObjects` / `registerStreamable` / `readRootObjects` / `save` / `saveToFile` / `setLoadHashMap` / `cacheObjectByFilename` / `getCachedObjectByFilename`
- `NiDevImageConverter::convertPixelDataWithMipmaps` / `mapTypeToPixelFormat` / `mapTypeToCompressedPixelFormat` / `convertToBumpMap`
- `NiPixelFormat::getChannelFieldWord0` / `getChannelFieldWord1`
- `NiTextureCodec::decompressDXTtoRGBA32` @ `0x706840` — DXT1/3/5 block decompressor
- `BSAnimationNode_static::resetAnimPhasesRecursive` @ `0x6f2290`

*Math / spline:*

- `matrix33::eigenDecompose` @ `0x6e87f0` — Householder + QL 3×3 symmetric eigendecomposition
- `matrix33::householderReduce` @ `0x6e88c0`
- `NiQuaternion_static::computeSquadControlPoint` @ `0x6fb900` — Squad spline control point
- `TArray::resize` @ `0x6bf720`

*Actor simulation (full method sets):*

- Full `MACT::*` method set (`simulate`, `onDeath`, `aiMovementFacing`, etc.)
- Full `ActorAnimationData::*` method set
- Full `CombatSession::*` method set
- Full `AIPlanner::*` method set
- Full `WorldController::*` and `DataHandler::*` method sets

*Rendering pipeline — NiDX8Renderer:*

- `NiDX8Renderer::validateDeviceCaps` @ `0x6ab960` — checks required D3D8 caps at device creation
- `NiDX8Renderer::createDefaultTextures` @ `0x6aba70` — creates fallback white/black textures
- `NiDX8Renderer::packGeometryBuffers` @ `0x6af610` — packs VB/IB with revision-ID reuse check
- `NiDX8Renderer::precacheGeometryData` @ `0x6afd90` — hash-map geometry cache lookup/insert
- `NiDX8Renderer::probeTextureFormats` @ `0x6b05e0` — probes supported D3D texture formats
- `NiDX8TexturePass::resetForNewFrame` @ `0x6b2fa0` — full per-frame reset of all stage banks, pass counters, renderer refs, and callback table
- `NiDX8TexturePass::initRenderer` @ `0x6b38a0` — binds renderer, creates 2×1 fallback texture, initialises 8 NiDX8TextureStage entries
- `NiDX8TexturePass::setStageObject` @ `0x6b3ab0` — sets/clears a `_1` entry in TArray_18, tracks active-stage count
- `NiDX8TexturePass::acquireSubpassWithCapacity` @ `0x6b3b30` — returns current `_1` subpass, advances to next if texture/sampler slots insufficient
- `NiDX8TexturePass::initOrCheckSubpass` @ `0x6b3d70` — creates `_1` subpass if absent; returns true if incompatible (shader-type mismatch or slot shortage)
- `NiDX8TexturePass::acquireSubpassForPixelShader` @ `0x6b3ee0` — like acquireSubpassWithCapacity but advances when pixel shader handle already assigned
- `NiDX8DeviceDesc::ScreenFormatInfo::findClosestDepthStencilFormat` @ `0x6b1940` — picks best-match depth/stencil D3DFORMAT from validDepthStencils hash map
- `NiDX8DeviceDesc::getScreenFormatEnum` @ `0x6b1ec0` — translates D3DFORMAT to NI internal screen format enum (1–11)
- `NiDX8DeviceDesc::findClosestDepthStencilForScreenFormat` @ `0x6b21d0` — finds ScreenFormatInfo by format, normalises depth/stencil bits, delegates to above
- `NiDX8AdapterDesc::findClosestMode` @ `0x6b25d0` — finds ModeDesc by format/width/height, picks closest refresh rate
- `NiDX8SelectResolutionTES_static::populateAdapterComboBox` @ `0x4f62b0` — fills adapter combo box in resolution dialog
- `NiDX8SelectResolutionTES_static::populateResolutionComboBox` @ `0x4f6370` — fills resolution combo box, selects current setting
- `NiDX8SelectResolutionTES_static::collectResolutionStrings` @ `0x4f64e0` — collects unique "W x H x BPP" strings from an adapter's mode list

*Rendering pipeline — WorldControllerRender\*:*

- `WorldControllerRenderCameraData::updateFrustumForViewport` @ `0x404a30`
- `WorldControllerRenderTarget::releaseResources` @ `0x42d410`
- `WorldControllerRenderCamera::setupOffscreenCamera` @ `0x42eb30`
- `WorldControllerRenderTarget::createSmallRenderedTexture` @ `0x42f9d0`
- `WorldControllerRenderTarget::renderFogOfWar` @ `0x42fc60` — 16×16 bitmask fog-of-war render
- `WorldControllerRenderTarget::setupAlphaOverrideRecursive` @ `0x430220` — injects forced-alpha material for portrait rendering
- `WorldControllerRenderTarget::restoreAlphaOverrideRecursive` @ `0x430640`
- `WorldControllerRenderCamera::moveCameraRootToEnd` @ `0x433610`

*Rendering pipeline — ShadowManager:*

- `ShadowManager::registerLight` @ `0x434db0` — creates NiNode shadow volume and ShadowListEntry for a light
- `ShadowManager::unregisterLight` @ `0x434f60` — removes one or all ShadowListEntry items
- `ShadowManager::broadcastNodeToShadowEntries` @ `0x434fe0` — iterates list_4, calls sub on each entry
- `ShadowManager::findEntryByNode` @ `0x435020` — lookup ShadowListEntry by NiNode* in list_4
- `ShadowManager::update` @ `0x4350a0` — clears processed flags, calls addObjectsAndLights
- `ShadowManager::prepareShadows` @ `0x435110` — per-frame pre-pass: conditional update + attach active shadow nodes to sgSceneObjects
- `ShadowManager::createShadowOverlayQuad` @ `0x4357a0` — builds per-light shadow overlay NiTriShape quad
- `ShadowManager::setEnabled` @ `0x4360f0` — enables/disables shadows, propagates flag to all entries

*Rendering pipeline — WaterController:*

- `WaterController::renderWater` @ `0x51c550` — two-path water render: TES3WaterNVLake pixel shader path (above/below detection, NiCamera::Click) vs. flat textured quad path
- `WaterController::animate` @ `0x51c880` — per-frame water animation: NVLake update or ripple/flipController advance
- `WaterController::setRainFrequency` @ `0x51dab0` — sets rain ripple intensity on NVLake (range 0.1–1.0)
- `WaterController::getSupportsPixelShader` @ `0x51db90` — queries D3DCAPS8: requires VS 1.x+, PS 1.x+, 4+ textures, 8+ blend stages
- `WaterController::invalidateSkyReflection` @ `0x51e150` — marks sky reflection texture dirty for next frame
- `WaterController::createSurfaceGeometry` @ `0x51e240` — builds water geometry: NVLake path or surfaceTileCount² NiTriShape grid with NiFlipController animation
- `WaterController::updateRippleParams` @ `0x51e860` — sets scale/speed on a ripple, scales NiKeyframeController data

**The class-prefixed `::sub_XXXXXX` survey is complete** — all functions with a known class namespace but unnamed body have been renamed. Future sessions start with a clean slate for any new `sub_` patterns.

---

## Output Format

After completing an investigation, report:

1. **What the function does** — one paragraph in plain English
2. **How it fits** — what system it belongs to, when it's called
3. **Updates applied** — bulleted list of all renames/comments/prototypes set in IDA
4. **Remaining unknowns** — any sub_ calls that weren't traced and why

If the user asks to export findings to a markdown file, create it in
`c:\Users\ivan_\source\repos\MWSE\<topic>.md` following the style of the existing
`animRootMovement.md` / `combatAI.md` files: header table of functions/addresses,
then sections per function with readable pseudocode blocks and a Key Observations table.
