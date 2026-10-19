
--
-- Load parameters passed by CSSE.
--

local test_environment = require("csse.environment").test_environment
if (test_environment == nil) then
	return
end

-- Lowercase IDs so we can test things better.
local function lowercaseKeys(tbl)
	local to_lower = {}
	for k, v in pairs(tbl) do
		if (type(k) == "string" and k:lower() ~= k) then
			to_lower[k] = v
		end
	end
	for k, v in pairs(to_lower) do
		tbl[k] = nil
		tbl[k:lower()] = v
	end
end
lowercaseKeys(test_environment.globals)
lowercaseKeys(test_environment.inventory)
lowercaseKeys(test_environment.journal)

-- Make sure we have a chargenstate specified.
if (test_environment.globals["chargenstate"] == nil) then
	test_environment.globals["chargenstate"] = -1.0
end

event.trigger("csse:environmentSetup", { environment = test_environment })
mwse.log("[CSSE] Loading test environment: %s", json.encode(test_environment, { indent = true }))


--
-- Overwrite the loading files to only load the files active in the CS.
--

-- Flag which files we want to load based on what we were given from CSSE, rather than loading from Morrowind.ini.
local function flagLoadFiles()
	for _, gameFile in ipairs(test_environment.game_files) do
		local file = tes3.dataHandler.nonDynamicData:getGameFile(gameFile)
		if (file == nil) then
			error(string.format("[CSSE] Could not locate game file for testing: %s", gameFile))
		end

		file.toLoad = true
	end
end

-- Do we even have any files to load?
if (#test_environment.game_files > 0) then
	mwse.memory.writeFunctionCall({ address = 0x419F53, call = flagLoadFiles, length = 0x419FE1 - 0x419F53 })
else
	mwse.log("[CSSE] No game files set to load!")
end


--
-- If start_new_game is set, automatically load into position.
--

local function noop() return false end

local chargenScripts = { "CharGen" }

local function ensureTestEnvironmentValues()
	for journal, index in pairs(test_environment.journal or {}) do
		local object = tes3.findDialogue({ topic = journal, type = tes3.dialogueType.journal })
		if (object) then
			tes3.setJournalIndex({ id = journal, index = index, showMessage = false })
		else
			mwse.log("[CSSE] Could not find journal: %s", journal)
		end
	end

	for _, topic in ipairs(test_environment.topics or {}) do
		local object = tes3.findDialogue({ topic = topic, type = tes3.dialogueType.topic })
		if (object) then
			tes3.addTopic({ topic = topic })
		else
			mwse.log("[CSSE] Could not find topic: %s", topic)
		end
	end

	for item, count in pairs(test_environment.inventory or {}) do
		local object = tes3.getObject(item)
		if (object) then
			local currentCount = tes3.getItemCount({ reference = tes3.player, item = item })
			if (currentCount < count) then
				tes3.addItem({ reference = tes3.player, item = item, count = count - currentCount, playSound = false })
			end
		else
			mwse.log("[CSSE] Could not find item: %s", item)
		end
	end

	for _, spell in ipairs(test_environment.spells or {}) do
		local object = tes3.getObject(spell) --- @cast object tes3spell
		if (object and object.objectType == tes3.objectType.spell) then
			tes3.addSpell({ reference = tes3.player, spell = object })
		else
			mwse.log("[CSSE] Could not find spell: %s", spell)
		end
	end

	for global, value in pairs(test_environment.globals or {}) do
		local object = tes3.findGlobal(global)
		if (object) then
			object.value = value
		else
			mwse.log("[CSSE] Could not find global: %s", global)
		end
	end
end

local function onLoaded(e)
	-- If we were starting a new game, reposition ourselves to the right place.
	if (test_environment.start_new_game and test_environment.load_save_morrowind == "") then
		local cell = nil
		if (test_environment.starting_cell ~= "") then
			cell = tes3.getCell({ id = test_environment.starting_cell })
			if (cell == nil) then
				error("Could not resolve starting cell!")
			end
		end
		tes3.positionCell({
			reference = tes3.player,
			cell = cell,
			position = test_environment.position,
			orientation = test_environment.orientation,
		})
	end

	-- Actually set our variables.
	ensureTestEnvironmentValues()

	-- Clean up any script overrides.
	for _, script in ipairs(chargenScripts) do
		mwse.clearScriptOverride(script)
	end

	event.trigger("csse:loaded", { environment = test_environment })

	mwse.log("[CSSE] Environment injection complete.")
end

local function loadTestingSave()
	-- Prevent any chargen scripts from running.
	for _, script in ipairs(chargenScripts) do
		mwse.overrideScript(script, noop)
	end

	-- Let us know when we've loaded in.
	event.register(tes3.event.loaded, onLoaded, { doOnce = true })

	-- Were we given a specific save to load?
	if (test_environment.load_save_morrowind ~= "") then
		tes3.loadGame(test_environment.load_save_morrowind)
		return
	end

	-- Otherwise start a new game.
	if (test_environment.start_new_game) then
		-- Prevent the intro movie from playing.
		mwse.memory.writeNoOperation({ address = 0x5FB44A, length = 0x5FB50A - 0x5FB44A })

		tes3.newGame()
		return
	end
end

--- @param e uiActivatedEventData
local function onOptionsMenuActivated(e)
	local optionsMenu = e.element

	optionsMenu.visible = false
	optionsMenu:updateLayout()

	timer.frame.delayOneFrame(loadTestingSave)
end

local function onAboutToShowMainMenu()
	-- If we're not loading a save or starting a new game, there's no point in continuing.
	if (test_environment.load_save_morrowind == "" and test_environment.start_new_game == false) then
		return
	end

	event.register(tes3.event.uiActivated, onOptionsMenuActivated, { doOnce = true })
end
event.register(tes3.event.initialized, onAboutToShowMainMenu, { doOnce = true })
