--- @meta

--- A mobile object for an NPC.
--- @class tes3mobileNPC : tes3mobileActor, tes3mobileObject
--- @field acrobatics tes3skillStatistic Direct access to the NPC's acrobatics skill statistic.
--- @field alchemy tes3skillStatistic Direct access to the NPC's alchemy skill statistic.
--- @field alteration tes3skillStatistic Direct access to the NPC's alteration skill statistic.
--- @field armorer tes3skillStatistic Direct access to the NPC's armorer skill statistic.
--- @field athletics tes3skillStatistic Direct access to the NPC's athletics skill statistic.
--- @field axe tes3skillStatistic Direct access to the NPC's axe skill statistic.
--- @field block tes3skillStatistic Direct access to the NPC's block skill statistic.
--- @field bluntWeapon tes3skillStatistic Direct access to the NPC's blunt weapon skill statistic.
--- @field conjuration tes3skillStatistic Direct access to the NPC's conjuration skill statistic.
--- @field destruction tes3skillStatistic Direct access to the NPC's destruction skill statistic.
--- @field enchant tes3skillStatistic Direct access to the NPC's enchant skill statistic.
--- @field flySpeed number The calculated fly movement speed.
--- @field forceJump boolean Toggle flag for if the NPC jumps.
--- @field forceMoveJump boolean Toggle flag for if the NPC moves and jumps.
--- @field forceRun boolean Toggle flag for if the NPC runs.
--- @field forceSneak boolean Toggle flag for if the NPC sneaks.
--- @field handToHand tes3skillStatistic Direct access to the NPC's hand to hand skill statistic.
--- @field heavyArmor tes3skillStatistic Direct access to the NPC's heavy armor skill statistic.
--- @field illusion tes3skillStatistic Direct access to the NPC's illusion skill statistic.
--- @field lightArmor tes3skillStatistic Direct access to the NPC's light armor skill statistic.
--- @field longBlade tes3skillStatistic Direct access to the NPC's long blade skill statistic.
--- @field marksman tes3skillStatistic Direct access to the NPC's marksman skill statistic.
--- @field mediumArmor tes3skillStatistic Direct access to the NPC's medium armor skill statistic.
--- @field mercantile tes3skillStatistic Direct access to the NPC's mercantile skill statistic.
--- @field moveSpeed number The calculated base movement speed.
--- @field mysticism tes3skillStatistic Direct access to the NPC's mysticism skill statistic.
--- @field object tes3npcInstance The actor object that maps to this mobile.
--- @field restoration tes3skillStatistic Direct access to the NPC's restoration skill statistic.
--- @field runSpeed number The calculated run movement speed.
--- @field security tes3skillStatistic Direct access to the NPC's security skill statistic.
--- @field shortBlade tes3skillStatistic Direct access to the NPC's short blade skill statistic.
--- @field skills table An array-style table with access to the twenty seven NPC skill statistics.
--- @field sneak tes3skillStatistic Direct access to the NPC's sneak skill statistic.
--- @field spear tes3skillStatistic Direct access to the NPC's spear skill statistic.
--- @field speechcraft tes3skillStatistic Direct access to the NPC's speechcraft skill statistic.
--- @field swimRunSpeed number The calculated swim movement speed while running.
--- @field swimSpeed number The calculated swim movement speed.
--- @field unarmored tes3skillStatistic Direct access to the NPC's unarmored skill statistic.
--- @field walkSpeed number The calculated walk movement speed.
tes3mobileNPC = {}

--- Equips an item, optionally adding the item if needed. If the best match is already equipped, it does not perform an unequip-equip cycle, but does return true.
--- @param params tes3mobileNPC.equip.params This table accepts the following values:
--- 
--- ``item``: tes3item|string — The item to equip.
--- 
--- ``itemData``: tes3itemData — The item data of the specific item to equip.
--- 
--- ``addItem``: boolean — If true, the item will be added to the actor's inventory if needed.
--- 
--- ``selectBestCondition``: boolean — If true, the item in the inventory with the best condition and best charge will be selected.
--- 
--- ``selectWorstCondition``: boolean — If true, the item in the inventory with the worst condition and worst charge will be selected. Can be useful for selecting tools.
--- @return boolean itemEquipped No description yet available.
function tes3mobileNPC:equip(params) end

---Table parameter definitions for ``tes3mobileNPC.equip``.
--- @class tes3mobileNPC.equip.params
--- @field item tes3item|string The item to equip.
--- @field itemData tes3itemData The item data of the specific item to equip.
--- @field addItem boolean If true, the item will be added to the actor's inventory if needed.
--- @field selectBestCondition boolean If true, the item in the inventory with the best condition and best charge will be selected.
--- @field selectWorstCondition boolean If true, the item in the inventory with the worst condition and worst charge will be selected. Can be useful for selecting tools.

--- Unequips one or more items from the actor.
--- @param params tes3mobileNPC.unequip.params This table accepts the following values:
--- 
--- ``item``: tes3item|string — The item to unequip.
--- 
--- ``type``: number — The item type to unequip. Only used if no other parameter is provided.
--- 
--- ``armorSlot``: number — The armor slot to unequip.
--- 
--- ``clothingSlot``: number — The clothing slot to unequip.
--- @return boolean result No description yet available.
function tes3mobileNPC:unequip(params) end

---Table parameter definitions for ``tes3mobileNPC.unequip``.
--- @class tes3mobileNPC.unequip.params
--- @field item tes3item|string The item to unequip.
--- @field type number The item type to unequip. Only used if no other parameter is provided.
--- @field armorSlot number The armor slot to unequip.
--- @field clothingSlot number The clothing slot to unequip.

