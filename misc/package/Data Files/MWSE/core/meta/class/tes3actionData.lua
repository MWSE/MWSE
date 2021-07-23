--- @meta

--- A substructure of mobile actors that provides information about the current or previous action.
--- @class tes3actionData
--- @field aiBehaviorState number The behavior state of the tes3actionData.
--- @field animationAttackState number No description yet available.
--- @field attackDirection number **Deprecated. Use tes3actionData.physicalAttackType instead.** When attacking, the direction swung with the weapon. This shows if the actor was thrusting, swinging, or chopping.
--- @field attackSwing number When attacking, this value represents how much the weapon has been pulled back. The value ranges from [0.0 - 1.0].
--- @field currentAnimationGroup number No description yet available.
--- @field hitTarget tes3mobileActor No description yet available.
--- @field lastBarterHoursPassed number No description yet available.
--- @field nockedProjectile tes3weapon The currently knocked projectile the associated actor is using.
--- @field physicalAttackType number A number from the tes3.physicalAttackType enumeration identifying the physical attack type. Can be tes3.physicalAttackType.slash, .chop, .thrust, .projectile, .creature1, .creature2, or .creature3.
--- @field physicalDamage number When attacking, this is the value of the weapon damage that was rolled. This value doesn't take into account the actor's strength, or other additional damage.
--- @field stolenFrom tes3object No description yet available.
--- @field target tes3mobileActor No description yet available.
--- @field walkDestination tes3vector3 If moving to a location, this is the position to be walked to.
tes3actionData = {}

