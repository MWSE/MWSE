--- @meta

--- An inventory composes of an iterator, and flags caching the state of the inventory.
--- @class tes3inventory
--- @field flags number Raw bit-based flags.
--- @field iterator tes3iterator Deprecated. Use ``for index, stack in pairs(inventory)`` to iterate over inventory stacks.
--- 	
--- Direct access to the container that holds the inventory's items.
tes3inventory = {}

--- Adds an item into the inventory directly. This should not be used, in favor of the tes3.addItem function.
--- @param params tes3inventory.addItem.params This table accepts the following values:
--- 
--- ``mobile``: tes3mobileActor|tes3reference|string — The mobile actor whose stats will be updated.
--- 
--- ``item``: tes3item — The item to add.
--- 
--- ``itemData``: tes3itemData — Any associated item data to add.
--- 
--- ``count``: number — The number of items to add.
function tes3inventory:addItem(params) end

---Table parameter definitions for ``tes3inventory.addItem``.
--- @class tes3inventory.addItem.params
--- @field mobile tes3mobileActor|tes3reference|string The mobile actor whose stats will be updated.
--- @field item tes3item The item to add.
--- @field itemData tes3itemData Any associated item data to add.
--- @field count number The number of items to add.

--- Calculates the weight of all items in the container.
--- @return number result No description yet available.
function tes3inventory:calculateWeight() end

--- Checks to see if the inventory contains an item.
--- @param item tes3item|string The item to check for.
--- @param itemData tes3itemData If provided, it will check for the specific data as well.
--- @return boolean result No description yet available.
function tes3inventory:contains(item, itemData) end

--- Checks to see if the inventory contains an item. This should not be used, instead use the tes3.dropItem function.
--- @param mobile tes3mobileActor|tes3reference|string The mobile actor whose stats will be updated.
--- @param item tes3item|string The item to drop.
--- @param itemData tes3itemData If provided, it will check for the specific data to drop it.
--- @param count number The number of items to drop.
--- @param position tes3vector3 A vector determining placement location.
--- @param orientation tes3vector3 A vector determining placement rotation.
--- @param ignoreItemData boolean No description yet available.
function tes3inventory:dropItem(mobile, item, itemData, count, position, orientation, ignoreItemData) end

--- Searches for an item stack in the inventory.
--- @param item tes3item The item to search for.
--- @return tes3itemStack result No description yet available.
function tes3inventory:findItemStack(item) end

--- Removes an item from the inventory directly. This should not be used, in favor of the tes3.removeItem function.
--- @param params tes3inventory.removeItem.params This table accepts the following values:
--- 
--- ``mobile``: tes3mobileActor|tes3reference|string — The mobile actor whose stats will be updated.
--- 
--- ``item``: tes3item — The item to add.
--- 
--- ``itemData``: tes3itemData — Any associated item data to add.
--- 
--- ``count``: number — The number of items to add.
--- 
--- ``deleteItemData``: boolean — If set, the itemData will be deleted after being removed.
function tes3inventory:removeItem(params) end

---Table parameter definitions for ``tes3inventory.removeItem``.
--- @class tes3inventory.removeItem.params
--- @field mobile tes3mobileActor|tes3reference|string The mobile actor whose stats will be updated.
--- @field item tes3item The item to add.
--- @field itemData tes3itemData Any associated item data to add.
--- @field count number The number of items to add.
--- @field deleteItemData boolean If set, the itemData will be deleted after being removed.

--- Resolves all contained leveled lists and adds the randomized items to the inventory. This should generally not be called directly.
--- @param mobile tes3mobileActor The mobile actor whose stats will be updated.
function tes3inventory:resolveLeveledItems(mobile) end

