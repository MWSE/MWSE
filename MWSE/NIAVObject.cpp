#include "NIAVObject.h"
#include "NIDefines.h"
#include "NINode.h"
#include "NIProperty.h"
#include "NICamera.h"

#include "BitUtil.h"
#include "MemoryUtil.h"
#include "StringUtil.h"

constexpr auto NI_AVObject_updateEffects = 0x6EB380;
constexpr auto NI_AVObject_updateProperties = 0x6EB0E0;
constexpr auto NI_AVObject_update = 0x6EB000;

namespace NI {
	Bound* AVObject::getWorldBound() {
		return vTable.asAVObject->getWorldBound(this);
	}

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

	AVObject* AVObject::getObjectByName(const char* name) {
		return vTable.asAVObject->getObjectByName(this, name);
	}

	AVObject* AVObject::getParentByName(const char* name) const {
		Node* result = parentNode;
		while (result != nullptr) {
			if (mwse::string::equal(name, result->name)) {
				return result;
			}
			result = result->parentNode;
		}
		return nullptr;
	}

	bool AVObject::getAppCulled() const {
		return vTable.asAVObject->getAppCulled(this);
	}

	void AVObject::setAppCulled(bool culled) {
		vTable.asAVObject->setAppCulled(this, culled);
	}

	bool AVObject::isAppCulled() const {
		if (getAppCulled()) {
			return true;
		}
		return parentNode ? parentNode->isAppCulled() : false;
	}

	bool AVObject::isFrustumCulled(Camera* camera) const {
		for (auto i = 0u; i < 6; i++) {
			auto plane = camera->cullingPlanes[i];
			auto distance = (
				plane.x * worldBoundOrigin.x +
				plane.y * worldBoundOrigin.y +
				plane.z * worldBoundOrigin.z - plane.w
			);
			if (distance < -worldBoundRadius) {
				return true;
			}
		}
		return false;
	}

	bool AVObject::getFlag(unsigned char index) const {
		return BIT_TEST(flags, index);
	}

	const auto NI_AVObject_setFlag = reinterpret_cast<void(__thiscall*)(AVObject*, bool, unsigned char)>(0x405960);
	void AVObject::setFlag(bool state, unsigned char index) {
		NI_AVObject_setFlag(this, state, index);
	}

	void AVObject::update(float fTime, bool bUpdateControllers, bool bUpdateChildren) {
		reinterpret_cast<void(__thiscall*)(AVObject*, float, int, int)>(NI_AVObject_update)(this, fTime, bUpdateControllers, bUpdateChildren);
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

	void AVObject::setLocalRotationMatrix(const TES3::Matrix33 * matrix) {
		reinterpret_cast<void(__thiscall *)(AVObject*, const TES3::Matrix33*)>(0x50E020)(this, matrix);
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

	const auto NI_AVObject_IntersectBounds = reinterpret_cast<bool(__thiscall*)(const AVObject*, const TES3::Vector3*, const TES3::Vector3*, float*)>(0x6EB930);
	bool AVObject::intersectBounds(const TES3::Vector3* position, const TES3::Vector3* direction, float* out_result) const {
		return NI_AVObject_IntersectBounds(this, position, direction, out_result);
	}

	const auto NI_CreateBoundingBoxForNode = reinterpret_cast<void(__cdecl*)(const AVObject*, TES3::Vector3*, TES3::Vector3*, const TES3::Vector3*, const TES3::Matrix33*, const float*)>(0x4EF410);
	std::shared_ptr<TES3::BoundingBox> AVObject::createBoundingBox_lua() const {
		constexpr auto min = std::numeric_limits<float>::min();
		constexpr auto max = std::numeric_limits<float>::max();
		auto bb = std::make_shared<TES3::BoundingBox>(max, max, max, min, min, min);
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

	void AVObject::copyTransforms(const AVObject* source) {
		setLocalRotationMatrix(source->getLocalRotationMatrix());
		localTranslate = source->localTranslate;
		localScale = source->localScale;
	}

	void AVObject::copyTransforms(const TES3::Transform* source) {
		setLocalRotationMatrix(&source->rotation);
		localTranslate = source->translation;
		localScale = source->scale;
	}

	void AVObject::copyTransforms_lua(const sol::stack_object source) {
		if (source.is<AVObject*>()) {
			copyTransforms(source.as<AVObject*>());
		}
		else if (source.is<TES3::Transform*>()) {
			copyTransforms(source.as<TES3::Transform*>());
		}
		else {
			throw std::invalid_argument("Invalid 'source' parameter provided");
		}
	}

	TES3::Transform AVObject::getTransforms() const {
		TES3::Transform t;
		t.rotation = *localRotation;
		t.translation = localTranslate;
		t.scale = localScale;
		return t;
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
			bool updateChildren = values.get_or("children", true);

			update(time, updateControllers, updateChildren);
		}
		else {
			update();
		}
	}
}

MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AVObject)
