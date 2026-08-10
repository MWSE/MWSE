#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"


namespace mwse {
	class xGetValue : InstructionInterface_t {
	public:
		xGetValue();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetValue xGetValueInstance;

	xGetValue::xGetValue() : mwse::InstructionInterface_t(OpCode::xGetValue) {}

	float xGetValue::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetValue: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get value.
		long value = 0;
		try {
			// Get record.
			auto object = reference->baseObject;
			if (object == NULL) {
				throw std::exception("No base record found.");
			}

			// Get base value for the record.
			value = object->getValue();

			// Multiply the value by the count of the item.
			auto varNode = reference->getAttachedItemData();
			if (varNode) {
				value *= varNode->count;
			}
		}
		catch (std::exception& e) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xGetValue: " << e.what() << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		mwse::Stack::getInstance().pushLong(value);

		return 0.0f;
	}
}
