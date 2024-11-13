#include "NIParticles.h"

#include "LuaUtil.h"
#include "MemoryUtil.h"

namespace NI {
	Particles::Particles(ParticlesData* data) : TriBasedGeometry(data) {
		vTable.asObject = reinterpret_cast<Object_vTable*>(0x74FCE8);
		staticBoundPositionSource = nullptr;
	}

	Pointer<Particles> Particles::create(unsigned short vertexCount, bool hasNormals, bool hasColors) {
		auto data = ParticlesData::create(vertexCount, hasNormals, hasColors);
		auto geom = new Particles(data);

		return geom;
	}

	AutoNormalParticles::AutoNormalParticles(AutoNormalParticlesData* data) : Particles(data) {
		vTable.asObject = reinterpret_cast<Object_vTable*>(0x74FDE8);
	}

	Pointer<AutoNormalParticles> AutoNormalParticles::create(unsigned short vertexCount, bool hasColors) {
		auto data = AutoNormalParticlesData::create(vertexCount, hasColors);
		auto geom = new AutoNormalParticles(data);

		return geom;
	}

	const auto NI_ParticlesData_ctor = reinterpret_cast<void(__thiscall*)(ParticlesData*, unsigned short, TES3::Vector3*, TES3::Vector3*, PackedColor*)>(0x6D1ED0);
	Pointer<ParticlesData> ParticlesData::create(unsigned short _vertexCount, TES3::Vector3* _vertices, TES3::Vector3* _normals, PackedColor* _colors) {
		auto ptr = mwse::tes3::_new<ParticlesData>();
		NI_ParticlesData_ctor(ptr, _vertexCount, _vertices, _normals, _colors);
		return ptr;
	}

	Pointer<ParticlesData> ParticlesData::create(unsigned short vertexCount, bool hasNormals, bool hasColors) {
		using TES3::Vector2;
		using TES3::Vector3;

		Vector3* vertices = mwse::tes3::_new<Vector3>(vertexCount);
		ZeroMemory(vertices, sizeof(Vector3) * vertexCount);

		Vector3* normals = nullptr;
		if (hasNormals) {
			normals = mwse::tes3::_new<Vector3>(vertexCount);
			ZeroMemory(normals, sizeof(Vector3) * vertexCount);
		}

		PackedColor* colors = nullptr;
		if (hasColors) {
			colors = mwse::tes3::_new<PackedColor>(vertexCount);
			ZeroMemory(colors, sizeof(PackedColor) * vertexCount);
		}

		return create(vertexCount, vertices, normals, colors);
	}

	nonstd::span<float> ParticlesData::getSizes() {
		if (sizes) {
			return nonstd::span(sizes, vertexCount);
		}
		return {};
	}

	const auto NI_AutoNormalParticlesData_ctor = reinterpret_cast<void(__thiscall*)(AutoNormalParticlesData*, unsigned short, TES3::Vector3*, PackedColor*)>(0x6D3020);
	Pointer<AutoNormalParticlesData> AutoNormalParticlesData::create(unsigned short _vertexCount, TES3::Vector3* _vertices, PackedColor* _colors) {
		auto ptr = mwse::tes3::_new<AutoNormalParticlesData>();
		NI_AutoNormalParticlesData_ctor(ptr, _vertexCount, _vertices, _colors);
		return ptr;
	}

	Pointer<AutoNormalParticlesData> AutoNormalParticlesData::create(unsigned short vertexCount, bool hasColors) {
		using TES3::Vector2;
		using TES3::Vector3;

		Vector3* vertices = mwse::tes3::_new<Vector3>(vertexCount);
		ZeroMemory(vertices, sizeof(Vector3) * vertexCount);

		PackedColor* colors = nullptr;
		if (hasColors) {
			colors = mwse::tes3::_new<PackedColor>(vertexCount);
			ZeroMemory(colors, sizeof(PackedColor) * vertexCount);
		}

		return create(vertexCount, vertices, colors);
	}

	nonstd::span<NI::Quaternion> RotatingParticlesData::getRotations() {
		if (rotations) {
			return nonstd::span(rotations, vertexCount);
		}
		return {};
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Particles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AutoNormalParticles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::RotatingParticles)
