#include "NILookAtController.h"

namespace NI {
	unsigned int LookAtController::getAxis() const {
		return (flags & LookAtControllerFlags::AxisMask) >> LookAtControllerFlags::AxisBit;
	}

	void LookAtController::setAxis(unsigned int axis) {
		auto f = axis << LookAtControllerFlags::AxisBit;
		flags = (flags & ~LookAtControllerFlags::AxisMask) | (f & LookAtControllerFlags::AxisMask);
	}

	bool LookAtController::getFlip() const {
		return flags & LookAtControllerFlags::FlipAxis;
	}

	void LookAtController::setFlip(bool flip) {
		if (flip) { flags |= LookAtControllerFlags::FlipAxis; }
		else       { flags &= ~LookAtControllerFlags::FlipAxis; }
	}

	AVObject* LookAtController::getLookAt() const {
		return lookAt;
	}

	void LookAtController::setLookAt(AVObject* lookAtObject) {
		lookAt = lookAtObject;
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::LookAtController)
#endif
