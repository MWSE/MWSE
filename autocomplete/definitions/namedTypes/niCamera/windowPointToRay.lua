return {
	type = "method",
	description = [[Given a screen space position, calculates the world position and outlook direction. This can be useful when trying to find a reference under a UI element, such as the cusor.]],
	arguments = {
		{ name = "point", type = "tes3vector2", description = "The screen position to calculate a world ray for. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen." },
	},
	returns = {
		{ name = "origin", type = "tes3vector3", description = "The world point that the given screen position looks out from." },
		{ name = "direction", type = "tes3vector3", description = "The look direction of the camera from the given origin point." },
	},
}