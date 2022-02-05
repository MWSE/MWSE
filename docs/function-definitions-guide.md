# Function Definitions Guide

Function definitions can be found in autocomplete\definitions\global. Some functions are exposed in global namespace and have their definitions in that folder, while some are exposed as libraries. Those have their own folders.


## Library Definitions

Each library definition is just a regular Lua table. The following fields are available:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` |  This flag is used when generating syntax highlighting files. The type should always be `"lib"` for library definitions. |
| deprecated | `boolean` | Passing `true` here will mark all the functions and values exposed by the library as deprecated in a IDE and the documentation page. |
| description | `string` | The description for the library. You can pass a string with `""` or `[[]]`. |
| link | `string` | If present, the generated autocomplete files will allow the IDEs to provide the link to the documentation for that library. |

For example, the table library definition looks like this:

```Lua
-- autocomplete\definitions\global\table.lua
return {
	type = "lib",
	description = "This library provides generic functions for table manipulation. It provides all its functions inside the table table.",
	link = "https://www.lua.org/manual/5.1/manual.html#5.5",
}
```

All the function definitions of a certain library are found in the folder of the same name. For example, all the function definitions for `tes3ui` API are found inside `tes3ui` folder.


## Function Definitions

Each function definition is just a regular Lua table. The following fields are available:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` |  This flag is used when generating syntax highlighting files. The type should always be `"function"` for function definitions. |
| description | `string` | The description for the function. You can pass a string with `""` or `[[]]`. |
| arguments | `table` | The arguments the function accepts. |
| examples | `table` | A table with entries that are the names of the files included as examples. Each entry is a table itself with on available field, `title`. The title will be shown as the title of the example on the documentation page. |
| returns | `table` | The table with return values. |

`returns` table has entries which are nested tables for each of the returned values, with the following fields:
| Field | Type | Description |
| ----- | ---- | ----------- |
| name  | `string` | The name of the argument. |
| type  | `string` | This allows to specify the type of the value in evenData. You can put string names for basic Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types split by `|`. For example, `tes3mobileCreature|tes3mobileNPC|nil`. |
| description | `string` | The description for the argument. |

An example of a function with multiple return values is `tes3.checkMerchantOffersService()`. Its definition looks like this:
```Lua
-- autocomplete\definitions\global\tes3\checkMerchantOffersService.lua
return {
	type = "function",
	description = [[Checks if a merchant will offer a service to you, including dialogue checks like disposition and faction membership. A specific service can be checked, or if no service is given, a generic dialogue check is made. If the service is refused, the dialogue reply for the refusal may also be returned (it may be nil, as there may not always be a reply available).]],
	arguments = {
		{ name = "reference", type = "tes3reference|tes3mobileActor|string" },
		{ name = "service", type = "number", optional = true, description = "The specific service to check for availability. Uses the [`tes3.merchantService`](https://mwse.github.io/MWSE/references/merchant-service-types/) constants." },
	},
	returns = {
		{ name = "offersService", type = "boolean" },
		{ name = "refusalReply", type = "tes3dialogueInfo" },
	},
}
```


Some functions accept multiple arguments. In that case the arguments table should have a nested table for each argument.
| Field | Type | Description |
| ----- | ---- | ----------- |
| name  | `string` | The name of the argument. |
| type  | `string` | This allows to specify the type of the value in evenData. You can put string names for basic Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types split by `|`. For example, `tes3mobileCreature|tes3mobileNPC|nil`. |
| description | `string` | The description for the argument. |

An example of such function is `tes3.claimSpellEffectId()`. This is the definition for that function:

```Lua
-- autocomplete\definitions\global\tes3\claimSpellEffectId.lua
return {
	type = "function",
	description = [[This function is used to claim a unique spell effect name and id. This is needed before actually creating a new effect by calling `tes3.addMagicEffect()`. A claimed effect id is then available as: `tes3.effect.effectName` (just like any other spell effect). For examples of this function in practice see [`tes3.addMagicEffect()`](https://mwse.github.io/MWSE/apis/tes3/#tes3addmagiceffect) example.]],
	arguments = {
		{ name = "name", type = "string", description = "The name of the new spell effect. Must be unique. An error will be thrown if it's non-unique." },
		{ name = "id", type = "number", description = "A unique number representing the new spell effect. An error will be thrown if it's non-unique." },
	},
}
```

Some other functions accept a table called `params`. In that case you can follow this convention (the function definition for `tes3.addItem()`):

```Lua
-- autocomplete\definitions\global\tes3\addItem.lua
return {
	type = "function",
	description = [[Adds an item to a given reference's inventory or mobile's inventory.]],
	arguments = {{
		name = "params",
		type = "table",
		tableParams = {
			{ name = "reference", type = "tes3reference|tes3mobileActor|string", description = "Who to give items to." },
			{ name = "item", type = "tes3item|tes3leveledItem|string", description = "The item to add. If a leveled item is passed, it will be resolved and added." },
			{ name = "itemData", type = "tes3itemData", optional = true, description = "The item data for the item." },
			{ name = "soul", type = "tes3creature", optional = true, description = "For creating filled soul gems." },
			{ name = "count", type = "number", default = "1", description = "The maximum number of items to add." },
			{ name = "playSound", type = "boolean", default = true, description = "If false, the up/down sound for the item won't be played." },
			{ name = "limit", type = "boolean", default = false, description = "If false, items can be placed into containers that shouldn't normally be allowed. This includes organic containers, and containers that are full." },
			{ name = "reevaluateEquipment", type = "boolean", default = true, description = "If true, and the item added is armor, clothing, or a weapon, the actor will reevaluate its equipment choices to see if the new item is worth equipping. This does not affect the player." },
			{ name = "updateGUI", type = "boolean", default = true, description = "If false, the function won't manually resync the player's GUI state. This can result in some optimizations, though [`tes3ui.forcePlayerInventoryUpdate()`](https://mwse.github.io/MWSE/apis/tes3ui/#tes3uiforceplayerinventoryupdate) must manually be called after all inventory updates are finished." },
		},
	}},
	returns = {{ name = "addedCount", type = "number" }},
}
```
