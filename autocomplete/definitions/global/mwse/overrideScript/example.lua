-- In this example, the vanilla "RaceCheck" script is overridden
-- with our own raceCheck() function that does the same thing.

local raceCheckScriptID = "RaceCheck"
local raceMap = {
	["argonian"] = 1,
	["breton"] = 2,
	["dark elf"] = 3,
	["high elf"] = 4,
	["imperial"] = 5,
	["khajiit"] = 6,
	["nord"] = 7,
	["orc"] = 8,
	["redguard"] = 9,
	["wood elf"] = 10,
}

local function raceCheck()
	-- It's almost always the desired behavior to stop the mwscript,
	-- since we are overriding it.
	---@diagnostic disable-next-line: deprecated
	mwscript.stopScript({ script = raceCheckScriptID })

	local pcRaceID = tes3.player.object.race.id:lower()
	local PCRace = tes3.findGlobal("PCRace")

	PCRace.value = raceMap[pcRaceID]
end

-- Script overrides can be queued when initialited event triggers.
event.register(tes3.event.initialized, function()
	mwse.overrideScript(raceCheckScriptID, raceCheck)
end)
