--- @meta
--- @diagnostic disable:undefined-doc-name

--- This event is raised when an in-world `tes3reference` for an item is about to be converted to fit into a `tes3itemStack`. Note that reference.itemData.owner is always nil, as the game clears it earlier when evaluating theft mechanics.
--- @class convertReferenceToItemEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field reference tes3reference *Read-only*. The reference about to be converted.