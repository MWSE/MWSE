#include "CSPhysicalObject.h"

#include "CSStatic.h"

#include "NIAVObject.h"
#include "NINode.h"
#include "NITransform.h"
#include "NIMatrix33.h"

namespace se::cs {
	bool PhysicalObject::canRotateOnAllAxes() const {
		switch (objectType) {
		case TES3::ObjectType::Creature:
		case TES3::ObjectType::LeveledCreature:
		case TES3::ObjectType::NPC:
			return false;
		case TES3::ObjectType::Static:
			if (this == Static::gDoorMarker::get()) {
				return false;
			}
		}
		return true;
	}

	void PhysicalObject::calculateBounds() {
		const auto PhysicalObject_calculateBounds = reinterpret_cast<void(__thiscall*)(PhysicalObject*)>(0x547E80);
		PhysicalObject_calculateBounds(this);
	}
}
