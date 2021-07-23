--- @meta

--- Base class for dynamic effects such as NiLights or projected textures effects.
--- @class niDynamicEffect : niAVObject, niObjectNET, niObject
--- @field affectedNodes niNodeLinkedList The list of nodes that a given dynamic effect will affect.
--- @field enabled boolean The enabled state of a given dynamic effect.
--- @field type number The enumerated type of a given dynamic effect. Types: `0 - niAmbientLight`, `1 - niDirectionalLight`, `2 - niPointLight`, `3 - niSpotLight`, `4 - niTextureEffect`.
niDynamicEffect = {}

--- Adds a node to the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.
--- @param node niNode The node to add to the affected nodes list.
function niDynamicEffect.attachAffectedNode(node) end

--- Removes a node from the dynamic effect's affected nodes list. The node's ``:updateEffects()`` function should be called afterwards to recognize the change.
--- @param node niNode The node to remove from the affected nodes list.
function niDynamicEffect.detachAffectedNode(node) end

