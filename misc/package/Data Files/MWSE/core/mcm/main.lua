--[[
	Mod configuration system.

	Part of the MWSE Project. This module is responsible for creating a uniform UI that mods can
	extend to provide a single place for users to configure their mods.
]]

--- Storage for mod config packages.
--- @type table<string, mwseModConfig>
local configMods = {}

--- The current package that we are configuring.
--- Used to properly deselect mod config menus when clicking on different mod names.
--- @type mwseModConfig?
local currentModConfig = nil

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

--- @type table
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

--- sort the given packages
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

-- update the image icons for the various states of the favorite button
---@param imageButton tes3uiElement
local function updateFavoriteImageButton(imageButton, favorite)
	local iconTable = favorite and favoriteIcons or nonFavoriteIcons
	imageButton.children[1].contentPath = iconTable.idle
	imageButton.children[2].contentPath = iconTable.over
	imageButton.children[3].contentPath = iconTable.pressed
end

local function saveConfig()
	mwse.saveConfig("MWSE.MCM", config)
end

-- Closes the currently opened mod config (if it exists)
-- called when the MCM itself is closed, or when a different mod is selected
local function closeCurrentModConfig()
	if not currentModConfig then return end
	
	if currentModConfig.onClose then
		local status, error = pcall(currentModConfig.onClose, modConfigContainer)
		if (status == false) then
			mwse.log("Error in mod config close callback: %s\n%s", error, debug.traceback())
		end
	end
	-- Fire the event after `onClose` gets called.
	local payload = {
		modName = currentModConfig.name, 
		isFavorite = isFavorite(currentModConfig.name),
	}
	event.trigger(tes3.event.modConfigEntryClosed, payload, {filter = currentModConfig.name})
end

--- Callback for when a mod name has been clicked in the left pane.
--- @param e tes3uiEventData
local function onClickModName(e)
	-- If we have a current mod, fire its close event.
	closeCurrentModConfig()
	
	local modName = e.source.text

	-- Update the current mod package.
	currentModConfig = configMods[modName]
	if (not currentModConfig) then
		error(string.format("No mod config could be found for key '%s'.", modName))
		return
	end

	if (previousModConfigSelector) then
		previousModConfigSelector.widget.state = tes3.uiState.normal
	end
	e.source.widget.state = tes3.uiState.active
	previousModConfigSelector = e.source

	-- Destroy and recreate the parent container.
	modConfigContainer:destroyChildren()

	-- Fire the mod's creation event if it has one.
	if (currentModConfig.onCreate) then
		local status, error = pcall(currentModConfig.onCreate, modConfigContainer)
		if (status == false) then
			mwse.log("Error in mod config create callback: %s\n%s", error, debug.traceback())
		end
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
	local package = configMods[e.source.parent.children[1].text]
	toggleFavorited(package.name)
	updateFavoriteImageButton(e.source, isFavorite(package.name))

	local menu = tes3ui.findMenu("MWSE:ModConfigMenu")
	if not menu then return end
	local modList = menu:findChild("ModList")
	local modListContents = modList and modList:getContentElement()

	if not modListContents then
		mwse.log("error! modListContents not found.")
		return
	end

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

	-- Get the mod package.
	local package = configMods[modName]

	-- Do we have a custom filter package?
	if (package.onSearch and package.onSearch(searchText)) then
		return true
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

		local configModsList = {} --- @type mwseModConfig[]
		for _, package in pairs(configMods) do
			if not package.hidden then
				table.insert(configModsList, package)
			end
		end

		table.sort(configModsList, sortPackages)

		-- Fill in the mod list.
		local modListContents = modList:getContentElement()

		for _, package in ipairs(configModsList) do
			local entryBlock = modListContents:createBlock{id = "ModEntryBlock"}
			entryBlock.flowDirection = tes3.flowDirection.leftToRight
			entryBlock.autoHeight = true
			entryBlock.autoWidth = true
			entryBlock.widthProportional = 1.0
			entryBlock.childAlignY = 0.5

			local modNameButton = entryBlock:createTextSelect({ id = "ModEntry", text = package.name })
			modNameButton:register("mouseClick", onClickModName)
			modNameButton.wrapText = true
			modNameButton.widthProportional = 0.95
			modNameButton.borderRight = 16
			modNameButton.heightProportional = 1

			local iconTable = isFavorite(package.name) and favoriteIcons or nonFavoriteIcons

			local imageButton = entryBlock:createImageButton(iconTable)
			updateFavoriteImageButton(imageButton, isFavorite(package.name))
			imageButton.childAlignY = 0.5
			imageButton.absolutePosAlignX = 0.97
			imageButton.absolutePosAlignY = 0.5
			imageButton.consumeMouseEvents = true
			imageButton.visible = isFavorite(package.name)

			imageButton:register(tes3.uiEvent.mouseClick, onClickFavoriteButton)
			---@param image tes3uiElement
			for _, image in ipairs(imageButton.children) do
				image.scaleMode = true
				image.height = 16
				image.width = 16
				image.paddingTop = 3
			end

			local onHover = function() imageButton.visible = true end
			local onLeave = function() imageButton.visible = isFavorite(package.name) end
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

		if lastModName ~= nil then
			for _, child in ipairs(modListContents.children) do
				if child.children[1].text == lastModName then
					child.children[1]:triggerEvent(tes3.uiEvent.mouseClick)
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

-- Get the number of mods that aren't hidden.
local function getActiveModConfigCount()
	local count = 0
	for _, package in pairs(configMods) do
		-- Allow package.hidden to be set to prevent it from showing up in the list.
		if (not package.hidden) then
			count = count + 1
		end
	end
	return count
end

--- Callback for when the MenuOptions element is created. We'll extend it with our new button.
--- @param e uiActivatedEventData
local function onCreatedMenuOptions(e)
	-- Only interested in menu creation, not updates
	if (not e.newlyCreated) then
		return
	end

	-- Don't show the UI if we don't have any mod configs to show.
	if (getActiveModConfigCount() == 0) then
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

--- @class mwseModConfig : mwse.registerModConfig.package
--- @field name string
--- @field hidden boolean hide it?
--- @field favorite boolean is this mod a favorite

--- Define a new function in the mwse namespace that lets mods register for mod config.
--- @param name string
--- @param package mwse.registerModConfig.package
function mwse.registerModConfig(name, package)
	-- Prevent duplicate registration.
	if (configMods[name] ~= nil) then
		error(string.format("mwse.registerModConfig: A mod with the name %s has already been registered!", name))
	end

	package = table.copy(package)
	--- @cast package mwseModConfig

	-- Add the package to the list.
	package.name = name
	configMods[name] = package
	mwse.log("%s mod config registered", name)
end

--- When we've initialized, set up our UI IDs and let other mods know that we are ready to boogie.
---
--- Set this up to run before most other initialized callbacks.
local function onInitialized()
	event.trigger("modConfigReady")
end
event.register("initialized", onInitialized, { priority = 100 })
