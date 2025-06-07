#include "TES3BodyPartLua.h"

#include "LuaManager.h"
#include "LuaUtil.h"

#include "TES3ObjectLua.h"

#include "TES3BodyPart.h"
#include "TES3BodyPartManager.h"
#include "TES3WearablePart.h"

namespace mwse::lua {
	void bindTES3BodyPart() {
		// Get our lua state.
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;

		// Binding for TES3::BodyPart.
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BodyPart>("tes3bodyPart");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::PhysicalObject, TES3::Object, TES3::BaseObject>();
			setUserdataForTES3PhysicalObject(usertypeDefinition);

			// Basic property binding.
			usertypeDefinition["raceName"] = sol::readonly_property(&TES3::BodyPart::raceName);
			usertypeDefinition["part"] = &TES3::BodyPart::part;
			usertypeDefinition["partType"] = &TES3::BodyPart::partType;
			usertypeDefinition["vampiric"] = &TES3::BodyPart::vampiric;

			// Properties that need extra work before returning.
			usertypeDefinition["sceneNode"] = &TES3::BodyPart::mirroredNode;

			// Functions exposed as properties.
			usertypeDefinition["mesh"] = sol::property(&TES3::BodyPart::getModelPath, &TES3::BodyPart::setModelPath);

			// Easier access to flags.
			usertypeDefinition["female"] = sol::property(&TES3::BodyPart::getIsFemale, &TES3::BodyPart::setIsFemale);
			usertypeDefinition["playable"] = sol::property(&TES3::BodyPart::getIsPlayable, &TES3::BodyPart::setIsPlayable);
		}

		// Binding for TES3::WearablePart
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::WearablePart>("tes3wearablePart");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["type"] = &TES3::WearablePart::bodypartID;
			usertypeDefinition["male"] = &TES3::WearablePart::male;
			usertypeDefinition["female"] = &TES3::WearablePart::female;
			usertypeDefinition["valid"] = sol::readonly_property(&TES3::WearablePart::isValid);

			// Basic function binding.
			usertypeDefinition["getPart"] = &TES3::WearablePart::getPart;
		}

		// Binding for TES3::BodyPartManager::AttachNode
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BodyPartManager::AttachNode>("tes3bodyPartManagerAttachNode");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["flags"] = &TES3::BodyPartManager::AttachNode::flags;
			usertypeDefinition["node"] = &TES3::BodyPartManager::AttachNode::node;
			usertypeDefinition["rotation"] = &TES3::BodyPartManager::AttachNode::rotation;
			usertypeDefinition["translation"] = &TES3::BodyPartManager::AttachNode::translation;
		}

		// Binding for TES3::BodyPartManager::ActiveBodyPart
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BodyPartManager::ActiveBodyPart>("tes3bodyPartManagerActiveBodyPart");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["bodyPart"] = sol::property(&TES3::BodyPartManager::ActiveBodyPart::getBodyPart, &TES3::BodyPartManager::ActiveBodyPart::setBodyPart);
			usertypeDefinition["flags"] = &TES3::BodyPartManager::ActiveBodyPart::flags;
			usertypeDefinition["item"] = &TES3::BodyPartManager::ActiveBodyPart::item;
			usertypeDefinition["node"] = &TES3::BodyPartManager::ActiveBodyPart::node;
		}

		// Binding for TES3::BodyPartManager
		{
			// Start our usertype.
			auto usertypeDefinition = state.new_usertype<TES3::BodyPartManager>("tes3bodyPartManager");
			usertypeDefinition["new"] = sol::no_constructor;

			// Basic property binding.
			usertypeDefinition["animationPhase"] = sol::readonly_property(&TES3::BodyPartManager::animationPhase);
			usertypeDefinition["attachNodes"] = sol::readonly_property(&TES3::BodyPartManager::getAttachNodes);
			usertypeDefinition["reference"] = sol::readonly_property(&TES3::BodyPartManager::reference);

			// Basic function bindings.
			usertypeDefinition["getActiveBodyPart"] = &TES3::BodyPartManager::getActiveBodyPart;
			usertypeDefinition["getActiveBodyPartForItem"] = &TES3::BodyPartManager::getActiveBodyPartForItem;
			usertypeDefinition["removeActiveBodyPart"] = &TES3::BodyPartManager::removeActiveBodyPart;
			usertypeDefinition["removeEquippedLayers"] = &TES3::BodyPartManager::removeEquippedLayers;
			usertypeDefinition["setBodyPartByIdForObject"] = &TES3::BodyPartManager::setBodyPartByIdForObject;
			usertypeDefinition["setBodyPartForObject"] = &TES3::BodyPartManager::setBodyPartForObject;
			usertypeDefinition["updateForReference"] = &TES3::BodyPartManager::updateForReference;
			usertypeDefinition["getAttachNode"] = &TES3::BodyPartManager::getAttachNode;
		}
	}
}
