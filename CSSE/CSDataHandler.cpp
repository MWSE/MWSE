#include "CSDataHandler.h"

#include "CSLight.h"
#include "CSReference.h"

import NILight;

namespace se::cs {
	void DataHandler::updateLightingForNonLightReference(Reference* reference) {
		const auto DataHandler_updateLightingForNonLightReference = reinterpret_cast<void(__thiscall*)(DataHandler*, Reference*)>(0x403A99);
		DataHandler_updateLightingForNonLightReference(this, reference);
	}

	void DataHandler::updateAllLights() {
		const auto DataHandler_updateAllLights = reinterpret_cast<void(__thiscall*)(DataHandler*)>(0x401F5A);
		DataHandler_updateAllLights(this);
	}

	void DataHandler::updateLightingForReference(Reference* reference) {
		auto attachedLightData = reference->getLightAttachment();
		auto object = reference->baseObject;
		if (object->objectType == ObjectType::Light && attachedLightData) {
			auto baseObjectLight = static_cast<Light*>(object);
			if (currentInteriorCell) {
				attachedLightData->light->detachAllAffectedNodes();
				baseObjectLight->updateLightingData(attachedLightData->light, currentInteriorCell);
			}
			else {
				updateAllLights();
			}
		}
		else {
			updateLightingForNonLightReference(reference);
		}
	}
}
