#include "NISkinInstance.h"

#include "ExceptionUtil.h"

namespace NI {
	nonstd::span<unsigned short> SkinPartition::Partition::getBones() {
		if (bones) {
			return nonstd::span<unsigned short>(bones, numBones);
		}
		return {};
	}

	nonstd::span<unsigned short> SkinPartition::Partition::getStripLengths() {
		if (bones) {
			return nonstd::span<unsigned short>(stripLengths, numStripLengths);
		}
		return {};
	}

	nonstd::span<Triangle> SkinPartition::Partition::getTriangles() {
		if (triangles) {
			return nonstd::span<Triangle>(triangles, numTriangles);
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

	void SkinInstance::deform(const NI::Vector3* srcVertices, const NI::Vector3* srcNormals, unsigned int vertexCount, NI::Vector3* dstVertices, NI::Vector3* dstNormals) const {
#if defined(SE_NI_SKININSTANCE_FNADDR_DEFORM) && SE_NI_SKININSTANCE_FNADDR_DEFORM > 0
		const auto NI_SkinInstance_Deform = reinterpret_cast<void(__thiscall*)(const NI::SkinInstance*, const NI::Vector3*, size_t, const NI::Vector3*, size_t, NI::Vector3*, NI::Vector3*, size_t)>(SE_NI_SKININSTANCE_FNADDR_DEFORM);
		return NI_SkinInstance_Deform(this, srcVertices, sizeof(Vector3), srcNormals, vertexCount, dstVertices, dstNormals, sizeof(Vector3));
#else
		throw not_implemented_exception();
#endif
	}

	nonstd::span<AVObject*> SkinInstance::getBoneObjects() {
		if (bones) {
			return nonstd::span<AVObject*>(bones, skinData->numBones);
		}
		return {};
	}
}
