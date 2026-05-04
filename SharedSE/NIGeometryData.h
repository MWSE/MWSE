#pragma once

#include "NIBound.h"
#include "NIObject.h"
#include "NIVector2.h"

namespace NI {
	struct GeometryData_vTable : Object_vTable {
		void(__thiscall* setActiveVertexCount)(GeometryData*, unsigned short); // 0x2C
		unsigned short(__thiscall* getActiveVertexCount)(const GeometryData*); // 0x30
		void(__thiscall* calculateNormals)(GeometryData*); // 0x34
	};
	static_assert(sizeof(GeometryData_vTable) == 0x38, "NI::GeometryData_vTable failed size validation");

	struct GeometryData : Object {
		unsigned short vertexCount; // 0x8
		unsigned short textureSets; // 0xA
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original uses SphereBound (defined in NIBound.h's MWSE branch).
		// Same memory layout as Bound (Vector3 center + float radius, 0x10 bytes),
		// so sizeof(GeometryData) is unaffected by the choice.
		SphereBound bounds; // 0xC
#else
		Bound bounds; // 0xC
#endif
		Vector3* vertex; // 0x1C
		Vector3* normal; // 0x20
		PackedColor* color; // 0x24
		Vector2* textureCoords; // 0x28
		unsigned int uniqueID; // 0x2C
		unsigned short revisionID; // 0x30
		bool unknown_0x32;

		//
		// vTable wrappers.
		//

		unsigned short getActiveVertexCount() const;

		//
		// Custom functions.
		//

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-source-of-truth: non-const accessors returning nonstd::span over
		// the underlying engine arrays. Implementations live in MWSE-private
		// NIGeometryData.cpp.
		nonstd::span<PackedColor> getColors();
		nonstd::span<Vector3> getVertices();
		nonstd::span<Vector3> getActiveVertices();
		nonstd::span<Vector3> getNormals();
		nonstd::span<Vector2> getTextureCoordinates();
#else
		// CSSE / standalone: const accessor (matches existing SharedSE impl).
		nonstd::span<Vector3> getVertices() const;
#endif
		void markAsChanged();
		void updateModelBound();
	};
	static_assert(sizeof(GeometryData) == 0x34, "NI::GeometryData failed size validation");
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::GeometryData)
#endif
