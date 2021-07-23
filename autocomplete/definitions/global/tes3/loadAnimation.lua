return {
	type = "function",
	description = [[Loads an animation and uses it to override existing animations on an actor. Animation groups present in the file will replace the actor's animation groups. The actor's model must be already loaded into memory to have its animations modified. The overridden animations only persist until the actor is unloaded.

Calling this more than once will remove the previous animation override before loading the new animation. Therefore, if applying animations to the player, you should call this before every playAnimation so that different animation mods can co-exist. For NPCs under your mod's control, you only need to do it when the player enters the cell containing the NPC.

Calling this without a file argument will reset the reference's animations to default.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3mobileActor|tes3reference|string", description = "The reference to the actor that is having its animations modified." },
			{ name = "file", type = "string", optional = true, description = "The specified animation base file that will be loaded. e.g. For an animation composed of files anim.nif, xanim.nif and xanim.kf, you should pass file=\"anim.nif\". The animation target skeleton _must_ match the skeleton of the actor to work. i.e. 3rd person anims for NPCs and tes3.player, 1st person anims for firstPersonReference." },
		},
	}},
}
