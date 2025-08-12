return {
	type = "value",
	description = [[If set to `true`, the header portion of all logging messages will be shortened. 
For example, suppose the following log message is written on line `20` of the file `My Mod/Skills/Player/Combat/swords.lua`.
```lua
log("My message")
```
Then the resulting output to MWSE.log would be:
```
-- abbreviateHeader == true:
[My Mod | s/p/c/swords:20  | D] My message
-- abbreviateHeader == false:
[My Mod | skills/player/combat/swords.lua:20  | DEBUG] My message
```
This does not affect how the "body" of log messages are displayed.]],
	valuetype = "boolean",
}
