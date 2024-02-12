
-- Pressing i key while looking at a container, NPC or creature
-- will transfer the target's weapons to the player.

---@param e keyDownEventData
local function onKeyDown(e)
	local target = tes3.getPlayerTarget()
	if not target then
		tes3.messageBox("No target!")
		return
	end

	tes3.transferInventory({
		from = target,
		to = tes3.player,
		---@param item tes3item
		---@param itemData tes3itemData?
		---@return boolean
		filter = function(item, itemData)
			-- You can use the itemData to filter more specifically
			if item.objectType == tes3.objectType.weapon then
				return true
			end
			return false
		end,
		limitCapacity = false,
	})
end
event.register(tes3.event.keyDown, onKeyDown, { filter = tes3.scanCode.i })
