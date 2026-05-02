return {
	type = "method",
	description = [[Unequips one or more items from the actor.

!!! note
	This method triggers the [unequipped](https://mwse.github.io/MWSE/events/unequipped/) event, unlike the `equip` method.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "item", type = "tes3item|string", optional = true, description = "The item to unequip." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data of the specific item to unequip, if a specific item is required." },
			{ name = "type", type = "tes3.objectType", optional = true, description = "The item type to unequip. Only used if no other parameter is provided. Only values pertaining to equipment from [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/) can be passed here." },
			{ name = "armorSlot", type = "tes3.armorSlot", optional = true, description = "The armor slot to unequip. Maps to values in [`tes3.armorSlot`](https://mwse.github.io/MWSE/references/armor-slots/) namespace." },
			{ name = "clothingSlot", type = "tes3.clothingSlot", optional = true, description = "The clothing slot to unequip. Maps to values in [`tes3.clothingSlot`](https://mwse.github.io/MWSE/references/clothing-slots/) namespace" },
		}
	}},
	returns = "itemUnequipped",
	valuetype = "boolean",
}