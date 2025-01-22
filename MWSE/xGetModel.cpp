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
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xGetModel xGetModelInstance;

	xGetModel::xGetModel() : mwse::InstructionInterface_t(OpCode::xGetModel) {}

	float xGetModel::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get our parameter.
		long param = Stack::getInstance().popLong();

		const char* model = NULL;

		// If we were given a value, it's supposed to be a string, and we'll get a record by this ID.
		if (param) {
			// Get the record by id string.
			mwseString& id = virtualMachine.getString(param);
			const auto record = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Object>(id);
			if (record == NULL) {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetModel: No record found for id '" << id << "'." << std::endl;
				}
				mwse::Stack::getInstance().pushLong(0);
				return 0.0f;
			}
			model = record->vTable.object->getModelPath(record);
		}

		// If we were not given a value, we try to use the function's given reference.
		else {
			TES3::Reference* reference = virtualMachine.getReference();
			if (reference == NULL) {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					log::getLog() << "xGetModel: Invalid reference." << std::endl;
				}
				mwse::Stack::getInstance().pushLong(0);
				return 0.0f;
			}
			model = reference->baseObject->vTable.object->getModelPath(reference->baseObject);
		}

		// Push the model back to the stack.
		Stack::getInstance().pushString(model);

		return 0.0f;
	}
}
