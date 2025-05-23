--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This event is raised when an item price is being determined when bartering.
--- @class calcBarterPriceEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field basePrice number *Read-only*. The base price of the item, before any event modifications.
--- @field buying boolean *Read-only*. If true, the player is buying, otherwise the player is selling.
--- @field count number *Read-only*. The number of items being bartered.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3item|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon *Read-only*. The item, if any, that is being bartered.
--- @field itemData tes3itemData *Read-only*. The item data for the bartered item.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor for who is selling or buying. May not always be available.
--- @field price number The price of the item. This can be modified, but ensure that the buy/sell price is matched or there will be odd behavior.
--- @field reference tes3reference *Read-only*. A shortcut to the mobile's reference. May not always be available.
