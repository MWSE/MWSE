/************************************************************************
	
	ScriptUtil.h - Copyright (c) 2008 The MWSE Project
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

#pragma once

#include "mwOffsets.h"
#include "mwseString.h"
#include "OpCodes.h"

#include "TES3Defines.h"

#undef PlaySound

namespace mwse
{
	namespace mwscript
	{

		//
		// Manipulation of script state.
		//

		int getInstructionPointer();
		void setInstructionPointer(int IP);

		//
		// Manipulation of script variables.
		//

		TES3::ScriptVariables* getLocalScriptVariables();

		TES3::Reference* getScriptTargetReference();
		void setScriptTargetReference(TES3::Reference* reference);

		TES3::BaseObject* getScriptTargetTemplate();
		void setScriptTargetTemplate(TES3::BaseObject* record);

		TES3::BaseObject* getScriptSecondObject();
		void setScriptSecondObject(const char* string);
		void setScriptSecondObject(TES3::BaseObject* record);

		TES3::BaseObject* getDataBufferObject();
		void setDataBufferObject(TES3::BaseObject*);

		const char* getDataBufferString();
		void setDataBufferString(const char*);

		long getScriptVariableIndex();
		void setScriptVariableIndex(long index);

		float getScriptDestinationX();
		void setScriptDestinationX(float value);

		float getScriptDestinationY();
		void setScriptDestinationY(float value);

		float getScriptDestinationZ();
		void setScriptDestinationZ(float value);

		void setScriptDestination(float x, float y, float z);

		float getScriptTargetRotationX();
		void setScriptTargetRotationX(float value);

		float getScriptTargetRotationY();
		void setScriptTargetRotationY(float value);

		float getScriptTargetRotationZ();
		void setScriptTargetRotationZ(float value);

		void setScriptTargetRotation(float x, float y, float z);

		//
		// Execute original op code.
		//

		float RunOriginalOpCode(TES3::Script* script, TES3::Reference* reference, OpCode::OpCode_t opCode, TES3::BaseObject* objectParam = nullptr, char charParam = '\0');

		//
		// Wrapper functions for original opcodes.
		//

		void Activate(TES3::Script* script, TES3::Reference* reference);

		void AddItem(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate, long count);

		void AddSoulGem(TES3::Script*, TES3::Reference*, TES3::Creature*, TES3::Misc*);

		void AddToLevCreature(TES3::Script*, TES3::Reference*, TES3::BaseObject*, TES3::Actor*, unsigned short);

		void AddToLevItem(TES3::Script*, TES3::Reference*, TES3::BaseObject*, TES3::PhysicalObject*, unsigned short);

		void AddTopic(TES3::Script* script, TES3::Reference* reference, TES3::Dialogue* topic);

		void AddSpell(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* spellTemplate);

		void AITravel(TES3::Script* script, TES3::Reference* reference, float x, float y, float z);

		void Cast(TES3::Script* script, TES3::Reference* reference, TES3::Spell* spell, TES3::BaseObject* target);

		void Disable(TES3::Script* script, TES3::Reference* reference);

		void Drop(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate, long count);

		void Enable(TES3::Script* script, TES3::Reference* reference);

		void Equip(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate);

		void ExplodeSpell(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* spellTemplate);

		int GetButtonPressed(TES3::Script* script, TES3::Reference* reference);

		bool HasItemEquipped(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate);

		bool GetDetected(TES3::Script* script, TES3::Reference* reference, TES3::Reference* target);

		bool GetDisabled(TES3::Script* script, TES3::Reference* reference);

		float GetDistance(TES3::Script* script, TES3::Reference* reference, TES3::Reference* target);
		
		long GetItemCount(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate);

		bool GetPCJumping(TES3::Script* script);

		bool GetPCRunning(TES3::Script* script);

		bool GetPCSneaking(TES3::Script* script);

		bool GetSpellEffects(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* spellTemplate);

		void PlaceAtPC(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* placedTemplate, long count, float distance, float direction);

		void PlaySound(TES3::Script* script, TES3::Reference* reference, TES3::Sound* sound);

		void Position(TES3::Script* script, TES3::Reference* reference, float x, float y, float z, float rotation);

		void PositionCell(TES3::Script* script, TES3::Reference* reference, float x, float y, float z, float rotation, const char* cell);

		void RemoveFromLevCreature(TES3::Script*, TES3::Reference*, TES3::BaseObject*, TES3::Actor*, unsigned short);

		void RemoveFromLevItem(TES3::Script*, TES3::Reference*, TES3::BaseObject*, TES3::PhysicalObject*, unsigned short);

		void RemoveItem(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* itemTemplate, long count);

		void RemoveSpell(TES3::Script* script, TES3::Reference* reference, TES3::BaseObject* spellTemplate);

		bool ScriptRunning(TES3::Script* script, TES3::Script* targetScript);

		void SetLevel(TES3::Script* script, TES3::Reference* reference, short level);

		void StartCombat(TES3::Script* script, TES3::Reference* reference, TES3::Reference* target);

		void StartScript(TES3::Script* script, TES3::Reference* reference, TES3::Script* targetScript);

		void StopCombat(TES3::Script* script, TES3::Reference* reference);

		void StopScript(TES3::Script* script, TES3::Script* targetScript);

		void StopSound(TES3::Script* script, TES3::Reference* reference, TES3::Sound* sound);

		//
		// In-function hook callbacks for getting script variables.
		//

		extern TES3::Reference* lastCreatedPlaceAtPCReference;

		void OnPlaceReferenceCreated(TES3::Reference* reference);
	}
}