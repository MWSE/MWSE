--- @meta
--- @diagnostic disable:undefined-doc-name

--- A game structure that keeps track of a magic source on an object.
--- @class tes3magicSourceInstance : tes3baseObject
--- @field castChanceOverride number No description yet available.
--- @field caster tes3reference *Read-only*. No description yet available.
--- @field corprusHoursSinceLastWorsen number The number of hours passed since the player's corprus state last worsened.
--- @field item tes3alchemy|tes3apparatus|tes3armor|tes3book|tes3clothing|tes3ingredient|tes3light|tes3lockpick|tes3misc|tes3probe|tes3repairTool|tes3weapon *Read-only*. No description yet available.
--- @field itemData tes3itemData *Read-only*. No description yet available.
--- @field itemID string *Read-only*. No description yet available.
--- @field magicID string *Read-only*. No description yet available.
--- @field projectile tes3mobileProjectile|tes3mobileSpellProjectile *Read-only*. No description yet available.
--- @field serialNumber number *Read-only*. Unique indentifier for just this magic source.
--- @field source tes3alchemy|tes3enchantment|tes3spell *Read-only*. No description yet available.
--- @field sourceEffects table *Read-only*. An array-style table holding spell effects this magic source has.
--- @field sourceType number *Read-only*. The type of this magic source. Maps to [`tes3.magicSourceType`](https://mwse.github.io/MWSE/references/magic-source-types/) constants.
--- @field state number Shows if the state is pre-cast, cast, beginning, working, ending, retired, etc. Maps to [`tes3.spellState`](https://mwse.github.io/MWSE/references/spell-states/) constants.
--- @field target tes3reference *Read-only*. No description yet available.
--- @field timestampCastBegin number No description yet available.
tes3magicSourceInstance = {}

--- Gets the magic effect instance for a given effect index and target reference. Effect instances may not all be active on a target, due to reflect, absorption, dispels, different durations and other factors.
--- @param index number The index in the effect list to fetch, between `0` and `7`.
--- @param target tes3reference The target actor for the effect.
--- @return tes3magicEffectInstance result No description yet available.
function tes3magicSourceInstance:getEffectInstance(index, target) end

--- Gets the magnitude from the casting source for a given effect index.
--- @param index number The index in the effect list to fetch, between `0` and `7`.
--- @return number result No description yet available.
function tes3magicSourceInstance:getMagnitudeForIndex(index) end

--- This function plays an animation for an effect from the `tes3magicSourceInstance` object.
---
--- [Examples available in online documentation](https://mwse.github.io/MWSE/types/tes3magicSourceInstance/#playVisualEffect).
--- @param params tes3magicSourceInstance.playVisualEffect.params This table accepts the following values:
--- 
--- `effectIndex`: number — The index in the effect whose visual will be played, a number in range [0, 7].
--- 
--- `position`: tes3vector3|table — A table or a `tes3vector3` holding `x`, `y` and `z` coordinates at which the visual effect will be played.
--- 
--- `visual`: tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3repairTool|tes3static|tes3weapon|string — The visual effect to be played.
--- 
--- `scale`: number — *Default*: `1`. The scale of the effect. Only applies to effects that are designed to be scaled.
--- 
--- `reference`: tes3reference|string — A reference on which the visual effect will be played.
function tes3magicSourceInstance:playVisualEffect(params) end

---Table parameter definitions for `tes3magicSourceInstance.playVisualEffect`.
--- @class tes3magicSourceInstance.playVisualEffect.params
--- @field effectIndex number The index in the effect whose visual will be played, a number in range [0, 7].
--- @field position tes3vector3|table A table or a `tes3vector3` holding `x`, `y` and `z` coordinates at which the visual effect will be played.
--- @field visual tes3activator|tes3alchemy|tes3apparatus|tes3armor|tes3bodyPart|tes3book|tes3clothing|tes3container|tes3containerInstance|tes3creature|tes3creatureInstance|tes3door|tes3ingredient|tes3leveledCreature|tes3leveledItem|tes3light|tes3lockpick|tes3misc|tes3npc|tes3npcInstance|tes3probe|tes3repairTool|tes3static|tes3weapon|string The visual effect to be played.
--- @field scale number *Default*: `1`. The scale of the effect. Only applies to effects that are designed to be scaled.
--- @field reference tes3reference|string A reference on which the visual effect will be played.
