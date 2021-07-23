--- @meta

--- The typical base type for most NetImmerse scene graph objects.
--- @class niAVObject : niObjectNET, niObject
--- @field alphaProperty niAlphaProperty|nil Convenient access to this object's alpha property. Setting this value to be nil will erase the property, while setting it to a valid alpha property will set (or replace) it.
--- @field appCulled boolean A flag indicating if this object is culled. When culled, it will not render, and raycasts ignore it.
--- @field flags number Flags, dependent on the specific object type.
--- @field fogProperty niFogProperty|nil Convenient access to this object's fog property. Setting this value to be nil will erase the property, while setting it to a valid fog property will set (or replace) it.
--- @field materialProperty niMaterialProperty|nil Convenient access to this object's material property. Setting this value to be nil will erase the property, while setting it to a valid material property will set (or replace) it.
--- @field parent niNode The object's parent. It may not have one if it is not attached to the scene.
--- @field properties niPropertyLinkedList The list of properties attached to this niAVObject.
--- @field rotation tes3matrix33 The object's local rotation matrix.
--- @field scale number The object's local uniform scaling factor.
--- @field stencilProperty niStencilProperty|nil Convenient access to this object's stencil property. Setting this value to be nil will erase the property, while setting it to a valid stencil property will set (or replace) it.
--- @field texturingProperty niTexturingProperty|nil Convenient access to this object's texturing property. Setting this value to be nil will erase the property, while setting it to a valid texturing property will set (or replace) it.
--- @field translation tes3vector3 The object's local translation vector.
--- @field vertexColorProperty niVertexColorProperty|nil Convenient access to this object's vertex coloring property. Setting this value to be nil will erase the property, while setting it to a valid vertex coloring property will set (or replace) it.
--- @field zBufferProperty niZBufferProperty|nil Convenient access to this object's z-buffer property. Setting this value to be nil will erase the property, while setting it to a valid z-buffer property will set (or replace) it.
niAVObject = {}

--- Attach a property to this object.
--- @param property niProperty No description yet available.
function niAVObject:attachProperty(property) end

--- Resets the object's local transform.
function niAVObject:clearTransforms() end

--- Detaches and returns a property from the object which matches the given property type.
--- @param type number No description yet available.
--- @return niProperty result No description yet available.
function niAVObject:detachProperty(type) end

--- Searches this node and all child nodes recursively for a node with a name that matches the argument.
--- @param name string No description yet available.
--- @return niAVObject result No description yet available.
function niAVObject:getObjectByName(name) end

--- Gets an attached property by property type.
--- @param type number No description yet available.
--- @return niProperty result No description yet available.
function niAVObject:getProperty(type) end

--- Updates the world transforms of this node and its children, which makes changes visible for rendering. Use after changing any local rotation, translation, scale, or bounds.
function niAVObject:update() end

--- Update all attached effects.
function niAVObject:updateEffects() end

--- Update all attached properties.
function niAVObject:updateProperties() end

