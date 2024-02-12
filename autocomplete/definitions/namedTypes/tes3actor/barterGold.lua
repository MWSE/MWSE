return {
	type = "value",
	description = [[The actor's base barter gold amount. This is the amount of the barter gold an actor initially has, and also when barter gold is refreshed. The actor's current barter gold amount is held in `tes3mobileActor.barterGold`.

Barter gold is reset on talking to an actor if fBarterGoldResetDelay hours have passed since the last transaction. If you want to change the base amount, for example in an investment mod, you must edit the barterGold of the baseObject.]],
	valuetype = "number",
}
