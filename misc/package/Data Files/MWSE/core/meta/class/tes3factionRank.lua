--- @meta
--- @diagnostic disable:undefined-doc-name

--- A container describing an individual rank inside a faction. In Morrowind, the player needs to meet all the requirements to advance to a rank in a faction. These include two of the faction's favored attributes, any two of the faction's favored skills and reputation requirement.
--- @class tes3factionRank
--- @field attributes number[] *Read-only*. An array-style table holding the two attribute requirements.
--- @field reputation number The reputation requirement to advance to this rank.
--- @field skills number[] *Read-only*. An array-style table holding the two skill requirements.
tes3factionRank = {}
