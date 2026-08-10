#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Reference.h"
#include "TES3Book.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3Armor.h"
#include "TES3Clothing.h"
#include "TES3Apparatus.h"
#include "TES3Misc.h"
#include "TES3Container.h"


namespace mwse {
	class xSetWeight : InstructionInterface_t {
	public:
		xSetWeight();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetWeight xSetWeightInstance;

	xSetWeight::xSetWeight() : mwse::InstructionInterface_t(OpCode::xSetWeight) {}

	float xSetWeight::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		float weight = mwse::Stack::getInstance().popFloat();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetWeight: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get record.
		TES3::BaseObject* record = reference->baseObject;
		if (record == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetWeight: No base record found." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get the weight from the base record. We group records here by the same offset.
		bool setWeight = true;
		TES3::ObjectType::ObjectType recordType = record->objectType;
		switch (recordType) {
		case TES3::ObjectType::Misc:
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Weapon:
			reinterpret_cast<TES3::Book*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Light:
			reinterpret_cast<TES3::Light*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
			reinterpret_cast<TES3::Lockpick*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Armor:
			reinterpret_cast<TES3::Armor*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Clothing:
			// Clothing has the same offset as armor, but it's a short rather than a long.
			reinterpret_cast<TES3::Clothing*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Apparatus:
			reinterpret_cast<TES3::Apparatus*>(record)->weight = weight;
			break;
		case TES3::ObjectType::Container:
			reinterpret_cast<TES3::Container*>(record)->capacity = weight;
			break;
		default:
			setWeight = false;
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetWeight: Call on invalid record type." << std::endl;
			}
			break;
		}

		mwse::Stack::getInstance().pushLong(setWeight);

		return 0.0f;
	}
}
