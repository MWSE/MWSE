--- @meta

--- A simple container that holds a relationship between an item, and any associated item data.
--- @class tes3equipmentStack
--- @field itemData tes3itemData Item data associated with the stack's object, if any. All equipped items except ammunition will have itemData.
--- @field object tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3weapon The core game object that the stack represents.
--- @field variables tes3itemData Variables that are associated with the stack's object, if any. Another alias for itemData.
tes3equipmentStack = {}

