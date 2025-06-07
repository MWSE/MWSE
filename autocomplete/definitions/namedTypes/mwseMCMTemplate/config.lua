return {
	type = "value",
	description = [[Stores a config that should be used by this mod's `Setting`s. Sub-configs can be accessed by passing a `configKey` to any `Page`s nested inside this template. If provided, this config will be used to generate [`mwseMCMTableVariable`s](./mwseMCMTableVariable.md) for any [`mwseMCMSetting`s](./mwseMCMSetting.md) made inside this template.]],
	valuetype = "table|nil",
}
