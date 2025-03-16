return {
	type = "value",
	description = [[Stores a default config that should be used by this mod's `Setting`s. This will initialize the `defaultSetting` field of any [`mwseMCMTableVariable`s](./mwseMCMTableVariable.md) created for this mod. Sub-configs can be accessed by passing a `configKey` to any `Category` that is nested inside this one.]],
	valuetype = "table|nil",
}
