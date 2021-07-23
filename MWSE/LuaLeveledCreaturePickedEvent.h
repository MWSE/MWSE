#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse::lua::event {
	class LeveledCreaturePickedEvent : public ObjectFilteredEvent, public DisableableEvent<LeveledCreaturePickedEvent> {
	public:
		LeveledCreaturePickedEvent(TES3::LeveledCreature * list, TES3::Object * vanillaResult);
		sol::table createEventTable();

		static TES3::Reference* m_LastLeveledSourceReference;
		static bool m_IsForEmptyCell;

	protected:
		TES3::LeveledCreature* m_List;
		TES3::Object* m_Result;
	};
}
