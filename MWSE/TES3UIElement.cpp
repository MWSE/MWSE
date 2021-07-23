#include "string.h"

#include "TES3UIInventoryTile.h"
#include "TES3UIManager.h"
#include "TES3UIElement.h"
#include "TES3UIWidgets.h"

#include "TES3Inventory.h"
#include "TES3ItemData.h"
#include "TES3GameFile.h"
#include "TES3MobileObject.h"
#include "TES3Object.h"

#include "LuaUtil.h"
#include "MemoryUtilLua.h"
#include "TES3Util.h"
#include "TES3UIManagerLua.h"

namespace TES3 {
	namespace UI {
		typedef Element* (__cdecl *TES3_UI_WidgetFactoryMethod_t)(Element*);

		const auto TES3_ui_createBlock = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, bool)>(0x588980);
		const auto TES3_ui_createImage = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, const char*, bool)>(0x588630);
		const auto TES3_ui_createLabel = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, const char*, bool, bool)>(0x588BE0);
		const auto TES3_ui_createNif = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, const char*, bool)>(0x588830);
		const auto TES3_ui_createRect = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, bool, bool)>(0x588340);
		const auto TES3_ui_createWidget = reinterpret_cast<Element* (__thiscall *)(Element*, UI_ID, TES3_UI_WidgetFactoryMethod_t, bool)>(0x588140);
		const auto TES3_deleting_dtor_UIElement = reinterpret_cast<void (__thiscall *)(Element*, char)>(0x578880);
		const auto TES3_ui_destroyChildren = reinterpret_cast<void (__thiscall *)(Element*)>(0x578820);

		const auto TES3_ui_factoryButton = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x639420);
		const auto TES3_ui_factoryDragFrame = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x63B5E0);
		const auto TES3_ui_factoryFillBar = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x63E640);
		const auto TES3_ui_factoryFixedFrame = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x642550);
		const auto TES3_ui_factoryHorzScrollPane = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x648C10);
		const auto TES3_ui_factoryHypertext = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x640970);
		const auto TES3_ui_factoryParagraphInput = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x643130);
		const auto TES3_ui_factorySlider = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x645EB0);
		const auto TES3_ui_factorySliderVert = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x646670);
		const auto TES3_ui_factoryTextInput = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x64A8E0);
		const auto TES3_ui_factoryTextSelect = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x64C180);
		const auto TES3_ui_factoryVertScrollPane = reinterpret_cast<TES3_UI_WidgetFactoryMethod_t>(0x648FD0);

		const auto TES3_ui_findChildElement = reinterpret_cast<Element* (__thiscall *)(const Element*, UI_ID)>(0x582DE0);
		const auto TES3_ui_getTopLevelParent = reinterpret_cast<Element* (__thiscall *)(const Element*)>(0x582EF0);
		const auto TES3_ui_performLayout = reinterpret_cast<Element* (__thiscall *)(Element*, bool)>(0x583B70);
		const auto TES3_ui_setAutoHeight = reinterpret_cast<void (__thiscall *)(Element*, bool)>(0x581400);
		const auto TES3_ui_setAutoWidth = reinterpret_cast<void (__thiscall *)(Element*, bool)>(0x5813C0);
		const auto TES3_ui_setVisible = reinterpret_cast<void (__thiscall *)(Element*, bool)>(0x57F2A0);
		const auto TES3_ui_timingUpdate = reinterpret_cast<long (__thiscall *)(Element*)>(0x583B60);
		const auto TES3_ui_updateLayout_propagateFlow = reinterpret_cast<void(__thiscall*)(Element*)>(0x584850);
		const auto TES3_ui_updateLayoutContent = reinterpret_cast<void(__thiscall*)(Element*)>(0x583760);

		const auto TES3_ui_getProperty = reinterpret_cast<PropertyValue* (__thiscall *)(const Element*, PropertyValue*, Property, PropertyType, const Element*, bool)>(0x581440);
		const auto TES3_ui_getPropertyType = reinterpret_cast<PropertyType (__thiscall*)(const Element*, Property)>(0x582AD0);
		const auto TES3_ui_getText = reinterpret_cast<const char* (__thiscall *)(const Element*)>(0x580BB0);
		const auto TES3_ui_setProperty = reinterpret_cast<void (__thiscall *)(Element*, Property, PropertyValue, PropertyType)>(0x581F30);
		const auto TES3_ui_setText = reinterpret_cast<void(__thiscall *)(Element*, const char*)>(0x58AD30);
		const auto TES3_ui_setIcon = reinterpret_cast<void(__thiscall *)(Element*, const char*)>(0x58AE20);
		const auto TES3_ui_setIconString = reinterpret_cast<void(__thiscall *)(Element*, String)>(0x58AF10);

		const auto TES3_ui_updateSceneGraph = reinterpret_cast<void(__thiscall *)(Element*)>(0x587000);

		using mwse::lua::valueDefaultAsNil;
		using mwse::lua::getPropertyFromObject;

		//
		// Widget creation/destruction methods
		//

		Element* Element::createBlock(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createBlock(this, id, bReplaceThisElement);
		}

		Element* Element::createButton(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryButton, bReplaceThisElement);
		}

		Element* Element::createDivider(UI_ID id, bool bReplaceThisElement) {
			auto divider = TES3_ui_createImage(this, id, "Textures\\menu_divider.tga", bReplaceThisElement);
			divider->borderAllSides = 8;
			divider->widthProportional = 1.0;
			divider->flagExtendImageToBounds = 1;
			return divider;
		}

		Element* Element::createDragFrame(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryDragFrame, bReplaceThisElement);
		}

		Element* Element::createFillBar(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryFillBar, bReplaceThisElement);
		}

		Element* Element::createFixedFrame(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryFixedFrame, bReplaceThisElement);
		}

		Element* Element::createHorizontalScrollPane(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryHorzScrollPane, bReplaceThisElement);
		}

		Element* Element::createHypertext(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryHypertext, bReplaceThisElement);
		}

		Element* Element::createImage(UI_ID id, const char* imagePath, bool bReplaceThisElement) {
			return TES3_ui_createImage(this, id, imagePath, bReplaceThisElement);
		}

		Element* Element::createLabel(UI_ID id, const char* text, bool bBlackText, bool bReplaceThisElement) {
			return TES3_ui_createLabel(this, id, text, bBlackText, bReplaceThisElement);
		}

		Element* Element::createNif(UI_ID id, const char* path, bool bReplaceThisElement) {
			return TES3_ui_createNif(this, id, path, bReplaceThisElement);
		}

		Element* Element::createParagraphInput(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryParagraphInput, bReplaceThisElement);
		}

		Element* Element::createRect(UI_ID id, bool bReplaceThisElement, bool bRandomColour) {
			return TES3_ui_createRect(this, id, bReplaceThisElement, bRandomColour);
		}

		Element* Element::createSlider(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factorySlider, bReplaceThisElement);
		}

		Element* Element::createSliderVertical(UI_ID id, bool bReplaceThisElement) {
			Element* slider = TES3_ui_createWidget(this, id, TES3_ui_factorySliderVert, bReplaceThisElement);
			// Set is_part prop to correct value for WidgetScrollBar
			static Property propPart = registerProperty("PartScrollBar");
			slider->setProperty(Property::is_part, propPart);
			return slider;
		}

		Element* Element::createTextInput(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryTextInput, bReplaceThisElement);
		}

		Element* Element::createTextSelect(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryTextSelect, bReplaceThisElement);
		}

		Element* Element::createThinBorder(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createNif(this, id, "menu_thin_border.nif", bReplaceThisElement);
		}

		Element* Element::createVerticalScrollPane(UI_ID id, bool bReplaceThisElement) {
			return TES3_ui_createWidget(this, id, TES3_ui_factoryVertScrollPane, bReplaceThisElement);
		}

		void Element::destroy() {
			TES3_deleting_dtor_UIElement(this, 1);
		}

		void Element::destroyChildren() {
			TES3_ui_destroyChildren(this);
		}

		//
		// Layout methods
		//

		Element* Element::findChild(UI_ID id) const {
			return TES3_ui_findChildElement(this, id);
		}

		Element* Element::findChild_lua(sol::object id) const {
			return findChild(mwse::lua::getUIIDFromObject(id));
		}

		int Element::getIndexOfChild(const Element *child) const {
			Element** it = vectorChildren.begin;
			Element** end = vectorChildren.end;

			for (int i = 0; it != this->vectorChildren.end; ++i, ++it) {
				if (*it == child) {
					return i;
				}
			}
			return -1;
		}

		Element* Element::getContentElement() {
			Property shunt = getProperty(PropertyType::Property, Property::shunt_children).propertyValue;
			return int(shunt) ? findChild(static_cast<UI_ID>(shunt)) : this;
		}

		Element* Element::getTopLevelParent() {
			return TES3_ui_getTopLevelParent(this);
		}

		Element* Element::performLayout(bool bUpdateTimestamp) {
			return TES3_ui_performLayout(this, bUpdateTimestamp);
		}

		const auto TES3_ui_reattachToParent = reinterpret_cast<void(__thiscall*)(Element*, Element*)>(0x57B850);
		void Element::reattachToParent(Element* parent) {
			TES3_ui_reattachToParent(this, parent);
		}

		bool Element::reorderChildren(int insertBefore, int moveFrom, int count) {
			// Move <count> children from index <moveFrom> to index <insertBefore>
			// Negative positions indicate distance from end, negative count moves all children after moveFrom
			if (vectorChildren.begin == vectorChildren.end) {
				return false;
			}

			Element** begin = vectorChildren.begin;
			Element** end = vectorChildren.end;
			int childCount = int(end - begin);

			// Convert negative indices to distance from end, and negative count to move all items after moveFrom
			insertBefore = (insertBefore < 0) ? childCount + insertBefore : insertBefore;
			moveFrom = (moveFrom < 0) ? childCount + moveFrom : moveFrom;
			count = (count < 0) ? childCount - moveFrom : count;

			// Array bounds checks
			if (insertBefore < 0 || insertBefore > childCount || moveFrom < 0 || moveFrom >= childCount || count < 0 || count + moveFrom > childCount) {
				return false;
			}

			// Copy <count> elements into a temp buffer
			Element** from = begin + moveFrom, ** to = begin + insertBefore;
			std::vector<const Element*> temp(from, from + count);

			// Slide <shift> children in-place, then write buffer into correct location
			if (moveFrom < insertBefore) {
				int shift = insertBefore - moveFrom;
				memmove_s(from, sizeof(Element*) * shift, from + count, sizeof(Element*) * shift);
				memmove_s(to - count, sizeof(Element*) * count, &*temp.begin(), sizeof(Element*) * count);
			}
			else {
				int shift = moveFrom - insertBefore;
				memmove_s(to + count, sizeof(Element*) * shift, to, sizeof(Element*) * shift);
				memmove_s(to, sizeof(Element*) * count, &*temp.begin(), sizeof(Element*) * count);
			}

			return true;
		}

		bool Element::getAutoHeight() const {
			return flagAutoHeight;
		}

		void Element::setAutoHeight(bool bAuto) {
			return TES3_ui_setAutoHeight(this, bAuto);
		}

		bool Element::getAutoWidth() const {
			return flagAutoWidth;
		}

		void Element::setAutoWidth(bool bAuto) {
			return TES3_ui_setAutoWidth(this, bAuto);
		}

		void Element::setVisible(bool bVisible) {
			return TES3_ui_setVisible(this, bVisible);
		}

		long Element::timingUpdate() {
			return TES3_ui_timingUpdate(this);
		}

		//
		// Property methods
		//

		PropertyValue* Element::getProperty(PropertyValue* propValue, Property prop, PropertyType propType, const Element* element, bool checkInherited) const {
			return TES3_ui_getProperty(this, propValue, prop, propType, element, checkInherited);
		}

		PropertyValue Element::getProperty(PropertyType propType, Property prop) const {
			PropertyValue v;
			TES3_ui_getProperty(this, &v, prop, propType, nullptr, 0);
			return v;
		}

		PropertyType Element::getPropertyType(Property prop) const {
			return TES3_ui_getPropertyType(this, prop);
		}

		bool Element::hasProperty(Property prop) const {
			return TES3_ui_getPropertyType(this, prop) != PropertyType::INVALID;
		}

		void Element::setProperty(Property prop, PropertyValue value, PropertyType type) {
			TES3_ui_setProperty(this, prop, value, type);
		}

		void Element::setProperty(Property prop, int value) {
			PropertyValue v;
			v.integerValue = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::Integer);
		}

		void Element::setProperty(Property prop, float value) {
			PropertyValue v;
			v.floatValue = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::Float);
		}

		void Element::setProperty(Property prop, void* value) {
			PropertyValue v;
			v.ptrValue = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::Pointer);
		}

		void Element::setProperty(Property prop, Property value) {
			PropertyValue v;
			v.propertyValue = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::Property);
		}

		void Element::setProperty(Property prop, EventCallback value) {
			PropertyValue v;
			v.eventCallback = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::EventCallback);
		}

		void Element::setProperty(Property prop, PropertyAccessCallback value) {
			PropertyValue v;
			v.propertyCallback = value;
			TES3_ui_setProperty(this, prop, v, PropertyType::PropertyAccessCallback);
		}

		const char* Element::getText() const {
			return TES3_ui_getText(this);
		}

		void Element::setText(const char* text) {
			TES3_ui_setText(this, text);
		}

		void Element::setIcon(const char* path) {
			TES3_ui_setIcon(this, path);
		}

		void Element::setIcon(String path) {
			TES3_ui_setIconString(this, path);
		}

		void Element::updateSceneGraph() {
			TES3_ui_updateSceneGraph(this);
		}

		const char* Element::getName() const {
			return name.cString;
		}

		std::string Element::toJson() const {
			std::ostringstream ss;
			ss << "\"tes3uiElement:" << id << ":" << name.cString << "\"";
			return std::move(ss.str());
		}

		sol::table Element::getChildren_lua(sol::this_state ts) const {
			sol::state_view state = ts;
			sol::table children = state.create_table();
			auto it = vectorChildren.begin;
			auto end = vectorChildren.end;
			for (int i = 1; it != end; ++it, ++i) {
				children[i] = *it;
			}
			return children;
		}

		// Helper function to build a table for an element's properties.
		void addPropertyToTable(sol::table& table, const TES3::UI::TreeNode* node) {
			TES3::UI::TreeNode* sentinal = *reinterpret_cast<TES3::UI::TreeNode**>(0x7D1CEC);
			if (node == sentinal) {
				return;
			}

			table.add(node->item);

			addPropertyToTable(table, node->branchLess);
			addPropertyToTable(table, node->branchGreaterThanOrEqual);
		}

		sol::table Element::getProperties_lua(sol::this_state ts) const {
			sol::state_view state = ts;
			sol::table result = state.create_table();

			addPropertyToTable(result, properties.root->nextLeafOrRoot);

			return result;
		}

		std::string Element::getContentTypeString() const {
			switch (contentType) {
			case TES3::UI::Property::model:
				return "model";
			case TES3::UI::Property::text:
				return "text";
			case TES3::UI::Property::image:
				return "image";
			case TES3::UI::Property::rect:
				return "rect";
			default:
				return "layout";
			}
		}

		static Property propButton, propFillbar, propParagraphInput;
		static Property propScrollBar, propScrollPaneH, propScrollPaneV;
		static Property propTextInput, propTextSelect;
		static UI_ID uiidButtonText, uiidParagraphInputText;
		void deferredPropInit() {
			static bool init = false;
			if (!init) {
				propButton = registerProperty("PartButton");
				propFillbar = registerProperty("PartFillbar");
				propParagraphInput = registerProperty("PartParagraphInput");
				propScrollBar = registerProperty("PartScrollBar");
				propScrollPaneH = registerProperty("PartScrollPaneHor");
				propScrollPaneV = registerProperty("PartScrollPaneVert");
				propTextInput = registerProperty("PartTextInput");
				propTextSelect = registerProperty("PartTextSelect");
				uiidButtonText = registerID("PartButton_text_ptr");
				uiidParagraphInputText = registerID("PartParagraphInput_text_input");
				init = true;
			}
		}

		sol::object Element::makeWidget(sol::this_state ts) {
			deferredPropInit();

			sol::state_view state = ts;
			Property part = getProperty(PropertyType::Property, Property::is_part).propertyValue;

			if (part == propButton) {
				return sol::make_object(state, WidgetButton::fromElement(this));
			}
			else if (part == propFillbar) {
				return sol::make_object(state, WidgetFillbar::fromElement(this));
			}
			else if (part == propParagraphInput) {
				return sol::make_object(state, WidgetParagraphInput::fromElement(this));
			}
			else if (part == propScrollBar) {
				return sol::make_object(state, WidgetScrollBar::fromElement(this));
			}
			else if (part == propScrollPaneH || part == propScrollPaneV) {
				return sol::make_object(state, WidgetScrollPane::fromElement(this));
			}
			else if (part == propTextInput) {
				return sol::make_object(state, WidgetTextInput::fromElement(this));
			}
			else if (part == propTextSelect) {
				return sol::make_object(state, WidgetTextSelect::fromElement(this));
			}

			return sol::nil;
		}

		std::string Element::getWidgetText() const {
			deferredPropInit();

			Property part = getProperty(PropertyType::Property, Property::is_part).propertyValue;

			if (part == propButton) {
				return WidgetButton::fromElement(this)->getText();
			}
			else if (part == propParagraphInput) {
				return std::move(WidgetParagraphInput::fromElement(this)->getText());
			}
			else if (part == propTextInput) {
				return std::move(WidgetTextInput::fromElement(this)->getText());
			}

			return getText();
		}

		void Element::setWidgetText(const char* text) {
			if (text == nullptr) {
				text = "";
			}

			deferredPropInit();

			Property part = getProperty(PropertyType::Property, Property::is_part).propertyValue;

			if (part == propButton) {
				WidgetButton::fromElement(this)->setText(text);
			}
			else if (part == propParagraphInput) {
				WidgetParagraphInput::fromElement(this)->setText(text);
			}
			else if (part == propTextInput) {
				WidgetTextInput::fromElement(this)->setText(text);
			}
			else {
				setText(text);
			}

			getTopLevelParent()->timingUpdate();
		}

		void Element::setWidgetText_lua(sol::object value, sol::this_state ts) {
			// Setting to nil clears text.
			if (value == sol::nil) {
				setWidgetText(nullptr);
			}
			// If it's a string, just set it normally.
			else if (value.is<std::string>()) {
				std::string& text = value.as<std::string&>();
				setWidgetText(text.c_str());
			}
			// Otherwise try to convert it to a string.
			else {
				sol::state_view state = ts;
				std::string text = state["tostring"](value);
				setWidgetText(text.c_str());
			}
		}

		sol::optional<float> Element::getAbsolutePosAlignX_lua() const {
			return valueDefaultAsNil(absolutePosAlignX, -1.0f);
		}

		void Element::setAbsolutePosAlignX_lua(sol::optional<float> value) {
			absolutePosAlignX = value.value_or(-1.0f);
		}

		sol::optional<float> Element::getAbsolutePosAlignY_lua() const {
			return valueDefaultAsNil(absolutePosAlignY, -1.0f);
		}

		void Element::setAbsolutePosAlignY_lua(sol::optional<float> value) {
			absolutePosAlignY = value.value_or(-1.0f);
		}

		float Element::getAlpha() const {
			return colourAlpha;
		}

		void Element::setAlpha(float value) {
			colourAlpha = value;
			flagUsesRGBA = true;
		}

		int Element::getBorderAllSides() const {
			return borderAllSides;
		}

		void Element::setBorderAllSides_lua(sol::optional<int> value) {
			borderAllSides = value.value_or(0);
		}

		sol::optional<int> Element::getBorderBottom_lua() const {
			return valueDefaultAsNil(borderBottom, -1);
		}

		void Element::setBorderBottom_lua(sol::optional<int> value) {
			borderBottom = value.value_or(-1);
		}

		sol::optional<int> Element::getBorderLeft_lua() const {
			return valueDefaultAsNil(borderLeft, -1);
		}

		void Element::setBorderLeft_lua(sol::optional<int> value) {
			borderLeft = value.value_or(-1);
		}

		sol::optional<int> Element::getBorderRight_lua() const {
			return valueDefaultAsNil(borderRight, -1);
		}

		void Element::setBorderRight_lua(sol::optional<int> value) {
			borderRight = value.value_or(-1);
		}

		sol::optional<int> Element::getBorderTop_lua() const {
			return valueDefaultAsNil(borderTop, -1);
		}

		void Element::setBorderTop_lua(sol::optional<int> value) {
			borderTop = value.value_or(-1);
		}

		float Element::getChildAlignX() const {
			return getProperty(TES3::UI::PropertyType::Float, TES3::UI::Property::align_x).floatValue;
		}

		void Element::setChildAlignX(float value) {
			setProperty(TES3::UI::Property::align_x, value);
		}

		float Element::getChildAlignY() const {
			return getProperty(TES3::UI::PropertyType::Float, TES3::UI::Property::align_y).floatValue;
		}

		void Element::setChildAlignY(float value) {
			setProperty(TES3::UI::Property::align_y, value);
		}

		sol::optional<int> Element::getChildOffsetX_lua() const {
			return valueDefaultAsNil(childOffsetX, INT32_MAX);
		}

		void Element::setChildOffsetX_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::child_offset_x, value.value_or(INT32_MAX));
		}

		sol::optional<int> Element::getChildOffsetY_lua() const {
			return valueDefaultAsNil(childOffsetY, INT32_MAX);
		}

		void Element::setChildOffsetY_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::child_offset_y, value.value_or(INT32_MAX));
		}

		sol::table Element::getColor_lua(sol::this_state ts) const {
			sol::state_view state = ts;
			return state.create_table_with(1, colourRed, 2, colourGreen, 3, colourBlue);
		}

		void Element::setColor_lua(sol::table value) {
			colourRed = value[1];
			colourGreen = value[2];
			colourBlue = value[3];
			flagUsesRGBA = true;
		}

		bool Element::getConsumeMouseEvents() const {
			return flagConsumeMouseEvents;
		}

		void Element::setConsumeMouseEvents_lua(sol::optional<bool> value) {
			flagConsumeMouseEvents = value.value_or(true);
		}

		const char* Element::getContentPath() const {
			return contentPath.cString;
		}

		void Element::setContentPath_lua(sol::optional<const char*> value) {
			setIcon(value.value_or(""));
		}

		bool Element::getDisabled() const {
			return toBool(getProperty(PropertyType::Property, Property::disabled).propertyValue);
		}

		void Element::setDisabled(bool value) {
			setProperty(TES3::UI::Property::disabled, toBooleanProperty(value));
		}

		std::string Element::getFlowDirectionString() const {
			auto flow = getProperty(TES3::UI::PropertyType::Property, TES3::UI::Property::flow_direction).propertyValue;
			return (flow == Property::top_to_bottom) ? "top_to_bottom" : "left_to_right";
		}

		void Element::setFlowDirectionString(std::string value) {
			auto prop = (value == "top_to_bottom") ? Property::top_to_bottom : Property::left_to_right;
			setProperty(TES3::UI::Property::flow_direction, prop);
		}

		int Element::getFont() const {
			return font;
		}

		void Element::setFont_lua(sol::optional<int> value) {
			setProperty(Property::font, value.value_or(0));
		}

		int Element::getHeight() const {
			return height;
		}

		void Element::setHeight(int value) {
			setProperty(Property::height, value);
		}

		sol::optional<float> Element::getHeightProportional_lua() const {
			return valueDefaultAsNil(heightProportional, -1.0f);
		}

		void Element::setHeightProportional_lua(sol::optional<float> value) {
			heightProportional = value.value_or(-1.0f);
		}

		bool Element::getImageFilter() const {
			if (sceneNode && texture) {
				using FilterMode = NI::TexturingProperty::FilterMode;
				auto prop = sceneNode->children.at(0)->getProperty(NI::PropertyType::Texturing);
				if (prop) {
					auto texturing = static_cast<NI::TexturingProperty*>(prop.get());
					return texturing->maps.at(0)->filterMode != FilterMode::NEAREST;
				}
			}
			return true;
		}

		void Element::setImageFilter(bool value) {
			if (sceneNode && texture) {
				using FilterMode = NI::TexturingProperty::FilterMode;
				auto prop = sceneNode->children.at(0)->getProperty(NI::PropertyType::Texturing);
				if (prop) {
					auto texturing = static_cast<NI::TexturingProperty*>(prop.get());
					texturing->maps.at(0)->filterMode = value ? FilterMode::BILERP : FilterMode::NEAREST;
					sceneNode->updateProperties();
				}
			}
		}

		float Element::getImageScaleX() const {
			return imageScaleX;
		}

		void Element::setImageScaleX(float value) {
			imageScaleX = value;
			flagContentChanged = true;
		}

		float Element::getImageScaleY() const {
			return imageScaleY;
		}

		void Element::setImageScaleY(float value) {
			imageScaleY = value;
			flagContentChanged = true;
		}

		std::string Element::getJustifyTextString() const {
			auto justify = getProperty(PropertyType::Property, Property::justify).propertyValue;
			if (justify == Property::center) return "center";
			if (justify == Property::right) return "right";
			return "left";
		}

		void Element::setJustifyTextString(std::string value) {
			auto prop = Property::left;
			if (value == "center") {
				prop = Property::center;
			}
			else if (value == "right") {
				prop = Property::right;
			}
			setProperty(TES3::UI::Property::justify, prop);
			flagContentChanged = true;
		}

		sol::optional<int> Element::getMaxHeight_lua() const {
			return valueDefaultAsNil(maxHeight, INT32_MAX);
		}

		void Element::setMaxHeight_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::max_height, value.value_or(INT32_MAX));
		}

		sol::optional<int> Element::getMaxWidth_lua() const {
			return valueDefaultAsNil(maxWidth, INT32_MAX);
		}

		void Element::setMaxWidth_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::max_width, value.value_or(INT32_MAX));
		}

		sol::optional<int> Element::getMinHeight_lua() const {
			return valueDefaultAsNil(minHeight, INT32_MIN);
		}

		void Element::setMinHeight_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::min_height, value.value_or(INT32_MIN));
		}

		sol::optional<int> Element::getMinWidth_lua() const {
			return valueDefaultAsNil(minWidth, INT32_MIN);
		}

		void Element::setMinWidth_lua(sol::optional<int> value) {
			setProperty(TES3::UI::Property::min_width, value.value_or(INT32_MIN));
		}

		int Element::getNodeOffsetX() const {
			return nodeOffsetX;
		}

		void Element::setNodeOffsetX(int value) {
			setProperty(Property::node_offset_x, value);
		}

		int Element::getNodeOffsetY() const {
			return nodeOffsetY;
		}

		void Element::setNodeOffsetY(int value) {
			setProperty(Property::node_offset_y, value);
		}

		int Element::getPaddingAllSides() const {
			return paddingAllSides;
		}

		void Element::setPaddingAllSides_lua(sol::optional<int> value) {
			paddingAllSides = value.value_or(0);
		}

		sol::optional<int> Element::getPaddingBottom_lua() const {
			return valueDefaultAsNil(paddingBottom, -1);
		}

		void Element::setPaddingBottom_lua(sol::optional<int> value) {
			paddingBottom = value.value_or(-1);
		}

		sol::optional<int> Element::getPaddingLeft_lua() const {
			return valueDefaultAsNil(paddingLeft, -1);
		}

		void Element::setPaddingLeft_lua(sol::optional<int> value) {
			paddingLeft = value.value_or(-1);
		}

		sol::optional<int> Element::getPaddingRight_lua() const {
			return valueDefaultAsNil(paddingRight, -1);
		}

		void Element::setPaddingRight_lua(sol::optional<int> value) {
			paddingRight = value.value_or(-1);
		}

		sol::optional<int> Element::getPaddingTop_lua() const {
			return valueDefaultAsNil(paddingTop, -1);
		}

		void Element::setPaddingTop_lua(sol::optional<int> value) {
			paddingTop = value.value_or(-1);
		}

		int Element::getPositionX() const {
			return positionX;
		}

		void Element::setPositionX(int value) {
			setProperty(Property::x_loc, value);
		}

		int Element::getPositionY() const {
			return positionY;
		}

		void Element::setPositionY(int value) {
			setProperty(Property::y_loc, value);
		}

		bool Element::getRepeatKeys() const {
			return toBool(getProperty(TES3::UI::PropertyType::Property, TES3::UI::Property::repeat_keys).propertyValue);
		}

		void Element::setRepeatKeys(bool value) {
			setProperty(TES3::UI::Property::repeat_keys, toBooleanProperty(value));
		}

		bool Element::getScaleMode() const {
			return toBool(scale_mode);
		}

		void Element::setScaleMode(bool value) {
			scale_mode = toBooleanProperty(value);
			flagContentChanged = true;
		}

		NI::Pointer<NI::SourceTexture> Element::getTexture() const {
			return texture;
		}

		void Element::setTexture(NI::Pointer<NI::SourceTexture> value) {
			texture = value;
			contentType = Property::image;
			flagContentChanged = true;
		}

		bool Element::getVisible() const {
			return visible;
		}

		void Element::setVisible_lua(sol::optional<bool> value) {
			setVisible(value.value_or(true));
		}

		int Element::getWidth() const {
			return width;
		}

		void Element::setWidth(int value) {
			setProperty(TES3::UI::Property::width, value);
		}

		sol::optional<float> Element::getWidthProportional_lua() const {
			return valueDefaultAsNil(widthProportional, -1.0f);
		}

		void Element::setWidthProportional_lua(sol::optional<float> value) {
			widthProportional = value.value_or(-1.0f);
		}

		bool Element::getWrapText() const {
			return toBool(getProperty(TES3::UI::PropertyType::Property, TES3::UI::Property::wrap_text).propertyValue);
		}

		void Element::setWrapText(bool value) {
			setProperty(TES3::UI::Property::wrap_text, toBooleanProperty(value));
			flagContentChanged = true;
		}

		bool Element::hasProperty_lua(sol::object key) const {
			return hasProperty(getPropertyFromObject(key));
		}

		PropertyType Element::getPropertyType_lua(sol::object key) const {
			return getPropertyType(getPropertyFromObject(key));
		}

		bool Element::getPropertyBool_lua(sol::object key) const {
			return toBool(getProperty(TES3::UI::PropertyType::Property, getPropertyFromObject(key)).propertyValue);
		}

		void Element::setPropertyBool_lua(sol::object key, bool value) {
			setProperty(getPropertyFromObject(key), toBooleanProperty(value));
		}

		int Element::getPropertyInt_lua(sol::object key) const {
			return getProperty(TES3::UI::PropertyType::Integer, getPropertyFromObject(key)).integerValue;
		}

		void Element::setPropertyInt_lua(sol::object key, int value) {
			setProperty(getPropertyFromObject(key), value);
		}

		float Element::getPropertyFloat_lua(sol::object key) const {
			return getProperty(TES3::UI::PropertyType::Float, getPropertyFromObject(key)).floatValue;
		}

		void Element::setPropertyFloat_lua(sol::object key, float value) {
			setProperty(getPropertyFromObject(key), value);
		}

		sol::object Element::getPropertyObject_lua(sol::this_state ts, sol::object key, sol::optional<std::string> typeCast) const {
			sol::state_view state = ts;

			TES3::UI::Property prop = getPropertyFromObject(key);
			auto ptr = getProperty(TES3::UI::PropertyType::Pointer, prop).ptrValue;

			if (!ptr) {
				return sol::nil;
			}

			if (!typeCast) {
				auto object = static_cast<TES3::BaseObject*>(ptr);

				switch (object->objectType) {
				case TES3::ObjectType::MobileCreature:
				case TES3::ObjectType::MobileNPC:
				case TES3::ObjectType::MobilePlayer:
					return static_cast<TES3::MobileObject*>(ptr)->getOrCreateLuaObject(state);
				default:
					return static_cast<TES3::BaseObject*>(ptr)->getOrCreateLuaObject(state);
				}
			}
			else {
				// New types are added to the mwse.memory.convertTo table, defined in MemoryUtilLua.cpp
				sol::protected_function converter = mwse::lua::convertTo[typeCast.value()];
				if (converter) {
					return converter(DWORD(ptr));
				}
			}

			return sol::nil;
		}

		void Element::setPropertyObject_lua(sol::object key, sol::object value) {
			if (value.is<void*>()) {
				TES3::UI::Property prop = getPropertyFromObject(key);
				void* ptr = *static_cast<void**>(value.as<void*>());
				setProperty(prop, ptr);
			}
			else {
				throw std::invalid_argument("Could not determine type of value to set.");
			}
		}

		void Element::registerBefore_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority) {
			if (!callback.valid()) {
				std::stringstream ss;
				ss << "UI register before-event has invalid callback: target " << (name.cString ? name.cString : "(unnamed)") << ", event " << eventID;
				throw std::invalid_argument(ss.str());
			}

			// Map friendlier event names to standard UI events
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}

			// Check UI registry for custom event
			mwse::lua::registerBeforeUIEvent(this, prop.value(), callback, priority.value_or(0.0));
		}

		void Element::registerAfter_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority) {
			if (!callback.valid()) {
				std::stringstream ss;
				ss << "UI register after-event has invalid callback: target " << (name.cString ? name.cString : "(unnamed)") << ", event " << eventID;
				throw std::invalid_argument(ss.str());
			}

			// Map friendlier event names to standard UI events
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}

			// Check UI registry for custom event
			mwse::lua::registerAfterUIEvent(this, prop.value(), callback, priority.value_or(0.0));
		}

		void Element::register_lua(const std::string& eventID, sol::object callback) {
			// Callback is supposed to be an address. Dangerous advanced usage, sets the actual property.
			if (callback.is<unsigned int>()) {
				// Map friendlier event names to standard UI events
				auto prop = getStandardEventFromName(eventID);
				if (!prop) {
					prop = registerProperty(eventID.c_str());
				}

				setProperty(prop.value(), reinterpret_cast<TES3::UI::EventCallback>(callback.as<unsigned int>()));
			}

			// Callback is a lua event, put it into our custom handler system.
			else if (callback.is<sol::protected_function>()) {
				if (!callback.valid()) {
					std::stringstream ss;
					ss << "UI register event has invalid callback: target " << (name.cString ? name.cString : "(unnamed)") << ", event " << eventID;
					throw std::invalid_argument(ss.str());
				}

				// Map friendlier event names to standard UI events
				auto prop = getStandardEventFromName(eventID);
				if (!prop) {
					prop = TES3::UI::registerProperty(eventID.c_str());
				}

				// Check UI registry for custom event
				mwse::lua::registerUIEvent(this, prop.value(), callback.as<sol::protected_function>());
			}

			// Unrecognized type, spit an error.
			else {
				std::stringstream ss;
				ss << "UI register event has invalid callback type: target " << (name.cString ? name.cString : "(unnamed)") << ", event " << eventID;
				throw std::invalid_argument(ss.str());
			}
		}

		void Element::unregisterBefore_lua(const std::string& eventID, sol::protected_function callback) {
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}
			mwse::lua::unregisterBeforeUIEvent(this, prop.value(), callback);
		}

		void Element::unregisterAfter_lua(const std::string& eventID, sol::protected_function callback) {
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}
			mwse::lua::unregisterAfterUIEvent(this, prop.value(), callback);
		}

		void Element::unregister_lua(const std::string& eventID) {
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}
			mwse::lua::unregisterUIEvent(this, prop.value());
		}

		void Element::forwardEvent_lua(sol::table eventData) const {
			if (eventData.valid()) {
				mwse::lua::eventForwarder(eventData);
			}
		}

		void Element::triggerEvent_lua(sol::object params) {
			if (params.get_type() == sol::type::table) {
				auto eventData = params.as<sol::table>();
				mwse::lua::triggerEvent(this, eventData["id"], eventData["data0"], eventData["data1"]);
				return;
			}

			// Map friendlier event names to standard UI events
			auto eventID = params.as<std::string>();
			auto prop = getStandardEventFromName(eventID);
			if (!prop) {
				prop = TES3::UI::registerProperty(eventID.c_str());
			}

			mwse::lua::triggerEvent(this, prop.value(), 0, 0);
		}

		bool Element::reorderChildren_lua(sol::object insertBefore, sol::object moveFrom, int count) {
			int indexInsertBefore, indexMoveFrom;

			if (insertBefore.is<int>()) {
				indexInsertBefore = insertBefore.as<int>();
			}
			else {
				indexInsertBefore = getIndexOfChild(insertBefore.as<Element*>());
				if (indexInsertBefore == -1) {
					return false;
				}
			}
			if (moveFrom.is<int>()) {
				indexMoveFrom = moveFrom.as<int>();
			}
			else {
				indexMoveFrom = getIndexOfChild(moveFrom.as<Element*>());
				if (indexMoveFrom == -1) {
					return false;
				}
			}

			return reorderChildren(indexInsertBefore, indexMoveFrom, count);
		}

		void Element::updateLayout_lua(sol::optional<bool> updateTimestamp) {
			performLayout(updateTimestamp.value_or(true));
		}

		Element* Element::createBlock_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createBlock(id);
		}

		Element* Element::createButton_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			Element* element = createButton(id);

			auto text = mwse::lua::getOptionalParam<const char*>(params, "text", nullptr);
			if (text) {
				element->setText(text);
			}

			return element;
		}

		Element* Element::createDivider_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createDivider(id);
		}

		Element* Element::createFillBar_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");

			Element* element = createFillBar(id);
			auto fillbar = WidgetFillbar::fromElement(element);
			fillbar->setCurrent(mwse::lua::getOptionalParam(params, "current", 0));
			fillbar->setMax(mwse::lua::getOptionalParam(params, "max", 0));

			return element;
		}

		Element* Element::createHorizontalScrollPane_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			Element* scrollpane = createHorizontalScrollPane(id);

			// Add mouse wheel handlers (see event dispatch patch in TES3UIManager.cpp)
			scrollpane->setProperty(TES3::UI::Property::event_mouse_scroll_down, onScrollPaneMousewheel);
			scrollpane->setProperty(TES3::UI::Property::event_mouse_scroll_up, onScrollPaneMousewheel);

			return scrollpane;
		}

		Element* Element::createHypertext_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createHypertext(id);
		}

		Element* Element::createImage_lua(sol::optional<sol::table> params) {
			std::string path = mwse::lua::getOptionalParam<const char*>(params, "path", "");
			if (path.empty()) {
				throw std::invalid_argument("createImage: path argument is required.");
			}

			// Sanitize path.
			std::replace(path.begin(), path.end(), '/', '\\');

			// Make sure the file exists.
			if (mwse::tes3::resolveAssetPath(path.c_str()) == 0) {
				throw std::invalid_argument("Provided file does not exist.");
			}

			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createImage(id, path.c_str());
		}

		Element* Element::createLabel_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			const char* text = mwse::lua::getOptionalParam<const char*>(params, "text", "(nil)");
			return createLabel(id, text);
		}

		Element* Element::createNif_lua(sol::optional<sol::table> params) {
			std::string path = mwse::lua::getOptionalParam<const char*>(params, "path", "");
			if (path.empty()) {
				throw std::invalid_argument("createNif: path argument is required.");
			}

			// Sanitize path.
			std::replace(path.begin(), path.end(), '/', '\\');

			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createNif(id, path.c_str());
		}

		Element* Element::createParagraphInput_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createParagraphInput(id);
		}

		Element* Element::createRect_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			bool randomColor = mwse::lua::getOptionalParam(params, "randomizeColor", false);
			auto element = createRect(id, false, randomColor);

			if (!randomColor) {
				auto color = mwse::lua::getOptionalParamVector3(params, "color");
				if (color) {
					element->colourRed = color.value().x;
					element->colourGreen = color.value().y;
					element->colourBlue = color.value().z;
					element->flagUsesRGBA = true;
				}
			}

			return element;
		}

		Element* Element::createSlider_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			auto element = createSlider(id);

			auto slider = WidgetScrollBar::fromElement(element);
			slider->setCurrent(mwse::lua::getOptionalParam(params, "current", 0));
			slider->setMax(mwse::lua::getOptionalParam(params, "max", 0));
			slider->setStepX(mwse::lua::getOptionalParam(params, "step", 1));
			slider->setJumpX(mwse::lua::getOptionalParam(params, "jump", 5));

			return element;
		}

		Element* Element::createSliderVertical_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			auto element = createSliderVertical(id);

			auto slider = WidgetScrollBar::fromElement(element);
			slider->setCurrent(mwse::lua::getOptionalParam(params, "current", 0));
			slider->setMax(mwse::lua::getOptionalParam(params, "max", 0));
			slider->setStepX(mwse::lua::getOptionalParam(params, "step", 1));
			slider->setJumpX(mwse::lua::getOptionalParam(params, "jump", 5));

			return element;
		}

		Element* Element::createTextInput_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createTextInput(id);
		}

		Element* Element::createTextSelect_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			auto element = createTextSelect(id);

			auto text = mwse::lua::getOptionalParam<const char*>(params, "text", nullptr);
			if (text) {
				element->setText(text);
			}

			auto state = mwse::lua::getOptionalParam<int>(params, "state");
			if (state) {
				auto textSelect = TES3::UI::WidgetTextSelect::fromElement(element);
				textSelect->setState(state.value());
			}

			return element;
		}

		Element* Element::createThinBorder_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			return createThinBorder(id);
		}

		Element* Element::createVerticalScrollPane_lua(sol::optional<sol::table> params) {
			auto id = mwse::lua::getOptionalUIID(params, "id");
			auto element = createVerticalScrollPane(id);

			if (mwse::lua::getOptionalParam(params, "hideFrame", false)) {
				element->setIcon("");
			}

			// Add mouse wheel handlers (see event dispatch patch in TES3UIManager.cpp)
			element->setProperty(TES3::UI::Property::event_mouse_scroll_down, TES3::UI::onScrollPaneMousewheel);
			element->setProperty(TES3::UI::Property::event_mouse_scroll_up, TES3::UI::onScrollPaneMousewheel);

			return element;
		}

		//
		// Patch methods
		//

		void Element::patchUpdateLayout_propagateFlow() {
			// Call original function
			TES3_ui_updateLayout_propagateFlow(this);

			// On size change, reflow wrapped text
			if (flagSizeChanged && rawText.length) {
				auto wrapped = getProperty(PropertyType::Property, Property::wrap_text).propertyValue;
				if (wrapped == Property::boolean_true) {
					flagContentChanged = 1;
					TES3_ui_updateLayoutContent(this);
				}
			}
		}

	}
}
