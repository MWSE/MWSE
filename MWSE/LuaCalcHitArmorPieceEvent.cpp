#include "LuaCalcHitArmorPieceEvent.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3MobileNPC.h"
#include "TES3Reference.h"

namespace mwse {
	namespace lua {
		namespace event {
			CalcHitArmorPiece::CalcHitArmorPiece(TES3::MobileNPC * actor, TES3::ArmorSlot::ArmorSlot slot, TES3::ArmorSlot::ArmorSlot fallback) :
				ObjectFilteredEvent("calcArmorPieceHit", actor->reference->baseObject),
				m_Mobile(actor),
				m_Slot(slot),
				m_Fallback(fallback)
			{

			}

			sol::table CalcHitArmorPiece::createEventTable() {
				auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
				sol::state& state = stateHandle.state;
				sol::table eventData = state.create_table();

				eventData["mobile"] = m_Mobile;
				eventData["reference"] = m_Mobile->reference;

				if (m_Slot != TES3::ArmorSlot::Invalid) {
					eventData["slot"] = m_Slot;
				}

				if (m_Fallback != TES3::ArmorSlot::Invalid) {
					eventData["fallback"] = m_Fallback;
				}

				return eventData;
			}

			bool CalcHitArmorPiece::m_EventEnabled = false;
		}
	}
}
