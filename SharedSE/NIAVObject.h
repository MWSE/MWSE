#pragma once

#include "NIObjectNET.h"

#include "NIBound.h"
#include "NIBoundingBox.h"
#include "NILinkedList.h"
#include "NIProperty.h"
#include "NITransform.h"

namespace NI {
	struct ObjectVelocities {
		Point3 localVelocity;
		Point3 worldVelocity;
	};

	struct AVObject_vTable : Object_vTable {
		void(__thiscall* updateControllers)(AVObject*, float); // 0x2C
		void(__thiscall* applyTransform)(AVObject*, Matrix33*, Point3*, bool); // 0x30
		Bound* (__thiscall* getWorldBound)(const AVObject*); // 0x34
		void(__thiscall* createWorldVertices)(AVObject*); // 0x38
		void(__thiscall* updateWorldVertices)(AVObject*); // 0x3C
		void(__thiscall* destroyWorldVertices)(AVObject*); // 0x40
		void(__thiscall* createWorldNormals)(AVObject*); // 0x44
		void(__thiscall* updateWorldNormals)(AVObject*); // 0x48
		void(__thiscall* destroyWorldNormals)(AVObject*); // 0x4C
		void(__thiscall* setAppCulled)(AVObject*, bool); // 0x50
		bool(__thiscall* getAppCulled)(const AVObject*); // 0x54
		void(__thiscall* setPropagationMode)(AVObject*, int); // 0x58
		AVObject* (__thiscall* getObjectByName)(const AVObject*, const char*); // 0x5C
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
		bool(__thiscall* findIntersections)(AVObject*, Point3*, Point3*, Pick*); // 0x88
		void(__thiscall* updateWorldData)(AVObject*); // 0x8C
		void(__thiscall* updateWorldBound)(AVObject*); // 0x90
	};
	static_assert(sizeof(AVObject_vTable) == 0x94, "NI::AVObject's vtable failed size validation");

	struct AVObject : ObjectNET {
		unsigned short flags; // 0x14
		short pad_16;
		Node* parentNode; // 0x18
		Point3 worldBoundOrigin; // 0x1C
		float worldBoundRadius; // 0x28
		Matrix33* localRotation; // 0x2C
		Point3 localTranslate; // 0x30
		float localScale; // 0x3C
		Transform worldTransform; // 0x40
		ObjectVelocities* velocities; // 0x74
		BoundingVolume* modelABV; // 0x78
		void* worldABV; // 0x7C
		int(__cdecl* collideCallback)(void*); // 0x80
		void* collideCallbackUserData; // 0x84
		PropertyLinkedList propertyNode; // 0x88

		//
		// vTable wrappers.
		//

		Bound* getWorldBound() const;

		Point3 getLocalVelocity() const;
		void setLocalVelocity(Point3*);

		AVObject* getObjectByName(const char*) const;
		Node* findRootCollisionNode() const;

		template <typename T>
		T* getObjectByNameAndType(const char* name) const {
			return static_cast<T*>(getObjectByName(name));
		}

		template <typename T>
		T* getObjectByNameAndType(const char* name, uintptr_t rtti, bool allowSubtypes = true) const {
			auto result = getObjectByNameAndType<T>(name);
			if (result == nullptr) {
				return nullptr;
			}

			if (allowSubtypes) {
				return result->isInstanceOfType(rtti) ? result : nullptr;
			}
			else {
				return result->isOfType(rtti) ? result : nullptr;
			}
		}

		bool getAppCulled() const;
		void setAppCulled(bool culled);

		bool isAppCulled() const;
		bool isFrustumCulled(const Camera*) const;

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

		bool intersectBounds(const Point3* position, const Point3* direction, float* out_result) const;

		void calculateBounds(Point3& min, Point3& max, const Point3& translation, const Matrix33& rotation, const float& scale, const bool accurateSkinned, const bool observeAppCullFlag, const bool onlyActiveChildren) const;

		void attachProperty(Property* property);
		Pointer<Property> detachPropertyByType(PropertyType type);
#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		sol::table detachAllProperties_lua(sol::this_state ts);
#endif

		//
		// Custom functions.
		//

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		std::shared_ptr<BoundingBox> createBoundingBox_lua(sol::optional<sol::table>) const;
#endif

		Transform getLocalTransform() const;
		float getLowestVertexZ() const;

		void clearTransforms();
		void copyTransforms(const AVObject* from);
		void copyTransforms(const Transform* from);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void copyTransforms_lua(const sol::stack_object from);
#endif

		AVObject* getParentByName(const char*) const;

		// ABV setter; uses the typed BoundingVolume from NIBound.h.
		void setModelSpaceABV(BoundingVolume* volume);

		void detachFromParent();

		Pointer<Property> getProperty(PropertyType type) const;
		Pointer<AlphaProperty> getAlphaProperty() const;
		void setAlphaProperty(AlphaProperty* prop);
		Pointer<FogProperty> getFogProperty() const;
		void setFogProperty(FogProperty* prop);
		Pointer<MaterialProperty> getMaterialProperty() const;
		void setMaterialProperty(MaterialProperty* prop);
		Pointer<StencilProperty> getStencilProperty() const;
		void setStencilProperty(StencilProperty* prop);
		Pointer<TexturingProperty> getTexturingProperty() const;
		void setTexturingProperty(TexturingProperty* prop);
		Pointer<VertexColorProperty> getVertexColorProperty() const;
		void setVertexColorProperty(VertexColorProperty* prop);
		Pointer<ZBufferProperty> getZBufferProperty() const;
		void setZBufferProperty(ZBufferProperty* prop);

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void setAlphaProperty_lua(sol::optional<AlphaProperty*> prop);
		void setFogProperty_lua(sol::optional<FogProperty*> prop);
		void setMaterialProperty_lua(sol::optional<MaterialProperty*> prop);
		void setStencilProperty_lua(sol::optional<StencilProperty*> prop);
		void setTexturingProperty_lua(sol::optional<TexturingProperty*> prop);
		void setVertexColorProperty_lua(sol::optional<VertexColorProperty*> prop);
		void setZBufferProperty_lua(sol::optional<ZBufferProperty*> prop);
#endif


#if defined(SE_USE_LUA) && SE_USE_LUA == 1
		void update_lua(sol::optional<sol::table> args);
#endif

#if defined(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE) && SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE > 0
		static constexpr auto _detachPropertyByType = reinterpret_cast<Pointer<Property>* (__thiscall*)(AVObject*, Pointer<Property>*, PropertyType)>(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE);
#endif
	};
	static_assert(sizeof(AVObject) == 0x90, "NI::AVObject failed size validation");

	void __cdecl CalculateBounds(const AVObject* object, Point3& out_min, Point3& out_max, const Point3& translation, const Matrix33& rotation, const float& scale);

	void __cdecl VerifyWorldVertices(const AVObject* object);
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AVObject)
#endif
