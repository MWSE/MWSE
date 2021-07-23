#pragma once

#include "LuaGenericEvent.h"
#include "LuaDisableableEvent.h"

#include "NIObject.h"

namespace mwse {
	namespace lua {
		namespace event {
			class MeshLoadedEvent : public GenericEvent, public DisableableEvent<MeshLoadedEvent> {
			public:
				MeshLoadedEvent(const char* path, NI::AVObject * mesh);
				sol::table createEventTable();
				sol::object getEventOptions();

			protected:
				std::string m_Path;
				NI::Pointer<NI::Object> m_Mesh;
			};
		}
	}
}
