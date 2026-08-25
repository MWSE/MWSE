return {
	type = "function",
	description = [[Returns counters for the physics optimizations toggled by the "Enable physics optimizations" MWSE option: whether they are active, and the size of the cached per-mesh bounding volume hierarchies that accelerate raytests and swept collision tests. Intended for measuring memory use and cache churn while testing.]],
	returns = {
		{ name = "stats", type = "table", description = "A table with the fields `enabled` (boolean), `meshCacheEntries` (number of meshes with a cached hierarchy), `meshCacheBytes` (memory held by those hierarchies), `meshCacheBuilds` (hierarchies built since startup), and `meshCacheEvictions` (hierarchies released with their mesh since startup)." },
	},
}
