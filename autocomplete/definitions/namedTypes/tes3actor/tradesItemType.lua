return {
	type = "method",
	description = [[Returns true if the actor trades in given item.]],
	arguments = {
		{ name = "type", type = "tes3.objectType constants", description = [[Type can only be one of the following:
		tes3.objectType.alchemy
		tes3.objectType.ammunition
		tes3.objectType.apparatus
		tes3.objectType.armor
		tes3.objectType.book
		tes3.objectType.clothing
		tes3.objectType.ingredient
		tes3.objectType.light
		tes3.objectType.lockpick
		tes3.objectType.miscItem
		tes3.objectType.probe
		tes3.objectType.repairItem
		tes3.objectType.weapon
		]] },
	},
	returns = "tradesItemType",
	valuetype = "boolean",
}
