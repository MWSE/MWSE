# pickpocket
<div class="search_terms" style="display: none">pickpocket</div>

<!---
	This file is autogenerated. Do not edit this file manually. Your changes will be ignored.
	More information: https://github.com/MWSE/MWSE/tree/master/docs
-->

This event fires when the player attempts to pickpocket an item stack, and also on closing the pickpocket contents window. When the window is closed, `item` will be `nil`.

```lua
--- @param e pickpocketEventData
local function pickpocketCallback(e)
end
event.register(tes3.event.pickpocket, pickpocketCallback)
```

!!! tip
	An event can be claimed by setting `e.claim` to `true`, or by returning `false` from the callback. Claiming the event prevents any lower priority callbacks from being called.

## Event Data

* `chance` (number): The % chance the pickpocket attempt will be successful.
* `count` (number): *Read-only*. The count of items in the chosen stack.
* `item` ([tes3item](../types/tes3item.md), nil): *Read-only*. The chosen item. `nil` when the window is being closed.
* `itemData` ([tes3itemData](../types/tes3itemData.md), nil): *Read-only*. The chosen item data. `nil` when the window is being closed.
* `mobile` ([tes3mobileActor](../types/tes3mobileActor.md)): *Read-only*. The mobile actor being pickpocketed.
* `reference` ([tes3reference](../types/tes3reference.md)): *Read-only*. The reference of the actor being pickpocketed.


## Related events

[trapDisarm](./trapDisarm.md){ .md-button }

