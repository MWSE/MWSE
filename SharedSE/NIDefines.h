#pragma once

#include "NIConfig.h"
#include "NIRTTIDefines.h"

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
	struct Camera;
	struct CollisionGroup;
	struct CollisionSwitch;
	struct Color;
	struct ColorA;
	struct ColorData;
	struct DirectionalLight;
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
#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
	// In MWSE context, NI::Matrix33 is a `using` alias to TES3::Matrix33 (typedef
	// bridge in NIMatrix33.h). A forward struct decl here would conflict.
	struct Matrix33;
#endif
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
	struct Quaternion;
	struct RenderedTexture;
	struct Renderer;
	struct RotatingParticles;
	struct RotatingParticlesData;
	struct RTTI;
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
#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
	// In MWSE context, NI::Transform is a `using` alias to TES3::Transform.
	struct Transform;
#endif
	struct Triangle;
	struct TriBasedGeometry;
	struct TriBasedGeometryData;
	struct TriShape;
	struct TriShapeData;
#if !defined(SE_IS_MWSE) || SE_IS_MWSE == 0
	// In MWSE context, NI::Vector3 is a `using` alias to TES3::Vector3.
	struct Vector3;
#endif
	struct VertexColorProperty;
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
