return {
	type = "method",
	description = [[Used in a for loop, iterates over objects in the cell.

!!! note
	This iterator will also yield disabled and deleted references.
]],
	arguments = {
		{ name = "filter", type = "integer|integer[]", description = "The TES3 object type to filter results by. If you need multiple filters, just pass them as a table, e.g. `{ tes3.objectType.npc, tes3.objectType.creature }`. Those are stored in [`tes3.objectType`](https://mwse.github.io/MWSE/references/object-types/) namespace.", optional = true },
	},
	returns = {
		name = "iterator",
		type = "fun(): tes3reference",
	}
}