#include "NIGeometryDataLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIGeometryData.h"
#include "NIRTTI.h"
#include "NiTriBasedGeometryData.h"
#include "NITriShapeData.h"

namespace mwse::lua {
	void bindNIGeometryData() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		sol::state& state = stateHandle.state;

		// Bind NI::GeometryData
		{
			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<NI::GeometryData>("niGeometryData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);
		}

		// Bind NI::TriBasedGeometryData
		{
			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<NI::TriBasedGeometryData>("niTriBasedGeometryData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::GeometryData>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);

			// Functions exposed as properties.
			usertypeDefinition["triangleCount"] = sol::readonly_property(&NI::TriBasedGeometryData::getTriangleCount);
		}

		// Bind NI::TriShapeData
		{
			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<NI::TriShapeData>("niTriShapeData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::GeometryData, NI::TriBasedGeometryData>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);

			// Functions exposed as properties.
			usertypeDefinition["triangleCount"] = sol::readonly_property(&NI::TriShapeData::getTriangleCount);

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::TriShapeData::copyData;
		}
	}
}
