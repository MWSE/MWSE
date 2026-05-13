#include "NIBillboardNode.h"

#include "NICamera.h"
#include "ExceptionUtil.h"

namespace NI {
	BillboardNode::BillboardNode() : Node() {
#if defined(SE_NI_BILLBOARDNODE_VTBL) && SE_NI_BILLBOARDNODE_VTBL > 0
		vTable.asNode = reinterpret_cast<Node_vTable*>(SE_NI_BILLBOARDNODE_VTBL);
		savedTime = 0.0f;
		updateControllers = true;
		flags |= BillboardNodeFlags::RotateAboutUp;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<BillboardNode> BillboardNode::create() {
		return new BillboardNode();
	}

#if defined(SE_NI_BILLBOARDNODE_FNADDR_ROTATETOCAMERA) && SE_NI_BILLBOARDNODE_FNADDR_ROTATETOCAMERA > 0
	void BillboardNode::rotateToCamera(Camera* camera) {
		reinterpret_cast<void(__thiscall*)(BillboardNode*, Camera*)>(SE_NI_BILLBOARDNODE_FNADDR_ROTATETOCAMERA)(this, camera);
	}
#else
	void BillboardNode::rotateToCamera(Camera*) { throw not_implemented_exception(); }
#endif

	unsigned int BillboardNode::getMode() const {
		return (flags & BillboardNodeFlags::Mask) >> BillboardNodeFlags::ModeBit;
	}

	void BillboardNode::setMode(unsigned int mode) {
		auto f = mode << BillboardNodeFlags::ModeBit;
		flags = (flags & ~BillboardNodeFlags::Mask) | (f & BillboardNodeFlags::Mask);
	}
}
