#include "NIParticles.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	Particles::Particles(ParticlesData* data) : TriBasedGeometry(data) {
#if defined(SE_NI_PARTICLES_VTBL) && SE_NI_PARTICLES_VTBL > 0
		vTable.asObject = reinterpret_cast<Object_vTable*>(SE_NI_PARTICLES_VTBL);
		staticBoundPositionSource = nullptr;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<Particles> Particles::create(unsigned short vertexCount, bool hasNormals, bool hasColors) {
		auto data = ParticlesData::create(vertexCount, hasNormals, hasColors);
		auto geom = new Particles(data);

		return geom;
	}

	Pointer<ParticlesData> Particles::getModelData() const {
		return static_cast<ParticlesData*>(modelData.get());
	}

	AutoNormalParticles::AutoNormalParticles(AutoNormalParticlesData* data) : Particles(data) {
#if defined(SE_NI_AUTONORMALPARTICLES_VTBL) && SE_NI_AUTONORMALPARTICLES_VTBL > 0
		vTable.asObject = reinterpret_cast<Object_vTable*>(SE_NI_AUTONORMALPARTICLES_VTBL);
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<AutoNormalParticles> AutoNormalParticles::create(unsigned short vertexCount, bool hasColors) {
		auto data = AutoNormalParticlesData::create(vertexCount, hasColors);
		auto geom = new AutoNormalParticles(data);

		return geom;
	}

	Pointer<AutoNormalParticlesData> AutoNormalParticles::getModelData() const {
		return static_cast<AutoNormalParticlesData*>(modelData.get());
	}

	Pointer<ParticlesData> ParticlesData::create(unsigned short _vertexCount, Vector3* _vertices, Vector3* _normals, PackedColor* _colors) {
#if defined(SE_NI_PARTICLESDATA_FNADDR_CTOR) && SE_NI_PARTICLESDATA_FNADDR_CTOR > 0 && defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		const auto NI_ParticlesData_ctor = reinterpret_cast<void(__thiscall*)(ParticlesData*, unsigned short, Vector3*, Vector3*, PackedColor*)>(SE_NI_PARTICLESDATA_FNADDR_CTOR);
		auto ptr = se::memory::_new<ParticlesData>();
		NI_ParticlesData_ctor(ptr, _vertexCount, _vertices, _normals, _colors);
		return ptr;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<ParticlesData> ParticlesData::create(unsigned short vertexCount, bool hasNormals, bool hasColors) {
#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		Vector3* vertices = se::memory::_new<Vector3>(vertexCount);
		ZeroMemory(vertices, sizeof(Vector3) * vertexCount);

		Vector3* normals = nullptr;
		if (hasNormals) {
			normals = se::memory::_new<Vector3>(vertexCount);
			ZeroMemory(normals, sizeof(Vector3) * vertexCount);
		}

		PackedColor* colors = nullptr;
		if (hasColors) {
			colors = se::memory::_new<PackedColor>(vertexCount);
			ZeroMemory(colors, sizeof(PackedColor) * vertexCount);
		}

		return create(vertexCount, vertices, normals, colors);
#else
		throw not_implemented_exception();
#endif
	}

	nonstd::span<float> ParticlesData::getSizes() {
		if (sizes) {
			return nonstd::span(sizes, vertexCount);
		}
		return {};
	}

	Pointer<AutoNormalParticlesData> AutoNormalParticlesData::create(unsigned short _vertexCount, Vector3* _vertices, PackedColor* _colors) {
#if defined(SE_NI_AUTONORMALPARTICLESDATA_FNADDR_CTOR) && SE_NI_AUTONORMALPARTICLESDATA_FNADDR_CTOR > 0 && defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		const auto NI_AutoNormalParticlesData_ctor = reinterpret_cast<void(__thiscall*)(AutoNormalParticlesData*, unsigned short, Vector3*, PackedColor*)>(SE_NI_AUTONORMALPARTICLESDATA_FNADDR_CTOR);
		auto ptr = se::memory::_new<AutoNormalParticlesData>();
		NI_AutoNormalParticlesData_ctor(ptr, _vertexCount, _vertices, _colors);
		return ptr;
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<AutoNormalParticlesData> AutoNormalParticlesData::create(unsigned short vertexCount, bool hasColors) {
#if defined(SE_MEMORY_FNADDR_NEW) && SE_MEMORY_FNADDR_NEW > 0
		Vector3* vertices = se::memory::_new<Vector3>(vertexCount);
		ZeroMemory(vertices, sizeof(Vector3) * vertexCount);

		PackedColor* colors = nullptr;
		if (hasColors) {
			colors = se::memory::_new<PackedColor>(vertexCount);
			ZeroMemory(colors, sizeof(PackedColor) * vertexCount);
		}

		return create(vertexCount, vertices, colors);
#else
		throw not_implemented_exception();
#endif
	}

	nonstd::span<NI::Quaternion> RotatingParticlesData::getRotations() {
		if (rotations) {
			return nonstd::span(rotations, vertexCount);
		}
		return {};
	}

	Pointer<RotatingParticlesData> RotatingParticles::getModelData() const {
		return static_cast<RotatingParticlesData*>(modelData.get());
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Particles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AutoNormalParticles)
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::RotatingParticles)
#endif
