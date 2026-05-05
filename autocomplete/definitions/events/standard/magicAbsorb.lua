return {
    type = "event",
    description = [[This event is triggered before a magic absorption check. It allows changing the absorption chance.

Each spell absorption effect active on a target will roll separately; the absorb chances are independent. This event also occurs once per effect in a spell, so a multi-effect spell may trigger this multiple times.]],
    related = { "absorbedMagic", "spellResist", "spellResisted" },
    blockable = true,
    eventData = {
        ["target"] = {
            type = "tes3reference",
            readOnly = true,
            description = "The actor that may absorb the spell.",
        },
        ["mobile"] = {
            type = "tes3mobileActor",
            readOnly = true,
            description = "The mobile actor that may absorb the spell.",
        },
        ["absorbEffect"] = {
            type = "tes3activeMagicEffect",
            readOnly = true,
            description =
            "The specific spell absorption effect being tested. This is a `tes3activeMagicEffect` instead of a more common magic instance. You can lookup the magic source instance or effect instance with `tes3activeMagicEffect` accessors.",
        },
        ["absorbChance"] = {
            type = "number",
            description = "The % chance that the magic is absorbed. May be modified.",
        },
        ["source"] = {
            type = "tes3alchemy|tes3enchantment|tes3spell",
            readOnly = true,
            description = "The magic source.",
        },
        ["effectIndex"] = {
            type = "number",
            readOnly = true,
            description = "The effect index into the `source` effect list that is being tested against.",
        },
        ["sourceInstance"] = {
            type = "tes3magicSourceInstance",
            readOnly = true,
            description = "The unique instance of the magic source.",
        },
    },
    examples = {
        ["BasicUsage"] = {
            title = "Reduce spell absorption chance for the player",
        },
    },
    filter = "target",
}
