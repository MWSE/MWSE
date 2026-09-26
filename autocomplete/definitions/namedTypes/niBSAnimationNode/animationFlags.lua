return {
	type = "value",
	description = [[A bit field of animation flags. The engine sets and clears these flags while the game runs. NIF files do not contain these flags.

To change one flag, use `bit.bor` or `bit.band`. Do not assign a new value to the full field, because that also changes the flags of the engine. Usually, the always update flag is the only flag that a mod must set. The engine controls the other flags.

- `0x2` (first time): The engine sets this flag when it creates the node. The first update of the node always updates the controllers. Then the engine clears this flag.
- `0x4` (managed): The node is in the list of the nearest `niBSAnimationManager` above it in the scene graph. The engine sets this flag only for nodes that have the animated flag (`0x20` in `flags`).
- `0x8` (displayed): The engine drew the node in the last frame. The engine sets this flag when it draws the node. The next update clears this flag.
- `0x10` (always update): The engine updates the controllers of the node and of its children in each frame.

If the always update flag is clear, the engine updates the controllers of the node and of its children only when the displayed flag is set. Thus, a node that the engine does not draw does not animate.

This is important for particle systems. A particle system has no particles until its controller runs. If the engine does not draw the particle system, its controller does not run. The particle system then stays empty, and you cannot see it.

When the engine launches a spell projectile, it sets the always update flag on all animation nodes of the projectile. If you attach a mesh to a projectile after the launch, for example in the `mobileActivated` event, set the always update flag on the animation nodes of that mesh.]],
	valuetype = "integer",
	examples = {
		["alwaysUpdateProjectileMesh"] = {
			title = "Replacing the mesh of a spell projectile",
		},
	},
}
