#include "Patch_IndexedVertexBlending.h"

#include "Log.h"
#include "MemoryUtil.h"

#include "NISkinInstance.h"

#include <d3d8.h>
#include <algorithm>
#include <cstring>

namespace mwse::patch::IndexedVertexBlending {
	// Set to true by the cap-probe hook when the device supports indexed vertex
	// blending with MaxVertexBlendMatrixIndex >= 4. Informational / logging
	// companion to s_ivbActive — the runtime hooks gate on s_ivbActive, not
	// this flag directly.
	static bool s_deviceSupportsIVB = false;

	// Global palette budget selected at cap-probe time. Equal to
	// min(MaxVertexBlendMatrixIndex + 1, kPaletteCeiling). Used everywhere a
	// "bones per partition" value would go.
	static DWORD s_paletteSize = 0;

	// Master runtime gate. Raised to true from computeCountHWBones once the
	// device has reported adequate IVB capability (MaxVertexBlendMatrixIndex
	// >= 4). Stays false on capability-limited devices, in which case the
	// installed Hook B/C/D detours all fall through to their vanilla target
	// and the game's skinning pipeline is behaviourally unchanged. The gate
	// is flipped from Hook A rather than install() because Hook A runs at
	// device creation, which happens after installPatches().
	static bool s_ivbActive = false;

	// Upper bound we're willing to request from NiSkinPartition::MakePartitions.
	// D3D8 IVB indexes into WorldMatrix[0..255] (256 slots), but both
	// NiDX8Renderer::countHWBones and MakePartitions' bonesPerPartition are
	// `unsigned __int8`, so 255 is the largest representable value. Losing one
	// slot vs. the hardware maximum is a trivial trade.
	static constexpr DWORD kPaletteCeiling = 255;

	// Minimum MaxVertexBlendMatrixIndex we're willing to consider worth the
	// switch. Four-bone skinning is vanilla already; anything <4 means the
	// device actually lacks IVB and we must stay on geometry blending.
	static constexpr DWORD kMinIndexedCap = 4;

	// Verified against IDA NiDX8Renderer struct (size 1696):
	//   +0x024  d3dDevice    (IDirect3DDevice8*)
	// Used by Hook A for live caps logging and Hook D for render-state upload.
	static constexpr unsigned kOffset_NiDX8Renderer_d3dDevice = 0x24;

	//
	// Hook A: NiDX8Renderer::validateDeviceCaps @ 0x6ab960
	//
	// Vanilla, at 0x6aba2c..0x6aba32:
	//   mov edx, [eax+D3DCAPS8.MaxVertexBlendMatrices]      ; 6 bytes @ 0x6aba2c
	//   mov [ecx+NiDX8Renderer.countHWBones], edx           ; 6 bytes @ 0x6aba2c+6
	// EAX = D3DCAPS8*, ECX = NiDX8Renderer*.
	//
	// We detour only the first mov (6 bytes -> call rel32 + 1 nop) to a naked
	// stub that computes the desired bone count in EDX while preserving EAX
	// and ECX so the following `mov [ecx+countHWBones], edx` and `mov al,
	// [eax+...]` instructions continue to work.
	//
	DWORD __stdcall computeCountHWBones(void* thisRenderer, const D3DCAPS8* caps) {
		const DWORD indexedCap = caps->MaxVertexBlendMatrixIndex;
		const DWORD nonIndexedCap = caps->MaxVertexBlendMatrices;

		DWORD liveIndexedCap = indexedCap;
		DWORD liveNonIndexedCap = nonIndexedCap;
		bool haveLiveCaps = false;

		if (thisRenderer) {
			auto device = *reinterpret_cast<IDirect3DDevice8**>(reinterpret_cast<BYTE*>(thisRenderer) + kOffset_NiDX8Renderer_d3dDevice);
			if (device) {
				D3DCAPS8 liveCaps{};
				if (SUCCEEDED(device->GetDeviceCaps(&liveCaps))) {
					haveLiveCaps = true;
					liveIndexedCap = liveCaps.MaxVertexBlendMatrixIndex;
					liveNonIndexedCap = liveCaps.MaxVertexBlendMatrices;
				}
			}
		}

		const DWORD selectedIndexedCap = haveLiveCaps ? liveIndexedCap : indexedCap;
		const DWORD selectedNonIndexedCap = haveLiveCaps ? liveNonIndexedCap : nonIndexedCap;
		const char* selectedCapsSource = haveLiveCaps ? "live device" : "cached";

		if (selectedIndexedCap >= kMinIndexedCap) {
			s_deviceSupportsIVB = true;
			s_paletteSize = std::min<DWORD>(selectedIndexedCap + 1, kPaletteCeiling);
			s_ivbActive = true;
			log::getLog() << "[MWSE] IndexedVertexBlending: cached caps report indexed blending (MaxVertexBlendMatrixIndex=" << indexedCap << ", MaxVertexBlendMatrices=" << nonIndexedCap << ")"
				<< (haveLiveCaps
					? " and live device caps report (MaxVertexBlendMatrixIndex=" + std::to_string(liveIndexedCap) + ", MaxVertexBlendMatrices=" + std::to_string(liveNonIndexedCap) + ")"
					: std::string("; live device caps unavailable"))
				<< "; using " << selectedCapsSource << " caps with palette size " << s_paletteSize << "." << std::endl;
			return s_paletteSize;
		}

		s_deviceSupportsIVB = false;
		s_paletteSize = 0;
		s_ivbActive = false;
		log::getLog() << "[MWSE] IndexedVertexBlending: cached caps report MaxVertexBlendMatrixIndex=" << indexedCap << " and MaxVertexBlendMatrices=" << nonIndexedCap
			<< (haveLiveCaps
				? "; live device caps report MaxVertexBlendMatrixIndex=" + std::to_string(liveIndexedCap) + " and MaxVertexBlendMatrices=" + std::to_string(liveNonIndexedCap)
				: std::string("; live device caps unavailable"))
			<< "; using " << selectedCapsSource << " caps and falling back to geometry blending (countHWBones=" << selectedNonIndexedCap << ")." << std::endl;
		return selectedNonIndexedCap;
	}

	__declspec(naked) void validateDeviceCaps_stub() {
		// Replaces the 6-byte `mov edx, [eax+MaxVertexBlendMatrices]` at 0x6aba2c.
		// EAX = D3DCAPS8*, ECX = NiDX8Renderer*. Must preserve both and set EDX.
		__asm {
			push eax                        // preserve D3DCAPS8* for 0x6aba38's later read
			push ecx                        // preserve renderer this* for 0x6aba32's store
			push eax                        // arg2: D3DCAPS8*
			push ecx                        // arg1: NiDX8Renderer*
			call computeCountHWBones        // __stdcall pops args; returns in EAX
			mov edx, eax                    // propagate count into EDX (matches vanilla)
			pop ecx
			pop eax
			ret
		}
	}

	//
	// Hook B1/B2: NiSkinPartition::MakePartitions call sites at 0x6adedf
	//             (NiDX8Renderer::DrawSkinnedPrimitive) and 0x6b00c3
	//             (NiDX8Renderer::maybe_PrePackSkinnedGeometryBuffer).
	//
	// Verified signature at 0x6c78f0:
	//   bool __thiscall NiSkinPartition::MakePartitions(
	//       NiSkinPartition* this, NiTriBasedGeomData* geom, NiSkinData* skin,
	//       unsigned __int8 bonesPerPartition, unsigned __int8 bonesPerVertex)
	//
	// Vanilla call sites pass (countHWBones, countHWBones). bNeedPalette inside
	// MakePartitions is computed as (bonesPerPartition != bonesPerVertex), so
	// passing identical values requests non-palette mode where each vertex's
	// bones[] array holds global bone indices directly.
	//
	// We force palette mode (bonesPerPartition = s_paletteSize, bonesPerVertex = 4)
	// when IVB is active so each Partition still keeps a bonePalette[] we can read
	// at VB-pack time (Hook C). For a typical humanoid rig (< ~100 bones) the
	// partitioner collapses the mesh to one partition that fits in the palette.
	//
	// Calling convention trick: we express __thiscall as __fastcall with a dummy
	// EDX parameter. MSVC passes __fastcall's first two args in ECX/EDX and the
	// rest on the stack (callee-cleans) — identical stack layout to __thiscall
	// when the second fastcall arg is ignored. Lets us write the hook as a plain
	// free function without naked asm.
	//
	using MakePartitions_t = bool(__fastcall*)(NI::SkinPartition* thisPart, DWORD edx_unused, void* geom, void* skin, unsigned char bonesPerPartition, unsigned char bonesPerVertex);
	static const auto MakePartitions_vanilla = reinterpret_cast<MakePartitions_t>(0x6c78f0);

	bool __fastcall MakePartitions_hook(NI::SkinPartition* thisPart, DWORD /*edx_unused*/, void* geom, void* skin, unsigned char bonesPerPartition, unsigned char bonesPerVertex) {
		unsigned char bp = bonesPerPartition;
		unsigned char bv = bonesPerVertex;

		if (s_ivbActive) {
			bp = static_cast<unsigned char>(s_paletteSize);
			bv = 4;

			static bool loggedOnce = false;
			if (!loggedOnce) {
				loggedOnce = true;
				log::getLog() << "[MWSE] IndexedVertexBlending: forcing MakePartitions into palette mode (bonesPerPartition=" << static_cast<unsigned>(bp) << ", bonesPerVertex=" << static_cast<unsigned>(bv) << ")." << std::endl;
			}
		}

		return MakePartitions_vanilla(thisPart, 0, geom, skin, bp, bv);
	}

	//
	// Hook C: NiDX8VertexBufferManager::PackSkinnedVB @ 0x6be2b0
	//
	// Verified signature (all three call sites pass the full 9-arg list):
	//   IDirect3DVertexBuffer8* __thiscall NiDX8VertexBufferManager::PackSkinnedVB(
	//       NiDX8VertexBufferManager* this, NiGeometryData* geom, NiSkinInstance* skin,
	//       NI::SkinPartition::Partition* partition,
	//       IDirect3DVertexBuffer8* unused1, int* unused2, DWORD pool,
	//       int unused3, int* out_vertexStride, int* out_fvf);
	//
	// Vanilla branches on partition->numBones (values 1..4) to pick an XYZBn
	// vertex format and writes numBones-1 explicit weights per vertex with the
	// last weight implied (1 - sum) by the GPU. For palette-mode IVB we need:
	//   * vertex format D3DFVF_XYZB5 | D3DFVF_LASTBETA_UBYTE4 (+ NORMAL / DIFFUSE
	//     / TEXn as per geomData), stride 32 base (12 pos + 16 weights + 4 idx);
	//   * 4 explicit float weights per vertex (last NOT implicit in XYZB5);
	//   * 4 bytes of local bone indices packed into the LASTBETA dword, read
	//     from partition->bones and truncated short→byte (safe: values are
	//     local palette indices in [0, numBones) ≤ 255).
	//
	// Three call sites patched via genCallEnforced — see install(). Each funnels
	// through PackSkinnedVB_hook, which tail-calls PackSkinnedVB_vanilla when the
	// master gate is off, giving a free pass-through during incremental rollout.
	//
	// Addresses / offsets verified at IDA (2026-04-14):
	//   NiGeometryData::getTextureSet @ 0x6ef500 (__thiscall)
	//   NiCriticalSection::Lock       @ 0x693f00 (__thiscall)
	//   NiCriticalSection::Unlock     @ 0x693f10 (__thiscall)
	//   CriticalSection               @ 0x7dea78 (VB manager mutex)
	//   NiDX8VertexBufferManager.device @ +0x08  -> IDirect3DDevice8*
	//   NiGeometryData.textureSets    @ +0x0a  (unsigned short)
	//   NiGeometryData.vertices       @ +0x1c  (TES3::Vector3*)
	//   NiGeometryData.normals        @ +0x20  (TES3::Vector3*)
	//   NiGeometryData.colors         @ +0x24  (NiPackedColor*, 4 bytes each)
	//

	using GetTextureSet_t = void*(__fastcall*)(void* thisGeom, DWORD edx_unused, unsigned int index);
	static const auto GetTextureSet_fn = reinterpret_cast<GetTextureSet_t>(0x6ef500);

	using NiCS_Lock_t = void(__fastcall*)(void* cs, DWORD edx_unused, const char* name);
	using NiCS_Unlock_t = void(__fastcall*)(void* cs, DWORD edx_unused);
	static const auto NiCS_Lock_fn = reinterpret_cast<NiCS_Lock_t>(0x693f00);
	static const auto NiCS_Unlock_fn = reinterpret_cast<NiCS_Unlock_t>(0x693f10);
	static const auto VbmCriticalSection = reinterpret_cast<void*>(0x7dea78);

	using PackSkinnedVB_t = IDirect3DVertexBuffer8*(__fastcall*)(
		void* thisVbm, DWORD edx_unused,
		void* geomData, void* skinInstance,
		NI::SkinPartition::Partition* partition,
		IDirect3DVertexBuffer8* unused1, int* unused2,
		DWORD pool, int unused3, int* out_vertexStride, int* out_fvf);
	static const auto PackSkinnedVB_vanilla = reinterpret_cast<PackSkinnedVB_t>(0x6be2b0);

	IDirect3DVertexBuffer8* __fastcall PackSkinnedVB_hook(
		void* thisVbm, DWORD /*edx_unused*/,
		void* geomData, void* skinInstance,
		NI::SkinPartition::Partition* partition,
		IDirect3DVertexBuffer8* unused1, int* unused2,
		DWORD pool, int unused3, int* out_vertexStride, int* out_fvf)
	{
		if (!s_ivbActive) {
			return PackSkinnedVB_vanilla(thisVbm, 0, geomData, skinInstance, partition, unused1, unused2, pool, unused3, out_vertexStride, out_fvf);
		}

		auto geomBytes = reinterpret_cast<BYTE*>(geomData);
		const auto vertices_geom = *reinterpret_cast<const BYTE**>(geomBytes + 0x1c);
		const auto normals = *reinterpret_cast<const BYTE**>(geomBytes + 0x20);
		const auto colors = *reinterpret_cast<const DWORD**>(geomBytes + 0x24);
		DWORD textureSets = *reinterpret_cast<unsigned short*>(geomBytes + 0x0a);
		if (textureSets > 8) {
			textureSets = 8;
		}

		const DWORD numVertices = partition->numVertices;
		const DWORD numBonesPerVertex = partition->numBonesPerVertex;

		// XYZB5 | LASTBETA_UBYTE4 is 3 floats position + 4 floats weights + 4 bytes
		// indices. All four weights are explicit; GPU does NOT reconstruct a fourth
		// from (1 - sum) in this format, so we write the full weight vector verbatim
		// and zero-fill any slots the partition doesn't use.
		DWORD fvf = D3DFVF_XYZB5 | D3DFVF_LASTBETA_UBYTE4;
		DWORD stride = 12 + 16 + 4;

		DWORD normalOffset = 0;
		if (normals) {
			fvf |= D3DFVF_NORMAL;
			normalOffset = stride;
			stride += 12;
		}

		DWORD colorOffset = 0;
		if (colors) {
			fvf |= D3DFVF_DIFFUSE;
			colorOffset = stride;
			stride += 4;
		}

		DWORD texOffset = stride;
		if (textureSets) {
			fvf |= (textureSets << D3DFVF_TEXCOUNT_SHIFT);
			stride += 8 * textureSets;
		}
		else {
			fvf |= D3DFVF_TEX1;
			stride += 8;
		}

		IDirect3DDevice8* device = *reinterpret_cast<IDirect3DDevice8**>(reinterpret_cast<BYTE*>(thisVbm) + 0x08);
		IDirect3DVertexBuffer8* vb = nullptr;

		NiCS_Lock_fn(VbmCriticalSection, 0, "NIDX8VERTEXBUF");
		HRESULT hr = device->CreateVertexBuffer(stride * numVertices, 0, fvf, static_cast<D3DPOOL>(pool), &vb);
		NiCS_Unlock_fn(VbmCriticalSection, 0);

		if (FAILED(hr) || !vb) {
			return nullptr;
		}

		BYTE* lockMem = nullptr;
		if (FAILED(vb->Lock(0, 0, &lockMem, 0))) {
			return nullptr;
		}

		// Position + 4 weights + 4 index bytes per vertex.
		for (DWORD i = 0; i < numVertices; ++i) {
			BYTE* vp = lockMem + i * stride;
			const unsigned short vIdx = partition->vertices[i];

			std::memcpy(vp, vertices_geom + vIdx * 12, 12);

			float* outWeights = reinterpret_cast<float*>(vp + 12);
			const float* srcWeights = partition->weights + i * numBonesPerVertex;
			for (DWORD b = 0; b < 4; ++b) {
				outWeights[b] = (b < numBonesPerVertex) ? srcWeights[b] : 0.0f;
			}

			BYTE* outIdx = vp + 28;
			const unsigned short* srcBones = partition->bones + i * numBonesPerVertex;
			for (DWORD b = 0; b < 4; ++b) {
				outIdx[b] = (b < numBonesPerVertex) ? static_cast<BYTE>(srcBones[b] & 0xFF) : 0;
			}
		}

		if (normals) {
			for (DWORD i = 0; i < numVertices; ++i) {
				const unsigned short vIdx = partition->vertices[i];
				std::memcpy(lockMem + i * stride + normalOffset, normals + vIdx * 12, 12);
			}
		}

		if (colors) {
			for (DWORD i = 0; i < numVertices; ++i) {
				const unsigned short vIdx = partition->vertices[i];
				*reinterpret_cast<DWORD*>(lockMem + i * stride + colorOffset) = colors[vIdx];
			}
		}

		for (DWORD ts = 0; ts < textureSets; ++ts) {
			const BYTE* texSet = static_cast<const BYTE*>(GetTextureSet_fn(geomData, 0, ts));
			BYTE* outBase = lockMem + texOffset + 8 * ts;
			for (DWORD i = 0; i < numVertices; ++i) {
				const unsigned short vIdx = partition->vertices[i];
				std::memcpy(outBase + i * stride, texSet + vIdx * 8, 8);
			}
		}

		vb->Unlock();

		if (out_vertexStride) {
			*out_vertexStride = stride;
		}
		if (out_fvf) {
			*out_fvf = static_cast<int>(fvf);
		}

		static bool loggedOnce = false;
		if (!loggedOnce) {
			loggedOnce = true;
			log::getLog() << "[MWSE] IndexedVertexBlending: first PackSkinnedVB in IVB mode (numVertices=" << numVertices << ", numBonesPerVertex=" << numBonesPerVertex << ", fvf=0x" << std::hex << fvf << std::dec << ", stride=" << stride << ")." << std::endl;
		}

		return vb;
	}

	//
	// Hook D: NiDX8Renderer::SetSkinnedModelTransforms @ 0x6acbe0
	//         (called from DrawSkinnedPrimitive2 inner loop at 0x6af188).
	//
	// Vanilla layout / behaviour:
	//   for (i = 0; i < partition->numBones; ++i) {
	//       globalBoneIndex = partition->bones[i];          // short, GLOBAL index
	//       matrix = skinInstance->bones[globalBoneIndex]->worldTransform
	//                * skinData->boneData[globalBoneIndex].invBind ...;
	//       SetBoneTransform(renderer, matrix, i);          // -> WorldMatrix[i]
	//   }
	// This is correct for non-palette MakePartitions output where `bones` IS the
	// partition's list of global bone indices (length numBones) and each vertex's
	// weights map positionally to WorldMatrix[0..numBones-1].
	//
	// In palette mode (which Hook B forces on when s_ivbActive) the same struct
	// fields take on different semantics:
	//   partition->bones       -> per-vertex LOCAL palette indices
	//                             (length numVertices * numBonesPerVertex)
	//   partition->bonePalette -> global bone index table
	//                             (unsigned __int8[numBones])
	// Reading `bones[0..numBones-1]` under this layout just samples the first few
	// vertices' local indices and treats them as global bone IDs — nonsense.
	//
	// Fix without rewriting any NiTransform math: build a temporary unsigned short
	// array from bonePalette[] and hot-swap partition->bones for the duration of
	// the vanilla call. The existing arithmetic then sees a familiar "global
	// indices at bones[i]" layout. After the matrix upload we restore bones and
	// raise D3DRS_INDEXEDVERTEXBLENDENABLE on the device so the
	// XYZB5 | LASTBETA_UBYTE4 stream emitted by Hook C actually uses byte indices
	// to look up WorldMatrix palette entries.
	//
	// D3DRS_VERTEXBLEND itself doesn't need adjusting: vertexBlendLookup[4] at
	// 0x7c3840 maps to D3DVBF_3WEIGHTS, which is also what IVB wants for
	// numBonesPerVertex=4 (four weights, four indices). Vanilla's SetRenderState
	// call at 0x6af16d handles that path unchanged.
	//
	// No symmetric "reset IVB" hook is needed. D3DRS_INDEXEDVERTEXBLENDENABLE
	// is only sampled by D3D8 when the FVF contains D3DFVF_LASTBETA_UBYTE4,
	// which is set exclusively by Hook C for skinned streams. Non-skinned
	// draws never carry that FVF bit, so leaving IVB=TRUE between skinned
	// batches is a true no-op for them. D3D8 also dedupes identical
	// SetRenderState calls, making the per-partition raise essentially free.
	//
	using SetSkinnedModelTransforms_t = void(__fastcall*)(void* thisRenderer, DWORD edx_unused, void* skinInstance, NI::SkinPartition::Partition* partition, void* transform, void* bound);
	static const auto SetSkinnedModelTransforms_vanilla = reinterpret_cast<SetSkinnedModelTransforms_t>(0x6acbe0);

	// Verified against IDA NiDX8Renderer struct (size 1696):
	//   +0x548  renderState  (NiDX8RenderState*)  -- unused here; we go direct to device.

	void __fastcall SetSkinnedModelTransforms_hook(void* thisRenderer, DWORD /*edx_unused*/, void* skinInstance, NI::SkinPartition::Partition* partition, void* transform, void* bound) {
		if (!s_ivbActive || !partition || partition->numBones == 0 || !partition->bonePalette) {
			SetSkinnedModelTransforms_vanilla(thisRenderer, 0, skinInstance, partition, transform, bound);
			return;
		}

		// kPaletteCeiling (255) caps the per-partition bone count so the temporary
		// array always fits on the stack.
		unsigned short fakeBones[kPaletteCeiling];
		const unsigned short numBones = partition->numBones;
		for (unsigned short i = 0; i < numBones; ++i) {
			fakeBones[i] = partition->bonePalette[i];
		}

		unsigned short* savedBones = partition->bones;
		partition->bones = fakeBones;
		SetSkinnedModelTransforms_vanilla(thisRenderer, 0, skinInstance, partition, transform, bound);
		partition->bones = savedBones;

		IDirect3DDevice8* device = *reinterpret_cast<IDirect3DDevice8**>(reinterpret_cast<BYTE*>(thisRenderer) + kOffset_NiDX8Renderer_d3dDevice);
		if (device) {
			// D3D8's runtime dedupes identical SetRenderState calls, so raising
			// this every partition is cheap. No matching reset is needed after
			// the skinned batch because non-skinned draws don't carry
			// D3DFVF_LASTBETA_UBYTE4 in their FVF and therefore ignore
			// D3DRS_INDEXEDVERTEXBLENDENABLE entirely.
			device->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, TRUE);
		}

		static bool loggedOnce = false;
		if (!loggedOnce) {
			loggedOnce = true;
			log::getLog() << "[MWSE] IndexedVertexBlending: first SetSkinnedModelTransforms in IVB mode (numBones=" << numBones << ")." << std::endl;
		}
	}

	void install() {
		// Hook A: cap-probe override. Replaces the 6-byte MaxVertexBlendMatrices
		// load at 0x6aba2c with a call that returns min(MaxVertexBlendMatrixIndex+1,
		// kPaletteCeiling) in EDX, preserving EAX/ECX for the surrounding reads.
		genCallUnprotected(0x6aba2c, reinterpret_cast<DWORD>(&validateDeviceCaps_stub), 6);

		// Hook B1/B2: intercept both MakePartitions call sites so we can force
		// palette mode with s_paletteSize bones when IVB is active. genCallEnforced
		// verifies the call at that address currently targets 0x6c78f0 before
		// patching; if Morrowind or another mod has moved it, the game asserts
		// instead of silently corrupting skinned geometry.
		genCallEnforced(0x6adedf, 0x6c78f0, reinterpret_cast<DWORD>(&MakePartitions_hook));
		genCallEnforced(0x6b00c3, 0x6c78f0, reinterpret_cast<DWORD>(&MakePartitions_hook));

		// Hook C: three call sites of NiDX8VertexBufferManager::PackSkinnedVB.
		// Two live in NiDX8Renderer::createPartitionGeomBuffer (the normal path
		// during skinned draw) and one in maybe_PrePackSkinnedGeometryBuffer
		// (hit during geometry precaching). All three funnel through the same
		// hook; behaviour differs only by whether s_ivbActive is set.
		genCallEnforced(0x6afb92, 0x6be2b0, reinterpret_cast<DWORD>(&PackSkinnedVB_hook));
		genCallEnforced(0x6afc46, 0x6be2b0, reinterpret_cast<DWORD>(&PackSkinnedVB_hook));
		genCallEnforced(0x6b01ae, 0x6be2b0, reinterpret_cast<DWORD>(&PackSkinnedVB_hook));

		// Hook D: redirect the single SetSkinnedModelTransforms call inside the
		// per-partition loop of DrawSkinnedPrimitive2. Hook swaps partition->bones
		// for a short[] built from bonePalette[] (palette-mode layout reads global
		// indices from bonePalette, not bones) before calling vanilla, and raises
		// D3DRS_INDEXEDVERTEXBLENDENABLE on the device so the GPU honours the
		// LASTBETA_UBYTE4 indices embedded in the stream by Hook C.
		genCallEnforced(0x6af188, 0x6acbe0, reinterpret_cast<DWORD>(&SetSkinnedModelTransforms_hook));
	}
}
