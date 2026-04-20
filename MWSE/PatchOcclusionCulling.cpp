#include "PatchOcclusionCulling.h"

#include "Log.h"
#include "MemoryUtil.h"
#include "MWSEConfig.h"

#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3WorldController.h"

#include "NIAVObject.h"
#include "NICamera.h"
#include "NIColor.h"
#include "NIDefines.h"
#include "NIGeometryData.h"
#include "NINode.h"
#include "NIProperty.h"
#include "NITArray.h"
#include "NITransform.h"
#include "NITriShape.h"
#include "NITriShapeData.h"

#include "external/msoc/CullingThreadpool.h"
#include "external/msoc/MaskedOcclusionCulling.h"

#include <algorithm>
#include <cfloat>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstring>
#include <limits>
#include <ostream>
#include <thread>
#include <unordered_map>
#include <vector>

namespace mwse::patch::occlusion {

	// MSOC tile-buffer resolution. Decoupled from the game viewport: the
	// rasterizer works in NDC via the camera's world-to-clip matrix, then maps
	// NDC to this framebuffer via its own internal scale/offset.
	constexpr unsigned int kMsocWidth = 512;
	constexpr unsigned int kMsocHeight = 256;

	// Clip-space w floor. Intel MSOC treats this as the near plane. In
	// Morrowind units (1 unit ~= 1.4 cm) 1.0 is a safe floor below the
	// engine's own near plane and well above the numerical noise that
	// explodes NDC after perspective divide. Not exposed as a knob —
	// numerical floor, lowering it risks NaN in projection math.
	constexpr float kNearClipW = 1.0f;

	// All other occluder/occludee thresholds (radius bounds, thin-axis
	// rejection, inside-occluder margin, testee depth slack, max triangles,
	// min occludee radius, scene-type gates) are Configuration::Occlusion*
	// knobs loaded from MWSE.json; see MWSEConfig.{h,cpp} for defaults and
	// PatchOcclusionCulling-plan.md for the rationale behind each default.

	// Intel MSOC is allocated on the heap via Create()/Destroy(). We leak
	// this on process exit (same pattern as other long-lived MWSE globals).
	static ::MaskedOcclusionCulling* g_msoc = nullptr;

	// World-to-clip matrix, transposed from NI's row-major M*v layout into
	// Intel's column-major v*M layout (consecutive memory = one column).
	// Intel reads clip.x = v.x*m[0] + v.y*m[4] + v.z*m[8] + m[12].
	// Refreshed each frame at the top-level entry of CullShow_detour.
	static float g_worldToClip[16];

	// Reusable buffers for rasterizeTriShape. Grown on demand, never shrunk;
	// MSOC is invoked single-threaded from the worldCamera pass so a single
	// module-level buffer is sufficient.
	static std::vector<float> g_occluderVerts;
	static std::vector<unsigned int> g_occluderIndices;

	// Per-frame matrix metrics used by testSphereVisible.
	// g_ndcRadiusX/Y: L2 norm of the world->clip.x / clip.y coefficients, so
	// the NDC half-extent of a sphere of radius r at clip-w cw is
	// r * g_ndcRadiusX / cw (resp. Y). Reads the leading 3 entries of each
	// row; the trailing entry is translation and doesn't contribute to the
	// gradient.
	// g_wGradMag: L2 norm of clip.w coefficients, so the worst-case clip-w
	// offset from the sphere center to its near surface is r * g_wGradMag.
	// For a standard perspective projection after a pure-rotation world->view
	// this is 1.0; computing it from the matrix handles any scaling.
	static float g_ndcRadiusX = 0.0f;
	static float g_ndcRadiusY = 0.0f;
	static float g_wGradMag = 0.0f;

	// _Claude_ DataHandler::worldLandscapeRoot captured at top-level frame
	// entry. Used by the drain loop to short-circuit occludee queries on
	// terrain patches (25v/32t, ~always visible under the camera). Plain
	// NiNode named "WorldLandscape" at DH+0x94, persists across cell
	// changes — so we could cache once, but re-reading per frame is a
	// single pointer load and stays robust to teardown / new-game reloads.
	// Null before the game world exists (load screen, menu).
	static NI::Node* g_worldLandscapeRoot = nullptr;

	// True only while TES3Game_static::renderMainScene (0x41C400) is on the
	// stack. Gates MSOC activation so every Click tree that fires outside
	// the per-frame main scene — load-screen splash, offscreen UI targets,
	// chargen race preview, MGE water reflection weather passes, etc. (see
	// rendering-engine-notes §1.6) — runs the pure engine-equivalent path.
	// Those callers wield cameras with setups we haven't validated for
	// occlusion culling, and the main-scene frame is the only place where
	// MSOC's cost is paid back.
	static bool g_inRenderMainScene = false;

	// True only while the scene graph is being traversed for the worldCamera
	// main pass. CullShow_detour gates MSOC work on this so shadow-manager,
	// water-refraction, armCamera and other non-main Clicks inside
	// renderMainScene still run the pure engine-equivalent path.
	static bool g_msocActive = false;

	// Per-frame diagnostics. Reset at the top of each worldCamera traversal.
	static uint64_t g_recursiveCalls = 0;
	static uint64_t g_recursiveAppCulled = 0;
	static uint64_t g_recursiveFrustumCulled = 0;
	static uint64_t g_rasterizedAsOccluder = 0;
	// _Claude_ Per-frame sum of outTri across successful rasterizeTriShape
	// calls. Used to size the threadpool queue (maxJobs): each submission
	// with outTri <= TRIS_PER_JOB (1024) is one queue slot, so this number
	// divided by TRIS_PER_JOB + rasterizedAsOccluder gives us the upper
	// bound on queue writes per frame.
	static uint64_t g_occluderTriangles = 0;
	static uint64_t g_skippedInside = 0;
	static uint64_t g_skippedThin = 0;
	static uint64_t g_skippedAlpha = 0;
	static uint64_t g_queryTested = 0;
	static uint64_t g_queryOccluded = 0;
	static uint64_t g_queryViewCulled = 0;
	static uint64_t g_queryNearClip = 0;
	static uint64_t g_deferred = 0;
	// Diagnostic: NiNodes / non-geom AVObjects tested inline during
	// traversal (as opposed to deferred NiTriBasedGeom leaves). Helps
	// attribute queryOccluded between "whole subtree culled early"
	// (inline) and "leaf shape hidden behind fully-populated buffer"
	// (drain-phase).
	static uint64_t g_inlineTested = 0;
	// Phase 2.2 gate counters. Non-zero at default config means a
	// gate is mis-placed (defaults are tuned to keep all three at 0
	// on typical scenes).
	static uint64_t g_skippedTriCount = 0;
	static uint64_t g_skippedTesteeTiny = 0;
	static uint64_t g_skippedSceneGate = 0;
	// _Claude_ Deferred terrain leaves bypassed from TestRect. Ground
	// patches sit under the camera and are nearly always visible;
	// skipping the query saves depth-buffer bandwidth + ~1 µs/leaf.
	static uint64_t g_skippedTerrain = 0;
	// _Claude_ Aggregate terrain occluder counters. Each visible Land node
	// produces one combined RenderTriangles submission; triCount is the
	// total sum across Lands that frame. Zero unless
	// Configuration::OcclusionAggregateTerrain is on.
	static uint64_t g_aggregateTerrainLands = 0;
	static uint64_t g_aggregateTerrainTris = 0;
	// _Claude_ Wall-clock time spent in rasterizeAggregateTerrain per
	// frame (tree walk + vertex transform + RenderTriangles submit).
	// Includes the async enqueue path when the threadpool is active —
	// actual worker rasterisation happens in parallel and is not counted.
	static uint64_t g_aggregateTerrainUs = 0;
	// _Claude_ Per-Land merged occluder cache. Key = per-Land NiNode
	// pointer under the WorldLandscape root. Value = world-space vertex
	// buffer + index buffer, prebuilt on first sight. Scene-graph walk
	// and vertex transform become O(1) hits after that. Entries are
	// mark-and-sweep evicted each frame: any key not touched this frame
	// is stale (cell change destroyed its LandscapeData subtree and its
	// NiNode pointer). Eviction is safe because previous-frame async
	// queues are consumed by ClearBuffer()/implicit Flush before the
	// next aggregate pass runs.
	struct LandCacheEntry {
		std::vector<float> verts;
		std::vector<unsigned int> indices;
		unsigned int triCount = 0;
		bool seen = false;
	};
	static std::unordered_map<NI::Node*, LandCacheEntry> g_landCache;
	static uint64_t g_landCacheHits = 0;
	static uint64_t g_landCacheMisses = 0;
	static uint64_t g_landCacheEvictions = 0;

	// _Claude_ Temporal coherence cache for drain-phase TestRect results.
	// Keyed by NI::AVObject* (the deferred shape pointer). Each entry
	// records the last TestRect verdict and the frame it was taken at.
	// Configuration::OcclusionTemporalCoherenceFrames (N) controls reuse:
	// N=0 disables the cache entirely; N>0 reuses a fresh entry for up
	// to N intervening frames before re-querying.
	//
	// _Claude_ We cache only OCCLUDED verdicts. VISIBLE and VIEW_CULLED
	// fall through to display() anyway, which dereferences the live
	// p.shape — caching them would add stale-pointer surface area while
	// gaining nothing on the hit path. OCCLUDED is the only verdict
	// where the hit lets us skip both TestRect and display().
	//
	// Move detection: we compare worldBoundOrigin + worldBoundRadius
	// against the snapshot taken at query time. Any meaningful change
	// (animated shape, physics movement) invalidates the entry.
	//
	// _Claude_ vtable verify: NI's first 4 bytes are a vtable pointer
	// constant for the process lifetime per class. We snapshot it on
	// insert and compare on hit. Mismatch → the address has been freed
	// and reused for a different-typed object → miss and re-query
	// (avoids dereferencing wrong-class fields via cached bounds).
	//
	// Lifetime safety: shapes can be destroyed between frames, and
	// NI::AVObject pointers can theoretically be reused by new allocations.
	// Stale entries are harmless for correctness because the miss path
	// overwrites them on the next query, but to bound memory we age-prune
	// entries older than 2*N frames at frame entry.
	struct DrainCacheEntry {
		::MaskedOcclusionCulling::CullingResult result;
		uint32_t lastQueryFrame;
		void* vtbl; // _Claude_ snapshot of *(void**)shape at insert
		float boundOriginX, boundOriginY, boundOriginZ;
		float boundRadius;
	};
	static std::unordered_map<NI::AVObject*, DrainCacheEntry> g_drainCache;
	static uint64_t g_drainCacheHits = 0;
	static uint64_t g_drainCacheMisses = 0;
	// File-scope frame counter. Needed by the drain loop to decide
	// cache freshness; incremented once per top-level frame.
	static uint32_t g_frameCounter = 0;
	// _Claude_ Cached currentCell pointer across frames. When it changes
	// (cell transition: ext→int, int→ext, int→int, ext→ext chunk swap),
	// we wipe both g_landCache and g_drainCache. Without this, destroyed
	// NI::Node* / NI::AVObject* pointers can be reused by new allocations
	// in the fresh cell, and a stale cache entry might dereference freed
	// memory. Pointer-compare is cheap; full clear happens at most once
	// per cell load.
	static TES3::Cell* g_lastCell = nullptr;
	static uint64_t g_cellChanges = 0;
	// Phase 2.2 timers (microseconds). steady_clock is QPC-backed
	// on MSVC so no QPC wrapper is needed. Rasterize accumulates
	// each RenderTriangles call; drain wraps the whole drain body.
	static uint64_t g_rasterizeTimeUs = 0;
	static uint64_t g_drainPhaseTimeUs = 0;
	// Phase 3.2: time spent in threadpool Flush() barrier before drain.
	// Only populated when async mode is active; zero otherwise.
	static uint64_t g_asyncFlushTimeUs = 0;

	// Phase 3.2 async-rasterization state.
	//
	// g_threadpool is created once in installPatches and lives for the
	// process. Runtime-toggle of OcclusionAsyncOccluders works without a
	// restart because the threadpool's lifecycle (Wake/Suspend) is driven
	// per-frame off that flag — a suspended threadpool sleeps at ~0% CPU.
	//
	// g_asyncThisFrame latches the async flag at top-level entry so any
	// mid-frame Configuration edits from Lua don't mix sync and async
	// submissions within one traversal.
	//
	// The two arena vectors own the per-submission triangle data until
	// Flush() retires the corresponding render job. CullingThreadpool
	// requires caller-owned buffers to stay unchanged between submission
	// and completion; we enforce that by moving g_occluderVerts /
	// g_occluderIndices into the arena per submission (next rasterize call
	// allocates fresh vectors). Cleared at top-level entry, after
	// ClearBuffer's implicit Flush has retired the previous frame's jobs.
	static ::CullingThreadpool* g_threadpool = nullptr;
	static bool g_asyncThisFrame = false;
	static std::vector<std::vector<float>> g_asyncOccluderVerts;
	static std::vector<std::vector<unsigned int>> g_asyncOccluderIndices;

	struct ScopedUsAccumulator {
		uint64_t& target;
		std::chrono::steady_clock::time_point start;
		explicit ScopedUsAccumulator(uint64_t& t)
			: target(t), start(std::chrono::steady_clock::now()) {}
		~ScopedUsAccumulator() {
			target += static_cast<uint64_t>(
				std::chrono::duration_cast<std::chrono::microseconds>(
					std::chrono::steady_clock::now() - start).count());
		}
	};

	// Deferred-display queue for small NiTriShape leaves. Small shapes are
	// queued during the main traversal and drained *after* all occluder
	// rasterisation is complete, so their MSOC visibility test runs
	// against the fully-populated depth buffer instead of whatever
	// occluders happened to precede them in scene-graph order. Avoids the
	// OpenMW two-pass cost (single traversal) and still gives big shapes
	// priority for rasterisation.
	//
	// Only NiTriShape leaves are deferred. NiNodes must stay inline so
	// their subtree keeps contributing occluders during the main pass;
	// other leaf types (Particles, etc.) are rare enough to keep inline.
	struct PendingDisplay {
		NI::AVObject* shape;
		NI::Camera* camera;
	};
	static std::vector<PendingDisplay> g_pendingDisplays;

	// Debug tint overlay. When any of the three DebugOcclusionTint* flags
	// in Configuration are set, classified leaves render in a distinct hue:
	//   red    — OCCLUDED (would have been culled; kept visible for compare)
	//   green  — test passed (survived the MSOC query)
	//   yellow — rasterised as an occluder (driving the depth buffer)
	//
	// Lifetime model (persistent-clone): the first time we tint a shape we
	// clone its effective MaterialProperty (ancestor-inherited or own),
	// detach any own material, and attach the clone. The clone stays
	// attached for the shape's lifetime; subsequent frames only overwrite
	// emissive + bump revisionID. This avoids the per-frame allocator churn
	// that caused a crash earlier (~1.7k new/delete per frame × hundreds of
	// frames drove the DX8 renderer's per-material state cache stale —
	// freed clones' addresses got recycled and the cache returned dangling
	// D3D state).
	//
	// At end of each main-scene frame every tracked clone is reset to the
	// source material's current emissive, so the default render state shows
	// the shape's real color; the next frame's classifications re-apply the
	// tint. Shapes that never get tinted never allocate.
	//
	// Morrowind's art shares NiMaterialProperty instances two ways:
	//   - via inheritance (a parent NiNode's material applies to every
	//     descendant without their own),
	//   - via NIF streamable dedup (two leaf shapes point at the same own
	//     MaterialProperty object),
	// and mutating either in place stains every sibling. Cloning avoids
	// both. Only NiTriBasedGeom leaves are tinted — tinting a NiNode would
	// propagate via inheritance to every child in its subtree.
	static const NI::Color kTintOccluded(1.0f, 0.0f, 0.0f);
	static const NI::Color kTintTested(0.0f, 1.0f, 0.0f);
	static const NI::Color kTintOccluder(1.0f, 1.0f, 0.0f);

	// NiMaterialProperty vtable address (see NIDefines.h VTableAddress
	// block). Property::Property() sets the base Property vtable; every
	// concrete subclass overwrites with its own (see AlphaProperty ctor
	// in NIProperty.cpp). MWSE exposes no public NiMaterialProperty ctor
	// so we open-code the same pattern here.
	constexpr uintptr_t kNiMaterialPropertyVTable = 0x75036C;

	struct TintClone {
		// Holds the shape alive so the map key + the clone (attached to
		// the shape's property list) stay valid as long as our map entry
		// exists. Without this, cell-unload would delete the shape and
		// leave us with a dangling key + freed clone.
		NI::Pointer<NI::AVObject> shape;
		// Holds the source material alive so we can read its current
		// emissive during end-of-frame reset. For own-material shapes we
		// detached it from the list; this Pointer is now the sole owner.
		// For purely-inheriting shapes source is the ancestor's material
		// (still alive via ancestor); keeping a Pointer is cheap insurance.
		// Null only if the shape had no reachable MaterialProperty at all.
		NI::Pointer<NI::MaterialProperty> source;
		// Owned by the shape's property list (attachProperty adds the ref).
		// Safe to raw-ptr because the shape Pointer above keeps the list —
		// and thus this clone — alive for the entry's lifetime.
		NI::MaterialProperty* clone;
	};
	static std::unordered_map<NI::AVObject*, TintClone> g_tintClones;

	// Walk from obj up through ancestors, returning the first
	// MaterialProperty reached. Matches the engine's property-inheritance
	// rule: a shape with no own MaterialProperty uses its ancestor's.
	static NI::MaterialProperty* findInheritedMaterial(NI::AVObject* obj) {
		for (NI::AVObject* cur = obj; cur; cur = cur->parentNode) {
			for (auto* node = &cur->propertyNode; node && node->data; node = node->next) {
				if (node->data->getType() == NI::PropertyType::Material) {
					return static_cast<NI::MaterialProperty*>(node->data);
				}
			}
		}
		return nullptr;
	}

	// True if obj has an effective NiAlphaProperty (own or ancestor) with
	// alpha blending OR alpha testing enabled. Such shapes are visually
	// transparent in parts and must not be rasterised as solid occluders —
	// doing so fills the depth buffer across their full quad footprint and
	// falsely occludes everything behind the "holes" (fences, banners,
	// grates, vines, tree leaves, window glass). They're still visibility-
	// tested as normal; only the occluder rasterisation is skipped.
	static bool hasTransparency(NI::AVObject* obj) {
		for (NI::AVObject* cur = obj; cur; cur = cur->parentNode) {
			for (auto* node = &cur->propertyNode; node && node->data; node = node->next) {
				if (node->data->getType() == NI::PropertyType::Alpha) {
					const unsigned short flags = node->data->flags;
					if (flags & (NI::AlphaProperty::ALPHA_MASK
						| NI::AlphaProperty::TEST_ENABLE_MASK)) {
						return true;
					}
					// First AlphaProperty wins per NI's property-inheritance
					// (nearest-ancestor rule); stop after inspecting it.
					return false;
				}
			}
		}
		return false;
	}

	// _Claude_ Ancestor-walk: true iff obj sits under DataHandler's
	// worldLandscapeRoot. The terrain tree is shallow — leaf trishape →
	// subcell NiNode → per-Land NiNode → WorldLandscape root is exactly
	// four levels, so terrain hits return at iteration 4. Non-terrain
	// shapes run the loop to scene root (~5–8 levels) and return false;
	// that's the cost we pay for the terrain skip, measured in cache-hot
	// pointer chases so it stays under ~1 µs even at 500 deferred leaves.
	static bool isLandscapeDescendant(NI::AVObject* obj, NI::Node* root) {
		if (!root) return false;
		for (NI::AVObject* cur = obj; cur; cur = cur->parentNode) {
			if (cur == root) return true;
		}
		return false;
	}

	// Allocate a standalone NiMaterialProperty seeded from source (if any)
	// and force the tint into ambient, diffuse, and emissive. All three
	// channels are set because a sibling NiVertexColorProperty may replace
	// one of them from baked per-vertex colors (SOURCE_IGNORE /
	// SOURCE_EMISSIVE / SOURCE_AMBIENT_DIFFUSE); with all three tinted at
	// most one is overridden and the rest carry the color. World statics
	// in Morrowind use SOURCE_EMISSIVE so an emissive-only tint is
	// invisible on them while skinned actors (IGNORE/AMBIENT_DIFFUSE) show
	// it — the asymmetry we originally saw.
	static NI::MaterialProperty* cloneMaterialProperty(NI::MaterialProperty* source, const NI::Color& tint) {
		auto* mat = new NI::MaterialProperty();
		mat->vTable.asProperty = reinterpret_cast<NI::Property_vTable*>(kNiMaterialPropertyVTable);
		if (source) {
			mat->flags = source->flags;
			mat->index = source->index;
			mat->specular = source->specular;
			mat->shininess = source->shininess;
			mat->alpha = source->alpha;
		}
		else {
			mat->flags = 1;
			mat->index = 0;
			mat->specular = NI::Color(0.0f, 0.0f, 0.0f);
			mat->shininess = 10.0f;
			mat->alpha = 1.0f;
		}
		mat->ambient = tint;
		mat->diffuse = tint;
		mat->emissive = tint;
		mat->revisionID = 0;
		return mat;
	}

	static void tintEmissive(NI::AVObject* shape, const NI::Color& color) {
		// Only leaves: tinting a NiNode would stain every inheriting child.
		if (!shape->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom)) {
			return;
		}

		// Subsequent tint on an already-cloned shape: overwrite emissive
		// only. No allocations, no property-list churn.
		auto it = g_tintClones.find(shape);
		if (it != g_tintClones.end()) {
			it->second.clone->emissive = color;
			it->second.clone->incrementRevisionId();
			return;
		}

		// First tint: clone the effective material, detach any own
		// material, attach the clone. findInheritedMaterial walks from
		// shape upward so it returns the own material when present,
		// falling back to an ancestor's.
		NI::MaterialProperty* source = findInheritedMaterial(shape);
		NI::MaterialProperty* clone = cloneMaterialProperty(source, color);

		// Detach returns null if the shape had no own material (pure
		// inheritance). Either way our clone becomes the leading (and
		// only) Material property on the shape.
		NI::Pointer<NI::Property> detachedOwn = shape->detachPropertyByType(NI::PropertyType::Material);
		shape->attachProperty(clone);

		// Keep the source alive. If the own material was detached,
		// detachedOwn holds the sole ref now — promote it to our map
		// entry. Otherwise source points into an ancestor's property
		// list; take a fresh Pointer.
		NI::Pointer<NI::MaterialProperty> sourcePtr;
		if (detachedOwn) {
			sourcePtr = static_cast<NI::MaterialProperty*>(detachedOwn.get());
		}
		else {
			sourcePtr = source;
		}

		g_tintClones.emplace(shape, TintClone{ shape, sourcePtr, clone });
	}

	// End-of-frame tint reset: copy each tracked source's current emissive
	// back into its clone so shapes that weren't re-tinted next frame
	// render with their real color. Cheaper than detach/re-attach; stays
	// pointer-stable so the DX8 renderer's per-material cache doesn't churn.
	static void resetFrameTints() {
		for (auto& [key, entry] : g_tintClones) {
			NI::Color target = entry.source
				? entry.source->emissive
				: NI::Color(0.0f, 0.0f, 0.0f);
			entry.clone->emissive = target;
			entry.clone->incrementRevisionId();
		}
	}

	// Fill g_worldToClip by transposing NI::Camera::worldToCamera into
	// Intel's column-major v*M layout. NI stores row-major M*v — clip[r] =
	// Σ_c ni[r*4+c]*v[c] (verified against NiCamera::ScreenSpaceBoundBound
	// at 0x6CCEC0). Intel's TransformVerts computes out[r] =
	// Σ_c v[c]*mtx[r*4+c] with mtx indexed column-major as mtx[col*4+row],
	// so the effective formula is out[r] = Σ_c v[c]*mtx[c*4+r]. For Intel
	// to produce the engine's clip coords we need mtx[c*4+r] = ni[r*4+c].
	static void uploadCameraTransform(NI::Camera* cam) {
		const float* ni = reinterpret_cast<const float*>(&cam->worldToCamera);
		for (int row = 0; row < 4; ++row) {
			for (int col = 0; col < 4; ++col) {
				g_worldToClip[col * 4 + row] = ni[row * 4 + col];
			}
		}

		// Per-frame metrics for sphere projection. g_ndcRadiusX is the
		// operator norm of the x-row coefficients (ignoring translation):
		// an upper bound on |d(x_clip)/d(pos)| used to scale the sphere
		// radius in clip space. Similarly for y and w. After the transpose
		// into Intel's column-major layout, NI's row-r coefficients live at
		// strided positions m[0+r], m[4+r], m[8+r] (column 0..2, row r).
		const float* m = g_worldToClip;
		g_ndcRadiusX = std::sqrt(m[0] * m[0] + m[4] * m[4] + m[8] * m[8]);
		g_ndcRadiusY = std::sqrt(m[1] * m[1] + m[5] * m[5] + m[9] * m[9]);
		g_wGradMag = std::sqrt(m[3] * m[3] + m[7] * m[7] + m[11] * m[11]);
	}

	// Project a world-space point using the cached Intel-layout matrix.
	// Produces clip-space (x, y, w); z is unused by Intel's rasterizer.
	// Uses the same column-major v*M formula as Intel's TransformVerts so
	// sphere screen rects stay aligned with rasterised occluder depth.
	struct ClipXYW {
		float x, y, w;
	};
	static inline ClipXYW projectWorld(float wx, float wy, float wz) {
		const float* m = g_worldToClip;
		return {
			wx * m[0] + wy * m[4] + wz * m[8] + m[12],
			wx * m[1] + wy * m[5] + wz * m[9] + m[13],
			wx * m[3] + wy * m[7] + wz * m[11] + m[15],
		};
	}

	// Direct sphere -> NDC-rect + wmin projection, then Intel's TestRect.
	// Projects only the sphere center and derives the NDC half-extent and
	// near-surface clip-w from per-frame matrix metrics (see
	// uploadCameraTransform). Tighter than an 8-corner world AABB projection
	// by ~sqrt(3) and stable under camera rotation — neither the NDC radius
	// nor wMin depend on how the sphere's bbox aligns with camera axes, so
	// small-mesh queries don't flicker across TestRect's hiZ thresholds.
	//
	// If the sphere's near surface straddles the near plane we bail as
	// VISIBLE: TestRect can't project a straddling rect safely.
	static ::MaskedOcclusionCulling::CullingResult testSphereVisible(
		const TES3::Vector3& center, float radius) {
		const ClipXYW c = projectWorld(center.x, center.y, center.z);

		const float wMin = c.w - (radius + Configuration::OcclusionDepthSlackWorldUnits) * g_wGradMag;
		if (wMin <= kNearClipW) {
			++g_queryNearClip;
			return ::MaskedOcclusionCulling::VISIBLE;
		}

		const float invW = 1.0f / c.w;
		const float cxNdc = c.x * invW;
		const float cyNdc = c.y * invW;
		const float rxNdc = radius * g_ndcRadiusX * invW;
		const float ryNdc = radius * g_ndcRadiusY * invW;

		float ndcMinX = std::max(cxNdc - rxNdc, -1.0f);
		float ndcMinY = std::max(cyNdc - ryNdc, -1.0f);
		float ndcMaxX = std::min(cxNdc + rxNdc, 1.0f);
		float ndcMaxY = std::min(cyNdc + ryNdc, 1.0f);
		if (ndcMinX >= ndcMaxX || ndcMinY >= ndcMaxY) {
			return ::MaskedOcclusionCulling::VIEW_CULLED;
		}

		return g_msoc->TestRect(ndcMinX, ndcMinY, ndcMaxX, ndcMaxY, wMin);
	}

	// Rasterise a shape's actual triangles (in world space) as an occluder.
	// Using real triangles instead of the shape's bounding box prevents the
	// classic "sign hanging off a wall gets falsely occluded because it
	// lives inside the wall's AABB volume" failure — small meshes embedded
	// in a large mesh's bbox no longer flicker across tile boundaries.
	// Returns true if we actually rasterised; false if the shape was skipped
	// (no data, too thin on some axis, or camera inside the tight AABB).
	static bool rasterizeTriShape(NI::TriShape* shape, const TES3::Vector3& eye) {
		// Skip skinned meshes (NPCs, creatures). Their vertices live in
		// bind-pose space and need skinInstance->deform() to produce the
		// current animated positions — rasterising the bind-pose verts
		// would draw the mesh in its T-pose at world origin, far from
		// where the engine actually renders it. Moving actors are also
		// poor occluders in practice.
		if (shape->skinInstance) {
			return false;
		}

		auto data = shape->getModelData();
		if (!data) {
			return false;
		}
		// Virtual accessors: the raw vertexCount / triangleListLength
		// fields are allocation sizes, while getActive*() returns the
		// logically-valid subset (e.g. after LOD trimming). Using the
		// raw values over-reads and Intel's gather crashes on the stale
		// tail entries.
		const unsigned short vertexCount = data->getActiveVertexCount();
		if (vertexCount == 0 || data->vertex == nullptr) {
			return false;
		}
		const unsigned short triCount = data->getActiveTriangleCount();
		const NI::Triangle* tris = data->getTriList();
		if (triCount == 0 || tris == nullptr) {
			return false;
		}
		// Skip absurdly dense meshes as occluders: per-frame vertex transform
		// and Intel's binning cost scale with triCount, and huge meshes rarely
		// produce proportionally better occlusion. Still visibility-tested via
		// their bounding sphere on the drain pass.
		if (triCount > Configuration::OcclusionOccluderMaxTriangles) {
			++g_skippedTriCount;
			return false;
		}

		const auto& xf = shape->worldTransform;
		const auto& R = xf.rotation;
		const auto& T = xf.translation;
		const float s = xf.scale;

		// Transform every vertex once into the reusable world-space buffer.
		// We also accumulate the tight AABB for the inside-guard and
		// thin-axis gate. Not cached across frames: TriShapeData pointers
		// can be freed and reused.
		g_occluderVerts.resize(static_cast<size_t>(vertexCount) * 3);
		float* out = g_occluderVerts.data();
		float minX = FLT_MAX, minY = FLT_MAX, minZ = FLT_MAX;
		float maxX = -FLT_MAX, maxY = -FLT_MAX, maxZ = -FLT_MAX;
		for (unsigned short i = 0; i < vertexCount; ++i) {
			const auto& v = data->vertex[i];
			const float rx = R.m0.x * v.x + R.m0.y * v.y + R.m0.z * v.z;
			const float ry = R.m1.x * v.x + R.m1.y * v.y + R.m1.z * v.z;
			const float rz = R.m2.x * v.x + R.m2.y * v.y + R.m2.z * v.z;
			const float wx = rx * s + T.x;
			const float wy = ry * s + T.y;
			const float wz = rz * s + T.z;
			out[i * 3 + 0] = wx;
			out[i * 3 + 1] = wy;
			out[i * 3 + 2] = wz;
			if (wx < minX) minX = wx;
			if (wx > maxX) maxX = wx;
			if (wy < minY) minY = wy;
			if (wy > maxY) maxY = wy;
			if (wz < minZ) minZ = wz;
			if (wz > maxZ) maxZ = wz;
		}

		// Reject pencil-shaped meshes: their silhouette area is tiny and
		// the per-frame rasterisation cost isn't worth the near-zero
		// occlusion contribution. Walls/floors with a single thin axis
		// still qualify.
		const float minDim = Configuration::OcclusionOccluderMinDimension;
		const float dx = maxX - minX;
		const float dy = maxY - minY;
		const float dz = maxZ - minZ;
		const int thinAxes = (dx < minDim ? 1 : 0)
			+ (dy < minDim ? 1 : 0)
			+ (dz < minDim ? 1 : 0);
		if (thinAxes >= 2) {
			++g_skippedThin;
			return false;
		}

		// Inside-guard: rasterising a mesh the camera sits inside would
		// cover the screen with near-face depths and falsely occlude
		// everything behind the far face. Tight AABB + small margin is
		// enough here since the real triangles are strictly inside it.
		const float m = Configuration::OcclusionInsideOccluderMargin;
		if (eye.x >= minX - m && eye.x <= maxX + m &&
			eye.y >= minY - m && eye.y <= maxY + m &&
			eye.z >= minZ - m && eye.z <= maxZ + m) {
			++g_skippedInside;
			return false;
		}

		// Expand NI's 16-bit triangle list into MSOC's 32-bit index
		// buffer. Indices beyond vertexCount-1 would out-of-bounds read
		// g_occluderVerts inside Intel's gather; clamp defensively.
		g_occluderIndices.resize(static_cast<size_t>(triCount) * 3);
		unsigned int* idx = g_occluderIndices.data();
		unsigned int outTri = 0;
		for (unsigned short i = 0; i < triCount; ++i) {
			const unsigned short a = tris[i].vertices[0];
			const unsigned short b = tris[i].vertices[1];
			const unsigned short c = tris[i].vertices[2];
			if (a >= vertexCount || b >= vertexCount || c >= vertexCount) {
				continue;
			}
			idx[outTri * 3 + 0] = a;
			idx[outTri * 3 + 1] = b;
			idx[outTri * 3 + 2] = c;
			++outTri;
		}
		if (outTri == 0) {
			return false;
		}

		// VertexLayout(12, 4, 8): stride 12 bytes, y at offset 4, z at
		// offset 8 — tightly-packed float[3] per vertex.
		// BACKFACE_NONE because NIF winding is not guaranteed consistent
		// and we want every face to contribute to occluder depth.
		if (g_asyncThisFrame) {
			// Move our owning buffers into the per-frame arena so the worker
			// threads have stable pointers until Flush() retires the job.
			// The next rasterizeTriShape call allocates fresh vectors
			// (~100us/frame of heap churn, acceptable per plan §3.2).
			g_asyncOccluderVerts.emplace_back(std::move(g_occluderVerts));
			g_asyncOccluderIndices.emplace_back(std::move(g_occluderIndices));
			// Inner vectors hold their buffer pointer across outer-vector
			// relocations (std::vector move ctor steals the pointer), so
			// .back().data() is stable for the submission's lifetime.
			const auto& vtx = g_asyncOccluderVerts.back();
			const auto& tri = g_asyncOccluderIndices.back();
			ScopedUsAccumulator t(g_rasterizeTimeUs);
			g_threadpool->RenderTriangles(vtx.data(), tri.data(),
				static_cast<int>(outTri),
				::MaskedOcclusionCulling::BACKFACE_NONE,
				::MaskedOcclusionCulling::CLIP_PLANE_ALL);
		}
		else {
			ScopedUsAccumulator t(g_rasterizeTimeUs);
			g_msoc->RenderTriangles(g_occluderVerts.data(), idx,
				static_cast<int>(outTri), g_worldToClip,
				::MaskedOcclusionCulling::BACKFACE_NONE,
				::MaskedOcclusionCulling::CLIP_PLANE_ALL,
				::MaskedOcclusionCulling::VertexLayout(12, 4, 8));
		}
		g_occluderTriangles += outTri; // _Claude_ frame-wide triangle sum
		return true;
	}

	// _Claude_ Simple frustum check for aggregate-terrain walk. No mask
	// bookkeeping — the aggregate pass runs before cullShowBody so
	// usedCullingPlanesBitfield is still clean and we don't mutate it.
	// Matches the plane-vs-sphere sign convention at cullShowBody L661.
	static bool frustumCulledSphere(NI::AVObject* obj, NI::Camera* camera) {
		const int n = camera->countCullingPlanes;
		const float r = obj->worldBoundRadius;
		for (int i = 0; i < n; ++i) {
			const auto* plane = static_cast<const TES3::Vector4*>(camera->cullingPlanePtrs.storage[i]);
			const float d = plane->x * obj->worldBoundOrigin.x
				+ plane->y * obj->worldBoundOrigin.y
				+ plane->z * obj->worldBoundOrigin.z
				- plane->w;
			if (d <= -r) return true;
		}
		return false;
	}

	// _Claude_ Append one terrain TriShape's 25v/32t mesh to the aggregate
	// buffers, transforming vertices to world space and offsetting indices
	// by the current vertex base. Mirrors rasterizeTriShape's vertex math
	// and OOB-index guard, minus the gates/skinning/thin-axis paths that
	// don't apply to terrain.
	static void appendTerrainShape(std::vector<float>& aggVerts,
		std::vector<unsigned int>& aggIdx, NI::TriShape* shape) {
		auto data = shape->getModelData();
		if (!data) return;
		const unsigned short vcount = data->getActiveVertexCount();
		if (vcount == 0 || data->vertex == nullptr) return;
		const unsigned short tcount = data->getActiveTriangleCount();
		const NI::Triangle* tris = data->getTriList();
		if (tcount == 0 || tris == nullptr) return;

		const unsigned int baseVert = static_cast<unsigned int>(aggVerts.size() / 3);
		const auto& xf = shape->worldTransform;
		const auto& R = xf.rotation;
		const auto& T = xf.translation;
		const float s = xf.scale;

		aggVerts.resize(aggVerts.size() + static_cast<size_t>(vcount) * 3);
		float* out = aggVerts.data() + baseVert * 3;
		for (unsigned short i = 0; i < vcount; ++i) {
			const auto& v = data->vertex[i];
			const float rx = R.m0.x * v.x + R.m0.y * v.y + R.m0.z * v.z;
			const float ry = R.m1.x * v.x + R.m1.y * v.y + R.m1.z * v.z;
			const float rz = R.m2.x * v.x + R.m2.y * v.y + R.m2.z * v.z;
			out[i * 3 + 0] = rx * s + T.x;
			out[i * 3 + 1] = ry * s + T.y;
			out[i * 3 + 2] = rz * s + T.z;
		}

		aggIdx.reserve(aggIdx.size() + static_cast<size_t>(tcount) * 3);
		for (unsigned short i = 0; i < tcount; ++i) {
			const unsigned short a = tris[i].vertices[0];
			const unsigned short b = tris[i].vertices[1];
			const unsigned short c = tris[i].vertices[2];
			if (a >= vcount || b >= vcount || c >= vcount) continue;
			aggIdx.push_back(baseVert + a);
			aggIdx.push_back(baseVert + b);
			aggIdx.push_back(baseVert + c);
		}
	}

	// _Claude_ Build the world-space VB+IB for one per-Land NiNode.
	// Called on cache miss only. Walks all subcells and trishapes
	// unconditionally (no per-shape frustum/appCulled filter) because the
	// result must be valid for any future camera angle. MSOC handles
	// view-frustum clipping internally during rasterisation, so the
	// "extra" triangles we include cost negligible compared to the
	// per-frame walk + matrix multiply they replace.
	static void buildLandCacheEntry(LandCacheEntry& entry, NI::Node* landNode) {
		entry.verts.clear();
		entry.indices.clear();
		const auto& subcells = landNode->children;
		for (size_t j = 0; j < subcells.endIndex; ++j) {
			auto* sub = subcells.storage[j].get();
			if (!sub) continue;
			if (!sub->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) continue;
			auto* subNode = static_cast<NI::Node*>(sub);
			const auto& shapes = subNode->children;
			for (size_t k = 0; k < shapes.endIndex; ++k) {
				auto* shape = shapes.storage[k].get();
				if (!shape) continue;
				if (!shape->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) continue;
				if (hasTransparency(shape)) continue;
				appendTerrainShape(entry.verts, entry.indices, static_cast<NI::TriShape*>(shape));
			}
		}
		entry.triCount = static_cast<unsigned int>(entry.indices.size() / 3);
	}

	// _Claude_ Aggregate terrain rasteriser. Walks the WorldLandscape
	// subtree (root → per-Land NiNode → 16 subcell NiNodes → N NiTriShapes)
	// and submits one combined occluder per visible Land. Individual
	// 25v/32t patches fail the thin-axis gate; merging them gives the
	// hill/horizon silhouette that actually occludes distant architecture.
	//
	// Must run inside the isTopLevel block — after ClearBuffer and
	// uploadCameraTransform, before cullShowBody traversal so the depth
	// buffer has terrain by the time the drain tests leaves against it.
	//
	// Uses g_landCache to amortise the per-Land walk + vertex transform:
	// first-frame sighting builds the entry, subsequent frames pass the
	// cached pointers straight to RenderTriangles. Cache lifetime is
	// tied to the per-Land NiNode pointer — cell-change teardown frees
	// the old NiNode and a new one is allocated; mark-and-sweep evicts
	// the stale entry at frame end.
	static void rasterizeAggregateTerrain(NI::Camera* camera) {
		if (!g_worldLandscapeRoot) return;
		if (g_worldLandscapeRoot->getAppCulled()) return;

		ScopedUsAccumulator timer(g_aggregateTerrainUs);

		// Mark all cached entries unseen; we'll flip this for entries we
		// touch this frame and evict the remainder below.
		for (auto& kv : g_landCache) kv.second.seen = false;

		const auto& landChildren = g_worldLandscapeRoot->children;
		for (size_t i = 0; i < landChildren.endIndex; ++i) {
			auto* land = landChildren.storage[i].get();
			if (!land) continue;
			if (!land->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) continue;

			auto* landNode = static_cast<NI::Node*>(land);

			// Get-or-build the cached buffer. Keyed by NiNode pointer;
			// a cell-change realloc produces a new key and rebuilds.
			auto [it, inserted] = g_landCache.try_emplace(landNode);
			LandCacheEntry& entry = it->second;
			if (inserted) {
				buildLandCacheEntry(entry, landNode);
				++g_landCacheMisses;
			}
			else {
				++g_landCacheHits;
			}
			entry.seen = true;

			// Per-frame submit decisions still apply. appCulled can toggle
			// on the per-Land root (game-driven distance culling) and the
			// frustum test remains a cheap pre-filter before MSOC's own
			// clipping. Shape-level culling is intentionally omitted — any
			// filter applied during cache build would bake view-specific
			// state into a reusable buffer.
			if (land->getAppCulled()) continue;
			if (frustumCulledSphere(land, camera)) continue;
			if (entry.triCount == 0) continue;

			if (g_asyncThisFrame) {
				// Cached buffers outlive this submission (persistent in
				// g_landCache until evicted), so we pass their pointers
				// directly. MSOC queues work for worker threads which
				// consume it during the drain-phase Flush; by the time
				// we'd ever evict, that Flush has completed.
				ScopedUsAccumulator t(g_rasterizeTimeUs);
				g_threadpool->RenderTriangles(entry.verts.data(), entry.indices.data(),
					static_cast<int>(entry.triCount),
					::MaskedOcclusionCulling::BACKFACE_NONE,
					::MaskedOcclusionCulling::CLIP_PLANE_ALL);
			}
			else {
				ScopedUsAccumulator t(g_rasterizeTimeUs);
				g_msoc->RenderTriangles(entry.verts.data(), entry.indices.data(),
					static_cast<int>(entry.triCount), g_worldToClip,
					::MaskedOcclusionCulling::BACKFACE_NONE,
					::MaskedOcclusionCulling::CLIP_PLANE_ALL,
					::MaskedOcclusionCulling::VertexLayout(12, 4, 8));
			}

			++g_aggregateTerrainLands;
			g_aggregateTerrainTris += entry.triCount;
		}

		// Sweep: drop entries whose per-Land NiNode isn't a child of
		// WorldLandscape this frame. Safe to free now because
		// ClearBuffer() at top-level entry already flushed previous-
		// frame async work that referenced these pointers.
		for (auto it = g_landCache.begin(); it != g_landCache.end(); ) {
			if (!it->second.seen) {
				it = g_landCache.erase(it);
				++g_landCacheEvictions;
			}
			else {
				++it;
			}
		}
	}

	// Body of the engine's CullShow, with MSOC query + occluder rasterisation
	// wedged between the frustum test and the Display call. Replaces the
	// engine's function at 0x6EB480 completely — our detour JMPs here.
	// Behaviour matches the engine 1:1 when g_msocActive is false.
	static void __fastcall cullShowBody(NI::AVObject* self, void* /*edx*/, NI::Camera* camera) {
		if (g_msocActive) ++g_recursiveCalls;
		if (self->getAppCulled()) {
			if (g_msocActive) ++g_recursiveAppCulled;
			return;
		}

		// Hierarchical frustum test, mirroring the engine's loop at 0x6EB4B7.
		// setBits tracks bits WE flipped in usedCullingPlanesBitfield so we
		// can unflip them before returning (the engine's LABEL_10 cleanup).
		uint32_t setBits[4] = { 0, 0, 0, 0 };
		const int nPlanes = camera->countCullingPlanes;
		auto* mask = camera->usedCullingPlanesBitfield;

		auto restoreIgnoreBits = [&]() {
			for (int j = 0; j < nPlanes; ++j) {
				const uint32_t jbit = 1u << (j & 0x1F);
				if (jbit & setBits[j >> 5]) {
					mask[j >> 5] &= ~jbit;
				}
			}
		};

		const float boundRadius = self->worldBoundRadius;
		for (int i = nPlanes - 1; i >= 0; --i) {
			const uint32_t bit = 1u << (i & 0x1F);
			const int word = i >> 5;
			if ((bit & mask[word]) != 0) {
				continue;
			}
			const auto* plane = static_cast<const TES3::Vector4*>(camera->cullingPlanePtrs.storage[i]);
			const float d = plane->x * self->worldBoundOrigin.x
				+ plane->y * self->worldBoundOrigin.y
				+ plane->z * self->worldBoundOrigin.z
				- plane->w;
			if (d <= -boundRadius) {
				if (g_msocActive) ++g_recursiveFrustumCulled;
				restoreIgnoreBits();
				return;
			}
			if (d >= boundRadius) {
				mask[word] |= bit;
				setBits[word] |= bit;
			}
		}

		if (g_msocActive) {
			// Two-pass: every NiTriBasedGeom leaf (NiTriShape + NiTriStrips,
			// the bulk of Morrowind's decorative/static meshes) defers its
			// visibility test until after the main traversal has finished
			// populating the depth buffer. This eliminates the single-pass
			// false positive where a leaf is tested against an occluder its
			// parent or earlier sibling rasterised moments before (door
			// behind its own wall, leaf in front of its own branch) — by
			// the time the drain phase runs, every occluder that can
			// contribute is already in the buffer, so the test result only
			// depends on geometry and camera, not on scene-graph order.
			//
			// Large NiTriShapes still rasterise inline as occluders, but
			// *without* an own-visibility test — MSOC's "closest 1/w wins"
			// means rasterising a shape that's itself occluded just
			// overdraws tiles with farther depths (harmless, wastes a few
			// triangles). Removing the own-test breaks the cycle where a
			// shape could fail the test against its own sibling's
			// rasterisation and still contribute usable occluder depth.
			//
			// NiNodes are *not* MSOC-tested — only frustum-culled (above).
			// Testing them inline against a partially-populated depth buffer
			// was a major source of false positives: a ref's root NiNode
			// tested before the rest of the scene rasterised could fail
			// against an earlier sibling's occluders and suppress the whole
			// subtree. Skipping the NiNode test means every leaf that
			// survives frustum reaches the drain and is tested once against
			// the final depth buffer — correctness first, at the cost of
			// losing the "skip whole subtree" shortcut.
			const bool isGeom = self->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom);
			if (isGeom) {
				// Only opaque NiTriShapes rasterise as occluders. Alpha-
				// tested/blended geometry (fences, banners, tree leaves)
				// would fill the depth buffer across their whole quad and
				// falsely occlude things behind the transparent parts.
				//
				// _Claude_ When aggregate-terrain is on, skip per-patch
				// rasterise for terrain descendants — they're already in
				// the depth buffer as merged per-Land submissions, and
				// re-rasterising them is duplicate work (harmless depth-
				// wise but wastes triangles / threadpool queue slots).
				const bool skipAsAggregated = Configuration::OcclusionAggregateTerrain
					&& isLandscapeDescendant(self, g_worldLandscapeRoot);
				if (!skipAsAggregated
					&& boundRadius >= Configuration::OcclusionOccluderRadiusMin
					&& boundRadius <= Configuration::OcclusionOccluderRadiusMax
					&& self->isInstanceOfType(NI::RTTIStaticPtr::NiTriShape)) {
					if (hasTransparency(self)) {
						++g_skippedAlpha;
					}
					else {
						const auto& eye = camera->worldTransform.translation;
						if (rasterizeTriShape(static_cast<NI::TriShape*>(self), eye)) {
							++g_rasterizedAsOccluder;
							if (Configuration::DebugOcclusionTintOccluder) {
								tintEmissive(self, kTintOccluder);
							}
						}
					}
				}
				g_pendingDisplays.push_back({ self, camera });
				++g_deferred;
				restoreIgnoreBits();
				return;
			}
		}

		self->vTable.asAVObject->display(self, camera);

		restoreIgnoreBits();
	}

	// Drain the deferred-display queue built during the main traversal.
	// Each entry is re-tested against the now-complete depth buffer and
	// displayed if still visible. Must run before g_msocActive is cleared
	// so any counters it updates land in the current frame's log line.
	static void drainPendingDisplays() {
		ScopedUsAccumulator t(g_drainPhaseTimeUs);
		// Fast path: nothing rasterised this frame means the depth buffer
		// is still cleared, so every TestRect would return VISIBLE for
		// shapes in frustum. Skip the query loop entirely and just display
		// each deferred shape. Saves ~1 µs/entry plus the async Flush cost
		// (handled at the caller) on occluder-free frames — rare at
		// defaults, but common in tiny rooms / dense fog / or after a
		// knob tweak that rejects every candidate.
		if (g_rasterizedAsOccluder == 0) {
			for (const auto& p : g_pendingDisplays) {
				p.shape->vTable.asAVObject->display(p.shape, p.camera);
			}
			g_pendingDisplays.clear();
			return;
		}
		for (const auto& p : g_pendingDisplays) {
			// _Claude_ Skip occludee tests on terrain patches. Ground sits
			// under the camera and is effectively always visible; querying
			// it wastes depth-buffer bandwidth and test time. Ancestor walk
			// matches against DataHandler::worldLandscapeRoot captured at
			// frame entry. Still displayed — we only bypass the test.
			// Gated by Configuration flag for A/B profiling.
			if (Configuration::OcclusionSkipTerrainOccludees
				&& isLandscapeDescendant(p.shape, g_worldLandscapeRoot)) {
				++g_skippedTerrain;
				p.shape->vTable.asAVObject->display(p.shape, p.camera);
				continue;
			}
			// Sub-threshold shapes bypass TestRect — their NDC footprint is
			// too small for the culler to decide reliably and the test cost
			// isn't worth the per-frame overhead. They still flow through
			// the queue so display order matches the rest of the deferred
			// shapes (reordering these would change alpha-sort timing).
			if (p.shape->worldBoundRadius < Configuration::OcclusionOccludeeMinRadius) {
				++g_skippedTesteeTiny;
				p.shape->vTable.asAVObject->display(p.shape, p.camera);
				continue;
			}
			// _Claude_ Temporal coherence: reuse a recent TestRect verdict
			// when the shape hasn't moved and the cached entry is still
			// fresh. N=0 disables the fast path entirely so the cache is
			// harmless (no lookup overhead, map stays empty after prune).
			const unsigned int tcFrames = Configuration::OcclusionTemporalCoherenceFrames;
			::MaskedOcclusionCulling::CullingResult result;
			bool reused = false;
			if (tcFrames > 0) {
				auto it = g_drainCache.find(p.shape);
				if (it != g_drainCache.end()) {
					const auto& e = it->second;
					const auto& o = p.shape->worldBoundOrigin;
					const float r = p.shape->worldBoundRadius;
					// _Claude_ vtable verify: any pointer load before
					// the bounds compare. If the slot got freed and
					// reallocated for a different class, the cached
					// bounds compare would be reading the wrong fields
					// at the wrong offsets. Mismatch → miss → re-query.
					const void* curVtbl =
						*reinterpret_cast<void* const*>(p.shape);
					const bool sameClass = e.vtbl == curVtbl;
					// Bit-exact compare is intentional: static shapes
					// produce identical floats across frames, and any
					// animated/moved shape will fail this and re-query.
					const bool stationary = sameClass &&
						e.boundOriginX == o.x &&
						e.boundOriginY == o.y &&
						e.boundOriginZ == o.z &&
						e.boundRadius == r;
					const bool fresh =
						(g_frameCounter - e.lastQueryFrame) <= tcFrames;
					if (stationary && fresh) {
						result = e.result;
						reused = true;
						++g_drainCacheHits;
					}
				}
			}
			if (!reused) {
				++g_queryTested;
				result = testSphereVisible(
					p.shape->worldBoundOrigin, p.shape->worldBoundRadius);
				if (tcFrames > 0) {
					++g_drainCacheMisses;
					// _Claude_ Cache only OCCLUDED. VISIBLE and
					// VIEW_CULLED still call display() this frame,
					// which dereferences p.shape anyway — caching them
					// gains nothing on the hit path while widening the
					// stale-pointer attack surface.
					if (result == ::MaskedOcclusionCulling::OCCLUDED) {
						auto& e = g_drainCache[p.shape];
						e.result = result;
						e.lastQueryFrame = g_frameCounter;
						e.vtbl = *reinterpret_cast<void* const*>(p.shape);
						e.boundOriginX = p.shape->worldBoundOrigin.x;
						e.boundOriginY = p.shape->worldBoundOrigin.y;
						e.boundOriginZ = p.shape->worldBoundOrigin.z;
						e.boundRadius = p.shape->worldBoundRadius;
					}
				}
			}
			if (result == ::MaskedOcclusionCulling::OCCLUDED) {
				if (!reused) ++g_queryOccluded;
				// Tint-debug mode keeps the shape visible so the user can
				// see which meshes the culler wrongly hides. Production
				// mode (default) culls by skipping display.
				if (Configuration::DebugOcclusionTintOccluded) {
					tintEmissive(p.shape, kTintOccluded);
					p.shape->vTable.asAVObject->display(p.shape, p.camera);
				}
				continue;
			}
			if (result == ::MaskedOcclusionCulling::VIEW_CULLED) {
				if (!reused) ++g_queryViewCulled;
			}
			else if (Configuration::DebugOcclusionTintTested) {
				tintEmissive(p.shape, kTintTested);
			}
			p.shape->vTable.asAVObject->display(p.shape, p.camera);
		}
		g_pendingDisplays.clear();
	}

	// Function-level detour installed at NiAVObject::CullShow (0x6EB480).
	// Every direct caller in the engine — NiNode::Display, all 4
	// NiBSPNode::Display quadrants, NiSwitchNode::Display, NiCamera::Click —
	// now lands here (verified via IDA get_callers: exactly 7 direct
	// callers, no indirect dispatch). The top-level entry for the
	// main-scene worldCamera pass (g_inRenderMainScene set by
	// renderMainScene_wrapper, camera matches mainCamera, not already
	// inside an MSOC traversal) drives the per-frame MSOC setup/teardown.
	// Every other entry — recursive descent inside an active traversal,
	// non-main-camera Clicks like shadow/water/arm, Clicks from outside
	// renderMainScene (load screen, screenshots, etc.) — just runs the
	// body, matching the engine 1:1.
	static void __fastcall CullShow_detour(NI::AVObject* self, void* edx, NI::Camera* camera) {
		bool isTopLevel = false;
		TES3::Cell* activeCell = nullptr;
		if (!g_msocActive && g_inRenderMainScene) {
			auto wc = TES3::WorldController::get();
			NI::Camera* mainCamera = wc ? wc->worldCamera.cameraData.camera.get() : nullptr;
			if (camera == mainCamera) {
				// Scene-type gate: let users disable MSOC in interiors or
				// exteriors independently. Cheap flag bit test on the active
				// cell; falls back to exterior behaviour if currentCell isn't
				// yet populated (main-menu preview, load screen).
				auto dh = TES3::DataHandler::get();
				const bool isInterior = dh && dh->currentCell
					&& dh->currentCell->getIsInterior();
				const bool sceneEnabled = isInterior
					? Configuration::OcclusionEnableInterior
					: Configuration::OcclusionEnableExterior;
				if (sceneEnabled) {
					isTopLevel = true;
					// _Claude_ Capture landscape root for drain-loop skip.
					// Null on load screen / main menu (dh already null there).
					g_worldLandscapeRoot = dh->worldLandscapeRoot;
					// Forwarded to the isTopLevel block below for cell-change
					// cache invalidation. Stored only on active frames so a
					// disabled-scene span doesn't spuriously "change cell".
					activeCell = dh ? dh->currentCell : nullptr;
				}
				else {
					// Counter accumulates only on skipped frames; no
					// top-level fires then so the reset at the top of
					// an active frame zeroes it out. Visible only on
					// the first active frame after a skipped stretch
					// — this is intended, per Phase 2.2 semantics.
					++g_skippedSceneGate;
				}
			}
		}

		if (isTopLevel) {
			// Latch async mode once per frame so a mid-frame Lua toggle
			// can't split submissions between threadpool and direct MOC.
			g_asyncThisFrame = g_threadpool && Configuration::OcclusionAsyncOccluders;
			if (g_asyncThisFrame) {
				// Wake up the worker threads ~100us before the first
				// RenderTriangles call. ClearBuffer() does an implicit
				// Flush() which retires any stragglers from the previous
				// frame — safe to clear the arena once it returns.
				g_threadpool->WakeThreads();
				g_threadpool->ClearBuffer();
				g_asyncOccluderVerts.clear();
				g_asyncOccluderIndices.clear();
			}
			else {
				g_msoc->ClearBuffer();
			}
			// _Claude_ Cell-change cache invalidation. Must run AFTER the
			// ClearBuffer()/Flush above so the threadpool has finished
			// consuming any cached buffers still referenced by last
			// frame's queued work — only then is it safe to free them.
			// Both caches key off pointers that a cell load can free and
			// reuse (per-Land NiNode*, shape NI::AVObject*), so a wipe
			// here eliminates the stale-pointer hazard wholesale.
			if (activeCell != g_lastCell) {
				g_landCache.clear();
				g_drainCache.clear();
				g_lastCell = activeCell;
				++g_cellChanges;
			}
			g_recursiveCalls = 0;
			g_recursiveAppCulled = 0;
			g_recursiveFrustumCulled = 0;
			g_rasterizedAsOccluder = 0;
			g_occluderTriangles = 0;
			g_skippedInside = 0;
			g_skippedThin = 0;
			g_skippedAlpha = 0;
			g_queryTested = 0;
			g_queryOccluded = 0;
			g_queryViewCulled = 0;
			g_queryNearClip = 0;
			g_deferred = 0;
			g_inlineTested = 0;
			g_skippedTriCount = 0;
			g_skippedTesteeTiny = 0;
			g_skippedSceneGate = 0;
			g_skippedTerrain = 0;
			g_aggregateTerrainLands = 0;
			g_aggregateTerrainTris = 0;
			g_aggregateTerrainUs = 0;
			g_landCacheHits = 0;
			g_landCacheMisses = 0;
			g_landCacheEvictions = 0;
			g_drainCacheHits = 0;
			g_drainCacheMisses = 0;
			++g_frameCounter;
			// _Claude_ Age-prune the temporal drain cache so entries for
			// shapes we haven't seen in a while (unloaded cell, destroyed
			// reference) don't accumulate. Window is 2*N frames — any
			// entry older than that can't be reused anyway. When the
			// feature is disabled (N=0) the whole map is dropped so it
			// doesn't linger from a previous session.
			{
				const unsigned int tcFrames = Configuration::OcclusionTemporalCoherenceFrames;
				if (tcFrames == 0) {
					if (!g_drainCache.empty()) g_drainCache.clear();
				}
				else {
					const uint32_t maxAge = tcFrames * 2;
					for (auto it = g_drainCache.begin(); it != g_drainCache.end(); ) {
						if (g_frameCounter - it->second.lastQueryFrame > maxAge) {
							it = g_drainCache.erase(it);
						}
						else {
							++it;
						}
					}
				}
			}
			g_rasterizeTimeUs = 0;
			g_drainPhaseTimeUs = 0;
			g_asyncFlushTimeUs = 0;
			uploadCameraTransform(camera);
			if (g_asyncThisFrame) {
				// SetMatrix copies into the threadpool's state ring buffer;
				// must be called AFTER uploadCameraTransform populated
				// g_worldToClip. Passed once per frame (all submissions
				// share the same world-to-clip transform).
				g_threadpool->SetMatrix(g_worldToClip);
			}
			g_msocActive = true;

			// _Claude_ Aggregate terrain pass: submit merged per-Land
			// occluders before the main traversal so the depth buffer
			// has hill/horizon silhouettes by the time non-terrain
			// leaves reach the drain. Individual 25v/32t patches would
			// fail the thin-axis gate; merging them reclaims terrain
			// as a useful occluder. Gated for A/B profiling.
			if (Configuration::OcclusionAggregateTerrain) {
				rasterizeAggregateTerrain(camera);
			}
		}

		cullShowBody(self, edx, camera);

		if (isTopLevel) {
			if (g_asyncThisFrame && g_rasterizedAsOccluder > 0) {
				// Barrier: Flush() blocks until every queued RenderTriangles
				// is fully rasterised into the shared buffer. Only after
				// this return is it safe to TestRect (drainPendingDisplays
				// runs on the main thread but queries the shared buffer).
				// Skipped when nothing was queued — no work to wait for and
				// drainPendingDisplays takes the no-TestRect fast path.
				ScopedUsAccumulator t(g_asyncFlushTimeUs);
				g_threadpool->Flush();
			}
			drainPendingDisplays();
			if (g_asyncThisFrame) {
				// Put the workers back to low-overhead sleep between frames.
				// Next frame's top-level entry will WakeThreads again.
				g_threadpool->SuspendThreads();
				g_asyncThisFrame = false;
			}
			g_msocActive = false;

			// Cumulative counters survive across frames so rare OCCLUDED
			// events — scenes where one building sits squarely behind
			// another — show up even when the 300-frame sampling misses
			// them. Logged alongside per-frame counts so we can spot when
			// the ratio totalOccluded/totalTested is nonzero.
			static uint64_t totalTested = 0;
			static uint64_t totalOccluded = 0;
			static uint64_t totalViewCulled = 0;
			totalTested += g_queryTested;
			totalOccluded += g_queryOccluded;
			totalViewCulled += g_queryViewCulled;

			// _Claude_ Two independently-toggled log channels:
			//   - OcclusionLogAggregate: periodic 300-frame sample. Steady
			//     baseline of cumulative counters; useful for "is the
			//     culler doing anything" at a glance.
			//   - OcclusionLogPerFrame: any frame that produced an
			//     OCCLUDED verdict. Reconciliation channel for "I see
			//     culling but the counters say 0" — every culling event
			//     gets a line.
			// Both default off. Identical line format on both channels.
			const bool baselineTick = Configuration::OcclusionLogAggregate
				&& (g_frameCounter % 300) == 0;
			const bool hadOccluded = Configuration::OcclusionLogPerFrame
				&& g_queryOccluded > 0;
			if (baselineTick || hadOccluded) {
				log::getLog() << "MSOC: frame " << g_frameCounter
					<< " rasterized=" << g_rasterizedAsOccluder
					<< " occluderTris=" << g_occluderTriangles // _Claude_
					<< " queryOccluded=" << g_queryOccluded
					<< "/" << g_queryTested
					<< " viewCulled=" << g_queryViewCulled
					<< " nearClip=" << g_queryNearClip
					<< " deferred=" << g_deferred
					<< " inlineTested=" << g_inlineTested
					<< " recursive=" << g_recursiveCalls
					<< " appCulled=" << g_recursiveAppCulled
					<< " frustumCulled=" << g_recursiveFrustumCulled
					<< " insideSkipped=" << g_skippedInside
					<< " thinSkipped=" << g_skippedThin
					<< " alphaSkipped=" << g_skippedAlpha
					<< " triSkipped=" << g_skippedTriCount
					<< " tinySkipped=" << g_skippedTesteeTiny
					<< " terrainSkipped=" << g_skippedTerrain // _Claude_
					<< " aggTerrainLands=" << g_aggregateTerrainLands // _Claude_
					<< " aggTerrainTris=" << g_aggregateTerrainTris // _Claude_
					<< " aggTerrainUs=" << g_aggregateTerrainUs // _Claude_
					<< " landCacheHit=" << g_landCacheHits // _Claude_
					<< " landCacheMiss=" << g_landCacheMisses // _Claude_
					<< " landCacheEvict=" << g_landCacheEvictions // _Claude_
					<< " tcHit=" << g_drainCacheHits // _Claude_
					<< " tcMiss=" << g_drainCacheMisses // _Claude_
					<< " tcSize=" << g_drainCache.size() // _Claude_
					<< " cellChanges=" << g_cellChanges // _Claude_
					<< " sceneGateSkipped=" << g_skippedSceneGate
					<< " rasterizeUs=" << g_rasterizeTimeUs
					<< " drainUs=" << g_drainPhaseTimeUs
					<< " asyncFlushUs=" << g_asyncFlushTimeUs
					<< " tp=" << (g_threadpool ? 1 : 0) // _Claude_ threadpool liveness
					<< " cfgAsync=" << (Configuration::OcclusionAsyncOccluders ? 1 : 0) // _Claude_ JSON-propagated flag (async fires iff tp && cfgAsync)
					<< " cumul=" << totalOccluded << "/" << totalTested
					<< "(vc=" << totalViewCulled << ")" << std::endl;
			}
		}
	}

	// Wrapper for TES3Game_static::renderMainScene (0x41C400). Installed at
	// its 3 known call sites (renderNextFrame, takeScreenshot,
	// createSaveScreenshot; verified via IDA get_callers). Sets
	// g_inRenderMainScene while the function is on the stack so
	// CullShow_detour knows the per-frame main scene is active. The
	// wasActive save/restore makes this reentry-safe even though reentry
	// isn't expected — nesting would just run the inner call with the
	// flag already set, and the outer restore is a no-op.
	using RenderMainSceneFn = void(__cdecl*)();
	static const auto renderMainScene_original = reinterpret_cast<RenderMainSceneFn>(0x41C400);

	static void __cdecl renderMainScene_wrapper() {
		const bool wasActive = g_inRenderMainScene;
		g_inRenderMainScene = true;
		renderMainScene_original();
		g_inRenderMainScene = wasActive;

		// Tint-debug reset: the batched renderer reads material state at
		// draw time, which happens *inside* renderMainScene_original after
		// the cull pass has populated the render list. By the time we
		// return here, all draws for this frame are done, so we flip every
		// tracked clone back to its source's current emissive. Next frame's
		// classifications re-apply the tint. No allocations, no property-
		// list churn — just a field write per tracked shape.
		//
		// Runs unconditionally and is a no-op when g_tintClones is empty,
		// saving us from gating on the three flags (which may toggle mid-
		// frame if the user edits from Lua, leaving state inconsistent).
		resetFrameTints();
	}

	void installPatches() {
		auto& log = log::getLog();

		log << "MSOC: installPatches entered; Configuration::EnableMSOC="
			<< (Configuration::EnableMSOC ? "true" : "false")
			<< std::endl;

		if (!Configuration::EnableMSOC) {
			log << "MSOC: disabled via Configuration::EnableMSOC." << std::endl;
			return;
		}

		g_msoc = ::MaskedOcclusionCulling::Create();
		if (!g_msoc) {
			log << "MSOC: MaskedOcclusionCulling::Create() returned null; occlusion disabled." << std::endl;
			return;
		}
		g_msoc->SetResolution(kMsocWidth, kMsocHeight);
		g_msoc->SetNearClipPlane(kNearClipW);

		// Always create the threadpool, even when async is disabled by
		// default, so Configuration::OcclusionAsyncOccluders can be toggled
		// at runtime (e.g. via Lua) without a restart. Workers sleep at
		// near-zero CPU when suspended; the only cost of unused creation
		// is the initial thread spin-up (~1ms total) and the ring-buffer
		// allocation (one-time).
		unsigned int threadCount = Configuration::OcclusionThreadpoolThreadCount;
		if (threadCount == 0) {
			// Auto: leave 2 threads (one full core) for the main thread.
			const unsigned hw = std::thread::hardware_concurrency();
			threadCount = hw > 2 ? hw - 2 : 1;
		}
		// _Claude_ Safety gate: clamp threadCount to binCount unconditionally.
		// Intel's CullingThreadpool assigns work by bin — each thread owns a
		// distinct bin and work-steals the rest. When threadCount > binCount,
		// surplus threads contend for the same bin mutexes and advance
		// mRenderPtrs past valid job slots; workers end up reading a recycled
		// or uninitialised TriList.mPtr and crash inside RenderTrilist
		// (observed: 26 threads / 8 bins → MaskedOcclusionCullingCommon.inl:1966).
		// Applies to both auto (0) and manual values — if the user wants more
		// parallelism, they raise the bin counts, which lifts this ceiling.
		const unsigned int binCount = Configuration::OcclusionThreadpoolBinsW
			* Configuration::OcclusionThreadpoolBinsH;
		if (threadCount > binCount) {
			log << "MSOC: clamping threadCount from " << threadCount
				<< " to binCount=" << binCount
				<< " (threads > bins causes worker crash)." << std::endl;
			threadCount = binCount;
		}
		// _Claude_ maxJobs=64 (default is 32). Our per-frame submission count
		// runs 380-580 meshes, each below TRIS_PER_JOB so each maps to one
		// queue slot. The default 32 overcommits ~15-18x and the main thread
		// yields repeatedly on CanWrite(). 64 halves the stall rate for a
		// ~57 MB ring-buffer cost (~55k floats * binsW*binsH * maxJobs * 4B).
		constexpr unsigned int kMaxJobs = 64;
		g_threadpool = new ::CullingThreadpool(threadCount,
			Configuration::OcclusionThreadpoolBinsW,
			Configuration::OcclusionThreadpoolBinsH,
			kMaxJobs);
		g_threadpool->SetBuffer(g_msoc);
		g_threadpool->SetResolution(kMsocWidth, kMsocHeight);
		g_threadpool->SetNearClipPlane(kNearClipW);
		// Same tightly-packed float[3] layout the sync path uses.
		g_threadpool->SetVertexLayout(::MaskedOcclusionCulling::VertexLayout(12, 4, 8));
		// Park the workers until the first async frame wakes them.
		g_threadpool->SuspendThreads();

		// _Claude_ Log threadpool creation + live config so we can diagnose
		// "asyncFlushUs=0 every frame" from the MWSE.log without needing a
		// debugger. If cfgAsync=1 here but per-frame cfgAsync=0 later, Lua
		// flipped it; if tp=0 here, creation failed (OOM, worker spin-up).
		log << "MSOC: threadpool created; threads=" << threadCount
			<< " binsW=" << Configuration::OcclusionThreadpoolBinsW
			<< " binsH=" << Configuration::OcclusionThreadpoolBinsH
			<< " maxJobs=" << kMaxJobs
			<< " tp=" << (g_threadpool ? 1 : 0)
			<< " cfgAsync=" << (Configuration::OcclusionAsyncOccluders ? 1 : 0)
			<< std::endl;

		// Function-level detour at NiAVObject::CullShow. The 5-byte
		// prologue (sub esp,14h; push ebx; push esi) gets overwritten
		// with a JMP to our detour, which reimplements the body
		// end-to-end (no trampoline needed — we never call back into
		// 0x6EB480). This replaces the previous 7-call-site patch and
		// is equivalent in coverage: IDA get_callers shows exactly 7
		// direct callers, all Display methods + NiCamera::Click.
		genJumpUnprotected(0x6EB480, reinterpret_cast<DWORD>(CullShow_detour));

		// Call-site wrappers for renderMainScene. Three known callers
		// (IDA get_callers on 0x41C400): the per-frame path, screenshot
		// capture, save-thumbnail capture. Call-site wrapping avoids
		// the function-prologue trampoline problem — we need to
		// actually call the original — and the enforcement check
		// catches address drift on new game builds.
		unsigned renderMainSceneInstalled = 0;
		const DWORD wrapperAddr = reinterpret_cast<DWORD>(renderMainScene_wrapper);
		static const uintptr_t kRenderMainSceneCallSites[3] = {
			0x41C08E, // TES3Game::renderNextFrame
			0x42E655, // WorldControllerRenderTarget::takeScreenshot
			0x4B50FF, // TES3File_static::createSaveScreenshot
		};
		for (auto site : kRenderMainSceneCallSites) {
			if (genCallEnforced(site, 0x41C400, wrapperAddr)) {
				++renderMainSceneInstalled;
			}
			else {
				log << "MSOC: failed to wrap renderMainScene call at 0x"
					<< std::hex << site << std::dec << std::endl;
			}
		}

		log << "MSOC: CullShow detour installed at 0x6EB480; wrapped "
			<< renderMainSceneInstalled << " / 3 renderMainScene call sites ("
			<< kMsocWidth << "x" << kMsocHeight << " tile buffer)." << std::endl;
	}

}
