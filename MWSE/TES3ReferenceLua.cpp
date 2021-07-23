#include "TES3ReferenceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"

#include "TES3Util.h"

#include "NINode.h"
#include "NIPointLight.h"

#include "TES3BodyPartManager.h"
#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3ItemData.h"
#include "TES3Misc.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace mwse {
	namespace lua {
		void bindTES3Reference() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<TES3::Reference>("tes3reference");
			usertypeDefinition["new"] = sol::no_constructor;

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition[sol::base_classes] = sol::bases<TES3::Object, TES3::BaseObject>();
			setUserdataForTES3Object(usertypeDefinition);

			// Access to other objects that need to be packaged.
			usertypeDefinition["baseObject"] = sol::readonly_property(&TES3::Reference::getBaseObject);
			usertypeDefinition["cell"] = sol::readonly_property(&TES3::Reference::getCell);
			usertypeDefinition["object"] = sol::readonly_property(&TES3::Reference::baseObject);
			usertypeDefinition["sceneNode"] = sol::readonly_property(&TES3::Reference::sceneNode);
			usertypeDefinition["leveledBaseReference"] = sol::readonly_property(&TES3::Reference::getLeveledBaseReference);

			// Basic function binding.
			usertypeDefinition["activate"] = &TES3::Reference::activate_lua;
			usertypeDefinition["clearActionFlag"] = &TES3::Reference::clearActionFlag;
			usertypeDefinition["clone"] = &TES3::Reference::clone;
			usertypeDefinition["deleteDynamicLightAttachment"] = &TES3::Reference::deleteDynamicLightAttachment;
			usertypeDefinition["detachDynamicLightFromAffectedNodes"] = &TES3::Reference::detachDynamicLightFromAffectedNodes;
			usertypeDefinition["disable"] = &TES3::Reference::disable;
			usertypeDefinition["enable"] = &TES3::Reference::enable;
			usertypeDefinition["getAngleTo"] = &TES3::Reference::getAngleToReference;
			usertypeDefinition["getAttachedDynamicLight"] = &TES3::Reference::getAttachedDynamicLight;
			usertypeDefinition["getOrCreateAttachedDynamicLight"] = &TES3::Reference::getOrCreateAttachedDynamicLight_lua;
			usertypeDefinition["onCloseInventory"] = &TES3::Reference::onCloseInventory;
			usertypeDefinition["setActionFlag"] = &TES3::Reference::setActionFlag;
			usertypeDefinition["testActionFlag"] = &TES3::Reference::testActionFlag;
			usertypeDefinition["setDynamicLighting"] = &TES3::Reference::setDynamicLighting;
			usertypeDefinition["setNoCollisionFlag"] = &TES3::Reference::setNoCollision_lua;
			usertypeDefinition["updateEquipment"] = &TES3::Reference::updateBipedParts;
			usertypeDefinition["updateLighting"] = &TES3::Reference::updateLighting;

			// Functions exposed as properties.
			usertypeDefinition["activationReference"] = sol::property(&TES3::Reference::getActionReference, &TES3::Reference::setActionReference);
			usertypeDefinition["attachments"] = sol::readonly_property(&TES3::Reference::getAttachments_lua);
			usertypeDefinition["context"] = sol::readonly_property(&TES3::Reference::getContext_lua);
			usertypeDefinition["data"] = sol::readonly_property(&TES3::Reference::getLuaTable);
			usertypeDefinition["facing"] = sol::property(&TES3::Reference::getFacing, &TES3::Reference::setFacing);
			usertypeDefinition["hasNoCollision"] = sol::property(&TES3::Reference::getNoCollision, &TES3::Reference::setNoCollision_lua);
			usertypeDefinition["isDead"] = sol::readonly_property(&TES3::Reference::isDead);
			usertypeDefinition["isEmpty"] = sol::property(&TES3::Reference::getEmptyInventoryFlag, &TES3::Reference::setEmptyInventoryFlag);
			usertypeDefinition["isLeveledSpawn"] = sol::readonly_property(&TES3::Reference::isLeveledSpawn);
			usertypeDefinition["isRespawn"] = sol::readonly_property(&TES3::Reference::isRespawn);
			usertypeDefinition["mesh"] = sol::property(&TES3::Reference::getModelPath, &TES3::Reference::setModelPath);
			usertypeDefinition["orientation"] = sol::property(&TES3::Reference::getOrientation, &TES3::Reference::setOrientationFromLua);
			usertypeDefinition["position"] = sol::property(&TES3::Reference::getPosition, &TES3::Reference::setPositionFromLua);
			usertypeDefinition["sourceFormId"] = sol::readonly_property(&TES3::Reference::getSourceFormId);
			usertypeDefinition["sourceModId"] = sol::readonly_property(&TES3::Reference::getSourceModId);
			usertypeDefinition["supportsLuaData"] = sol::readonly_property(&TES3::Reference::getSupportsLuaData);
			usertypeDefinition["targetFormId"] = sol::readonly_property(&TES3::Reference::getTargetFormId);
			usertypeDefinition["targetModId"] = sol::readonly_property(&TES3::Reference::getTargetModId);
			usertypeDefinition["tempData"] = sol::readonly_property(&TES3::Reference::getLuaTempTable);

			// Functions for manually syncing the scene graph, for if orientation or position is manually modified.
			usertypeDefinition["updateSceneGraph"] = &TES3::Reference::updateSceneGraph_lua;

			// Quick access to attachment data.
			usertypeDefinition["bodyPartManager"] = sol::property(&TES3::Reference::getAttachedBodyPartManager);
			usertypeDefinition["itemData"] = sol::property(&TES3::Reference::getAttachedItemData, &TES3::Reference::setAttachedItemData);
			usertypeDefinition["lockNode"] = sol::readonly_property(&TES3::Reference::getAttachedLockNode);
			usertypeDefinition["destination"] = sol::readonly_property(&TES3::Reference::getAttachedTravelDestination);
			usertypeDefinition["light"] = sol::readonly_property(&TES3::Reference::getAttachedNiLight);
			usertypeDefinition["mobile"] = sol::readonly_property(&TES3::Reference::getAttachedMobileActor);
			usertypeDefinition["stackSize"] = sol::property(&TES3::Reference::getStackSize, &TES3::Reference::setStackSize);
			
			// Allow references to behave like a linked list node.
			usertypeDefinition["previousNode"] = sol::readonly_property(&TES3::Reference::previousInCollection);
			usertypeDefinition["nextNode"] = sol::readonly_property(&TES3::Reference::nextInCollection);
			usertypeDefinition["nodeData"] = sol::readonly_property(&TES3::Reference::getThis);
		}
	}
}
