return {
    type = "event",
    description =
    "This event is raised when the enchanting menu determines the spell point cost of the current enchantment. Modifying this value affects the displayed enchantment cost, the derived cast/charge use values, the self-enchant success chance, and the base value used for enchanting service pricing.",
    related = { "calcEnchantmentPrice", "calcSpellmakingSpellPointCost" },
    eventData = {
        ["mobile"] = {
            type = "tes3mobileActor",
            readOnly = true,
            description = "The mobile actor offering enchanting services. May not always be available.",
        },
        ["reference"] = {
            type = "tes3reference",
            readOnly = true,
            description = "A shortcut to the mobile's reference. May not always be available.",
        },
        ["spellPointCost"] = {
            type = "number",
            description = "The total spell point cost of the enchantment. This can be modified.",
        },
        ["item"] = {
            type = "tes3item",
            readOnly = true,
            description = "The item, if any, that is being enchanted.",
        },
        ["itemData"] = {
            type = "tes3itemData",
            readOnly = true,
            description = "The item data for the enchanted item.",
        },
        ["soulGem"] = {
            type = "tes3misc",
            readOnly = true,
            description = "The soul gem being used to make the enchantment.",
        },
        ["soulGemData"] = {
            type = "tes3itemData",
            readOnly = true,
            description = "The soul gem's associated item data.",
        },
        ["soul"] = {
            type = "tes3actor",
            readOnly = true,
            description = "The soul contained in the soul gem.",
        },
        ["castType"] = {
            type = "tes3.enchantmentType",
            readOnly = true,
            description = "The enchantment cast type. Maps to `tes3.enchantmentType.*`.",
        },
        ["effects"] = {
            type = "table[]",
            readOnly = true,
            description =
            "An array of the effects being enchanted onto the item. This includes the `effect` object, `magnitudeLow`/`magnitudeHigh` values, the `area`, `duration`, and `range`. The `attribute` and `skill` values are available as appropriate.",
        },
    },
    filter = "reference",
    examples = {
        ["basic"] = {
            title = "Adjust the enchanting spell point cost",
        },
    },
}
