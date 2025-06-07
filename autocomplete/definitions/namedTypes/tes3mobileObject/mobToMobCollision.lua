return {
	type = "value",
	description = [[Allows modifying if this mobile will collide with other mobiles (actors and projectiles). When `true` (default), the actor cannot move through other actors, and projectiles will collide with actors. When `false`, the actor is allowed to move through other actors, and other actors can move through it. Projectiles will pass through actors and other projectiles.

May be useful when free movement is required in crowded situations, or to temporarily let the player move past an actor.]],
	valuetype = "boolean",
}