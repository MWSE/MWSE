--- @meta

--- A mobile object for a physical projectile.
--- @class tes3mobileProjectile : tes3mobileObject
--- @field animTime number The time index for the projectile model's animation controllers.
--- @field attackSwing number The attack swing value of the attacker at the time of releasing the projectile.
--- @field damage number Access to the raw damage that this projectile will inflict.
--- @field expire number A flag that causes the projectile to expire when set to 1.
--- @field firingMobile tes3mobileActor The mobile that fired this projectile.
--- @field firingWeapon tes3weapon The weapon that fired this projectile.
--- @field initialSpeed number The initial speed that the projectile was launched at.
--- @field velocity tes3vector3 Access to the current velocity of the projectile.
tes3mobileProjectile = {}

