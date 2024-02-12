return {
	type = "function",
	description = [[This function opens the magic select menu, which lets the player select a spell or enchanted item. This is originally used by the quick key menu. The spells or enchanted items are taken from the player's spell list and inventory. The selected spell or item can be retrieved in the function assigned to `callback`.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{
				name = "title",
				type = "string",
				description = "The text used for the title of the magic select menu."
			},
			{
				name = "selectSpells",
				type = "boolean",
				optional = true,
				default = "true",
				description = "If spells are included in the selection list."
			},
			{
				name = "selectPowers",
				type = "boolean",
				optional = true,
				default = "true",
				description = "If powers are included in the selection list."
			},
			{
				name = "selectEnchanted",
				type = "boolean",
				optional = true,
				default = "true",
				description = "If enchanted items are included in the selection list."
			},
			{
				name = "callback",
				type = "fun(params: tes3ui.showMagicSelectMenu.callbackParams)",
				optional = true,
				description = [[A function which will be called once the magic select menu has been closed, including when no item or spell has been selected. A table `callbackParams` will be passed to this function.
		- `callbackParams` (table)
			- `spell` ([tes3spell](https://mwse.github.io/MWSE/types/tes3spell/)): The spell or power that has been selected. Can be `nil`.
			- `item` ([tes3item](https://mwse.github.io/MWSE/types/tes3item/)): The enchanted item that has been selected. The actual magic will be `item.enchantment`. Can be `nil`.
			- `itemData` ([tes3itemData](https://mwse.github.io/MWSE/types/tes3itemData/)): The item data of the enchanted item that has been selected. Fully recharged items may not have itemData. Can be `nil`.
]],
			},
		},
	}},
}
