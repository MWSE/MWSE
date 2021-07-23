/************************************************************************
	
	xActivate.cpp - Copyright (c) 2008 The MWSE Project
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

#include "InstructionInterface.h"
#include "Stack.h"

#include "TES3Attachment.h"
#include "TES3Reference.h"

#include "ScriptUtil.h"

#include "MemoryUtil.h"

using namespace mwse;

namespace mwse
{
	class xActivate : mwse::InstructionInterface_t
	{
	public:
		xActivate();
		virtual float execute(VMExecuteInterface &virtualMachine);
		virtual void loadParameters(VMExecuteInterface &virtualMachine);
	};

	static xActivate xActivateInstance;

	xActivate::xActivate() : mwse::InstructionInterface_t(OpCode::xActivate) {}

	void xActivate::loadParameters(mwse::VMExecuteInterface &virtualMachine) {
	}

	float xActivate::execute(mwse::VMExecuteInterface &virtualMachine) {
		// Get potential target.
		long parameter = Stack::getInstance().popLong();

		// Verify that the script is called on a valid reference.
		TES3::Reference* reference = virtualMachine.getReference();
		if (reference == NULL) {
#if _DEBUG
			log::getLog() << __FUNCTION__ << ": Called on invalid reference." << std::endl;
#endif
			return 0.0f;
		}

		// Only certain types of objects can actually activate things without crashing.
		auto baseObjectType = reference->baseObject->objectType;
		if (baseObjectType != TES3::ObjectType::NPC && baseObjectType != TES3::ObjectType::Creature) {
#if _DEBUG
			log::getLog() << __FUNCTION__ << ": Called on non-NPC, non-creature reference.." << std::endl;
#endif
			return 0.0f;
		}

		// Determine if the target is a reference.
		TES3::Reference* target = NULL;
		try {
			TES3::Reference* potential = reinterpret_cast<TES3::Reference*>(parameter);
			if (potential && potential->objectType == TES3::ObjectType::Reference) {
				target = potential;
			}
		}
		catch (std::exception&) {
#if _DEBUG
			log::getLog() << __FUNCTION__ << ": Target reference could not be resolved." << std::endl;
#endif
			return 0.0f;
		}

		// Verify that the target reference was found.
		if (target == NULL) {
#if _DEBUG
			log::getLog() << __FUNCTION__ << ": Target reference is invalid." << std::endl;
#endif
			return 0.0f;
		}
		
		// Set up the attachment node.
		auto activator = static_cast<TES3::ActionAttachment*>(target->getAttachment(TES3::AttachmentType::Action));
		if (activator == NULL) {
			activator = tes3::malloc<TES3::ActionAttachment>();
			activator->type = TES3::AttachmentType::Action;
			activator->next = NULL;
			activator->reference = NULL;
			activator->flags = 1;

			target->insertAttachment(activator);
		}

		// Ensure that our flags and reference is accurate.
		activator->flags = 2;
		activator->reference = reference;

		// Call the original activate function.
		TES3::Script* script = virtualMachine.getScript();
		mwscript::Activate(script, target);
		Stack::getInstance().pushLong(true);

		return 0.0f;
	}
}