return {
	type = "value",
	description = [[The current amount of gold that the actor has access to for bartering.

Barter gold is reset on talking to an actor if fBarterGoldResetDelay hours have passed since the last transaction. The base value is held in `tes3npc.barterGold`, which is the base object and not an instance.]],
	valuetype = "number",
}