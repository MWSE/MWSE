#include "PatchNI.h"

#include "MemoryUtil.h"
#include "MWSEConfig.h"

#include "NIBSAnimationManager.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NIFlipController.h"
#include "NILinesData.h"
#include "NIPointLight.h"
#include "NISortAdjustNode.h"
#include "NITriShape.h"
#include "NITriShapeData.h"
#include "NIUVController.h"

namespace mwse::patch::ni {
	//
	// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix cloning with no accumulator.
	//

	const auto NI_SortAdjustNode_Display = reinterpret_cast<void(__thiscall*)(NI::SortAdjustNode*, NI::Camera*)>(0x6DE030);
	const auto NI_ClusterAccumulator_RegisterObject = reinterpret_cast<void(__thiscall*)(NI::Accumulator*, NI::AVObject*)>(0x6CF200);

	void __fastcall PatchNISortAdjustNodeDisplay(NI::SortAdjustNode* node, DWORD unused, NI::Camera* camera) {
		// Add extra sort adjust mode for accumulating a node instead of geom.
		auto accumulator = camera->renderer->accumulator.get();
		if (node->sortingMode == NI::SortAdjustMode::SORTING_ORDERED_SUBTREE_MWSE
			&& accumulator != nullptr
			&& accumulator->isInstanceOfType(NI::RTTIStaticPtr::NiAlphaAccumulator)) {
			NI_ClusterAccumulator_RegisterObject(accumulator, node);
		}
		else {
			NI_SortAdjustNode_Display(node, camera);
		}
	}

	NI::Object* __fastcall PatchNISortAdjustNodeCloneAccumulator(NI::Accumulator* accumulator) {
		// Only call createClone if accumulator exists.
		return accumulator ? accumulator->vTable.asObject->createClone(accumulator) : nullptr;
	}

	//
	// Patch: Optimize NiBound::Merge.
	//

	void __fastcall PatchNiBoundMerge(NI::Bound* bound, DWORD _, const NI::Bound* other) {
		bound->merge(*other);
	}

	//
	// Patch: Optimize NiBound::ComputeMinimalBound.
	//

	void __fastcall PatchNiBoundComputeMinimalBound(NI::Bound* bound, DWORD _, NI::TArray<const NI::Bound*>* others) {
		// Vanilla dereferences storage[0] unconditionally. For us that
		// is UB if the compiler can ever prove an empty array was here.
		if (others->getEndIndex() == 0) {
			return;
		}

		// We use a local accumulator here to avoid pointer aliasing.
		// Allows the compiler to accumulate directly into registers.
		NI::Bound accumulator = *others->storage[0];

		for (size_t i = 1; i < others->getEndIndex(); ++i) {
			accumulator.merge(*others->storage[i]);
		}

		*bound = accumulator;
	}

	//
	// Patch: Manually specialize NiNode::UpdateWorldBound implementation.
	// Why: The majority of calls to NiBound::Merge originate from NiNode.
	//

	static NI::Bound* getWorldBound(NI::AVObject* object) {
		return reinterpret_cast<NI::Bound*>(&object->worldBoundOrigin);
	}

	void __fastcall PatchNiNodeUpdateWorldBound(NI::AVObject* node) {
		auto* niNode = static_cast<NI::Node*>(node);
		auto* bound = getWorldBound(node);

		auto* storage = niNode->children.storage;
		auto endIndex = niNode->children.getEndIndex();

		// The original vanilla function has only one loop here, but we split it
		// intentionally because `isVisualObject` is an optimization barrier due
		// to being both dynamic dispatch and a call to foreign code in another
		// binary.

		// This loop can also break early, and in vast majority of cases it will
		// break on the 1st or 2nd iteration because most nodes do have visuals.

		bool hasVisual = false;

		for (size_t i = 0; i < endIndex; ++i) {
			auto* child = static_cast<NI::AVObject*>(storage[i]);
			if (child && child->vTable.asAVObject->isVisualObject(child)) {
				hasVisual = true;
				break;
			}
		}

		// Local accumulator to avoid aliasing and keep the bound in registers.
		NI::Bound accumulator;
		accumulator.center = node->worldTransform.translation;
		accumulator.radius = 0.01f;

		// This loop is all our own code, `accumulator.merge` is forced inline.
		for (size_t i = 0; i < endIndex; ++i) {
			auto* child = static_cast<NI::AVObject*>(storage[i]);
			if (child) {
				accumulator.merge(*getWorldBound(child));
			}
		}

		*bound = accumulator;

		const auto NiNodeFlags_ContainsRenderableGeom = 0x8;

		if (hasVisual) {
			node->flags |= NiNodeFlags_ContainsRenderableGeom;
		}
		else {
			node->flags &= ~NiNodeFlags_ContainsRenderableGeom;
		}
	}

	void __fastcall PatchNiBSAnimationManagerAttachChild(NI::BSAnimationManager* manager, DWORD _EDX_, NI::AVObject* child, bool useFirstAvailable) {
		const auto NI_Node_AttachChild = reinterpret_cast<void(__thiscall*)(NI::Node*, NI::AVObject*, bool)>(SE_NI_NODE_FNADDR_ATTACHCHILD);
		NI_Node_AttachChild(manager, child, useFirstAvailable);

		if (child != nullptr) {
			manager->growWorldBoundFromChild(child);
		}
	}

	NI::Pointer<NI::AVObject>* __fastcall PatchNiBSAnimationManagerSetChildAt(NI::BSAnimationManager* manager, DWORD _EDX_, NI::Pointer<NI::AVObject>* outPreviousChild, unsigned int index, NI::AVObject* child) {
		const auto NI_Node_SetChildAt = reinterpret_cast<NI::Pointer<NI::AVObject>*(__thiscall*)(NI::Node*, NI::Pointer<NI::AVObject>*, unsigned int, NI::AVObject*)>(SE_NI_NODE_FNADDR_SETCHILDAT);
		NI_Node_SetChildAt(manager, outPreviousChild, index, child);

		if (child != nullptr) {
			manager->growWorldBoundFromChild(child);
		}

		return outPreviousChild;
	}

	void __fastcall PatchNiAVObjectUpdate(NI::AVObject* object, DWORD _EDX_, float fTime, bool updateControllers, bool updateChildren) {
		object->vTable.asAVObject->updateDownwardPass(object, fTime, updateControllers, updateChildren);

		auto* changedBranch = object;
		auto* ancestor = object->parentNode;
		while (ancestor != nullptr) {
			auto* nextParent = ancestor->parentNode;
			if (NI::BSAnimationManager::isExactType(ancestor)) {
				auto* manager = static_cast<NI::BSAnimationManager*>(ancestor);
				manager->growWorldBoundFromChild(changedBranch);
			}
			else {
				ancestor->vTable.asAVObject->updateWorldBound(ancestor);
			}

			changedBranch = ancestor;
			ancestor = nextParent;
		}
	}

	//
	// Patch: Allow per-shape control of whether software or hardware skinning is used.
	//

	// Define a constant usable in inline asm.
	#define Const_SoftwareSkinningFlag 0x200
	static_assert(Const_SoftwareSkinningFlag == NI::TriShapeFlags::SoftwareSkinningFlag);

	__declspec(naked) void PatchNITriBasedGeom_Ctor1() {
		__asm {
			movzx eax, word ptr[esp + 0x1C]	// eax = zero extended triangleCount
			mov dword ptr[esi], 0x751268		// Set NiTriBasedGeom vtable
			mov[esi + 0x34], eax				// Initialize triangleCount and patchRenderFlags together
			nop
		}
	}
	const size_t PatchNITriBasedGeom_Ctor1_size = 0xF;

	__declspec(naked) void PatchNITriBasedGeom_Ctor2() {
		__asm {
			xor edx, edx
			mov[esi + 0x34], edx				// Initialize triangleCount and patchRenderFlags together
			nop
		}
	}
	const size_t PatchNITriBasedGeom_Ctor2_size = 0x6;

	const auto NI_TriBasedGeometry_CopyMembers = reinterpret_cast<void(__thiscall*)(NI::TriBasedGeometry*, NI::TriBasedGeometry*)>(0x6F15B0);
	void __fastcall PatchNITriShapeCopyMembers(NI::TriShape* _this, DWORD _EDX_, NI::TriShape* to) {
		NI_TriBasedGeometry_CopyMembers(_this, to);

		// Ensure that if the geometry data has been deep cloned, that the render flags are copied too.
		if (to->modelData != _this->modelData) {
			to->getModelData()->patchRenderFlags = _this->getModelData()->patchRenderFlags;
		}
	}

	__declspec(naked) void PatchNIDX8Renderer_RenderShape() {
		__asm {
			nop
			test word ptr[esi + 0x36], Const_SoftwareSkinningFlag	// Skip hardware skinning if patchRenderFlags matches SoftwareSkinningFlag
			__asm _emit 0x75 __asm _emit 0x19						// jnz short $ + 0x1B (assembler can't output short offsets correctly)
		}
	}
	const size_t PatchNIDX8Renderer_RenderShape_size = 0x8;

	//
	// Patch: Fix NiDX8TexturePass::setCTPipelineState always setting the base map
	// texture coordinate index to 0 instead of reading it from the NIF property.
	//
	// Bug at 0x6B42AC: `mov [edi+28h], ebp` hardcodes ebp (=0) into
	// NiDX8TextureStage::sourceTexcoordIndex (+0x28) for stage[0]. Every other
	// map type (detail, glow, dark, decal, bump, gloss) correctly reads
	// NiTexturingProperty::Map::texCoordSet (+0x10) and clamps it to
	// [0, textureSetCount-1]. The base map skips that read entirely.
	//
	// Fix: a 5-byte CALL replaces the buggy 3-byte mov plus the first 2 bytes of
	// the following `mov [edi+48h], ebp`. A single NOP covers the leftover byte
	// at 0x6B42B1. The hook reads baseMap->texCoordSet, clamps it, writes it to
	// stage[0].sourceTexcoordIndex, and replicates the clobbered [edi+48h]=0 write.
	//
	// Stack layout on entry (ecx/thiscall frame of 0x6B41B0):
	//   sub esp,38h + push ebx/ebp/esi/edi → 0x48 total frame
	//   [esp+0x2C] = NiTexturingProperty*  (var_1C in IDA; caller saved to stack)
	//   [esp+0x58] = textureSetCount       (arg_10, 5th explicit parameter)
	// After the CALL at 0x6B42AC pushes a return address (+4):
	//   [esp+0x30] = NiTexturingProperty*
	//   [esp+0x5C] = textureSetCount
	//
	// NiTexturingProperty::TArray<Map*> maps is at prop+0x1C.
	// TArray.storage (pointer to element array) is at TArray+0x4 → prop+0x20.
	// maps.storage[0] is the base Map*; Map::texCoordSet is at Map+0x10.
	//
	__declspec(naked) void PatchNiDX8TexturePass_BaseMapTexcoord() {
		__asm {
			// edi = NiDX8TextureStage* (stage[0], always valid at this call site)
			// ebp = 0

			// --- Fix: read baseMap->texCoordSet and clamp ---
			mov  eax, [esp + 0x30]    // eax = NiTexturingProperty* (caller's var_1C)
			mov  eax, [eax + 0x20]    // eax = maps.storage pointer (TArray.storage at prop+0x20)
			mov  eax, [eax]         // eax = maps.storage[0] = baseMap*
			test eax, eax
			jz   use_zero           // baseMap == nullptr → default to 0

			mov  eax, [eax + 0x10]    // eax = baseMap->texCoordSet

			// Clamp: if texCoordSet >= textureSetCount, use textureSetCount-1
			mov  ecx, [esp + 0x5C]    // ecx = textureSetCount (caller's arg_10)
			test ecx, ecx
			jz   use_zero           // textureSetCount == 0 → clamp to 0
			cmp  eax, ecx
			jb   write_index        // texCoordSet < textureSetCount → no clamp needed
			lea  eax, [ecx - 1]       // eax = textureSetCount - 1
			jmp  write_index

			use_zero :
			xor eax, eax

				write_index :
			mov[edi + 0x28], eax    // stage[0].sourceTexcoordIndex = texCoordSet

				// --- Replicate clobbered instruction: mov [edi+48h], ebp (= 0) ---
				mov[edi + 0x48], ebp

				ret
		}
	}

	//
	// Patch: Improve lights
	//

	const auto TES3_DynamicLightingTest = reinterpret_cast<void(__cdecl*)(NI::PointLight * light, NI::Node * node, int radius, int lightFlags, bool isLand, bool highPriority)>(0x4D2F40);
	static void __cdecl PatchDynamicLightingTest(NI::PointLight* light, NI::Node* node, int radius, int lightFlags, bool isLand, bool highPriority) {
		if (!Configuration::ReplaceLightSorting) {
			TES3_DynamicLightingTest(light, node, radius, lightFlags, isLand, highPriority);
			return;
		}

		if (light == nullptr || node == nullptr) {
			return;
		}

		// Store information about the light into the light itself. Because that's what Morrowind does.
		light->setFlag(highPriority, 3u);
		light->specular = { float(radius), float(radius), float(radius) };

		node->updatePointLight(light, isLand);
	}

	void install() {
		using se::memory::genCallEnforced;
		using se::memory::genCallUnprotected;
		using se::memory::overrideVirtualTableEnforced;
		//using se::memory::writeDoubleWordUnprotected;
		using se::memory::writeValueEnforced;
		using se::memory::genJumpEnforced;
		using se::memory::genJumpUnprotected;
		using se::memory::genNOPUnprotected;
		using se::memory::writePatchCodeUnprotected;
		// using se::memory::writeBytesUnprotected;
		using se::memory::writeDoubleWordEnforced;

		// Patch: Fix NiSwitchNode::UpdateWorldBound malfunctioning when using UpdateOnlyActive and a switchIndex of 0.
		writeValueEnforced<BYTE>(0x6D85B6, 0x7E, 0x7C);

		// Patch: Add deterministic subtree ordering mode to NiSortAdjustNode. Fix cloning with no accumulator.
		overrideVirtualTableEnforced(0x750580, 0x78, 0x6DE030, reinterpret_cast<DWORD>(PatchNISortAdjustNodeDisplay));
		genCallUnprotected(0x6DE21B, reinterpret_cast<DWORD>(PatchNISortAdjustNodeCloneAccumulator));

		// Patch: Optimize NiBound::Merge and NiBound::ComputeMinimalBound.
		genCallEnforced(0x6C8BF7, 0x6ED310, reinterpret_cast<DWORD>(PatchNiBoundMerge));
		genCallEnforced(0x6C8CEE, 0x6ED310, reinterpret_cast<DWORD>(PatchNiBoundMerge));
		genCallEnforced(0x6D152B, 0x6ED310, reinterpret_cast<DWORD>(PatchNiBoundMerge));
		genCallEnforced(0x71746A, 0x6ED310, reinterpret_cast<DWORD>(PatchNiBoundMerge));
		genCallEnforced(0x709EBB, 0x6F2610, reinterpret_cast<DWORD>(PatchNiBoundComputeMinimalBound));

		// Patch: Optimize UpdateWorldBound so mergeBound can be inlined.
		// These are all vtables whose updateWorldBound slot is the shared
		// NiNode::UpdateWorldBound. NiSwitchNode, NiFltAnimationNode, and
		// NiLODNode instead run their own updateWorldBound that calls 0x6C8C90,
		// so we redirect that internal call site (0x6D85F2) as well.
		auto updateWorldBound = reinterpret_cast<DWORD>(PatchNiNodeUpdateWorldBound);
		genJumpEnforced(0x6D85F2, 0x6C8C90, updateWorldBound);
		writeDoubleWordEnforced(0x74771C, 0x6C8C90, updateWorldBound); // BSMirroredNode
		writeDoubleWordEnforced(0x74A75C, 0x6C8C90, updateWorldBound); // RootCollisionNode
		writeDoubleWordEnforced(0x74F400, 0x6C8C90, updateWorldBound); // AvoidNode
		writeDoubleWordEnforced(0x74F4A8, 0x6C8C90, updateWorldBound); // NiCollisionSwitch
		writeDoubleWordEnforced(0x74FA58, 0x6C8C90, updateWorldBound); // NiNode
		writeDoubleWordEnforced(0x74FFC0, 0x6C8C90, updateWorldBound); // NiBSPNode
		writeDoubleWordEnforced(0x750610, 0x6C8C90, updateWorldBound); // NiSortAdjustNode
		writeDoubleWordEnforced(0x750C68, 0x6C8C90, updateWorldBound); // NiBSAnimationManager
		writeDoubleWordEnforced(0x750DF8, 0x6C8C90, updateWorldBound); // NiBSAnimationNode
		writeDoubleWordEnforced(0x7511C0, 0x6C8C90, updateWorldBound); // NiBSParticleNode

		// Patch: Make scene graph bounds updates incremental for NiBSAnimationManager.
		genJumpUnprotected(0x6EB000, reinterpret_cast<DWORD>(PatchNiAVObjectUpdate), 0x5);
		writeDoubleWordEnforced(0x750C6C, 0x6C8410, reinterpret_cast<DWORD>(PatchNiBSAnimationManagerAttachChild));
		writeDoubleWordEnforced(0x750C78, 0x6C8780, reinterpret_cast<DWORD>(PatchNiBSAnimationManagerSetChildAt));

		// Patch: Allow control of whether software or hardware skinning is used through TriShape flags.
		auto TriShape_linkObject = &NI::TriShape::linkObject;
		writePatchCodeUnprotected(0x6FF0A8, (BYTE*)&PatchNITriBasedGeom_Ctor1, PatchNITriBasedGeom_Ctor1_size);
		writePatchCodeUnprotected(0x6FF0F0, (BYTE*)&PatchNITriBasedGeom_Ctor2, PatchNITriBasedGeom_Ctor2_size);
		genCallEnforced(0x6E54C5, 0x6F15B0, reinterpret_cast<DWORD>(PatchNITriShapeCopyMembers));
		writePatchCodeUnprotected(0x6ACF1F, (BYTE*)&PatchNIDX8Renderer_RenderShape, PatchNIDX8Renderer_RenderShape_size);
		overrideVirtualTableEnforced(0x7508B0, offsetof(NI::TriShape_vTable, NI::TriShape_vTable::linkObject), 0x6E56D0, *reinterpret_cast<DWORD*>(&TriShape_linkObject));

		// Patch: Fix base map texture coordinate index hardcoded to 0 in NiDX8TexturePass::setCTPipelineState.
		// 0x6B42AC: `mov [edi+28h], ebp` — replace with CALL + NOP the leftover byte at 0x6B42B1.
		genNOPUnprotected(0x6B42B1, 1);
		genCallUnprotected(0x6B42AC, reinterpret_cast<DWORD>(PatchNiDX8TexturePass_BaseMapTexcoord));

		// Patch: Fix NiLinesData binary loading.
		auto NiLinesData_loadBinary = &NI::LinesData::loadBinary;
		overrideVirtualTableEnforced(0x7501E0, offsetof(NI::Object_vTable, loadBinary), 0x6DA410, *reinterpret_cast<DWORD*>(&NiLinesData_loadBinary));

		// Patch: Fix NiUVController losing its texture set on clone.
		auto UVController_clone = &NI::UVController::copy;
		genCallEnforced(0x722317, 0x722330, *reinterpret_cast<DWORD*>(&UVController_clone));

		// Patch: Add pick proxy behaviour to NiCollisionSwitch.
		auto CollisionSwitch_linkObject = &NI::CollisionSwitch::linkObject;
		auto CollisionSwitch_findIntersectons = &NI::CollisionSwitch::findIntersections;
		overrideVirtualTableEnforced(0x74F418, 0x10, 0x6D7100, *reinterpret_cast<DWORD*>(&CollisionSwitch_linkObject));
		overrideVirtualTableEnforced(0x74F418, 0x88, 0x6D6E10, *reinterpret_cast<DWORD*>(&CollisionSwitch_findIntersectons));

#if false
		// Patch: Update dynamic lights to implement custom light sorting.
		genCallEnforced(0x485B60, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2C9C, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2D04, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2D9F, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D2F10, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
		genCallEnforced(0x4D3350, 0x4D2F40, reinterpret_cast<DWORD>(PatchDynamicLightingTest));
#endif

		// Patch: Optimize renderer hash map lookups. Use `NiDX8RendererHashBuckets` buckets instead of 37.
		constexpr DWORD NiDX8RendererHashBuckets = 4093; // Prime, ~16KB per map.
		writeDoubleWordEnforced(0x6A9AED, 37, NiDX8RendererHashBuckets); // mov ebp, 25h
		writeDoubleWordEnforced(0x6A9AF2, 0x94, NiDX8RendererHashBuckets * 4); // push 94h (geometry buffers)
		writeDoubleWordEnforced(0x6A9B38, 0x94, NiDX8RendererHashBuckets * 4); // push 94h (skin partitions)
		writeDoubleWordEnforced(0x6A9B7E, 0x94, NiDX8RendererHashBuckets * 4); // push 94h (rendered textures)
		writeDoubleWordEnforced(0x6A9BC4, 0x94, NiDX8RendererHashBuckets * 4); // push 94h (rendered cubemaps)

		// Patch: Always clone scene graph nodes.
		writeValueEnforced(0x4EF9FB, BYTE(0x02), BYTE(0x00));

		// Patch: Always copy all NiExtraData on clone, instead of only the first NiStringExtraData.
		genJumpUnprotected(0x4E8295, 0x4E82BB);
		genJumpUnprotected(0x4E82C4, 0x4E82CE);

		// Patch: Improve raycast accuracy.
		auto NiTriBasedGeometry_FindIntersections = &NI::TriBasedGeometry::findIntersections;
		overrideVirtualTableEnforced(0x7508B0, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriShape
		overrideVirtualTableEnforced(0x750A00, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriStrips
		overrideVirtualTableEnforced(0x750CC0, offsetof(NI::TriBasedGeometry_vTable, findIntersections), 0x6F0350, *reinterpret_cast<DWORD*>(&NiTriBasedGeometry_FindIntersections)); // NiTriBasedGeometry
	}

	void installPostLua() {
		// Patch: Fix NiFlipController losing its affectedMap on clone.
		if (Configuration::PatchNiFlipController) {
			auto NiFlipController_clone = &NI::FlipController::copy;
			se::memory::genCallEnforced(0x715D26, DWORD(NI::FlipController::_copy), *reinterpret_cast<DWORD*>(&NiFlipController_clone));
		}
	}
}
