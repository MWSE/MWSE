--[[
	Mod configuration system.

	Part of the MWSE Project. This module is responsible for creating a uniform UI that mods can
	extend to provide a single place for users to configure their mods.
]]


--- @class mwseLegacyMod : mwse.registerModConfig.package
--- @field name string

--- Storage for mod config packages.

-- Stores all mod templates, indexed by display string
-- Most mods make their MCMs using `mwseMCMTemplate`, and storing the template allows for
-- direct access to the mods menu structure.
local modTemplates = {} ---@type table<string, mwseMCMTemplate>

-- This will store legacy config menus that were made without using `mwseMCMTemplate`.
-- These mods were registered using `mwse.registerModConfig(...)` instead of `template:register()`.
-- The lua code dump says this accounts for roughly 80 Lua mods in total (and a few of those hits are false positives).
local legacyMods = {} ---@type table<string, mwseLegacyMod>

-- Stores the name of the currently active mod. 
-- Could correspond to a `mwseMCMTemplate` or a legacy mod.
local currentModName = nil ---@type string?


-- Stores the index of the most recently looked at tab. 
-- This is used to reopen the menu to the last seen tab.
-- This will only work for mods made using a `mwseMCMTemplate`,
-- as there are no guarantees as to how the pages of legacy mods are organized.
-- As such, it's only updated when a `mwseMCMTemplate` mod is interacted with.
local lastPageIndex = nil ---@type integer?

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

local fmt = string.format

local config = mwse.loadConfig("MWSE.MCM", { favorites = {}, }) --[[@as {favorites: table<string, boolean>}]]

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
mwse.mcm.i18n = mwse.loadTranslations("mcm")

-- credit to Pherim for the default icons
local favoriteIcons = {
	idle = "textures/mwse/menu_modconfig_favorite_idle.dds",
	over = "textures/mwse/menu_modconfig_favorite_over.dds",
	pressed = "textures/mwse/menu_modconfig_favorite_pressed.dds",
}

local nonFavoriteIcons = {
	idle = "textures/mwse/menu_modconfig_nonfavorite_idle.dds",
	over = "textures/mwse/menu_modconfig_nonfavorite_over.dds",
	pressed = "textures/mwse/menu_modconfig_nonfavorite_pressed.dds",
}


--- Checks to see if a mod is favorited.
--- @param modName string The name of the mod to check the state of.
--- @return boolean isFavorite If true, the mod will be favorited.
local function isFavorite(modName)
	return config.favorites[modName] == true
end

--- Sets a mod favorite status.
--- @param uiModName string The name of the mod to set the state of.
--- @param favorited boolean should the mod be favorited?
local function setFavorite(uiModName, favorited)
	config.favorites[uiModName] = favorited or nil
end

--- Toggles the favorited state of a mod.
--- @param modName string The name of the mod to toggle favoriting for.
local function toggleFavorited(modName)
	-- Make it be `nil` instead of `false` so we don't bloat up the config file with a bunch of disabled mods.
	-- The `isFavorite` function handles `nil` values, so this isn't a problem.
	config.favorites[modName] = not config.favorites[modName] or nil
end


-- update the image icons for the various states of the favorite button
---@param imageButton tes3uiElement
---@param modName string
local function updateFavoriteImageButton(imageButton, modName)
	local iconTable = isFavorite(modName) and favoriteIcons or nonFavoriteIcons
	imageButton.children[1].contentPath = iconTable.idle
	imageButton.children[2].contentPath = iconTable.over
	imageButton.children[3].contentPath = iconTable.pressed
end

--- Compares two mod names, based on their favorite status and their names.
---@param a string Name of the first mod to compare.
---@param b string Name of the second mod to compare.
---@return boolean -- true if `a < b`
local function compareModNames(a, b)
	local aIsFavorite = isFavorite(a)
	-- check if `a` and `b` have different "favorite" statuses
	if aIsFavorite ~= isFavorite(b) then
		-- `true` if `a` is favorited and `b` isn't (so `a < b`)
		-- `false` if `b` is favorited and `a` isn't (so `b < a`)
		return aIsFavorite
	end
	return a:lower() < b:lower()
end

local function saveConfig()
	mwse.saveConfig("MWSE.MCM", config)
end

-- Closes the currently opened mod config menu, if it exists.
-- This is called when the user closes the MCM, or when the user clicks on a different mod name.
local function closeCurrentModConfig()
	-- if not currentTemplate then return end
	if not currentModName then return end
	
	local onClose
	local template = modTemplates[currentModName]
	-- was it made using a template or the legacy structure?
	if template then
		lastPageIndex = table.find(template.pages, template.currentPage)
		onClose = template.onClose
	else -- it's a legacy mod
		onClose = table.get(legacyMods[currentModName], "onClose")
	end

	if not onClose then return end

	local status, error = pcall(onClose, modConfigContainer)

	if (status == false) then
		mwse.log('Error in mod config close callback for mod "%s": %s\n%s', 
			currentModName, error, debug.traceback()
		)
	end
end
 

--- Callback for when a mod name has been clicked in the left pane.
--- @param e tes3uiEventData
local function onClickModName(e)
	
	if not modConfigContainer then
		error(fmt('mod config container not found for "%s"!', e.source and e.source.text))
	end
	
	local modNameButton = e.source
	local modName = modNameButton.text
	-- If we have a current mod, fire its close event.
	closeCurrentModConfig()
	currentModName = modName

	local onCreate
	local template = modTemplates[modName]

	if template then
		-- templates are created using methods, but we are expecting a regular function
		onCreate = function(container) template:create(container) end
	elseif legacyMods[modName] then
		onCreate = legacyMods[modName].onCreate
	else
		error(fmt("No mod config could be found for key '%s'.", modName))
		return
	end

	if (previousModConfigSelector) then
		previousModConfigSelector.widget.state = tes3.uiState.normal
	end
	modNameButton.widget.state = tes3.uiState.active
	previousModConfigSelector = modNameButton

	-- Destroy and recreate the parent container.
	modConfigContainer:destroyChildren()

	-- Fire the mod's creation event if it has one.
	local status, err = pcall(onCreate, modConfigContainer)
	if (status == false) then
		mwse.log("Error in mod config create callback: %s\n%s", err, debug.traceback())
	end

	-- Change the mod config title bar to include the mod's name.
	local menu = tes3ui.findMenu("MWSE:ModConfigMenu") --[[@as tes3uiElement]]
	menu.text = mwse.mcm.i18n("Mod Configuration - %s", { modName })
	menu:updateLayout()
	lastModName = modName
end

-- Used to reopen the MCM to the most recently viewed menu, and ideally the most recently viewed tab of that menu.
---@param modName string Name of the mod to open.
---@param pageIndex integer? The index of the mods MCM to set as active. Only works if `modName` corresponds to a `mwseTemplate`.
local function setActiveModMenu(modName, pageIndex)
	if not modName then return end
	local menu = tes3ui.findMenu("MWSE:ModConfigMenu")
	if not menu then return end
	local modList = menu:findChild("ModList")
	local modListContents = modList and modList:getContentElement()
	if not modListContents then return end
	
	-- Iterate through each mod button and check if the text matches `modName`.
	-- If there's a match:
	--	 1. Click on that mod button. 
	--	 2. Check if `modName` corresponds to a `modTemplate`. If it does, update the active tab.
	for _, child in ipairs(modListContents.children) do
		local modNameButton = child.children[1]

		if modNameButton.text == modName then
			modNameButton:triggerEvent(tes3.uiEvent.mouseClick)

			local template = modTemplates[modNameButton.text]
			-- bail out early if it's a legacy mod
			if not template then return end
			-- try to reopen the right tab
			local lastPage = pageIndex and template.pages[pageIndex]
			if lastPage then
				template:clickTab(lastPage)
			end
			return -- Found the mod, so stop iterating.
		end
	end
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
	updateFavoriteImageButton(e.source, modName)

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
		return compareModNames(a.children[1].text, b.children[1].text)
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
	-- first try a basic search, then do the internal search
	if modName:lower():find(searchText, nil, true) then return true end
	
	-- if the mod has a template, use the template's search logic
	local template = modTemplates[modName]
	if template then
		return template:onSearchInternal(searchText)
	end

	-- otherwise, see if the legacy mod has any custom search logic
	local package = legacyMods[modName]
	return package and package.onSearch and package.onSearch(searchText) 
		or false
end

--- @param e tes3uiEventData
local function onSearchUpdated(e)
	local lowerSearchText = e.source.text:lower()
	local mcm = e.source:getTopLevelMenu()
	local modList = mcm:findChild("ModList")
	for _, child in ipairs(modList:getContentElement().children) do
		child.visible = filterModByName(child.children[1].text, lowerSearchText)
	end

	mcm:updateLayout()
	modList.widget:contentsChanged()
end

--- @param e tes3uiEventData
local function onSearchCleared(e)
	local mcm = e.source:getTopLevelMenu()
	local modList = mcm:findChild("ModList")
	for _, child in ipairs(modList:getContentElement().children) do
		child.visible = true
	end
	mcm:updateLayout()
	modList.widget:contentsChanged()
end


local function cleanupMCM(e)
	currentModName = nil
	modConfigContainer = nil
	previousModConfigSelector = nil
end

---@return tes3uiElement menu
local function makeRootMenu()
	-- Create the main menu frame.
	local menu = tes3ui.createMenu({ id = "MWSE:ModConfigMenu", dragFrame = true })
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
	menu:register("unfocus", function() return false end)

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

	-- List of all mod names (both legacy mods and mods made using a `mwseMCMTemplate`).
	local sortedModNames = table.keys(modTemplates) ---@type string[]

	-- Add in the legacy mods.
	for legacyModName in pairs(legacyMods) do
		table.insert(sortedModNames, legacyModName)
	end

	table.sort(sortedModNames, compareModNames)

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
		local imageButton = entryBlock:createImageButton(nonFavoriteIcons)
		updateFavoriteImageButton(imageButton, modName)
		imageButton.childAlignY = 0.5
		imageButton.absolutePosAlignX = .97
		imageButton.absolutePosAlignY = 0.5

		imageButton:register(tes3.uiEvent.mouseClick, onClickFavoriteButton)
		---@param image tes3uiElement
		for _, image in ipairs(imageButton.children) do
			image.scaleMode = true
			image.height = 16
			image.width = 16
			image.paddingTop = 3
		end
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
		text = tes3.findGMST(tes3.gmst.sClose).value
	})
	closeButton:register("mouseClick", onClickCloseButton)
	event.register("keyDown", onClickCloseButton, { filter = tes3.scanCode.escape })

	-- Cause the menu to refresh itself.
	menu:updateLayout()
	modList.widget:contentsChanged()

	-- Reopen the most recently viewed config menu (if there was one)
	setActiveModMenu(lastModName, lastPageIndex)
	return menu
end

-- Callback for when the mod config button has been clicked.
-- Here, we'll create the GUI and set up everything.
local function onClickModConfigButton()
	-- Play the click sound.
	tes3.worldController.menuClickSound:play()

	local menu = tes3ui.findMenu("MWSE:ModConfigMenu") or makeRootMenu()
	menu.visible = true

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

	-- Don't show the UI if we don't have any mod configs to show.
	if (table.empty(modTemplates) and table.empty(legacyMods)) then
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


--- checks if a given mod name was taken
---@param modName string
---@return boolean isTaken
local function isModNameTaken(modName)
	return (modTemplates[modName] or legacyMods[modName]) ~= nil
end


--- Define a new function in the mwse namespace that lets mods register for mod config.
--- @deprecated
--- @param name string
--- @param package mwse.registerModConfig.package|mwseMCMTemplate
function mwse.registerModConfig(name, package)

	if isModNameTaken(name) then
		-- Awkward backwards compatibility fix to account for `mcm.registerModData` not returning anything anymore
		-- According to the lua code dump, only 6 mods used the `mcm.registerModData` function, and they all called
		-- `registerModConfig` immediately afterwards.
		if package ~= nil then
			error(fmt('mwse.registerModConfig: A mod with the name "%s" has already been registered!', name))
		end
		return
	end
	-- Check if it's a `mwseMCMTemplate`, and call the new registration function if possible.
	if package.componentType == "Template" and package.class == "Template" then
		mwse.registerModTemplate(package)
		return
	end
	-- Actually register the package.
	--- @cast package mwseLegacyMod
	package.name = name
	legacyMods[name] = package
	mwse.log("[MCM] Registered legacy mod config: %s", name)
end

-- New registration function.
---@param template mwseMCMTemplate
function mwse.registerModTemplate(template)
	assert(template, "mwse.registerModTemplate: No template provided")
	assert(type(template) == "table" and template.componentType == "Template", 
		"mwse.registerModTemplate: Invalid template provided"
	)

	local name = template.name
	assert(not isModNameTaken(name), 
		fmt("mwse.registerModTemplate: A mod with the name %s has already been registered!", name)
	)
	-- Actually register the package.
	modTemplates[name] = template
	mwse.log("[MCM] Registered mod config: %s", name)
end

--- When we've initialized, set up our UI IDs and let other mods know that we are ready to boogie.
---
--- Set this up to run before most other initialized callbacks.
local function onInitialized()
	event.trigger("modConfigReady")
end
event.register("initialized", onInitialized, { priority = 100 })
