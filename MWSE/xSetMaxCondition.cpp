#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3ItemData.h"
#include "TES3Reference.h"
#include "TES3Armor.h"
#include "TES3Weapon.h"
#include "TES3Lockpick.h"
#include "TES3Probe.h"
#include "TES3RepairTool.h"

namespace mwse {
	class xSetMaxCondition : InstructionInterface_t {
	public:
		xSetMaxCondition();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xSetMaxCondition xSetMaxConditionInstance;

	xSetMaxCondition::xSetMaxCondition() : mwse::InstructionInterface_t(OpCode::xSetMaxCondition) {}

	float xSetMaxCondition::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameter from the stack.
		int maxCondition = static_cast<int>(mwse::Stack::getInstance().popLong());
		bool success = false;

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetMaxCondition: No reference provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get the base object.
		TES3::BaseObject* object = reference->baseObject;
		if (object == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetMaxCondition: No object found for reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set the max condition based on object type.
		if (object->objectType == TES3::ObjectType::Armor) {
			reinterpret_cast<TES3::Armor*>(object)->maxCondition = maxCondition;
		}
		else if (object->objectType == TES3::ObjectType::Weapon) {
			reinterpret_cast<TES3::Weapon*>(object)->maxCondition = maxCondition;
		}
		else if (object->objectType == TES3::ObjectType::Lockpick) {
			reinterpret_cast<TES3::Lockpick*>(object)->maxCondition = maxCondition;
		}
		else if (object->objectType == TES3::ObjectType::Probe) {
			reinterpret_cast<TES3::Probe*>(object)->maxCondition = maxCondition;
		}
		else if (object->objectType == TES3::ObjectType::Repair) {
			reinterpret_cast<TES3::RepairTool*>(object)->maxCondition = maxCondition;
		}
		else {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetMaxCondition: Invalid object type: " << object->objectType << std::endl;
			}
			mwse::Stack::getInstance().pushLong(success);
			return 0.0f;
		}

		// If there's a variable node containing condition, and we need to change it, do so.
		auto varNode = reference->getAttachedItemData();
		if (varNode && varNode->condition > maxCondition) {
			varNode->condition = maxCondition;
			success = true;
		}

		// Push success state.
		mwse::Stack::getInstance().pushLong(success);

		return 0.0f;
	}
}
