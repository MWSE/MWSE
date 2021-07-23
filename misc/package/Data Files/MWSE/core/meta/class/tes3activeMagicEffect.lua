--- @meta

--- An active magic effect.
--- @class tes3activeMagicEffect
--- @field attributeId number The attribute ID (note that this may be the skill ID if the effect affects skills).
--- @field duration number The total duration of this specific effect.
--- @field effectId number The magic effect ID.
--- @field effectIndex number The index of the effect inside the magic source instance.
--- @field effectInstance tes3magicEffectInstance The magic effect instance for this effect.
--- @field harmful boolean No description yet available.
--- @field instance tes3magicSourceInstance The magic source instance for this effect.
--- @field magnitude number The unresisted magnitude of the magic effect, that is the magnitude before resistance attributes are applied. This is always an integer number. To find the actual applied magnitude, you must check the magic effect instance.
--- @field magnitudeMin number Deprecated.
--- @field mobile tes3magicSourceInstance The mobile affected by the magic effect.
--- @field next tes3activeMagicEffect Deprecated.
--- @field previous tes3activeMagicEffect Deprecated.
--- @field serial number The magic source instance serial number. See the ``instance`` and ``effectInstance`` properties to easily access the magic and effect instances.
--- @field skillId number The skill ID (note that this may be the attribute ID if the effect affects attributes).
tes3activeMagicEffect = {}

