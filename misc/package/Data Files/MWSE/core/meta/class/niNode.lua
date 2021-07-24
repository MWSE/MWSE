--- @meta

--- Base class that represents the nodes of a scene graph. A node can have any number of child nodes.
--- @class niNode : niAVObject, niObjectNET, niObject
--- @field children niAVObjectTArray The children of the node.
--- @field effectList niDynamicEffectLinkedList The effect list of the node.
niNode = {}

--- Attachs the child to the children list of the node.
--- @param child niAmbientLight|niCamera|niCollisionSwitch|niDirectionalLight|niDynamicEffect|niGeometry|niLight|niNode|niPointLight|niSpotLight|niSwitchNode|niTriBasedGeometry|niTriShape No description yet available.
--- @param useFirstAvailable bool Use the first available space in the list. Default value is False.
function niNode:attachChild(child, useFirstAvailable) end

--- Detaches the child from the children list of the node. Returns the detached child.
--- @param child niAmbientLight|niCamera|niCollisionSwitch|niDirectionalLight|niDynamicEffect|niGeometry|niLight|niNode|niPointLight|niSpotLight|niSwitchNode|niTriBasedGeometry|niTriShape No description yet available.
--- @return niAmbientLight|niCamera|niCollisionSwitch|niDirectionalLight|niDynamicEffect|niGeometry|niLight|niNode|niPointLight|niSpotLight|niSwitchNode|niTriBasedGeometry|niTriShape result No description yet available.
function niNode:detachChild(child) end

--- Detaches the child at the specified index from the children list of the node. Returns the detached child.
--- @param index number No description yet available.
--- @return niAmbientLight|niCamera|niCollisionSwitch|niDirectionalLight|niDynamicEffect|niGeometry|niLight|niNode|niPointLight|niSpotLight|niSwitchNode|niTriBasedGeometry|niTriShape result No description yet available.
function niNode:detachChildAt(index) end

--- Gets the effect for the given type.
--- @param type number No description yet available.
--- @return niAmbientLight|niDirectionalLight|niDynamicEffect|niLight|niPointLight|niSpotLight result No description yet available.
function niNode:getEffect(type) end

