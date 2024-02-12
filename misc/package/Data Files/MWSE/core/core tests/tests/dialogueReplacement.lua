local common = require("core tests.common")

local configPath = "mwse_test_dialogueReplacement"
local config = mwse.loadConfig(configPath, { testedCells = {} })

local testLog = require("logging.logger").new({
    name = "MWSE Core Tests.DialogueReplacement",
    logLevel = "TRACE",
    logToConsole = false,
    includeTimestamp = true,
})

local infoIds = {}

local function getInfoId(info)
	local cached = infoIds[info]
	if (cached == nil) then
		cached = info.id
		infoIds[info] = cached
	end
	return cached
end

local testCount = 0

--- @param dialogue tes3dialogue
--- @param info tes3dialogueInfo
local function testDialogueInfo(dialogue, info, reference)
	local infoId = getInfoId(info)

	local actor = reference.object

	mwseConfig.ReplaceDialogueFiltering = false --- @diagnostic disable-line
	local withoutPatch = info:filter(actor, reference, 0, dialogue)

	mwseConfig.ReplaceDialogueFiltering = true --- @diagnostic disable-line
	local withPatch = info:filter(actor, reference, 0, dialogue)

	if (withPatch ~= withoutPatch) then
		testLog:error("Actor: %s; Reference: %s; Dialogue: %s; Info: %s; With Patch: %s; Without Patch: %s\n\tText: %s", actor, reference, dialogue, infoId, withPatch, withoutPatch, info.text)
		mwse.breakpoint()
		mwseConfig.ReplaceDialogueFiltering = false --- @diagnostic disable-line
		info:filter(actor, reference, 0, dialogue)
		mwseConfig.ReplaceDialogueFiltering = true --- @diagnostic disable-line
		info:filter(actor, reference, 0, dialogue)
		return false
	end

	-- testLog:assert(withPatch == withoutPatch, "Actor: %s; Reference: %s; Dialogue: %s; Info: %s; With Patch: %s; Without Patch: %s", actor, reference, dialogue, infoId, withPatch, withoutPatch)

	testCount = testCount + 1

	return true
end

--- @param dialogue tes3dialogue
local function testDialogue(dialogue, reference)
	if (dialogue.type == tes3.dialogueType.journal) then
		return
	end

	for _, info in ipairs(dialogue.info) do
		if (testDialogueInfo(dialogue, info, reference) == false) then
			return false
		end
	end

	return true
end

local function doTestWithGatheredActors(references)
	if (#references == 0) then
		testLog:warn("Attempting to perform test against an empty reference list.")
		return
	end

	testLog:debug("Testing against %d active references.", #references)
	testCount = 0
	for _, reference in ipairs(references) do
		testLog:debug("Testing against active reference: %s", reference)
		for _, dialogue in ipairs(tes3.dataHandler.nonDynamicData.dialogues) do
			if (testDialogue(dialogue, reference) == false) then
				return false
			end
		end
	end

	testLog:debug("Test completed and successful after %d comparisons.", testCount)
	return true
end

--- @param e cellChangedEventData
local function onCellChanged(e)
	if (config.testedCells[e.cell.editorName]) then
		testLog:trace("Skipping cell '%s', as it was already validated.", e.cell.editorName)
		return
	end

	local references = {}
	for reference in e.cell:iterateReferences({ tes3.objectType.npc, tes3.objectType.creature }) do
		if (not reference.disabled and not reference.deleted) then
			table.insert(references, reference)
		end
	end

	timer.delayOneFrame(function()
		local result = doTestWithGatheredActors(references)
		if (result) then
			testLog:debug("Cell '%s' passed all tests. Cached to not repeat.", e.cell.editorName)
			config.testedCells[e.cell.editorName] = true
		elseif (result == false) then
			testLog:error("Cell '%s' failed test. Test needs repeating.", e.cell.editorName)
		end
		mwse.saveConfig(configPath, config)
	end)
end
event.register(tes3.event.cellChanged, onCellChanged)
