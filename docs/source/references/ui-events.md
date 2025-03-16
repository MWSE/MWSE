---
hide:
  - toc
---

# UI Events

!!! tip
	These values are available in Lua by their index in the `tes3.uiEvent` table. For example, `tes3.uiEvent.focus` has a value of `"focus"`.

Index                    | Value
------------------------ | ----------------------------
destroy                  | `"destroy"`
focus                    | `"focus"`
help                     | `"help"`
keyEnter                 | `"keyEnter"`
keyPress                 | `"keyPress"`
mouseClick               | `"mouseClick"`
mouseDoubleClick         | `"mouseDoubleClick"`
mouseDown                | `"mouseDown"`
mouseLeave               | `"mouseLeave"`
mouseOver                | `"mouseOver"`
mouseRelease             | `"mouseRelease"`
mouseScrollDown          | `"mouseScrollDown"`
mouseScrollUp            | `"mouseScrollUp"`
mouseStillIdle           | `"mouseStillIdle"`
mouseStillOver           | `"mouseStillOver"`
mouseStillPressed        | `"mouseStillPressed"`
mouseStillPressedOutside | `"mouseStillPressedOutside"`
partScrollBarChanged     | `"PartScrollBar_changed"`
colorChanged             | `"colorChanged"`
valueChanged             | `"valueChanged"`
tabFocus                 | `"tabFocus"`
tabUnfocus               | `"tabUnfocus"`
textCleared              | `"textCleared"`
textUpdated              | `"textUpdated"`
preUpdate                | `"preUpdate"`
unfocus                  | `"unfocus"`
update                   | `"update"`
