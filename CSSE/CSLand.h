#pragma once

#include "CSBaseObject.h"
#include "NIObject.h"

#include "NIColor.h"
#include "NINode.h"
#include "NITriShape.h"
#include "NIVector3.h"

namespace se::cs {
	struct Land : BaseObject {
		// 16 blocks (4x4) per land
		template <typename T>
		struct Block {
			struct Array {
				Block<T>* block[16];
			};
			T data[17][17];
		};
		using VertexBlock = Block<NI::Vector3>;
		using VertexNormalBlock = Block<NI::Vector3>;
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


	};
	static_assert(sizeof(Land) == 0x250, "TES3::Land failed size validation");
	static_assert(sizeof(Land::VertexBlock) == 0xD8C, "TES3::Land::VertexBlock failed size validation");
	static_assert(sizeof(Land::VertexNormalBlock) == 0xD8C, "TES3::Land::VertexNormalBlock failed size validation");
	static_assert(sizeof(Land::VertexColorBlock) == 0x484, "TES3::Land::VertexColorBlock failed size validation");
}