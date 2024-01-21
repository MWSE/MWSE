-- Check if two items are on the same bookshelf
---@param ref1 tes3reference reference to one ingredient
---@param ref2 tes3reference reference to another ingredient
local function isOnSameShelf(ref1, ref2)
	local maxDistXY = 75 -- XY tolerance
	local maxDistZ = 150 -- Z tolerance

	-- distanceXY ignores the Z coordinate, which has the effect of 
	-- "pretending" `ref1` and `ref2` are on the same shelf
	local distanceXY = ref1.position:distanceXY(ref2.position)
	-- check the height difference separately, to make sure it's not too crazy 
	-- for example, if we're inside a building, this would make sure that 
	--  `ref1` and `ref2` are on the same floor of the building.
	local distanceZ = ref1.position:heightDifference(ref2.position)

	return distanceXY <= maxDistXY and distanceZ <= maxDistZ
end