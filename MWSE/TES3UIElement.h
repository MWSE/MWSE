#pragma once

#include "TES3Defines.h"

#include "TES3UIManager.h"
#include "TES3UIString.h"
#include "TES3UITree.h"
#include "TES3UIVector.h"

#include "NINode.h"
#include "NISourceTexture.h"

namespace TES3::UI {
	struct Element {
		char tag; // 0x0
		String name; // 0x4
		short id; // 0x14
		Vector<Element*> vectorChildren; // 0x18
		Element* parent; // 0x28
		Tree properties; // 0x2C
		Element* uielement_3C;
		Vector<void> vectorVerts_40;
		Vector<void> vector_50;
		Vector<void> vector_60;
		int updateReqTimestamp; // 0x70
		bool flagExtendImageToBounds; // 0x74
		bool visible; // 0x75
		bool visibleAtLastUpdate; // 0x76
		bool flagColourChanged; // 0x77
		bool flagPosChanged; // 0x78
		bool flagSizeChanged; // 0x79
		bool flagVisibilityChanged; // 0x7A
		bool flagClippingChanged; // 0x7B
		bool flagContentChanged; // 0x7C
		char unknown_0x7D;
		char unknown_0x7E;
		bool flagConsumeMouseEvents; // 0x7F
		char unknown_0x80;
		char unknown_0x81;
		char unknown_0x82;
		char unknown_0x83;
		int unknown_0x84;
		NI::Pointer<NI::Node> sceneNode; // 0x88
		void* extraData; // 0x8C
		NI::Pointer<NI::SourceTexture> texture; // 0x90
		int cached_offsetX; // 0x94
		int cached_offsetY; // 0x98
		int nodeMinX; // 0x9C
		int nodeMaxX; // 0xA0
		int nodeMinY; // 0xA4
		int nodeMaxY; // 0xA8
		int cached_unknown_x; // 0xAC
		int cached_unknown_y; // 0xB0
		int borderAllSides; // 0xB4
		int borderLeft; // 0xB8
		int borderRight; // 0xBC
		int borderBottom; // 0xC0
		int borderTop; // 0xC4
		int paddingAllSides; // 0xC8
		int paddingLeft; // 0xCC
		int paddingRight; // 0xD0
		int paddingBottom; // 0xD4
		int paddingTop; // 0xD8
		int positionX; // 0xDC
		int positionY; // 0xE0
		int cached_screenX; // 0xE4
		int cached_screenY; // 0xE8
		int width; // 0xEC
		int height; // 0xF0
		int childOffsetX; // 0xF4
		int childOffsetY; // 0xF8
		int minWidth; // 0xFC
		int minHeight; // 0x100
		int maxWidth; // 0x104
		int maxHeight; // 0x108
		int inheritedWidth; // 0x10C
		int inheritedHeight; // 0x110
		bool flagAutoWidth; // 0x114
		bool flagAutoHeight; // 0x115
		float widthProportional; // 0x118
		float heightProportional; // 0x1C
		float absolutePosAlignX; // 0x120
		float absolutePosAlignY; // 0x124
		int clipMinX; // 0x128
		int clipMaxX; // 0x12C
		int clipMinY; // 0x130
		int clipMaxY; // 0x134
		Property scale_mode; // 0x138
		int nodeOffsetX; // 0x13C
		int nodeOffsetY; // 0x140
		float imageScaleX; // 0x144
		float imageScaleY; // 0x148
		float colourRed; // 0x14C
		float colourGreen; // 0x150
		float colourBlue; // 0x154
		float colourAlpha; // 0x158
		int font; // 0x15C
		Property contentType; // 0x160
		String rawText; // 0x164
		String contentPath; // 0x168

		Element() = delete;
		~Element() = delete;

		//
		// Widget creation/destruction methods
		//

		Element* createBlock(UI_ID id, bool bReplaceThisElement = false);
		Element* createButton(UI_ID id, bool bReplaceThisElement = false);
		Element* createDivider(UI_ID id, bool bReplaceThisElement = false);
		Element* createDragFrame(UI_ID id, bool bReplaceThisElement = false);
		Element* createFillBar(UI_ID id, bool bReplaceThisElement = false);
		Element* createFixedFrame(UI_ID id, bool bReplaceThisElement = false);
		Element* createHorizontalScrollPane(UI_ID id, bool bReplaceThisElement = false);
		Element* createHypertext(UI_ID id, bool bReplaceThisElement = false);
		Element* createImage(UI_ID id, const char* imagePath, bool bReplaceThisElement = false);
		Element* createLabel(UI_ID id, const char* text, bool bBlackText = 0, bool bReplaceThisElement = false);
		Element* createNif(UI_ID id, const char* path, bool bReplaceThisElement = false);
		Element* createParagraphInput(UI_ID id, bool bReplaceThisElement = false);
		Element* createRect(UI_ID id, bool bReplaceThisElement = 0, bool bRandomColour = false);
		Element* createSlider(UI_ID id, bool bReplaceThisElement = false);
		Element* createSliderVertical(UI_ID id, bool bReplaceThisElement = false);
		Element* createTextInput(UI_ID id, bool bReplaceThisElement = false);
		Element* createTextSelect(UI_ID id, bool bReplaceThisElement = false);
		Element* createThinBorder(UI_ID id, bool bReplaceThisElement = false);
		Element* createVerticalScrollPane(UI_ID id, bool bReplaceThisElement = false);
		void destroy();
		void destroyChildren();

		//
		// Layout methods
		//

		Element* findChild(UI_ID id) const;
		Element* findChild(const char* id) const;
		Element* findChild_lua(sol::object id) const;
		int getIndexOfChild(const Element *child) const;
		Element* getContentElement();
		Element* getTopLevelParent();
		Element* performLayout(bool bUpdateTimestamp = true);
		void reattachToParent(Element* parent);
		bool reorderChildren(int insertBefore, int moveFrom, int count);
		bool getAutoHeight() const;
		void setAutoHeight(bool bAuto);
		bool getAutoWidth() const;
		void setAutoWidth(bool bAuto);
		void setVisible(bool bVisible);
		long timingUpdate();

		//
		// Property methods
		//

		PropertyValue* getProperty(PropertyValue* propValue, Property prop, PropertyType propType, const Element* element = nullptr, bool checkInherited = false) const;
		PropertyValue getProperty(PropertyType propType, Property prop) const;
		PropertyType getPropertyType(Property prop) const;
		bool hasProperty(Property prop) const;
		void removeProperty(Property prop);
		const char* getText() const;

		template <typename T>
		T* getPropertyPointer(Property prop) {
			return static_cast<T*>(getProperty(PropertyType::Pointer, prop).ptrValue);
		}

		template <typename T>
		T* getPropertyPointer(const char* prop) {
			return static_cast<T*>(getProperty(PropertyType::Pointer, registerProperty(prop)).ptrValue);
		}

		void setProperty(Property prop, PropertyValue value, PropertyType type);
		void setProperty(Property prop, int value);
		void setProperty(Property prop, float value);
		void setProperty(Property prop, void* value);
		void setProperty(Property prop, Property value);
		void setProperty(Property prop, EventCallback value);
		void setProperty(Property prop, PropertyAccessCallback value);
		void setText(const char *);
		void setIcon(const char *);

		//
		// Other related this-call functions.
		//

		void updateSceneGraph();

		//
		// Custom functions.
		//

		bool isValid() const;

		const char* getName() const;

		std::string toJson() const;

		// This must be kept as a table-creating function for backwards compatibility.
		sol::table getChildren_lua(sol::this_state ts) const;

		// TODO: Add std collection access to the container so this doesn't need to be copied to a table.
		sol::table getProperties_lua(sol::this_state ts) const;

		const char* getContentTypeString() const;
		const char* getGeneralTypeString() const;

		sol::object makeWidget(sol::this_state ts);
		std::string getWidgetText() const;
		void setWidgetText(const char* text);
		void setWidgetText_lua(sol::object text, sol::this_state ts);

		sol::optional<float> getAbsolutePosAlignX_lua() const;
		void setAbsolutePosAlignX_lua(sol::optional<float> value = FLOAT_FIELD_INACTIVE);
		sol::optional<float> getAbsolutePosAlignY_lua() const;
		void setAbsolutePosAlignY_lua(sol::optional<float> value = FLOAT_FIELD_INACTIVE);
		float getAlpha() const;
		void setAlpha(float value);
		int getBorderAllSides() const;
		void setBorderAllSides_lua(sol::optional<int> value = 0);
		sol::optional<int> getBorderBottom_lua() const;
		void setBorderBottom_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getBorderLeft_lua() const;
		void setBorderLeft_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getBorderRight_lua() const;
		void setBorderRight_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getBorderTop_lua() const;
		void setBorderTop_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		float getChildAlignX() const;
		void setChildAlignX(float value);
		float getChildAlignY() const;
		void setChildAlignY(float value);
		sol::optional<int> getChildOffsetX_lua() const;
		void setChildOffsetX_lua(sol::optional<int> value = INT32_MAX);
		sol::optional<int> getChildOffsetY_lua() const;
		void setChildOffsetY_lua(sol::optional<int> value = INT32_MAX);
		sol::table getColor_lua(sol::this_state ts) const;
		void setColor_lua(sol::table value);
		bool getConsumeMouseEvents() const;
		void setConsumeMouseEvents_lua(sol::optional<bool> value = true);
		const char* getContentPath() const;
		void setContentPath_lua(sol::optional<std::string> value);
		bool getDisabled() const;
		void setDisabled(bool value);
		std::string getFlowDirectionString() const;
		void setFlowDirectionString(std::string value);
		int getFont() const;
		void setFont_lua(sol::optional<int> value = 0);
		int getHeight() const;
		void setHeight(int value);
		sol::optional<float> getHeightProportional_lua() const;
		void setHeightProportional_lua(sol::optional<float> value = 1.0f);
		bool getIgnoreLayoutX() const;
		void setIgnoreLayoutX(bool value);
		bool getIgnoreLayoutY() const;
		void setIgnoreLayoutY(bool value);
		bool getImageFilter() const;
		void setImageFilter(bool value);
		float getImageScaleX() const;
		void setImageScaleX(float value);
		float getImageScaleY() const;
		void setImageScaleY(float value);
		std::string getJustifyTextString() const;
		void setJustifyTextString(std::string value);
		sol::optional<int> getMaxHeight_lua() const;
		void setMaxHeight_lua(sol::optional<int> value = INT32_MAX);
		sol::optional<int> getMaxWidth_lua() const;
		void setMaxWidth_lua(sol::optional<int> value = INT32_MAX);
		sol::optional<int> getMinHeight_lua() const;
		void setMinHeight_lua(sol::optional<int> value = INT32_MIN);
		sol::optional<int> getMinWidth_lua() const;
		void setMinWidth_lua(sol::optional<int> value = INT32_MIN);
		int getNodeOffsetX() const;
		void setNodeOffsetX(int value);
		int getNodeOffsetY() const;
		void setNodeOffsetY(int value);
		int getPaddingAllSides() const;
		void setPaddingAllSides_lua(sol::optional<int> value = 0);
		sol::optional<int> getPaddingBottom_lua() const;
		void setPaddingBottom_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getPaddingLeft_lua() const;
		void setPaddingLeft_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getPaddingRight_lua() const;
		void setPaddingRight_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		sol::optional<int> getPaddingTop_lua() const;
		void setPaddingTop_lua(sol::optional<int> value = INT_FIELD_INACTIVE);
		int getPositionX() const;
		void setPositionX(int value);
		int getPositionY() const;
		void setPositionY(int value);
		bool getRepeatKeys() const;
		void setRepeatKeys(bool value);
		bool getScaleMode() const;
		void setScaleMode(bool value);
		NI::Pointer<NI::SourceTexture> getTexture() const;
		void setTexture(NI::Pointer<NI::SourceTexture> value);
		bool getVisible() const;
		void setVisible_lua(sol::optional<bool> value = true);
		int getWidth() const;
		void setWidth(int value);
		sol::optional<float> getWidthProportional_lua() const;
		void setWidthProportional_lua(sol::optional<float> value = FLOAT_FIELD_INACTIVE);
		bool getWrapText() const;
		void setWrapText(bool value);

		bool hasProperty_lua(sol::object key) const;
		void removeProperty_lua(sol::object key);
		PropertyType getPropertyType_lua(sol::object key) const;
		bool getPropertyBool_lua(sol::object key) const;
		void setPropertyBool_lua(sol::object key, bool value);
		DWORD getPropertyCallback_lua(sol::object key) const;
		void setPropertyCallback_lua(sol::object key, DWORD value);
		int getPropertyInt_lua(sol::object key) const;
		void setPropertyInt_lua(sol::object key, int value);
		float getPropertyFloat_lua(sol::object key) const;
		void setPropertyFloat_lua(sol::object key, float value);
		sol::object getPropertyObject_lua(sol::this_state ts, sol::object key, sol::optional<std::string> typeCast) const;
		void setPropertyObject_lua(sol::object key, sol::object value);
		Property getPropertyProperty_lua(sol::object key) const;
		void setPropertyProperty_lua(sol::object key, sol::object value);

		LuaData* getLuaDataContainer() const;
		sol::object getAllLuaData() const;
		sol::object getLuaData(const std::string_view& key) const;
		void setLuaData(sol::this_state ts, const std::string_view& key, sol::object value);

		void registerBefore_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority);
		void registerAfter_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority);
		void register_lua(const std::string& eventID, sol::object callback);
		bool unregisterBefore_lua(const std::string& eventID, sol::protected_function callback);
		bool unregisterAfter_lua(const std::string& eventID, sol::protected_function callback);
		bool unregister_lua(const std::string& eventID);
		void forwardEvent_lua(sol::table eventData) const;
		void triggerEvent_lua(sol::object params);

		void saveMenuPosition();
		bool loadMenuPosition();

		bool reorder_lua(sol::table params);
		bool reorderChildren_lua(sol::object insertBefore, sol::object moveFrom, int count);
		void updateLayout_lua(sol::optional<bool> updateTimestamp = true);

		Element* createBlock_lua(sol::optional<sol::table> params);
		Element* createButton_lua(sol::optional<sol::table> params);
		Element* createDivider_lua(sol::optional<sol::table> params);
		Element* createFillBar_lua(sol::optional<sol::table> params);
		Element* createHorizontalScrollPane_lua(sol::optional<sol::table> params);
		Element* createHypertext_lua(sol::optional<sol::table> params);
		Element* createImage_lua(sol::optional<sol::table> params);
		Element* createLabel_lua(sol::optional<sol::table> params);
		Element* createNif_lua(sol::optional<sol::table> params);
		Element* createParagraphInput_lua(sol::optional<sol::table> params);
		Element* createRect_lua(sol::optional<sol::table> params);
		Element* createSlider_lua(sol::optional<sol::table> params);
		Element* createSliderVertical_lua(sol::optional<sol::table> params);
		Element* createTextInput_lua(sol::optional<sol::table> params);
		Element* createTextSelect_lua(sol::optional<sol::table> params);
		Element* createThinBorder_lua(sol::optional<sol::table> params);
		Element* createVerticalScrollPane_lua(sol::optional<sol::table> params);

		//
		// Patch methods
		//

		void patchUpdateLayout_propagateFlow();

		//
		// Static data
		//

		static constexpr int INT_FIELD_INACTIVE = -1;
		static constexpr float FLOAT_FIELD_INACTIVE = -1.0f;
		static constexpr float IGNORE_ABSPOSALIGN_AND_LAYOUT = -2.0f; // Any negative value that is not -1.0f will result in the same behaviour.
	};
	static_assert(sizeof(Element) == 0x184, "TES3::UI::Element failed size validation");
}
