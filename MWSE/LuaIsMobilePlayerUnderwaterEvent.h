#pragma once

#include "TES3MobilePlayer.h"

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class IsMobilePlayerUnderwaterEvent : public GenericEvent, public DisableableEvent<IsMobilePlayerUnderwaterEvent> {
			public:
				IsMobilePlayerUnderwaterEvent(TES3::MobilePlayer* mobilePlayer, bool isUnderwater);
				sol::table createEventTable();

			protected:
				TES3::MobilePlayer* m_MobilePlayer;
				bool m_IsUnderwater;
			};
		}
	}
}
