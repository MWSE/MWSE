# Type Definitions Guide

Type definitions can be found in autocomplete\definitions\namedTypes. Each type has a definition file of the type's name. In addition, some types have a folder of the same name which contains definitions for all of the type's exposed values and methods. The type definition file is just a regular Lua table with the following fields:

| Field       | Type      | Description |
| ----------- | --------- | ----------- |
| type        | `string`  | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"class"` for type defintions. |
| brief       | `string`  | Is this even a thing in new docs? |
| description | `string`  | The description for the type. You can pass a string inside double quotes (`""`) or double square brackets (`[[]]`). |
| inherits    | `string`  | The type from which this type inherits should be passed here. This will allow the documentation builders to build the proper inheritance chains. For example, when a function accepts `tes3mobileActor`, because `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` have `inherits = "tes3mobileActor"`, the docs will be built with `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` parameters for that function automatically. This saves you the job of figuring out the complete inheritance chains. |
| isAbstract  | `boolean` | This is a flag for types that can't be accessed normally. There are some types which inherit from abstract ones. |

An example of a typical type definition:

```lua
-- autocomplete\definitions\namedTypes\niAVObject.lua
return {
	type = "class",
	description = [[The typical base type for most NetImmerse scene graph objects.]],
	inherits = "niObjectNET",
	isAbstract = true,
}
```

## Value Definitions

Each of the exposed type's properties and methods has a definition file in a subfolder of the type's name. Each type property definition file is just a regular Lua table with the following fields:
| Field       | Type      | Description |
| ----------- | --------- | ----------- |
| type        | `string`  | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"value"` for property defintions. |
| description | `string`  | The description for the property. |
| readOnly    | `boolean` | If the property is writable, this field is unnecessary. |
| valuetype   | `string`  | This allows to specify the property type. You can put string names for basic Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types joined by the vertical bar character, thereby passing a union of all the possible types. If the type is `table`, consider adding the key and value types like this: `table<indexType, keyType>`. This will allow autocomplete to automatically deduce the types when the table is indexed. See more in the example below. |
| examples    | `table`   | A table with entries that are the names of the files included as examples. Each entry is a table itself with one available field, `title`. The title will be shown as the title of the example on the documentation page. It works the same as examples for [event](https://github.com/MWSE/MWSE/blob/master/docs/event-definitions-guide.md) or [function](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md) definitions. |

An example of a typical property definition:

```lua
-- autocomplete\definitions\namedTypes\tes3reference\isDead.lua
return {
	type = "value",
	description = [[Returns `true` if the object is dead, `false` if they are alive, or `nil` if that couldn't be determined.]],
	readOnly = true,
	valuetype = "boolean|nil",
}
```

An example of a value property, whose type is a table:

```lua
-- autocomplete\definitions\namedTypes\tes3weatherController\weathers.lua
return {
	type = "value",
	description = [[Array-style table for the different weather types. Each object in the table is a [tes3weather](https://mwse.github.io/MWSE/types/tes3weather/). The indexes in the table correspond to the [`tes3.weather`](https://mwse.github.io/MWSE/references/weather-types/) enumeration.]],
	readOnly = true,
	valuetype = "table<tes3.weather, tes3weather>",
}
```

### Type Property Definitions

This kind of definitions is meant for documenting constants available on types not instances. For example some math classes such as `tes3vector3` could have a `forward` property documented this way. Type property definition schema:
| Field       | Type      | Description |
| ----------- | --------- | ----------- |
| type        | `string`  | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"typeValue"` for type property defintions. |
| description | `string`  | The description for the property. |
| readOnly    | `boolean` | If the property is writable, this field is unnecessary. |
| valuetype   | `string`  | This allows to specify the property type. You can put string names for basic
Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types joined by the vertical bar character, thereby passing a union of all the possible types. If the type is `table`, consider adding the key and value types like this: `table<indexType, keyType>`. This will allow autocomplete to automatically deduce the types when the table is indexed. See more in the example below. |
| examples    | `table`   | A table with entries that are the names of the files included as examples. Each entry is a table itself with one available field, `title`. The title will be shown as the title of the example on the documentation page. It works the same as examples for [event](https://github.com/MWSE/MWSE/blob/master/docs/event-definitions-guide.md) or [function](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md) definitions. |

An example of a hypothetical type property definition:

```lua
-- autocomplete\definitions\namedTypes\tes3vector3\forward.lua
return {
	type = "typeValue",
	description = [[A vector pointing along the positive X axis.]],
	valuetype = "tes3vector3",
}
```

## Method Definitions

Each type method definition file is just a regular Lua table with the following fields:
| Field       | Type     | Description |
| ----------- | -------- | ----------- |
| type        | `string` | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"method"` for method defintions. |
| description | `string` | The description for the method. |
| generics    | `table`  | The generics available as types for arguments and return values. |
| arguments   | `table`  | The arguments the function accepts. |
| examples    | `table`  | A table with entries that are the names of the files included as examples. Each entry is a table itself with one available field, `title`. The title will be shown as the title of the example on the documentation page. It works the same as examples for [event](https://github.com/MWSE/MWSE/blob/master/docs/event-definitions-guide.md) or [function](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md) definitions. |
| returns     | `table`  | The table with return values. |

An example of a typical method definition:

```lua
-- autocomplete\definitions\namedTypes\tes3reference\getAngleTo.lua
return {
	type = "method",
	description = [[Calculates the angle from this reference's current facing to the target reference.]],
	arguments = {
		{ name = "reference", type = "tes3reference", description = "The reference to calculate the angle to." },
	},
	returns = {{ name = "angle", type = "number", description = "The angle to the given reference." }},
}
```

For a more elaborate description of the argument and return tables, please refer to [function definitions guide.](https://github.com/MWSE/MWSE/blob/master/docs/function-definitions-guide.md)

## Notes

Some types may have fields that are not values or methods. In that case, you can pass different types to the `type` value of the definition. For example, some types also expose functions. In that case, you can pass `type = "function"`. Here is an example of such a definition:

```lua
-- autocomplete\definitions\namedTypes\tes3vector3\new.lua
return {
	type = "function",
	description = [[Creates a new vector. If no parameters are provided, an empty set will be constructed.]],
	arguments = {
		{ name = "x", type = "number", optional = true },
		{ name = "y", type = "number", optional = true },
		{ name = "z", type = "number", optional = true },
	},
	returns = {{ name = "vector", type = "tes3vector3" }},
}
```
