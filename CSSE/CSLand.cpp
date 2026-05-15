#include "CSLand.h"

namespace se::cs {
	const auto TES3CS_Land_worldPosToTileInfo = reinterpret_cast<bool(__thiscall*)(Land*, LandTileInfo*, NI::Point3*, bool)>(0x511A20);
	const auto TES3CS_Land_getVertexColorAtTileInfo = reinterpret_cast<bool(__thiscall*)(Land*, LandTileInfo*, NI::PackedColor*)>(0x514070);
	const auto TES3CS_Land_setVertexColorAtTile = reinterpret_cast<bool(__thiscall*)(Land*, LandTileInfo*, NI::PackedColor*, bool)>(0x5147C0);
	const auto TES3CS_Land_setVertexColorAtBlockTile = reinterpret_cast<void(__thiscall*)(Land*, int, int, NI::PackedColor*)>(0x514E90);
	const auto TES3CS_Land_refreshGeometry = reinterpret_cast<void(__thiscall*)(Land*, char, int)>(0x5110D0);
	const auto TES3CS_Land_notifyModified = reinterpret_cast<void(__thiscall*)(Land*)>(0x510780);

	bool Land::worldPosToTileInfo(LandTileInfo* tileInfo, NI::Point3* worldPos, bool snapToNearestVertex) {
		return TES3CS_Land_worldPosToTileInfo(this, tileInfo, worldPos, snapToNearestVertex);
	}

	bool Land::getVertexColorAtTileInfo(LandTileInfo* tileInfo, NI::PackedColor* outColor) {
		return TES3CS_Land_getVertexColorAtTileInfo(this, tileInfo, outColor);
	}

	bool Land::setVertexColorAtTile(LandTileInfo* tileInfo, NI::PackedColor* color, bool skipAdjacentCells) {
		return TES3CS_Land_setVertexColorAtTile(this, tileInfo, color, skipAdjacentCells);
	}

	void Land::setVertexColorAtBlockTile(int blockIndex, int vertexIndex, NI::PackedColor* color) {
		TES3CS_Land_setVertexColorAtBlockTile(this, blockIndex, vertexIndex, color);
	}

	void Land::refreshGeometry() {
		TES3CS_Land_refreshGeometry(this, 0, 0);
	}

	void Land::notifyModified() {
		TES3CS_Land_notifyModified(this);
	}
}
