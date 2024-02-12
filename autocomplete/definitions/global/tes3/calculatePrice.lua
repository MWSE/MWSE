return {
	type = "function",
	description = [[Calculates a price, given a merchant and associated trading data. This is useful beyond accessing the object's `.value` field in that it raises the appropriate events to let other mods modify the values.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "object", type = "tes3object", optional = true, description = "The object to calculate the price for. If not provided, `basePrice` is required." },
			{ name = "basePrice", type = "number", optional = true, description = "The base price to calculate the end price for. This defaults to the `object` param's `value`, if provided. This parameter is required if `object` is not provided." },
			{ name = "buying", type = "boolean", optional = true, default = true, description = "If `true`, uses the logic for buying a service/item. This is exclusive with `selling`." },
			{ name = "selling", type = "boolean", optional = true, default = false, description = "If `true`, uses the logic for selling an item. This is exclusive with `buying`." },
			{ name = "merchant", type = "tes3mobileActor", description = "The merchant to use for calculating the price." },
			{ name = "bartering", type = "boolean", optional = true, default = false, description = "If `true`, a [calcBarterPrice](https://mwse.github.io/MWSE/events/calcBarterPrice) or [calcRepairPrice](https://mwse.github.io/MWSE/events/calcRepairPrice) event will be triggered." },
			{ name = "repairing", type = "boolean", optional = true, default = false, description = "If `true`, a [calcRepairPrice](https://mwse.github.io/MWSE/events/calcRepairPrice) event will be triggered." },
			{ name = "training", type = "boolean", optional = true, default = false, description = "If `true`, a [calcTrainingPrice](https://mwse.github.io/MWSE/events/calcTrainingPrice) event will be triggered, passing the given `skill` ID." },
			{ name = "count", type = "number", optional = true, default = 1, description = "If `bartering`, the count passed to the [calcBarterPrice](https://mwse.github.io/MWSE/events/calcBarterPrice) event." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "If `bartering` or `repairing`, the item data passed to the [calcBarterPrice](https://mwse.github.io/MWSE/events/calcBarterPrice) or [calcRepairPrice](https://mwse.github.io/MWSE/events/calcRepairPrice) event." },
			{ name = "skill", type = "tes3.skill|integer", optional = true, description = "If `training`, the skill ID passed to the [calcTrainingPrice](https://mwse.github.io/MWSE/events/calcTrainingPrice) event. Maps to values in [`tes3.skill`](https://mwse.github.io/MWSE/references/skills/) table." },
		},
	}},
	returns = {
		{ name = "price", type = "number", description = "The calculated price, filtered by events." },
	},
}
