return {
	type = "value",
	description = [[WARNING: Only used for debugging purposes. Enabling this without knowing what you're doing WILL lead to memory leaks. When enabled, NetImmerse objects will use the same lua object caching system that TES3 objects use, allowing them to be string-compared or used as table keys.]],
	valuetype = "boolean",
	default = false,
}
