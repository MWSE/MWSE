# debug

This is an extension of Lua Debug library.

## Functions

### `debug.clearLogCacheForFile`

Clears any prefix information from cache for use with `debug.log`. This is useful if your file loads dynamically via `dofile`, and is subject to change during development.

```lua
debug.clearLogCacheForFile(file)
```

**Parameters**:

* `file` (string?): The path to the file. If omitted, the calling file will have its associated log cache removed.

***

### `debug.log`

Logs a message to `MWSE.log` file. The output format is `[path] "msg" = msg`, where the `path` is the path and line at which the function was called.

```lua
local value = debug.log(value)
```

**Parameters**:

* `value` (string): The message to log.

**Returns**:

* `value` (string)

***

### `debug.printNodeTree`

Prints node name tree structure of passed `niNode` object.

```lua
debug.printNodeTree(root)
```

**Parameters**:

* `root` (niNode): The root node to traverse and print the tree for.

## Examples

!!! example "Example: Print tree structure of `worldRoot`"

	```lua
	-- Print tree structure of worldRoot
	debug.printNodeTree(tes3.game.worldRoot)
	```

Output:

```log
[...]

- WorldObjectRoot
		- activation ambientLight
		- CLONE PlayerSaveGame
			- MRT
			- Bip01
				- Bip01 Pelvis
					- Bip01 Spine
						- Bip01 Spine1
							- Bip01 Spine2
								- Bip01 Neck
									- Bip01 Head
										- Head
											- nil
											- nil
									- ab01node
										- Tri Amulet_Extravagant_1 0
										- Tri Amulet_Extravagant_1 1
										- Tri Amulet_Extravagant_1 2
										- Tri Amulet_Extravagant_1 3

[...]
```


***
