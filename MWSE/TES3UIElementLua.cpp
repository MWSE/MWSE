#include "TES3UIElementLua.h"

#include "LuaUtil.h"
#include "NIProperty.h"

#include "TES3GameFile.h"
#include "TES3Item.h"
#include "TES3ItemData.h"
#include "TES3MobileObject.h"
#include "TES3UIElement.h"
#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIManagerLua.h"
#include "TES3UIWidgets.h"
#include "TES3UIWidgetsLua.h"

#include "LuaManager.h"
#include "Log.h"

namespace mwse {
	namespace lua {
		using TES3::UI::Element;
		using TES3::UI::UI_ID;

		const UI_ID idNull = static_cast<UI_ID>(TES3::UI::Property::null);

		void bindTES3UIElement() {
			// Get our lua state.
			auto stateHandle = LuaManager::getInstance().getThreadSafeStateHandle();
			sol::state& state = stateHandle.state;

			// Start our usertype. We must finish this with state.set_usertype.
			auto usertypeDefinition = state.new_usertype<Element>("tes3uiElement");
			usertypeDefinition["new"] = sol::no_constructor;

			// Allow elements to be converted to strings using their object ID.
			usertypeDefinition[sol::meta_function::to_string] = &Element::getName;

			// Allow elements to be serialized to json using their ID/name.
			usertypeDefinition["__tojson"] = &Element::toJson;

			// Read-only property bindings.
			usertypeDefinition["children"] = sol::readonly_property(&Element::getChildren_lua);
			usertypeDefinition["contentType"] = sol::readonly_property(&Element::getContentTypeString);
			usertypeDefinition["id"] = sol::readonly_property(&Element::id);
			usertypeDefinition["name"] = sol::readonly_property(&Element::getName);
			usertypeDefinition["parent"] = sol::readonly_property(&Element::parent);
			usertypeDefinition["properties"] = sol::readonly_property(&Element::getProperties_lua);
			usertypeDefinition["sceneNode"] = sol::readonly_property(&Element::sceneNode);
			usertypeDefinition["widget"] = sol::readonly_property(&Element::makeWidget);

			// Read-write property bindings.
			// Many properties also set lazy-update flags through setProperty.
			usertypeDefinition["absolutePosAlignX"] = sol::property(&Element::getAbsolutePosAlignX_lua, &Element::setAbsolutePosAlignX_lua);
			usertypeDefinition["absolutePosAlignY"] = sol::property(&Element::getAbsolutePosAlignY_lua, &Element::setAbsolutePosAlignY_lua);
			usertypeDefinition["alpha"] = sol::property(&Element::getAlpha, &Element::setAlpha);
			usertypeDefinition["autoHeight"] = sol::property(&Element::getAutoHeight, &Element::setAutoHeight);
			usertypeDefinition["autoWidth"] = sol::property(&Element::getAutoWidth, &Element::setAutoWidth);
			usertypeDefinition["borderAllSides"] = sol::property(&Element::getBorderAllSides, &Element::setBorderAllSides_lua);
			usertypeDefinition["borderBottom"] = sol::property(&Element::getBorderBottom_lua, &Element::setBorderBottom_lua);
			usertypeDefinition["borderLeft"] = sol::property(&Element::getBorderLeft_lua, &Element::setBorderLeft_lua);
			usertypeDefinition["borderRight"] = sol::property(&Element::getBorderRight_lua, &Element::setBorderRight_lua);
			usertypeDefinition["borderTop"] = sol::property(&Element::getBorderTop_lua, &Element::setBorderTop_lua);
			usertypeDefinition["childAlignX"] = sol::property(&Element::getChildAlignX, &Element::setChildAlignX);
			usertypeDefinition["childAlignY"] = sol::property(&Element::getChildAlignY, &Element::setChildAlignY);
			usertypeDefinition["childOffsetX"] = sol::property(&Element::getChildOffsetX_lua, &Element::setChildOffsetX_lua);
			usertypeDefinition["childOffsetY"] = sol::property(&Element::getChildOffsetY_lua, &Element::setChildOffsetY_lua);
			usertypeDefinition["color"] = sol::property(&Element::getColor_lua, &Element::setColor_lua);
			usertypeDefinition["consumeMouseEvents"] = sol::property(&Element::getConsumeMouseEvents, &Element::setConsumeMouseEvents_lua);
			usertypeDefinition["contentPath"] = sol::property(&Element::getContentPath, &Element::setContentPath_lua);
			usertypeDefinition["disabled"] = sol::property(&Element::getDisabled, &Element::setDisabled);
			usertypeDefinition["flowDirection"] = sol::property(&Element::getFlowDirectionString, &Element::setFlowDirectionString);
			usertypeDefinition["font"] = sol::property(&Element::getFont, &Element::setFont_lua);
			usertypeDefinition["height"] = sol::property(&Element::getHeight, &Element::setHeight);
			usertypeDefinition["heightProportional"] = sol::property(&Element::getHeightProportional_lua, &Element::setHeightProportional_lua);
			usertypeDefinition["imageFilter"] = sol::property(&Element::getImageFilter, &Element::setImageFilter);
			usertypeDefinition["imageScaleX"] = sol::property(&Element::getImageScaleX, &Element::setImageScaleX);
			usertypeDefinition["imageScaleY"] = sol::property(&Element::getImageScaleY, &Element::setImageScaleY);
			usertypeDefinition["justifyText"] = sol::property(&Element::getJustifyTextString, &Element::setJustifyTextString);
			usertypeDefinition["maxHeight"] = sol::property(&Element::getMaxHeight_lua, &Element::setMaxHeight_lua);
			usertypeDefinition["maxWidth"] = sol::property(&Element::getMaxWidth_lua, &Element::setMaxWidth_lua);
			usertypeDefinition["minHeight"] = sol::property(&Element::getMinHeight_lua, &Element::setMinHeight_lua);
			usertypeDefinition["minWidth"] = sol::property(&Element::getMinWidth_lua, &Element::setMinWidth_lua);
			usertypeDefinition["nodeMaxX"] = &Element::nodeMaxX;
			usertypeDefinition["nodeMaxY"] = &Element::nodeMaxY;
			usertypeDefinition["nodeMinX"] = &Element::nodeMinX;
			usertypeDefinition["nodeMinY"] = &Element::nodeMinY;
			usertypeDefinition["nodeOffsetX"] = sol::property(&Element::getNodeOffsetX, &Element::setNodeOffsetX);
			usertypeDefinition["nodeOffsetY"] = sol::property(&Element::getNodeOffsetY, &Element::setNodeOffsetY);
			usertypeDefinition["paddingAllSides"] = sol::property(&Element::getPaddingAllSides, &Element::setPaddingAllSides_lua);
			usertypeDefinition["paddingBottom"] = sol::property(&Element::getPaddingBottom_lua, &Element::setPaddingBottom_lua);
			usertypeDefinition["paddingLeft"] = sol::property(&Element::getPaddingLeft_lua, &Element::setPaddingLeft_lua);
			usertypeDefinition["paddingRight"] = sol::property(&Element::getPaddingRight_lua, &Element::setPaddingRight_lua);
			usertypeDefinition["paddingTop"] = sol::property(&Element::getPaddingTop_lua, &Element::setPaddingTop_lua);
			usertypeDefinition["positionX"] = sol::property(&Element::getPositionX, &Element::setPositionX);
			usertypeDefinition["positionY"] = sol::property(&Element::getPositionY, &Element::setPositionY);
			usertypeDefinition["rawText"] = sol::property(&Element::getText, &Element::setText);
			usertypeDefinition["repeatKeys"] = sol::property(&Element::getRepeatKeys, &Element::setRepeatKeys);
			usertypeDefinition["scaleMode"] = sol::property(&Element::getScaleMode, &Element::setScaleMode);
			usertypeDefinition["text"] = sol::property(&Element::getWidgetText, &Element::setWidgetText_lua);
			usertypeDefinition["texture"] = sol::property(&Element::getTexture, &Element::setTexture);
			usertypeDefinition["visible"] = sol::property(&Element::getVisible, &Element::setVisible_lua);
			usertypeDefinition["width"] = sol::property(&Element::getWidth, &Element::setWidth);
			usertypeDefinition["widthProportional"] = sol::property(&Element::getWidthProportional_lua, &Element::setWidthProportional_lua);
			usertypeDefinition["wrapText"] = sol::property(&Element::getWrapText, &Element::setWrapText);

			// Deprecated properties.
			usertypeDefinition["acceptMouseEvents"] = sol::property(&Element::getConsumeMouseEvents, &Element::setConsumeMouseEvents_lua);
			usertypeDefinition["alignX"] = sol::property(&Element::getChildAlignX, &Element::setChildAlignX);
			usertypeDefinition["alignY"] = sol::property(&Element::getChildAlignY, &Element::setChildAlignY);
			usertypeDefinition["layoutHeightFraction"] = sol::property(&Element::getHeightProportional_lua, &Element::setHeightProportional_lua);
			usertypeDefinition["layoutOriginFractionX"] = sol::property(&Element::getAbsolutePosAlignX_lua, &Element::setAbsolutePosAlignX_lua);
			usertypeDefinition["layoutOriginFractionY"] = sol::property(&Element::getAbsolutePosAlignY_lua, &Element::setAbsolutePosAlignY_lua);
			usertypeDefinition["layoutWidthFraction"] = sol::property(&Element::getWidthProportional_lua, &Element::setWidthProportional_lua);

			// Custom property accessor functions.
			usertypeDefinition["hasProperty"] = &Element::hasProperty_lua;
			usertypeDefinition["getPropertyType"] = &Element::getPropertyType_lua;;
			usertypeDefinition["getPropertyBool"] = &Element::getPropertyBool_lua;
			usertypeDefinition["getPropertyFloat"] = &Element::getPropertyFloat_lua;
			usertypeDefinition["getPropertyInt"] = &Element::getPropertyInt_lua;
			usertypeDefinition["getPropertyObject"] = &Element::getPropertyObject_lua;
			usertypeDefinition["setPropertyBool"] = &Element::setPropertyBool_lua;
			usertypeDefinition["setPropertyFloat"] = &Element::setPropertyFloat_lua;
			usertypeDefinition["setPropertyInt"] = &Element::setPropertyInt_lua;
			usertypeDefinition["setPropertyObject"] = &Element::setPropertyObject_lua;

			// Event functions.
			usertypeDefinition["registerBefore"] = &Element::registerBefore_lua;
			usertypeDefinition["registerAfter"] = &Element::registerAfter_lua;
			usertypeDefinition["unregisterBefore"] = &Element::unregisterBefore_lua;
			usertypeDefinition["unregisterAfter"] = &Element::unregisterAfter_lua;

			// Legacy event functions.
			usertypeDefinition["register"] = &Element::register_lua;
			usertypeDefinition["unregister"] = &Element::unregister_lua;
			usertypeDefinition["forwardEvent"] = &Element::forwardEvent_lua;
			usertypeDefinition["triggerEvent"] = &Element::triggerEvent_lua;

			// Layout functions.
			usertypeDefinition["destroyChildren"] = &Element::destroyChildren;
			usertypeDefinition["findChild"] = &Element::findChild_lua;
			usertypeDefinition["getContentElement"] = &Element::getContentElement;
			usertypeDefinition["getTopLevelMenu"] = &Element::getTopLevelParent;
			usertypeDefinition["getTopLevelParent"] = &Element::getTopLevelParent;
			usertypeDefinition["reorderChildren"] = &Element::reorderChildren_lua;
			usertypeDefinition["updateLayout"] = &Element::updateLayout_lua;

			// Creation/destruction functions.
			usertypeDefinition["createBlock"] = &Element::createBlock_lua;
			usertypeDefinition["createButton"] = &Element::createButton_lua;
			usertypeDefinition["createDivider"] = &Element::createDivider_lua;
			usertypeDefinition["createFillBar"] = &Element::createFillBar_lua;
			usertypeDefinition["createHypertext"] = &Element::createHypertext_lua;
			usertypeDefinition["createHorizontalScrollPane"] = &Element::createHorizontalScrollPane_lua;
			usertypeDefinition["createImage"] = &Element::createImage_lua;
			usertypeDefinition["createLabel"] = &Element::createLabel_lua;
			usertypeDefinition["createNif"] = &Element::createNif_lua;
			usertypeDefinition["createParagraphInput"] = &Element::createParagraphInput_lua;
			usertypeDefinition["createRect"] = &Element::createRect_lua;
			usertypeDefinition["createSlider"] = &Element::createSlider_lua;
			usertypeDefinition["createSliderVertical"] = &Element::createSliderVertical_lua;
			usertypeDefinition["createTextInput"] = &Element::createTextInput_lua;
			usertypeDefinition["createTextSelect"] = &Element::createTextSelect_lua;
			usertypeDefinition["createThinBorder"] = &Element::createThinBorder_lua;
			usertypeDefinition["createVerticalScrollPane"] = &Element::createVerticalScrollPane_lua;

			usertypeDefinition["destroy"] = &Element::destroy;
		}
	}
}
