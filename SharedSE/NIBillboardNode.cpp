#include "NIBillboardNode.h"

#include "NICamera.h"
#include "ExceptionUtil.h"

namespace NI {
	const auto BillboardNode_vTable = reinterpret_cast<Node_vTable*>(0x746B00);

	BillboardNode::BillboardNode() : Node() {
		vTable.asNode = BillboardNode_vTable;
		savedTime = 0.0f;
		updateControllers = true;
		flags |= BillboardNodeFlags::RotateAboutUp;
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
