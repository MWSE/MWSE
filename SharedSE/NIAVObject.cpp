#include "NIAVObject.h"

#include "NIDefines.h"
#include "NICamera.h"
#include "NICollisionSwitch.h"
#include "NINode.h"
#include "NIProperty.h"
#include "NISwitchNode.h"
#include "NITriBasedGeometry.h"

#include "BitUtil.h"
#include "ExceptionUtil.h"
#include "MemoryUtil.h"

#if defined(SE_IS_MWSE) && SE_IS_MWSE == 1
#include "LuaUtil.h"
#endif

namespace NI {

	Bound* AVObject::getWorldBound() const {
		return vTable.asAVObject->getWorldBound(this);
	}

	Point3 AVObject::getLocalVelocity() const {
		if (velocities) {
			return velocities->localVelocity;
		}
		return Point3{ 0, 0, 0 };
	}

	void AVObject::setLocalVelocity(Point3* v) {
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

	Node* AVObject::findRootCollisionNode() const {
#if defined(SE_NI_AVOBJECT_FNADDR_FINDROOTCOLLISIONNODE) && SE_NI_AVOBJECT_FNADDR_FINDROOTCOLLISIONNODE > 0
		const auto NI_AVObject_findRootCollisionNode = reinterpret_cast<Node*(__cdecl*)(const AVObject*)>(SE_NI_AVOBJECT_FNADDR_FINDROOTCOLLISIONNODE);
		return NI_AVObject_findRootCollisionNode(this);
#else
		throw not_implemented_exception();
#endif
	}

	AVObject* AVObject::getParentByName(const char* name) const {
		Node* result = parentNode;
		while (result != nullptr) {
			if (result->name && _stricmp(name, result->name) == 0) {
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

	bool AVObject::isFrustumCulled(const Camera* camera) const {
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

	bool AVObject::getFlag(unsigned char index) const {
		return BIT_TEST(flags, index);
	}

	void AVObject::setFlag(bool state, unsigned char index) {
#if defined(SE_NI_AVOBJECT_FNADDR_SETFLAG) && SE_NI_AVOBJECT_FNADDR_SETFLAG > 0
		const auto NI_AVObject_setFlag = reinterpret_cast<void(__thiscall*)(AVObject*, bool, unsigned char)>(SE_NI_AVOBJECT_FNADDR_SETFLAG);
		NI_AVObject_setFlag(this, state, index);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::update(float fTime, bool bUpdateControllers, bool bUpdateChildren) {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATE) && SE_NI_AVOBJECT_FNADDR_UPDATE > 0
		reinterpret_cast<void(__thiscall*)(AVObject*, float, int, int)>(SE_NI_AVOBJECT_FNADDR_UPDATE)(this, fTime, bUpdateControllers, bUpdateChildren);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::updateEffects() {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS) && SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS > 0
		reinterpret_cast<void(__thiscall*)(AVObject*)>(SE_NI_AVOBJECT_FNADDR_UPDATEEFFECTS)(this);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::updateProperties() {
#if defined(SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES) && SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES > 0
		reinterpret_cast<void(__thiscall*)(AVObject*)>(SE_NI_AVOBJECT_FNADDR_UPDATEPROPERTIES)(this);
#else
		throw not_implemented_exception();
#endif
	}

	Matrix33* AVObject::getLocalRotationMatrix() const {
		return localRotation;
	}

	void AVObject::setLocalRotationMatrix(const Matrix33* matrix) {
#if defined(SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX) && SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX > 0
		reinterpret_cast<void(__thiscall*)(AVObject*, const Matrix33*)>(SE_NI_AVOBJECT_FNADDR_SETLOCALROTATIONMATRIX)(this, matrix);
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
#if defined(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE) && SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE > 0
		const auto NI_AVObject_detachPropertyByType = reinterpret_cast<Pointer<Property>* (__thiscall*)(AVObject*, Pointer<Property>*, PropertyType)>(SE_NI_AVOBJECT_FNADDR_DETACHPROPERTYBYTYPE);

		Pointer<Property> prop;
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
#endif

	bool AVObject::intersectBounds(const Point3* position, const Point3* direction, float* out_result) const {
#if defined(SE_NI_AVOBJECT_FNADDR_INTERSECTBOUNDS) && SE_NI_AVOBJECT_FNADDR_INTERSECTBOUNDS > 0
		const auto NI_AVObject_IntersectBounds = reinterpret_cast<bool(__thiscall*)(const AVObject*, const Point3*, const Point3*, float*)>(SE_NI_AVOBJECT_FNADDR_INTERSECTBOUNDS);
		return NI_AVObject_IntersectBounds(this, position, direction, out_result);
#else
		throw not_implemented_exception();
#endif
	}

	void AVObject::calculateBounds(
		Point3& outMin,
		Point3& outMax,
		const Point3& translation,
		const Matrix33& rotation,
		const float& scale,
		const bool accurateSkinned,
		const bool observeAppCullFlag,
		const bool onlyActiveChildren
	) const
	{
		// Ignore collision-disabled subgraphs.
		if (isOfType(RTTIStaticPtr::NiCollisionSwitch)) {
			const auto asCollisionSwitch = static_cast<const CollisionSwitch*>(this);
			if (!asCollisionSwitch->getCollisionActive()) {
				return;
			}
		}

		// Recurse until we get to a leaf node.
		if (isInstanceOfType(RTTIStaticPtr::NiNode)) {
			auto calculateChildBounds = [&](const AVObject* child) {
				if (!child) {
					return;
				}
				if (observeAppCullFlag && child->getAppCulled()) {
					return;
				}
				child->calculateBounds(
					outMin,
					outMax,
					rotation * child->localTranslate * scale + translation, // translation
					rotation * (*child->localRotation), // rotation
					scale * child->localScale, // scale
					accurateSkinned,
					observeAppCullFlag,
					onlyActiveChildren
				);
			};
			if (onlyActiveChildren && isInstanceOfType(RTTIStaticPtr::NiSwitchNode)) {
				const auto asNode = static_cast<const SwitchNode*>(this);
				const auto child = asNode->getActiveChild();
				calculateChildBounds(child);
			}
			else {
				const auto asNode = static_cast<const Node*>(this);
				for (const auto& child : asNode->children) {
					calculateChildBounds(child);
				}
			}
			return;
		}

		// Optionally ignore culled objects.
		if (observeAppCullFlag && getAppCulled()) {
			return;
		}

		// Only care about geometry leaf nodes.
		if (!isInstanceOfType(RTTIStaticPtr::NiGeometry)) {
			return;
		}

		// Ignore particles.
		if (isInstanceOfType(RTTIStaticPtr::NiParticles)) {
			return;
		}

		const auto asGeometry = static_cast<const Geometry*>(this);
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
			static std::vector<Point3> deformVertices;
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

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	std::shared_ptr<BoundingBox> AVObject::createBoundingBox_lua(sol::optional<sol::table> maybeParams) const {
		auto accuratedSkinned = mwse::lua::getOptionalParam(maybeParams, "accurateSkinned", false);
		auto observeAppCullFlag = mwse::lua::getOptionalParam(maybeParams, "observeAppCullFlag", false);
		auto onlyActiveChildren = mwse::lua::getOptionalParam(maybeParams, "onlyActiveChildren", false);
		auto bb = std::make_shared<BoundingBox>();
		bb->initialize();
		calculateBounds(bb->minimum, bb->maximum, NI::Point3::ZEROES, NI::Matrix33::IDENTITY, 1.0, accuratedSkinned, observeAppCullFlag, onlyActiveChildren);
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

		// Only process active child of switch nodes.
		if (isInstanceOfType(RTTIStaticPtr::NiSwitchNode)) {
			const auto asSwitchNode = static_cast<const SwitchNode*>(this);
			const auto activeChild = asSwitchNode->getActiveChild();
			if (activeChild) {
				return activeChild->getLowestVertexZ();
			}
			return FLOAT_MAX;
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
#if defined(SE_NI_MATRIX33_GLOBADDR_IDENTITY) && SE_NI_MATRIX33_GLOBADDR_IDENTITY > 0
		setLocalRotationMatrix(reinterpret_cast<Matrix33*>(SE_NI_MATRIX33_GLOBADDR_IDENTITY));
#else
		throw not_implemented_exception();
#endif
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

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void AVObject::copyTransforms_lua(const sol::stack_object source) {
		if (source.is<AVObject*>()) {
			copyTransforms(source.as<AVObject*>());
		}
		else if (source.is<NI::Transform*>()) {
			copyTransforms(source.as<NI::Transform*>());
		}
		else {
			throw std::invalid_argument("Invalid 'source' parameter provided");
		}
	}
#endif

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

	void AVObject::setAlphaProperty(AlphaProperty* prop) {
		detachPropertyByType(PropertyType::Alpha);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<FogProperty> AVObject::getFogProperty() const {
		return static_cast<FogProperty*>(getProperty(PropertyType::Fog).get());
	}

	void AVObject::setFogProperty(FogProperty* prop) {
		detachPropertyByType(PropertyType::Fog);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<MaterialProperty> AVObject::getMaterialProperty() const {
		return static_cast<MaterialProperty*>(getProperty(PropertyType::Material).get());
	}

	void AVObject::setMaterialProperty(MaterialProperty* prop) {
		detachPropertyByType(PropertyType::Material);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<StencilProperty> AVObject::getStencilProperty() const {
		return static_cast<StencilProperty*>(getProperty(PropertyType::Stencil).get());
	}

	void AVObject::setStencilProperty(StencilProperty* prop) {
		detachPropertyByType(PropertyType::Stencil);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<TexturingProperty> AVObject::getTexturingProperty() const {
		return static_cast<TexturingProperty*>(getProperty(PropertyType::Texturing).get());
	}

	void AVObject::setTexturingProperty(TexturingProperty* prop) {
		detachPropertyByType(PropertyType::Texturing);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<VertexColorProperty> AVObject::getVertexColorProperty() const {
		return static_cast<VertexColorProperty*>(getProperty(PropertyType::VertexColor).get());
	}

	void AVObject::setVertexColorProperty(VertexColorProperty* prop) {
		detachPropertyByType(PropertyType::VertexColor);
		if (prop) {
			attachProperty(prop);
		}
	}

	Pointer<ZBufferProperty> AVObject::getZBufferProperty() const {
		return static_cast<ZBufferProperty*>(getProperty(PropertyType::ZBuffer).get());
	}

	void AVObject::setZBufferProperty(ZBufferProperty* prop) {
		detachPropertyByType(PropertyType::ZBuffer);
		if (prop) {
			attachProperty(prop);
		}
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void AVObject::setAlphaProperty_lua(sol::optional<AlphaProperty*> prop) {
		setAlphaProperty(prop.value_or(nullptr));
	}

	void AVObject::setFogProperty_lua(sol::optional<FogProperty*> prop) {
		setFogProperty(prop.value_or(nullptr));
	}

	void AVObject::setMaterialProperty_lua(sol::optional<MaterialProperty*> prop) {
		setMaterialProperty(prop.value_or(nullptr));
	}

	void AVObject::setStencilProperty_lua(sol::optional<StencilProperty*> prop) {
		setStencilProperty(prop.value_or(nullptr));
	}

	void AVObject::setTexturingProperty_lua(sol::optional<TexturingProperty*> prop) {
		setTexturingProperty(prop.value_or(nullptr));
	}

	void AVObject::setVertexColorProperty_lua(sol::optional<VertexColorProperty*> prop) {
		setVertexColorProperty(prop.value_or(nullptr));
	}

	void AVObject::setZBufferProperty_lua(sol::optional<ZBufferProperty*> prop) {
		setZBufferProperty(prop.value_or(nullptr));
	}
#endif

	void AVObject::setModelSpaceABV(BoundingVolume* volume) {
#if defined(SE_NI_AVOBJECT_FNADDR_SETMODELSPACEABV) && SE_NI_AVOBJECT_FNADDR_SETMODELSPACEABV > 0
		const auto NI_AVObject_setModelSpaceABV = reinterpret_cast<bool(__thiscall*)(AVObject*, BoundingVolume*)>(SE_NI_AVOBJECT_FNADDR_SETMODELSPACEABV);
		NI_AVObject_setModelSpaceABV(this, volume);
#else
		throw not_implemented_exception();
#endif
	}

#if defined(SE_USE_LUA) && SE_USE_LUA == 1
	void AVObject::update_lua(sol::optional<sol::table> args) {
		if (args.has_value()) {
			auto& values = args.value();
			float time = values.get_or("time", 0.0f);
			bool updateControllers = values.get_or("controllers", false);
			bool updateChildren = values.get_or("children", true);

			update(time, updateControllers, updateChildren);
		}
		else {
			update();
		}
	}
#endif

	void __cdecl CalculateBounds(const AVObject* object, Point3& outMin, Point3& outMax, const Point3& translation, const Matrix33& rotation, const float& scale) {
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
			static std::vector<Point3> deformVertices;
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
