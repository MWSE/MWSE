--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This event is triggered when a keyframes file is loaded.
--- @class keyframesLoadedEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field path string The path to the keyframes file, relative to Data Files\Meshes.
--- @field sequenceName string The name of the associated NiSequence object.
--- @field textKeys niTextKeyExtraData? Convience access to the sequence text keys, or `nil` if it has none.
