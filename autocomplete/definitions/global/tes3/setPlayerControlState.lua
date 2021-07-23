return {
	type = "function",
	description = [[Enables or disables player's controls state.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "enabled", type = "boolean", optional = true, default = false, description = "Setting this to false will disable any kind of control." },
			{ name = "attack", type = "boolean", optional = true, default = false, description = "If this is false, it will block player from attacking." },
			{ name = "jumping", type = "boolean", optional = true, default = false, description = "If this is false, it will block player from jumping." },
			{ name = "magic", type = "boolean", optional = true, default = false, description = "If this is false, it will block player from using magic." },
			{ name = "vanity", type = "boolean", optional = true, default = false, description = "If this is false, it will block player from going to vanity mode." },
			{ name = "viewSwitch", type = "boolean", optional = true, default = false, description = "If this is false, it will block player changing view mod from 1st to 3rd person camera and vice versa." },
		},
	}},
	returns = "changedControlState",
	valuetype = "boolean",
}
