return {
	description = "This event is invoked after a UI menu has been created or recreated. This can be useful for when important game subcomponents are destroyed and remade, without the entire UI being activated.",
	eventData = {
		["element"] = {
			type = "tes3uiElement",
			readOnly = true,
			description = "The menu element that was recreated or refreshed. The event is filtered on element.name",
		},
		["newlyCreated"] = {
			type = "boolean",
			readOnly = true,
			description = "true if the menu was created for the first time or destroyed and re-created; false if it was made visible after being hidden. This can be used when adding custom elements to a menu, as elements previously added will still exist if the menu was hidden. If the menu was re-created, the elements will need to be added again.",
		},
	},
	filter = "element.name",
}