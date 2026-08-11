#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Class.h"


namespace mwse {
	class xIsProvider : InstructionInterface_t {
	public:
		xIsProvider();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xIsProvider xIsProviderInstance;

	xIsProvider::xIsProvider() : mwse::InstructionInterface_t(OpCode::xIsProvider) {}

	float xIsProvider::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xIsProvider: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		long npcServiceFlags = 0;
		long classServiceFlags = 0;
		
		using namespace TES3::ServiceFlag;
		constexpr auto providerMask = OffersSpells | OffersSpellmaking | OffersEnchanting | OffersRepairs;
		static_assert(providerMask == 0x38800);
		
		// Get the gold based on the base record type.
		TES3::AIConfig* aiConfig = reference->baseObject->getAIConfig();
		if (aiConfig) {
			npcServiceFlags = aiConfig->merchantFlags & providerMask;

			// Get the class flags.
			TES3::Class* npcClass = reference->baseObject->getClass();
			if (npcClass) {
				npcServiceFlags = npcClass->services & providerMask;
			}
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xIsProvider: Failed to get AI configuration for target." << std::endl;
			}
		}

		mwse::Stack::getInstance().pushLong(npcServiceFlags | classServiceFlags);

		return 0.0f;
	}
}
