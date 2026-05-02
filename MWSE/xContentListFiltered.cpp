#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "TES3Util.h"

#include "TES3Actor.h"
#include "TES3Inventory.h"
#include "TES3Reference.h"


namespace mwse {
	class xContentListFiltered : InstructionInterface_t {
	public:
		xContentListFiltered();
		virtual float execute(VMExecuteInterface& virtualMachine);

	private:
		long getBitMaskForRecordType(long recordType);
		bool passesFilter(TES3::Object* record, long filter);

		enum FilterMask {
			FILTER_ACTI = 1,
			FILTER_ALCH = 2,
			FILTER_AMMO = 4,
			FILTER_APPA = 8,
			FILTER_ARMO = 16,
			FILTER_BODY = 32,
			FILTER_BOOK = 64,
			FILTER_CLOT = 128,
			FILTER_CONT = 256,
			FILTER_CREA = 512,
			FILTER_DOOR = 1024,
			FILTER_INGR = 2048,
			FILTER_LEVC = 4096,
			FILTER_LEVI = 8192,
			FILTER_LIGH = 16384,
			FILTER_LOCK = 32768,
			FILTER_MISC = 65536,
			FILTER_NPC = 131072,
			FILTER_PROB = 262144,
			FILTER_REPA = 524288,
			FILTER_STAT = 1048576,
			FILTER_WEAP = 2097152,
			FILTER_ENCH = 4194304
		};
	};

	static xContentListFiltered xContentListFilteredInstance;

	xContentListFiltered::xContentListFiltered() : mwse::InstructionInterface_t(OpCode::xContentListFiltered) {}

	float xContentListFiltered::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		TES3::IteratedList<TES3::ItemStack*>::Node* node = reinterpret_cast<TES3::IteratedList<TES3::ItemStack*>::Node*>(mwse::Stack::getInstance().popLong());
		long filter = mwse::Stack::getInstance().popLong();

		// If we're not filtering, abandon ship.
		if (filter == 0) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xContentListFiltered: No filter provided." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xContentListFiltered: Called on invalid reference." << std::endl;
			}
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushFloat(0.0f);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			mwse::Stack::getInstance().pushLong(0);
			return 0.0f;
		}

		if (!reference->baseObject->isActor()) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xContentListFiltered: Reference is not for an actor." << std::endl;
			}
			mwse::Stack::getInstance().pushFloat(0.0f);
			return 0.0f;
		}

		// Results.
		const char* id = NULL;
		long count = 0;
		long type = 0;
		long value = 0;
		float weight = 0;
		const char* name = NULL;
		TES3::IteratedList<TES3::ItemStack*>::Node* next = NULL;

		// If we aren't given a node, get the first one.
		if (node == NULL) {
			node = static_cast<TES3::Actor*>(reference->baseObject)->inventory.itemStacks.head;

			// Pass over any records that don't match the current filter.
			while (node && node->data && node->data->object && !passesFilter(node->data->object, filter)) {
				node = node->next;
			}
		}

		// Validate the node we've obtained.
		if (node && node->data && node->data->object) {
			TES3::Object* object = node->data->object;

			id = object->vTable.object->getObjectID(object);
			count = node->data->count;
			type = object->objectType;
			value = object->vTable.object->getValue(object);
			weight = object->vTable.object->getWeight(object);
			name = object->vTable.object->getName(object);

			// Get next node. Pass over any records that don't match the given filter.
			next = node->next;
			while (next && next->data && next->data->object && !passesFilter(next->data->object, filter)) {
				next = next->next;
			}
		}

		// Push values to the stack.
		mwse::Stack::getInstance().pushLong((long)next);
		mwse::Stack::getInstance().pushString(name);
		mwse::Stack::getInstance().pushFloat(weight);
		mwse::Stack::getInstance().pushLong(value);
		mwse::Stack::getInstance().pushLong(type);
		mwse::Stack::getInstance().pushLong(count);
		mwse::Stack::getInstance().pushString(id);

		return 0.0f;
	}

	long xContentListFiltered::getBitMaskForRecordType(long recordType) {
		switch (recordType) {
		case TES3::ObjectType::Activator: return FILTER_ACTI;
		case TES3::ObjectType::Alchemy: return FILTER_ALCH;
		case TES3::ObjectType::Ammo: return FILTER_AMMO;
		case TES3::ObjectType::Apparatus: return FILTER_APPA;
		case TES3::ObjectType::Armor: return FILTER_ARMO;
		case TES3::ObjectType::Bodypart: return FILTER_BODY;
		case TES3::ObjectType::Book: return FILTER_BOOK;
		case TES3::ObjectType::Clothing: return FILTER_CLOT;
		case TES3::ObjectType::Container: return FILTER_CONT;
		case TES3::ObjectType::Creature: return FILTER_CREA;
		case TES3::ObjectType::Door: return FILTER_DOOR;
		case TES3::ObjectType::Ingredient: return FILTER_INGR;
		case TES3::ObjectType::LeveledCreature: return FILTER_LEVC;
		case TES3::ObjectType::LeveledItem: return FILTER_LEVI;
		case TES3::ObjectType::Light: return FILTER_LIGH;
		case TES3::ObjectType::Lockpick: return FILTER_LOCK;
		case TES3::ObjectType::Misc: return FILTER_MISC;
		case TES3::ObjectType::NPC: return FILTER_NPC;
		case TES3::ObjectType::Probe: return FILTER_PROB;
		case TES3::ObjectType::Repair: return FILTER_REPA;
		case TES3::ObjectType::Static: return FILTER_STAT;
		case TES3::ObjectType::Weapon: return FILTER_WEAP;
		}

		return 0x0;
	}

	bool xContentListFiltered::passesFilter(TES3::Object* object, long filter) {
		// Filter by record type. Unless we're not filtering only by enchantment.
		if (filter != FILTER_ENCH && !(getBitMaskForRecordType(object->objectType) & filter)) {
			return false;
		}

		// If we're filtering by enchantment, verify that the record has one.
		if (filter & FILTER_ENCH) {
			try {
				if (object->vTable.object->getEnchantment(object) == NULL) {
					return false;
				}
			}
			catch (std::exception&) {
				return false;
			}
		}

		return true;
	}
}
