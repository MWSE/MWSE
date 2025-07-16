#include "NIObject.h"

#include "NIDefines.h"

#include "NIRTTI.h"
#include "NIStream.h"

#include "MemoryUtil.h"

#include "NIAmbientLight.h"
#include "NIAnimationData.h"
#include "NIAVObject.h"
#include "NIBillboardNode.h"
#include "NIBSAnimationNode.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NIDirectionalLight.h"
#include "NIExtraData.h"
#include "NIKeyframeController.h"
#include "NILines.h"
#include "NILookAtController.h"
#include "NINode.h"
#include "NIObjectNET.h"
#include "NIParticleModifier.h"
#include "NIParticles.h"
#include "NIParticleSystemController.h"
#include "NIPathController.h"
#include "NIPixelData.h"
#include "NIPointLight.h"
#include "NIProperty.h"
#include "NISkinInstance.h"
#include "NISortAdjustNode.h"
#include "NISourceTexture.h"
#include "NISpotLight.h"
#include "NISwitchNode.h"
#include "NITextureEffect.h"
#include "NITimeController.h"
#include "NITriShape.h"

#include "LuaObjectInvalidatedEvent.h"

#include "LuaManager.h"

#include "MWSEConfig.h"

namespace NI {
	const auto NI_Object_ctor = reinterpret_cast<Object * (__thiscall*)(Object *)>(0x6E98A0);
	Object::Object() {
		NI_Object_ctor(this);
	}

	const auto NI_Object_dtor = reinterpret_cast<Object * (__thiscall*)(Object *)>(0x6E98F0);
	Object::~Object() {
		NI_Object_dtor(this);
	}

	void * Object::operator new(size_t size) {
		return mwse::tes3::_new(size);
	}

	void Object::operator delete(void * address) {
		mwse::tes3::_delete(address);
	}

	RTTI * Object::getRunTimeTypeInformation() const {
		return vTable.asObject->getRTTI(this);
	}

	bool Object::registerStreamables(Stream* stream) {
		return vTable.asObject->registerStreamables(this, stream);
	}

	void Object::loadBinary(Stream* stream) {
		vTable.asObject->loadBinary(this, stream);
	}

	bool Object::isEqual(Object* other) const {
		return vTable.asObject->isEqual(this, other);
	}

	Object* Object::createClone() {
		return reinterpret_cast<Object * (__thiscall*)(Object*)>(0x6E9910)(this);
	}

	void Object::release() {
		reinterpret_cast<void(__thiscall *)(Object *)>(0x404630)(this);
	}

	bool Object::isOfType(const RTTI* type) const {
		return (vTable.asObject->getRTTI(this) == type);
	}

	bool Object::isInstanceOfType(const RTTI* type) const {
		for (const RTTI* rtti = vTable.asObject->getRTTI(this); rtti; rtti = rtti->baseRTTI) {
			if (rtti == type) {
				return true;
			}
		}

		return false;
	}

	bool Object::saveBinary(const char* filename) {
		Stream stream;
		stream.insertObject(this);
		return stream.save(filename);
	}

	std::string Object::toString() const {
		std::ostringstream ss;
		const char* name = nullptr;
		if (isInstanceOfType(NI::RTTIStaticPtr::NiObjectNET)) {
			name = static_cast<const ObjectNET*>(this)->name;
		}
		ss << getRunTimeTypeInformation()->toString() << ":" << (name ? name : "(unnamed)");
		return std::move(ss.str());
	}

	std::string Object::toJson() const {
		std::ostringstream ss;
		const char* name = nullptr;
		if (isInstanceOfType(NI::RTTIStaticPtr::NiObjectNET)) {
			name = static_cast<const ObjectNET*>(this)->name;
		}
		ss << "\"" << getRunTimeTypeInformation()->toString() << ":" << (name ? name : "(unnamed)") << "\"";
		return std::move(ss.str());
	}

	static std::unordered_map<const Object*, sol::object> niObjectCache;

	sol::object Object::getOrCreateLuaObject(lua_State* L) {
		if (this == nullptr) {
			return sol::nil;
		}

		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();

		if (mwse::Configuration::KeepAllNetImmerseObjectsAlive) {
			auto cacheHit = niObjectCache.find(this);
			if (cacheHit != niObjectCache.end()) {
				auto result = cacheHit->second;
				return result;
			}
		}

		// Make sure we're looking at the main state.
		L = stateHandle.getState();

		// Loop through RTTI information until we find a type we like.
		auto currentRTTI = getRunTimeTypeInformation();
		sol::object ref = sol::nil;
		while (currentRTTI != nullptr && ref == sol::nil) {
			switch ((uintptr_t)currentRTTI) {
			case RTTIStaticPtr::NiAlphaProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<AlphaProperty*>(this)));
				break;
			case RTTIStaticPtr::NiAmbientLight:
				ref = sol::make_object_userdata(L, Pointer(static_cast<AmbientLight*>(this)));
				break;
			case RTTIStaticPtr::NiAutoNormalParticles:
				ref = sol::make_object_userdata(L, Pointer(static_cast<AutoNormalParticles*>(this)));
				break;
			case RTTIStaticPtr::NiAVObject:
				ref = sol::make_object_userdata(L, Pointer(static_cast<AVObject*>(this)));
				break;
			case RTTIStaticPtr::NiBillboardNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<BillboardNode*>(this)));
				break;
			case RTTIStaticPtr::NiBSAnimationNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<BSAnimationNode*>(this)));
				break;
			case RTTIStaticPtr::NiBSParticleNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<BSParticleNode*>(this)));
				break;
			case RTTIStaticPtr::NiCamera:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Camera*>(this)));
				break;
			case RTTIStaticPtr::NiCollisionSwitch:
				ref = sol::make_object_userdata(L, Pointer(static_cast<CollisionSwitch*>(this)));
				break;
			case RTTIStaticPtr::NiColorData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ColorData*>(this)));
				break;
			case RTTIStaticPtr::NiDirectionalLight:
				ref = sol::make_object_userdata(L, Pointer(static_cast<DirectionalLight*>(this)));
				break;
			case RTTIStaticPtr::NiExtraData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ExtraData*>(this)));
				break;
			case RTTIStaticPtr::NiFloatData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<FloatData*>(this)));
				break;
			case RTTIStaticPtr::NiFogProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<FogProperty*>(this)));
				break;
			case RTTIStaticPtr::NiGravity:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Gravity*>(this)));
				break;
			case RTTIStaticPtr::NiKeyframeController:
				ref = sol::make_object_userdata(L, Pointer(static_cast<KeyframeController*>(this)));
				break;
			case RTTIStaticPtr::NiKeyframeData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<KeyframeData*>(this)));
				break;
			case RTTIStaticPtr::NiLines:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Lines*>(this)));
				break;
			case RTTIStaticPtr::NiLookAtController:
				ref = sol::make_object_userdata(L, Pointer(static_cast<LookAtController*>(this)));
				break;
			case RTTIStaticPtr::NiMaterialProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<MaterialProperty*>(this)));
				break;
			case RTTIStaticPtr::NiNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Node*>(this)));
				break;
			case RTTIStaticPtr::NiObject:
				ref = sol::make_object_userdata(L, Pointer(this));
				break;
			case RTTIStaticPtr::NiObjectNET:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ObjectNET*>(this)));
				break;
			case RTTIStaticPtr::NiParticleBomb:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleBomb*>(this)));
				break;
			case RTTIStaticPtr::NiParticleCollider:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleCollider*>(this)));
				break;
			case RTTIStaticPtr::NiParticleColorModifier:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleColorModifier*>(this)));
				break;
			case RTTIStaticPtr::NiParticleGrowFade:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleGrowFade*>(this)));
				break;
			case RTTIStaticPtr::NiParticleModifier:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleModifier*>(this)));
				break;
			case RTTIStaticPtr::NiParticleRotation:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleRotation*>(this)));
				break;
			case RTTIStaticPtr::NiParticles:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Particles*>(this)));
				break;
			case RTTIStaticPtr::NiParticleSystemController:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ParticleSystemController*>(this)));
				break;
			case RTTIStaticPtr::NiPathController:
				ref = sol::make_object_userdata(L, Pointer(static_cast<PathController*>(this)));
				break;
			case RTTIStaticPtr::NiPlanarCollider:
				ref = sol::make_object_userdata(L, Pointer(static_cast<PlanarCollider*>(this)));
				break;
			case RTTIStaticPtr::NiPixelData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<PixelData*>(this)));
				break;
			case RTTIStaticPtr::NiPointLight:
				ref = sol::make_object_userdata(L, Pointer(static_cast<PointLight*>(this)));
				break;
			case RTTIStaticPtr::NiPosData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<PosData*>(this)));
				break;
			case RTTIStaticPtr::NiRotatingParticles:
				ref = sol::make_object_userdata(L, Pointer(static_cast<RotatingParticles*>(this)));
				break;
			case RTTIStaticPtr::NiSkinData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SkinData*>(this)));
				break;
			case RTTIStaticPtr::NiSkinInstance:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SkinInstance*>(this)));
				break;
			case RTTIStaticPtr::NiSkinPartition:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SkinPartition*>(this)));
				break;
			case RTTIStaticPtr::NiSortAdjustNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SortAdjustNode*>(this)));
				break;
			case RTTIStaticPtr::NiSourceTexture:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SourceTexture*>(this)));
				break;
			case RTTIStaticPtr::NiSphericalCollider:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SphericalCollider*>(this)));
				break;
			case RTTIStaticPtr::NiSpotLight:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SpotLight*>(this)));
				break;
			case RTTIStaticPtr::NiStencilProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<StencilProperty*>(this)));
				break;
			case RTTIStaticPtr::NiStringExtraData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<StringExtraData*>(this)));
				break;
			case RTTIStaticPtr::NiSwitchNode:
				ref = sol::make_object_userdata(L, Pointer(static_cast<SwitchNode*>(this)));
				break;
			case RTTIStaticPtr::TES3ObjectExtraData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<Tes3ExtraData*>(this)));
				break;
			case RTTIStaticPtr::NiTextureEffect:
				ref = sol::make_object_userdata(L, Pointer(static_cast<TextureEffect*>(this)));
				break;
			case RTTIStaticPtr::NiTexturingProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<TexturingProperty*>(this)));
				break;
			case RTTIStaticPtr::NiTextKeyExtraData:
				ref = sol::make_object_userdata(L, Pointer(static_cast<TextKeyExtraData*>(this)));
				break;
			case RTTIStaticPtr::NiTimeController:
				ref = sol::make_object_userdata(L, Pointer(static_cast<TimeController*>(this)));
				break;
			case RTTIStaticPtr::NiTriShape:
				ref = sol::make_object_userdata(L, Pointer(static_cast<TriShape*>(this)));
				break;
			case RTTIStaticPtr::NiVertexColorProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<VertexColorProperty*>(this)));
				break;
			case RTTIStaticPtr::NiZBufferProperty:
				ref = sol::make_object_userdata(L, Pointer(static_cast<ZBufferProperty*>(this)));
				break;
			}

			currentRTTI = currentRTTI->baseRTTI;
		}

		if (mwse::Configuration::KeepAllNetImmerseObjectsAlive) {
			if (ref != sol::nil) {
				niObjectCache[this] = ref;
			}
		}

		return ref;
	}

	void Object::clearCachedLuaObject(const Object* object) {
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();

		if (!niObjectCache.empty()) {
			// Clear any events that make use of this object.
			auto it = niObjectCache.find(object);
			if (it != niObjectCache.end()) {
				// Let people know that this object is invalidated.
				stateHandle.triggerEvent(new mwse::lua::event::ObjectInvalidatedEvent(it->second));

				// Clear any events that make use of this object.
				mwse::lua::event::clearObjectFilter(it->second);

				// Remove it from the cache.
				niObjectCache.erase(it);
			}
		}
	}

	void Object::clearCachedLuaObjects() {
		const auto stateHandle = mwse::lua::LuaManager::getInstance().getThreadSafeStateHandle();
		niObjectCache.clear();
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::Object)
