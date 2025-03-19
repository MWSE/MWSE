#include "NIBSAnimationNode.h"

namespace NI {
	const auto NI_BSAnimationNode_ctor = reinterpret_cast<void(__thiscall*)(BSAnimationNode*)>(0x6F1AC0);
	BSAnimationNode::BSAnimationNode() {
		NI_BSAnimationNode_ctor(this);
	}

	Pointer<BSAnimationNode> BSAnimationNode::create() {
		return new BSAnimationNode();
	}

	BSParticleNode::BSParticleNode() {
		NI_BSAnimationNode_ctor(this);
		flags |= BSParticleNode::AVObjectFlag::Follow;
		vTable.asObject = reinterpret_cast<Object_vTable*>(0x750D68);;
	}

	Pointer<BSParticleNode> BSParticleNode::create() {
		return new BSParticleNode();
	}

	bool BSParticleNode::getFollowMode() const {
		return (flags & BSParticleNode::AVObjectFlag::Follow) != 0;
	}

	void BSParticleNode::setFollowMode(bool enable) {
		if (enable) {
			flags |= BSParticleNode::AVObjectFlag::Follow;
		}
		else {
			flags &= ~BSParticleNode::AVObjectFlag::Follow;
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::BSAnimationNode)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::BSParticleNode)
