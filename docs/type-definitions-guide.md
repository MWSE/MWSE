# Type Definitions Guide

Type definitions can be found in autocomplete\definitions\namedTypes. Each type has a Lua file of the name of the type and a folder of the same name which contains definitions for all of the type's exposed values and methods. The type definition file is just a regular Lua table with the following fields:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"class"` for type defintions. |
| brief | `string` | Is this even a thing in new docs? |
| description | `string` | The description for the type. You can pass a string with `""` or `[[]]`. |
| inherits | `string` | The type from which this type inherits should be passed here. This will allow the documentation builders to build the proper inheritance chains. For example, when a function accepts `tes3mobileActor`, because `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` have `inherits = "tes3mobileActor"`, the docs will be built with `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` parameters for that function automatically. This saves you the job for finding out how these inheritances work when writing the function definitions. |
| isAbstract | `boolean` | This is a flag for types that can't be accessed normally. There are some types which inherit from abstract ones. |

An example of a typical type definition:
```Lua
-- autocomplete\definitions\namedTypes\niAVObject.lua
return {
	type = "class",
	description = [[The typical base type for most NetImmerse scene graph objects.]],
	inherits = "niObjectNET",
	isAbstract = true,
}
```

## Value Definitions

Each of the exposed type values and methods have definition files in a subfolder of the type's name. Each type value definition file is just a regular Lua table with the following fields:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"value"` for value defintions. |
| description | `string` | The description for the value. You can pass a string with `""` or `[[]]`. |
| readOnly | `boolean` | If a value is writable, this field is unnecessary. |
| valuetype | `string` | This allows to specify the value type. You can put string names for basic Lua types: `number`, `boolean` and `string`, or objects exposed by MWSE, such as `tes3reference`. If the value can be of two or more types, then you should pass all the types split by `|`. For example, `tes3mobileCreature|tes3mobileNPC|nil`. |
| examples | `table` | A table with entries that are the names of the files included as examples. Each entry is a table itself with on available field, `title`. The title will be shown as the title of the example on the documentation page. |

An example of a typical value definition:
```Lua
-- autocomplete\definitions\namedTypes\tes3reference\isDead.lua
return {
	type = "value",
	description = [[Returns `true` if the object is dead, `false` if they are alive, or `nil` if that couldn't be determined.]],
	readOnly = true,
	valuetype = "boolean|nil",
}
```

## Method Definitions

Each type method definition file is just a regular Lua table with the following fields:
| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"method"` for method defintions. |
| description | `string` | The description for the method. You can pass a string with `""` or `[[]]`. |
| arguments | `table` | The arguments the function accepts. |
| examples | `table` | A table with entries that are the names of the files included as examples. Each entry is a table itself with on available field, `title`. The title will be shown as the title of the example on the documentation page. |
| returns | `table` | The table with return values. |

An example of a typical method definition:
```Lua
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

For a more elaborate description of the argument and return tables, please refer to [function definitions guide.](https://github.com/MWSE/MWSE/blob/docs-contributions-guide/docs/function-definitions-guide.md)

## Notes

Some types may have fields which are not valuer nor methods. In that case, you can pass different type to the `type` value of the definition.
