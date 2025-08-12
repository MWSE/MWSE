#include "NIGeometryDataLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "NIGeometryData.h"
#include "NIRTTI.h"
#include "NILinesData.h"
#include "NiTriBasedGeometryData.h"
#include "NITriShapeData.h"

namespace mwse::lua {
	void bindNIGeometryData() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Bind NI::GeometryData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::GeometryData>("niGeometryData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);
		}

		// Bind NI::LinesData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::LinesData>("niLinesData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::GeometryData>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);
		}

		// Bind NI::TriBasedGeometryData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TriBasedGeometryData>("niTriBasedGeometryData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::GeometryData>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["renderFlags"] = &NI::TriBasedGeometryData::patchRenderFlags;

			// Functions exposed as properties.
			usertypeDefinition["activeTriangleCount"] = sol::property(&NI::TriBasedGeometryData::getActiveTriangleCount, &NI::TriBasedGeometryData::setActiveTriangleCount);
			usertypeDefinition["triangleCount"] = sol::readonly_property(&NI::TriBasedGeometryData::triangleCount);
		}

		// Bind NI::TriShapeData
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::TriShapeData>("niTriShapeData");
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object, NI::GeometryData, NI::TriBasedGeometryData>();
			usertypeDefinition["new"] = sol::no_constructor;

			// Base class binding.
			setUserdataForNIGeometryData(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["renderFlags"] = &NI::TriShapeData::patchRenderFlags;

			// Functions exposed as properties.
			usertypeDefinition["activeTriangleCount"] = sol::property(&NI::TriShapeData::getActiveTriangleCount, &NI::TriShapeData::setActiveTriangleCount);
			usertypeDefinition["triangleCount"] = sol::readonly_property(&NI::TriShapeData::triangleCount);
			usertypeDefinition["triangles"] = sol::readonly_property(&NI::TriShapeData::getTriangles);

			// Basic function binding.
			usertypeDefinition["copy"] = &NI::TriShapeData::copyData;
		}

		// Binding for NI::Triangle.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Triangle>("niTriangle");
			usertypeDefinition["new"] = sol::constructors<NI::Triangle(), NI::Triangle(unsigned short, unsigned short, unsigned short), NI::Triangle(const sol::table)>();

			// Basic bindings.
			usertypeDefinition["vertices"] = sol::readonly_property(&NI::Triangle::getVertices);
		}
	}
}
