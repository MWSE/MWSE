#pragma once

// Engine function addresses for TESConstructionSet.exe (the Construction Set editor binary).
// Consumers select the appropriate target via a thin per-project NIConfig.h shim.

// NI::Matrix33
#define SE_NI_MATRIX33_FNADDR_ADDMATRIX 0x5E1F20
#define SE_NI_MATRIX33_FNADDR_FROMEULERXYZ 0x5E3760
#define SE_NI_MATRIX33_FNADDR_INVERSE 0x5E23A0
#define SE_NI_MATRIX33_FNADDR_INVERSERAW 0x5E22B0
#define SE_NI_MATRIX33_FNADDR_MULTIPLYMATRIX 0x5E2060
#define SE_NI_MATRIX33_FNADDR_MULTIPLYSCALAR 0x5E2170
#define SE_NI_MATRIX33_FNADDR_MULTIPLYVECTOR 0x5E21F0
#define SE_NI_MATRIX33_FNADDR_REORTHOGONALIZE 0x5E26D0
#define SE_NI_MATRIX33_FNADDR_SUBTRACTMATRIX 0x5E1FC0
#define SE_NI_MATRIX33_FNADDR_TESTEQUAL 0x5E1E90
#define SE_NI_MATRIX33_FNADDR_TOEULERXYZ 0x5E3110
#define SE_NI_MATRIX33_FNADDR_TOIDENTITY 0x5E1C60
#define SE_NI_MATRIX33_FNADDR_TOROTATIONX 0x5E1CC0
#define SE_NI_MATRIX33_FNADDR_TOROTATIONXYZ 0x5E1D80
#define SE_NI_MATRIX33_FNADDR_TOROTATIONY 0x5E1D00
#define SE_NI_MATRIX33_FNADDR_TOROTATIONZ 0x5E1D40
#define SE_NI_MATRIX33_FNADDR_TRANSPOSE 0x5E23E0
// Despite the macro name (kept for migration-compat), this points at
// NiQuaternion::ToRotation (quat->matrix). Engine signature is
// (const Quaternion* this, Matrix33* rotation), NOT (Matrix33*, Quaternion*).
#define SE_NI_MATRIX33_FNADDR_CTOR_FROMQUATERNION 0x5F3F80

// NI::Quaternion
#define SE_NI_QUATERNION_FNADDR_FROMANGLEAXIS 0x5F3E80
#define SE_NI_QUATERNION_FNADDR_FROMROTATION 0x5F4050
#define SE_NI_QUATERNION_FNADDR_INVERT 0x5F37C0
#define SE_NI_QUATERNION_FNADDR_EXP 0x5F30D0
#define SE_NI_QUATERNION_FNADDR_LOG 0x5F37F0
#define SE_NI_QUATERNION_FNADDR_MULTIPLYQUATERNION 0x5F3730
#define SE_NI_QUATERNION_FNADDR_SLERP 0x5F3880
#define SE_NI_QUATERNION_FNADDR_TOANGLEAXIS 0x5F3EB0

// NI::Object — CS names this NI::RefObject; same class semantically.
// Sizes match MW NiObject::ctor (0x23) / dtor (0x14) exactly. The migration
// previously had CTOR/DTOR pointing at NiObjectNET's larger versions
// (0x5B27B0 / 0x5B2860, sizes 0x87 / 0xc7) — calling them on a bare
// NI::Object allocation (only 8 bytes: vtbl + refcount) would have written
// past its end and corrupted heap memory.
#define SE_NI_OBJECT_FNADDR_CREATECLONE 0x5DCC70
#define SE_NI_OBJECT_FNADDR_CTOR 0x5DCC00
#define SE_NI_OBJECT_FNADDR_DTOR 0x5DCC50
#define SE_NI_OBJECT_FNADDR_RELEASE 0x40A710
#define SE_NI_OBJECT_FNADDR_REGISTERSTREAMABLES 0x5DCF70

// NI::ObjectNET
#define SE_NI_OBJECTNET_FNADDR_ADDEXTRADATA 0x5B2AA0
#define SE_NI_OBJECTNET_FNADDR_PREPENDCONTROLLER 0x5B2CD0
#define SE_NI_OBJECTNET_FNADDR_REMOVEALLCONTROLLERS 0x5B2ED0
#define SE_NI_OBJECTNET_FNADDR_REMOVEALLEXTRADATA 0x5B2CA0
#define SE_NI_OBJECTNET_FNADDR_REMOVECONTROLLER 0x5B2D80
#define SE_NI_OBJECTNET_FNADDR_REMOVEEXTRADATA 0x5B2B50
#define SE_NI_OBJECTNET_FNADDR_SETFLAG 0x44A130
#define SE_NI_OBJECTNET_FNADDR_SETNAME 0x5B2A80

// NI::AVObject
#define SE_NI_AVOBJECT_FNADDR_ATTACHPROPERTY 0x44CF00
#define SE_NI_AVObject_FNADDR_DETACHPROPERTYBYTYPE 0x5DADD0
#define SE_NI_AVOBJECT_FNADDR_SETFLAG 0x44A130
#define SE_NI_AVOBJECT_FNADDR_INTERSECTBOUNDS 0x5DBAD0
#define SE_NI_AVOBJECT_FNADDR_SETMODELSPACEABV 0x5DB730
#define SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX 0x462C70
#define SE_NI_AVOBJECT_FNADDR_UPDATE 0x5DAFB0
#define SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS 0x5DB340
#define SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES 0x5DB090

// NI::Matrix33 identity matrix global (CS.exe address not yet known)
#define SE_NI_MATRIX33_GLOBADDR_IDENTITY 0x0

// NI::Node
#define SE_NI_NODE_FNADDR_ATTACHEFFECT 0x5B71D0
#define SE_NI_NODE_FNADDR_CTOR 0x5B6100
#define SE_NI_NODE_FNADDR_DETACHALLEFFECTS 0x5B72B0
#define SE_NI_NODE_FNADDR_DETACHEFFECT 0x5B7210
#define SE_NI_NODE_FNADDR_FINDINTERSECTIONS 0x5B7F10
#define SE_NI_NODE_FNADDR_LINKOBJECT 0x5B87E0

// NI::CollisionSwitch vTable address (CS.exe address not yet known)
#define SE_NI_COLLISIONSWITCH_VTBL 0x0

// NI::Particles (CS.exe addresses not yet known)
#define SE_NI_PARTICLES_VTBL 0x0
#define SE_NI_PARTICLESDATA_FNADDR_CTOR 0x5C17F0

// NI::AutoNormalParticles (CS.exe addresses not yet known)
#define SE_NI_AUTONORMALPARTICLES_VTBL 0x0
#define SE_NI_AUTONORMALPARTICLESDATA_FNADDR_CTOR 0x5C2C50

// NI::PointLight vTable address (CS.exe address not yet known)
#define SE_NI_POINTLIGHT_VTBL 0x0

// NI::KeyframeData (CS.exe address not yet known)
#define SE_NI_KEYFRAMEDATA_FNADDR_REPLACESCALEDATA 0x6035C0

// NI::SortAdjustNode vTable address (CS.exe address not yet known)
#define SE_NI_SORTADJUSTNODE_VTBL 0x0

// NI::PixelFormat (D3D8-bound; CSSE has no D3D8 link, addresses irrelevant)
#define SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMFORMAT 0x5E8650
#define SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMRGBA 0x5E83D0
#define SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMPALETTE 0x5E85E0
#define SE_NI_PIXELFORMAT_FNADDR_CTOR_FROMBUMPLUMA 0x5E8940
#define SE_NI_PIXELFORMAT_FNADDR_GETD3DFORMAT 0x594B10

// NI::Sequence (CS.exe address not yet known)
#define SE_NI_SEQUENCE_FNADDR_DTOR 0x605C20

// NI::TimeController vTable template (CS.exe address not yet known)
#define SE_NI_TIMECONTROLLER_VTBL_TEMPLATE 0x751200

// NI::AnimationKey global table (CS.exe address not yet known)
#define SE_NI_ANIMATIONKEY_GLOBADDR_FILLDERIVEDVALUESFUNCTIONS 0x0
// __cdecl key-data-size helpers (NiFloatKey_static / NiPosKey_static GetDataSize)
#define SE_NI_FLOATDATA_FNADDR_GETKEYSIZE 0x61C950
#define SE_NI_POSDATA_FNADDR_GETKEYSIZE 0x61C920

// NI::BillboardNode (vTable CS.exe address not yet known)
#define SE_NI_BILLBOARDNODE_VTBL 0x0
#define SE_NI_BILLBOARDNODE_FNADDR_ROTATETOCAMERA 0x5C39F0

// NI::BSAnimationNode / BSParticleNode (BSParticleNode VTBL not yet known)
#define SE_NI_BSANIMATIONNODE_FNADDR_CTOR 0x5EBF90
#define SE_NI_BSPARTICLENODE_VTBL 0x0

// NI::TextureEffect
#define SE_NI_TEXTUREEFFECT_FNADDR_CTOR 0x5D5050

// NI::RTTI
#define SE_NI_RTTI_ctor 0x5E9F90

// NI::CollisionGroup engine fns (CS.exe addresses not yet known)
// NiCollisionGroup is absent from TESConstructionSet.exe — the editor
// has no runtime collision-group manager (only NiAVObject::Test/Find
// Collisions and NI::CollisionSwitch are present). Verified by string
// search ("collisiongroup" → 0 hits) and func name search.
#define SE_NI_COLLISIONGROUP_FNADDR_CONTAINSCOLLIDER 0x0
#define SE_NI_COLLISIONGROUP_FNADDR_ADDCOLLIDER 0x0
#define SE_NI_COLLISIONGROUP_FNADDR_REMOVECOLLIDER 0x0

// NI::SourceTexture engine fns + global (CS.exe addresses not yet known)
#define SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPATH 0x5CFAD0
#define SE_NI_SOURCETEXTURE_FNADDR_CREATEFROMPIXELDATA 0x5CFC20
#define SE_NI_SOURCETEXTURE_GLOBADDR_BPRELOAD 0x0

// NI::UVController engine fn (CS.exe address not yet known)
#define SE_NI_UVCONTROLLER_FNADDR_COPY 0x61AF90

// NI::AVObject engine fn — value cross-referenced from the older lowercase
// alias SE_NI_AVObject_FNADDR_DETACHPROPERTYBYTYPE (line 54), which is a
// case-typo carried over for backwards compat.
#define SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE 0x5DADD0

// NI::TimeController engine functions (CS.exe addresses not yet known)
#define SE_NI_TIMECONTROLLER_FNADDR_CTOR 0x5E9030
// Actual dtor body — extracted from NI::TimeController::deleting_dtor at
// 0x5E90E0 which calls sub_5E9100. SharedSE expects single-arg signature.
#define SE_NI_TIMECONTROLLER_FNADDR_DTOR 0x5E9100
#define SE_NI_TIMECONTROLLER_FNADDR_LOADBINARY 0x5E98D0
#define SE_NI_TIMECONTROLLER_FNADDR_REGISTERSTREAMABLES 0x5E99B0
#define SE_NI_TIMECONTROLLER_FNADDR_LINKOBJECT 0x5E9950
#define SE_NI_TIMECONTROLLER_FNADDR_SAVEBINARY 0x5E99E0
#define SE_NI_TIMECONTROLLER_FNADDR_ISEQUAL 0x5E9A80
#define SE_NI_TIMECONTROLLER_FNADDR_ADDVIEWERSTRINGS 0x5E9B10
#define SE_NI_TIMECONTROLLER_FNADDR_START 0x5E91A0
#define SE_NI_TIMECONTROLLER_FNADDR_STOP 0x5E91D0
#define SE_NI_TIMECONTROLLER_FNADDR_SETTARGET 0x5E9740
#define SE_NI_TIMECONTROLLER_FNADDR_COMPUTESCALEDTIME 0x5E9200

// NI::KeyframeManager (CS.exe addresses not yet known)
#define SE_NI_KEYFRAMEMANAGER_FNADDR_ACTIVATESEQUENCE 0x607FD0
#define SE_NI_KEYFRAMEMANAGER_FNADDR_DEACTIVATESEQUENCE 0x6083B0

// NI light-radius test (CS.exe address not yet known)
// LightRadiusTest is a Morrowind-runtime helper (game_dynamicLightTestHelper)
// called only from game_dynamicLightTest. CS has no game-runtime light culling
// path — verified by string search ("dynamicLight" / "lightRadius" → 0 hits).
#define SE_NI_FNADDR_LIGHTRADIUSTEST 0x0

// NI::DynamicEffect
#define SE_NI_DYNAMICEFFECT_FNADDR_ATTACHAFFECTEDNODE 0x5E5650
#define SE_NI_DYNAMICEFFECT_FNADDR_CTOR 0x5E53A0
#define SE_NI_DYNAMICEFFECT_FNADDR_DETACHAFFECTEDNODE 0x5E5690
#define SE_NI_FNADDR_CLEARDYNAMICEFFECTNODES 0x402824

// NI::Property
#define SE_NI_PROPERTY_FNADDR_CTOR 0x44A170
// Property::dtor is a thunk-of-thunk that resolves to the ObjectNET dtor
// body (NI::ObjectNET::dtor at 0x5B2860); 0x4043D6 is the entry point.
#define SE_NI_PROPERTY_FNADDR_DTOR 0x4043D6
// NI::Property::setPropertyFlag in CS — same address used for the
// inherited-via-ObjectNET SetFlag macro above (CS hoisted SetFlag onto
// the ObjectNET interface; impl lives on Property).
#define SE_NI_PROPERTY_FNADDR_SETFLAG 0x44A130
// Cross-referenced from Morrowind's setFlagBitField at 0x408A10 and the CS
// Property cluster (0x44A0F0..0x44A1F0). Body matches Morrowind's bitfield
// math: flags = (value<<index) | (flags & ~(mask<<index)).
#define SE_NI_PROPERTY_FNADDR_SETFLAGBITFIELD 0x44A0F0

// NI::AlphaProperty
#define SE_NI_ALPHAPROPERTY_VTBL 0x6732E8

// NI::StencilProperty
// CS.exe address not yet known.
#define SE_NI_STENCILPROPERTY_VTBL 0x0

// NI::TexturingProperty::Map
// CS inlines NiTexturingProperty::Map ctors directly into the parent
// TexturingProperty ctor / Clone / CreateFromStream paths (e.g. sub_5D3150
// at 0x5D31D0-0x5D3236). No standalone Map ctor functions exist; vtable
// 0x67A7F4 / off_6758A8 is written inline. Both stay 0x0.
#define SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTOR 0x0
#define SE_NI_TEXTURINGPROPERTY_MAP_FNADDR_CTORWITHPARAMS 0x0

// NI::TexturingProperty::BumpMap
#define SE_NI_TEXTURINGPROPERTY_BUMPMAP_VTBL 0x0

// NI::TexturingProperty
#define SE_NI_TEXTURINGPROPERTY_FNADDR_CTOR 0x5D2F10
#define SE_NI_TEXTURINGPROPERTY_FNADDR_DTOR 0x5D30D0

// NI::VertexColorProperty
#define SE_NI_VERTEXCOLORPROPERTY_VTBL 0x673170

// NI::ZBufferProperty
#define SE_NI_ZBUFFERPROPERTY_VTBL 0x6731F0

// NI::Light
#define SE_NI_LIGHT_FNADDR_CTOR 0x5EA4C0

// NI::SkinInstance
#define SE_NI_SKININSTANCE_FNADDR_DEFORM 0x5F08A0

// NI::TriShape
#define SE_NI_TRISHAPE_FNADDR_CTOR 0x05D6D10

// NI::TriShapeData
#define SE_NI_TRISHAPEDATA_FNADDR_CREATE 0x5D6260

// NI::Lines
#define SE_NI_LINES_FNADDR_CTOR 0x5CB2A0

// NI::GeometryData
#define SE_NI_GEOMETRYDATA_FNADDR_LOADBINARY 0x5ED2C0

// NI::PixelData
// CS.exe addresses not yet known.
#define SE_NI_PIXELDATA_FNADDR_CTOR_ARGS 0x5C5120
#define SE_NI_PIXELFORMAT_GLOBADDR_RGBA32 0x0

// NI::StringExtraData
#define SE_NI_STRINGEXTRADATA_FNADDR_CTOR 0x5D25F0
#define SE_NI_STRINGEXTRADATA_FNADDR_DTOR 0x5D2D80

// NI::Texture
#define SE_NI_TEXTURE_FNADDR_DEFAULT_PREFS 0x6FC710

// NI::TimeController
// CS.exe address not yet known. CSSE call sites (e.g.
// SharedSE/NIFlipController.cpp::copy) must throw not_implemented_exception
// when this is 0; otherwise a call would jump to whatever happens to live at
// 0x6FC8E0 in TESConstructionSet.exe -- almost certainly wrong.
#define SE_NI_TIMECONTROLLER_FNADDR_COPY 0x5E97C0

// NI::Camera
#define SE_NI_CAMERA_FNADDR_CLEAR 0x5BAB70
#define SE_NI_CAMERA_FNADDR_CLICK 0x5BABA0
#define SE_NI_CAMERA_FNADDR_CTOR 0x5BA5F0
#define SE_NI_CAMERA_FNADDR_DTOR 0x5BA900
// LookAtWorldPoint CS counterpart not yet located (MW size 0x3DA didn't
// match cleanly in the 0x5BA-0x5BD Camera cluster — possibly inlined or
// renamed in the editor build).
#define SE_NI_CAMERA_FNADDR_LOOKATWORLDPOINT 0x0
#define SE_NI_CAMERA_FNADDR_SWAPBUFFERS 0x5BAD20
#define SE_NI_CAMERA_FNADDR_WINDOWPOINTTORAY 0x5BC680
#define SE_NI_CAMERA_FNADDR_WORLDPOINTTOSCREENPOINT 0x5BB870

// NI::Pick
#define SE_NI_PICK_FNADDR_CLEARRESULTS 0x5B34E0
#define SE_NI_PICK_FNADDR_CTOR 0x5B3350
#define SE_NI_PICK_FNADDR_DTOR 0x5B3400
#define SE_NI_PICK_FNADDR_PICKOBJECTS 0x5B35B0

// NI::Stream
#define SE_NI_STREAM_FNADDR_CTOR 0x5AD3D0
#define SE_NI_STREAM_FNADDR_DTOR 0x5AD640
// GETCOPIEDOBJECT is a global pointer (NiObject cloning hash), not a function.
#define SE_NI_STREAM_FNADDR_GETCOPIEDOBJECT 0x6D7A70
#define SE_NI_STREAM_FNADDR_GETLINKID 0x5AE020
#define SE_NI_STREAM_FNADDR_GETLINKOBJECT 0x5ADF30
#define SE_NI_STREAM_FNADDR_INSERTOBJECT 0x5AD840
#define SE_NI_STREAM_FNADDR_LOAD 0x5AEAC0
#define SE_NI_STREAM_FNADDR_READSTRING 0x5AFB60
#define SE_NI_STREAM_FNADDR_REGISTERLOADER 0x5AED50
#define SE_NI_STREAM_FNADDR_SAVE 0x5AF100
#define SE_NI_STREAM_FNADDR_WRITESTRING 0x5AFC00

// NI::Stream::LoadingObject
#define SE_NI_STREAM_LOADINGOBJECT_VTBL 0x0

// NI::Bound
#define SE_NI_BOUND_FNADDR_COMPUTEFROMDATA 0x5B13F0

// NI::BoxBoundingVolume (CS.exe address not yet known)
#define SE_NI_BOXBOUNDINGVOLUME_FNADDR_CREATE 0x57B040

// NI::TriBasedGeometry
// CS.exe addresses not yet known. CSSE call sites must throw not_implemented_exception
// when these are 0; otherwise call would jump to whatever happens to live at the
// Morrowind address in TESConstructionSet.exe.
// CS has only a no-arg NI::TriBasedGeom::ctor at 0x5E6400; the args
// overload (passing NiGeometryData* through to NiGeometry::ctor_args)
// doesn't exist in the editor binary — left as 0x0.
#define SE_NI_TRIBASEDGEOMETRY_FNADDR_CTORFROMDATA 0x0
#define SE_NI_TRIBASEDGEOMETRY_FNADDR_FINDINTERSECTIONS 0x5E68B0
#define SE_NI_FNADDR_FINDINTERSECTRAYWITHTRIANGLE 0x5E64F0

// NI::Geometry (base of TriBasedGeometry)
#define SE_NI_GEOMETRY_FNADDR_LINKOBJECT 0x5E4AF0

// NI::TriShape vTable address (CS.exe address not yet known)
#define SE_NI_TRISHAPE_VTBL 0x0
