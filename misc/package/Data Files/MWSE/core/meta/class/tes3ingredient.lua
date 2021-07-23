--- @meta

--- An ingredient game object.
--- @class tes3ingredient : tes3physicalObject, tes3object, tes3baseObject
--- @field effectAttributeIds table An array-style table access the attributes associated with the effects.
--- @field effects table An array-style table access to the four ingredient effects. Unlike alchemy or enchanting objects, these are simple numbers representing the effect ID.
--- @field effectsSkillIds table An array-style table access the skills associated with the effects.
--- @field icon string The path to the object's icon.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field script tes3script The script that runs on the object.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
tes3ingredient = {}

