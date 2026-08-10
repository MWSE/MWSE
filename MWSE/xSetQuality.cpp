#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Reference.h"
#include "TES3Lockpick.h"
#include "TES3Probe.h"
#include "TES3RepairTool.h"
#include "TES3Apparatus.h"

namespace mwse {
	class xSetQuality : InstructionInterface_t {
	public:
		xSetQuality();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetQuality xSetQualityInstance;

	xSetQuality::xSetQuality() : mwse::InstructionInterface_t(OpCode::xSetQuality) {}

	float xSetQuality::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		float value = mwse::Stack::getInstance().popFloat();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetQuality: No reference provided." << std::endl;
			}
			return 0.0f;
		}

		// Get record.
		TES3::BaseObject* record = reference->baseObject;
		if (record == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetQuality: No base record found." << std::endl;
			}
			return 0.0f;
		}

		// Get the quality.
		bool valueSet = false;
		TES3::ObjectType::ObjectType recordType = record->objectType;
		if (recordType == TES3::ObjectType::Lockpick) {
			reinterpret_cast<TES3::Lockpick*>(reference->baseObject)->quality = value;
			valueSet = true;
		}
		else if (recordType == TES3::ObjectType::Probe) {
			reinterpret_cast<TES3::Probe*>(reference->baseObject)->quality = value;
			valueSet = true;
		}
		else if (recordType == TES3::ObjectType::Repair) {
			reinterpret_cast<TES3::RepairTool*>(reference->baseObject)->quality = value;
			valueSet = true;
		}
		else if (recordType == TES3::ObjectType::Apparatus) {
			reinterpret_cast<TES3::Apparatus*>(reference->baseObject)->quality = value;
			valueSet = true;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetQuality: Call on unsupported record type: " << recordType << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(valueSet);

		return 0.0f;
	}
}
