return {
	type = "class",
	description = [[Item stack represents all copies of an item with the same id inside an inventory. This complex container holds a relationship between an item, and zero or more associated item datas. The `itemStack.variables` is a list of different itemData for each item in the stack, not a single itemData. Not every item in the stack needs to have associated itemData. The game adds itemData to items on equipping with the exception of thrown weapons and ammo.

For example, you might have five journeyman lockpicks:

- 3 new ones (25 uses)
- 1 with 23 uses
- 1 with 18 uses

In this example, all of these lockpicks are represented by a single tes3itemStack object. The `stack.count` is 5. The `#stack.variables` is 2, since there are only 2 used lockpicks, with each having different itemData. The count of new lockpicks is equal to `stack.count - #stack.variables`.
]],
	examples = {
		["..\\..\\tes3inventory\\items\\iteration"] = {
			title = "In the iterItems() function we can see that the an item stack can consist of items with itemData and items without it"
		}
	}
}