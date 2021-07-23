--- @meta

--- A collection that can be iterated over Contains items in a simple linked list, and stores its head/tail.
---
--- [Examples available in online documentation](https://mwse.readthedocs.io/en/latest/lua/tes3iterator.html).
--- @class tes3iterator
--- @field current tes3iteratorNode A reference for the currently iterated node. This is used by the core game engine, but should not be accessed from lua.
--- @field head tes3iteratorNode The first node in the collection.
--- @field size number The amount of items in the iterator.
--- @field tail tes3iteratorNode The last node in the collection.
tes3iterator = {}

