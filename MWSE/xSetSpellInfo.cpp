/************************************************************************
	
	xSetSpellInfo.cpp - Copyright (c) 2008 The MWSE Project
	https://github.com/MWSE/MWSE/

	This program is free software; you can redistribute it and/or
	modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation; either version 2
	of the License, or (at your option) any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

	You should have received a copy of the GNU General Public License
	along with this program; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

**************************************************************************/

#include "VMExecuteInterface.h"
#include "Stack.h"
#include "InstructionInterface.h"
#include "MemoryUtil.h"

#include "TES3DataHandler.h"
#include "TES3Spell.h"

using namespace mwse;

namespace mwse
{
	class xSetSpellInfo : mwse::InstructionInterface_t
	{
	public:
		xSetSpellInfo();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xSetSpellInfo xSetSpellInfoInstance;

	xSetSpellInfo::xSetSpellInfo() : mwse::InstructionInterface_t(OpCode::xSetSpellInfo) {}

	void xSetSpellInfo::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xSetSpellInfo::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		// Get parameters.
		mwseString& spellId = virtualMachine.getString(mwse::Stack::getInstance().popLong());
		long nameId = mwse::Stack::getInstance().popLong();
		long type = mwse::Stack::getInstance().popLong();
		long cost = mwse::Stack::getInstance().popLong();
		long flags = mwse::Stack::getInstance().popLong();
		long origin = mwse::Stack::getInstance().popLong();

		// Validate spell type.
		if (type < TES3::SpellCastType::FirstCastType || type > TES3::SpellCastType::LastCastType) {
#if _DEBUG
			mwse::log::getLog() << "xSetSpellInfo: Spell type out of range: " << type << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate spell flags.
		if (flags < TES3::SpellFlag::NoSpellFlags || flags > TES3::SpellFlag::AllSpellFlags) {
#if _DEBUG
			mwse::log::getLog() << "xSetSpellInfo: Spell flags out of range: " << flags << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Validate spell origin.
		if (origin != 0 && (origin < TES3::SpellOrigin::FirstSpellOrigin || origin > TES3::SpellOrigin::LastSpellOrigin)) {
#if _DEBUG
			mwse::log::getLog() << "xSetSpellInfo: Spell origin out of range: " << origin << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Get spell data by id.
		TES3::Spell* spell = TES3::DataHandler::get()->nonDynamicData->resolveObjectByType<TES3::Spell>(spellId, TES3::ObjectType::Spell);;
		if (spell == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xSetSpellInfo: Could not find spell of id '" << spellId << "'" << std::endl;
#endif
			mwse::Stack::getInstance().pushLong(false);
			return 0.0f;
		}

		// Set spell name if one is provided.
		if (nameId) {
			mwseString& name = virtualMachine.getString(nameId);
			if (name.length() > 31) {
#if _DEBUG
				mwse::log::getLog() << "xSetSpellInfo: Given name must be 31 characters or less." << std::endl;
#endif
				mwse::Stack::getInstance().pushLong(false);
				return 0.0f;
			}
			
			// Expand name length if needed.
			if (spell->name == nullptr) {
				spell->name = (char*)tes3::_new(32);
			}
			else if (name.length() > strlen(spell->name)) {
				spell->name = reinterpret_cast<char*>(tes3::realloc(spell->name, 32));
			}

			// Copy name over.
			strcpy(spell->name, name.c_str());
		}

		// Set cost.
		if (flags & TES3::SpellFlag::AutoCalc && !(spell->spellFlags & TES3::SpellFlag::AutoCalc)) {
			//! TODO: Recalculate spell cost.
		}
		else if (!(flags & TES3::SpellFlag::AutoCalc)) {
			spell->magickaCost = cost;
		}

		// Set other information.
		spell->castType = static_cast<TES3::SpellCastType::value_type>(type);
		spell->spellFlags = flags;
		if (origin != 0) {
			spell->objectFlags = (TES3::ObjectFlag::value_type)origin;
		}

		// Report success.
		mwse::Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}