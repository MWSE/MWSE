#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Class.h"

namespace mwse {
	class xModService : InstructionInterface_t {
	public:
		xModService();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xModService xModServiceInstance;

	xModService::xModService() : mwse::InstructionInterface_t(OpCode::xModService) {}

	float xModService::execute(mwse::VMExecuteInterface& virtualMachine) {
		if (mwse::Stack::getInstance().size() < 1) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModService: Function called with too few arguments." << std::endl;
			}
			return 0.0f;
		}

		unsigned long data = Stack::getInstance().popLong();

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModService: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get AI configuration.
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (!aiConfig) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModService: Called on non-NPC reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get the NPC's class.
		TES3::Class* classRecord = reference->baseObject->getClass();
		if (!classRecord) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xModService: Failed to obtain NPC's class." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long services = aiConfig->merchantFlags | classRecord->services;

		// Want to remove services.
		if (data < 0) {
			services = services & (~(0 - data));
		}
		// Want to add services.
		else {
			services = services | (data & TES3::ServiceFlag::AllServicesMask);
		}

		aiConfig->merchantFlags = services;

		return 0.0f;
	}
}
