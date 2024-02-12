local dialogueMenuUIID = tes3ui.registerID("MenuDialog")
local goodbyeButtonUIID = tes3ui.registerID("MenuDialog_button_bye")
local barterButtonUIID = tes3ui.registerID("MenuDialog_service_barter")
local shareButtonUIID = tes3ui.registerID("MenuDialog_service_companion")
local enchantingButtonUIID = tes3ui.registerID("MenuDialog_service_enchanting")
local persuasionButtonUIID = tes3ui.registerID("MenuDialog_persuasion")
local repairButtonUIID = tes3ui.registerID("MenuDialog_service_repair")
local spellmakingButtonUIID = tes3ui.registerID("MenuDialog_service_spellmaking")
local spellsButtonUIID = tes3ui.registerID("MenuDialog_service_spells")
local trainingButtonUIID = tes3ui.registerID("MenuDialog_service_training")
local travelButtonUIID = tes3ui.registerID("MenuDialog_service_travel")

local alchemyMenuUIID = tes3ui.registerID("MenuAlchemy")
local barterMenuUIID = tes3ui.registerID("MenuBarter")
local classMenuUIID = tes3ui.registerID("MenuClassChoice")
local companionShareMenuUIID = tes3ui.registerID("MenuContents")
local enchantingMenuUIID = tes3ui.registerID("MenuEnchantment")
local persuasionMenuUIID = tes3ui.registerID("MenuPersuasion")
local raceMenuUIID = tes3ui.registerID("MenuRaceSex")
local repairMenuUIID = tes3ui.registerID("MenuServiceRepair")
local spellmakingMenuUIID = tes3ui.registerID("MenuSpellmaking")
local spellsServiceMenuUIID = tes3ui.registerID("MenuServiceSpells")
local trainingMenuUIID = tes3ui.registerID("MenuServiceTraining")
local travelMenuUIID = tes3ui.registerID("MenuServiceTravel")

-- This is the vertical offset in pixels of Persuasion menu
-- from the top of the Dialogue menu.
local constOffsetY = -65

--- The function opens a menu within dialogue window. Returns true if the menu is open or was opened.
---@param menuID number The id of the menu to open.
---@param buttonID number The id of the button that opens the menu from dialogue menu window.
---@return boolean opened
local function openMenu(menuID, buttonID)
	local menu = tes3ui.findMenu(menuID)
	if menu then
		return true
	end

	local dialogueMenu = tes3ui.findMenu(dialogueMenuUIID)
	if dialogueMenu then
		local button = dialogueMenu:findChild(buttonID)
		button:triggerEvent(tes3.uiEvent.mouseClick)
		return true
	end
	return false
end

--- @param e dialogueEnvironmentCreatedEventData
local function onDialogueEnvironmentCreated(e)
	-- Cache the environment variables outside the function for easier access.
	-- Dialogue scripters shouldn't have to constantly pass these to the functions anyway.
	local env = e.environment
	local reference = env.reference --- @type tes3reference
	---@diagnostic disable-next-line:assign-type-mismatch
	local mobile = reference.mobile --- @type tes3mobileActor
	local dialogue = env.dialogue --- @type tes3dialogue
	local info = env.info --- @type tes3dialogueInfo

	-- These are default environment functions available to dialogue scripters.
	-- They should be designed in a way that is easy for them to be called.

	function env.CloseDialogue(force)
		return tes3.closeDialogueMenu({ force = force })
	end

	function env.FollowPlayer()
		tes3.setAIFollow({
			reference = reference,
			target = tes3.player,
		})
	end

	function env.WaitHere()
		tes3.setAIWander({
			reference = reference,
			idles = { 60, 10, 10, 1, 10, 5, 2, 2 }
		})
	end

	function env.OpenAlchemyMenu()
		error("Not yet implemented.")
	end

	function env.OpenBarterMenu()
		return openMenu(barterMenuUIID, barterButtonUIID)
	end

	function env.OpenClassMenu()
		error("Not yet implemented.")
	end

	function env.OpenCompanionInventory()
		return openMenu(companionShareMenuUIID, shareButtonUIID)
	end

	function env.OpenEnchantMenu()
		return openMenu(enchantingMenuUIID, enchantingButtonUIID)
	end

	function env.OpenPersuasionMenu()
		-- This menu is opened under the mouse cursor
		openMenu(persuasionMenuUIID, persuasionButtonUIID)

		-- Move it under the persuasion button
		local dialogueMenu = tes3ui.findMenu(dialogueMenuUIID)
		local mainBlock = dialogueMenu:findChild("MenuDialog_scroll_pane")
		local persuasionMenu = tes3ui.findMenu(persuasionMenuUIID)
		persuasionMenu.positionX = dialogueMenu.positionX + mainBlock.width
		persuasionMenu.positionY = dialogueMenu.positionY + constOffsetY
		persuasionMenu:updateLayout()
	end

	function env.OpenRaceMenu()
		error("Not yet implemented.")
	end

	function env.OpenRepairMenu()
		return openMenu(repairMenuUIID, repairButtonUIID)
	end

	function env.OpenSpellmakingMenu()
		return openMenu(spellmakingMenuUIID, spellmakingButtonUIID)
	end

	function env.OpenSpellMenu()
		return openMenu(spellsServiceMenuUIID, spellsButtonUIID)
	end

	function env.OpenTrainingMenu()
		return openMenu(trainingMenuUIID, trainingButtonUIID)
	end


	function env.OpenTravelMenu()
		-- Opening travel menu by using :triggerEvent crashes the game if used
		-- on a NPC that doesn't provide travelling services. Handle that.
		local travelDestinations = reference.object.aiConfig.travelDestinations
		if travelDestinations == nil then
			return false
		end
		return openMenu(travelMenuUIID, travelButtonUIID)
	end
end
event.register(tes3.event.dialogueEnvironmentCreated, onDialogueEnvironmentCreated, { priority = 100 })
