--- @meta

--- An Actor (not to be confused with a Mobile Actor) is an object that can be cloned and that has an inventory. Creatures, NPCs, and containers are all considered actors.
--- 
--- It is standard for creatures and NPCs to be composed of an actor and a mobile actor, linked together with a reference.
--- @class tes3actor : tes3physicalObject, tes3object, tes3baseObject
--- @field actorFlags number A number representing the actor flags. Truly a bit field.
--- @field cloneCount number The number of clones that exist of this actor.
--- @field equipment tes3iterator The items currently equipped to the actor.
--- @field inventory tes3iterator The items currently carried by the actor.
tes3actor = {}

--- Checks if the actor will offer a service in dialogue. This an offer and may still be refused by dialogue checks. To also get the result of dialogue checks, use tes3.checkMerchantOffersService.
--- @param service number Use one of the tes3.merchantService.* constants.
--- @return boolean result No description yet available.
function tes3actor:offersService(service) end

--- A callback function invoked when an inventory is closed. Typically not used outside of specific purposes. You may find tes3.reference's onCloseInventory() to be more convenient to use.
--- @param reference tes3reference No description yet available.
function tes3actor:onInventoryClose(reference) end

--- Checks if the actor will buy and sell items of a given object type. e.g. actor:tradesItemType(tes3.objectType.repairItem)
--- @param objectType tes3objectType No description yet available.
--- @return boolean result No description yet available.
function tes3actor:tradesItemType(objectType) end

