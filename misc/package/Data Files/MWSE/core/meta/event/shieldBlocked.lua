-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- @meta
--- This event is fired when a melee strike is blocked, and the equipped shield is about to take damage from the strike. It allows modification of the damage applied to the shield.
--- @class shieldBlockedEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field attacker tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor dealing the damage.
--- @field conditionDamage number The shield's condition will be reduced by this amount. It is initially equal to the pre-armor-mitigation damage value of the strike.
--- @field mobile tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor which is blocking the strike.
--- @field reference tes3reference *Read-only*. A shortcut to the mobile's reference.