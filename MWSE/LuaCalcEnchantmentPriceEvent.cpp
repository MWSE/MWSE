#include "LuaCalcEnchantmentPriceEvent.h"

#include "LuaManager.h"

#include "TES3Actor.h"
#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MagicEffect.h"
#include "TES3MobileActor.h"
#include "TES3Reference.h"
#include "TES3UIElement.h"
#include "TES3UIManager.h"
#include "TES3DataHandler.h"
#include "TES3GameSetting.h"

#include "StringUtil.h"

namespace mwse::lua::event {
	CalculateEnchantmentPriceEvent::CalculateEnchantmentPriceEvent(TES3::MobileActor* serviceActor, int basePrice, int price) :
		ObjectFilteredEvent("calcEnchantmentPrice", serviceActor->reference),
		m_ServiceActor(serviceActor),
		m_BasePrice(basePrice),
		m_Price(price)
	{

	}

	sol::table CalculateEnchantmentPriceEvent::createEventTable() {
		auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
		auto& state = stateHandle.state;
		auto eventData = state.create_table();

		eventData["mobile"] = m_ServiceActor;
		if (m_ServiceActor) {
			eventData["reference"] = m_ServiceActor->reference;
		}

		eventData["basePrice"] = m_BasePrice;
		eventData["price"] = m_Price;

		auto menuEnchantment = TES3::UI::findMenu("MenuEnchantment");
		if (menuEnchantment) {
			auto menuEnchantItem = menuEnchantment->findChild("MenuEnchantment_Item");
			if (menuEnchantItem) {
				eventData["item"] = menuEnchantItem->getPropertyPointer<TES3::Item>("MenuEnchantment_SoulGem");
				eventData["itemData"] = menuEnchantItem->getPropertyPointer<TES3::ItemData>("MenuEnchantment_Item");
			}

			auto menuEnchantSoulGem = menuEnchantment->findChild("MenuEnchantment_SoulGem");
			if (menuEnchantSoulGem) {
				eventData["soulGem"] = menuEnchantSoulGem->getPropertyPointer<TES3::Item>("MenuEnchantment_SoulGem");
				auto soulGemData = menuEnchantSoulGem->getPropertyPointer<TES3::ItemData>("MenuEnchantment_Item");
				if (soulGemData) {
					eventData["soulGemData"] = soulGemData;
					eventData["soul"] = soulGemData->getSoul();
				}
			}

			auto menuEnchantCastType = menuEnchantment->findChild("MenuEnchantment_casttype");
			if (menuEnchantCastType) {
				eventData["castType"] = menuEnchantCastType->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_Value")).integerValue;
			}

			auto effectsList = menuEnchantment->findChild("MenuEnchantment_scroll");
			effectsList = effectsList ? effectsList->getContentElement() : nullptr;
			if (effectsList) {
				auto effects = eventData.create_named("effects");
				for (const auto child : effectsList->vectorChildren) {
					auto effectData = state.create_table();
					auto effect = child->getPropertyPointer<TES3::MagicEffect>("MenuEnchantment_Effect");

					effectData["effect"] = effect;
					effectData["magnitudeLow"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_MagLow")).integerValue;
					effectData["magnitudeHigh"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_MagHigh")).integerValue;
					effectData["area"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_Area")).integerValue;
					effectData["duration"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_Duration")).integerValue;

					if (effect->getFlagTargetAttribute()) {
						effectData["attribute"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_Attribute")).integerValue;
					}

					if (effect->getFlagTargetSkill()) {
						effectData["skill"] = child->getProperty(TES3::UI::PropertyType::Integer, TES3::UI::registerProperty("MenuEnchantment_Skill")).integerValue;
					}

					auto text = child->getText();
					if (text) {
						auto ndd = TES3::DataHandler::get()->nonDynamicData;
						if (mwse::string::equal(text, ndd->GMSTs[TES3::GMST::sRangeTouch]->value.asString)) {
							effectData["range"] = TES3::EffectRange::Touch;
						}
						else if (mwse::string::equal(text, ndd->GMSTs[TES3::GMST::sRangeTarget]->value.asString)) {
							effectData["range"] = TES3::EffectRange::Target;
						}
						else {
							effectData["range"] = TES3::EffectRange::Self;
						}
					}

					effects.add(effectData);
				}
			}
		}

		return eventData;
	}
}
