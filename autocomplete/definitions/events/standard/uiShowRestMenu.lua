return {
	description = "The uiShowRestMenu event is raised when the rest menu is about to be displayed. It allows the callback to change if sleeping is allowed, or to prevent the UI from showing at all.",
	eventData = {
		["allowRest"] = {
			type = "boolean",
			description = "true if the player is allowed to sleep (determined primarily from the cell flags), or false if they must wait instead. This value can be modified.",
		},
		["scripted"] = {
			type = "boolean",
			readOnly = true,
			description = "This value is true when the menu is shown from using the ShowRestMenu or equivalent function.",
		},
	},
}