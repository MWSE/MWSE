return {
	type = "method",
	description = [[Damages the actor, with options to control mitigation and difficulty scaling. Invokes the 'damage' and 'damaged' events, with 'script' source. Returns the actual damage done after armor mitigation and resistance, but before difficulty scaling.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "damage", type = "number", description = "The amount of damage to apply." },
			{ name = "applyArmor", type = "boolean", optional = true, description = "If armor should mitigate the incoming damage. If the player is the target, armor experience will be gained." },
			{ name = "resistAttribute", type = "number", optional = true, description = "The resistance attribute that is applied to the damage. It can reduce damage or exploit weakness. Uses values from tes3.effectAttributes." },
			{ name = "applyDifficulty", type = "boolean", optional = true, description = "If the game difficulty modifier should be applied. Must be used with the 'playerAttack' argument to apply the correct modifier." },
			{ name = "playerAttack", type = "boolean", optional = true, description = "If the attack came from the player. Used for difficulty calculation." },
			{ name = "doNotChangeHealth", type = "boolean", optional = true, description = "If all armor effects except the health change should be applied. These include hit sounds, armor condition damage, and player experience gain from being hit." },
		},
	}},
	valuetype = "number",
}