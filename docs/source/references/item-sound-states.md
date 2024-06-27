---
hide:
  - toc
---

# Item Sound States

!!! tip
	These values are available in Lua by their index in the `tes3.itemSoundState` table. For example, `tes3.itemSoundState.down` has a value of `1`.

Index    | Value | Description
-------- | ----- | ------------------------
up       | `0`   | Item picked up. Sound ID is "<item class> Up".
down     | `1`   | Item set down. Sound ID is "<item class> Down".
direct   | `2`   | Unknown usage. Sound ID is "<item class>".
consume  | `3`   | Item eaten or consumed. Sound ID is "Drink" (alchemy) or "Swallow" (ingredient).
