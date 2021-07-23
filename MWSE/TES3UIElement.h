#pragma once

#include "TES3Defines.h"

#include "TES3UIString.h"
#include "TES3UITree.h"
#include "TES3UIVector.h"

#include "NINode.h"
#include "NISourceTexture.h"

namespace TES3 {
	namespace UI {
		struct Element {
			char tag;
			String name;
			short id;
			Vector<Element*> vectorChildren;
			Element* parent;
			Tree properties;
			Element* uielement_3C;
			Vector<void> vectorVerts_40;
			Vector<void> vector_50;
			Vector<void> vector_60;
			int updateReqTimestamp;
			bool flagExtendImageToBounds;
			bool visible;
			bool visibleAtLastUpdate;
			bool flagUsesRGBA;
			bool flagPosChanged;
			bool flagSizeChanged;
			bool flagVisibilityChanged;
			bool flagClippingChanged;
			bool flagContentChanged;
			char unknown_0x7D;
			char unknown_0x7E;
			bool flagConsumeMouseEvents;
			char unknown_0x80;
			char unknown_0x81;
			char unknown_0x82;
			char unknown_0x83;
			int unknown_0x84;
			NI::Pointer<NI::Node> sceneNode;
			void* extraData;
			NI::Pointer<NI::SourceTexture> texture;
			int cached_offsetX, cached_offsetY;
			int nodeMinX, nodeMaxX;
			int nodeMinY, nodeMaxY;
			int cached_unknown_x, cached_unknown_y;
			int borderAllSides;
			int borderLeft, borderRight, borderBottom, borderTop;
			int paddingAllSides;
			int paddingLeft, paddingRight, paddingBottom, paddingTop;
			int positionX, positionY;
			int cached_screenX, cached_screenY;
			int width, height;
			int childOffsetX, childOffsetY;
			int minWidth, minHeight;
			int maxWidth, maxHeight;
			int inheritedWidth, inheritedHeight;
			bool flagAutoWidth, flagAutoHeight;
			float widthProportional, heightProportional;
			float absolutePosAlignX, absolutePosAlignY;
			int clipMinX, clipMaxX;
			int clipMinY, clipMaxY;
			Property scale_mode;
			int nodeOffsetX, nodeOffsetY;
			float imageScaleX, imageScaleY;
			float colourRed, colourGreen, colourBlue, colourAlpha;
			int font;
			Property contentType;
			String rawText;
			String contentPath;

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
			const char* getText() const;

			void setProperty(Property prop, PropertyValue value, PropertyType type);
			void setProperty(Property prop, int value);
			void setProperty(Property prop, float value);
			void setProperty(Property prop, void* value);
			void setProperty(Property prop, Property value);
			void setProperty(Property prop, EventCallback value);
			void setProperty(Property prop, PropertyAccessCallback value);
			void setText(const char *);
			void setIcon(const char *);
			void setIcon(String);

			//
			// Other related this-call functions.
			//

			void updateSceneGraph();

			//
			// Custom functions.
			//

			const char* getName() const;

			std::string toJson() const;

			// TODO: Add std collection access to the container so this doesn't need to be copied to a table.
			sol::table getChildren_lua(sol::this_state ts) const;

			// TODO: Add std collection access to the container so this doesn't need to be copied to a table.
			sol::table getProperties_lua(sol::this_state ts) const;

			std::string getContentTypeString() const;

			sol::object makeWidget(sol::this_state ts);
			std::string getWidgetText() const;
			void setWidgetText(const char* text);
			void setWidgetText_lua(sol::object text, sol::this_state ts);

			sol::optional<float> getAbsolutePosAlignX_lua() const;
			void setAbsolutePosAlignX_lua(sol::optional<float> value = -1.0f);
			sol::optional<float> getAbsolutePosAlignY_lua() const;
			void setAbsolutePosAlignY_lua(sol::optional<float> value = -1.0f);
			float getAlpha() const;
			void setAlpha(float value);
			int getBorderAllSides() const;
			void setBorderAllSides_lua(sol::optional<int> value = 0);
			sol::optional<int> getBorderBottom_lua() const;
			void setBorderBottom_lua(sol::optional<int> value = -1);
			sol::optional<int> getBorderLeft_lua() const;
			void setBorderLeft_lua(sol::optional<int> value = -1);
			sol::optional<int> getBorderRight_lua() const;
			void setBorderRight_lua(sol::optional<int> value = -1);
			sol::optional<int> getBorderTop_lua() const;
			void setBorderTop_lua(sol::optional<int> value = -1);
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
			void setContentPath_lua(sol::optional<const char*> value);
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
			void setPaddingBottom_lua(sol::optional<int> value = -1);
			sol::optional<int> getPaddingLeft_lua() const;
			void setPaddingLeft_lua(sol::optional<int> value = -1);
			sol::optional<int> getPaddingRight_lua() const;
			void setPaddingRight_lua(sol::optional<int> value = -1);
			sol::optional<int> getPaddingTop_lua() const;
			void setPaddingTop_lua(sol::optional<int> value = -1);
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
			void setWidthProportional_lua(sol::optional<float> value = -1.0f);
			bool getWrapText() const;
			void setWrapText(bool value);

			bool hasProperty_lua(sol::object key) const;
			PropertyType getPropertyType_lua(sol::object key) const;
			bool getPropertyBool_lua(sol::object key) const;
			void setPropertyBool_lua(sol::object key, bool value);
			int getPropertyInt_lua(sol::object key) const;
			void setPropertyInt_lua(sol::object key, int value);
			float getPropertyFloat_lua(sol::object key) const;
			void setPropertyFloat_lua(sol::object key, float value);
			sol::object getPropertyObject_lua(sol::this_state ts, sol::object key, sol::optional<std::string> typeCast) const;
			void setPropertyObject_lua(sol::object key, sol::object value);

			void registerBefore_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority);
			void registerAfter_lua(const std::string& eventID, sol::protected_function callback, sol::optional<double> priority);
			void register_lua(const std::string& eventID, sol::object callback);
			void unregisterBefore_lua(const std::string& eventID, sol::protected_function callback);
			void unregisterAfter_lua(const std::string& eventID, sol::protected_function callback);
			void unregister_lua(const std::string& eventID);
			void forwardEvent_lua(sol::table eventData) const;
			void triggerEvent_lua(sol::object params);

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

		};
		static_assert(sizeof(Element) == 0x184, "TES3::UI::Element failed size validation");
	}
}
