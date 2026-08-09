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
		constexpr auto traderMask = TES3::ServiceFlag::OffersBarteringMask | TES3::ServiceFlag::BartersEnchantedItems;
		static_assert(traderMask == 0x37FF);

		// Get the gold based on the base record type.
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			npcServiceFlags = aiConfig->merchantFlags & traderMask;

			// Get the class flags.
			TES3::Class* npcClass = reference->baseObject->getClass();
			if (npcClass) {
				npcServiceFlags = npcClass->services & traderMask;
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
