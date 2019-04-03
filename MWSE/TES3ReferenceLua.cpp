#include "TES3ReferenceLua.h"

#include "LuaManager.h"
#include "TES3ObjectLua.h"
#include "TES3ScriptLua.h"

#include "TES3Util.h"

#include "NINode.h"

#include "TES3Cell.h"
#include "TES3DataHandler.h"
#include "TES3ItemData.h"
#include "TES3MobilePlayer.h"
#include "TES3Reference.h"
#include "TES3WorldController.h"

namespace TES3 {
	sol::object Reference::getAttachments() {
		mwse::lua::LuaManager& luaManager = mwse::lua::LuaManager::getInstance();
		sol::state& state = luaManager.getState();

		sol::table result = luaManager.createTable();

		Attachment* attachment = this->attachments;
		while (attachment) {
			switch (attachment->type) {
			case AttachmentType::Lock:
				result["lock"] = sol::make_object(state, reinterpret_cast<LockAttachment*>(attachment)->data);
				break;
			case AttachmentType::TravelDestination:
				result["travelDestination"] = sol::make_object(state, reinterpret_cast<TravelDestinationAttachment*>(attachment)->data);
				break;
			case AttachmentType::Variables:
				result["variables"] = sol::make_object(state, reinterpret_cast<ItemDataAttachment*>(attachment)->data);
				break;
			case AttachmentType::ActorData:
				result["actor"] = mwse::lua::makeLuaObject(reinterpret_cast<MobileActorAttachment*>(attachment)->data);
				break;
			}

			attachment = attachment->next;
		}

		return result;
	}

	sol::table Reference::getLuaTable() {
		auto dataHandler = TES3::DataHandler::get();
		if (dataHandler != nullptr && dataHandler->mainThreadID != GetCurrentThreadId()) {
			throw std::exception("Cannot be called from outside the main thread.");
		}

		auto attachment = getOrCreateAttachedItemData();
		if (attachment == nullptr) {
			throw std::exception("Could not create ItemData attachment.");
		}

		return attachment->getOrCreateLuaDataTable();
	}
}

namespace mwse {
	namespace lua {
		sol::object getContext(TES3::Reference* reference) {
			auto variables = reference->getAttachedItemData();
			if (variables == NULL) {
				return sol::nil;
			}
			sol::state& state = mwse::lua::LuaManager::getInstance().getState();
			return sol::make_object(state, std::shared_ptr<ScriptContext>(new ScriptContext(variables->script, variables->scriptData)));
		}

		void bindTES3Reference() {
			// Get our lua state.
			sol::state& state = LuaManager::getInstance().getState();

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.create_simple_usertype<TES3::Reference>();
			usertypeDefinition.set("new", sol::no_constructor);

			// Define inheritance structures. These must be defined in order from top to bottom. The complete chain must be defined.
			usertypeDefinition.set(sol::base_classes, sol::bases<TES3::Object, TES3::BaseObject>());
			setUserdataForObject(usertypeDefinition);

			// Access to other objects that need to be packaged.
			usertypeDefinition.set("cell", sol::readonly_property([](TES3::Reference& self) -> sol::object {
				// Handle case for the player.
				if (TES3::WorldController::get()->getMobilePlayer()->reference == &self) {
					return makeLuaObject(TES3::DataHandler::get()->currentCell);
				}

				if (self.owningCollection.asReferenceList == nullptr) {
					return sol::nil;
				}

				return makeLuaObject(self.owningCollection.asReferenceList->cell);
			}));
			usertypeDefinition.set("object", sol::readonly_property([](TES3::Reference& self) { return makeLuaObject(self.baseObject); }));
			usertypeDefinition.set("sceneNode", sol::readonly_property([](TES3::Reference& self) { return makeLuaObject(self.sceneNode); }));

			// Basic function binding.
			usertypeDefinition.set("activate", [](TES3::Reference& self, TES3::Reference& target) { target.activate(&self); });
			usertypeDefinition.set("clone", &TES3::Reference::clone);
			usertypeDefinition.set("setActionFlag", &TES3::Reference::setActionFlag);
			usertypeDefinition.set("clearActionFlag", &TES3::Reference::clearActionFlag);
			usertypeDefinition.set("testActionFlag", &TES3::Reference::testActionFlag);
			usertypeDefinition.set("updateEquipment", &TES3::Reference::updateEquipment);

			// Functions exposed as properties.
			usertypeDefinition.set("activationReference", sol::property(&TES3::Reference::getActionReference, &TES3::Reference::setActionReference));
			usertypeDefinition.set("attachments", sol::readonly_property(&TES3::Reference::getAttachments));
			usertypeDefinition.set("context", sol::readonly_property([](TES3::Reference& self) { return getContext(&self); }));
			usertypeDefinition.set("data", sol::readonly_property(&TES3::Reference::getLuaTable));
			usertypeDefinition.set("isEmpty", sol::property(&TES3::Reference::getEmptyInventoryFlag, &TES3::Reference::setEmptyInventoryFlag));
			usertypeDefinition.set("isRespawn", sol::readonly_property(&TES3::Reference::isRespawn));
			usertypeDefinition.set("orientation", sol::property([](TES3::Reference& self) { return self.getOrientation(); }, &TES3::Reference::setOrientationFromLua));
			usertypeDefinition.set("position", sol::property([](TES3::Reference& self) { return self.getPosition(); }, &TES3::Reference::setPositionFromLua));

			// Functions for manually syncing the scene graph, for if orientation or position is manually modified.
			usertypeDefinition.set("updateSceneGraph", [](TES3::Reference& self) {
				TES3::Matrix33 tempOutArg;
				self.sceneNode->setLocalRotationMatrix(self.updateSceneMatrix(&tempOutArg));
				self.sceneNode->propagatePositionChange();
				self.setObjectModified(true);
			});

			// Quick access to attachment data.
			usertypeDefinition.set("lockNode", sol::property([](TES3::Reference& self) {
				return self.getAttachedLockNode();
			}));
			usertypeDefinition.set("destination", sol::property([](TES3::Reference& self) -> TES3::TravelDestination * {
				TES3::TravelDestinationAttachment * attachment = static_cast<TES3::TravelDestinationAttachment*>(self.getAttachment(TES3::AttachmentType::TravelDestination));
				if (attachment) {
					return attachment->data;
				}
				return nullptr;
			}));
			usertypeDefinition.set("mobile", sol::property([](TES3::Reference& self) {
				return makeLuaObject(self.getAttachedMobileActor());
			}));
			usertypeDefinition.set("stackSize", sol::property(
				[](TES3::Reference& self)
			{
				TES3::ItemData* itemData = self.getAttachedItemData();
				return itemData ? itemData->count : 1;
			},
				[](TES3::Reference& self, int count)
			{
				TES3::ItemData* itemData = self.getOrCreateAttachedItemData();
				itemData->count = count;
			}
			));
			
			// Allow references to behave like a linked list node.
			usertypeDefinition.set("previousNode", sol::readonly_property([](TES3::Reference& self) { return makeLuaObject(self.previousInCollection); }));
			usertypeDefinition.set("nextNode", sol::readonly_property([](TES3::Reference& self) { return makeLuaObject(self.nextInCollection); }));
			usertypeDefinition.set("nodeData", sol::readonly_property([](TES3::Reference& self) { return makeLuaObject(&self); }));

			// Finish up our usertype.
			state.set_usertype("tes3reference", usertypeDefinition);
		}
	}
}
