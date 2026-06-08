#include "NiBSAnimationManager.h"

#include "NIBound.h"

namespace NI {

	namespace {
		constexpr auto NI_Node_Flags_ContainsRenderableGeom = 0x8;

		const auto NI_Node_AttachChild = reinterpret_cast<void(__thiscall*)(Node*, AVObject*, bool)>(SE_NI_NODE_FNADDR_ATTACHCHILD);
		const auto NI_Node_SetChildAt = reinterpret_cast<Pointer<AVObject>* (__thiscall*)(Node*, Pointer<AVObject>*, unsigned int, AVObject*)>(SE_NI_NODE_FNADDR_SETCHILDAT);

		Bound* getAVObjectWorldBound(AVObject* object) {
			return reinterpret_cast<Bound*>(&object->worldBoundOrigin);
		}
	}

	void BSAnimationManager::growWorldBoundFromChild(AVObject* child) {
		if (child == nullptr || child->parentNode != this) {
			vTable.asAVObject->updateWorldBound(this);
			return;
		}

		getAVObjectWorldBound(this)->merge(*getAVObjectWorldBound(child));
		if (child->vTable.asAVObject->isVisualObject(child)) {
			flags |= NI_Node_Flags_ContainsRenderableGeom;
		}
	}

	void BSAnimationManager::attachChildConservative(AVObject* child, bool useFirstAvailable) {
		NI_Node_AttachChild(this, child, useFirstAvailable);
		if (child != nullptr) {
			growWorldBoundFromChild(child);
		}
	}

	bool BSAnimationManager::isExactType(const AVObject* object) {
		return object != nullptr && reinterpret_cast<DWORD>(object->vTable.asObject) == static_cast<DWORD>(NI::VirtualTableAddress::NiBSAnimationManager);
	}

	Pointer<AVObject> BSAnimationManager::setChildAtConservative(unsigned int index, AVObject* child) {
		Pointer<AVObject> result;
		NI_Node_SetChildAt(this, &result, index, child);
		if (child != nullptr) {
			growWorldBoundFromChild(child);
		}
		return result;
	}

}
