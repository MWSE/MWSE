#pragma once

// Include the headers required by the target application.
#if defined(SE_TARGETS_MW) && SE_TARGETS_MW == 1
	#include "NIConfig.Morrowind.h"
	#include "NIRTTIDefines.Morrowind.h"
	#include "NIVirtualTableDefines.Morrowind.h"

	namespace TES3 {
		struct Reference;
	}
	namespace NI {
		using GameReferenceType = TES3::Reference;
	}
#elif defined(SE_TARGETS_CS) && SE_TARGETS_CS == 1
	#include "NIConfig.TESConstructionSet.h"
	#include "NIRTTIDefines.TESConstructionSet.h"
	// NIVirtualTableDefines.TESConstructionSet.h intentionally not included —
	// it is a stub (no consumer in SharedSE or CSSE today; only MWSE-only
	// files reference NI::VirtualTableAddress, and they get the Morrowind
	// table via the SE_TARGETS_MW branch above). The stub file has a
	// load-bearing #error guarding accidental inclusion.
	namespace se::cs {
		struct Reference;
	}
	namespace NI {
		using GameReferenceType = se::cs::Reference;
	}
#else
	static_assert(false, "Invalid target scope. Define an SE_TARGETS_ macro.");
#endif

namespace NI {
	struct Accumulator;
	struct AlphaProperty;
	struct AmbientLight;
	struct AVObject;
	struct BinaryStream;
	struct Bound;
	struct BoundingVolume;
	struct BoxBound;
	struct BoxBoundingVolume;
	struct BSAnimationNode;
	struct BSParticleNode;
	struct Camera;
	struct CollisionGroup;
	struct CollisionSwitch;
	struct Color;
	struct ColorA;
	struct ColorData;
	struct DirectionalLight;
	struct DX8Renderer;
	struct DynamicEffect;
	struct ExtraData;
	struct FogProperty;
	struct Geometry;
	struct GeometryData;
	struct Gravity;
	struct KeyframeController;
	struct KeyframeManager;
	struct Light;
	struct MaterialProperty;
	struct Matrix33;
	struct Matrix44;
	struct Node;
	struct Object;
	struct ObjectNET;
	struct PackedColor;
	struct ParticleBomb;
	struct ParticleCollider;
	struct ParticleColorModifier;
	struct ParticleGrowFade;
	struct ParticleModifier;
	struct ParticleRotation;
	struct Particles;
	struct ParticlesData;
	struct ParticleSystemController;
	struct PerParticleData;
	struct Pick;
	struct PickRecord;
	struct PixelData;
	struct PixelFormat;
	struct PlanarCollider;
	struct PointLight;
	struct PosData;
	struct Property;
	struct PropertyState;
	struct Quaternion;
	struct RenderedTexture;
	struct Renderer;
	struct RotatingParticles;
	struct RotatingParticlesData;
	struct RTTI;
	struct ScreenPolygon;
	struct Sequence;
	struct SkinInstance;
	struct SourceTexture;
	struct SphereBound;
	struct SphereBoundingVolume;
	struct SphericalCollider;
	struct SpotLight;
	struct StencilProperty;
	struct Stream;
	struct StringExtraData;
	struct SwitchNode;
	struct TextKey;
	struct TextKeyExtraData;
	struct Texture;
	struct TextureEffect;
	struct TexturingProperty;
	struct TimeController;
	struct Transform;
	struct Triangle;
	struct TriBasedGeometry;
	struct TriBasedGeometryData;
	struct TriShape;
	struct TriShapeData;
	struct Point2;
	struct Point3;
	struct Point4;
	struct VertexColorProperty;
	struct WireframeProperty;
	struct ZBufferProperty;

	struct AVObject_vTable;
	struct DynamicEffect_vTable;
	struct Geometry_vTable;
	struct GeometryData_vTable;
	struct Node_vTable;
	struct Object_vTable;
	struct Property_vTable;
	struct Renderer_vTable;
	struct SourceTexture_vTable;
	struct Texture_vTable;
	struct TimeController_vTable;
	struct TriBasedGeometryData_vTable;
	struct TriShape_vTable;
}
