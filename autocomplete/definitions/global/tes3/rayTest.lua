return {
	type = "function",
	description = [[Performs a ray test and returns various information related to the result(s). If `findAll` is set, the result will be a table of results, otherwise only the first result is returned.

!!! tip Improving performance of rayTest

	1. Keep maximum size of objects reasonable, as well as triangle counts
	2. Whenever possible set a maxDistance in your rayTest calls
	3. Keep a cached table of ignored objects that you pass to rayTest
	4. Whenever possible call ray test on only a subset of the game's scene graph. It can be `worldPickRoot` for interactable objects, `worldLandscapeRoot`, or `worldObjectRoot` for other static, non-interactable objects. You could even pass a smaller subset of the scene graph with a different `NiNode` you aquired yourself. If your mod's logic only needs specific things you can narrow it down for big performance improvement.
]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "position", type = "tes3vector3|number[]", description = "Position of the ray origin." },
			{ name = "direction", type = "tes3vector3|number[]", description = "Direction of the ray. Does not have to be unit length." },
			{ name = "findAll", type = "boolean", optional = true, default = false, description = "If true, the ray test won't stop after the first result." },
			{ name = "maxDistance", type = "number", optional = true, default = 0, description = "The maximum distance that the test will run." },
			{ name = "sort", type = "boolean", optional = true, default = true, description = "If true, the results will be sorted by distance from the origin position." },
			{ name = "useModelBounds", type = "boolean", optional = true, default = false, description = "If true, model bounds will be tested for intersection. Otherwise triangles will be used." },
			{ name = "useModelCoordinates", type = "boolean", optional = true, default = false, description = "If true, model coordinates will be used instead of world coordinates." },
			{ name = "useBackTriangles", type = "boolean", optional = true, default = false, description = "Include intersections with back-facing triangles." },
			{ name = "observeAppCullFlag", type = "boolean", optional = true, default = true, description = "Ignore intersections with culled (hidden) models." },
			{ name = "root", type = "niNode", optional = true, default = "tes3.game.worldRoot", description = "Node pointer to node scene. To reduce the computational work, consider passing only a smaller subset of the `worldRoot` to improve performance. The typical nodes you can pass here are: [`tes3.game.worldLandscapeRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldLandscapeRoot), [`worldObjectRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldObjectRoot), and [`worldPickRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldPickRoot)."},
			{ name = "returnColor", type = "boolean", optional = true, default = false, description = "Calculate and return the vertex color at intersections." },
			{ name = "returnNormal", type = "boolean", optional = true, default = false, description = "Calculate and return the vertex normal at intersections." },
			{ name = "returnSmoothNormal", type = "boolean", optional = true, default = false, description = "Use normal interpolation for calculating vertex normals." },
			{ name = "returnTexture", type = "boolean", optional = true, default = false, description = "Calculate and return the texture coordinate at intersections." },
			{ name = "ignore", type = "table<integer, niNode|tes3reference>", description = "An array of references and/or scene graph nodes to cull from the result(s). In most cases when testing from the camera position, it's desirable to pass `ignore = { tes3.player }`.", optional = true },
			{ name = "accurateSkinned", type = "boolean", default = false, optional = true, description = "If true, the raytest will deform skinned objects to accurately raytest against them. This significantly slows down the operation." },
		},
	}},
	examples = {
		["GetActivationTarget"] = {
			title = "Get Activation Target",
			description = "This example performs a ray test to match the normal activation target test. Unlike `tes3.getPlayerTarget()` this will return objects not normally available for activation.",
		},
		["GetCameraTarget"] = {
			title = "Get Camera Target",
			description = "This example performs a ray test to see what the camera is currently looking at.",
		},
		["MultipleResults"] = {
			title = "Multiple Results",
			description = "This example performs a ray test and displays all results in the entire ray test, rather than ending at the first object hit.",
		},
		["rayTestForLater"] = {
			title = "Save rayTest result for use at a later point",
			description = "If you plan to use the results of rayTest, you should make sure it still exists. For example, an object which was in a list of results of rayTest can get unloaded when the player changes cells and become invalid, so it shouldn't be accessed.",
		},
	},
	returns = { name = "result", type = "niPickRecord|niPickRecord[]|nil" },
}
