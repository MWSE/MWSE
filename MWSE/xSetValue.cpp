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


namespace mwse {
	class xSetValue : InstructionInterface_t {
	public:
		xSetValue();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetValue xSetValueInstance;

	xSetValue::xSetValue() : mwse::InstructionInterface_t(OpCode::xSetValue) {}

	float xSetValue::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter.
		long value = mwse::Stack::getInstance().popLong();
		bool setValue = false;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetValue: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get record.
		TES3::BaseObject* record = reference->baseObject;
		if (record == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetValue: No base record found." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set the value from the base record. We group records here by the same offset.
		TES3::ObjectType::ObjectType recordType = record->objectType;
		switch (recordType) {
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Weapon:
			reinterpret_cast<TES3::Book*>(record)->value = value;
			setValue = true;
			break;
		case TES3::ObjectType::Light:
			reinterpret_cast<TES3::Light*>(record)->value = value;
			setValue = true;
			break;
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
			reinterpret_cast<TES3::Lockpick*>(record)->value = value;
			setValue = true;
			break;
		case TES3::ObjectType::Armor:
			reinterpret_cast<TES3::Armor*>(record)->value = value;
			setValue = true;
			break;
		case TES3::ObjectType::Clothing:
			// Clothing has the same offset as armor, but it's a short rather than a long.
			reinterpret_cast<TES3::Clothing*>(record)->value = static_cast<unsigned short>(value);
			setValue = true;
			break;
		case TES3::ObjectType::Apparatus:
			reinterpret_cast<TES3::Apparatus*>(record)->value = value;
			setValue = true;
			break;
		case TES3::ObjectType::Misc:
		{
			reinterpret_cast<TES3::Misc*>(record)->value = value;
			setValue = true;
			break;
		}
		default:
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetValue: Call on invalid record type." << std::endl;
			}
			break;
		}

		mwse::Stack::getInstance().pushLong(setValue);

		return 0.0f;
	}
}
