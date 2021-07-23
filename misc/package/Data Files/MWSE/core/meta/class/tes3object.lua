--- @meta

--- Almost anything that can be represented in the Construction Set is based on this structure.
--- @class tes3object : tes3baseObject
--- @field nextInCollection tes3object The next object in parent collection's list.
--- @field owningCollection tes3referenceList The collection responsible for holding this object.
--- @field previousInCollection tes3object The previous object in parent collection's list.
--- @field scale number The object's scale.
--- @field sceneNode niNode The scene graph node for this object.
--- @field sceneReference niNode The scene graph reference node for this object.
tes3object = {}

