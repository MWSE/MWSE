# The Hierarchy of NetImmerse Classes

This page puts into relation various classes that compromise Morrowind's scene graph. Some of these classes are abstract, meaning they aren't directly found in the scene graph, but are used by other classes to inherit common variables and methods. Game's scene graph can be inspected by entering ssg (show scene graph) command in the console. Morrowind's engine is Gamebryo, which was before known as NetImmerse. That's why these classes have prefix ni in their name. These classes in MWSE follow the naming as in the vanilla engine. But, there are some classes that contain information about the scene graph in MWSE and have tes3 prefix such as `tes3vector3`.

The primary building blocks of scene graph are called nodes. Each node has one parent and zero or more child nodes. For example, the base node of the Morrowind's world scene graph, named `worldRoot`, and all the game world objects are represented by nodes that are children of `worldRoot` node.

Note that the relations described here don't reflect how these types are organized in the scene graph. For instance, `worldRoot` node mentioned earlier is of type `niNode`, but is the root node of the world's scene graph, while the base class of most types described here is `niObject`.

The types are arranged into functional groupings. Within each grouping, indentation implies inheritance from the class of the previous indentation level. The color of the taxt carries the following meanings:
 - <span style = "color:Turquoise">Turquoise - Classes read by MWSE but not yet exposed to Lua scripts</span>
 - <span style = "color:YellowGreen">Green - Classes present in Morrowind but not yet read by MWSE</span>
 - All the other classes in standard text color in this list are available in Lua scripts

---

## niMain

### Basic Foundation Types
 - niColor
 - niColorA
 - niPackedColor
 - niRTTI

### Basic Math and Geometry Classes
 - <span style = "color:Turquoise">niBound</span>
 - tes3boundingBox
 - tes3matrix33
 - tes3vector2
 - tes3vector3
 - niQuaternion
 - tes3transform

### Templated Container Classes
 - niTArray - not exposed as a type but can be iterated as a standard Lua array.
 - niLinkedList
	- niDynamicEffectLinkedList
	- niNodeLinkedList
	- niPropertyLinkedList

### Animation Classes
 - niObject
	- niTimeController
	- niObjectNET
		- niAVObject
			- niNode
				- <span  style = "color:YellowGreen">niBSAnimationManager</span>
				- <span  style = "color:YellowGreen">niBSAnimationNode</span>
					- <span  style = "color:YellowGreen">niBSParticleNode</span>
		- <span  style = "color:YellowGreen">niSequenceStreamHelper</span>

### Texturing Classes
- niObject
	- <span  style = "color:YellowGreen">niBltSource</span>
	- niObjectNET
		- niTexture
			- <span style = "color:Turquoise">niRenderedTexture</span>
				- <span  style = "color:YellowGreen">niRenderedCubeMap</span >
			- niSourceTexture
	- <span  style = "color:YellowGreen">niPalette</span>
	- niPixelData
- <span style = "color:Turquoise">niPixelFormat</span>

### Object Depth Sorting Classes
- niObject
	- <span  style = "color:YellowGreen">niAccumulator</span>
		- <span  style = "color:YellowGreen">niClusterAccumulator</span>
			- <span  style = "color:YellowGreen">niAlphaAccumulator</span>

### Geometry Data Classes
- niObject
	- niGeometryData
		- <span style = "color:Turquoise">niLinesData</span>
		- niTriBasedGeomData
			- niParticlesData
				- <span  style = "color:YellowGreen">niAutoNormalParticlesData</span>
				- niRotatingParticlesData
			- niTriShapeData
				- <span  style = "color:YellowGreen">niTriShapeDynamicData</span>
			- <span  style = "color:YellowGreen">niTriStripsData</span>

### Geometry Rendering Classes
- niObject
	- niObjectNET
		- niAVObject
			- <span style = "color:Turquoise">niGeometry</span>
				- <span style = "color:Turquoise">niLines</span>
				- <span style = "color:Turquoise">niTriBasedGeometry</span>
					- niTriShape
					- <span  style = "color:YellowGreen">niTriStrips</span>
					- niParticles
						- <span  style = "color:YellowGreen">niAutoNormalParticles</span>
						- niRotatingParticles
	- <span style = "color:Turquoise">niScreenPolygon</span>
	- niSkinData
	- niSkinDataBoneData
	- niSkinDataBoneDataVertexWeight
	- niSkinInstance
	- niSkinPartition
	- niSkinPartitionPartition

### Scene Graph Organization Classes
- niObject
	- niExtraData
		- <span  style = "color:YellowGreen">BrickNiExtraData</span>
		- niStringExtraData
		- niTES3ExtraData
		- niTextKeyExtraData
		- <span  style = "color:YellowGreen">niVertWeightsExtraData</span>
	- niObjectNET
		- niAVObject
			- niNode
				- <span  style = "color:YellowGreen">AvoidNode</span>
				- niBillboardNode
				- <span  style = "color:YellowGreen">niBSPNode</span>
				- <span  style = "color:YellowGreen">niSortAdjustNode</span>
				- niSwitchNode
					- <span  style = "color:YellowGreen">niFltAnimationNode</span>
					- <span  style = "color:YellowGreen">niLODNode</span>

### Lightning and Effects Classes
- niObject
	- niObjectNET
		- niAVObject
			- niDynamicEffect
				- niLight
					- niAmbientLight
					- niDirectionalLight
					- niPointLight
						- niSpotLight
				- niTextureEffect

### Rendering and Property Classes
- niObject
	- niObjectNET
		- niAVObject
			- niCamera
		- niProperty
			- niAlphaProperty
			- <span  style = "color:YellowGreen">niDitherProperty</span>
			- niFogProperty
			- niMaterialProperty
			- <span  style = "color:YellowGreen">niRendererSpecificProperty</span>
			- <span  style = "color:YellowGreen">niShadeProperty</span>
			- <span  style = "color:YellowGreen">niSpecularProperty</span>
			- niStencilProperty
			- niTexturingProperty
			- niVertexColorProperty
			- <span  style = "color:YellowGreen">niWireframeProperty</span>
			- niZBufferProperty
	- <span style = "color:Turquoise">niRenderer</span>
		- <span  style = "color:YellowGreen">niDX8Renderer</span>

---

## niParticle

### Particle System Classes
- niObject
	- <span  style = "color:YellowGreen">niEmitterModifier - Wo don't have this one</span>
	- niParticleModifier
		- niGravity
		- niParticleBomb
		- niParticleCollider
			- niPlanarCollider
			- niSphericalCollider
		- niParticleColorModifier
		- niParticleGrowFade
		- niParticleRotation
	- niTimeController - Not only related to particles
		- niParticleSystemController
- <span style = "color:Turquoise">niPerParticleData</span>

---

## niCollision

### Collision Classes
- niCollisionGroup
- niCollisionGroupRecord
- niObject
	- niObjectNET
		- niAVObject
			- niNode
				- <span  style = "color:YellowGreen">RootCollisionNode</span>
				- niCollisionSwitch
- niPick
- niPickRecord

---

## niAnimation

### Animation Classes
- niObject
	- niColorData
	- niExtraData - Not related to animation
		- niTextKeyExtraData
	- <span  style = "color:YellowGreen">niFloatData</span>
	- niKeyframeData
	- <span  style = "color:YellowGreen">niMorphData</span>
	- <span style = "color:Turquoise">niPosData</span>
	- <span style = "color:Turquoise">niSequence</span>
	- niTimeController
		- <span style = "color:Turquoise">niFlipController</span>
		- <span  style = "color:YellowGreen">niFloatController</span>
			- <span  style = "color:YellowGreen">niAlphaController</span>
			- <span  style = "color:YellowGreen">niRollController</span>
		- niKeyframeController
		- <span style = "color:Turquoise">niKeyframeManager</span>
		- <span  style = "color:YellowGreen">niLightColorController</span>
		- niLookAtController
		- <span  style = "color:YellowGreen">niMaterialColorController</span>
		- <span  style = "color:YellowGreen">niMorpherController</span>
			- <span  style = "color:YellowGreen">niGeomMorpherController</span>
		- niParticleSystemController
			- <span  style = "color:YellowGreen">niBSPArrayController</span>
		- <span  style = "color:YellowGreen">niPathController</span>
		- <span style = "color:Turquoise">niUVController</span>
		- <span  style = "color:YellowGreen">niVisController</span>
	- <span style = "color:Turquoise">niUVData</span>
	- <span  style = "color:YellowGreen">niVisData</span>
- niAnimationKey
	- niColorKey
	- niFloatKey
		- niBezFloatKey
		- niTCBFloatKey
	- niPosKey
		- niBezPosKey
		- niTCBPosKey
	- niRotKey
		- niBezRotKey
		- niTCBRotKey
		- niEulerRotKey
- niTextKey


```mermaid
graph LR
    A(NiObject) --> B(NiAccumulator);
    B --> C(NiClusterAccumulator);
    C --> D(NiAlphaAccumulator);

    A --> E(NiExtraData);
    E --> F(BrickNiExtraData);
    E --> G(TES3ObjectExtraData);
    E --> H(NiStringExtraData);
    E --> I(NiTextKeyExtraData);
    E --> J(NiVertWeightsExtraData);

    A --> K(NiObjectNET);
    K --> L(NiDynamicEffect);
    L --> M(NiLight);
    M --> N(NiPointLight);
    N --> O(NiSpotLight);

    classDef notExposed fill:#ADFF2F,stroke:#333,stroke-width:2px;
    class B,C,D notExposed;

    classDef notResearched fill:#40E0D0,stroke:#333,stroke-width:2px;
    class F,J notResearched;
```