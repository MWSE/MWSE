return {
	type = "function",
	description = [[Controls the magic activation of equipped constant effect items on actors. The game is not very consistent in the activation on constant effect magic on non-player actors. It will activate them on equipping, and on combat start, but does not do this at other times, like cell change. This function allows control over this part of the magic system. It is designed for non-players, and is not recommend to use on the player.

One of `activate` or `deactivate` must be true. Only constant effects on equipped items are considered. `activate` will start constant effects, which will take effect on the next frame. `deactivate` immediately removes constant effects. Activating or deactivating multiple times will not cause stacking problems.]],
	arguments = {{
        name = "params",
        type = "table",
        tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "The actor reference." },
			{ name = "activate", type = "boolean", optional = true, default = false, description = "Activate constant effects on equipped items." },
			{ name = "deactivate", type = "boolean", optional = true, default = false, description = "Deactivate constant effects on equipped items." },
        }
    }},
}
