#pragma once

#include "NIRTTIDefines.h"

namespace NI {
	struct Accumulator;
	struct AlphaProperty;
	struct AmbientLight;
	struct AVObject;
	struct BinaryStream;
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
	struct Transform;
#endif
	struct Triangle;
	struct TriBasedGeometry;
	struct TriBasedGeometryData;
	struct TriShape;
	struct TriShapeData;
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

	namespace VirtualTableAddress {
		enum VirtualTableAddress : unsigned int {
			AvoidNode = 0x74F370,
			BrickNiExtraData = 0x74B418,
			BSAnimationManager = 0x750BD8,
			BSAnimationNode = 0x750D68,
			BSMirroredNode = 0x74768C,
			HashMapLight = 0x74F6E8,
			NiAccumulator = 0x74697C,
			NiAlphaAccumulator = 0x7468D0,
			NiAlphaController = 0x74AA8C,
			NiAlphaProperty = 0x7465A8,
			NiAmbientLight = 0x74FB90,
			NiApplication = 0x74A920,
			NiAutoNormalParticles = 0x74FDE8,
			NiAutoNormalParticlesData = 0x74FD90,
			NiAVObject = 0x750B40,
			NiAVObjectLinkData = 0x74F368,
			NiBillboardNode = 0x746B00,
			NiBinaryStream = 0x74F208,
			NiBltSource = 0x74FEF4,
			NiBMPReader = 0x751330,
			NiBoundingVolume = 0x74F25C,
			NiBoxBV = 0x74F218,
			NiBSPArrayController = 0x751AD0,
			NiBSParticleNode = 0x751130,
			NiBSPNode = 0x74FF30,
			NiCamera = 0x74FAA8,
			NiClusterAccumulator = 0x74FB4C,
			NiClusterAccumulator_2 = 0x751250,
			NiCollisionGroup = 0x751248,
			NiCollisionSwitch = 0x74F418,
			NiColorData = 0x7514F0,
			NiDDSReader = 0x751320,
			NiDevImageConverter = 0x751078,
			NiDirectionalLight = 0x74FFE0,
			NiDitherProperty = 0x746A24,
			NiDX8ConfigurableTexturePipeline = 0x74F6D0,
			NiDX8CTPipelineState = 0x74F828,
			NiDX8IndexBufferManager = 0x74F724,
			NiDX8LightManager = 0x74F6E4,
			NiDX8MaterialManager = 0x74F82C,
			NiDX8RenderedCubeMapData = 0x74F7CC,
			NiDX8RenderedTextureData = 0x74F7A0,
			NiDX8Renderer = 0x74F4D0,
			NiDX8RenderState = 0x74F6CC,
			NiDX8SourceCubeMapData = 0x74F7F8,
			NiDX8SourceTextureData = 0x74F770,
			NiDX8TextureData = 0x74F688,
			NiDX8TextureManager = 0x74F6FC,
			NiDX8TexturePass = 0x74F650,
			NiDX8TexturePass_1 = 0x74F6C4,
			NiDX8VertexBufferManager = 0x74F73C,
			NiDynamicEffect = 0x750E20,
			NiDynamicEffectState = 0x7469B4,
			NiExtraData = 0x74F954,
			NiExtraDataLinkData = 0x74F980,
			NiFile = 0x74F1F4,
			NiFileTES3 = 0x74766C,
			NiFlipController = 0x751520,
			NiFloatController = 0x74AB00,
			NiFloatData = 0x74AAD4,
			NiFltAnimationNode = 0x750130,
			NiFogProperty = 0x7466F4,
			NiGeometry = 0x750EB8,
			NiGeometryData = 0x750C84,
			NiGeomMorpherController = 0x7515A0,
			NiGravity = 0x7515F4,
			NiHalfSpaceBV = 0x74F32C,
			NiImageConverter = 0x751064,
			NiKeyframeController = 0x7513E0,
			NiKeyframeControllerLinkData = 0x751428,
			NiKeyframeData = 0x7513AC,
			NiKeyframeDataLinkData = 0x7513D8,
			NiKeyframeManager = 0x751460,
			NiLight = 0x751098,
			NiLightColorController = 0x7516A8,
			NiLines = 0x750220,
			NiLinesData = 0x7501E0,
			NiLinesDataLinkData = 0x750218,
			NiLinesLinkData = 0x7502BC,
			NiLODNode = 0x7502C0,
			NiLODNodeLinkData = 0x750368,
			NiLookAtController = 0x751700,
			NiLozengeBV = 0x74F28C,
			NiMaterialColorController = 0x751750,
			NiMaterialProperty = 0x75036C,
			NiMaterialPropertyLinkData = 0x7503A0,
			NiMorphData = 0x751570,
			NiMorpherController = 0x751B28,
			NiNIFImageReader = 0x7512E0,
			NiNode = 0x74F9C8,
			NiNodeLinkData = 0x74F35C,
			NiObject = 0x750AE4,
			NiObjectNET = 0x750B10,
			NiPalette = 0x74FE98,
			NiParticleBomb = 0x75179C,
			NiParticleCollider = 0x751B78,
			NiParticleColorModifier = 0x7517DC,
			NiParticleGrowFade = 0x75181C,
			NiParticleModifier = 0x751630,
			NiParticleRotation = 0x75185C,
			NiParticles = 0x74FCE8,
			NiParticlesData = 0x74FC90,
			NiParticleSystemController = 0x751348,
			NiPathController = 0x7518A0,
			NiPixelData = 0x74FEC4,
			NiPlanarCollider = 0x7518F0,
			NiPointLight = 0x749D40,
			NiPointLightLinkData = 0x7503A4,
			NiPosData = 0x751674,
			NiProperty = 0x74656C,
			NiPropertyState = 0x75124C,
			NiRefObject = 0x7469B8,
			NiRenderedTexture = 0x7503A8,
			NiRenderer = 0x750F58,
			NiRendererSelect = 0x74A984,
			NiRendererSpecificProperty = 0x750410,
			NiRendererSpecificPropertyLinkData = 0x750444,
			NiRendererSpecificPropertyRenderData7 = 0x74B6E0,
			NiRendererSpecificPropertyRenderDataBase = 0x74B6E8,
			NiRollController = 0x751938,
			NiRotatingParticles = 0x7504A0,
			NiRotatingParticlesData = 0x750448,
			NiRotatingParticlesDataLinkData = 0x75049C,
			NiRotatingParticlesLinkData = 0x750544,
			NiScreenPolygon = 0x74FA74,
			NiSequenceStreamHelper = 0x747774,
			NiSGIReader = 0x751310,
			NiShadeProperty = 0x750548,
			NiSkinData = 0x74FC30,
			NiSkinInstance = 0x74FC60,
			NiSkinPartition = 0x74F988,
			NiSkinPartitionPartition = 0x74F984,
			NiSortAdjustNode = 0x750580,
			NiSourceTexture = 0x75062C,
			NiSourceTextureLinkData = 0x750668,
			NiSpecularProperty = 0x75066C,
			NiSpecularPropertyLinkData = 0x7506A0,
			NiSphereBV = 0x74F2BC,
			NiSphericalCollider = 0x751988,
			NiSpotLight = 0x7506A8,
			NiSpotLightLinkData = 0x750740,
			NiStencilProperty = 0x746A5C,
			NiStream = 0x74F8F0,
			NiStringExtraData = 0x750748,
			NiSwitchNode = 0x750080,
			NiSwitchNodeLinkData = 0x750128,
			NiTextKeyExtraData = 0x75142C,
			NiTexture = 0x7503DC,
			NiTextureEffect = 0x7507C0,
			NiTextureEffectLinkData = 0x75085C,
			NiTextureReader = 0x7512F0,
			NiTextureRendererData = 0x74F6B0,
			NiTexturingProperty = 0x750778,
			NiTexturingPropertyBumpMap = 0x7507B0,
			NiTexturingPropertyMap = 0x7465E8,
			NiTGAReader = 0x751300,
			NiTimeController = 0x751200,
			NiTriBasedGeom = 0x750CC0,
			NiTriBasedGeomData = 0x751268,
			NiTriShape = 0x7508B0,
			NiTriShapeData = 0x750860,
			NiTriShapeDataLinkData = 0x7508AC,
			NiTriShapeDynamicData = 0x750960,
			NiTriShapeLinkData = 0x75095C,
			NiTriStrips = 0x750A00,
			NiTriStripsData = 0x7509B0,
			NiUnionBV = 0x74F2F4,
			NiUVController = 0x751A00,
			NiUVData = 0x7519CC,
			NiVertexColorProperty = 0x7464F8,
			NiVertWeightsExtraData = 0x750AAC,
			NiVisController = 0x751A80,
			NiVisData = 0x751A4C,
			NiWireframeProperty = 0x7466C0,
			NiZBufferProperty = 0x74652C,
			RootCollisionNode = 0x74A6CC,
			TArray_7477A4 = 0x7477A4,
			TArray_7477B4 = 0x7477B4,
			TArray_of_NiPickRecord = 0x750E14,
			TArray_of_NiPointer_BSAnimationNode = 0x750C80,
			TArray_of_NiPointer_to_Map = 0x7507AC,
			TArrayAdapters = 0x74F64C,
			TArrayModeDesc = 0x74F648,
			Tes3ExtraData = 0x74A774,
			TES3WaterNVLakeUnk2 = 0x74B660,
		};
	}
}
