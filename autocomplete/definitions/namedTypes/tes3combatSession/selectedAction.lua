return {
	type = "value",
	description = [[The current combat action, until the next AI decision. This can be changed to affect what the AI is doing in the short term. From observed behavior, this roughly maps to:

Value | Behavior
----- | ---------
0     | Undecided
1     | Use melee weapon
2     | Use marksman weapon
3     | Use hand to hand attacks
4     | Use on-touch offensive spell
5     | Use on-target offensive spell
6     | Use summon spell
7     | Flee
8     | Cast on-self empowering spell (For example, Ancestor Guardian)
9     | Use alchemy item
10    | Use enchanted item
]],
	valuetype = "integer",
}