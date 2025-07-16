#include "NIParticlesLua.h"
#include "NIObjectLua.h"

#include "LuaManager.h"

#include "NIPerParticleData.h"
#include "NIParticleSystemController.h"
#include "NIParticles.h"
#include "NIParticleModifier.h"

namespace mwse::lua {
	void bindNIParticles() {
		// Get our lua state.
		const auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.getState();

		// Binding for NI::Particles.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Particles>("niParticles");
			usertypeDefinition["new"] = &NI::Particles::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TriBasedGeometry, NI::Geometry, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["data"] = sol::property(&NI::Particles::getModelData, &NI::Particles::setModelData);
		}

		// Binding for NI::AutoNormalParticles.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::AutoNormalParticles>("niAutoNormalParticles");
			usertypeDefinition["new"] = &NI::AutoNormalParticles::create;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Particles, NI::TriBasedGeometry, NI::Geometry, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["data"] = sol::property(&NI::AutoNormalParticles::getModelData, &NI::AutoNormalParticles::setModelData);
		}

		// Binding for NI::RotatingParticles.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::RotatingParticles>("niRotatingParticles");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Particles, NI::TriBasedGeometry, NI::Geometry, NI::AVObject, NI::ObjectNET, NI::Object>();
			setUserdataForNIAVObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["data"] = sol::property(&NI::RotatingParticles::getModelData, &NI::RotatingParticles::setModelData);
		}

		// Binding for NI::ParticlesData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticlesData>("niParticlesData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TriBasedGeometryData, NI::GeometryData, NI::Object>();

			// Basic property binding.
			usertypeDefinition["activeCount"] = &NI::ParticlesData::activeCount;
			usertypeDefinition["radius"] = &NI::ParticlesData::radius;
			usertypeDefinition["sizes"] = sol::readonly_property(&NI::ParticlesData::getSizes);
		}

		// Binding for NI::AutoNormalParticlesData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::AutoNormalParticlesData>("niAutoNormalsParticlesData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticlesData, NI::TriBasedGeometryData, NI::GeometryData, NI::Object>();
		}

		// Binding for NI::RotatingParticlesData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::RotatingParticlesData>("niRotatingParticlesData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticlesData, NI::TriBasedGeometryData, NI::GeometryData, NI::Object>();

			// Basic property binding.
			usertypeDefinition["rotations"] = sol::readonly_property(&NI::RotatingParticlesData::getRotations);
		}

		// Binding for NI::ParticleSystemController.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleSystemController>("niParticleSystemController");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::TimeController, NI::Object>();

			// Basic property binding.
			usertypeDefinition["scaledLastTime"] = &NI::ParticleSystemController::scaledLastTime;
			usertypeDefinition["speed"] = &NI::ParticleSystemController::speed;
			usertypeDefinition["speedVariation"] = &NI::ParticleSystemController::speedVariation;
			usertypeDefinition["declinationAngle"] = &NI::ParticleSystemController::declinationAngle;
			usertypeDefinition["declinationAngleVariation"] = &NI::ParticleSystemController::declinationAngleVariation;
			usertypeDefinition["planarAngle"] = &NI::ParticleSystemController::planarAngle;
			usertypeDefinition["planarAngleVariation"] = &NI::ParticleSystemController::planarAngleVariation;
			usertypeDefinition["initialNormal"] = &NI::ParticleSystemController::initialNormal;
			usertypeDefinition["initialColor"] = &NI::ParticleSystemController::initialColor;
			usertypeDefinition["initialSize"] = &NI::ParticleSystemController::initialSize;
			usertypeDefinition["emitStartTime"] = &NI::ParticleSystemController::emitStartTime;
			usertypeDefinition["emitStopTime"] = &NI::ParticleSystemController::emitStopTime;
			usertypeDefinition["resetParticleSystem"] = &NI::ParticleSystemController::resetParticleSystem;
			usertypeDefinition["birthRate"] = &NI::ParticleSystemController::birthRate;
			usertypeDefinition["lifespan"] = &NI::ParticleSystemController::lifespan;
			usertypeDefinition["lifespanVariance"] = &NI::ParticleSystemController::lifespanVariance;
			usertypeDefinition["useBirthRate"] = &NI::ParticleSystemController::useBirthRate;
			usertypeDefinition["spawnOnDeath"] = &NI::ParticleSystemController::spawnOnDeath;
			usertypeDefinition["emitterWidth"] = &NI::ParticleSystemController::emitterWidth;
			usertypeDefinition["emitterHeight"] = &NI::ParticleSystemController::emitterHeight;
			usertypeDefinition["emitterDepth"] = &NI::ParticleSystemController::emitterDepth;
			usertypeDefinition["emitter"] = &NI::ParticleSystemController::emitter;
			usertypeDefinition["spawnGenerationsCount"] = &NI::ParticleSystemController::spawnGenerationsCount;
			usertypeDefinition["spawnPercentage"] = &NI::ParticleSystemController::spawnPercentage;
			usertypeDefinition["spawnMultiplier"] = &NI::ParticleSystemController::spawnMultiplier;
			usertypeDefinition["spawnSpeedChaos"] = &NI::ParticleSystemController::spawnSpeedChaos;
			usertypeDefinition["spawnDirectionChaos"] = &NI::ParticleSystemController::spawnDirectionChaos;
			usertypeDefinition["particleData"] = sol::readonly_property(&NI::ParticleSystemController::getPerParticleData);
			usertypeDefinition["particleDataCount"] = sol::readonly_property(&NI::ParticleSystemController::particleDataCount);
			usertypeDefinition["activeParticleCount"] = sol::readonly_property(&NI::ParticleSystemController::activeParticleCount);
			usertypeDefinition["currentParticleIndex"] = sol::readonly_property(&NI::ParticleSystemController::currentParticleIndex);
			usertypeDefinition["emitterModifiers"] = &NI::ParticleSystemController::emitterModifiers;
			usertypeDefinition["particleModifiers"] = &NI::ParticleSystemController::particleModifiers;
			usertypeDefinition["particleColliders"] = &NI::ParticleSystemController::particleColliders;
			usertypeDefinition["staticBounds"] = &NI::ParticleSystemController::staticBounds;
			usertypeDefinition["firstTime"] = &NI::ParticleSystemController::firstTime;
			usertypeDefinition["lastEmit"] = &NI::ParticleSystemController::lastEmit;
		}

		// Binding for NI::PerParticleData.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::PerParticleData>("niPerParticleData");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["age"] = &NI::PerParticleData::age;
			usertypeDefinition["generation"] = &NI::PerParticleData::generation;
			usertypeDefinition["index"] = sol::readonly_property(&NI::PerParticleData::index);
			usertypeDefinition["lastUpdate"] = &NI::PerParticleData::lastUpdate;
			usertypeDefinition["lifeSpan"] = &NI::PerParticleData::lifeSpan;
			usertypeDefinition["rotationAxis"] = &NI::PerParticleData::rotationAxis;
			usertypeDefinition["velocity"] = &NI::PerParticleData::velocity;
		}

		// Binding for NI::ParticleModifier.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleModifier>("niParticleModifier");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::Object>();

			// Basic property binding.
			usertypeDefinition["controller"] = &NI::ParticleModifier::controller;
			usertypeDefinition["next"] = &NI::ParticleModifier::nextModifier;
		}

		// Binding for NI::Gravity.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::Gravity>("niGravity");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["decay"] = &NI::Gravity::decay;
			usertypeDefinition["direction"] = &NI::Gravity::direction;
			usertypeDefinition["force"] = &NI::Gravity::force;
			usertypeDefinition["forceType"] = &NI::Gravity::forceType;
			usertypeDefinition["position"] = &NI::Gravity::position;
		}

		// Binding for NI::ParticleBomb.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleBomb>("niParticleBomb");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["decay"] = &NI::ParticleBomb::decay;
			usertypeDefinition["decayType"] = &NI::ParticleBomb::decayType;
			usertypeDefinition["deltaV"] = &NI::ParticleBomb::deltaV;
			usertypeDefinition["direction"] = &NI::ParticleBomb::direction;
			usertypeDefinition["duration"] = &NI::ParticleBomb::duration;
			usertypeDefinition["position"] = &NI::ParticleBomb::position;
			usertypeDefinition["start"] = &NI::ParticleBomb::start;
			usertypeDefinition["symmetryType"] = &NI::ParticleBomb::symmetryType;
		}

		// Binding for NI::ParticleColorModifier.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleColorModifier>("niParticleColorModifier");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["colorData"] = &NI::ParticleColorModifier::colorData;
		}

		// Binding for NI::ParticleGrowFade.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleGrowFade>("niParticleGrowFade");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["fade"] = &NI::ParticleGrowFade::fade;
			usertypeDefinition["grow"] = &NI::ParticleGrowFade::grow;
		}

		// Binding for NI::ParticleRotation.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleRotation>("niParticleRotation");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["initialAxis"] = &NI::ParticleRotation::initialAxis;
			usertypeDefinition["randomInitialAxis"] = &NI::ParticleRotation::randomInitialAxis;
			usertypeDefinition["rotationSpeed"] = &NI::ParticleRotation::rotationSpeed;
		}

		// Binding for NI::ParticleCollider.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::ParticleCollider>("niParticleCollider");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["collisionPoint"] = &NI::ParticleCollider::collisionPoint;
			usertypeDefinition["collisionTime"] = &NI::ParticleCollider::collisionTime;
			usertypeDefinition["dieOnCollide"] = &NI::ParticleCollider::dieOnCollide;
			usertypeDefinition["restitution"] = &NI::ParticleCollider::restitution;
			usertypeDefinition["spawnOnCollide"] = &NI::ParticleCollider::spawnOnCollide;
		}

		// Binding for NI::PlanarCollider.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::PlanarCollider>("niPlanarCollider");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleCollider, NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["height"] = &NI::PlanarCollider::height;
			usertypeDefinition["width"] = &NI::PlanarCollider::width;
			usertypeDefinition["planeEquation"] = &NI::PlanarCollider::planeEquation;
			usertypeDefinition["position"] = &NI::PlanarCollider::position;
			usertypeDefinition["xAxis"] = &NI::PlanarCollider::xAxis;
			usertypeDefinition["yAxis"] = &NI::PlanarCollider::yAxis;
		}

		// Binding for NI::SphericalCollider.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<NI::SphericalCollider>("niSphericalCollider");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<NI::ParticleCollider, NI::ParticleModifier, NI::Object>();

			// Basic property binding.
			usertypeDefinition["position"] = &NI::SphericalCollider::position;
			usertypeDefinition["radius"] = sol::property(&NI::SphericalCollider::getRadius, &NI::SphericalCollider::setRadius);
		}

	}
}
