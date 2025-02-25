--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- A managed visual effect, attached to position, reference, or arbitrary node. These can be created through script with [`tes3.createVisualEffect`](https://mwse.github.io/MWSE/apis/tes3/#tes3createvisualeffect) and destroyed with [`tes3.removeVisualEffect`](https://mwse.github.io/MWSE/apis/tes3/#tes3removevisualeffect).
--- @class tes3vfx
--- @field age number The VFX's current age, in seconds.
--- @field attachNode niBSAnimationNode|niBSParticleNode|niBillboardNode|niCollisionSwitch|niNode|niSortAdjustNode|niSwitchNode|nil The node the VFX was created to attach to, if it was created for node attachment.
--- @field beginKeyTime number The start of the VFX's keyframe timing. This is fetched from the creating effect's time controller at the time of the VFX's creation.
--- @field effectNode niBSAnimationNode|niBSParticleNode|niBillboardNode|niCollisionSwitch|niNode|niSortAdjustNode|niSwitchNode The cloned node of the associated `effectObject`.
--- @field effectObject tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3physicalObject|tes3probe|tes3repairTool|tes3static|tes3weapon The object responsible for this VFX's appearance. Its scene node is cloned into `effectNode`.
--- @field endKeyTime number The end of the VFX's keyframe timing. This is fetched from the creating effect's time controller at the time of the VFX's creation.
--- @field expired boolean A flag controlling if the effect is expired. This can be set to cause the effect to remove itself on the next update.
--- @field id number *Read-only*. The unique numberic identifier for this visual effect.
--- @field keyTime number The VFX's current keyframe timing. When this value exceeds the `endKeyTime`, the effect will either loop back to `beginKeyTime` or the effect will expire.
--- @field maxAge number The VFX's maximum age, in seconds.
--- @field position tes3vector3 The position the VFX was created at.
--- @field sourceInstance tes3magicSourceInstance *Read-only*. The the associated magic source instance.
--- @field sourceInstanceSerial number *Read-only*. The serial number of the associated source instance.
--- @field target tes3reference The reference the target is associated with, if any.
--- @field verticalOffset number The VFX's vertical offset, at the time of creation. Changing this value will not automatically readjust the VFX's visual node.
