#include "NITimeController.h"

namespace NI {
	TimeController_vTable::TimeController_vTable() {
		// Copy everything from the original virtual table.
		memcpy_s(this, sizeof(TimeController_vTable), (void*)0x751200, sizeof(TimeController_vTable));
	}

	void TimeController::ctor() {
		_ctor(this);
	}

	void TimeController::dtor() {
		_dtor(this);
	}

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

	bool TimeController::targetIsRequiredType() {
		return vTable.asController->targetIsRequiredType(this);
	}

}
