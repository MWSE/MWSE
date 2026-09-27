#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Reference.h"

namespace mwse {
	class xRefType : InstructionInterface_t
	{
	public:
		xRefType();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xRefType xRefTypeInstance;

	xRefType::xRefType() : InstructionInterface_t(OpCode::xRefType) {}

	float xRefType::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		TES3::Reference* refr = virtualMachine.getReference();
		if (refr == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xRefType: Called on invalid reference." << std::endl;
			}
			stack.pushLong(0);
			return 0.0f;
		}

		TES3::BaseObject* temp = reinterpret_cast<TES3::BaseObject*>(refr->baseObject);

		long type = static_cast<long>(temp->objectType);

		stack.pushLong(type);

		return 0.0f;
	}
}
