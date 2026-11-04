#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetBaseAgi : InstructionInterface_t {
	public:
		xGetBaseAgi();
		float execute(VMExecuteInterface& virtualMachine) override;
	private:
		const float INVALID_VALUE = -1.0f;
	};

	static xGetBaseAgi xGetBaseAgiInstance;

	xGetBaseAgi::xGetBaseAgi() : InstructionInterface_t(OpCode::xGetBaseAgi) {}

	float xGetBaseAgi::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get the associated MACP record.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		auto mobileObject = reference->getAttachedMobileActor();
		if (mobileObject == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xGetBaseAgi: Could not find MACP record for reference." << std::endl;
			}
			stack.pushFloat(INVALID_VALUE);
			return 0.0f;
		}

		// Push the current value of that attribute.
		stack.pushFloat(mobileObject->attributes[TES3::Attribute::Agility].base);

		return 0.0f;
	}
}
