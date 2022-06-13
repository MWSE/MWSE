--- @meta
--- @diagnostic disable:undefined-doc-name

--- An inventory composes of an iterator, and flags caching the state of the inventory.
--- @class tes3inventory
--- @field flags number *Read-only*. Raw bit-based flags.
--- @field items tes3iterator *Read-only*. Direct acces to the container that holds the inventory's items.
--- @field iterator tes3iterator *Read-only*. Deprecated. Use `for index, stack in pairs(inventory)` to iterate over inventory stacks.
--- 	
--- Direct access to the container that holds the inventory's items.
tes3inventory = {}

--- Adds an item into the inventory directly. This should not be used, in favor of the [`tes3.addItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3additem) function.
--- @param params tes3inventory.addItem.params This table accepts the following values:
--- 
--- `mobile`: tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3reference|string|nil — *Optional*. The mobile actor whose stats will be updated.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon — The item to add.
--- 
--- `itemData`: tes3itemData? — *Optional*. Any associated item data to add.
--- 
--- `count`: number? — *Default*: `1`. The number of items to add.
function tes3inventory:addItem(params) end

---Table parameter definitions for `tes3inventory.addItem`.
--- @class tes3inventory.addItem.params
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3reference|string|nil *Optional*. The mobile actor whose stats will be updated.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon The item to add.
--- @field itemData tes3itemData? *Optional*. Any associated item data to add.
--- @field count number? *Default*: `1`. The number of items to add.

--- Calculates the weight of all items in the container.
--- @return number result No description yet available.
function tes3inventory:calculateWeight() end

--- Checks to see if the inventory contains an item.
--- @param item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to check for.
--- @param itemData tes3itemData? *Optional*. If provided, it will check for the specific data as well.
--- @return boolean result No description yet available.
function tes3inventory:contains(item, itemData) end

--- Checks to see if the inventory contains an item. This should not be used, instead use the [`tes3.dropItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3dropitem) function.
--- @param mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3reference|string The mobile actor whose stats will be updated.
--- @param item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to drop.
--- @param itemData tes3itemData If provided, it will check for the specific data to drop it.
--- @param count number The number of items to drop.
--- @param position tes3vector3 A vector determining placement location.
--- @param orientation tes3vector3 A vector determining placement rotation.
--- @param ignoreItemData boolean No description yet available.
function tes3inventory:dropItem(mobile, item, itemData, count, position, orientation, ignoreItemData) end

--- Searches for an item stack in the inventory.
--- @param item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon|string The item to search for.
--- @param itemData tes3itemData? *Optional*. If provided, it will check for the specific data as well.
--- @return tes3itemStack result No description yet available.
function tes3inventory:findItemStack(item, itemData) end

--- Removes an item from the inventory directly. This should not be used, in favor of the [`tes3.removeItem()`](https://mwse.github.io/MWSE/apis/tes3/#tes3removeitem) function.
--- @param params tes3inventory.removeItem.params This table accepts the following values:
--- 
--- `mobile`: tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3reference|string|nil — *Optional*. The mobile actor whose stats will be updated.
--- 
--- `item`: tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon — The item to add.
--- 
--- `itemData`: tes3itemData? — *Optional*. Any associated item data to add.
--- 
--- `count`: number? — *Default*: `1`. The number of items to add.
--- 
--- `deleteItemData`: boolean? — *Default*: `false`. If set, the itemData will be deleted after being removed.
function tes3inventory:removeItem(params) end

---Table parameter definitions for `tes3inventory.removeItem`.
--- @class tes3inventory.removeItem.params
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|tes3reference|string|nil *Optional*. The mobile actor whose stats will be updated.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon The item to add.
--- @field itemData tes3itemData? *Optional*. Any associated item data to add.
--- @field count number? *Default*: `1`. The number of items to add.
--- @field deleteItemData boolean? *Default*: `false`. If set, the itemData will be deleted after being removed.

--- Resolves all contained leveled lists and adds the randomized items to the inventory. This should generally not be called directly.
--- @param mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer|nil *Optional*. The mobile actor whose stats will be updated.
function tes3inventory:resolveLeveledItems(mobile) end
