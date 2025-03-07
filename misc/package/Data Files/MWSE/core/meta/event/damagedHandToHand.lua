--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- The damagedHandToHand event triggers after an actor takes fatigue damage from hand-to-hand combat. It does not trigger on health damage, but the `damaged` event will.
--- @class damagedHandToHandEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field attacker tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor dealing the damage. Can be nil.
--- @field attackerReference tes3reference *Read-only*. The attacker mobile's associated reference. Can be nil.
--- @field fatigueDamage number *Read-only*. The amount of fatigue damage done.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor that took fatigue damage.
--- @field reference tes3reference *Read-only*. The mobile’s associated reference.
--- @field source tes3.damageSource *Read-only*. The origin of the damage. May be `tes3.damageSource.attack` or `tes3.damageSource.script`.
