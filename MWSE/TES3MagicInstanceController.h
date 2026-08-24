#pragma once

#include "NIDefines.h"

#include "TES3Defines.h"

namespace TES3 {
	struct MagicInstanceController {
		struct StlMap {
			// Node of the engine's std::map. The root member points at the sentinel head node, whose left child is the first element in key order.
			struct Node {
				Node* left; // 0x0
				Node* parent; // 0x4
				Node* right; // 0x8
				unsigned int key; // 0xC
				void* value; // 0x10
				char redBlack; // 0x14
			};

			char tag;
			Node * root;
			char unknown_8;
			size_t itemCount;

			StlMap() = delete;
			~StlMap() = delete;
		};

		NI::Node * worldSpellRoot;
		bool flagNoProcess;
		StlMap mapSerialToMagicSourceInstance;
		StlMap mapItemDataToSerial;
		StlMap mapReferenceToSerial;

		MagicInstanceController() = delete;
		~MagicInstanceController() = delete;

		//
		// Other related this-call functions.
		//

		unsigned int activateSpell(Reference* reference, EquipmentStack* sourceItem, MagicSourceCombo* source);
		void removeSpellsByEffect(Reference* reference, int effectId, int percentChance);
		void clearSpellEffect(Reference* reference, int castType, int percentChance, bool removeSpell);
		MagicSourceInstance* getInstanceFromSerial(unsigned int serial);
		void retireMagicBySerial(unsigned int serial);
		void retireMagicCastedByActor(Reference* reference);
		void interruptCasting(Reference* reference);

		//
		// Custom functions.
		//

		StlMap::Node* getNextSerialInstanceNode(StlMap::Node* node) const;
		void cleanupReference(Reference* reference);

		//
		// Other related static functions.
		//

		static unsigned int getSerialCount();
		static void updateActiveMagicEffectIcons();
	};
	static_assert(sizeof(MagicInstanceController) == 0x38, "TES3::MagicInstanceController failed size validation");
}
