return {
	type = "value",
	description = [[The array-style access to input bindings. To index this array use [tes3.keybind](https://mwse.github.io/MWSE/references/keybinds/) constants increased by `1`. For example, to get the input binding for steal action, you would do: `inputController.inputMaps[tes3.keybind.sneak + 1]`.]],
	readOnly = true,
	valuetype = "tes3inputConfig[]",
}
