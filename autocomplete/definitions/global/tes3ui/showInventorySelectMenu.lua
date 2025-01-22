return {
	type = "function",
	description = [[This function opens the inventory select menu which lets the player select items from an inventory. These items can be selected from any actor's inventory and can be filtered with the `filter` callback. The selected item can be retrieved in the function assigned to `callback`.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{
				name = "reference",
				type = "tes3reference",
				optional = true,
				default = "tes3player",
				description = "The reference of a `tes3actor` whose inventory will be used."
			},
			{
				name = "title",
				type = "string",
				description = "The text used for the title of the inventory select menu."
			},
			{
				name = "leaveMenuMode",
				type = "boolean",
				optional = true,
				description = "Determines if menu mode should be exited after closing the inventory select menu. By default, it will be in the state it was in before this function was called."
			},
			{
				name = "noResultsText",
				type = "string",
				optional = true,
				description = "The text used for the message that gets shown to the player if no items have been found in the inventory. The default text is equivalent to the `sInventorySelectNoItems` GMST value, unless `\"ingredients\"` or `\"soulGemFilled\"` has been assigned to `filter`, in which case the default text is equivalent to either the `sInventorySelectNoIngredients` or `sInventorySelectNoSoul` GMST value respectively."
			},
			{
				name = "noResultsCallback",
				type = "function",
				optional = true,
				description = "A function which is called when no items have been found in the inventory, right before the message containing `noResultsText` is shown."
			},
			{
				name = "filter",
				type = "string|fun(params: tes3ui.showInventorySelectMenu.filterParams): boolean",
				optional = true,
				description = [[This determines which items should be shown in the inventory select menu. Accepts either a string or a function.

		If assigning a string, the available values are present in [`tes3.inventorySelectFilter`](https://mwse.github.io/MWSE/references/inventory-select-filters/) namespace. The available filters are:

		- `alembic`: Only [tes3apparatus](https://mwse.github.io/MWSE/types/tes3apparatus/) items of type `tes3.apparatusType.alembic` will be shown.
		- `calcinator`: Only [tes3apparatus](https://mwse.github.io/MWSE/types/tes3apparatus/) items of type `tes3.apparatusType.calcinator` will be shown.
		- `enchanted`: Only non-enchanted items will be shown. That's because that filter is usually used in the enchanting menu to select items viable for enchanting.
		- `ingredients`: Only [tes3ingredient](https://mwse.github.io/MWSE/types/tes3ingredient/) items will be shown.
		- `mortar`: Only [tes3apparatus](https://mwse.github.io/MWSE/types/tes3apparatus/) items of type `tes3.apparatusType.mortarAndPestle` will be shown.
		- `quickUse`: Only items that can be assigned as quick keys will be shown.
		- `retort`: Only [tes3apparatus](https://mwse.github.io/MWSE/types/tes3apparatus/) items of type `tes3.apparatusType.retort` will be shown.
		- `soulGemFilled`: Only filled soulgems will be shown.

		If assigning a custom function it will be called when determining if an item should be added to the inventory select menu. Returning `true` from this function will add the item to the inventory select menu, whereas returning `false` will prevent it from being added.]],
			},
			{
				name = "callback",
				type = "fun(params: tes3ui.showInventorySelectMenu.callbackParams)",
				optional = true,
				description = [[A function which will be called once the inventory select menu has been closed, including when no item has been selected.]],
			},
		},
	}},
	examples = {
		["bribe"] = {
			title = [[Bribe an NPC with items from your inventory]],
			description = [[This code allows the player to give an item to the actor the player is currently looking at.]]
		},
		["filters"] = {
			title = [[Filter functions]],
			description = [[A few possible filtering functions.]]
		}
	}
}
