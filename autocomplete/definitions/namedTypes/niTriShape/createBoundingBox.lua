return {
	type = "method",
	description =
	[[Calculates and creates a bounding box for the object. The existing bounding box, if any, will not be used, a fresh one will always be calculated.]],
	arguments = { {
		name = "args",
		optional = true,
		type = "table",
		tableParams = {
			{ name = "accurateSkinned",    type = "boolean", optional = true, default = false, description = "If true, [`niSkinInstance`](http://mwse.github.io/MWSE/types/niSkinInstance/?h=niskininstan) deformations will be applied before calculating the bounding box. This has an additional performance cost" },
			{ name = "observeAppCullFlag", type = "boolean", optional = true, default = false, description = "If true, objects that have the [`appCulled`](http://mwse.github.io/MWSE/types/niAVObject/#appculled) flag set will be ignored." },
			{ name = "onlyActiveChildren", type = "boolean", optional = true, default = false, description = "If true, only the [`active children`](https://mwse.github.io/MWSE/types/niSwitchNode/#getactivechild) will be processed." },
		},
	} },
	returns = {
		{ name = "boundingBox", type = "tes3boundingBox", description = "The newly created bounding box." },
	},
}
