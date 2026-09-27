#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3Activator.h"
#include "TES3Apparatus.h"
#include "TES3Armor.h"
#include "TES3Container.h"
#include "TES3Door.h"
#include "TES3Light.h"
#include "TES3Lockpick.h"
#include "TES3NPC.h"
#include "TES3Reference.h"

namespace mwse {
	class xSetName : InstructionInterface_t {
	public:
		xSetName();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetName xSetNameInstance;

	xSetName::xSetName() : InstructionInterface_t(OpCode::xSetName) {}

	float xSetName::execute(VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameter from the stack.
		mwseString& name = virtualMachine.getString(stack.popLong());

		// Enforce name length.
		if (name.length() > 31) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetName: Given name length must be 31 characters or less." << std::endl;
			}
			stack.pushShort(false);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = getReference(virtualMachine, __FUNCTION__);
		if (reference == nullptr) {
			stack.pushShort(false);
			return 0.0f;
		}

		// Get the base record.
		TES3::BaseObject* recordGeneric = reference->baseObject;
		if (recordGeneric == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetName: No record found for reference." << std::endl;
			}
			stack.pushShort(false);
			return 0.0f;
		}

		// Get string pointer based on record type.
		char* namePtr = nullptr;
		char** nameContainer = nullptr;
		TES3::ObjectType::ObjectType recordType = recordGeneric->objectType;
		switch (recordType) {
		case TES3::ObjectType::NPC:
		case TES3::ObjectType::Creature:
			nameContainer = &reinterpret_cast<TES3::NPCInstance*>(recordGeneric)->baseNPC->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Container:
			nameContainer = &reinterpret_cast<TES3::Container*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Light:
			nameContainer = &reinterpret_cast<TES3::Light*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Alchemy:
		case TES3::ObjectType::Ammo:
		case TES3::ObjectType::Armor:
		case TES3::ObjectType::Book:
		case TES3::ObjectType::Clothing:
		case TES3::ObjectType::Misc:
		case TES3::ObjectType::Weapon:
			nameContainer = &reinterpret_cast<TES3::Armor*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Activator:
			nameContainer = &reinterpret_cast<TES3::Activator*>(recordGeneric)->name;
			namePtr = *nameContainer;
			break;
		case TES3::ObjectType::Door:
			namePtr = reinterpret_cast<TES3::Door*>(recordGeneric)->name;
			break;
		case TES3::ObjectType::Apparatus:
			namePtr = reinterpret_cast<TES3::Apparatus*>(recordGeneric)->name;
			break;
		case TES3::ObjectType::Ingredient:
		case TES3::ObjectType::Lockpick:
		case TES3::ObjectType::Probe:
		case TES3::ObjectType::Repair:
			namePtr = reinterpret_cast<TES3::Lockpick*>(recordGeneric)->name;
			break;
		}

		// Bail out if we haven't found the name.
		if (namePtr == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				log::getLog() << "xSetName: Unsupported record format: " << recordType << "." << std::endl;
			}
			stack.pushShort(false);
			return 0.0f;
		}

		// Some strings might need to be recreated.
		if (nameContainer != nullptr && name.length() > strlen(namePtr)) {
			namePtr = reinterpret_cast<char*>(se::memory::realloc(namePtr, name.length() + 1));
			*nameContainer = namePtr;
		}

		strcpy(namePtr, name.c_str());

		stack.pushShort(true);
		return 0.0f;
	}
}
