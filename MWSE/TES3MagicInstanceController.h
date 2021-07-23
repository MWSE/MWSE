#pragma once

#include "NIDefines.h"
#include "TES3Defines.h"

namespace TES3 {
	struct MagicInstanceController {
		struct StlMap {
			char tag;
			void * root;
			char unknown_8;
			size_t itemCount;
		};

		NI::Node * worldSpellRoot;
		bool flagNoProcess;
		StlMap mapSerialToMagicSourceInstance;
		StlMap mapItemDataToSerial;
		StlMap mapReferenceToSerial;

		//
		// Other related this-call functions.
		//

		unsigned int activateSpell(Reference * reference, EquipmentStack * sourceItem, MagicSourceCombo* source);
		void removeSpellsByEffect(Reference * reference, int effectId, int percentChance);
		void clearSpellEffect(Reference * reference, int castType, int percentChance, bool removeSpell);
		MagicSourceInstance * getInstanceFromSerial(unsigned int serial);

	};
	static_assert(sizeof(MagicInstanceController) == 0x38, "TES3::MagicInstanceController failed size validation");
}
