return {
	type = "value",
	description = [[A table with friendly named access to all supported attachments. See the table below for available keys in this table.

Key               | Attachment type | Description
----------------- | --------------- | -----------
actor             | [tes3mobileActor](https://mwse.github.io/MWSE/types/tes3mobileActor/) | The associated mobile object, if applicable.
animation         | [tes3animationData](https://mwse.github.io/MWSE/types/tes3animationData/) |
bodyPartManager   | [tes3bodyPartManager](https://mwse.github.io/MWSE/types/tes3bodyPartManager/) |
leveledBase       | [tes3reference](https://mwse.github.io/MWSE/types/tes3reference/) |
light             | [tes3lightNode](https://mwse.github.io/MWSE/types/tes3lightNode/) | The dynamic light attachment.
lock              | [tes3lockNode](https://mwse.github.io/MWSE/types/tes3lockNode/) | Only present on locked containers or doors.
travelDestination | [tes3travelDestinationNode](https://mwse.github.io/MWSE/types/tes3travelDestinationNode/) | Only present on teleport doors.
variables         | [tes3itemData](https://mwse.github.io/MWSE/types/tes3itemData/) | Only present on items when needed e.g. the item is fully repaired.


```lua
-- Accessing the attached tes3animationData
local animData = myRef.attachments.animation
```
]],
	readOnly = true,
	valuetype = "table<string, tes3bodyPartManager|tes3itemData|tes3lightNode|tes3lockNode|tes3mobileActor|tes3reference|tes3travelDestinationNode|tes3animationData>",
}
