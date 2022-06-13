--- @meta
--- @diagnostic disable:undefined-doc-name

--- This event is triggered after magic absorption absorbs and cancels a magic effect, and just before magic absorption gives magicka to the target. It can control the amount of magicka restored.
--- 
--- This event occurs once per effect restored, so a multi-effect spell may trigger this multiple times. The default amount restored is equal to the casting cost of the entire spell, which is the vanilla behaviour.
--- @class absorbedMagicEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field absorb number The amount of magicka to restore to the actor.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor that absorbed the spell.
--- @field source tes3alchemy|tes3enchantment|tes3spell *Read-only*. The magic source.
--- @field sourceInstance tes3magicSourceInstance *Read-only*. The unique instance of the magic source.
--- @field target tes3reference *Read-only*. The actor that absorbed the spell.