--- @meta

--- A complex container that holds a relationship between an item, and zero or more associated item datas.
--- @class tes3itemStack
--- @field count number The total number of items in the stack.
--- @field object tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3weapon The core game object that the stack represents.
--- @field variables tes3TArray A collection of variables that are associated with the stack's object, or nil if there aren't any.
tes3itemStack = {}

