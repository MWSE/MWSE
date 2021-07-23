#pragma once

#include "LuaObjectFilteredEvent.h"
#include "LuaDisableableEvent.h"

namespace mwse {
	namespace lua {
		namespace event {
			class DetectSneakEvent : public ObjectFilteredEvent, public DisableableEvent<DetectSneakEvent> {
			public:
				DetectSneakEvent(TES3::MobileActor* detector, TES3::MobileActor* target, bool isDetected);
				sol::table createEventTable();

			protected:
				TES3::MobileActor* m_Detector;
				TES3::MobileActor* m_Target;
				bool m_IsDetected;
			};
		}
	}
}
