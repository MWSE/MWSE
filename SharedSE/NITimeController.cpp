#include "NITimeController.h"

#include "ExceptionUtil.h"

#include <cstring>

namespace NI {
	TimeController_vTable::TimeController_vTable() {
#if defined(SE_NI_TIMECONTROLLER_VTBL_TEMPLATE) && SE_NI_TIMECONTROLLER_VTBL_TEMPLATE > 0
		// Copy the engine's TimeController vtable layout as a starting template;
		// derived MWSE controllers (e.g. CopyTransformController) overwrite
		// individual slots after this base ctor returns.
		memcpy_s(this, sizeof(TimeController_vTable), reinterpret_cast<void*>(SE_NI_TIMECONTROLLER_VTBL_TEMPLATE), sizeof(TimeController_vTable));
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
	void TimeController::ctor() {
		_ctor(this);
	}

	void TimeController::dtor() {
		_dtor(this);
	}
#else
	void TimeController::ctor() {
		throw not_implemented_exception();
	}

	void TimeController::dtor() {
		throw not_implemented_exception();
	}
#endif

	void TimeController::start(float time) {
		vTable.asController->start(this, time);
	}

	void TimeController::stop() {
		vTable.asController->stop(this);
	}

	void TimeController::update(float dt) {
		vTable.asController->update(this, dt);
	}

	void TimeController::setTarget(ObjectNET* target) {
		vTable.asController->setTarget(this, target);
	}

	float TimeController::computeScaledTime(float dt) {
		return vTable.asController->computeScaledTime(this, dt);
	}

	bool TimeController::targetIsRequiredType() const {
		return vTable.asController->targetIsRequiredType(this);
	}

	bool TimeController::getActive() const {
		return flags & TimeControllerFlags::Active;
	}

	void TimeController::setActive(bool active) {
		if (active) {
			flags |= TimeControllerFlags::Active;
		}
		else {
			flags &= ~TimeControllerFlags::Active;
		}
	}

	unsigned int TimeController::getAnimTimingType() const {
		return flags & TimeControllerFlags::AppTimingMask;
	}

	void TimeController::setAnimTimingType(unsigned int type) {
		flags = (flags & ~TimeControllerFlags::AppTimingMask) | (type & TimeControllerFlags::AppTimingMask);
	}

	unsigned int TimeController::getCycleType() const {
		return flags & TimeControllerFlags::CycleTypeMask;
	}

	void TimeController::setCycleType(unsigned int type) {
		flags = (flags & ~TimeControllerFlags::CycleTypeMask) | (type & TimeControllerFlags::CycleTypeMask);
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::TimeController)
#endif
