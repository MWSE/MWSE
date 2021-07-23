#pragma once

#include "NIObjectNET.h"
#include "NILinkedList.h"
#include "NIProperty.h"
#include "NITransform.h"

namespace NI {
	struct ObjectVelocities {
		TES3::Vector3 localVelocity;
		TES3::Vector3 worldVelocity;
	};

	struct AVObject : ObjectNET {
		unsigned short flags; // 0x14
		short pad_16;
		Node * parentNode; // 0x18
		TES3::Vector3 worldBoundOrigin; // 0x1C
		float worldBoundRadius; // 0x28
		TES3::Matrix33 * localRotation; // 0x2C
		TES3::Vector3 localTranslate; // 0x30
		float localScale; // 0x3C
		TES3::Transform worldTransform; // 0x40
		ObjectVelocities* velocities; // 0x74
		void * modelABV; // 0x78
		void * worldABV; // 0x7C
		int (__cdecl * collideCallback)(void*); // 0x80
		void * collideCallbackUserData; // 0x84
		PropertyLinkedList propertyNode; // 0x88

		//
		// vTable wrappers.
		//

		TES3::Vector3 getLocalVelocity() const;
		void setLocalVelocity(TES3::Vector3*);

		AVObject * getObjectByName(const char*);

		template <typename T>
		T * getObjectByNameAndType(const char* name) {
			return static_cast<T*>(vTable.asAVObject->getObjectByName(this, name));
		}

		bool getAppCulled();
		void setAppCulled(bool culled);
		
		//
		// Other related this-call functions.
		//

		void update(float fTime = 0.0f, bool bUpdateControllers = false, bool bUpdateBounds = true);
		void updateEffects();
		void updateProperties();
		TES3::Matrix33* getLocalRotationMatrix() const;
		void setLocalRotationMatrix(TES3::Matrix33* matrix);

		void attachProperty(Property* property);
		Pointer<Property> detachPropertyByType(PropertyType type);
		sol::table detachAllProperties_lua(sol::this_state ts);

		//
		// Custom functions.
		//

		std::shared_ptr<TES3::BoundingBox> createBoundingBox_lua() const;

		void clearTransforms();

		Pointer<Property> getProperty(PropertyType type) const;
		Pointer<AlphaProperty> getAlphaProperty() const;
		void setAlphaProperty(sol::optional<AlphaProperty*> prop);
		Pointer<FogProperty> getFogProperty() const;
		void setFogProperty(sol::optional<FogProperty*> prop);
		Pointer<MaterialProperty> getMaterialProperty() const;
		void setMaterialProperty(sol::optional<MaterialProperty*> prop);
		Pointer<StencilProperty> getStencilProperty() const;
		void setStencilProperty(sol::optional<StencilProperty*> prop);
		Pointer<TexturingProperty> getTexturingProperty() const;
		void setTexturingProperty(sol::optional<TexturingProperty*> prop);
		Pointer<VertexColorProperty> getVertexColorProperty() const;
		void setVertexColorProperty(sol::optional<VertexColorProperty*> prop);
		Pointer<ZBufferProperty> getZBufferProperty() const;
		void setZBufferProperty(sol::optional<ZBufferProperty*> prop);

		void update_lua(sol::optional<sol::table> args);

		//
		// Access to this type's raw functions.
		//

		static constexpr auto _detachPropertyByType = reinterpret_cast<Pointer<Property> * (__thiscall*)(AVObject*, Pointer<Property>*, PropertyType)>(0x6EAE20);

	};
	static_assert(sizeof(AVObject) == 0x90, "NI::AVObject failed size validation");

	struct AVObject_vTable : Object_vTable {
		void * unknown_0x2C;
		void * unknown_0x30;
		void * getWorldBound; // 0x34
		void * unknown_0x38;
		void * unknown_0x3C;
		void * unknown_0x40;
		void * unknown_0x44;
		void * unknown_0x48;
		void * unknown_0x4C;
		void (__thiscall * setAppCulled)(AVObject*, bool); // 0x50
		bool (__thiscall * getAppCulled)(AVObject*); // 0x54
		void * unknown_0x58;
		AVObject * (__thiscall * getObjectByName)(AVObject*, const char*); // 0x5C
		void * unknown_0x60;
		void * unknown_0x64;
		void * unknown_0x68;
		void * unknown_0x6C;
		void * unknown_0x70;
		void * unknown_0x74;
		void * unknown_0x78;
		void * unknown_0x7C;
		void * unknown_0x80;
		void * unknown_0x84;
		void * findIntersections; // 0x88
		void * updateWorldData; // 0x8C
		void * updateWorldBound; // 0x90
	};
	static_assert(sizeof(AVObject_vTable) == 0x94, "NI::AVObject's vtable failed size validation");
}

MWSE_SOL_CUSTOMIZED_PUSHER_DECLARE_NI(NI::AVObject)
