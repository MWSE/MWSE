return {
	type = "function",
	description = [[This function opens the resting menu and returns true on success. If the player can't rest currently, it returns false.

Various parameters can be used to allow resting in situations not normally possible.]],
	arguments = {{
		name = "params",
		type = "table",
		optional = true,
		tableParams = {
			{ name = "checkForEnemies", type = "boolean", optional = true, default = true, description = "Perform a check whether there are enemies nearby before opening rest menu. If there are, false is returned." },
			{ name = "checkForSolidGround", type = "boolean", optional = true, default = true, description = "Perform a check if the player is underwater. If underwater, false is returned." },
			{ name = "checkSleepingIllegal", type = "boolean", optional = true, default = true, description = "Perform a check if the sleeping in the current cell is illegal. If illegal, then the player will be prompted to wait instead of rest." },
			{ name = "checkIsWerewolf", type = "boolean", optional = true, default = true, description = "Perform a check if the player is Werewolf. If they are, then the player will be prompted to wait instead of rest." },
			{ name = "showMessage", type = "boolean", optional = true, default = true, description = "Should a messagebox be shown if the player can't open resting menu because some condition isn't met." },
			{ name = "resting", type = "boolean", optional = true, default = true, description = "Should this be a rest?" },
			{ name = "waiting", type = "boolean", optional = true, default = false, description = "Or, is this a wait?" },
		},
	}},
	returns = {{ name = "success", type = "boolean" }}
}
