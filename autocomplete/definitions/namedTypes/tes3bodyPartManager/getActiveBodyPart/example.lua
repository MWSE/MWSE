local slot = tes3.player1stPerson.bodyPartManager:getActiveBodyPart(
	-- armor layer also contains weapons
	tes3.activeBodyPartLayer.armor,
	tes3.activeBodyPart.weapon
)
