# NiDX8Renderer Batching — Lessons Learned

Branch: `nidx8-render-batching`. These notes capture things worth remembering
from wiring up Morrowind's latent `BeginBatch / BatchRenderShape |
BatchRenderStrips / EndBatch` pipeline and chasing a whiteout regression.

## Context

`NiDX8Renderer` ships with a fully-implemented batch API whose only xref is its
own vtable at `0x74F4D0`. The goal is to detour `RenderShape` /
`RenderTristrips` so opaque non-skinned geometry is enqueued and drained
through a single `BeginBatch … EndBatch` envelope per Click, amortising the
per-shape pipeline setup over many entries.

## What bit us

### 1. `NiCamera::Click` is called many times per frame, on many different scenes

A single rendered frame dispatches Click from many call sites. Enumerated via
`get_callers(0x6CC7B0)`:

| Caller | Purpose | Camera | Scene |
|---|---|---|---|
| `TES3Game_static::renderMainScene` | splash / cutscene | `splashscreenCamera` | splash root |
| `TES3Game_static::renderMainScene` | world pass with shadows | `worldCamera` | `worldCamera.root` |
| `TES3Game_static::renderMainScene` | world pass without shadows | `worldCamera` | `worldCamera.root` |
| `TES3Game_static::renderMainScene` | first-person arm | `armCamera` | arm root |
| `TES3Game::renderNextFrame` | menu 3D | `menuCamera` | menu root |
| `ShadowManager::render` | shadow map | `shadowManager->sgShadowCamera` | shadow volume roots |
| `WaterController::renderWater` | reflection / NVLake | **`worldCamera`** (!) | **`sgWaterPlane`** (swapped!) |
| `WorldControllerRenderTarget::draw` / `drawMap` / `takeScreenshot` / `updateFogOfWarVertexBuffer` | ancillary RTs | target-specific | target-specific |
| `LoadScreenManager::render` | loading screens | load cam | load scene |
| `ui_MenuRaceSex_renderHead` | race/sex preview | UI cam | preview head |

A hook inside `NiCamera::Click` (or at the `CullShow` call it makes) fires for
**every** one of these. Batching that you wrote with "the world pass" in mind
silently inherits shadow, reflection, screenshot and loadscreen re-traversals
of the same scene graph, and those re-traversals happen with different render
targets and different transient property state.

**Takeaway:** before hooking anywhere near Click or CullShow, run
`get_callers` and decide which call sites you actually want to apply to.
Default-on-all is almost never correct for anything that mutates D3D state.

### 2. Camera identity alone is not enough — water reflection swaps `spScene`

Naïve scope gate: "batch only when `camera == worldController->worldCamera`."
That correctly excludes splash, arm, menu, and shadow (which uses a different
`NiCamera`), but **not** water reflection. `WaterController::renderWater`
temporarily rewrites `camera->spScene` to `sgWaterPlane`, calls
`NiCamera::Click(camera, …)`, then restores. The camera pointer is identical
to the main-pass camera throughout.

The fix is a two-pointer match at `CullShowAndFlush` entry:

```cpp
if (camera == worldCam.cameraData.camera.get()
 && scene  == reinterpret_cast<NI::AVObject*>(worldCam.root.get())) {
    // activate batching
}
```

Scene pointer equality against the stable `worldCamera.root` excludes the
reflection pass because its scene argument is `sgWaterPlane`, not the world
root.

**Takeaway:** when a render pass is defined by a temporary state swap on a
shared object, the gate has to check the swapped state, not the identity of
the shared object.

### 3. `EndBatch` uses "firstBatch" as the representative entry

`NiDX8Renderer::EndBatch` runs `selectAndLinkPass` / `buildPass` once using
the first queued entry, then reuses that pipeline for every entry in the
batch. Two consequences:

- **Skinned geometry cannot batch.** Per-entry skin state (partition, bone
  palette, vertex-shader variant) can't be represented by a single firstBatch.
  Our hooks fall through to vanilla when `skinInstance != 0`.
- **Property-state divergence inside the queue silently draws with the wrong
  pipeline.** If entries are enqueued with different property / effect
  states, only firstBatch's state is bound. Either group by pipeline signature
  before draining, or open a new Begin/End envelope when the signature
  changes.

**This isn't theoretical.** It was the dropped-shape bug. Batching the
*entire* `worldCamera.root` traversal into a single queue and draining at the
end caused `firstBatch` mismatches across `WorldXxxRoot` subtrees (different
attached light sets, different ancestor property state, different effect
lists). Shapes whose actual pipeline didn't match firstBatch's got drawn with
the wrong state and were silently discarded. The fix was to add a **flush
boundary between each top-level subtree** — see lesson 10.

Additionally, `EndBatch` has a `(textureSets & 0x40000000) == 0` gate around
the entire per-entry draw loop. If that bit is ever set on firstBatch, every
entry in the batch is silently dropped. Worth sanity-checking firstBatch's
flags before handing off.

### 4. `EndBatch` leaves D3D state pointing at the last batched shape

The vertex-blend cache slot at `renderState + 0x6B0` (D3DRS_VERTEXBLEND, index
151) and the currently-bound property state are left pointing at whatever
EndBatch last drew. A subsequent vanilla `RenderShape` that happens to match
those cached values will **not** push a `SetRenderState`, so the D3D device
state is effectively inherited from the batched draw.

Fix: after draining, invalidate the relevant cache slots and re-run
`updateD3DState` with the renderer's `currentPropertyState` so the next shape
sees a clean "as-if-no-batching-happened" state.

### 5. "Whiteout" was dropped shapes, not saturated color

Screenshot inspection disproved the "drew white" hypothesis: the visible
background where the cuirass / banner should have been was sky blue, not flat
white, and it matched the Z-buffer background exactly. That means the draw
call never happened (or was discarded) — not that it ran and blew the color
channels.

**Takeaway:** when the artifact is uniform-colored, verify whether it matches
the clear color / sky color before chasing pipeline saturation bugs. Dropped
shapes and oversaturated draws fail to render equally well from a distance.

### 6. A mitigation with no effect is evidence against its hypothesis

We added a per-shape "skip if any ancestor owns a TimeController" filter on
the theory that animated-ancestor children were the whiteout source. The
filter walked `parentNode` per enqueue; the user confirmed it added noticeable
sorting cost and **did not change the artifact at all**.

That's strong signal: either the walk doesn't catch the offending shapes, or
the animated-ancestor correlation was coincidence. Either way, keeping the
mitigation while continuing to search is spending real CPU for zero diagnostic
value.

**Takeaway:** when a mitigation you added as a hypothesis-test has no visible
effect, remove it before adding the next one. Stacked "maybe this helps" code
makes subsequent diagnoses harder.

### 7. Scope-narrowing at pass level beats per-shape filtering

The fix we converged on — gating batching to a single Click pass via camera +
scene identity comparison — is:

- **Cheaper:** one pointer-pair compare per Click vs. one parent-chain walk
  per shape.
- **More robust:** catches *all* cross-pass contamination sources at once
  (shadow, water, arm, menu, screenshot, etc.), not just the one we
  hypothesised.
- **Easier to reason about:** "only the world pass batches" is a small
  invariant; "skip descendants of any animated node" is subtle and data-
  dependent.

**Takeaway:** when the potential-failure axis spans entire passes, scope at
the pass level. Per-shape filters are for per-shape failure axes.

### 8. Flush boundaries are a correctness requirement, not just perf

The dropped-shape bug persisted even after the main-world-pass scope gate was
in place. We initially added per-worldroot config flags expecting to *bisect*
which `WorldXxxRoot` subtree contained the offending geometry. The surprise:
the artifact vanished with **all flags at their defaults (true)** — the
structural change alone fixed it, before any flag was ever disabled.

What changed structurally: the old hook ran `vanilla(worldCamera.root)` once
and flushed a single queue at the end. The new hook iterates
`worldCamera.root.children`, calling `vanilla(child)` per subtree and
`FlushBatches()` between each. Same vanilla traversal, same total set of
enqueued shapes — the **only** difference is that the queue is drained at
subtree boundaries.

Why that fixes it, restated from lesson 3: `EndBatch` draws the whole queue
with `firstBatch`'s pipeline state (modulo same-signature grouping). When the
queue spans multiple `WorldXxxRoot` subtrees with different attached light
sets, ancestor effect lists, or property-state ancestry, entries whose real
pipeline doesn't match firstBatch's draw with wrong state — and in some
combinations, not at all.

Implications:

- The per-subtree flush is **load-bearing**, not a perf optimisation. If
  future refactors remove or skip it, dropped shapes will come back.
- The per-worldroot config flags are now **kill-switches**, not diagnostic
  bisectors. Keep them for safety (cheap: ~8 bool loads + strcmps per frame)
  but don't expect them to carry useful bisection signal any more.
- Any extension that expands batching scope (water reflection is the obvious
  next candidate) must keep subtree-boundary flushing. Draining one queue
  across dissimilar pipeline ancestries is the bug.

**Takeaway:** for any batch API that reuses a representative entry's
pipeline, the flush granularity is part of the correctness contract. Ask
"what's the coarsest group of draws that share pipeline state?" and never
let a queue span coarser than that.

### 9. Struct offsets to keep handy

| Field | Offset | Notes |
|---|---|---|
| `NI::AVObject::flags` | `+0x14` | test `0x36` for `Const_SoftwareSkinningFlag` via `[eax + 0x36]` word |
| `NI::AVObject::parentNode` | `+0x18` | |
| `NI::AVObject::worldTransform` | `+0x40` | RenderShape's `transform` arg is `&shape->worldTransform` — recover AVObject via `-0x40` |
| `NI::ObjectNET::controllers` | `+0x10` | raw `Pointer<TimeController>` = 4 bytes |
| `NI::Node::children` | `+0x90` | TArray |
| `NI::Geometry::propertyState` | `+0x90` | |
| `NI::Geometry::effectState` | `+0x94` | |
| `NI::Geometry::skinInstance` | `+0x9C` | |
| `TES3::WorldControllerRenderCamera::root` | `+0x8` | stable sgRoot (not swapped by water) |
| `TES3::WorldControllerRenderCamera::cameraData.camera` | `+0x10` | the `NI::Camera*` |
| `NiDX8Renderer` vertex-blend cache slot | `renderState + 0x6B0` | D3DRS_VERTEXBLEND (index 151) |

### 10. Relevant addresses

| Address | Symbol |
|---|---|
| `0x6ACEF0` | `NiDX8Renderer::RenderShape` (5-byte JMP detour) |
| `0x6ACFC0` | `NiDX8Renderer::RenderTristrips` (5-byte JMP detour) |
| `0x6AE030` | `NiDX8Renderer::BeginBatch` |
| `0x6AE090` | `NiDX8Renderer::EndBatch` |
| `0x6AE600` | `NiDX8Renderer::BatchRenderShape` |
| `0x6AE8C0` | `NiDX8Renderer::BatchRenderStrips` |
| `0x6AEB80` | `NiDX8Renderer::DrawPrimitive` |
| `0x6B82A0` | `NiDX8Renderer::updateD3DState` |
| `0x6CC7B0` | `NiCamera::Click` |
| `0x6CC874` | `CullShow` call inside Click — our `genCallEnforced` site |
| `0x6EB480` | `NiAVObject::CullShow` (vanilla target) |
| `0x74F4D0` | `NiDX8Renderer` vtable (only xref source for the batch API) |
| `0x41BE90` | `TES3Game::renderNextFrame` |
| `0x41C400` | `TES3Game_static::renderMainScene` |
| `0x4352A0` | `ShadowManager::render` |
| `0x51C550` | `WaterController::renderWater` |

## Deferred scope — passes that could batch but currently don't

The scope gate is conservative: only the main `worldCamera` pass with its own
`sgRoot` batches. That was chosen to isolate variables while chasing the
whiteout bug, not because batching is fundamentally incompatible with the
other passes. Ranked by potential win and risk:

### Water reflection — good candidate, deferred

Reflection re-renders most of the world geometry (minus the water surface)
through `WaterController::renderWater`, so the per-shape pipeline cost is the
same shape of problem batching solves. The fix to include it would be: loosen
the scene-pointer check at `CullShowAndFlush` entry to also allow
`scene == waterController->sgWaterPlane`, with the `camera == worldCamera`
check unchanged.

Specific risks to validate before enabling:

- `NiAlphaPropertyFlagBit_NoSorter` is flipped per-pass based on camera
  above/below water height. If reflection entries get enqueued with mixed
  alpha-property signatures, firstBatch's signature drives the rest.
- NVLake path adds/removes `texturingProperty` and `rendererSpecificProp` on
  `sgWaterPlane` around the Click, so the effect state at enqueue time can
  differ from the last frame.
- Reflection may invert culling for the reflection render target.

Per-Click batching is fine for consistent-state queues because the queue
drains inside the same Click. The grouped-sort path *should* open a new
`Begin / End` envelope on property-state-signature change. That path is
untested against reflection-pass geometry.

### Shadow render — weaker candidate

`ShadowManager::render` issues back-to-back Clicks per shadow caster, flipping
`NiStencilProperty::drawMode` (CW/CCW) and `zFailAction` (Increment/Decrement)
between them. That is exactly the kind of state variance that EndBatch's
firstBatch-as-representative model handles poorly. Entry counts are also low
(one Click per caster, few casters per frame), so the amortisation win is
small. Not worth the exposure until (if ever) the representative model is
replaced by strict per-entry signature gating.

### Arm / menu / splash — not worth it

Entry counts are trivial (arm = 1–2 shapes per frame; menu = a handful of UI
models; splash fires at most once per session). The per-Click overhead of
BeginBatch/EndBatch would likely eat any amortisation. Leave excluded.

### Progression plan

1. Validate main pass is clean (current state).
2. Extend scene-pointer check to accept `sgWaterPlane` → adds reflection.
   Measure frame time with water visible; confirm no visual regression in the
   reflection.
3. If reflection regresses visually, the firstBatch-signature mismatch is the
   most likely cause — add per-entry property-state-signature guarding at
   drain time before trying again.
4. Do not touch shadow / arm / menu / splash until points 1-3 are stable.

## Process notes

- **Verify hypotheses before adding mitigation code.** A 10-minute visual
  check of a screenshot saved chasing a non-bug (color saturation) for an
  indefinite period.
- **Enumerate callers before hooking.** Especially for engine primitives like
  `Click`, `CullShow`, `Display`. The hook fires on every caller, not just
  the one you had in mind.
- **When investigating, make the gate Lua-togglable.** `EnableDX8BatchRendering`
  flipping at runtime meant we could A/B the full patch vs. vanilla without
  rebuilding.
- **Remove diagnostic code once its hypothesis is disproved.** Leaving the
  ancestor walk in while adding the scope gate would have confounded the
  next measurement.
