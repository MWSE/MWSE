--- @meta

--- A parent-level dialogue, such as a topic, voice, greeting, persuasion response, or journal.
--- @class tes3dialogue : tes3baseObject
--- @field info tes3iterator A collection of individual entries in the dialogue.
--- @field journalIndex number For journal dialogues, the currently active journal index.
--- @field type number The type of the dialogue.
tes3dialogue = {}

--- Adds the dialogue to the player's journal, if applicable, at a given index.
--- @param params tes3dialogue.addToJournal.params This table accepts the following values:
--- 
--- ``index``: number — No description yet available.
--- 
--- ``actor``: tes3mobileActor|tes3reference|string — No description yet available.
--- @return boolean result No description yet available.
function tes3dialogue:addToJournal(params) end

---Table parameter definitions for ``tes3dialogue.addToJournal``.
--- @class tes3dialogue.addToJournal.params
--- @field index number No description yet available.
--- @field actor tes3mobileActor|tes3reference|string No description yet available.

--- Fetches the info that a given actor would produce for the dialogue.
--- @param params tes3dialogue.getInfo.params This table accepts the following values:
--- 
--- ``actor``: tes3mobileActor|tes3reference|string — No description yet available.
--- @return tes3dialogueInfo result No description yet available.
function tes3dialogue:getInfo(params) end

---Table parameter definitions for ``tes3dialogue.getInfo``.
--- @class tes3dialogue.getInfo.params
--- @field actor tes3mobileActor|tes3reference|string No description yet available.

