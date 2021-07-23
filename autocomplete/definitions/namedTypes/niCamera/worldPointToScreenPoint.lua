return {
	type = "method",
	description = [[Given a world coordinate position, calculates where that point relates to the screen space.]],
	arguments = {
		{ name = "point", type = "tes3vector3", description = "The world position to calculate screen position for." },
	},
	returns = {{ name = "screenPosition", type = "tes3vector2", description = "The screen space position. Screen space is measured as ([-screenWidth/2, screenWidth/2], [-screenHeight/2, screenHeight/2]) with up-right being positive and an origin at the center of the screen." }},
}