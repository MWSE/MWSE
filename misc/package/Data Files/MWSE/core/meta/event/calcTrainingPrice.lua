--- @meta

-- This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
-- More information: https://github.com/MWSE/MWSE/tree/master/docs

--- This event is called when determining the cost of training, and can be used to modify the given price.
--- @class calcTrainingPriceEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field basePrice number *Read-only*. The price before adjustment.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor of the trainer the player is interacting with.
--- @field price number The adjusted price of the training.
--- @field reference tes3reference *Read-only*. mobile’s related reference.
--- @field skill tes3statisticSkill *Read-only*. The ID of the skill to be trained.
--- @field skillId number *Read-only*. The ID of the skill to be trained.
