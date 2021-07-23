--- @meta

--- A structure that keeps track of combat session data.
--- @class tes3combatSession
--- @field alchemyPriority number No description yet available.
--- @field distance number No description yet available.
--- @field mobile tes3mobileActor No description yet available.
--- @field selectedAction number No description yet available.
--- @field selectedAlchemy tes3alchemy No description yet available.
--- @field selectedShield tes3equipmentStack No description yet available.
--- @field selectedSpell tes3spell No description yet available.
--- @field selectedWeapon tes3equipmentStack No description yet available.
--- @field selectionPriority number No description yet available.
tes3combatSession = {}

--- Selects the alchemy item with the greatest value, for a given effect ID and loads it into the selectedAlchemy property.
--- @return number result No description yet available.
function tes3combatSession:selectAlchemyWithEffect() end

