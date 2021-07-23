--- @meta

--- The base object from which all other mobiles (AI/movement using) structures derive.
--- @class tes3mobileObject
--- @field boundSize tes3vector3 A vector that shows the size of the bounding box in each direction.
--- @field cellX number The X grid coordinate of the cell the mobile is in.
--- @field cellY number The Y grid coordinate of the cell the mobile is in.
--- @field flags number Access to the root mobile object flags, represented as an integer. Should not be accessed directly.
--- @field height number The height of the mobile above the ground.
--- @field impulseVelocity tes3vector3 A vector that represents the 3D acceleration of the object.
--- @field movementCollision boolean Controls if the mobile has movement collision active. When false, the mobile can move through any object, but can still block other mobiles, and can still be hit in combat. Actors will still follow pathgrids, ramps and stairs when navigating.
--- @field movementFlags number Access to the root mobile object movement flags, represented as an integer. Should not be accessed directly.
--- @field objectType number The type of mobile object. Maps to values in tes3.objectType.
--- @field playerDistance number The distance to the player. Updated every frame when the mobile is in an active cell.
--- @field position tes3vector3 A vector that represents the 3D position of the object.
--- @field prevMovementFlags number Access to the root mobile object movement flags from the previous frame, represented as an integer. Should not be accessed directly.
--- @field reference tes3reference Access to the reference object for the mobile, if any.
--- @field velocity tes3vector3 A vector that represents the 3D velocity of the object.
tes3mobileObject = {}

