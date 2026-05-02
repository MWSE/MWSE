# The Hierarchy of NetImmerse Classes

This page puts into relation various classes that compromise Morrowind's scene graph. Some of these classes are abstract, meaning they aren't directly found in the scene graph but are used by other classes to inherit common variables and methods. The game's scene graph can be inspected by entering ssg (show scene graph) command in the console. Morrowind's engine is Gamebryo, which was before known as NetImmerse. That's why these classes have the prefix ni in their name. These classes in MWSE follow the naming as in the vanilla engine. But, some classes contain information about the scene graph in MWSE and have tes3 prefixes such as `tes3vector3`.

The primary building blocks of the scene graph are called nodes. Each node has one parent and zero or more child nodes. For example, the base node of Morrowind's world scene graph, named `worldRoot`, and all the game world objects are represented by nodes that are children of the `worldRoot` node.

Note that the relations described here don't reflect how these types are organized in the scene graph. For instance, the `worldRoot` node mentioned earlier is of type `niNode`, but is the root node of the world's scene graph, while the base class of most types described here is `niObject`.

The types are arranged into functional groupings. Within each grouping, indentation implies inheritance from the class of the previous indentation level. The color of the text carries the following meanings:

 - <span style = "color:Turquoise">Turquoise - Classes read by MWSE but not yet exposed to Lua scripts</span>
 - <span style = "color:YellowGreen">Green - Classes present in Morrowind but not yet read by MWSE</span>
 - All the other classes in standard text color in this list are available in Lua scripts

---

### niMain

#### Basic Foundation Types
 - niColor
 - niColorA
 - niPackedColor
 - niRTTI

#### Basic Math and Geometry Classes
 - niBound
 - tes3boundingBox
 - tes3matrix33
 - tes3vector2
 - tes3vector3
 - niQuaternion
 - tes3transform

#### Templated Container Classes
 - niTArray - not exposed as a type but can be iterated as a standard Lua array.
 - niLinkedList
	- niDynamicEffectLinkedList
	- niNodeLinkedList
	- niPropertyLinkedList

#### Animation Classes
 - niObject
	- niTimeController
	- niObjectNET
		- niAVObject
			- niNode
				- <span  style = "color:YellowGreen">niBSAnimationManager</span>
				- niBSAnimationNode
					- niBSParticleNode
		- <span  style = "color:YellowGreen">niSequenceStreamHelper</span>

#### Texturing Classes
- niObject
	- <span  style = "color:YellowGreen">niBltSource</span>
	- niObjectNET
		- niTexture
			- niRenderedTexture
				- <span  style = "color:YellowGreen">niRenderedCubeMap</span >
			- niSourceTexture
	- <span  style = "color:YellowGreen">niPalette</span>
	- niPixelData
- <span style = "color:Turquoise">niPixelFormat</span>

#### Object Depth Sorting Classes
- niObject
	- <span  style = "color:Turquoise">niAccumulator</span>
		- <span  style = "color:YellowGreen">niClusterAccumulator</span>
			- <span  style = "color:YellowGreen">niAlphaAccumulator</span>

#### Geometry Data Classes
- niObject
	- niGeometryData
		- niLinesData
		- niTriBasedGeomData
			- niParticlesData
				- niAutoNormalsParticlesData
				- niRotatingParticlesData
			- niTriShapeData
				- <span  style = "color:YellowGreen">niTriShapeDynamicData</span>
			- <span  style = "color:YellowGreen">niTriStripsData</span>

#### Geometry Rendering Classes
- niObject
	- niObjectNET
		- niAVObject
			- <span style = "color:Turquoise">niGeometry</span>
				- niLines
				- <span style = "color:Turquoise">niTriBasedGeometry</span>
					- niTriShape
					- <span  style = "color:YellowGreen">niTriStrips</span>
					- niParticles
						- niAutoNormalParticles
						- niRotatingParticles
	- <span style = "color:Turquoise">niScreenPolygon</span>
	- niSkinData
	- niSkinDataBoneData
	- niSkinDataBoneDataVertexWeight
	- niSkinInstance
	- niSkinPartition
	- niSkinPartitionPartition

#### Scene Graph Organization Classes
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
				- niSortAdjustNode
				- niSwitchNode
					- <span  style = "color:YellowGreen">niFltAnimationNode</span>
					- <span  style = "color:YellowGreen">niLODNode</span>

#### Lightning and Effects Classes
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

#### Rendering and Property Classes
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
	- niRenderer
		- <span  style = "color:YellowGreen">niDX8Renderer</span>

---

### niParticle

#### Particle System Classes
- niObject
	- <span  style = "color:YellowGreen">niEmitterModifier</span>
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
- niPerParticleData

---

### niCollision

#### Collision Classes
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

### niAnimation

#### Animation Classes
- niObject
	- niColorData
	- niExtraData - this type is not related to animation by itself.
		- niTextKeyExtraData
	- <span  style = "color:Turquoise">niFloatData</span>
	- niKeyframeData
	- <span  style = "color:YellowGreen">niMorphData</span>
	- <span style = "color:Turquoise">niPosData</span>
	- niSequence
	- niTimeController
		- <span style = "color:Turquoise">niFlipController</span>
		- <span  style = "color:YellowGreen">niFloatController</span>
			- <span  style = "color:YellowGreen">niAlphaController</span>
			- <span  style = "color:YellowGreen">niRollController</span>
		- niKeyframeController
		- niKeyframeManager
		- <span  style = "color:YellowGreen">niLightColorController</span>
		- niLookAtController
		- <span  style = "color:YellowGreen">niMaterialColorController</span>
		- <span  style = "color:YellowGreen">niMorpherController</span>
			- <span  style = "color:YellowGreen">niGeomMorpherController</span>
		- niParticleSystemController
			- <span  style = "color:YellowGreen">niBSPArrayController</span>
		- niPathController
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

## Diagrammatic representation

```mermaid
%%{
	init: {
		'er': {
			'diagramPadding': 5,
			'entityPadding': 5
		},
		'flowchart': {
			'diagramPadding': 2,
			'curve': 'monotoneX'
		}
	}
}%%

graph LR
	A(niObject) --> B(niAccumulator);
		B --> BA(niClusterAccumulator);
			BA --> BAA(niAlphaAccumulator);

	A --> C(niExtraData);
		C --> CA(BrickNiExtraData);
		C --> CB(TES3ObjectExtraData);
		C --> CC(niStringExtraData);
		C --> CD(niTextKeyExtraData);
		C --> CE(niVertWeightsExtraData);

	A --> D(niGeometryData);
		D --> DA(niLinesData);
		D --> DB(niParticlesData);
			DB --> DBA(niAutoNormalParticlesData);
			DB --> DBB(niRotatingParticlesData);
	subgraph Scroll down for more info
		D --> DC(niTriBasedGeomData);
	end

	A --> E(niObjectNET);

	subgraph Scroll down for more info
		E --> EA(niAVObject);
		E --> ED(niTexture);
	end

		E --> EC(niSequenceStreamHelper);

		E --> EB(niProperty);
			EB --> EBA(niAlphaProperty);
			EB --> EBB(niDitherProperty);
			EB --> EBC(niFogProperty);
			EB --> EBD(niMaterialProperty);
			EB --> EBE(niRendererSpecificProperty);
			EB --> EBF(niShadeProperty);
			EB --> EBG(niSpecularProperty);
			EB --> EBH(niStencilProperty);
			EB --> EBI(niTexturingProperty);
			EB --> EBJ(niVertexColorProperty);
			EB --> EBK(niWireframeProperty);
			EB --> EBL(niZBufferProperty);

	A --> F(niParticleModifier);
		F --> FA(niGravity);
		F --> FB(niParticleBomb);
		F --> FC(niParticleCollider);
			FC --> FCA(niPlanarCollider);
			FC --> FCB(niSphericalCollider);
		F --> FD(niParticleColorModifier);
		F --> FE(niParticleGrowFade);
		F --> FF(niParticleRotation);

	A --> G(niRenderer);
		G --> GA(niDX8Renderer);

	A --> H(niTimeController);
		H --> HA(niFlipController);
		H --> HB(niFloatController);
			HB --> HBA(niAlphaController);
			HB --> HBB(niRollController);
		H --> HC(niKeyframeController);
		H --> HD(niKeyframeManager);
		H --> HE(niLightColorController);
		H --> HF(niLookAtController);
		H --> HG(niMaterialColorController);
		H --> HH(niMorpherController);
			HH --> HHA(niGeomMorpherController);
		H --> HI(niParticleSystemController);
			HI --> HIA(niBSPArrayController);
		H --> HJ(niPathController);
		H --> HK(niUVController);
		H --> HL(niVisController);

	A --> I(niBltSource);
	A --> J(niColorData);
	A --> K(niEmitterModifier);
	A --> L(niFloatData);
	A --> M(niKeyframeData);
	A --> N(niMorphData);
	A --> O(niPalette);
	A --> P(niPixelData);
	A --> Q(niPosData);
	A --> R(niScreenPolygon);
	A --> S(niSequence);
	A --> T(niSkinData);
	A --> U(niSkinInstance);
	A --> V(niSkinPartition);
	A --> W(niUVData);
	A --> X(niVisData);

	%% Default style
	classDef default color: #fafafa, fill:#434659, stroke:#e6e6e6, stroke-width:2px;

	%% Turquoise
	classDef notExposed color:#000, fill: #40E0D0, stroke: #333, stroke-width: 2px;
	class B,HA,HK,L,Q,R,W notExposed;

	%% YellowGreen
	classDef notResearched color:#000, fill: #ADFF2F, stroke: #333, stroke-width: 2px;
	class BA,BAA,CA,CE,EBB,EBE,EBF,EBG,EBK,EC,GA,HB,HBA,HBB,HE,HG,HH,HHA,HIA,HL,I,K,N,O,X notResearched;
```

```mermaid
%%{
	init: {
		'er': {
			'diagramPadding': 5,
			'entityPadding': 5
		},
		'flowchart': {
			'diagramPadding': 2,
			'curve': 'monotoneX'
		}
	}
}%%

graph LR
	ED(niTexture);
		ED --> EDA(niRenderedTexture);
			EDA --> EDAA(niRenderedCubeMap);
		ED --> EDB(niSourceTexture);

	%% Default style
	classDef default color: #fafafa, fill:#434659, stroke:#e6e6e6, stroke-width:2px;

	%% YellowGreen
	classDef notResearched color:#000, fill: #ADFF2F, stroke: #333, stroke-width: 2px;
	class EDAA notResearched;
```

```mermaid
%%{
	init: {
		'er': {
			'diagramPadding': 5,
			'entityPadding': 5
		},
		'flowchart': {
			'diagramPadding': 2,
			'curve': 'monotoneX'
		}
	}
}%%

graph LR
	DC(niTriBasedGeomData);
		DC --> DCA(niTriShapeData);
			DCA --> DCAA(niTriShapeDynamicData);
		DC --> DCB(niTriStripsData);

	%% Default style
	classDef default color: #fafafa, fill:#434659, stroke:#e6e6e6, stroke-width:2px;

	%% YellowGreen
	classDef notResearched color:#000, fill: #ADFF2F, stroke: #333, stroke-width: 2px;
	class DCAA,DCB notResearched;
```

```mermaid
%%{
	init: {
		'er': {
			'diagramPadding': 5,
			'entityPadding': 5
		},
		'flowchart': {
			'diagramPadding': 2,
			'curve': 'monotoneX'
		}
	}
}%%

graph LR
	EA(niAVObject);
	EA --> EAA(niCamera);

	EA --> EAB(niDynamicEffect);
		EAB --> EABA(niTextureEffect);
		EAB --> EABB(niLight);
			EABB --> EABBA(niAmbientLight);
			EABB --> EABBB(niDirectionalLight);
			EABB --> EABBC(niPointLight);
				EABBC --> EABBCA(niSpotLight);

	EA --> EAC(niGeometry);
		EAC --> EACA(niLines);
		EAC --> EACB(niParticles);
			EACB --> EACBA(niAutoNormalParticles);
			EACB --> EACBB(niRotatingParticles);
		EAC --> EACC(niTriBasedGeometry);
			EACC --> EACCA(niTriShape);
			EACC --> EACCB(niTriStrips);

	EA --> EAD(niNode);
		EAD --> EADA(AvoidNode);
		EAD --> EADB(RootCollisionNode);
		EAD --> EADC(niBillboardNode);
		EAD --> EADD(niBSAnimationManager);
		EAD --> EADE(niBSPNode);
		EAD --> EADF(niCollisionSwitch);
		EAD --> EADG(niSortAdjustNode);
		EAD --> EADH(niBSAnimationNode);
			EADH --> EADHA(niBSParticleNode);
		EAD --> EADI(niSwitchNode);
			EADI --> EADIA(niFltAnimationNode);
			EADI --> EADIB(niLODNode);

	%% Default style
	classDef default color: #fafafa, fill:#434659, stroke:#e6e6e6, stroke-width:2px;

	%% Turquoise
	classDef notExposed color:#000, fill: #40E0D0, stroke: #333, stroke-width: 2px;
	class EAC,EACC notExposed;

	%% YellowGreen
	classDef notResearched color:#000, fill: #ADFF2F, stroke: #333, stroke-width: 2px;
	class EACCB,EADA,EADB,EADD,EADE,EADIA,EADIB notResearched;
```
