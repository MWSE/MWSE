return {
	type = "function",
	description = [[Performs a ray test and returns various information related to the result(s). If findAll is set, the result will be a table of results, otherwise only the first result is returned.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "position", type = "tes3vector3|table", description = "Position of the ray origin." },
			{ name = "direction", type = "tes3vector3|table", description = "Direction of the ray. Does not have to be unit length." },
			{ name = "findAll", type = "boolean", default = false, description = "If true, the ray test won't stop after the first result." },
			{ name = "maxDistance", type = "number", optional = true, description = "The maximum distance that the test will run." },
			{ name = "sort", type = "boolean", default = true, description = "If true, the results will be sorted by distance from the origin position." },
			{ name = "useModelBounds", type = "boolean", default = false, description = "If true, model bounds will be tested for intersection. Otherwise triangles will be used." },
			{ name = "useModelCoordinates", type = "boolean", default = false, description = "If true, model coordinates will be used instead of world coordinates." },
			{ name = "useBackTriangles", type = "boolean", default = false, description = "Include intersections with back-facing triangles." },
			{ name = "observeAppCullFlag", type = "boolean", default = true, description = "Ignore intersections with culled (hidden) models." },
			{ name = "root", type = "node*", default = "tes3.game.worldSceneGraphRoot", description = "Node pointer to node scene."},
			{ name = "ignoreSkinned", type = "boolean", default = false, description = "Ignore results from skinned objects."},
			{ name = "returnColor", type = "boolean", default = false, description = "Calculate and return the vertex color at intersections." },
			{ name = "returnNormal", type = "boolean", default = true, description = "Calculate and return the vertex normal at intersections." },
			{ name = "returnSmoothNormal", type = "boolean", default = false, description = "Use normal interpolation for calculating vertex normals." },
			{ name = "returnTexture", type = "boolean", default = false, description = "Calculate and return the texture coordinate at intersections." },
			{ name = "ignore", type = "table", description = "An array of references and/or scene graph nodes to cull from the result(s).", optional = true },
		},
	}},
	examples = {
		["GetActivationTarget"] = {
			title = "Get Activation Target",
			description = "This example performs a ray test to match the normal activation target test. Unlike ``tes3.getPlayerTarget()`` this will return objects not normally available for activation.",
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
	returns = "result",
	valuetype = "niPickRecord|table",
}
