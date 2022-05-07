--- @meta
--- @diagnostic disable:undefined-doc-name

--- A collection that can be iterated over Contains items in a simple linked list, and stores its head/tail.
---
--- [Examples available in online documentation](https://mwse.github.io/MWSE/types/tes3iterator).
--- @class tes3iterator
--- @field current tes3iteratorNode *Read-only*. A reference for the currently iterated node. This is used by the core game engine, but should not be accessed from lua.
--- @field head tes3iteratorNode *Read-only*. The first node in the collection.
--- @field tail tes3iteratorNode *Read-only*. The last node in the collection.
tes3iterator = {}

--- *Read-only*. Returns the amount of items in the iterator.
--- @return number result No description yet available.
function tes3iterator:size() end
