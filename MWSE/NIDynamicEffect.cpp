#include "NIDynamicEffect.h"

namespace NI {
	const auto NI_DynamicEffect_ctor = reinterpret_cast<void(__thiscall*)(DynamicEffect*)>(0x6F34E0);
	DynamicEffect::DynamicEffect() {
		NI_DynamicEffect_ctor(this);
	}

	DynamicEffect::~DynamicEffect() {
		// Call dtor without deletion.
		vTable.asObject->destructor(this, 0);
	}

	int DynamicEffect::getType() const {
		return vTable.asDynamicEffect->getType(this);
	}

	bool DynamicEffect::isLight() const {
		return getType() <= TYPE_SPOT_LIGHT;
	}

	const auto NI_DynamicEffect_attachAffectedNode = reinterpret_cast<void(__thiscall*)(DynamicEffect*, Node*)>(0x6F3790);
	void DynamicEffect::attachAffectedNode(Node* node) {
		NI_DynamicEffect_attachAffectedNode(this, node);
	}

	const auto NI_DynamicEffect_detachAffectedNode = reinterpret_cast<void(__thiscall*)(DynamicEffect*, Node*)>(0x6F37D0);
	void DynamicEffect::detachAffectedNode(Node* node) {
		NI_DynamicEffect_detachAffectedNode(this, node);
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::DynamicEffect)
