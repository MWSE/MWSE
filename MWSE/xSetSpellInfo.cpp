#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

namespace mwse {
	class xSetSpellInfo : InstructionInterface_t {
	public:
		xSetSpellInfo();
		float execute(VMExecuteInterface& virtualMachine) override;
	};

	static xSetSpellInfo xSetSpellInfoInstance;

	xSetSpellInfo::xSetSpellInfo() : mwse::InstructionInterface_t(OpCode::xSetSpellInfo) {}

	float xSetSpellInfo::execute(mwse::VMExecuteInterface& virtualMachine) {
		auto& stack = Stack::getInstance();
		// Get parameters.
		mwseString& spellId = virtualMachine.getString(stack.popLong());
		long nameId = stack.popLong();
		long type = stack.popLong();
		long cost = stack.popLong();
		long flags = stack.popLong();
		long origin = stack.popLong();

		// Validate spell type.
		if (type < TES3::SpellCastType::FirstCastType || type > TES3::SpellCastType::LastCastType) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSpellInfo: Spell type out of range: " << type << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Validate spell flags.
		if (flags < TES3::SpellFlag::NoSpellFlags || flags > TES3::SpellFlag::AllSpellFlags) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSpellInfo: Spell flags out of range: " << flags << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Validate spell origin.
		if (origin != 0 && (origin < TES3::SpellOrigin::FirstSpellOrigin || origin > TES3::SpellOrigin::LastSpellOrigin)) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSpellInfo: Spell origin out of range: " << origin << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Get spell data by id.
		const auto spell = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Spell>(spellId);;
		if (spell == nullptr) {
			if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
				mwse::log::getLog() << "xSetSpellInfo: Could not find spell of id '" << spellId << "'" << std::endl;
			}
			stack.pushLong(false);
			return 0.0f;
		}

		// Set spell name if one is provided.
		if (nameId) {
			mwseString& name = virtualMachine.getString(nameId);
			if (name.length() > 31) {
				if constexpr (DEBUG_MWSCRIPT_FUNCTIONS) {
					mwse::log::getLog() << "xSetSpellInfo: Given name must be 31 characters or less." << std::endl;
				}
				stack.pushLong(false);
				return 0.0f;
			}

			// Expand name length if needed.
			if (spell->name == nullptr) {
				spell->name = (char*)se::memory::_new(32);
			}
			else if (name.length() > strlen(spell->name)) {
				spell->name = reinterpret_cast<char*>(se::memory::realloc(spell->name, 32));
			}

			// Copy name over.
			strcpy(spell->name, name.c_str());
		}

		// Set cost.
		if (flags & TES3::SpellFlag::AutoCalc && !(spell->spellFlags & TES3::SpellFlag::AutoCalc)) {
			//! TODO: Recalculate spell cost.
		}
		else if (!(flags & TES3::SpellFlag::AutoCalc)) {
			spell->magickaCost = static_cast<unsigned short>(cost);
		}

		// Set other information.
		spell->castType = static_cast<TES3::SpellCastType::value_type>(type);
		spell->spellFlags = flags;
		if (origin != 0) {
			spell->objectFlags = (TES3::ObjectFlag::value_type)origin;
		}

		// Report success.
		stack.pushLong(true);

		return 0.0f;
	}
}
