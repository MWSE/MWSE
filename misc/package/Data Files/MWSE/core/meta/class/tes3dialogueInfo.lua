--- @meta

--- A child for a given dialogue. Whereas a dialogue may be a conversation topic, a tes3dialogueInfo would be an individual response.
--- @class tes3dialogueInfo : tes3baseObject
--- @field actor tes3actor The speaker's actor that the info is filtered for.
--- @field cell tes3cell The speaker's current cell that the info is filtered for.
--- @field disposition number The minimum disposition that the info is filtered for.
--- @field firstHeardFrom tes3actor The actor that the player first heard the info from.
--- @field id string The unique long string ID for the info. This is not kept in memory, and must be loaded from files for each call.
--- @field npcClass tes3class The speaker's class that the info is filtered for.
--- @field npcFaction tes3faction The speaker's faction that the info is filtered for.
--- @field npcRace tes3actor The speaker's race that the info is filtered for.
--- @field npcRank number The speaker's faction rank that the info is filtered for.
--- @field npcSex number The speaker's sex that the info is filtered for.
--- @field pcFaction number The player's joined faction that the info is filtered for.
--- @field pcRank number The player's rank required rank in the speaker's faction.
--- @field text string String contents for the info. This is not kept in memory, and must be loaded from files for each call.
--- @field type number The type of the info.
tes3dialogueInfo = {}

