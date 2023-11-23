-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- @meta
--- A Filter Page is a Sidebar Page with additional functionality: The components container is a vertical scroll pane with a search bar. This is especially useful if you have a large or unknown number of settings.
--- @class mwseMCMFilterPage : mwseMCMSideBarPage, mwseMCMPage, mwseMCMCategory, mwseMCMComponent
--- @field elements mwseMCMFilterPageElements This dictionary-style table holds all the UI elements of the Filter Page, for easy access.
--- @field placeholderSearchText string The default is a localised version of "Search...".
mwseMCMFilterPage = {}

--- Creates a label UI element with `self.description` text and adds it to `self.elements.description`.
--- @param parentBlock tes3uiElement No description yet available.
function mwseMCMFilterPage:createDescription(parentBlock) end

--- Creates a thin border UI element as the left column of the Filter Page and stores it in `self.elements.outerContainer`.
--- @param parentBlock tes3uiElement No description yet available.
function mwseMCMFilterPage:createLeftColumn(parentBlock) end

--- Creates the search bar for Filter Page.
--- @param parentBlock tes3uiElement No description yet available.
function mwseMCMFilterPage:createSearchBar(parentBlock) end

--- Creates Filter Page's scrollPane UI element inside given `parentBlock`, and stores it in the `self.elements.scrollPane`.
--- 
--- Then creates a block UI element as the child of scrollPane and stores it in `self.elements.subcomponentsContainer`. This is the parent UI element for all the UI elements of MCM Components that are in this Filter Page.
--- @param parentBlock tes3uiElement No description yet available.
function mwseMCMFilterPage:createSubcomponentsContainer(parentBlock) end

--- This method will hide all the UI elements of components in `self.components` if their label doesn't contain the text currently inputted in the search bar.
function mwseMCMFilterPage:filterComponents() end

--- Creates a new Filter Page.
--- @param data mwseMCMFilterPage.new.data? This table accepts the following values:
--- 
--- `showHeader`: boolean? — *Default*: `false`. The page's label will only be created if set to true.
--- 
--- `label`: string? — *Optional*. The label field is displayed in the tab for that page at the top of the menu. Defaults to: "Page {number}".
--- 
--- `noScroll`: boolean? — *Default*: `false`. When set to true, the page will not have a scrollbar. Particularly useful if you want to use a [ParagraphField](./mwseMCMParagraphField.md), which is not compatible with scroll panes.
--- 
--- `description`: string? — *Optional*. Default sidebar text shown when the mouse isn't hovering over a component inside this Sidebar Page. It will be added to right column as a mwseMCMInfo.
--- 
--- `placeholderSearchText`: string? — *Default*: `Search...`. The text shown in the search bar when no text is entered.
--- 
--- `components`: mwseMCMComponent.getComponent.componentData[]? — *Optional*. Use this if you want to directly create all the nested components in this Page. This table is described at [getComponent](./mwseMCMFilterPage.md#getcomponent).
--- 
--- `indent`: integer? — *Default*: `6`. The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component.
--- 
--- `childIndent`: integer? — *Optional*. The left padding size in pixels. Used on all the child components.
--- 
--- `paddingBottom`: integer? — *Default*: `4`. The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component.
--- 
--- `childSpacing`: integer? — *Optional*. The bottom border size in pixels. Used on all the child components.
--- 
--- `inGameOnly`: boolean? — *Default*: `false`. No description yet available.
--- 
--- `postCreate`: nil|fun(self: mwseMCMFilterPage) — *Optional*. Can define a custom formatting function to make adjustments to any element saved in `self.elements`.
--- 
--- `class`: string? — *Optional*. No description yet available.
--- 
--- `componentType`: string? — *Optional*. No description yet available.
--- 
--- `parentComponent`: mwseMCMActiveInfo|mwseMCMButton|mwseMCMCategory|mwseMCMComponent|mwseMCMCycleButton|mwseMCMDecimalSlider|mwseMCMDropdown|mwseMCMExclusionsPage|mwseMCMFilterPage|mwseMCMHyperlink|mwseMCMInfo|mwseMCMKeyBinder|mwseMCMMouseOverInfo|mwseMCMMouseOverPage|mwseMCMOnOffButton|mwseMCMPage|mwseMCMParagraphField|mwseMCMSetting|mwseMCMSideBarPage|mwseMCMSideBySideBlock|mwseMCMSlider|mwseMCMTemplate|mwseMCMTextField|mwseMCMYesNoButton|nil — *Optional*. No description yet available.
--- @return mwseMCMFilterPage page No description yet available.
function mwseMCMFilterPage:new(data) end

---Table parameter definitions for `mwseMCMFilterPage.new`.
--- @class mwseMCMFilterPage.new.data
--- @field showHeader boolean? *Default*: `false`. The page's label will only be created if set to true.
--- @field label string? *Optional*. The label field is displayed in the tab for that page at the top of the menu. Defaults to: "Page {number}".
--- @field noScroll boolean? *Default*: `false`. When set to true, the page will not have a scrollbar. Particularly useful if you want to use a [ParagraphField](./mwseMCMParagraphField.md), which is not compatible with scroll panes.
--- @field description string? *Optional*. Default sidebar text shown when the mouse isn't hovering over a component inside this Sidebar Page. It will be added to right column as a mwseMCMInfo.
--- @field placeholderSearchText string? *Default*: `Search...`. The text shown in the search bar when no text is entered.
--- @field components mwseMCMComponent.getComponent.componentData[]? *Optional*. Use this if you want to directly create all the nested components in this Page. This table is described at [getComponent](./mwseMCMFilterPage.md#getcomponent).
--- @field indent integer? *Default*: `6`. The left padding size in pixels. Only used if the `childIndent` isn't set on the parent component.
--- @field childIndent integer? *Optional*. The left padding size in pixels. Used on all the child components.
--- @field paddingBottom integer? *Default*: `4`. The bottom border size in pixels. Only used if the `childSpacing` is unset on the parent component.
--- @field childSpacing integer? *Optional*. The bottom border size in pixels. Used on all the child components.
--- @field inGameOnly boolean? *Default*: `false`. No description yet available.
--- @field postCreate nil|fun(self: mwseMCMFilterPage) *Optional*. Can define a custom formatting function to make adjustments to any element saved in `self.elements`.
--- @field class string? *Optional*. No description yet available.
--- @field componentType string? *Optional*. No description yet available.
--- @field parentComponent mwseMCMActiveInfo|mwseMCMButton|mwseMCMCategory|mwseMCMComponent|mwseMCMCycleButton|mwseMCMDecimalSlider|mwseMCMDropdown|mwseMCMExclusionsPage|mwseMCMFilterPage|mwseMCMHyperlink|mwseMCMInfo|mwseMCMKeyBinder|mwseMCMMouseOverInfo|mwseMCMMouseOverPage|mwseMCMOnOffButton|mwseMCMPage|mwseMCMParagraphField|mwseMCMSetting|mwseMCMSideBarPage|mwseMCMSideBySideBlock|mwseMCMSlider|mwseMCMTemplate|mwseMCMTextField|mwseMCMYesNoButton|nil *Optional*. No description yet available.
