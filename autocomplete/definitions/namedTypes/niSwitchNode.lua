return {
	type = "class",
	description = [[Represents groups of multiple scene graph subtrees, only one of which is visible at any given time. They are useful for showing different states of a model depending on engine / lua logic. If you detach the active subtree, the switch node will set the active subtree to none, or to an index of -1.]],
	inherits = "niNode",
}