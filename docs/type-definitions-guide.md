# Type Definitions Guide

Type definitions can be found in autocomplete\definitions\namedTypes. Each type has a Lua file of the name of the type and a folder of the same name which contains definitions for all of the type's exposed values and methods. The type definition file is just a regular Lua table with the following fields:

| Field | Type | Description |
| ----- | ---- | ----------- |
| type  | `string` | The type of the definition. This flag is used when generating syntax highlighting files. This should always be `"class"` for type defintions. |
| brief | `string` | Is this even a thing in new docs? |
| description | `string` | The description for the type. You can pass a string with `""` or `[[]]`. |
| inherits | `string` | The type from which this type inherits should be passed here. This will allow the documentation builders to build the proper inheritance chains. For example, when a function accepts `tes3mobileActor`, because `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` have `inherits = "tes3mobileActor"`, the docs will be built with `tes3mobileNPC`, `tes3mobileCreature`, and `tes3mobilePlayer` parameters for that function automatically. This saves you the job for finding out how these inheritances work when writing the function definitions. |
| isAbstract | `boolean` |  |

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

TODO: describe value and method definitions
