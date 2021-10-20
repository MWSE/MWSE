#pragma once

namespace NI {
	struct AlphaProperty;
	struct AmbientLight;
	struct AVObject;
	struct BinaryStream;
	struct Bound;
	struct Camera;
	struct CollisionGroup;
	struct CollisionSwitch;
	struct Color;
	struct ColorA;
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
	struct Property;
	struct Quaternion;
	struct RenderedTexture;
	struct Renderer;
	struct RotatingParticles;
	struct RotatingParticlesData;
	struct RTTI;
	struct Sequence;
	struct SourceTexture;
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
	struct TriBasedGeometry;
	struct TriBasedGeometryData;
	struct TriShape;
	struct TriShapeData;
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

	namespace RTTIStaticPtr {
		enum RTTIStaticPtr : uintptr_t {
			AvoidNode = 0x7DD794,
			BrickNiExtraData = 0x7D1D48,
			BSMirroredNode = 0x7C8AF8,
			NiAccumulator = 0x7DECC0,
			NiAlphaAccumulator = 0x7DDF48,
			NiAlphaController = 0x7DF0A0,
			NiAlphaProperty = 0x7DDF7C,
			NiAmbientLight = 0x7DDF8C,
			NiAutoNormalParticles = 0x7DE064,
			NiAutoNormalParticlesData = 0x7DE044,
			NiAVObject = 0x7DE7CC,
			NiBillboardNode = 0x7DE0A0,
			NiBltSource = 0x7DE108,
			NiBSAnimationManager = 0x7DE91C,
			NiBSAnimationNode = 0x7DE98C,
			NiBSPArrayController = 0x7DF978,
			NiBSParticleNode = 0x7DED3C,
			NiBSPNode = 0x7DE13C,
			NiCamera = 0x7DDEF8,
			NiClusterAccumulator = 0x7DDF20,
			NiCollisionSwitch = 0x7DE150,
			NiColorData = 0x7DF100,
			NiDirectionalLight = 0x7DE16C,
			NiDitherProperty = 0x7DE194,
			NiDX8Renderer = 0x7DD89C,
			NiDynamicEffect = 0x7DEA08,
			NiExtraData = 0x7DDE68,
			NiFlipController = 0x7DF138,
			NiFloatController = 0x7DF9B8,
			NiFloatData = 0x7DF084,
			NiFltAnimationNode = 0x7DE1E8,
			NiFogProperty = 0x7DE1F0,
			NiGeometry = 0x7DEA2C,
			NiGeometryData = 0x7DE93C,
			NiGeomMorpherController = 0x7DF180,
			NiGravity = 0x7DF1B0,
			NiKeyframeController = 0x7DF000,
			NiKeyframeData = 0x7DEFC8,
			NiKeyframeManager = 0x7DF058,
			NiLight = 0x7DECF8,
			NiLightColorController = 0x7DF1F8,
			NiLines = 0x7DE228,
			NiLinesData = 0x7DE20C,
			NiLODNode = 0x7DE270,
			NiLookAtController = 0x7DF244,
			NiMaterialColorController = 0x7DF278,
			NiMaterialProperty = 0x7DE288,
			NiMorphData = 0x7DF148,
			NiMorpherController = 0x7DFA08,
			NiNode = 0x7DDEB8,
			NiObject = 0x7DE734,
			NiObjectNET = 0x7DE76C,
			NiPalette = 0x7DE0C4,
			NiParticleBomb = 0x7DF2AC,
			NiParticleCollider = 0x7DFA8C,
			NiParticleColorModifier = 0x7DF2DC,
			NiParticleGrowFade = 0x7DF304,
			NiParticleModifier = 0x7DFA58,
			NiParticleRotation = 0x7DF340,
			NiParticles = 0x7DE004,
			NiParticlesData = 0x7DDFEC,
			NiParticleSystemController = 0x7DEF64,
			NiPathController = 0x7DF394,
			NiPixelData = 0x7DE0EC,
			NiPlanarCollider = 0x7DF3CC,
			NiPointLight = 0x7DE2A8,
			NiPosData = 0x7DF1D4,
			NiProperty = 0x7DE720,
			NiRenderedCubeMap = 0x7DEE14,
			NiRenderedTexture = 0x7DE2BC,
			NiRenderer = 0x7DEA9C,
			NiRendererSpecificProperty = 0x7DE2F0,
			NiRollController = 0x7DF3E0,
			NiRotatingParticles = 0x7DE324,
			NiRotatingParticlesData = 0x7DE308,
			NiScreenPolygon = 0x7DDEE0,
			NiSequenceStreamHelper = 0x7DF3F8,
			NiShadeProperty = 0x7DE364,
			NiSkinData = 0x7DDFBC,
			NiSkinInstance = 0x7DDFD0,
			NiSkinPartition = 0x7DDE84,
			NiSortAdjustNode = 0x7DE388,
			NiSourceTexture = 0x7DE3C0,
			NiSpecularProperty = 0x7DE3DC,
			NiSphericalCollider = 0x7DF420,
			NiSpotLight = 0x7DE400,
			NiStencilProperty = 0x7DE418,
			NiStringExtraData = 0x7DE42C,
			NiSwitchNode = 0x7DE1AC,
			NiTextKeyExtraData = 0x7DF038,
			NiTexture = 0x7DE7E0,
			NiTextureEffect = 0x7DE484,
			NiTexturingProperty = 0x7DE440,
			NiTimeController = 0x7DEDB4,
			NiTriBasedGeom = 0x7DE94C,
			NiTriBasedGeomData = 0x7DEE7C,
			NiTriShape = 0x7DE550,
			NiTriShapeData = 0x7DE490,
			NiTriShapeDynamicData = 0x7DE568,
			NiTriStrips = 0x7DE5A8,
			NiTriStripsData = 0x7DE588,
			NiUVController = 0x7DF4A4,
			NiUVData = 0x7DF47C,
			NiVertexColorProperty = 0x7DE5D0,
			NiVertWeightsExtraData = 0x7DE5E8,
			NiVisController = 0x7DF4D8,
			NiVisData = 0x7DF4C0,
			NiWireframeProperty = 0x7DE604,
			NiZBufferProperty = 0x7DE630,
			OffscreenSceneGraph_MasterPropertyAccumulator = 0x7C7438,
			RootCollisionNode = 0x7DD7AC,
			TES3ObjectExtraData = 0x7CE280,
		};
	}
}
