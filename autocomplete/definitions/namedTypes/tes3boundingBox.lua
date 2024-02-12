return {
	type = "class",
	description = [[A pair of vectors marking a bounding box.
	
!!! warning "Bounding boxes will always be centered at the world origin."
	i.e., their midpoint will always be `(0,0,0)`. You may want to offset the `min` and `max` of the bounding box by a specified position before using them.
]],
}