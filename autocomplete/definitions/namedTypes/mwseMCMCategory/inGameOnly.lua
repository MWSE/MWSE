return {
	type = "value",
	description = [[If `true`, then this component, as well as any nested components, will only be shown when in game. i.e., after a save has been loaded.
If `false` or `nil`, then this component will be hidden if all subcomponents are disabled (e.g., if all subcomponents have `inGameOnly == true` and a save hasn't been loaded).]],
	valuetype = "boolean",
}
