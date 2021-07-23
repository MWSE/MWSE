/************************************************************************
	
	xGetSpell.cpp - Copyright (c) 2008 The MWSE Project
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
#include "TES3Util.h"

#include "TES3MobileNPC.h"
#include "TES3NPC.h"
#include "TES3Reference.h"
#include "TES3Spell.h"

using namespace mwse;

namespace mwse
{
	class xGetSpell : mwse::InstructionInterface_t
	{
	public:
		xGetSpell();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xGetSpell xGetSpellInstance;

	xGetSpell::xGetSpell() : mwse::InstructionInterface_t(OpCode::xGetSpell) {}

	void xGetSpell::loadParameters(mwse::VMExecuteInterface &virtualMachine) {}

	float xGetSpell::execute(mwse::VMExecuteInterface &virtualMachine)
	{
		short result = 0;

		// Get spell id from the stack.
		mwseString& spellId = virtualMachine.getString(mwse::Stack::getInstance().popLong());

		// Get reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			mwse::log::getLog() << "xGetSpell: Could not find reference." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(result);
			return 0.0f;
		}
		else if (reference->baseObject->objectType != TES3::ObjectType::NPC) {
#if _DEBUG
			mwse::log::getLog() << "xGetSpell: Target is not an NPC." << std::endl;
#endif
			mwse::Stack::getInstance().pushShort(result);
			return 0.0f;
		}

		TES3::NPC* npc = reinterpret_cast<TES3::NPCInstance*>(reference->baseObject)->baseNPC;
		TES3::IteratedList<TES3::Spell*>::Node* currentNode = npc->spellList.list.head;
		while (currentNode != NULL) {
			TES3::Spell* spell = currentNode->data;
			if (strcmp(spell->objectID, spellId.c_str()) == 0) {
				result = 1;
				break;
			}
			currentNode = currentNode->next;
		}

		mwse::Stack::getInstance().pushShort(result);

		return 0.0f;
	}
}