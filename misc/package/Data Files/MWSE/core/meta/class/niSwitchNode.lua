--- @meta

--- Represents groups of multiple scene graph subtrees, only one of which is visible at any given time. They are useful for showing different states of a model depending on engine / lua logic. If you detach the active subtree, the switch node will set the active subtree to none, or to an index of -1.
--- @class niSwitchNode : niNode, niAVObject, niObjectNET, niObject
--- @field switchIndex number The value of the active child index. This index represents the index of the active child in the array of the NiSwitchNode. If this index is –1, then no child is active.
niSwitchNode = {}

