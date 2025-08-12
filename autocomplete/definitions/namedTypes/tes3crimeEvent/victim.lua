return {
	type = "value",
	description = [[The player mobile will be used as a default value, meaning that there is no victim.

If the default value is set, the proper victim can be retrieved from the stolenFrom field for crimes of type `tes3.crimeType.theft` or `tes3.crimeType.pickpocket`.

For crimes of type `tes3.crimeType.witnessReaction` the victim will always be the player mobile.]],
	valuetype = "tes3mobileActor",
}