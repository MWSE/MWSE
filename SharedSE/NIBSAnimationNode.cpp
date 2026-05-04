#include "NIBSAnimationNode.h"

#include "ExceptionUtil.h"

namespace NI {
#if defined(SE_NI_BSANIMATIONNODE_FNADDR_CTOR) && SE_NI_BSANIMATIONNODE_FNADDR_CTOR > 0
	BSAnimationNode::BSAnimationNode() {
		reinterpret_cast<void(__thiscall*)(BSAnimationNode*)>(SE_NI_BSANIMATIONNODE_FNADDR_CTOR)(this);
	}
#else
	BSAnimationNode::BSAnimationNode() { throw not_implemented_exception(); }
#endif

	Pointer<BSAnimationNode> BSAnimationNode::create() {
		return new BSAnimationNode();
	}

	BSParticleNode::BSParticleNode() {
		// Re-uses BSAnimationNode ctor then patches the vtable and sets Follow flag.
		BSAnimationNode::BSAnimationNode();
		flags |= BSParticleNode::AVObjectFlag::Follow;
		vTable.asObject = reinterpret_cast<Object_vTable*>(0x750D68);
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

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::BSAnimationNode)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::BSParticleNode)
#endif
