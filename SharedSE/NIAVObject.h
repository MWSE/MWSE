#pragma once

#include "NIObjectNET.h"

#include "NIBound.h"
#include "NIBoundingBox.h"
#include "NILinkedList.h"
#include "NIProperty.h"
#include "NITransform.h"

namespace NI {
	struct ObjectVelocities {
		Vector3 localVelocity;
		Vector3 worldVelocity;
	};

	struct AVObject_vTable : Object_vTable {
		void(__thiscall* updateControllers)(AVObject*, float); // 0x2C
		void(__thiscall* applyTransform)(AVObject*, Matrix33*, Vector3*, bool); // 0x30
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original types this as SphereBound* (the real engine return type).
		SphereBound* (__thiscall* getWorldBound)(AVObject*); // 0x34
#else
		Bound* (__thiscall* getWorldBound)(AVObject*); // 0x34
#endif
		void(__thiscall* createWorldVertices)(AVObject*); // 0x38
		void(__thiscall* updateWorldVertices)(AVObject*); // 0x3C
		void(__thiscall* destroyWorldVertices)(AVObject*); // 0x40
		void(__thiscall* createWorldNormals)(AVObject*); // 0x44
		void(__thiscall* updateWorldNormals)(AVObject*); // 0x48
		void(__thiscall* destroyWorldNormals)(AVObject*); // 0x4C
		void(__thiscall* setAppCulled)(AVObject*, bool); // 0x50
		bool(__thiscall* getAppCulled)(const AVObject*); // 0x54
		void(__thiscall* setPropagationMode)(AVObject*, int); // 0x58
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original: non-const this. Matches MWSE-private NIAVObject.cpp.
		AVObject* (__thiscall* getObjectByName)(AVObject*, const char*); // 0x5C
#else
		// SharedSE/CSSE: const this. Matches SharedSE/NIAVObject.cpp.
		AVObject* (__thiscall* getObjectByName)(const AVObject*, const char*); // 0x5C
#endif
		void(__thiscall* updateDownwardPass)(AVObject*, float, bool, bool); // 0x60
		bool(__thiscall* isVisualObject)(AVObject*); // 0x64
		void(__thiscall* updatePropertiesDownward)(AVObject*, void*); // 0x68
		void(__thiscall* updateEffectsDownward)(AVObject*, void*); // 0x6C
		void* (__thiscall* getPropertyState)(AVObject*, void**); // 0x70
		void* (__thiscall* getEffectsState)(AVObject*, void**); // 0x74
		void(__thiscall* display)(AVObject*, Camera*); // 0x78
		void(__thiscall* updateCollisionData)(AVObject*); // 0x7C
		bool(__thiscall* testCollisions)(AVObject*, float, void*, void*); // 0x80
		int(__thiscall* findCollisions)(AVObject*, float, void*, void*); // 0x84
		bool(__thiscall* findIntersections)(AVObject*, Vector3*, Vector3*, Pick*); // 0x88
		void(__thiscall* updateWorldData)(AVObject*); // 0x8C
		void(__thiscall* updateWorldBound)(AVObject*); // 0x90
	};
	static_assert(sizeof(AVObject_vTable) == 0x94, "NI::AVObject's vtable failed size validation");

	struct AVObject : ObjectNET {
		unsigned short flags; // 0x14
		short pad_16;
		Node* parentNode; // 0x18
		Vector3 worldBoundOrigin; // 0x1C
		float worldBoundRadius; // 0x28
		Matrix33* localRotation; // 0x2C
		Vector3 localTranslate; // 0x30
		float localScale; // 0x3C
		Transform worldTransform; // 0x40
		ObjectVelocities* velocities; // 0x74
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original types this as BoundingVolume* (engine truth, defined in
		// NIBound.h's MWSE branch). Same memory (4-byte pointer) on both targets.
		BoundingVolume* modelABV; // 0x78
#else
		void* modelABV; // 0x78
#endif
		void* worldABV; // 0x7C
		int(__cdecl* collideCallback)(void*); // 0x80
		void* collideCallbackUserData; // 0x84
		PropertyLinkedList propertyNode; // 0x88

		//
		// vTable wrappers.
		//

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original: returns SphereBound* (the engine's actual return type).
		SphereBound* getWorldBound();
#endif

		Vector3 getLocalVelocity() const;
		void setLocalVelocity(Vector3*);

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE-original: non-const this (matches MWSE-private NIAVObject.cpp impl).
		AVObject* getObjectByName(const char*);

		// MWSE-original template form: type-safe variant alongside the
		// RTTI-based non-template version below.
		template <typename T>
		T* getObjectByNameAndType(const char* name) {
			return static_cast<T*>(vTable.asAVObject->getObjectByName(this, name));
		}
#else
		// SharedSE/CSSE: const, RTTI-based getObjectByNameAndType.
		AVObject* getObjectByName(const char*) const;
		AVObject* getObjectByNameAndType(const char* name, uintptr_t rtti, bool allowSubtypes = true) const;
#endif

		bool getAppCulled() const;
		void setAppCulled(bool culled);

		// Conveniences combining flag + culled state. Implementations live in
		// MWSE-private NIAVObject.cpp; CSSE doesn't currently call them.
		bool isAppCulled() const;
		bool isFrustumCulled(Camera*) const;

		void createWorldVertices();
		void updateWorldVertices();
		void createWorldNormals();
		void updateWorldNormals();
		void updateWorldDeforms();
		void updateWorldBound();

		//
		// Other related this-call functions.
		//

		void update(float fTime = 0.0f, bool bUpdateControllers = false, bool bUpdateChildren = true);
		void updateEffects();
		void updateProperties();
		Matrix33* getLocalRotationMatrix() const;
		void setLocalRotationMatrix(const Matrix33* matrix);

		// Flag accessors over the bitfield at offset 0x14.
		bool getFlag(unsigned char index) const;
		void setFlag(bool state, unsigned char index);

		// Ray/sphere intersection test against world-bound. Impl in MWSE-private
		// NIAVObject.cpp.
		bool intersectBounds(const Vector3* position, const Vector3* direction, float* out_result) const;

		// Member-form bounds calculation. Coexists with the free CalculateBounds()
		// declared at the bottom of this file.
		void calculateBounds(
			Vector3& min,
			Vector3& max,
			const Vector3& translation,
			const Matrix33& rotation,
			const float& scale,
			const bool accurateSkinned,
			const bool observeAppCullFlag,
			const bool onlyActiveChildren
		) const;

		void attachProperty(Property* property);
		Pointer<Property> detachPropertyByType(PropertyType type);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		sol::table detachAllProperties_lua(sol::this_state ts);
#endif

		//
		// Custom functions.
		//

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE form takes an optional config table.
		std::shared_ptr<TES3::BoundingBox> createBoundingBox_lua(sol::optional<sol::table>) const;
#else
		std::shared_ptr<TES3::BoundingBox> createBoundingBox_lua() const;
#endif
#endif

		Transform getLocalTransform() const;
		float getLowestVertexZ() const;

		void clearTransforms();
		void copyTransforms(const AVObject* from);
		void copyTransforms(const Transform* from);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void copyTransforms_lua(const sol::stack_object from);
#endif

		// Alias for getLocalTransform() (kept for API compatibility).
		Transform getTransforms() const;

		// Parent-walk helper (counterpart to SharedSE's child-side detach).
		AVObject* getParentByName(const char*) const;

		// ABV setter; uses the typed BoundingVolume from NIBound.h.
		void setModelSpaceABV(BoundingVolume* volume);

		void detachFromParent();

		Pointer<Property> getProperty(PropertyType type) const;
		Pointer<AlphaProperty> getAlphaProperty() const;
		Pointer<FogProperty> getFogProperty() const;
		Pointer<MaterialProperty> getMaterialProperty() const;
		Pointer<StencilProperty> getStencilProperty() const;
		Pointer<TexturingProperty> getTexturingProperty() const;
		Pointer<VertexColorProperty> getVertexColorProperty() const;
		Pointer<ZBufferProperty> getZBufferProperty() const;

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
		// MWSE setters take sol::optional (matches MWSE-private NIAVObject.cpp).
		void setAlphaProperty(sol::optional<AlphaProperty*> prop);
		void setFogProperty(sol::optional<FogProperty*> prop);
		void setMaterialProperty(sol::optional<MaterialProperty*> prop);
		void setStencilProperty(sol::optional<StencilProperty*> prop);
		void setTexturingProperty(sol::optional<TexturingProperty*> prop);
		void setVertexColorProperty(sol::optional<VertexColorProperty*> prop);
		void setZBufferProperty(sol::optional<ZBufferProperty*> prop);
#else
		// SharedSE/CSSE setters take std::optional (matches SharedSE/NIAVObject.cpp).
		void setAlphaProperty(std::optional<AlphaProperty*> prop);
		void setFogProperty(std::optional<FogProperty*> prop);
		void setMaterialProperty(std::optional<MaterialProperty*> prop);
		void setStencilProperty(std::optional<StencilProperty*> prop);
		void setTexturingProperty(std::optional<TexturingProperty*> prop);
		void setVertexColorProperty(std::optional<VertexColorProperty*> prop);
		void setZBufferProperty(std::optional<ZBufferProperty*> prop);
#endif

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void update_lua(sol::optional<sol::table> args);
#endif

		// Engine raw function. Per-target address resolved via NIConfig.{Morrowind,TESConstructionSet}.h.
#if defined(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE) && SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE > 0
		static constexpr auto _detachPropertyByType = reinterpret_cast<Pointer<Property>* (__thiscall*)(AVObject*, Pointer<Property>*, PropertyType)>(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE);
#endif
	};
	static_assert(sizeof(AVObject) == 0x90, "NI::AVObject failed size validation");

	void __cdecl CalculateBounds(const AVObject* object, Vector3& out_min, Vector3& out_max, const Vector3& translation, const Matrix33& rotation, const float& scale);

	void __cdecl VerifyWorldVertices(const AVObject* object);
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AVObject)
#endif
