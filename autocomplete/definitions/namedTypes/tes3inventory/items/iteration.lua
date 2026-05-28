
--- This is a generic iterator function that is used
--- to loop over all the items in an inventory
---@param actor tes3actor
---@return fun(): tes3item, integer, tes3itemData|nil
local function iterItems(actor)
	local function iterator()
		for _, stack in pairs(actor.inventory) do
			local item = stack.object
			-- Skip uncarryable lights. They are hidden from the interface. A MWSE mod
			-- could make the player glow from transferring such lights, which the player
			-- can't remove. Some creatures like atronaches have uncarryable lights
			-- in their inventory to make them glow that are not supposed to be looted.
			if item.canCarry == false then
				goto continue
			end

			-- Account for restocking items,
			-- since their count is negative.
			local count = math.abs(stack.count)

			-- First yield stacks with custom data
			for _, data in pairs(stack.variables or {}) do
				coroutine.yield(item, data.count, data)
				count = count - data.count
			end

			-- Then yield all the remaining copies
			if count > 0 then
				coroutine.yield(item, count)
			end

			:: continue ::
		end
	end
	return coroutine.wrap(iterator)
end

local player = tes3.player.object --[[@as tes3actor]]
for item, count, itemData in iterItems(player) do
	debug.log(item)
	debug.log(count)
	debug.log(itemData)
end
