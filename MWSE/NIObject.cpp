#include "NIObject.h"

#include "NIDefines.h"

#include "NIRTTI.h"

#include <cstring>

namespace NI {
	RTTI * Object::getRunTimeTypeInformation() {
		return vTable.asObject->getRTTI(this);
	}

	Object * Object::createClone() {
		return reinterpret_cast<Object * (__thiscall *)(Object *)>(0x6E9910)(this);
	}

	void Object::release() {
		reinterpret_cast<void(__thiscall *)(Object *)>(0x404630)(this);
	}

	bool Object::isOfType(const RTTI* type) {
		return (vTable.asObject->getRTTI(this) == type);
	}

	bool Object::isInstanceOfType(const RTTI* type) {
		for (const RTTI* rtti = vTable.asObject->getRTTI(this); rtti; rtti = rtti->baseRTTI) {
			if (rtti == type) {
				return true;
			}
		}

		return false;
	}
}
