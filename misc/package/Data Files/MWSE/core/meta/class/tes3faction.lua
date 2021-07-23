--- @meta

--- A faction game object.
--- @class tes3faction : tes3baseObject
--- @field attributes table An array-style table holding the two attributes that govern advancement.
--- @field name string The faction's player-facing name.
--- @field playerExpelled boolean The player's expelled state in the faction.
--- @field playerJoined boolean The player's join state for the faction.
--- @field playerRank number The player's current rank in the faction.
--- @field playerReputation number The player's current reputation in the faction.
--- @field ranks table An array-style table holding the ten related tes3factionRanks.
--- @field reactions tes3iterator A collection of tes3factionReactions.
--- @field skills table An array-style table holding the seven skills that govern advancement.
tes3faction = {}

