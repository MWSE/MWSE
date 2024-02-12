return {
	type = "method",
	description = [[Registers an event handler on each given UI element for the `tes3.uiEvent.mouseOver` and `tes3.uiEvent.mouseLeave` that will trigger "MCM:MouseOver" event. That event is used by the MCM to update the sidebar on the [mwseMCMSideBarPage](https://mwse.github.io/MWSE/types/mwseMCMSideBarPage/).]],
	arguments = {{
		name = "mouseOverList",
		type = "tes3uiElement[]",
		optional = true,
		description = "If this argument isn't passed, does nothing.",
	}},
}
