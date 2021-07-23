--- @meta

--- A mobile object for a spell projectile.
--- @class tes3mobileSpellProjectile : tes3mobileProjectile, tes3mobileObject
--- @field rotationSpeed number The angular speed that the projectile rotates around its local Y axis.
tes3mobileSpellProjectile = {}

--- Detonates the projectile, as if it hit a static object. The effects of detonation are not applied immediately, but later in the frame, at the same time as projectile collisions. This ensures consistent behaviour of the magic system.
function tes3mobileSpellProjectile:explode() end

