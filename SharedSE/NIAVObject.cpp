#include "NIAVObject.h"

#include "NICollisionSwitch.h"
#include "NITriBasedGeometry.h"
#include "NISwitchNode.h"

#include "ExceptionUtil.h"
#include "MemoryUtil.h"

namespace NI {
	Vector3 AVObject::getLocalVelocity() const {
		if (velocities) {
			return velocities->localVelocity;
		}
		return Vector3{ 0, 0, 0 };
	}

	void AVObject::setLocalVelocity(Vector3* v) {
		if (velocities) {
			velocities->localVelocity = *v;
		}
		else {
			velocities = se::memory::_new<ObjectVelocities>();
			velocities->localVelocity = *v;
			velocities->worldVelocity = { 0, 0, 0 };
		}
	}

	AVObject* AVObject::getObjectByName(const char* name) const {
		return vTable.asAVObject->getObjectByName(this, name);
	}

	AVObject* AVObject::getObjectByNameAndType(const char* name, uintptr_t rtti, bool allowSubtypes) const {
		auto result = getObjectByName(name);
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

	bool AVObject::getAppCulled() const {
		return vTable.asAVObject->getAppCulled(this);
	}

	void AVObject::setAppCulled(bool culled) {
		vTable.asAVObject->setAppCulled(this, culled);
	}


	void AVObject::createWorldVertices() {
		vTable.asAVObject->createWorldVertices(this);
	}

	void AVObject::updateWorldVertices() {
		vTable.asAVObject->updateWorldVertices(this);
	}

	void AVObject::createWorldNormals() {
		vTable.asAVObject->createWorldNormals(this);
	}

	void AVObject::updateWorldNormals() {
		vTable.asAVObject->updateWorldNormals(this);
	}

	void AVObject::updateWorldDeforms() {
		// Recurse until we get to a leaf node.
		if (isInstanceOfType(RTTIStaticPtr::NiNode)) {
			const auto asNode = static_cast<Node*>(this);
			for (const auto& child : asNode->children) {
				if (child) {
					child->updateWorldDeforms();
				}
			}
			return;
		}

		// Only care about geometry.
		if (!isInstanceOfType(RTTIStaticPtr::NiGeometry)) {
			return;
		}

		// Skip anything without a skin instance.
		auto geometry = static_cast<NI::Geometry*>(this);
		if (!geometry->skinInstance) {
			return;
		}

		// Actually update the skin instance.
		geometry->updateDeforms();
	}

	void AVObject::updateWorldBound() {
		vTable.asAVObject->updateWorldBound(this);
	}

	void AVObject::update(float fTime, bool bUpdateControllers, bool bUpdateChildren) {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATE) && SE_NI_AVOBJECT_FNADDR_UPDATE > 0
		const auto NI_PropertyList_update = reinterpret_cast<void(__thiscall*)(AVObject*, float, int, int)>(SE_NI_AVOBJECT_FNADDR_UPDATE);
		NI_PropertyList_update(this, fTime, bUpdateControllers, bUpdateChildren);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::updateEffects() {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS) && SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS > 0
		const auto NI_PropertyList_updateEffects = reinterpret_cast<void(__thiscall*)(AVObject*)>(SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS);
		NI_PropertyList_updateEffects(this);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::updateProperties() {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES) && SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES > 0
		const auto NI_AVObject_updateProperties = reinterpret_cast<void(__thiscall*)(AVObject*)>(SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES);
		NI_AVObject_updateProperties(this);
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33* AVObject::getLocalRotationMatrix() const {
		return localRotation;
	}

	void AVObject::setLocalRotationMatrix(const Matrix33* matrix) {
#if defined(SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX) && SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX > 0
		const auto NI_PropertyList_setLocalRotationMatrix = reinterpret_cast<void(__thiscall*)(AVObject*, const Matrix33*)>(SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX);
		NI_PropertyList_setLocalRotationMatrix(this, matrix);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::attachProperty(Property* property) {
#if defined(SE_NI_AVOBJECT_FNADDR_ATTACHPROPERTY) && SE_NI_AVOBJECT_FNADDR_ATTACHPROPERTY > 0
		const auto NI_PropertyList_addHead = reinterpret_cast<void(__thiscall*)(PropertyLinkedList*, Pointer<Property>)>(SE_NI_AVOBJECT_FNADDR_ATTACHPROPERTY);
		NI_PropertyList_addHead(&propertyNode, property);
#else
		throw not_implemented_exception();
#endif
	}

	Pointer<Property> AVObject::detachPropertyByType(PropertyType type) {
#if defined(SE_NI_AVObject_FNADDR_DETACHPROPERTYBYTYPE) && SE_NI_AVObject_FNADDR_DETACHPROPERTYBYTYPE > 0
		const auto NI_AVObject_detachPropertyByType = reinterpret_cast<Pointer<Property> *(__thiscall*)(AVObject*, Pointer<Property>*, PropertyType)>(SE_NI_AVObject_FNADDR_DETACHPROPERTYBYTYPE);

		NI::Pointer<NI::Property> prop;
		NI_AVObject_detachPropertyByType(this, &prop, type);
		return prop;
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
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

	const auto NI_CreateBoundingBoxForNode = reinterpret_cast<void(__cdecl*)(const AVObject*, TES3::Vector3*, TES3::Vector3*, const TES3::Vector3*, const Matrix33*, const float*)>(0x4EF410);
	std::shared_ptr<TES3::BoundingBox> AVObject::createBoundingBox_lua() const {
		constexpr auto min = std::numeric_limits<float>::lowest();
		constexpr auto max = std::numeric_limits<float>::max();
		auto bb = std::make_shared<TES3::BoundingBox>(max, max, max, min, min, min);
		float scale = localScale;
		NI_CreateBoundingBoxForNode(this, &bb->minimum, &bb->maximum, (const TES3::Vector3*)0x7DE6CC, (const Matrix33*)0x7DE664, &scale);
		return bb;
	}
#endif

	Transform AVObject::getLocalTransform() const {
		return { *localRotation, localTranslate, localScale };
	}

	float AVObject::getLowestVertexZ() const {
		constexpr auto FLOAT_MAX = std::numeric_limits<float>::max();

		// Ignore culled nodes.
		if (getAppCulled()) {
			return FLOAT_MAX;
		}

		// Ignore collision-disabled subgraphs.
		if (isOfType(RTTIStaticPtr::NiCollisionSwitch)) {
			const auto asCollisionSwitch = static_cast<const CollisionSwitch*>(this);
			if (asCollisionSwitch->getCollisionActive()) {
				return FLOAT_MAX;
			}
		}

		// Recurse until we get to a leaf node.
		if (isInstanceOfType(RTTIStaticPtr::NiNode)) {
			const auto asNode = static_cast<const Node*>(this);
			auto lowestChildZ = FLOAT_MAX;
			for (const auto& child : asNode->children) {
				if (child) {
					const auto childLowestZ = child->getLowestVertexZ();
					lowestChildZ = std::min(lowestChildZ, childLowestZ);
				}
			}
			return lowestChildZ;
		}

		// Only care about geometry leaf nodes.
		if (!isInstanceOfType(RTTIStaticPtr::NiTriBasedGeom)) {
			return FLOAT_MAX;
		}

		// Ignore particles.
		if (isInstanceOfType(RTTIStaticPtr::NiParticles)) {
			return FLOAT_MAX;
		}

		// Figure out the lowest vertex's Z.
		auto geometry = static_cast<const NI::TriBasedGeometry*>(this);
		auto lowestZ = FLOAT_MAX;
		for (auto i = 0u; i < geometry->modelData->vertexCount; ++i) {
			lowestZ = std::min(lowestZ, geometry->worldVertices[i].z);
		}

		return lowestZ;
	}

	void AVObject::clearTransforms() {
		localScale = 1.0f;
		localTranslate.x = 0.0f;
		localTranslate.y = 0.0f;
		localTranslate.z = 0.0f;
		setLocalRotationMatrix(Matrix33::IdentityMatrix);
	}

	void AVObject::copyTransforms(const AVObject* source) {
		setLocalRotationMatrix(source->getLocalRotationMatrix());
		localTranslate = source->localTranslate;
		localScale = source->localScale;
	}

	void AVObject::copyTransforms(const Transform* source) {
		setLocalRotationMatrix(&source->rotation);
		localTranslate = source->translation;
		localScale = source->scale;
	}

	void AVObject::detachFromParent() {
		if (!parentNode) {
			return;
		}

		parentNode->detachChild(this);
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

	void AVObject::setAlphaProperty(std::optional<AlphaProperty*> prop) {
		detachPropertyByType(PropertyType::Alpha);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<FogProperty> AVObject::getFogProperty() const {
		return static_cast<FogProperty*>(getProperty(PropertyType::Fog).get());
	}

	void AVObject::setFogProperty(std::optional<FogProperty*> prop) {
		detachPropertyByType(PropertyType::Fog);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<MaterialProperty> AVObject::getMaterialProperty() const {
		return static_cast<MaterialProperty*>(getProperty(PropertyType::Material).get());
	}

	void AVObject::setMaterialProperty(std::optional<MaterialProperty*> prop) {
		detachPropertyByType(PropertyType::Material);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<StencilProperty> AVObject::getStencilProperty() const {
		return static_cast<StencilProperty*>(getProperty(PropertyType::Stencil).get());
	}

	void AVObject::setStencilProperty(std::optional<StencilProperty*> prop) {
		detachPropertyByType(PropertyType::Stencil);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<TexturingProperty> AVObject::getTexturingProperty() const {
		return static_cast<TexturingProperty*>(getProperty(PropertyType::Texturing).get());
	}

	void AVObject::setTexturingProperty(std::optional<TexturingProperty*> prop) {
		detachPropertyByType(PropertyType::Texturing);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<VertexColorProperty> AVObject::getVertexColorProperty() const {
		return static_cast<VertexColorProperty*>(getProperty(PropertyType::VertexColor).get());
	}

	void AVObject::setVertexColorProperty(std::optional<VertexColorProperty*> prop) {
		detachPropertyByType(PropertyType::VertexColor);
		if (prop) {
			attachProperty(prop.value());
		}
	}

	Pointer<ZBufferProperty> AVObject::getZBufferProperty() const {
		return static_cast<ZBufferProperty*>(getProperty(PropertyType::ZBuffer).get());
	}

	void AVObject::setZBufferProperty(std::optional<ZBufferProperty*> prop) {
		detachPropertyByType(PropertyType::ZBuffer);
		if (prop) {
			attachProperty(prop.value());
		}
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
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
#endif

	void __cdecl CalculateBounds(const AVObject* object, Vector3& outMin, Vector3& outMax, const Vector3& translation, const Matrix33& rotation, const float& scale) {
		// Note: This function is a copy of the MWSE equivilent with hardcoded params.
		auto accurateSkinned = false;
		auto observeAppCullFlag = false;
		auto onlyActiveChildren = false;

		// Ignore collision-disabled subgraphs.
		if (object->isOfType(RTTIStaticPtr::NiCollisionSwitch)) {
			const auto asCollisionSwitch = static_cast<const CollisionSwitch*>(object);
			if (!asCollisionSwitch->getCollisionActive()) {
				return;
			}
		}

		// Recurse until we get to a leaf node.
		if (object->isInstanceOfType(RTTIStaticPtr::NiNode)) {
			auto calculateChildBounds = [&](const AVObject* child) {
				if (!child) {
					return;
				}
				if (observeAppCullFlag && child->getAppCulled()) {
					return;
				}
				CalculateBounds(
					child,
					outMin,
					outMax,
					rotation * child->localTranslate * scale + translation, // translation
					rotation * (*child->localRotation), // rotation
					scale * child->localScale // scale
				);
			};
			if (onlyActiveChildren && object->isInstanceOfType(RTTIStaticPtr::NiSwitchNode)) {
				const auto asNode = static_cast<const SwitchNode*>(object);
				const auto child = asNode->getActiveChild();
				calculateChildBounds(child);
			}
			else {
				const auto asNode = static_cast<const Node*>(object);
				for (const auto& child : asNode->children) {
					calculateChildBounds(child);
				}
			}
			return;
		}

		// Optionally ignore culled objects.
		if (observeAppCullFlag && object->getAppCulled()) {
			return;
		}

		// Only care about geometry leaf nodes.
		if (!object->isInstanceOfType(RTTIStaticPtr::NiGeometry)) {
			return;
		}

		// Ignore particles.
		if (object->isInstanceOfType(RTTIStaticPtr::NiParticles)) {
			return;
		}

		const auto asGeometry = static_cast<const Geometry*>(object);
		const auto modelData = asGeometry->modelData.get();
		if (!modelData) {
			return;
		}

		const auto vertexCount = modelData->getActiveVertexCount();
		if (vertexCount == 0) {
			return;
		}

		auto vertices = modelData->vertex;

		// Optionally apply skin deformations. Note: This is not thread-safe.
		auto useDeform = accurateSkinned && (asGeometry->skinInstance != nullptr);
		if (useDeform) {
			static std::vector<Vector3> deformVertices;
			deformVertices.reserve(vertexCount);
			vertices = deformVertices.data();
			asGeometry->skinInstance->deform(modelData->vertex, nullptr, vertexCount, vertices, nullptr);
		}

		// Actually look at the vertices.
		for (auto i = 0u; i < vertexCount; ++i) {
			auto v = rotation * vertices[i] * scale + translation;
			outMin.x = std::min(outMin.x, v.x);
			outMin.y = std::min(outMin.y, v.y);
			outMin.z = std::min(outMin.z, v.z);
			outMax.x = std::max(outMax.x, v.x);
			outMax.y = std::max(outMax.y, v.y);
			outMax.z = std::max(outMax.z, v.z);
		}
	}

	void __cdecl VerifyWorldVertices(const NI::AVObject* object) {
#if _DEBUG
		// Ignore collision-disabled subgraphs.
		if (object->isOfType(NI::RTTIStaticPtr::NiCollisionSwitch)) {
			const auto asCollisionSwitch = static_cast<const NI::CollisionSwitch*>(object);
			if (!asCollisionSwitch->getCollisionActive()) {
				return;
			}
		}

		// Recurse until we get to a leaf node.
		if (object->isInstanceOfType(NI::RTTIStaticPtr::NiNode)) {
			bool success = false;
			const auto asNode = static_cast<const NI::Node*>(object);
			for (const auto& child : asNode->children) {
				if (child) {
					VerifyWorldVertices(child);
				}
			}
			return;
		}

		// Only care about geometry leaf nodes.
		if (!object->isInstanceOfType(NI::RTTIStaticPtr::NiTriBasedGeom)) {
			return;
		}

		// Ignore particles.
		if (object->isInstanceOfType(NI::RTTIStaticPtr::NiParticles)) {
			return;
		}

		// Ignore skinned.
		auto geometry = static_cast<const NI::Geometry*>(object);
		if (geometry->skinInstance) {
			return;
		}

		// Verify the vertices.
		const auto transform = object->worldTransform;
		const auto vertices = static_cast<const NI::Geometry*>(object)->modelData->getVertices();
		for (auto i = 0u; i < geometry->modelData->vertexCount; ++i) {
			const auto calculated = transform * geometry->modelData->vertex[i];
			if (geometry->worldVertices[i] != calculated) {
				const auto rtti = geometry->getRunTimeTypeInformation();
				throw std::exception("Vertex does not calculate to the same value!");
			}
		}
#endif
	}
}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
MWSE_SOL_CUSTOMIZED_PUSHER_DEFINE_NI(NI::AVObject)
#endif
