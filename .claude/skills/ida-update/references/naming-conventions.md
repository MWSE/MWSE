# Morrowind.exe — IDA Naming Conventions

## Function Naming

### Standard class methods
```
ClassName::methodName
ClassName::sub_ClassName_XXXXXX    ← use when purpose is unclear but class is known
```

### Free functions (no class)
```
game_functionName           ← utility/helper not belonging to a class
```

### Script-facing wrappers
```
ScriptHelpers::ClassName::methodName   ← thin wrappers that expose engine internals to scripts
```

### Aliased / compiler-merged methods
```
aliased_method::ClassName::methodName  ← when MSVC merged two methods into one body
```

### SEH wrappers — DO NOT RENAME
```
ClassName::methodName_SEH   ← auto-generated structured exception handler stubs
j_ClassName::methodName     ← jump thunks (also auto-generated)
j_j_ClassName::methodName   ← double-jump thunks
```
These appear in the function list alongside their targets. Their addresses are valid but their names must not change.

### Constructors / destructors
```
ClassName::ctor             ← constructor (no virtual dispatch needed)
ClassName::ctor_args        ← constructor with non-default arguments
ClassName::dtor             ← destructor body
ClassName::deleting_dtor    ← virtual destructor: takes flag=1 (heap) or 0 (stack)
```
The `deleting_dtor` always has signature `void* __thiscall(ClassName* this, int heapFlag)`.

---

## Variable Naming

### Global variables
```
global_ClassName_purposeName   ← static/global associated with a class
vtbl_ClassName                 ← vtable for ClassName (always a global in .rdata)
data_ClassName_fieldName       ← static data arrays (e.g. render target poly verts)
```

### Local variables (inside decompiled functions)
Prefer names that describe **what the value is**, not how it was computed:
```
✓  targetActor, rayOrigin, loopStartTime, groundDist
✗  v3, v12, result, temp
```

Use the type name as a hint:
```
MACT* mobileActor       (not "actor" or "v5")
AIPlanner* planner      (not "pPlanner")
float deltaTime         (not "dt")
```

When a variable is a pointer to a struct field accessed repeatedly, name it after the field:
```
AIPackage* currentPkg = planner->aiPackages[idx];
```

### Stack variables (decompiler artifacts)
When the decompiler creates variables like `v2`, `v12` because two different code paths share a register slot, IDA refuses the rename — leave them alone and document with a comment instead.

---

## Type / Struct Naming

Structs follow PascalCase:
```
MACT, MACH, MACR, MACP        ← four-letter engine acronyms (all caps)
NiObject, NiAVObject, NiNode   ← NetImmerse classes keep their Ni prefix
TList, TArray, THashMap        ← engine collection templates
WorldController, DataHandler   ← singleton subsystems
CombatSession, AIPlanner       ← compound words, no underscores
```

Enum values use PascalCase or SCREAMING_SNAKE:
```
AIBehaviour enum: Attack, Flee, Alarmed, Undecided, ...
AnimGroupClass_Looping         ← category prefix underscore
```

Interface types (vtable structs):
```
InterfaceMACT, InterfaceNiAVObject, InterfaceAIPackage
```

---

## Comment Style

### Function-level (at entry point address)
Describe **what** the function does and **when** it is called:
```
// Called once per animation group at load time by mergeAnimGroups().
// Pre-bakes approxRootMovementSpeed[group] = XY-distance / loop-duration.
// Only runs for AnimGroupClass_Looping groups with a valid keyframe source.
```

### Inline comments (at specific addresses inside a function)
Short, imperative phrases describing the operation at that line:
```
// zero the Z axis — vertical MRT displacement is discarded; physics owns it
// delta = end_position − start_position over one complete loop cycle
// deactivate sequence and reset offset to zero
```

### Commenting unknown sub_ functions (before renaming)
When you encounter `sub_XXXXXX` in a callee list, add a comment at the call site:
```
// → sub_700E70: sorts 3 floats; records original index permutation (see SAT axis tests)
```

---

## Prototype / Signature Conventions

IDA uses `__thiscall` for all non-static methods. Set prototypes as:
```c
void __thiscall ClassName::methodName(ClassName *this, int arg1, float arg2);
bool __thiscall ClassName::boolReturner(ClassName *this);
```

For `deleting_dtor`:
```c
void* __thiscall ClassName::deleting_dtor(ClassName *this, int heapFlag);
```

For vtable function pointers, the first argument is always the concrete type or `void*`:
```c
void (__thiscall *run)(AIPackage *this, MACT *actor, float deltaTime);
```

---

## When to Use `declare_c_type` vs `set_global_variable_type`

- `declare_c_type` — adds a new struct/enum/typedef to the IDA type library. Use when you've reverse-engineered a layout that isn't already defined.
- `set_global_variable_type` — applies a type to a specific global address. Use to tell IDA that `0x7c67dc` is `WorldController*`.
- `set_local_variable_type` / `set_stack_frame_variable_type` — apply types to local vars in a decompiled function. Triggers re-analysis of the pseudocode.

Order matters: declare the type first, then apply it to globals/locals.

---

## Recognizing Common Patterns

### Refcount assignment (NiObject smart pointer)
```c
if (oldPtr) {
    if (--oldPtr->refCount == 0)
        oldPtr->vtbl.asObject->deleting_dtor(oldPtr, 1);
}
newObj->refCount++;
field = newObj;
```
This is NetImmerse's manual refcounting. Always means the field is a `NiPointer<T>`.

### MSVC `.CRT$XCU` static initializer
No direct callers + data xref from `.CRT$XCU` table = static initializer called before `main()`. Companion `atexit()` call registers a destructor. Name as:
```
ClassName_static_init_fieldName     ← initializer
ClassName_atexit_release_fieldName  ← destructor
```

### Virtual dispatch
```c
actor->vtbl->methodName(actor, ...)
```
The vtbl field is always at offset `+0x0`. Cast the `this` pointer to the concrete type to access non-virtual fields.

### TList iteration
```c
for (TList::Node *n = TList::iterateBegin(list); n; n = TList::iterateNext(list))
    process(n->data.asXxx);
```

### Cell 3×3 exterior grid
```c
DataHandler::ExteriorData **v = this->arrayExteriorCellData[0];
for (int i = 9; i--; v++)
    if ((*v)->loadingFlags == 1) { cell = (*v)->cell; ... }
```
Always a 9-element loop (3×3 cells centred on player).

### Animation layer sub-steps
`ActorAnimationData::update()` calls `updateMovementRoot()` multiple times per frame (once per layer/transition). The accumulated `positionDeltaOfMRT` is zeroed at the top of `update()` and consumed once by `getAnimationDelta()`.

### MWSE patch injection points
Functions named `patchXxx` (e.g. `patchReadKeyStateMapZoom`, `patchProcessSpells`,
`patchParticleAttachOnSimulateActor`) are **MWSE-injected hooks** — they appear in
decompiled output because MWSE patches the binary at runtime. Do not rename these;
they are intentional extension points, not game-original code. Their call sites in
`ProcessManager::update`, `ActiveMagicManager::processAllMagics`, etc. are correct.

### Simulation gate — `MobileActorFlags_ActiveInSimulation`
This flag appears as the outer guard in nearly every per-actor loop. A common double-check pattern:
```c
if ((mobile->mobileFlags & MobileActorFlags_ActiveInSimulation) == 0
 || (ref = mobile->reference) == 0
 || (ref->recordFlags & RecordFlags_Disabled) == 0)
    && (mobile->mobileFlags & MobileActorFlags_ActiveInSimulation) != 0)
```
This is the compiler's AND/OR optimisation — the net logic is: actor must be
`ActiveInSimulation` AND its reference must not be `Disabled`. Both conditions matter.

### Invisibility / Chameleon visibility check
The pattern `!effectAttributes[Invisibility] && effectAttributes[Chameleon] < 75`
appears in every head-tracking and detection context. 75 is the game's hardcoded
Chameleon threshold for "effectively invisible". When you see this pattern in an
unknown function, the function deals with perception / detection.

### effectAttributes[] indexing
`MACT::effectAttributes` (`+0x2f0`, `int[24]`) is indexed by the `EffectAttribute`
enum. Common indices to recognise in decompiled code:
- `[0]` Strength bonus from effects
- `[4]` Paralyze (> 0 means paralyzed)
- `[10]` Invisibility (non-zero = invisible)
- `[11]` Chameleon (value = %)
- `[17]` Sanctuary
- `[20]` Reflect

### StdMap red-black tree pattern
`ActiveMagicManager::mapSerialToMagicSourceInstance` and similar maps are MSVC
`std::map` — a red-black tree. In decompiled code you see:
```c
node->branch_less, node->branch_greaterequal   // left / right child
node->nextLeafOrRoot                            // parent pointer
node->red_black                                 // colour bit
global_Xxx_sentinel                             // the "end" sentinel node
```
When iterating, the global `sentinel` marks the end of the tree. The iterator's
`next()` method traverses left-spine → root → right-spine. Recognise this as
`std::map` traversal — name it `StdMap_KeyType_ValueType` and its iterator as
`StdMap_Xxx::iterator::next`.

### RecordsHandler — master record lists
`dataHandler->records` (`RecordsHandler*`) holds `TList*` for every record type.
To find a record by type, check the appropriate list:
```
records->pRacesList, pClassesList, pFactionsList, pScriptsList,
pSoundsList, pGlobalsList, pDialoguesList, pRegionsList, etc.
records->allObjectsById   (THashMap** — fast lookup by string ID)
records->magicEffects[143] — indexed by MagicEffect enum (base 0)
```

### Attribute vs effectAttribute
- `MACT::strength` (etc.) at `+0x254` — `Attribute` struct with `.base` and `.current`
  (current includes temporary bonuses from equipment, not magic)
- `MACT::effectAttributes[24]` at `+0x2f0` — magic effect modifiers (Fortify Strength, etc.)
  The true effective value is `strength.current + effectAttributes[StrengthBonus]`.

### AnimStateAttack enum — dead/dying check
The pattern:
```c
if (animStateAttack != AnimStateAttack_Dead && animStateAttack != AnimStateAttack_Dying
    && MACT::isNotKnockedDownOrOut(actor)
    && actor->effectAttributes[EffectAttribute_Paralyze] <= 0)
```
appears before any "this actor can act" logic. Recognise it as the standard
"actor is capable of acting" guard.
