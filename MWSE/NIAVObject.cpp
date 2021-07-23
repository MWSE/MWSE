#include "NIAVObject.h"

#include "NIProperty.h"

#include "MemoryUtil.h"

#define NI_AVObject_updateEffects 0x6EB380
#define NI_AVObject_updateProperties 0x6EB0E0
#define NI_AVObject_update 0x6EB000

namespace NI {
	TES3::Vector3 AVObject::getLocalVelocity() const {
		if (velocities) {
			return velocities->localVelocity;
		}
		return TES3::Vector3{ 0, 0, 0 };
	}

	void AVObject::setLocalVelocity(TES3::Vector3* v) {
		if (velocities) {
			velocities->localVelocity = *v;
		}
		else {
			velocities = mwse::tes3::_new<ObjectVelocities>();
			velocities->localVelocity = *v;
			velocities->worldVelocity = { 0, 0, 0 };
		}
	}

	AVObject * AVObject::getObjectByName(const char* name) {
		return vTable.asAVObject->getObjectByName(this, name);
	}

	bool AVObject::getAppCulled() {
		return vTable.asAVObject->getAppCulled(this);
	}

	void AVObject::setAppCulled(bool culled) {
		vTable.asAVObject->setAppCulled(this, culled);
	}

	void AVObject::update(float fTime, bool bUpdateControllers, bool bUpdateBounds) {
		reinterpret_cast<void(__thiscall*)(AVObject*, float, int, int)>(NI_AVObject_update)(this, fTime, bUpdateControllers, bUpdateBounds);
	}

	void AVObject::updateEffects() {
		reinterpret_cast<void(__thiscall *)(AVObject *)>(NI_AVObject_updateEffects)(this);
	}

	void AVObject::updateProperties() {
		reinterpret_cast<void(__thiscall *)(AVObject *)>(NI_AVObject_updateProperties)(this);
	}

	TES3::Matrix33* AVObject::getLocalRotationMatrix() const {
		return localRotation;
	}

	void AVObject::setLocalRotationMatrix(TES3::Matrix33 * matrix) {
		reinterpret_cast<void(__thiscall *)(AVObject*, TES3::Matrix33*)>(0x50E020)(this, matrix);
	}

	const auto NI_PropertyList_addHead = reinterpret_cast<void(__thiscall*)(PropertyLinkedList*, Pointer<Property>)>(0x405840);
	void AVObject::attachProperty(Property* property) {
		NI_PropertyList_addHead(&propertyNode, property);
	}

	Pointer<Property> AVObject::detachPropertyByType(PropertyType type) {
		NI::Pointer<NI::Property> prop;
		_detachPropertyByType(this, &prop, type);
		return prop;
	}

	sol::table AVObject::detachAllProperties_lua(sol::this_state ts) {
		sol::state_view state = ts;
		auto removedProperties = state.create_table();

		while (propertyNode.data) {
			auto removed = detachPropertyByType(propertyNode.data->getType());
			if (removed) {
				removedProperties.add(removed);
			}
		}

		return removedProperties;
	}

	const auto NI_CreateBoundingBoxForNode = reinterpret_cast<void(__cdecl*)(const AVObject*, TES3::Vector3*, TES3::Vector3*, const TES3::Vector3*, const TES3::Matrix33*, const float*)>(0x4EF410);
	std::shared_ptr<TES3::BoundingBox> AVObject::createBoundingBox_lua() const {
		auto bb = std::make_shared<TES3::BoundingBox>(FLT_MAX, FLT_MAX, FLT_MAX, FLT_MIN, FLT_MIN, FLT_MIN);
		float scale = localScale;
		NI_CreateBoundingBoxForNode(this, &bb->minimum, &bb->maximum, (const TES3::Vector3*)0x7DE6CC, (const TES3::Matrix33*)0x7DE664, &scale);
		return bb;
	}

	void AVObject::clearTransforms() {
		localScale = 1.0f;
		localTranslate.x = 0.0f;
		localTranslate.y = 0.0f;
		localTranslate.z = 0.0f;
		setLocalRotationMatrix(reinterpret_cast<TES3::Matrix33*>(0x7DE664));
	}

	Pointer<Property> AVObject::getProperty(PropertyType type) const {
		auto propNode = &propertyNode;
		while (propNode && propNode->data) {
			if (propNode->data->getType() == type) {
				return propNode->data;
			}
			propNode = propNode->next;
		}
		return nullptr;
	}

	Pointer<AlphaProperty> AVObject::getAlphaProperty() const {
		return static_cast<AlphaProperty*>(getProperty(PropertyType::Alpha).get());
	}

	void AVObject::setAlphaProperty(sol::optional<AlphaProperty*> prop) {
		detachPropertyByType(PropertyType::Alpha);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<FogProperty> AVObject::getFogProperty() const {
		return static_cast<FogProperty*>(getProperty(PropertyType::Fog).get());
	}

	void AVObject::setFogProperty(sol::optional<FogProperty*> prop) {
		detachPropertyByType(PropertyType::Fog);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<MaterialProperty> AVObject::getMaterialProperty() const {
		return static_cast<MaterialProperty*>(getProperty(PropertyType::Material).get());
	}

	void AVObject::setMaterialProperty(sol::optional<MaterialProperty*> prop) {
		detachPropertyByType(PropertyType::Material);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<StencilProperty> AVObject::getStencilProperty() const {
		return static_cast<StencilProperty*>(getProperty(PropertyType::Stencil).get());
	}

	void AVObject::setStencilProperty(sol::optional<StencilProperty*> prop) {
		detachPropertyByType(PropertyType::Stencil);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<TexturingProperty> AVObject::getTexturingProperty() const {
		return static_cast<TexturingProperty*>(getProperty(PropertyType::Texturing).get());
	}

	void AVObject::setTexturingProperty(sol::optional<TexturingProperty*> prop) {
		detachPropertyByType(PropertyType::Texturing);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<VertexColorProperty> AVObject::getVertexColorProperty() const {
		return static_cast<VertexColorProperty*>(getProperty(PropertyType::VertexColor).get());
	}

	void AVObject::setVertexColorProperty(sol::optional<VertexColorProperty*> prop) {
		detachPropertyByType(PropertyType::VertexColor);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<ZBufferProperty> AVObject::getZBufferProperty() const {
		return static_cast<ZBufferProperty*>(getProperty(PropertyType::ZBuffer).get());
	}

	void AVObject::setZBufferProperty(sol::optional<ZBufferProperty*> prop) {
		detachPropertyByType(PropertyType::ZBuffer);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	void AVObject::update_lua(sol::optional<sol::table> args) {
		if (args) {
			auto values = args.value();
			float time = values.get_or("time", 0.0f);
			bool updateControllers = values.get_or("controllers", false);
			bool updateBounds = values.get_or("bounds", true);

			update(time, updateControllers, updateBounds);
		}
		else {
			update();
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AVObject)
