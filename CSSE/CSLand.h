#pragma once

#include "CSBaseObject.h"
#include "NIObject.h"

#include "NIColor.h"
#include "NINode.h"
#include "NITriShape.h"
#include "NIPoint3.h"

namespace se::cs {
	struct LandTileInfo {
		float cellLocalX; // 0x0
		float cellLocalY; // 0x4
		float blockLocalX; // 0x8
		float blockLocalY; // 0xC
		int blockX; // 0x10
		int blockY; // 0x14
		int blockIndex; // 0x18
		float tileLocalX; // 0x1C
		float tileLocalY; // 0x20
		int tileX; // 0x24
		int tileY; // 0x28
		int textureTileIndex; // 0x2C
		float nearestVertexWorldX; // 0x30
		float nearestVertexWorldY; // 0x34
		float nearestVertexWorldZ; // 0x38
		int vertexIndex; // 0x3C
		int candidateVertexIndex0; // 0x40
		int candidateVertexIndex1; // 0x44
		int candidateVertexIndex2; // 0x48
		unsigned char triangleFlag0; // 0x4C
		unsigned char chosenTriangleFlag; // 0x4D
		unsigned char padding_0x4E[2];
	};
	static_assert(sizeof(LandTileInfo) == 0x50, "LandTileInfo failed size validation");

	struct Land : BaseObject {
		// 16 blocks (4x4) per land
		template <typename T>
		struct Block {
			struct Array {
				Block<T>* block[16];
			};
			T data[17][17];
		};
		using VertexBlock = Block<NI::Point3>;
		using VertexNormalBlock = Block<NI::Point3>;
		using VertexColorBlock = Block<NI::PackedColor>;

		NI::Pointer<NI::Node> sceneNode; // 0x10
		unsigned int flags; // 0x14
		int unknown_0x18;
		FILETIME lastWriteTime;  // 0x1C
		NI::Node** blockTextures; // 0x24
		VertexBlock::Array* vertexBlocks; // 0x28
		VertexNormalBlock::Array* vertexNormalBlocks; // 0x2C
		VertexColorBlock::Array* vertexColorBlocks; // 0x30
		int unknown_0x34;
		bool* blockCreated; // 0x38
		unsigned short textureIndices[16][16]; // 0x2C
		NI::Pointer<NI::Node> cellBorderVisual; // 0x22C
		int gridX; // 0x240
		int gridY; //0x244
		int unknown_0x248;
		int unknown_0x24C;


		bool worldPosToTileInfo(LandTileInfo* tileInfo, NI::Point3* worldPos, bool snapToNearestVertex);
		bool getVertexColorAtTileInfo(LandTileInfo* tileInfo, NI::PackedColor* outColor);
		bool setVertexColorAtTile(LandTileInfo* tileInfo, NI::PackedColor* color, bool skipAdjacentCells);
		void setVertexColorAtBlockTile(int blockIndex, int vertexIndex, NI::PackedColor* color);

		void refreshGeometry();
		void notifyModified();
	};
	static_assert(sizeof(Land) == 0x250, "TES3::Land failed size validation");
	static_assert(sizeof(Land::VertexBlock) == 0xD8C, "TES3::Land::VertexBlock failed size validation");
	static_assert(sizeof(Land::VertexNormalBlock) == 0xD8C, "TES3::Land::VertexNormalBlock failed size validation");
	static_assert(sizeof(Land::VertexColorBlock) == 0x484, "TES3::Land::VertexColorBlock failed size validation");
}