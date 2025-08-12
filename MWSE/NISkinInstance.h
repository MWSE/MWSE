#pragma once

#include "NIObject.h"

#include "NIBound.h"
#include "NITriangle.h"

#include "TES3Vectors.h"

namespace NI {
	struct SkinPartition : Object {
		struct Partition {
			void* vtbl;
			unsigned short* bones; // 0x4
			float* weights; // 0x8
			unsigned short* vertices; // 0xC
			unsigned char* bonePalette; // 0x10
			Triangle* triangles; // 0x14
			unsigned short* stripLengths; // 0x18
			unsigned short numVertices; // 0x1C
			unsigned short numTriangles; // 0x1E
			unsigned short numBones; // 0x20
			unsigned short numStripLengths; // 0x22
			unsigned short numBonesPerVertex; // 0x24
			void* bufferData; // 0x28

			nonstd::span<unsigned short> getBones();
			nonstd::span<unsigned short> getStripLengths();
			nonstd::span<Triangle> getTriangles();
			nonstd::span<unsigned short> getVertices();
		};
		unsigned int partitionCount; // 0x8
		Partition* partitions; // 0xC

		nonstd::span<Partition> getPartitions();
	};
	static_assert(sizeof(SkinPartition) == 0x10, "NI::SkinPartition failed size validation");
	static_assert(sizeof(SkinPartition::Partition) == 0x2C, "NI::SkinPartition::Partition failed size validation");

	struct SkinData : Object {
		struct BoneData {
			struct VertexWeight {
				unsigned short index; // 0x0
				float weight; // 0x4
			};
			TES3::Transform transform; // 0x0
			Bound bounds; // 0x34
			VertexWeight* weights; // 0x48
			unsigned short weightCount; // 0x48

			nonstd::span<VertexWeight> getWeights();
		};
		Pointer<SkinPartition> partition; // 0x8
		TES3::Transform transform; // 0xC
		unsigned int numBones; // 0x40
		BoneData* boneData; // 0x44

		nonstd::span<BoneData> getBones();
	};
	static_assert(sizeof(SkinData) == 0x48, "NI::SkinData failed size validation");
	static_assert(sizeof(SkinData::BoneData) == 0x4C, "NI::SkinData::BoneData failed size validation");
	static_assert(sizeof(SkinData::BoneData::VertexWeight) == 0x8, "NI::SkinData::VertexWeight failed size validation");

	struct SkinInstance : Object {
		Pointer<SkinData> skinData; // 0x8
		AVObject* rootParent; // 0xC
		AVObject** bones; // 0x10
		int unknown_0x14;

		void deform(const TES3::Vector3* srcVertices, const TES3::Vector3* srcNormals, unsigned int vertexCount, TES3::Vector3* dstVertices, TES3::Vector3* dstNormals) const;

		nonstd::span<AVObject*> getBoneObjects();
	};
	static_assert(sizeof(SkinInstance) == 0x18, "NI::SkinInstance failed size validation");
}
