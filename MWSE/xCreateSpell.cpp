#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3Util.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

#include "LuaManager.h"
#include "LuaSpellCreatedEvent.h"

namespace mwse {
	class xCreateSpell : InstructionInterface_t {
	public:
		xCreateSpell();
		virtual float execute(VMExecuteInterface& virtualMachine);
	};

	static xCreateSpell xCreateSpellInstance;

	xCreateSpell::xCreateSpell() : mwse::InstructionInterface_t(OpCode::xCreateSpell) {}

	float xCreateSpell::execute(mwse::VMExecuteInterface& virtualMachine) {
		// Get parameters.
		mwseString& spellId = virtualMachine.getString(Stack::getInstance().popLong());
		mwseString& spellName = virtualMachine.getString(Stack::getInstance().popLong());

		// Verify spell Id length.
		if (spellId.length() > 31) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xCreateSpell: Spell id length of '" << spellId << "' is invalid. Must be 31 characters of less." << std::endl;
			}
			Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Verify spell Id length.
		if (spellName.length() > 31) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xCreateSpell: Spell name length of '" << spellName << "' is invalid. Must be 31 characters of less." << std::endl;
			}
			Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Verify that a spell of this id doesn't already exist.
		if (TES3::DataHandler::get()->nonDynamicData->getSpellById(spellId.c_str()) != NULL) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xCreateSpell: A spell of the given id '" << spellId << "' already exists." << std::endl;
			}
			Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get spell list.
		auto spellsList = TES3::DataHandler::get()->nonDynamicData->spellsList;
		auto spellListHead = *spellsList->begin();

		// Create new spell.
		auto newSpell = new TES3::Spell();
		newSpell->owningCollection.asSpellList = spellsList;
		newSpell->magickaCost = 1;

		// Set ID/name.
		newSpell->setID(spellId.c_str());
		newSpell->setName(spellName.c_str());

		// Set effects.
		for (auto i = 0; i < 8; ++i) {
			newSpell->effects[i].effectID = TES3::EffectID::None;
		}

		// Set the first effect, otherwise the game has issues.
		tes3::setEffect(newSpell->effects, 1, TES3::EffectID::WaterBreathing, TES3::SkillID::Invalid, int(TES3::EffectRange::Self), 0, 1, 0, 0);

		// Add object to the game.
		TES3::DataHandler::get()->nonDynamicData->addNewObject(newSpell);

		// Fire off spell created event.
		if (mwse::lua::event::SpellCreatedEvent::getEventEnabled()) {
			mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle().triggerEvent(new mwse::lua::event::SpellCreatedEvent(newSpell, "script"));
		}

		Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}
