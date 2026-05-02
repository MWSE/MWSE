return {
	type = "function",
	description = [[Fetches the first reference for a given base object ID. It will find the first clone if the object is an actor. It will scan every cell's references for a match, so performance must be considered when using this.

!!!note
	This is a slow operation, so ideally a reference should be looked up once on game load. Use a safe object handle to store references.]],
	arguments = {
		{ name = "id", optional = true, type = "string", description = [[Passing "player" or "playersavegame" will return the player reference.]] }
	},
	returns = {{ name = "reference", type = "tes3reference" }}
}