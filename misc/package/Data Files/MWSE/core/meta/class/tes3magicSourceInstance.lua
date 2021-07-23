--- @meta

--- A game structure that keeps track of a magic source on an object.
--- @class tes3magicSourceInstance : tes3baseObject
--- @field castChanceOverride number No description yet available.
--- @field caster tes3reference No description yet available.
--- @field item tes3item No description yet available.
--- @field itemData tes3itemData No description yet available.
--- @field itemID string No description yet available.
--- @field magicID string No description yet available.
--- @field projectile tes3mobileProjectile No description yet available.
--- @field source tes3alchemy|tes3enchantment|tes3spell No description yet available.
--- @field sourceType number Shows if the source is a spell, enchantment, or alchemy.
--- @field state number Shows if the state is pre-cast, cast, beginning, working, ending, retired, etc.
--- @field target tes3reference No description yet available.
--- @field timestampCastBegin number No description yet available.
tes3magicSourceInstance = {}

--- Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.
--- @param index number The index in the effect list to fetch, between 0 and 7.
--- @param target tes3reference The target actor for the effect.
--- @return tes3magicEffectInstance result No description yet available.
function tes3magicSourceInstance:getEffectInstance(index, target) end

--- Gets the magnitude from the casting source for a given effect index.
--- @param index number The index in the effect list to fetch, between 0 and 7.
--- @return number result No description yet available.
function tes3magicSourceInstance:getMagnitudeForIndex(index) end

