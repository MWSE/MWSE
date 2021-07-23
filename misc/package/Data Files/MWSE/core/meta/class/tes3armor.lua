--- @meta

--- An armor game object.
--- @class tes3armor : tes3physicalObject, tes3object, tes3baseObject
--- @field armorRating number The armor's defensive rating.
--- @field enchantCapacity number The object's enchantment capacity.
--- @field enchantment tes3enchantment The enchantment used by the object.
--- @field icon string The path to the object's icon.
--- @field isLeftPart boolean Determines if the armor is the left part of a pair.
--- @field maxCondition number The object's maximum condition.
--- @field mesh string The path to the object's mesh.
--- @field name string The player-facing name for the object.
--- @field parts table An array-style table of the tes3wearablePart data on the object.
--- @field script tes3script The script that runs on the object.
--- @field slot number The slot used by the armor.
--- @field slotName string The name of the slot used by the armor.
--- @field value number The value of the object.
--- @field weight number The weight of the object.
--- @field weightClass number The weight class of the armor.
tes3armor = {}

--- Calculates what armor rating is provided for a given mobile actor.
--- @param mobile tes3mobileActor No description yet available.
function tes3armor:calculateArmorRating(mobile) end

