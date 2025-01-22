return {
	type = "function",
	description = [[Performs a ray test and returns various information related to the result(s). The ray test works by effectively shooting out a line, starting at `position` and pointing towards `direction`, and then checking to see which objects intersect that line.
	
Here is an overview of how some commonly used parameters will alter how `tes3.rayTest` checks for collisions:
	
1. `root`: Things that aren't a `child` of the specified `root` will be skipped. If `root` is not provided, then nothing will be skipped by this process.
2. `ignore`: Objects in this array will be skipped.
3. `maxDistance`: If specified, only objects within the specified distance will be checked.
4. `findAll`: If `true`, then all intersections will be returned. Otherwise, only the first intersection will be returned.

!!! tip Improving performance of rayTest
	The performance of `tes3.rayTest` depends quite a bit on the parameters the function is called with.
	The following suggestions will help to minimize the performance impact of calls to `tes3.rayTest`.

	1. Set a `maxDistance`.
	2. Filter objects by using the `root` parameter. This will make the algorithm **much** faster, and can make it behave more predictably as well. If you're only checking for interactable objects (containers/actors/plants/etc), use `worldPickRoot`. If you're looing for static, non-interable objects, use `worldObjectRoot`. You could even pass a smaller subset of the scene graph with a different `NiNode` you aquired yourself.
	3. Try to keep a cached copy of the array used for the `ignore` parameter (if possible).
	4. Keep maximum size of objects reasonable, and try to limit triangle counts.
]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			-- most commonly used options
			{ name = "position", type = "tes3vector3|number[]", 
				description = "Position of the ray origin." 
			},
			{ name = "direction", type = "tes3vector3|number[]", 
				description = "Direction of the ray. Does not have to be unit length." 
			},
			{ name = "findAll", type = "boolean", optional = true, default = false, 
				description = "If true, the ray test won't stop after the first result." 
			},
			{ name = "maxDistance", type = "number", optional = true, default = 0, 
				description = "The maximum distance that the test will run. If set to `0`, no maximum distance will be used." 
			},
			{ name = "ignore", type = "table<integer, niNode|tes3reference>", 
				description = "An array of references and/or scene graph nodes to cull from the result(s).", optional = true 
			},
			{ name = "root", type = "niNode", optional = true, default = "tes3.game.worldRoot", 
				description = "Node pointer to node scene. Only nodes that are a child of this root will be checked by this function. This option can considerably increase performance if used properly. \z
					Common choices for the root node are: \z
						[`tes3.game.worldLandscapeRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldLandscapeRoot), \z
						[`worldObjectRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldObjectRoot) (for most static objects), and \z
						[`worldPickRoot`](https://mwse.github.io/MWSE/types/tes3game/#worldPickRoot) (for containers, NPCs, plants, doors, etc)."
			},

			-- more niche options related to collision checking
			{ name = "useModelBounds", type = "boolean", optional = true, default = false, 
				description = "If `true`, model bounds will be tested for intersection. Otherwise triangles will be used. \z
					This will result in more accurate collision testing, but will be more computationally expensive. This is rarely needed." 
			},
			{ name = "useModelCoordinates", type = "boolean", optional = true, default = false, 
				description = "If true, model coordinates will be used instead of world coordinates. Typically not needed." 
			},
			{ name = "useBackTriangles", type = "boolean", optional = true, default = false, 
				description = "Include intersections with back-facing triangles. \z
					This essentially makes it possible to intersect with the \"back-side\" of an object, \z
						which could make it possible to return a hit on an object if the `position` parameter is \"inside\" the object in question.\z
					This will result in more accurate collision testing, but will be more computationally expensive. This is rarely needed." 
			},
			{ name = "observeAppCullFlag", type = "boolean", optional = true, default = true, 
				description = "Ignore intersections with culled (hidden) models." 
			},
			{ name = "accurateSkinned", type = "boolean", default = false, optional = true, 
				description = "If `true`, skinned objects will be deformed, allowing for more accurate collision checking. \z
					This **significantly** slows down the operation, and is rarely needed." 
			},

			-- more niche options that alter how values are returned
			{ name = "sort", type = "boolean", optional = true, default = true, 
				description = "Sort results by distance from the specified `position`? Only applicable if `findAll == true`." 
			},
			{ name = "returnColor", type = "boolean", optional = true, default = false, 
				description = "Calculate and return the vertex color at intersections?" 
			},
			{ name = "returnNormal", type = "boolean", optional = true, default = false, 
				description = "Calculate and return the vertex normal at intersections?" 
			},
			{ name = "returnSmoothNormal", type = "boolean", optional = true, default = false, 
				description = "Use normal interpolation for calculating vertex normals?" 
			},
			{ name = "returnTexture", type = "boolean", optional = true, default = false, 
				description = "Calculate and return the texture coordinate at intersections?" 
			},
		},
	}},
	examples = {
		["GetActivationTarget"] = {
			title = "Get Activation Target",
			description = "This example performs a `tes3.rayTest` to match the normal activation target test. Unlike `tes3.getPlayerTarget()` this will return objects not normally available for activation.",
		},
		["GetCameraTarget"] = {
			title = "Get Camera Target",
			description = "See what the player's camera is looking at.",
		},
		["MultipleResults"] = {
			title = "Multiple Results",
			description = "This example performs a `tes3.rayTest` and displays all results in the entire ray test, rather than ending at the first object hit.",
		},
		["rayTestForLater"] = {
			title = "Save `tes3.rayTest` result for use at a later point",
			description = "If you plan to use the results of rayTest, you should make sure it still exists. For example, an object which was in a list of results of rayTest can get unloaded when the player changes cells and become invalid, so it shouldn't be accessed.",
		},
	},
	returns = { name = "result", type = "niPickRecord|niPickRecord[]|nil" },
}
