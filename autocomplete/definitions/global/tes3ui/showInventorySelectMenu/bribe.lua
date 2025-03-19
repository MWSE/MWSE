-- This is a working example. You are encouraged to create a new file
-- main.lua, paste this code and see the results in-game!
-- Use `u` key to bribe an actor you are looking at with items from your inventory.


local function bribe()
	local actorReference = tes3.getPlayerTarget()
	-- This function returns nil value if the player isn't aiming at a reference,
	-- so we skip that case
	if not actorReference then return end

	tes3ui.showInventorySelectMenu({
		reference = tes3.player,
		-- .. is Lua operator of concatenation. It joins 2 strings together.
		title = "Bribe " .. actorReference.object.name,
		callback = function(e)
			if not e.item then return end

			-- If e.item exist, that means that the player picked an
			-- item in the menu. It up to us to do something with it.
			tes3.transferItem({
				from = tes3.player,
				to = actorReference,
				item = e.item,
				itemData = e.itemData,
				count = e.count,
			})
			-- Here we calculate the total gold value of the transfered item(s), since that
			-- can be a stack of items. e.count holds the amount of the items selected.
			local itemWorth = e.item.value * e.count

			-- At last! Now the actual persuasion part. We use `modifier` argument.
			-- The higher the value we pass there the higher the disposition change.
			tes3.persuade({
				actor = actorReference,
				modifier = math.log10(itemWorth)
			})
		end,
		-- At first it's counter intuitive that this filter selects all the non-enchanted items
		-- This illusion disappears soon as we relize that the game uses this filter in the
		-- enchanting menu to select items viable for enchanting.
		filter = tes3.inventorySelectFilter.enchanted
	})

end

-- We registered our bribe function on keyDown event, and filtered it for `u` key,
-- so our bribe function is only called once `u` key is pressed
event.register(tes3.event.keyDown, bribe, { filter = tes3.scanCode.u })
