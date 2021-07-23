--- @meta

--- A child for a given dialogue. Whereas a dialogue may be a conversation topic, a tes3dialogueInfo would be an individual response.
--- @class tes3dialogueInfo : tes3baseObject
--- @field actor tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3npc|tes3npcInstance The speaker's actor that the info is filtered for.
--- @field cell tes3cell The speaker's current cell that the info is filtered for.
--- @field disposition number The minimum disposition that the info is filtered for.
--- @field firstHeardFrom tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3npc|tes3npcInstance The actor that the player first heard the info from.
--- @field npcClass tes3class The speaker's class that the info is filtered for.
--- @field npcFaction tes3faction The speaker's faction that the info is filtered for.
--- @field npcRace tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3npc|tes3npcInstance The speaker's race that the info is filtered for.
--- @field npcRank number The speaker's faction rank that the info is filtered for.
--- @field npcSex number The speaker's sex that the info is filtered for.
--- @field pcFaction number The player's joined faction that the info is filtered for.
--- @field pcRank number The player's rank required rank in the speaker's faction.
--- @field text string String contents for the info. This is not kept in memory, and must be loaded from files for each call.
--- @field type number The type of the info.
tes3dialogueInfo = {}

--- This method filters the associated dialogue info for given arguments and returns true if the dialogue info filtering passes. This method rises infoFilter event.
--- @param actor tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3enchantment|tes3ingredient|tes3item|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3reference|tes3spell|tes3static|tes3weapon No description yet available.
--- @param reference tes3reference No description yet available.
--- @param source number No description yet available.
--- @param dialogue tes3dialogue No description yet available.
--- @return boolean result No description yet available.
function tes3dialogueInfo:filter(actor, reference, source, dialogue) end

--- This method runs the dialogue info's associated script on a given reference.
--- @param reference tes3reference The reference on which to run the script on.
function tes3dialogueInfo:runScript(reference) end

