--- @meta

--- A set of variables that differentiates one item from another.
--- @class tes3itemData
--- @field charge number The charge of the item. Provides incorrect values on misc items, which instead have a soul actor.
--- @field condition number The condition/health of the item. Provides incorrect values on light items, which instead have a timeLeft property.
--- @field context tes3scriptContext Returns an ease of use script context for variable access.
--- @field count number The total number of identical items represented by this data. Reserved for use by in-world references. It should always be 1 for items in inventories. Inventories utilize the stack count in tes3itemStack.
--- @field data table A generic lua table that data can be written to, and synced to/from the save. All information stored must be valid for serialization to json. For data that doesn't persist, use tempData.
--- @field owner tes3npc|tes3faction|nil The script associated with the scriptVariables.
--- @field requirement tes3globalVariable|number|nil A requirement, typically associated with ownership and when the player may freely interact with an object. The type depends on the owner. Faction owners provide a required rank as a number, while NPCs provide a global variable to check.
--- @field script tes3script The script associated with the scriptVariables.
--- @field scriptVariables tes3scriptVariables Access to the structure where individual mwscript data is stored.
--- @field soul tes3actor Only available on misc items. The actor that is stored inside the soul gem.
--- @field tempData table A generic lua table that data can be written to, but is not synced to/from a save.
--- @field timeLeft number The time remaining on a light. Provides incorrect values on non-light items, which instead have a condition property.
tes3itemData = {}

