return {
	type = "value",
	description = [[A function which will be called when the menu is closed. Useful for saving your config after exiting the MCM.

Use `template:saveOnClose(configFilename, configTable)` to assign a simple save function to onClose. If you want to do more on closing, assign a custom function to `onClose` and call `mwse.saveConfig(configFilename, configTable)` when you want to save.
]],
	valuetype = "nil|fun(modConfigContainer: tes3uiElement)",
}
