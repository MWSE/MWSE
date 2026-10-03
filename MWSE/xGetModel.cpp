#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Reference.h"

namespace mwse {
	class xGetModel : InstructionInterface_t {
	public:
		xGetModel();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xGetModel xGetModelInstance;

	xGetModel::xGetModel() : InstructionInterface_t(OpCode::xGetModel) {}

	float xGetModel::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get our parameter.
		long param = stack.popLong();

		const char* model = nullptr;

		// If we were given a value, it's supposed to be a string, and we'll get a record by this ID.
		if (param) {
			// Get the record by id string.
			mwseString& id = virtualMachine.getString(param);
			const auto record = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Object>(id);
			if (record == nullptr) {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetModel: No record found for id '" << id << "'." << std::endl;
				}
				stack.pushLong(0);
				return 0.0f;
			}
			model = record->getModelPath();
		}

		// If we were not given a value, we try to use the function's given reference.
		else {
			TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
			if (reference == nullptr) {
				stack.pushLong(0);
				return 0.0f;
			}
			model = reference->baseObject->getModelPath();
		}

		// Push the model back to the stack.
		stack.pushString(model);

		return 0.0f;
	}
}
