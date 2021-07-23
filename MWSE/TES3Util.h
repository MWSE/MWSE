/************************************************************************

	TES3Util.h - Copyright (c) 2008 The MWSE Project
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

#include "mwseString.h"

#include "TES3Defines.h"

#include "TES3MagicEffect.h"

namespace mwse {
	namespace tes3 {
		TES3::Reference* getReference(const char* id);
		TES3::Reference* getReference(std::string& id);

		void setDataString(char** string, const char* contents, bool allowEmpty = false);

		bool setEffect(TES3::Effect * effects, long index, long effectId,
			long skillAttributeId, long range, long area, long duration,
			long minimumMagnitude, long maximumMagnitude);

		void checkForLevelUp(long progress);

		int getSkillNameGMST(int);
		int getAttributeNameGMST(int);
		int getCastRangeNameGMST(int);

		TES3::SoulGemData * addCustomSoulGem(const TES3::Misc * item);
		TES3::SoulGemData * getSoulGemData(const TES3::Misc * item);
		bool isSoulGem(const TES3::Object* objectOrReference);

		std::shared_ptr<TES3::ClothingSlotData> getClothingSlotData(int slot);
		void setClothingSlotData(std::shared_ptr<TES3::ClothingSlotData> data);
		std::shared_ptr<TES3::ArmorSlotData> getArmorSlotData(int slot);
		void setArmorSlotData(std::shared_ptr<TES3::ArmorSlotData> data);

		// Used in xFirstNPC/Static/Item. The last element should never be non-null.
		// The first eight elements are pointers to the first reference from the 8 surrounding cells.
		extern TES3::Reference* exteriorRefs[9];

		void clearExteriorRefs();

		void startNewGame();

		int getRestHoursInterrupted();
		void setRestHoursInterrupted(int);
		int getRestInterruptCount();
		void setRestInterruptCount(int);

		int resolveAssetPath(const char* path, char * out_buffer = nullptr);

		char* getThreadSafeStringBuffer();

		bool testLineOfSight(TES3::Vector3* pos1, float height1, TES3::Vector3* pos2, float height2);

		inline constexpr float& getSimulationTimestamp() {
			return *reinterpret_cast<float*>(0x7C6708);
		};

		//
		// Original function calls in Morrowind.
		//

		int rand(unsigned int arg0 = 0);
	}
};
