-- This example has a few implementations of filter functions that
-- can be passed to `filter` argument of tes3.showInventorySelectMenu().

-- This function will filter only weapon items.
---@param e tes3ui.showInventorySelectMenu.filterParams
local function weaponFilter(e)
	if e.item.objectType == tes3.objectType.weapon then
		-- The filter function needs to return `true`
		-- for a certain item to appear in the menu.
		return true
	end

	return false
end

-- This is a dictinary of items that can be damaged (have a condition)
local damageableItems = {
	[tes3.objectType.weapon] = true,
	[tes3.objectType.armor] = true,
}
-- This function will filter only items that aren't at full condition.
---@param e tes3ui.showInventorySelectMenu.filterParams
local function damagedItemsFilter(e)
	-- The first check is whether the item is in our
	-- dictionary of items with condition
	if damageableItems[e.item.objectType] and
	-- Only damaged items have this field. If it does
	-- not exist the item is in perfect condition.
	e.itemData and
	(e.itemData.condition < e.item.maxCondition) then
		return true
	end

	return false
end

local myFilterValue = 256
-- This function will filter only items that have a value less than `myFilterValue`.
---@param e tes3ui.showInventorySelectMenu.filterParams
local function valueFilter(e)
	if (e.item.value < myFilterValue) then
		return true
	end

	return false
end