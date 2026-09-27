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
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetQuality xSetQualityInstance;

	xSetQuality::xSetQuality() : InstructionInterface_t(OpCode::xSetQuality) {}

	float xSetQuality::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		float value = stack.popFloat();

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			return 0.0f;
		}

		// Get record.
		TES3::BaseObject* record = reference->baseObject;
		if (record == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetQuality: No base record found." << std::endl;
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
				log::getLog() << "xSetQuality: Call on unsupported record type: " << recordType << std::endl;
			}
		}

		stack.pushLong(valueSet);

		return 0.0f;
	}
}
