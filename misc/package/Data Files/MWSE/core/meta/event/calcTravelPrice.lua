--- @meta
--- @diagnostic disable:undefined-doc-name

--- This event is called when determining the price of traveling, and can be used to modify the given price.
--- @class calcTravelPriceEventData
--- @field claim boolean If set to `true`, any lower-priority event callbacks will be skipped. Returning `false` will set this to `true`.
--- @field basePrice number *Read-only*. The price before adjustment.
--- @field companions tes3reference[]|nil *Read-only*. Companions that will travel with the player, or `nil` if no companions are present.
--- @field destination tes3reference *Read-only*. The travel marker that marks the destination.
--- @field mobile tes3mobileActor|tes3mobileCreature|tes3mobileNPC|tes3mobilePlayer *Read-only*. The mobile actor of the merchant the player is interacting with.
--- @field price number The adjusted price of travelling.
--- @field reference tes3reference *Read-only*. mobile’s related reference.