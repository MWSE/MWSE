--[[
	Mod configuration system.

	Part of the MWSE Project. This module is responsible for creating a uniform UI that mods can
	extend to provide a single place for users to configure their mods.
]]

--- Stores information about an entry in the MCM menu. 
--- This corresponds to either an `mwseMCMTemplate` or a legacy mod package.
--- @class mwseModConfig
--- @field name string Name of the mod.
--- @field hidden boolean Hide the mod from the MCM?
--- @field ["package"] mwse.registerModConfig.package? A legacy mod package. This is mutually exclusive with `template`.
--- @field template mwseMCMTemplate? The template for this mod. This is mutually exclusive with `package`.
--- @field lastSelectedPageIndex integer? If this mod is a template, stores information about which page was previously accessed.


--- @type table<string, mwseModConfig>
local configMods = {}


-- Stores information about the currently active mod config
---@type mwseModConfig?
local currentModConfig


--- Name of the last mod selected in the MCM.
--- Used to reopen the most recently closed mod config menu when the MCM is reopened during a play session.
--- Stored separately from `currentModConfig` for stability reasons.
--- @type string
local lastModName = nil

--- The previously selected element.
--- @type tes3uiElement?
local previousModConfigSelector = nil

--- Reusable access to UI elements.
--- @type tes3uiElement?
local modConfigContainer = nil

--- Store the config used by the MCM itself.
---@type {favorites: {[string]: boolean}}
local config = mwse.loadConfig("MWSE.MCM", {
	favorites = {},
})


-- Try to migrate over existing favorites.
if (table.empty(config.favorites) and lfs.fileexists("config\\core\\MCM Favorite Mods.json")) then
	-- Migrate over the contents of the old file, and overwrite.
	config.favorites = json.loadfile("config\\core\\MCM Favorite Mods")
	-- Delete old file (and directory if it is empty).
	os.remove("config\\core\\MCM Favorite Mods.json")
	lfs.rmdir("config\\core", false)
end



-- Convert array-style favorites list to the newer dictionary format.
if (#config.favorites > 0) then
	local newFavorites = {}
	for _, favorite in ipairs(config.favorites) do
		newFavorites[favorite] = true
	end
	config.favorites = newFavorites
end

-- Expose the mcm API.
mwse.mcm = require("mcm.mcm")
mwse.mcm.i18n = mwse.loadTranslations("..")

-- credit to Pherim and MelchiorDahrk for the default icons
local favoriteIcons = {
	id = "UnFavoriteButton",
	idle = "textures/mwse/menu_modconfig_favorite.tga",
	over = "textures/mwse/menu_modconfig_favorite_unset.tga",
	pressed = "textures/mwse/menu_modconfig_favorite_unset.tga",
}

local nonFavoriteIcons = {
	id = "FavoriteButton",
	idle = "textures/mwse/menu_modconfig_favorite.tga",
	over = "textures/mwse/menu_modconfig_favorite_set.tga",
	pressed = "textures/mwse/menu_modconfig_favorite_set.tga",
}

--- Checks to see if a mod is favorited.
--- @param mod string The name of the mod to check the state of.
--- @return boolean isFavorite If true, the mod will be favorited.
local function isFavorite(mod)
	return config.favorites[mod] == true
end

--- Sets a mod favorite status.
--- @param mod string The name of the mod to set the state of.
--- @param favorited boolean
local function setFavorite(mod, favorited)
	if (favorited) then
		config.favorites[mod] = true
	else
		config.favorites[mod] = nil
	end
end

--- Toggles the favorited state of a mod.
--- @param mod string The name of the mod to toggle favoriting for.
local function toggleFavorited(mod)
	setFavorite(mod, not isFavorite(mod))
end

--- @param a mwseModConfig
--- @param b mwseModConfig
--- @return boolean -- true if `a < b`
local function sortPackages(a, b)
	-- check if `a` and `b` have different "favorite" statuses
	-- `not a.favorite ~= not b.favorite` handles the case when `a.favorite == nil` and `b.favorite == false`
	if not isFavorite(a.name) ~= not isFavorite(b.name) then
		-- `true` if `a` is favorited and `b` isn't (so `a < b`)
		-- `false` if `b` is favorited and `a` isn't (so `b < a`)
		return isFavorite(a.name)
	end
	return a.name:lower() < b.name:lower()
end

-- Update the image icons for the various states of the favorite button.
---@param imageButton tes3uiElement
---@param favorite boolean Is the mod favorited?
local function updateFavoriteImageButton(imageButton, favorite)
	local iconTable = favorite and favoriteIcons or nonFavoriteIcons
	imageButton.children[1].contentPath = iconTable.idle
	imageButton.children[2].contentPath = iconTable.over
	imageButton.children[3].contentPath = iconTable.pressed
end

local function saveConfig()
	mwse.saveConfig("MWSE.MCM", config)
end

--- Closes the currently opened mod config menu, if it exists.
--- This will also update the `lastPageIndex` if a mod template is currently selected.
--- This is called when the user closes the MCM, or when the user clicks on a different mod name.
local function closeCurrentModConfig()

	if not currentModConfig then
		return
	end

	local onClose

	local template = currentModConfig.template
	if template then
		currentModConfig.lastSelectedPageIndex = table.find(template.pages, template.currentPage)
		onClose = template.onClose
	else
		onClose = currentModConfig.package and currentModConfig.package.onClose
	end

	if not onClose then 
		return 
	end

	local status, error = pcall(onClose, modConfigContainer)
	if (status == false) then
		mwse.log("Error in mod config create callback: %s\n%s", error, debug.traceback())
	end
end
 

--- Callback for when a mod name has been clicked in the left pane.
--- @param e tes3uiEventData The event data that triggers when a mod name is clicked, so that `e.source`
--- corresponds to a button on the left pane that stores a `modName`.
local function onClickModName(e)
	local modName = e.source.text

	-- If we have a current mod, fire its close event.
	closeCurrentModConfig()
	currentModConfig = configMods[modName]


	if (previousModConfigSelector) then
		previousModConfigSelector.widget.state = tes3.uiState.normal
	end
	e.source.widget.state = tes3.uiState.active
	previousModConfigSelector = e.source

	-- Destroy and recreate the parent container.
	modConfigContainer:destroyChildren()
	
	--- The result of creating the template/legacy mod package, and an error message if there was a problem.
	---@type boolean, string?
	local status, errorMsg
	

	local modConfig = configMods[modName]
	local template = modConfig.template
	if template then
		-- Call `create` as a method.
		status, errorMsg = pcall(template.create, template, modConfigContainer)
	elseif modConfig.package then
		status, errorMsg = pcall(modConfig.package.onCreate, modConfigContainer)
	else
		-- We couldn't find the mod config.
		-- I'm not even sure how this could ever happen.
		error(string.format("No mod config could be found for '%s'.", modName))
		return
	end

	-- Fire the mod's creation event if it has one.
	if (status == false) then
		mwse.log("Error in mod config create callback: %s\n%s", errorMsg, debug.traceback())
	end

	-- Change the mod config title bar to include the mod's name.
	local menu = tes3ui.findMenu("MWSE:ModConfigMenu") --[[@as tes3uiElement]]
	menu.text = mwse.mcm.i18n("Mod Configuration - %s", { modName })
	menu:updateLayout()
	-- Record that this was the most recently opened mod config menu.
	lastModName = modName
end


local keyBinderPopupId = tes3ui.registerID("KeyMouseBinderPopup")

--- Callback for when the close button has been clicked.
--- @param e keyDownEventData|tes3uiEventData
local function onClickCloseButton(e)
	-- Disallow closing MCM menu while KeyBinder popup is active
	local keyBinderPopup = tes3ui.findMenu(keyBinderPopupId)
	if keyBinderPopup then
		return
	end

	event.unregister("keyDown", onClickCloseButton, { filter = tes3.scanCode.escape })

	-- save the list of favorites
	saveConfig()
	-- If we have a current mod, fire its close event.
	closeCurrentModConfig()

	-- Destroy the mod config menu.
	local modConfigMenu = tes3ui.findMenu("MWSE:ModConfigMenu")
	if (modConfigMenu) then
		modConfigMenu:destroy()
	end

	-- Get the main menu so we can show it again.
	local mainMenu = tes3ui.findMenu(tes3ui.registerID("MenuOptions"))
	if (mainMenu) then
		-- Show the main menu again.
		mainMenu.visible = true
	end
end

--- Callback for when the favorite button has been clicked.
--- @param e tes3uiEventData
local function onClickFavoriteButton(e)
	-- `source` is the button, which is right of the mod name, so we need to up and then down-left
	local modName = e.source.parent.children[1].text
	toggleFavorited(modName)
	updateFavoriteImageButton(e.source, isFavorite(modName))

	local menu = tes3ui.findMenu("MWSE:ModConfigMenu")
	if not menu then return end
	local modList = menu:findChild("ModList")
	local modListContents = modList and modList:getContentElement()

	if not modListContents then
		mwse.log("error! modListContents not found.")
		return
	end

	-- Favorites have been updated, resort the list of mods to take the new configuration into account.
	-- Sorting is done by comparing the `modName`s in each `entryBlock`.
	modListContents:sortChildren(function(a, b)
		return sortPackages(configMods[a.children[1].text], configMods[b.children[1].text])
	end)

	modList:getTopLevelMenu():updateLayout()
end

--- @param e tes3uiEventData
local function focusSearchBar(e)
	local searchBar = e.source:findChild("SearchBar")
	if (not searchBar) then return end

	tes3ui.acquireTextInput(searchBar)
end

--- @param modName string
--- @param searchText string
local function filterModByName(modName, searchText)
	-- Perform a basic search.
	local nameMatch = modName:lower():find(searchText, nil, true)
	if (nameMatch ~= nil) then
		return true
	end

	local modConfig = configMods[modName]

	-- If the mod has a template, use the template's search logic.
	if modConfig.template then
		return modConfig.template:onSearchInternal(searchText)
	elseif modConfig.package and modConfig.package.onSearch then
		return modConfig.package.onSearch(searchText)
	end

	return false
end

--- @param e tes3uiEventData
local function onSearchUpdated(e)
	local lowerSearchText = e.source.text:lower()
	local mcm = e.source:getTopLevelMenu()
	local modList = mcm:findChild("ModList")
	local modListContents = modList:getContentElement()
	for _, child in ipairs(modListContents.children) do
		child.visible = filterModByName(child.children[1].text, lowerSearchText)
	end
	mcm:updateLayout()
	modList.widget:contentsChanged()
end

--- @param e tes3uiEventData
local function onSearchCleared(e)
	local mcm = e.source:getTopLevelMenu()
	local modList = mcm:findChild("ModList")
	local modListContents = modList:getContentElement()
	for _, child in ipairs(modListContents.children) do
		child.visible = true
	end
	mcm:updateLayout()
	modList.widget:contentsChanged()
end


local function cleanupMCM(e)
	currentModConfig = nil
	modConfigContainer = nil
	previousModConfigSelector = nil
end

-- Callback for when the mod config button has been clicked.
-- Here, we'll create the GUI and set up everything.
local function onClickModConfigButton()
	-- Play the click sound.
	tes3.worldController.menuClickSound:play()

	local menu = tes3ui.findMenu("MWSE:ModConfigMenu")
	if (not menu) then
		-- Create the main menu frame.
		menu = tes3ui.createMenu({ id = "MWSE:ModConfigMenu", dragFrame = true })
		menu.text = mwse.mcm.i18n("Mod Configuration")
		menu.minWidth = 600
		menu.minHeight = 500
		menu.width = 1200
		menu.height = 800
		menu.positionX = menu.width / -2
		menu.positionY = menu.height / 2
		menu:registerAfter("destroy", cleanupMCM)

		-- Register and block unfocus event, to prevent players
		-- messing up state by opening their inventory.
		menu:register("unfocus", function(e)
			return false
		end)

		-- Create the left-right flow.
		local mainHorizontalBlock = menu:createBlock({ id = "MainFlow" })
		mainHorizontalBlock.flowDirection = "left_to_right"
		mainHorizontalBlock.widthProportional = 1.0
		mainHorizontalBlock.heightProportional = 1.0

		local leftBlock = mainHorizontalBlock:createBlock({ id = "LeftFlow" })
		leftBlock.flowDirection = "top_to_bottom"
		leftBlock.width = 250
		leftBlock.minWidth = 250
		leftBlock.maxWidth = 250
		leftBlock.widthProportional = -1.0
		leftBlock.heightProportional = 1.0

		local searchBlock = leftBlock:createThinBorder({ id = "SearchBlock" })
		searchBlock.widthProportional = 1.0
		searchBlock.autoHeight = true

		local searchBar = searchBlock:createTextInput({
			id = "SearchBar",
			placeholderText = mwse.mcm.i18n("Search..."),
			autoFocus = true,
		})
		searchBar.borderLeft = 5
		searchBar.borderRight = 5
		searchBar.borderTop = 3
		searchBar.borderBottom = 5
		searchBar:registerAfter("textUpdated", onSearchUpdated)
		searchBar:registerAfter("textCleared", onSearchCleared)

		-- Make clicking on the block focus the search input.
		searchBlock:register("mouseClick", focusSearchBar)

		-- Create the mod list.
		local modList = leftBlock:createVerticalScrollPane({ id = "ModList" })
		modList.widthProportional = 1.0
		modList.heightProportional = 1.0
		modList:setPropertyBool("PartScrollPane_hide_if_unneeded", true)

		---@type string[]
		local sortedModNames = {}
		for modName, modConfig in pairs(configMods) do
			if not modConfig.hidden then
				table.insert(sortedModNames, modName)
			end
		end
		table.sort(sortedModNames, function (a, b)
			return sortPackages(configMods[a], configMods[b])
		end)

		-- Fill in the mod list UI.
		local modListContents = modList:getContentElement()

		for _, modName in ipairs(sortedModNames) do
			local entryBlock = modListContents:createBlock{id = "ModEntryBlock"}
			entryBlock.flowDirection = tes3.flowDirection.leftToRight
			entryBlock.autoHeight = true
			entryBlock.autoWidth = true
			entryBlock.widthProportional = 1.0
			entryBlock.childAlignY = 0.5

			local modNameButton = entryBlock:createTextSelect({ id = "ModEntry", text = modName })
			modNameButton:register("mouseClick", onClickModName)
			modNameButton.wrapText = true
			modNameButton.widthProportional = 0.95
			modNameButton.borderRight = 16
			modNameButton.heightProportional = 1

			-- Icons will be updated by `updateFavoriteImageButton`.
			local favorite = isFavorite(modName)
			local imageButton = entryBlock:createImageButton(nonFavoriteIcons)
			updateFavoriteImageButton(imageButton,favorite)
			imageButton.childAlignY = 0.5
			imageButton.absolutePosAlignX = .97
			imageButton.absolutePosAlignY = 0.5
			imageButton.consumeMouseEvents = true
			imageButton.visible = favorite

			imageButton:register(tes3.uiEvent.mouseClick, onClickFavoriteButton)
			---@param image tes3uiElement
			for _, image in ipairs(imageButton.children) do
				image.scaleMode = true
				image.height = 16
				image.width = 16
				image.paddingTop = 3
			end

			local onHover = function() imageButton.visible = true end
			local onLeave = function() imageButton.visible = isFavorite(modName) end
			entryBlock:registerAfter(tes3.uiEvent.mouseOver, onHover)
			entryBlock:registerAfter(tes3.uiEvent.mouseLeave, onLeave)
			modNameButton:registerAfter(tes3.uiEvent.mouseOver, onHover)
			modNameButton:registerAfter(tes3.uiEvent.mouseLeave, onLeave)
			imageButton:registerAfter(tes3.uiEvent.mouseOver, onHover)
			imageButton:registerAfter(tes3.uiEvent.mouseLeave, onLeave)
		end

		-- Create container for mod content. This will be deleted whenever the pane is reloaded.
		modConfigContainer = mainHorizontalBlock:createBlock({ id = "ModContainer" })
		modConfigContainer.flowDirection = "top_to_bottom"
		modConfigContainer.widthProportional = 1.0
		modConfigContainer.heightProportional = 1.0
		modConfigContainer.paddingLeft = 4

		local containerPane = modConfigContainer:createThinBorder({ id = "ContainerPane" })
		containerPane.widthProportional = 1.0
		containerPane.heightProportional = 1.0
		containerPane.paddingAllSides = 12
		containerPane.flowDirection = "top_to_bottom"

		-- Splash screen.
		local splash = containerPane:createImage({ id = "MWSESplash", path = "textures/mwse/menu_modconfig_splash.tga" })
		splash.absolutePosAlignX = 0.5
		splash.borderTop = 25

		-- Create a link back to the website.
		local site = containerPane:createHyperlink({ id = "MWSELink", text = "mwse.github.io/MWSE", url = "https://mwse.github.io/MWSE" })
		site.absolutePosAlignX = 0.5

		-- Create bottom button block.
		local bottomBlock = menu:createBlock({ id = "BottomFlow" })
		bottomBlock.widthProportional = 1.0
		bottomBlock.autoHeight = true
		bottomBlock.childAlignX = 1.0

		-- Add a close button to the bottom block.
		local closeButton = bottomBlock:createButton({
			id = "MWSE:ModConfigMenu_Close",
			text = tes3.findGMST(tes3.gmst.sClose).value --[[@as string]]
		})
		closeButton:register("mouseClick", onClickCloseButton)
		event.register("keyDown", onClickCloseButton, { filter = tes3.scanCode.escape })

		-- Cause the menu to refresh itself.
		menu:updateLayout()
		modList.widget:contentsChanged()
		-- Mods with a certain title length can add an unnecessary newline, which goes away when the layout is refreshed.
		menu:updateLayout()

		-- Reopen the most recently viewed config menu (if there was one).
		-- If the most recently viewed mod config was a template, we should also reopen it to the previously viewed tab.
		if lastModName ~= nil then
			for _, child in ipairs(modListContents.children) do
				local modNameButton = child.children[1]
	
				if modNameButton.text == lastModName then
					modNameButton:triggerEvent(tes3.uiEvent.mouseClick)
					
					-- Open the previously selected page if possible.
					local modConfig = configMods[modNameButton.text]
					
					local template = modConfig.template
					local lastPageIndex = modConfig.lastSelectedPageIndex

					-- Try to open the last page by clicking the appropriate tab.
					-- The `lastPageIndex > 1` check is to make sure we don't end up clicking the first tab twice.
					if template and lastPageIndex and lastPageIndex > 1 then
						template:clickTab(template.pages[lastPageIndex])
					end

					-- We found the mod, so stop iterating.
					break 
				end
			end
		end
	else
		menu.visible = true
	end

	-- Hide main menu.
	local mainMenu = tes3ui.findMenu(tes3ui.registerID("MenuOptions"))
	if (mainMenu) then
		mainMenu.visible = false
	else
		mwse.log("Couldn't find main menu!")
	end

	tes3ui.enterMenuMode(menu.id)
end


--- Callback for when the MenuOptions element is created. We'll extend it with our new button.
--- @param e uiActivatedEventData
local function onCreatedMenuOptions(e)
	-- Only interested in menu creation, not updates
	if (not e.newlyCreated) then
		return
	end

	local mainMenu = e.element

	local creditsButton = mainMenu:findChild(tes3ui.registerID("MenuOptions_Credits_container"))
	local buttonContainer = creditsButton.parent

	local button = buttonContainer:createImageButton({
		id = tes3ui.registerID("MenuOptions_MCM_container"),
		idleId = tes3ui.registerID("MenuOptions_MCMidlebutton"),
		idle = "textures/mwse/menu_modconfig.dds",
		overId = tes3ui.registerID("MenuOptions_MCMoverbutton"),
		over = "textures/mwse/menu_modconfig_over.dds",
		pressedId = tes3ui.registerID("MenuOptions_MCMpressedbutton"),
		pressed = "textures/mwse/menu_modconfig_pressed.dds",
	})
	button.height = 50
	button.autoHeight = false
	button:register("mouseClick", onClickModConfigButton)

	buttonContainer:reorderChildren(creditsButton, button, 1)

	mainMenu.autoWidth = true
	mainMenu.autoHeight = true

	mainMenu:updateLayout()
end
event.register("uiActivated", onCreatedMenuOptions, { filter = "MenuOptions" })



--- Define a new function in the mwse namespace that lets mods register for mod config.
--- @deprecated
--- @param name string
--- @param package mwse.registerModConfig.package|mwseMCMTemplate
function mwse.registerModConfig(name, package)

	if (configMods[name] ~= nil) then
		error(string.format('mwse.registerModConfig: A mod with the name "%s" has already been registered!', name))
	end
	-- Check if it's a `mwseMCMTemplate` and register it accordingly.
	if package.componentType == "Template" and package.class == "Template" then
		-- Actually register the package.
		configMods[name] = {name = name, template = package, hidden = false}
		mwse.log("[MCM] Registered mod config: %s", name)
	else
		configMods[name] = {name = name, package = package, hidden = false}
		mwse.log("[MCM] Registered legacy mod config: %s", name)
	end
end

--- When we've initialized, set up our UI IDs and let other mods know that we are ready to boogie.
---
--- Set this up to run before most other initialized callbacks.
local function onInitialized()
	event.trigger("modConfigReady")
end
event.register("initialized", onInitialized, { priority = 100 })
