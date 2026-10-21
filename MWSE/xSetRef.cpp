#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"

namespace mwse {
	class xSetRef : public InstructionInterface_t {
	public:
		xSetRef();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetRef xSetRefInstance;

	xSetRef::xSetRef() : InstructionInterface_t(OpCode::xSetRef) {}

	float xSetRef::execute(VMExecuteInterface& virtualMachine) {
		TES3::Reference* reference = reinterpret_cast<TES3::Reference*>(Stack::getInstance().popLong());
		virtualMachine.setReference(reference);
		return 0.0f;
	}
}
