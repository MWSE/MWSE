--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- Orients an axis of the controlled object towards a lookAt target. The axis is selectable.
--- 
--- Warning: The lookAt property is not reference counted, so this controller must be removed, or lookAt set to nil before the lookAt object is deleted.
--- @class niLookAtController : niTimeController, niObject
--- @field axis ni.lookAtControllerAxis A number representing the axis that points at the lookAt object. `flip` determines which end of this axis points at the target.
--- 
--- - `0` X axis
--- - `1` Y axis
--- - `2` Z axis
--- 
--- Maps to values in [`ni.lookAtControllerAxis`](https://mwse.github.io/MWSE/references/ni/look-at-controller-axes/) table.
--- @field flip boolean Determines which end of the chosen axis points towards the lookAt target.
--- @field lookAt niAVObject|niAmbientLight|niAutoNormalParticles|niBSAnimationNode|niBSParticleNode|niBillboardNode|niCamera|niCollisionSwitch|niDirectionalLight|niNode|niParticles|niPointLight|niRotatingParticles|niSortAdjustNode|niSpotLight|niSwitchNode|niTextureEffect|niTriShape The object that this controller will point towards. This is not a reference counted pointer, so this controller must be removed or lookAt set to nil before the lookAt object is deleted. This was probably designed to avoid reference cycles where controllers keep an ancestor node alive.
