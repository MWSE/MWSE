--- @meta

--- A structure that holds statistical information relating to the current and base values of a statistic such as health, fatigue, magicka, attributes, or skills.
--- 
--- Morrowind statistics do not track buffs and debuffs separately. Instead, total buff or debuff values can be calculated with the tes3.getEffectMagnitude function, using the appropriate buff or debuff magic effect for the statistic.
--- @class tes3statistic
--- @field base number The base value of the statistic. For example, for a mobile with a normal health amount of 100, fortified by 50, and having taken damage of 25, the base value would be 100, the normal health amount.
--- @field baseRaw number The base value of the statistic, as stored in memory. Prefer to use .base unless you know what you're doing.
--- @field current number The current value of the statistic. For example, for a mobile with a normal health amount of 100, fortified by 50, and having taken damage of 25, the current value would be 125, the health amount after all buffs and debuffs have been accounted for.
--- @field currentRaw number The current value of the statistic, as stored in memory. Prefer to use .current unless you know what you're doing.
--- @field normalized number The normalized value of the statistic. For example, for an account with current health of 50 and base health of 100, the normalized value would be equal to current divided by base. 50 (current) / 100 (base) = .5 (normalized). This is useful for quickly calculating percentages for statistics.
tes3statistic = {}

