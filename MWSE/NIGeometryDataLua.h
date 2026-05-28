#pragma once

#include "NIObjectLua.h"
#include "NIGeometryData.h"

namespace mwse::lua {
	template <typename T>
	void setUserdataForNIGeometryData(sol::usertype<T>& usertypeDefinition) {
		setUserdataForNIObject(usertypeDefinition);

		// Basic property binding.
		usertypeDefinition["bounds"] = &NI::GeometryData::bounds;
		usertypeDefinition["textureSets"] = &NI::GeometryData::textureSets;
		usertypeDefinition["uniqueID"] = sol::readonly_property(&NI::GeometryData::uniqueID);
		usertypeDefinition["vertexCount"] = sol::readonly_property(&NI::GeometryData::getActiveVertexCount);

		// Safe access to arrays.
		usertypeDefinition["activeVertices"] = sol::readonly_property(&NI::GeometryData::getActiveVertices);
		usertypeDefinition["colors"] = sol::readonly_property(&NI::GeometryData::getColors);
		usertypeDefinition["normals"] = sol::readonly_property(&NI::GeometryData::getNormals);
		usertypeDefinition["texCoords"] = sol::readonly_property(&NI::GeometryData::getTextureCoordinates);
		usertypeDefinition["textures"] = sol::readonly_property(&NI::GeometryData::getTextureCoordinates);
		usertypeDefinition["vertices"] = sol::readonly_property(&NI::GeometryData::getVertices);

		// Basic function binding.
		usertypeDefinition["markAsChanged"] = &NI::GeometryData::markAsChanged;
		usertypeDefinition["updateModelBound"] = &NI::GeometryData::updateModelBound;
	}

	void bindNIGeometryData();
}
