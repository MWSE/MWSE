return {
	type = "method",
	description = [[Tells the renderer that the object has changed. Should be called after you have finished changing any vertex data.

If you have altered vertex positions, you may need to also call `updateModelBound`_. You should call it if vertices have been moved outside the bounds of the original model.]],
}