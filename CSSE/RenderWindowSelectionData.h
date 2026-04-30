#pragma once

#include "CSDefines.h"

#include "NIBound.h"
#include "CSReference.h"

#include "MemoryUtil.h"

namespace se::cs::dialog::render_window {
	struct SelectionData {
		enum class RotationAxis : unsigned int {
			X = 1,
			Z = 2,
			Y = 3,
		};
		struct Target {
			Reference* reference; // 0x0
			Target* previous; // 0x4
			Target* next; // 0x8
		};
		Target* firstTarget; // 0x0
		unsigned int numberOfTargets; // 0x4
		NI::Bound bound; // 0x8

		void recalculateCenter();
		void recalculateRadius();
		void recalculateBound();

		void clear(bool updateVisuals = true);

		void addReference(Reference* objRef, bool updateVisuals = true);
		bool removeReference(Reference* objRef, bool updateVisuals = true);
		bool isSelected(Reference* objRef);

		static inline auto get() {
			return memory::ExternalGlobal<SelectionData*, 0x6CE968>::get();
		}

		Target* getLastTarget() const;
	};
	static_assert(sizeof(SelectionData) == 0x18, "SelectionData failed size validation");
	static_assert(sizeof(SelectionData::Target) == 0xC, "SelectionData::Target failed size validation");

	struct UndoManager {
		enum class Action : unsigned int {
			Deleted = 1,
			Added = 2,
			Moved = 3,
			Land_related_4 = 4,
			Land_related_5 = 5,
			Land_related_6 = 6,
		};
		struct Undoable {
			int groupIndex; // 0x0
			int groupTotal; // 0x4
			Reference* reference; // 0x8
			Action action; // 0xC
			NI::Vector3 unknown_10; // 0x10
			NI::Vector3 unknown_1C; // 0x1C
			float scale; // 0x28
			int unknown_2C[7]; // 0x2C
			Undoable* nextRedo; // 0x48
			Undoable* nextUndo; // 0x4C
		};

		Undoable* sentinel; // 0x0
		Undoable* currentUndoPosition; // 0x4

		void storeCheckpoint(Action action); // When using Action::Moved you must have a checkpoint both before and after the action or it will crash later on.
		void undo();
		void redo();

		static inline auto get() {
			return memory::ExternalGlobal<UndoManager*, 0x6CEA24>::get();
		}
	};
	static_assert(sizeof(UndoManager) == 0x8, "UndoManager failed size validation");
	static_assert(sizeof(UndoManager::Undoable) == 0x50, "UndoManager::Undoable failed size validation");
}