return {
    type = "method",
	description = "Retrieves the text that this component should display in any related [`mouseOverInfo`s](./mwseMCMMouseOverInfo.md). \z
        This method currently utilized to display this component's description whenever the component is in a [`SideBarPage`](./mwseMCMSideBarPage.md).\n\n\z
        Primarily intended for internal use.",
	returns = {{
		name = "text", type = "string|nil", description = "The text to display. \z
            Returning `nil` means that the `mouseOverInfo` should display text from a different source. \z
            e.g. from the `description` of the relevant [`SideBarPage`](./mwseMCMSideBarPage.md)."
	}}
}