return {
	type = "value",
	description = [[Sets element position to a point relative to the parent element. 0.0 = left/top content edge, 1.0 = right/bottom content edge. The positioning is absolute, which frees the element from the standard flow layout and allows overlapping elements.
    
    Bug note: Elements may not respond to widthProportional/heightProportional sizing after either of these properties are set. If you need to use both you should consider testing if it works first.]],
	valuetype = "number",
}