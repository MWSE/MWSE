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
		// Re-runs the engine BSAnimationNode ctor on `this` (the implicit base
		// call has already done so once; the original code does it twice).
#if defined(SE_NI_BSANIMATIONNODE_FNADDR_CTOR) && SE_NI_BSANIMATIONNODE_FNADDR_CTOR > 0 && defined(SE_NI_BSPARTICLENODE_VTBL) && SE_NI_BSPARTICLENODE_VTBL > 0
		reinterpret_cast<void(__thiscall*)(BSAnimationNode*)>(SE_NI_BSANIMATIONNODE_FNADDR_CTOR)(this);
		flags |= BSParticleNode::AVObjectFlag::Follow;
		vTable.asObject = reinterpret_cast<Object_vTable*>(SE_NI_BSPARTICLENODE_VTBL);
#else
		throw not_implemented_exception();
#endif
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
