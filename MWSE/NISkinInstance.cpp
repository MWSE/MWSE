#include "NISkinInstance.h"

namespace NI {
	nonstd::span<unsigned short> SkinPartition::Partition::getBones() {
		if (bones) {
			return nonstd::span<unsigned short>(bones, numBones);
		}
		return {};
	}

	nonstd::span<unsigned short> SkinPartition::Partition::getStripLengths() {
		if (bones) {
			return nonstd::span<unsigned short>(bones, numBones);
		}
		return {};
	}

	nonstd::span<unsigned short> SkinPartition::Partition::getTriangles() {
		if (triangles) {
			return nonstd::span<unsigned short>(triangles, numTriangles);
		}
		return {};
	}

	nonstd::span<unsigned short> SkinPartition::Partition::getVertices() {
		if (vertices) {
			return nonstd::span<unsigned short>(vertices, numVertices);
		}
		return {};
	}

	nonstd::span<SkinPartition::Partition> SkinPartition::getPartitions() {
		if (partitions) {
			return nonstd::span<Partition>(partitions, partitionCount);
		}
		return {};
	}

	nonstd::span<SkinData::BoneData::VertexWeight> SkinData::BoneData::getWeights() {
		if (weights) {
			return nonstd::span<VertexWeight>(weights, weightCount);
		}
		return {};
	}

	nonstd::span<SkinData::BoneData> SkinData::getBones() {
		if (boneData) {
			return nonstd::span<BoneData>(boneData, numBones);
		}
		return {};
	}

	nonstd::span<AVObject*> SkinInstance::getBoneObjects() {
		if (bones) {
			return nonstd::span<AVObject*>(bones, skinData->numBones);
		}
		return {};
	}
}
