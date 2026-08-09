#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Class.h"
#include "TES3Reference.h"


namespace mwse {
	class xIsTrader : InstructionInterface_t {
	public:
		xIsTrader();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xIsTrader xIsTraderInstance;

	xIsTrader::xIsTrader() : mwse::InstructionInterface_t(OpCode::xIsTrader) {}

	float xIsTrader::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (!reference) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xIsTrader: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long npcServiceFlags = 0;
		long classServiceFlags = 0;

		// Get the gold based on the base record type.
		TES3::AIConfig* aiConfig = reference->baseObject->vTable.object->getAIConfig(reference->baseObject);
		if (aiConfig) {
			npcServiceFlags = aiConfig->merchantFlags & 0x000037FF;

			// Get the class flags.
			TES3::Class* npcClass = reference->baseObject->vTable.object->getClass(reference->baseObject);
			if (npcClass) {
				npcServiceFlags = npcClass->services & 0x000037FF;
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xIsTrader: Failed to get AI configuration for target." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(npcServiceFlags | classServiceFlags);

		return 0.0f;
	}
}
